use uo;
use os;
use storage;
use datafile;

include ":containers:storageAreas";
include ":playervendor:include/escrow";
include "include/objtype";
include "include/random";

const WFH_ESCROW_VENDOR_NAME := "Warrior for Hire";
const WFH_ESCROW_STORAGE_NAME := "Merchant Escrow";
const WFH_ESCROW_REGISTRY_DATAFILE := "wfhescrowregistry";
const WFH_ESCROW_EXPIRE_SECONDS := 7776000; //90 days

// ReadGameClock() is uptime-since-boot and resets to 0 on every restart --
// fine for the short in-session cooldowns elsewhere in this package, but
// useless for a 90-day expiry that will outlive many restarts. POLCORE().systime
// is real wall-clock (same source pkg/opt/powerhour/powerhour.src uses for its
// own weekly schedule), so that's what the registry's "created" is stamped with.

function WFH_SaveToEscrow(payout_pack, ownerserial, sourceserial)

	var area := FindStorageArea(WFH_ESCROW_STORAGE_NAME);
	if (!area)
		area := CreateStorageArea(WFH_ESCROW_STORAGE_NAME);
	endif
	if (!area)
		return 0;
	endif

	var escrow_name := "WFHEscrow " + CStr(ownerserial) + " " + CStr(ReadGameClock()) + " " + CStr(Random(99999));
	var escrow_root := CreateRootItemInStorageArea(area, escrow_name, UOBJ_BACKPACK);
	if (!escrow_root)
		return 0;
	endif

	var created := POLCORE().systime;

	escrow_root.name := "Warrior for Hire Escrow";
	SetObjProperty(escrow_root, "ownerserial", ownerserial);
	SetObjProperty(escrow_root, "vendorserial", sourceserial);
	SetObjProperty(escrow_root, "vendorname", WFH_ESCROW_VENDOR_NAME);
	SetObjProperty(escrow_root, "created", created);

	if (!MoveItemToContainer(payout_pack, escrow_root))
		DestroyRootItemInStorageArea(area, escrow_name);
		return 0;
	endif

	// Registered here, before the owner-resolution branch below, and
	// unconditionally regardless of how the rest of this function goes --
	// this is the only record WFH_SweepExpiredEscrow can find "every entry
	// across every owner" through (the datafile below is per-owner only), so
	// if this is skipped whenever the owner can't be resolved, exactly the
	// entries the 90-day expiry exists to eventually clean up would never be
	// swept at all.
	var entry_id := CStr(ownerserial) + "-" + CStr(ReadGameClock()) + "-" + CStr(Random(100000));
	WFH_RegisterEscrowEntry(entry_id, ownerserial, escrow_name, created);

	var owner_obj := SystemFindObjectBySerial(CInt(ownerserial), SYSFIND_SEARCH_OFFLINE_MOBILES);
	if (!owner_obj)
		return 1; //items are safely in escrow, and swept in 90 days regardless, even without a per-owner index entry below
	endif

	var owner_name := "Unknown Owner";
	if (owner_obj.name)
		owner_name := owner_obj.name;
	endif

	var df_name := BuildEscrowDatafileName(owner_obj);
	if (!df_name)
		return 1;
	endif

	var df := OpenDataFile(df_name);
	if (!df)
		CreateDataFile(df_name);
		df := OpenDataFile(df_name);
		if (!df)
			return 1;
		endif
	endif

	var vendor_key := CStr(sourceserial);
	var vendor_elem := df.FindElement(vendor_key);
	if (!vendor_elem)
		vendor_elem := df.CreateElement(vendor_key);
	endif
	if (!vendor_elem)
		UnloadDataFile(df_name);
		return 1;
	endif

	var entry := entry_id+"|"+CStr(ownerserial)+"|"+escrow_name+"|"+WFH_ESCROW_VENDOR_NAME+"|"+CStr(sourceserial)+"|"+CStr(created)+"|"+owner_name;

	var entries_str := vendor_elem.GetProp("entries");
	if (entries_str)
		entries_str := entries_str + "^" + entry;
	else
		entries_str := entry;
	endif
	vendor_elem.SetProp("entries", entries_str);

	UnloadDataFile(df_name);

	return 1;

endfunction

