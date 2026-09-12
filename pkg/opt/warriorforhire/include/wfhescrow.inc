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

	escrow_root.name := "Warrior for Hire Escrow";
	SetObjProperty(escrow_root, "ownerserial", ownerserial);
	SetObjProperty(escrow_root, "vendorserial", sourceserial);
	SetObjProperty(escrow_root, "vendorname", WFH_ESCROW_VENDOR_NAME);
	SetObjProperty(escrow_root, "created", ReadGameClock());

	if (!MoveItemToContainer(payout_pack, escrow_root))
		DestroyRootItemInStorageArea(area, escrow_name);
		return 0;
	endif

	var owner_obj := SystemFindObjectBySerial(CInt(ownerserial), SYSFIND_SEARCH_OFFLINE_MOBILES);
	if (!owner_obj)
		return 1; //items are safely in escrow even if we can't index them by name below
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

	var entry_id := CStr(ownerserial) + "-" + CStr(ReadGameClock()) + "-" + CStr(Random(100000));
	var entry := entry_id+"|"+CStr(ownerserial)+"|"+escrow_name+"|"+WFH_ESCROW_VENDOR_NAME+"|"+CStr(sourceserial)+"|"+CStr(ReadGameClock())+"|"+owner_name;

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
	endif

	return moved;

endfunction

function WFH_DestroyEscrowEntry(who, entry)

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