// Global index of every live WFH escrow entry (entry_id -> ownerserial,
// created), separate from the per-owner datafiles above. Those are keyed by
// owner, so there's no way to sweep "every WFH escrow entry across every
// owner" without one of these -- this is the only thing WFH_SweepExpiredEscrow
// reads from directly; the actual claim/destroy/index-removal logic below is
// all reused as-is from the functions that already existed.
function WFH_RegisterEscrowEntry(entry_id, ownerserial, escrow_name, created)

	var df := OpenDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
	if (!df)
		CreateDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
		df := OpenDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
		if (!df)
			return 0;
		endif
	endif

	var elem := df.FindElement(entry_id);
	if (!elem)
		elem := df.CreateElement(entry_id);
	endif
	if (!elem)
		UnloadDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
		return 0;
	endif

	elem.SetProp("ownerserial", ownerserial);
	elem.SetProp("escrow_name", escrow_name);
	elem.SetProp("created", created);

	UnloadDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
	return 1;

endfunction

function WFH_UnregisterEscrowEntry(entry_id)

	var df := OpenDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
	if (!df)
		return;
	endif

	df.DeleteElement(entry_id);
	UnloadDataFile(WFH_ESCROW_REGISTRY_DATAFILE);

endfunction

// Called on a slow, indefinite loop by escrowsweep.src (started once at boot
// via start.src). Finds every registry entry older than 90 real days and
// destroys it. Destruction works straight off the registry's own
// escrow_name -- it does NOT depend on a matching per-owner index entry
// existing (WFH_SaveToEscrow registers here unconditionally, even when the
// owner couldn't be resolved and no per-owner index entry was ever written,
// so this has to be able to clean those up too). When the owner *can* be
// resolved, this reuses the existing WFH_DestroyEscrowEntry (same path the
// High Priest's "no, dispose of them" branch already uses) so the per-owner
// index also gets pruned as a bonus; otherwise it destroys the storage-area
// item directly.
function WFH_SweepExpiredEscrow()

	var df := OpenDataFile(WFH_ESCROW_REGISTRY_DATAFILE);
	if (!df)
		return;
	endif

	var now := POLCORE().systime;
	var expired_names := dictionary;
	var expired_owners := dictionary;

	foreach entry_id in (df.Keys())
		var elem := df.FindElement(entry_id);
		if (!elem) continue; endif
		var created := CInt(elem.GetProp("created"));
		if (!created) continue; endif
		if ((now - created) >= WFH_ESCROW_EXPIRE_SECONDS)
			expired_names.insert(entry_id, elem.GetProp("escrow_name"));
			expired_owners.insert(entry_id, CInt(elem.GetProp("ownerserial")));
		endif
	endforeach

	// One open/delete-many/close pass for the registry itself, instead of a
	// separate open+close per expired entry.
	foreach entry_id in (expired_names.keys())
		df.DeleteElement(entry_id);
	endforeach

	UnloadDataFile(WFH_ESCROW_REGISTRY_DATAFILE);

	var area := FindStorageArea(WFH_ESCROW_STORAGE_NAME);

	foreach entry_id in (expired_names.keys())
		var escrow_name := expired_names[entry_id];
		var ownerserial := expired_owners[entry_id];
		var owner_obj := SystemFindObjectBySerial(ownerserial, SYSFIND_SEARCH_OFFLINE_MOBILES);

		if (owner_obj and escrow_name)
			// WFH_DestroyEscrowContents only (not the full WFH_DestroyEscrowEntry) --
			// the registry side is already handled by the batched delete pass above,
			// so calling the full version here would reopen/close the registry
			// datafile once per entry, right back into the same I/O pattern this
			// rewrite exists to avoid.
			WFH_DestroyEscrowContents(owner_obj, {entry_id, CStr(ownerserial), escrow_name});
		elseif (area and escrow_name)
			var escrowroot := FindRootItemInStorageArea(area, escrow_name);
			if (escrowroot)
				foreach item in EnumerateItemsInContainer(escrowroot)
					DestroyItem(item);
				endforeach
				DestroyItem(escrowroot);
				DestroyRootItemInStorageArea(area, escrow_name);
			endif
		endif
	endforeach

endfunction

function WFH_LoadEscrowEntriesForOwner(ownerobj)

	var entries := {};
	var df_name := BuildEscrowDatafileName(ownerobj);
	if (!df_name)
		return entries;
	endif

	var df := OpenDataFile(df_name);
	if (!df)
		return entries;
	endif

	foreach vendor_key in (df.Keys())
		var vendor_elem := df.FindElement(vendor_key);
		if (!vendor_elem) continue; endif
		var entries_str := vendor_elem.GetProp("entries");
		if (!entries_str) continue; endif

		var parts := SplitWords(entries_str, "^");
		foreach part in parts
			if (part)
				var fields := SplitWords(part, "|");
				if (len(fields) >= 7 and fields[4] == WFH_ESCROW_VENDOR_NAME)
					entries.append({fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], fields[7]});
				endif
			endif
		endforeach
	endforeach

	UnloadDataFile(df_name);
	return entries;

endfunction

function WFH_ClaimEscrowEntry(who, entry)

	var area := FindStorageArea(WFH_ESCROW_STORAGE_NAME);
	if (!area)
		return 0;
	endif

	var escrow_name := entry[3];
	var escrowroot := FindRootItemInStorageArea(area, escrow_name);
	if (!escrowroot)
		WFH_RemoveEscrowEntryById(who, entry[1]);
		return 0;
	endif

	var moved := 0;
	foreach item in EnumerateItemsInContainer(escrowroot)
		if (item.container == escrowroot)
			if (MoveItemToContainer(item, who.backpack))
				moved := moved + 1;
			endif
		endif
	endforeach

	var remaining := 0;
	foreach rem in EnumerateItemsInContainer(escrowroot)
		if (rem.container == escrowroot)
			remaining := remaining + 1;
		endif
	endforeach

	if (!remaining)
		DestroyItem(escrowroot);
		DestroyRootItemInStorageArea(area, escrow_name);
		WFH_RemoveEscrowEntryById(who, entry[1]);
		WFH_UnregisterEscrowEntry(entry[1]);
	endif

	return moved;

endfunction

// Destroys the escrow item + per-owner index entry only -- does NOT touch
// the global registry. Split out from WFH_DestroyEscrowEntry so
// WFH_SweepExpiredEscrow (which already removed the registry row itself, in
// a single batched pass across every expired entry) can reuse this without
// each call reopening/closing the registry datafile a second time.
function WFH_DestroyEscrowContents(who, entry)

	var area := FindStorageArea(WFH_ESCROW_STORAGE_NAME);
	if (area)
		var escrow_name := entry[3];
		var escrowroot := FindRootItemInStorageArea(area, escrow_name);
		if (escrowroot)
			foreach item in EnumerateItemsInContainer(escrowroot)
				DestroyItem(item);
			endforeach
			DestroyItem(escrowroot);
			DestroyRootItemInStorageArea(area, escrow_name);
		endif
	endif

	WFH_RemoveEscrowEntryById(who, entry[1]);

endfunction

function WFH_DestroyEscrowEntry(who, entry)

	WFH_DestroyEscrowContents(who, entry);
	WFH_UnregisterEscrowEntry(entry[1]);

endfunction

function WFH_RemoveEscrowEntryById(who, entry_id)

	var df_name := BuildEscrowDatafileName(who);
	if (!df_name) return; endif

	var df := OpenDataFile(df_name);
	if (!df) return; endif

	foreach vendor_key in (df.Keys())
		var vendor_elem := df.FindElement(vendor_key);
		if (!vendor_elem) continue; endif
		var entries_str := vendor_elem.GetProp("entries");
		if (!entries_str) continue; endif

		var parts := SplitWords(entries_str, "^");
		var new_parts := {};
		var found := 0;
		foreach part in parts
			if (part)
				var fields := SplitWords(part, "|");
				if (len(fields) >= 1 and fields[1] == entry_id)
					found := 1;
				else
					new_parts.append(part);
				endif
			endif
		endforeach

		if (found)
			if (len(new_parts) > 0)
				var encoded := "";
				foreach p in new_parts
					if (encoded)
						encoded := encoded + "^";
					endif
					encoded := encoded + p;
				endforeach
				vendor_elem.SetProp("entries", encoded);
			else
				df.DeleteElement(vendor_key);
			endif
		endif
	endforeach

	UnloadDataFile(df_name);

endfunction
