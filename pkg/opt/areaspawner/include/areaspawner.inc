use os;
use uo;
use cfgfile;
use datafile;
use util;
use vitals;
use attributes;
use basic;

include "include/constants/propids";
include "include/constants/cfgfiles";
include "include/constants/usescriptids";	// USESCRIPTID_TRAPPED_CONTAINER - chest kind traps, matches pkg/opt/spawnpoint
include "include/constants/locations";	// DEFAULT_LOCATION_ITEM_CREATION_X/Y/Z/R - chest kind stages offsite before moving into the box, matches pkg/opt/spawnpoint's CreateSpawnPointContainer
include "include/all";	// KillMobile
include "include/attributes";	// GetStrength/GetDexterity/GetIntelligence/GetMaxHp/GetMaxStamina/GetMaxMana/SetBaseStrength/SetBaseDexterity/SetBaseIntelligence/VITALID_* - custom npc stat capture/apply
include "include/starteqp";	// MakeLoot - chest kind loot rolling, matches pkg/opt/spawnpoint's CreateSpawnPointContainer
include ":areas:include/areapolicy";	// pulls in :datafile:datafile for DFOpenDataFile/DFFindElement/DFGetProp

const AREASPAWNER_CFG				:= ":areaspawner:areagroups";
const AREASPAWNER_CHESTS_CFG			:= ":areaspawner:areachests";
// Event chest spawners get their own curated catalog, separate from the
// regular one - starts as a plain copy (see config/eventareachests.cfg) but
// is free to diverge over time (richer/rarer event-only chest types, etc.)
// without touching what regular Chest spawners offer.
const AREASPAWNER_EVENT_CHESTS_CFG		:= ":areaspawner:eventareachests";

const AREASPAWNER_ENTRY_MIN_CAP		:= 1;
const AREASPAWNER_ENTRY_MAX_CAP		:= 200;
const AREASPAWNER_PLACEMENT_MAX_TRIES		:= 20;
const AREASPAWNER_PLACEMENT_RETRY_DELAY	:= 100;	// ms between placement tries, keeps a bad box from hammering the shard

// A mixed-terrain box (land + water) can legitimately roll all-water on a
// given 20-try cycle by chance without meaning the entry is actually stuck -
// this is how many CONSECUTIVE full-cycle failures in a row before an entry
// pauses itself rather than retrying forever. 10 cycles * 20 tries = 200
// random samples of the box is a fair shot before assuming it's not working.
const AREASPAWNER_ENTRY_FAIL_THRESHOLD		:= 10;

const AREASPAWNER_HOME_X			:= 6338;
const AREASPAWNER_HOME_Y			:= 1581;

const AREASPAWNER_DEBUG			:= 0;	// 1 = print debug to console, 0 = silent

// Coarse safety-net sweep interval (seconds) - not a poll loop, just a rare
// catch-all in case a kick chain died or drifted, mirroring spawnpoint's
// allspawner.src TimeBetweenAllSpawn (default there is 5 hours).
const AREASPAWNER_SAFETYNET_INTERVAL		:= 4 * 3600;

function AreaSpawnerDebug(msg)
	if( AREASPAWNER_DEBUG )
		Print("[areaspawner] " + msg);
	endif
	return 1;
endfunction


function AreaSpawnerRealms()
	return array{ "britannia", "britannia_alt", "malas", "tokuno", "ilshenar", "termur" };
endfunction


function AreaSpawnerKinds()
	return array{ "area", "individual", "customnpc", "item", "chest" };
endfunction


// Every (kind, regular/event) combination gets its own datafile per realm, so
// e.g. wiping all Item spawns is just deleting that kind's datafiles across
// realms - it never shares storage with Area/Chest/etc. "area" keeps its
// original unsuffixed name (no "_area" suffix) so spawners already saved
// before this scheme existed - like live production data - aren't orphaned.
function AreaSpawnerKindPrefix(kind, is_event := 0)
	var base := is_event ? "eventareaspawner" : "areaspawner";
	if( kind == "area" )
		return base;
	endif
	return base+"_"+kind;
endfunction


// Derives which of AreaSpawnerKinds() a datafile prefix belongs to - "area"
// has no suffix (legacy, pre-dates the kind split) so it's the fallback; every
// other kind is "areaspawner_"+kind or "eventareaspawner_"+kind per
// AreaSpawnerKindPrefix. Used where a function only has realm/name/prefix in
// hand (e.g. areaspawnerentrytick) but needs to know which kind's placement/
// box rules apply, without a separate record field to keep in sync.
function AreaSpawnerKindFromPrefix(prefix)
	var kinds := AreaSpawnerKinds();
	foreach kind in kinds
		if( kind == "area" )
			continue;
		endif
		if( prefix == "areaspawner_"+kind || prefix == "eventareaspawner_"+kind )
			return kind;
		endif
	endforeach
	return "area";
endfunction


// True for any "eventareaspawner"/"eventareaspawner_"+kind prefix - used
// where a function only has prefix in hand but needs to pick between a
// regular-side and event-side resource (right now: which chest catalog to
// read, see AREASPAWNER_EVENT_CHESTS_CFG).
function AreaSpawnerIsEventPrefix(prefix)
	return (SubStr(prefix, 1, 5) == "event");
endfunction


// "area" and "chest" spawn across a roamed box - a degenerate (single-point)
// box for them means "never configured." "individual"/"item"/"customnpc" are
// single-XY by design (the box fields just hold that one point), so the same
// box shape is normal, not a misconfiguration.
function AreaSpawnerKindUsesBox(kind)
	return (kind == "area" || kind == "chest");
endfunction


function AreaSpawnerRealmBounds(realm)
	var bounds := struct;
	case( Lower(realm) )
		"britannia":		bounds := struct{"minx":=0,"maxx":=7167,"miny":=0,"maxy":=4094};
		"britannia_alt":	bounds := struct{"minx":=0,"maxx":=7167,"miny":=0,"maxy":=4094};
		"malas":		bounds := struct{"minx":=0,"maxx":=2560,"miny":=0,"maxy":=2048};
		"tokuno":		bounds := struct{"minx":=0,"maxx":=1448,"miny":=0,"maxy":=1448};
		"ilshenar":		bounds := struct{"minx":=0,"maxx":=2304,"miny":=0,"maxy":=1600};
		"termur":		bounds := struct{"minx":=0,"maxx":=2304,"miny":=0,"maxy":=1600};
		default:		bounds := struct{"minx":=0,"maxx":=0,"miny":=0,"maxy":=0};
	endcase
	return bounds;
endfunction


// prefix selects which datafile namespace to use - "areaspawner" (default, the
// regular GM spawner list) or "eventareaspawner" (isolated event spawners, kept
// out of the regular list so admins don't have to scroll past event content).
function GetAreaSpawnerDatafileName(realm, prefix := "areaspawner")
	return ":areaspawner:"+prefix+"_"+Lower(realm);
endfunction


function OpenAreaSpawnerDataFile(realm, create := DF_CREATE, prefix := "areaspawner")
	return DFOpenDataFile( GetAreaSpawnerDatafileName(realm, prefix), create );
endfunction


function GetAreaSpawnerNames(realm, prefix := "areaspawner")
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE, prefix);
	if( !df || df == error )
		return array{};
	endif

	var names := DFGetElemNames(df);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );

	if( names == error )
		return array{};
	endif

	return names;
endfunction


function NewAreaSpawnerRecord(who)
	var record := struct;
	record.+enabled	:= 0;
	record.+disabled_reason	:= "";	// set when areaspawnerentrytick auto-disables (e.g. zero-sized box), cleared on any manual Enabled toggle
	record.+allow_water	:= 0;	// 0 (default) = land-only via GetStandingHeight, ignoring each template's MoveMode - 1 = movemode-aware via GetStandingCoordinates, so a swim-capable template can land on water tiles in the box
	record.+area_label	:= "";	// cosmetic only - last area.cfg entry picked, box below is authoritative
	record.+minx		:= 0;
	record.+maxx		:= 0;
	record.+miny		:= 0;
	record.+maxy		:= 0;
	record.+entries	:= array{};
	record.+next_entry_id	:= 1;
	record.+updated_at	:= ReadGameClock();
	record.+updated_by	:= who ? who.name : "";
	return record;
endfunction


function CreateAreaSpawner(realm, name, who, prefix := "areaspawner")
	var df := OpenAreaSpawnerDataFile(realm, DF_CREATE, prefix);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_NO_CREATE);
	if( elem && elem != error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
		return 0;	// already exists
	endif

	elem := DFFindElement(df, name, DF_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
		return 0;
	endif

	elem.SetProp( "Data", NewAreaSpawnerRecord(who) );
	UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
	AreaSpawnerDebug("created '"+name+"' ("+realm+") ["+prefix+"]"+(who ? " by "+who.name : ""));
	return 1;
endfunction


function LoadAreaSpawner(realm, name, prefix := "areaspawner")
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE, prefix);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_NO_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
		return 0;
	endif

	var record := DFGetProp(elem, "Data", DF_NO_CREATE);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );

	if( record == error )
		return 0;
	endif

	return record;
endfunction


function SaveAreaSpawner(realm, name, record, who, prefix := "areaspawner")
	var df := OpenAreaSpawnerDataFile(realm, DF_CREATE, prefix);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
		return 0;
	endif

	record.updated_at := ReadGameClock();
	if( who )
		record.updated_by := who.name;
	endif

	elem.SetProp( "Data", record );
	UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
	return 1;
endfunction


function DeleteAreaSpawnerRecord(realm, name, prefix := "areaspawner")
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE, prefix);
	if( !df || df == error )
		return 0;
	endif

	df.DeleteElement(name);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm, prefix) );
	return 1;
endfunction


function ClampEntryCount(value)
	value := CInt(value);
	if( value < AREASPAWNER_ENTRY_MIN_CAP )
		value := AREASPAWNER_ENTRY_MIN_CAP;
	elseif( value > AREASPAWNER_ENTRY_MAX_CAP )
		value := AREASPAWNER_ENTRY_MAX_CAP;
	endif
	return value;
endfunction


// overrides is only meaningful for source=="customnpc" - the full field set
// is NewCustomNpcOverrides()'s shape (stats/appearance/protections/etc),
// captured/edited via the Custom NPC editor and applied on top of the
// scaffold template at placement time. Every other kind leaves it an empty
// struct, unused.
function AddAreaSpawnerEntry(byref record, source, ref, min_count, max_count, frequency, overrides := 0)
	min_count := ClampEntryCount(min_count);
	max_count := ClampEntryCount(max_count);
	if( max_count < min_count )
		max_count := min_count;
	endif

	frequency := CInt(frequency);
	if( frequency < 1 )
		frequency := 1;
	endif

	var entry := struct;
	entry.+id		:= record.next_entry_id;
	entry.+source		:= Lower(source);
	entry.+ref		:= ref;
	entry.+min		:= min_count;
	entry.+max		:= max_count;
	entry.+frequency	:= frequency;
	entry.+target		:= min_count + RandomInt(max_count-min_count+1);
	entry.+serials		:= array{};
	entry.+next_spawn_time	:= 0;
	entry.+last_fail_time	:= 0;
	entry.+refilling	:= 0;	// 1 while a areaspawnerentrytick chain is already in flight for this entry
	entry.+config_error	:= "";	// set by TickAreaSpawnerEntry/FullRespawn when the problem won't fix itself by retrying (bad template, etc.)
	entry.+fail_cycles	:= 0;	// consecutive full-cycle placement failures - resets to 0 on any success
	entry.+paused_reason	:= "";	// set once fail_cycles crosses AREASPAWNER_ENTRY_FAIL_THRESHOLD - pauses just this entry, not the whole spawner
	entry.+overrides	:= overrides ? overrides : struct;	// customnpc stat overrides, see comment above

	record.entries.append(entry);
	record.next_entry_id := record.next_entry_id + 1;
	return record;
endfunction


function RemoveAreaSpawnerEntry(byref record, entry_id)
	var kept := array{};
	var current_entries := record.entries;
	foreach entry in current_entries
		if( entry.id != entry_id )
			kept.append(entry);
		endif
	endforeach
	record.entries := kept;
	return record;
endfunction


function FindAreaSpawnerEntryIndex(record, entry_id)
	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		if( record.entries[i].id == entry_id )
			return i;
		endif
	endfor
	return 0;
endfunction


function UpdateAreaSpawnerEntry(byref record, entry_id, source, ref, min_count, max_count, frequency, overrides := 0)
	var idx := FindAreaSpawnerEntryIndex(record, entry_id);
	if( !idx )
		return record;
	endif

	min_count := ClampEntryCount(min_count);
	max_count := ClampEntryCount(max_count);
	if( max_count < min_count )
		max_count := min_count;
	endif

	frequency := CInt(frequency);
	if( frequency < 1 )
		frequency := 1;
	endif

	var entry := record.entries[idx];
	entry.source	:= Lower(source);
	entry.ref	:= ref;
	entry.min	:= min_count;
	entry.max	:= max_count;
	entry.frequency	:= frequency;
	if( overrides )
		entry.overrides := overrides;
	endif
	if( entry.target < min_count )
		entry.target := min_count;
	elseif( entry.target > max_count )
		entry.target := max_count;
	endif

	// an explicit edit is the admin signaling "I changed something, try
	// again fresh" - give it a full new run at the fail threshold instead
	// of counting against whatever it had accumulated before the edit.
	// Also clears refilling - same reasoning as AreaSpawnerResetEntry-
	// Cooldowns: if a prior kick chain died mid-run and left this stuck at
	// 1, nothing else would ever recover it short of the 4-hour safety net,
	// and an explicit "I edited and saved this entry" is exactly the moment
	// a GM expects it to actually start trying again.
	entry.fail_cycles := 0;
	entry.paused_reason := "";
	entry.refilling := 0;

	record.entries[idx] := entry;
	return record;
endfunction


function DespawnAreaSpawnerEntryByIndex(byref record, idx)
	var serials := record.entries[idx].serials;
	foreach serial in serials
		var obj := SystemFindObjectBySerial(serial);
		if( obj )
			AreaSpawnerDespawnObject(obj);
		endif
	endforeach
	record.entries[idx].serials := array{};
	return record;
endfunction


function RemoveAreaSpawnerEntryAndDespawn(byref record, entry_id)
	var idx := FindAreaSpawnerEntryIndex(record, entry_id);
	if( idx )
		record := DespawnAreaSpawnerEntryByIndex(record, idx);
	endif
	return RemoveAreaSpawnerEntry(record, entry_id);
endfunction


// The record's minx/maxx/miny/maxy are always authoritative - whether they were
// typed in directly or filled in once via LookupAreaCfgBox() below, there's no
// ongoing "cfg vs coords" distinction to resolve at tick time.
function ResolveAreaSpawnerBox(realm, record)
	var box := struct;
	box.+minx := CInt(record.minx);
	box.+maxx := CInt(record.maxx);
	box.+miny := CInt(record.miny);
	box.+maxy := CInt(record.maxy);
	return box;
endfunction


// One-time lookup used by the "Pick Area" gump flow to fill minx/maxx/miny/maxy
// from a chosen areas.cfg entry. Returns 0 if the realm/id can't be resolved.
function LookupAreaCfgBox(realm, area_id)
	var lines := GetRealmAreaLines(realm);
	if( lines == error )
		return 0;
	endif

	foreach line in lines
		var info := ParseAreaLine(line);
		if( info == error )
			continue;
		endif

		if( Lower(info.area_id) == Lower(area_id) )
			var box := struct;
			box.+minx := info.min_x;
			box.+maxx := info.max_x;
			box.+miny := info.min_y;
			box.+maxy := info.max_y;
			box.+label := info.name;
			return box;
		endif
	endforeach

	return 0;
endfunction


function ClampBoxToRealm(realm, box)
	var bounds := AreaSpawnerRealmBounds(realm);
	var clamped := struct;
	clamped.+minx := box.minx;
	clamped.+maxx := box.maxx;
	clamped.+miny := box.miny;
	clamped.+maxy := box.maxy;

	if( clamped.minx < bounds.minx )
		clamped.minx := bounds.minx;
	endif
	if( clamped.maxx > bounds.maxx )
		clamped.maxx := bounds.maxx;
	endif
	if( clamped.miny < bounds.miny )
		clamped.miny := bounds.miny;
	endif
	if( clamped.maxy > bounds.maxy )
		clamped.maxy := bounds.maxy;
	endif
	if( clamped.minx > clamped.maxx )
		clamped.minx := clamped.maxx;
	endif
	if( clamped.miny > clamped.maxy )
		clamped.miny := clamped.maxy;
	endif

	return clamped;
endfunction


function AreaSpawnerPopulationSummary(record)
	var alive := 0;
	var target_sum := 0;
	var entries := record.entries;
	foreach entry in entries
		alive := alive + entry.serials.size();
		target_sum := target_sum + entry.target;
	endforeach
	return CStr(alive)+"/"+CStr(target_sum);
endfunction


// A paused entry doesn't disable the whole record (see AREASPAWNER_ENTRY_FAIL_
// THRESHOLD), so it needs its own visibility path - this is that path, used
// by both the list gump and the staff login/`.areaspawnerauto` notice. Only
// reports the first one found; if more than one entry is paused, the admin
// sees the rest by opening Entries.
function AreaSpawnerFindPausedReason(record)
	var entries := record.entries;
	foreach entry in entries
		if( entry.paused_reason )
			return "entry #"+CStr(entry.id)+" ("+entry.source+":"+CStr(entry.ref)+") - "+entry.paused_reason;
		endif
	endforeach
	return "";
endfunction


// prefix picks which chest catalog a "chestgroup" entry resolves against
// (regular vs event, see AREASPAWNER_EVENT_CHESTS_CFG) - unused for every
// other source, default keeps existing regular-side callers unchanged.
function ResolveAreaSpawnerTemplate(entry, prefix := "areaspawner")
	if( entry.source == "group" )
		var cfg := ReadConfigFile(AREASPAWNER_CFG);
		if( !cfg )
			return 0;
		endif

		var elem := cfg[CInt(entry.ref)];
		if( !elem )
			return 0;
		endif

		var possibles := GetConfigStringArray(elem, "spawn");
		if( !possibles || !possibles.size() )
			return 0;
		endif

		return possibles[RandomInt(possibles.size())+1];
	endif

	// Chest kind entries reference a curated areachests.cfg id (same shape as
	// "group" above, just a different cfg/key) instead of a raw item template -
	// keeps chest spawns limited to the curated, known-good container list
	// instead of any arbitrary itemdesc entry. Event-side spawners read from
	// their own separate catalog.
	if( entry.source == "chestgroup" )
		var chestcfgname := AreaSpawnerIsEventPrefix(prefix) ? AREASPAWNER_EVENT_CHESTS_CFG : AREASPAWNER_CHESTS_CFG;
		var chestcfg := ReadConfigFile(chestcfgname);
		if( !chestcfg )
			return 0;
		endif

		var chestelem := chestcfg[CInt(entry.ref)];
		if( !chestelem )
			return 0;
		endif

		var chestpossibles := GetConfigStringArray(chestelem, "item");
		if( !chestpossibles || !chestpossibles.size() )
			return 0;
		endif

		return chestpossibles[RandomInt(chestpossibles.size())+1];
	endif

	return CStr(entry.ref);
endfunction


// Only meaningful for npc/group sources - PROPID_GLOBAL_SPAWNPOINTS_FORBIDDEN_
// TEMPLATES is spawnpoint's forbidden NPC template list, keyed by npc template
// name. Checking it against an item/chest template would just be a coincidental
// name collision, never an intentional exclusion, so item/chestgroup sources
// skip this entirely (see call sites in TickAreaSpawnerEntry/FullRespawn).
function IsAreaSpawnerTemplateForbidden(template)
	var forbidden := GetGlobalProperty( PROPID_GLOBAL_SPAWNPOINTS_FORBIDDEN_TEMPLATES );
	if( !forbidden )
		return 0;
	endif
	return (Lower(template) in forbidden);
endfunction


// Resolves a template string to an itemdesc objtype - Cint() first (a numeric
// objtype typed directly), falling back to GetObjtypeByName() (a template
// name), same idiom pkg/opt/spawnpoint's IsValidTemplate() Container branch
// uses since this repo has no dedicated FindItemDesc/ItemDescExists helper.
// Returns 0 if neither resolves, or no itemdesc entry exists for the result.
function ResolveItemDescObjtype(template)
	var objtype := Cint(template);
	if( !objtype )
		objtype := GetObjtypeByName(CStr(template));
	endif
	if( !objtype )
		return 0;
	endif

	var cfg := ReadConfigFile(CFGFILE_ITEMS_ALL);
	if( !cfg || !cfg[objtype] )
		return 0;
	endif

	return objtype;
endfunction


function TagAreaSpawnerObject(obj, realm, name, entry_id, prefix := "areaspawner")
	var info := struct;
	info.+name	:= name;
	info.+realm	:= realm;
	info.+entry_id	:= entry_id;
	info.+prefix	:= prefix;
	SetObjProperty( obj, PROPID_MOBILE_AREASPAWNER, info );
	SetObjProperty( obj, PROPID_MOBILE_SELF_SERIAL, obj.serial );
	return 1;
endfunction


// obj can be an NPC (area/individual/customnpc entries) or an item (item/
// chest entries) - despawn means different engine calls for each, so this is
// the one place that has to know both, instead of every call site branching.
function AreaSpawnerDespawnObject(obj)
	if( obj.isa(POLCLASS_NPC) )
		KillMobile(obj, 1);
	else
		DestroyItem(obj);
	endif
	return 1;
endfunction


// Creates the critter once, then retries placement inside the box with a small
// delay between attempts so a bad box (mostly water, packed city) can't hammer
// the shard with an unthrottled retry loop.
//
// CreateNpcFromTemplate's home x/y/realm here are just a scratch staging spot -
// the critter gets relocated to the real target realm/box below via
// MoveObjectToLocation. AREASPAWNER_HOME_X/Y are britannia_alt coordinates
// (same convention as pkg/opt/spawnpoint/checkpoint.src), so the creation realm
// must always be "britannia_alt" regardless of which realm this spawner targets
// - passing the target realm here made creation fail outright for any realm
// whose map is smaller than britannia_alt (e.g. tokuno).
// Returns struct{critter, config_error}. config_error is only ever set for
// a problem that will NEVER resolve itself by retrying (right now: a
// template name that doesn't exist) - callers use that to auto-disable the
// spawner instead of hammering the same failure every tick forever. A plain
// "couldn't find room" (critter=0, config_error="") is treated as transient.
//
// allow_water gates which placement check runs (this is the record's
// allow_water toggle, not per-template) - default 0 uses plain GetWorldHeight/
// GetStandingHeight (land-only, ignores the template's MoveMode entirely, same
// as spawnpoint), so a mixed-terrain box never accidentally drops something in
// open water just because the template happens to be swim-capable. Only when
// an admin explicitly opts a spawner in via allow_water=1 does this switch to
// GetStandingCoordinates(...,critter.movemode,...), which is movemode-aware
// and can land a swim-capable template on water tiles.
function TryPlaceAreaSpawnerCritter(realm, box, template, allow_water := 0)
	var critter := CreateNpcFromTemplate( template, AREASPAWNER_HOME_X, AREASPAWNER_HOME_Y, 0, 0, "britannia_alt" );
	if( !critter )
		AreaSpawnerDebug("CreateNpcFromTemplate FAILED for template '"+template+"' - bad/missing npc template?");
		return struct{ critter := 0, config_error := "npc template '"+template+"' does not exist" };
	endif
	critter.hidden := 1;

	var tries := 0;
	var placed := 0;
	var x;
	var y;

	// breaks down *why* attempts failed, since "placement FAILED" alone
	// doesn't say whether it's void/blocked or just rejected by the final
	// move - GetStandingCoordinates(...,critter.movemode,...) is movemode-
	// aware (unlike the old GetStandingHeight land-only check), so a
	// swim-capable template's water tiles come back as valid candidates now.
	var fail_no_standing := 0;
	var fail_multi := 0;
	var fail_moveobject := 0;

	while( tries < AREASPAWNER_PLACEMENT_MAX_TRIES && !placed )
		tries := tries + 1;
		x := box.minx + RandomInt(box.maxx - box.minx + 1);
		y := box.miny + RandomInt(box.maxy - box.miny + 1);

		if( allow_water )
			// radius 0 = just this exact tile, tested against this template's
			// own MoveMode (e.g. "LS" for a walrus) instead of assuming land-only
			var candidates := GetStandingCoordinates(x, y, 0, -128, 127, realm, critter.movemode, 0);
			if( candidates && candidates.size() )
				var spot := candidates[1];
				// GetStandingCoordinates doesn't filter out house/boat footprints
				// on its own (pkg/multis/house/walkOn.src needs its own separate
				// ban-tile filter on top of it for the same reason) - so still
				// cross-check .multi here, same guard the land-only path has,
				// just applied to the movemode-valid spot
				var standing := GetStandingHeight(spot.x, spot.y, spot.z, realm);
				if( standing && standing.multi )
					fail_multi := fail_multi + 1;
				elseif( MoveObjectToLocation( critter, spot.x, spot.y, spot.z, realm, MOVEOBJECT_FORCELOCATION ) )
					placed := 1;
					x := spot.x;
					y := spot.y;
				else
					fail_moveobject := fail_moveobject + 1;
				endif
			else
				fail_no_standing := fail_no_standing + 1;
			endif
		else
			// land-only, same check spawnpoint uses - never lands anything on
			// open water regardless of what the template's MoveMode allows
			var z := CInt( GetWorldHeight(x, y, realm) );
			var standing := GetStandingHeight(x, y, z, realm);
			if( !standing )
				fail_no_standing := fail_no_standing + 1;
			elseif( standing.multi )
				fail_multi := fail_multi + 1;
			elseif( MoveObjectToLocation( critter, x, y, z, realm, MOVEOBJECT_FORCELOCATION ) )
				placed := 1;
			else
				fail_moveobject := fail_moveobject + 1;
			endif
		endif

		if( !placed )
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endif
	endwhile

	if( !placed )
		AreaSpawnerDebug("placement FAILED for template '"+template+"' after "+CStr(tries)+" tries, realm="+realm+" - MoveMode='"+CStr(critter.movemode)+"', allow_water="+CStr(allow_water)+", no_standing="+CStr(fail_no_standing)+" multi_blocked="+CStr(fail_multi)+" moveobject_rejected="+CStr(fail_moveobject));
		KillMobile(critter, 1);
		return struct{ critter := 0, config_error := "" };
	endif

	AreaSpawnerDebug("placed '"+template+"' at "+CStr(x)+","+CStr(y)+" ("+realm+") after "+CStr(tries)+" tries");
	RestartScript(critter);
	critter.hidden := 0;
	return struct{ critter := critter, config_error := "" };
endfunction


// Item/chest analog of TryPlaceAreaSpawnerCritter - same retry-with-delay
// shape, but items never swim, so this always uses the land-only check
// regardless of the spawner's allow_water toggle (that toggle only means
// anything for a template with a MoveMode, which items don't have). Returns
// struct{item, config_error}, same contract as the critter version.
function TryPlaceAreaSpawnerItem(realm, box, template)
	var objtype := ResolveItemDescObjtype(template);
	if( !objtype )
		AreaSpawnerDebug("item template '"+template+"' does not resolve to a known itemdesc entry");
		return struct{ item := 0, config_error := "item template '"+template+"' does not exist" };
	endif

	var tries := 0;
	var placed := 0;
	var x;
	var y;
	var z;
	var item := 0;

	while( tries < AREASPAWNER_PLACEMENT_MAX_TRIES && !placed )
		tries := tries + 1;
		x := box.minx + RandomInt(box.maxx - box.minx + 1);
		y := box.miny + RandomInt(box.maxy - box.miny + 1);
		z := CInt( GetWorldHeight(x, y, realm) );

		var standing := GetStandingHeight(x, y, z, realm);
		if( standing && !standing.multi )
			item := CreateItemAtLocation(x, y, z, objtype, 1, realm);
			if( item )
				placed := 1;
			endif
		endif

		if( !placed )
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endif
	endwhile

	if( !placed )
		AreaSpawnerDebug("item placement FAILED for template '"+template+"' after "+CStr(tries)+" tries, realm="+realm);
		return struct{ item := 0, config_error := "" };
	endif

	AreaSpawnerDebug("placed item '"+template+"' at "+CStr(x)+","+CStr(y)+" ("+realm+") after "+CStr(tries)+" tries");
	return struct{ item := item, config_error := "" };
endfunction


// Chest kind's placement - a chest isn't just a plain container like
// TryPlaceAreaSpawnerItem places: it needs rolled loot, a lock difficulty,
// and a trap. This ports pkg/opt/spawnpoint's CreateSpawnPointContainer
// (checkpoint.src) almost verbatim, including staging: the chest is created
// hidden at DEFAULT_LOCATION_ITEM_CREATION_X/Y/Z/R (same offsite spot
// dressme.src/playermerchant.src etc. use for scratch creation) and fully
// loaded with loot/lock/trap there, BEFORE it ever appears in the box - so
// there's no window where a real, findable chest sits unlocked/untrapped/
// empty. Only once it's fully set up does it get moved to its real spot.
// template resolves the CHEST'S GRAPHIC only (which physical chest shape to
// use, from the curated areachests.cfg) - the loot inside is independent of
// which graphic was picked, exactly like spawnpoint (a "Container"
// spawnpoint's pt_data[2] objtype vs its separate pt_data[5]==300 loot-roll
// gate).
function TryPlaceAreaSpawnerChest(realm, box, template)
	var objtype := ResolveItemDescObjtype(template);
	if( !objtype )
		AreaSpawnerDebug("chest template '"+template+"' does not resolve to a known itemdesc entry");
		return struct{ item := 0, config_error := "chest template '"+template+"' does not exist" };
	endif

	var chest := CreateItemAtLocation( DEFAULT_LOCATION_ITEM_CREATION_X, DEFAULT_LOCATION_ITEM_CREATION_Y, DEFAULT_LOCATION_ITEM_CREATION_Z, objtype, 1, DEFAULT_LOCATION_ITEM_CREATION_R );
	if( !chest )
		AreaSpawnerDebug("CreateItemAtLocation FAILED for chest template '"+template+"' - bad/missing item template?");
		return struct{ item := 0, config_error := "chest template '"+template+"' does not exist" };
	endif
	if( !chest.IsA(POLCLASS_CONTAINER) )
		// a curated areachests.cfg entry should never hit this - guards
		// against a bad hand-edit to that file
		AreaSpawnerDebug("chest template '"+template+"' resolved to objtype "+CStr(objtype)+" but is not a container");
		DestroyItem(chest);
		return struct{ item := 0, config_error := "chest template '"+template+"' is not a container" };
	endif
	chest.hidden := 1;

	SetObjProperty( chest, PROPID_CHEST_SPAWNPOINT_CHEST, 1 );

	// Weighted loot group roll (301-307) out of 1000, same distribution as
	// checkpoint.src: 307:2% 306:10% 305:12% 304:15% 303:18% 302:21% 301:22%
	var roll := RandomInt(1000);
	var lootgroup;
	if( roll < 20 )
		lootgroup := 307;
	elseif( roll < 120 )
		lootgroup := 306;
	elseif( roll < 240 )
		lootgroup := 305;
	elseif( roll < 390 )
		lootgroup := 304;
	elseif( roll < 570 )
		lootgroup := 303;
	elseif( roll < 780 )
		lootgroup := 302;
	else
		lootgroup := 301;
	endif

	var magic_chance;
	var magic_level;
	var lock_difficulty;
	if( lootgroup == 307 )
		magic_level  := 10;
		magic_chance := 80 + RandomInt(20);	// 80-99
		lock_difficulty := 140 + RandomInt(11);	// 140-150
	elseif( lootgroup == 306 )
		magic_level  := 9;
		magic_chance := 70 + RandomInt(30);	// 70-99
		lock_difficulty := 120 + RandomInt(21);	// 120-140
	else
		magic_level  := 1 + RandomInt(9);	// 1-9
		magic_chance := 30 + RandomInt(70);	// 30-99
		lock_difficulty := 50 + RandomInt(71);	// 50-120
	endif

	MakeLoot( chest, lootgroup, magic_chance, magic_level );

	chest.locked := 1;
	SetObjProperty( chest, PROPID_CHEST_SPAWNPOINT_LOCK_DIFFICULTY, lock_difficulty );
	SetObjProperty( chest, PROPID_CHEST_TRAP_TYPE, RandomInt(3)+1 );
	SetObjProperty( chest, PROPID_CHEST_TRAP_STRENGTH, CInt(magic_level + 1) );
	SetObjProperty( chest, PROPID_CHEST_TRAPPED_BY, "AreaSpawner" );
	chest.usescript := USESCRIPTID_TRAPPED_CONTAINER;

	var tries := 0;
	var placed := 0;
	var x;
	var y;
	var z;
	chest.movable := 1;

	while( tries < AREASPAWNER_PLACEMENT_MAX_TRIES && !placed )
		tries := tries + 1;
		x := box.minx + RandomInt(box.maxx - box.minx + 1);
		y := box.miny + RandomInt(box.maxy - box.miny + 1);
		z := CInt( GetWorldHeight(x, y, realm) );

		var standing := GetStandingHeight(x, y, z, realm);
		if( standing && !standing.multi )
			// Unlike NPCs/plain items, a chest can't tolerate landing on top
			// of another container - two lootable chests stacked on one tile
			// is confusing and lets a player miss/loot the wrong one. Reject
			// the tile (regardless of exact z) if any container is already
			// sitting there and just retry like any other bad placement.
			var tile_occupied := 0;
			foreach existing in ListItemsNearLocation(x, y, LIST_IGNORE_Z, 0, realm)
				if( existing.IsA(POLCLASS_CONTAINER) )
					tile_occupied := 1;
					break;
				endif
			endforeach

			if( !tile_occupied )
				if( MoveObjectToLocation( chest, x, y, z, realm, MOVEOBJECT_FORCELOCATION ) )
					placed := 1;
				endif
			endif
		endif

		if( !placed )
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endif
	endwhile

	if( !placed )
		AreaSpawnerDebug("chest placement FAILED for template '"+template+"' after "+CStr(tries)+" tries, realm="+realm);
		DestroyItem(chest);
		return struct{ item := 0, config_error := "" };
	endif

	chest.movable := 0;
	chest.hidden := 0;

	AreaSpawnerDebug("placed chest '"+template+"' (lootgroup "+CStr(lootgroup)+", lock "+CStr(lock_difficulty)+") at "+CStr(x)+","+CStr(y)+" ("+realm+") after "+CStr(tries)+" tries");
	return struct{ item := chest, config_error := "" };
endfunction


// Canonical shape of a Custom NPC entry's overrides struct - both
// GetNpcTemplateBaseStats (below) and ShowCustomNpcEntryEditor's "New Custom
// NPC" default must produce a struct with exactly this field set, since
// Load Template replaces the whole struct wholesale (ov := basestats) rather
// than merging field-by-field. Every field follows the same "0/blank means
// not overridden, scaffold template's own value wins" convention already
// established for str/dex/intel/etc - EXCEPT gender, which has no unset
// state worth protecting (see TryPlaceAreaSpawnerCustomNpc's apply block).
function NewCustomNpcOverrides()
	return struct{
		name := "",
		str := 0, dex := 0, intel := 0,
		hits := 0, stam := 0, mana := 0,
		hpregen := 0, manaregen := 0,
		color := 0, truecolor := 0,
		objtype := 0,
		gender := 0,
		lesserboss := 0, boss := 0, superboss := 0,
		runspeed := 0, movespeed := 0,
		snoopme := 0, stealme := 0, looter := 0,
		slaytype := "",
		fireprot := 0, waterprot := 0, airprot := 0, earthprot := 0, holyprot := 0, necroprot := 0, physicalprot := 0,
		permpoisonimmunity := 0, permmagicimmunity := 0, freeaction := 0, permmr := 0
	};
endfunction


// The full set of NPC "Type" CProp values this shard's combat system
// actually checks for slayer-weapon bonus damage - see
// pkg/systems/combat/include/hitscriptinc.inc's GetSlayMultiplier, whose
// RandomInt(16) case statement for SlayType "WhoKnows" is the single place
// in the codebase that enumerates them (npcdesc.cfg's own header comment
// documents the same list by hand, which is what actually drifts - this
// mirrors the combat script instead, plus "Human", which GetSlayMultiplier
// checks for separately since it means "not an npctemplate" rather than a
// Type CProp match, but which npcdesc.cfg entries do still assign npcs
// directly (e.g. the "beckon" fairy template)). There's no runtime API to
// enumerate a case statement's literals, so this has to be hand-kept in sync
// with that one function - nowhere else.
function AreaSpawnerSlayTypes()
	return array{
		"Slime", "Ratkin", "Plant", "Animal", "Beholder", "Orc", "Terathan",
		"Ophidian", "Animated", "Gargoyle", "Troll", "Giantkin", "Elemental",
		"Undead", "Daemon", "Dragonkin", "Human"
	};
endfunction


// Reads a template's base stats by instantiating a scratch NPC (hidden, at
// the same staging spot placement uses) and reading its live values back -
// same approach pkg/opt/spawnpoint's customnpc.inc uses to capture a live
// NPC's stats, and more reliable than trying to parse STR/DEX/INT/Hits/etc.
// out of npcdesc.cfg directly (CProp overrides, ranges, script-computed
// values, etc. make that fragile). Returns 0 if the template doesn't exist.
// Read-only lookup of a template's loot-roll fields straight from
// npcdesc.cfg - same 3 fields pkg/opt/spawnpoint/include/customnpc.inc's
// CustomNPCGump reads for the same purpose (informational display next to
// the template picker, not a capturable/override stat like Appearance/
// Stats/Prots). Returns 0 if the template doesn't exist in npcdesc.cfg.
function AreaSpawnerNpcLootInfo(template)
	var npccfg := ReadConfigFile(CFGFILE_NPCS);
	var elem := FindConfigElem(npccfg, template);
	if( !elem )
		return 0;
	endif

	var info := struct;
	info.+lootgroup	:= GetConfigInt(elem, "lootgroup");
	info.+miclevel	:= GetConfigInt(elem, "MagicItemLevel");
	info.+micchance	:= GetConfigInt(elem, "MagicItemChance");
	return info;
endfunction


// This is the "copy stats from a template" half of the Custom NPC kind - the
// GM then edits the returned struct's fields before it's saved as an entry's
// overrides.
function GetNpcTemplateBaseStats(template)
	var scratch := CreateNpcFromTemplate( template, AREASPAWNER_HOME_X, AREASPAWNER_HOME_Y, 0, 0, "britannia_alt" );
	if( !scratch )
		return 0;
	endif
	scratch.hidden := 1;

	// CreateNpcFromTemplate runs the scratch npc's normal AI startup just
	// like any other spawn, so every CProp npcdesc.cfg sets on this template
	// (BaseHpRegen, the *Protection fields, Type, etc.) is already sitting on
	// this live instance as an ObjProperty - straight GetObjProperty reads,
	// same as pkg/opt/spawnpoint/include/customnpc.inc and
	// scripts/ai/setup/modsetup.inc both already rely on for a live npc.
	var stats := NewCustomNpcOverrides();
	stats.name	:= scratch.name;
	stats.str	:= GetStrength(scratch);
	stats.dex	:= GetDexterity(scratch);
	stats.intel	:= GetIntelligence(scratch);
	stats.hits	:= GetMaxHp(scratch);
	stats.stam	:= GetMaxStamina(scratch);
	stats.mana	:= GetMaxMana(scratch);
	stats.hpregen	:= CInt(GetObjProperty(scratch, "BaseHpRegen"));
	stats.manaregen	:= CInt(GetObjProperty(scratch, "BaseManaRegen"));
	stats.color	:= scratch.color;
	stats.truecolor	:= CInt(scratch.TrueColor);
	stats.objtype	:= scratch.graphic;
	stats.gender	:= CInt(scratch.gender);
	stats.lesserboss	:= GetObjProperty(scratch, "LesserBoss") ? 1 : 0;
	stats.boss		:= GetObjProperty(scratch, "Boss") ? 1 : 0;
	stats.superboss		:= GetObjProperty(scratch, "SuperBoss") ? 1 : 0;
	stats.runspeed	:= scratch.run_speed;
	stats.movespeed	:= CInt(GetObjProperty(scratch, "MoveSpeed"));
	stats.snoopme	:= CInt(GetObjProperty(scratch, "snoopme"));
	stats.stealme	:= CInt(GetObjProperty(scratch, "stealme"));
	stats.looter	:= GetObjProperty(scratch, "Looter") ? 1 : 0;
	stats.slaytype	:= GetObjProperty(scratch, "Type") ? GetObjProperty(scratch, "Type") : "";
	stats.fireprot		:= CInt(GetObjProperty(scratch, "FireProtection"));
	stats.waterprot		:= CInt(GetObjProperty(scratch, "WaterProtection"));
	stats.airprot		:= CInt(GetObjProperty(scratch, "AirProtection"));
	stats.earthprot		:= CInt(GetObjProperty(scratch, "EarthProtection"));
	stats.holyprot		:= CInt(GetObjProperty(scratch, "HolyProtection"));
	stats.necroprot		:= CInt(GetObjProperty(scratch, "NecroProtection"));
	stats.physicalprot	:= CInt(GetObjProperty(scratch, "PhysicalProtection"));
	stats.permpoisonimmunity	:= CInt(GetObjProperty(scratch, "PermPoisonImmunity"));
	stats.permmagicimmunity	:= CInt(GetObjProperty(scratch, "PermMagicImmunity"));
	stats.freeaction		:= GetObjProperty(scratch, "FreeAction") ? 1 : 0;
	stats.permmr			:= CInt(GetObjProperty(scratch, "Permmr"));

	KillMobile(scratch, 1);
	return stats;
endfunction


// Custom NPC kind's placement - same retry-with-delay shape as
// TryPlaceAreaSpawnerCritter, but stamps the entry's captured/edited stat
// overrides onto the fresh scaffold critter right after creation, before any
// placement attempt - so if placement ultimately fails, the already-
// customized critter still gets cleaned up the normal way. overrides missing
// a field (an old entry saved before some field existed) just leaves that
// stat at the scaffold template's own default via the 0-check per field.
function TryPlaceAreaSpawnerCustomNpc(realm, box, template, overrides, allow_water := 0)
	var critter := CreateNpcFromTemplate( template, AREASPAWNER_HOME_X, AREASPAWNER_HOME_Y, 0, 0, "britannia_alt" );
	if( !critter )
		AreaSpawnerDebug("CreateNpcFromTemplate FAILED for custom npc scaffold '"+template+"' - bad/missing npc template?");
		return struct{ critter := 0, config_error := "npc template '"+template+"' does not exist" };
	endif
	critter.hidden := 1;

	if( overrides )
		if( overrides.name )
			critter.name := overrides.name;
		endif
		if( overrides.str )
			SetBaseStrength(critter, Cint(overrides.str));
		endif
		if( overrides.dex )
			SetBaseDexterity(critter, Cint(overrides.dex));
		endif
		if( overrides.intel )
			SetBaseIntelligence(critter, Cint(overrides.intel));
		endif
		// Plain SetVital(critter, VITALID_LIFE, overrides.hits) is wrong two
		// ways: SetVital's raw value is hundredths (overrides.hits is a real
		// HP number, so it'd be 100x under-scaled), and it only stamps the
		// CURRENT vital, not the ceiling - this repo's actual max-HP formula
		// (pkg/opt/shilhook/regen.src's GetLifeMaximumValueExported, wired via
		// vitals.cfg's MaximumFunction) checks the CustomHitsLevel ObjProperty
		// first and only falls back to STR*100 if it's unset, exactly like
		// npcdesc.cfg's own CustomHitsLevel field. So the override has to set
		// that property (in hundredths, matching how it's stored on real
		// templates) to actually raise the ceiling, then SetHp - which is
		// already correctly scaled/clamped - to fill current HP to match.
		// Mana/Stamina have the identical CustomManaLevel/CustomStaminaLevel
		// mechanism in the same file, so the same fix applies to both.
		if( overrides.hits )
			SetObjProperty(critter, "CustomHitsLevel", Cint(overrides.hits) * 100);
			SetHp(critter, Cint(overrides.hits));
		endif
		if( overrides.stam )
			SetObjProperty(critter, "CustomStaminaLevel", Cint(overrides.stam) * 100);
			SetStamina(critter, Cint(overrides.stam));
		endif
		if( overrides.mana )
			SetObjProperty(critter, "CustomManaLevel", Cint(overrides.mana) * 100);
			SetMana(critter, Cint(overrides.mana));
		endif
		if( overrides.color )
			critter.color := Cint(overrides.color);
		endif
		if( overrides.truecolor )
			critter.TrueColor := Cint(overrides.truecolor);
		endif
		if( overrides.objtype )
			critter.graphic := Cint(overrides.objtype);
			critter.trueobjtype := Cint(overrides.objtype);
		endif
		// No 0-skip here - unlike everything else, 0 (male) is exactly as
		// valid an explicit choice as 1 (female), and GetNpcTemplateBaseStats
		// always seeds this from the real scratch npc on Load, so this is
		// just "whatever Appearance last showed", never a stale/missing value
		critter.gender := Cint(overrides.gender);
		if( overrides.lesserboss )
			SetObjProperty(critter, "LesserBoss", 1);
		endif
		if( overrides.boss )
			SetObjProperty(critter, "Boss", 1);
		endif
		if( overrides.superboss )
			SetObjProperty(critter, "SuperBoss", 1);
		endif
		if( overrides.runspeed )
			critter.run_speed := Cint(overrides.runspeed);
		endif
		if( overrides.movespeed )
			SetObjProperty(critter, "MoveSpeed", Cint(overrides.movespeed));
		endif
		if( overrides.snoopme )
			SetObjProperty(critter, "snoopme", Cint(overrides.snoopme));
		endif
		if( overrides.stealme )
			SetObjProperty(critter, "stealme", Cint(overrides.stealme));
		endif
		if( overrides.looter )
			SetObjProperty(critter, "Looter", 1);
		endif
		if( overrides.slaytype )
			SetObjProperty(critter, "Type", overrides.slaytype);
		endif
		// Elemental/alt protections and regen bonuses are plain ObjProperty
		// CProps read by scripts/ai/setup/modsetup.inc (Base*Regen ->
		// regen rate, *Protection/Perm*/FreeAction -> its possible_prots
		// Karma/Fame bonus loop) - that setup script runs when the AI
		// process (re)starts, which is exactly what RestartScript below
		// does, so setting these here before that call is what makes them
		// actually take effect, same mechanism as CustomHitsLevel above.
		if( overrides.hpregen )
			SetObjProperty(critter, "BaseHpRegen", Cint(overrides.hpregen));
		endif
		if( overrides.manaregen )
			SetObjProperty(critter, "BaseManaRegen", Cint(overrides.manaregen));
		endif
		if( overrides.fireprot )
			SetObjProperty(critter, "FireProtection", Cint(overrides.fireprot));
		endif
		if( overrides.waterprot )
			SetObjProperty(critter, "WaterProtection", Cint(overrides.waterprot));
		endif
		if( overrides.airprot )
			SetObjProperty(critter, "AirProtection", Cint(overrides.airprot));
		endif
		if( overrides.earthprot )
			SetObjProperty(critter, "EarthProtection", Cint(overrides.earthprot));
		endif
		if( overrides.holyprot )
			SetObjProperty(critter, "HolyProtection", Cint(overrides.holyprot));
		endif
		if( overrides.necroprot )
			SetObjProperty(critter, "NecroProtection", Cint(overrides.necroprot));
		endif
		if( overrides.physicalprot )
			SetObjProperty(critter, "PhysicalProtection", Cint(overrides.physicalprot));
		endif
		if( overrides.permpoisonimmunity )
			SetObjProperty(critter, "PermPoisonImmunity", Cint(overrides.permpoisonimmunity));
		endif
		if( overrides.permmagicimmunity )
			SetObjProperty(critter, "PermMagicImmunity", Cint(overrides.permmagicimmunity));
		endif
		if( overrides.freeaction )
			SetObjProperty(critter, "FreeAction", 1);
		endif
		if( overrides.permmr )
			SetObjProperty(critter, "Permmr", Cint(overrides.permmr));
		endif
	endif

	var tries := 0;
	var placed := 0;
	var x;
	var y;
	var fail_no_standing := 0;
	var fail_multi := 0;
	var fail_moveobject := 0;

	while( tries < AREASPAWNER_PLACEMENT_MAX_TRIES && !placed )
		tries := tries + 1;
		x := box.minx + RandomInt(box.maxx - box.minx + 1);
		y := box.miny + RandomInt(box.maxy - box.miny + 1);

		if( allow_water )
			var candidates := GetStandingCoordinates(x, y, 0, -128, 127, realm, critter.movemode, 0);
			if( candidates && candidates.size() )
				var spot := candidates[1];
				var standing := GetStandingHeight(spot.x, spot.y, spot.z, realm);
				if( standing && standing.multi )
					fail_multi := fail_multi + 1;
				elseif( MoveObjectToLocation( critter, spot.x, spot.y, spot.z, realm, MOVEOBJECT_FORCELOCATION ) )
					placed := 1;
					x := spot.x;
					y := spot.y;
				else
					fail_moveobject := fail_moveobject + 1;
				endif
			else
				fail_no_standing := fail_no_standing + 1;
			endif
		else
			var z := CInt( GetWorldHeight(x, y, realm) );
			var standing2 := GetStandingHeight(x, y, z, realm);
			if( !standing2 )
				fail_no_standing := fail_no_standing + 1;
			elseif( standing2.multi )
				fail_multi := fail_multi + 1;
			elseif( MoveObjectToLocation( critter, x, y, z, realm, MOVEOBJECT_FORCELOCATION ) )
				placed := 1;
			else
				fail_moveobject := fail_moveobject + 1;
			endif
		endif

		if( !placed )
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endif
	endwhile

	if( !placed )
		AreaSpawnerDebug("custom npc placement FAILED for template '"+template+"' after "+CStr(tries)+" tries, realm="+realm+" - allow_water="+CStr(allow_water)+", no_standing="+CStr(fail_no_standing)+" multi_blocked="+CStr(fail_multi)+" moveobject_rejected="+CStr(fail_moveobject));
		KillMobile(critter, 1);
		return struct{ critter := 0, config_error := "" };
	endif

	AreaSpawnerDebug("placed custom npc '"+template+"' at "+CStr(x)+","+CStr(y)+" ("+realm+") after "+CStr(tries)+" tries");
	RestartScript(critter);
	critter.hidden := 0;
	return struct{ critter := critter, config_error := "" };
endfunction


// Dispatches to the NPC, custom-npc, or item placement path based on
// entry.source, and normalizes each struct's differently-named payload field
// (critter/item) to a common .obj - lets TickAreaSpawnerEntry/
// FullRespawnAreaSpawner stay source-agnostic instead of branching at every
// call site. overrides is only read for source=="customnpc".
function TryPlaceAreaSpawnerEntity(realm, box, source, template, allow_water := 0, overrides := 0)
	if( source == "chestgroup" )
		var chestresult := TryPlaceAreaSpawnerChest(realm, box, template);
		return struct{ obj := chestresult.item, config_error := chestresult.config_error };
	endif
	if( source == "item" )
		var itemresult := TryPlaceAreaSpawnerItem(realm, box, template);
		return struct{ obj := itemresult.item, config_error := itemresult.config_error };
	endif
	if( source == "customnpc" )
		var customresult := TryPlaceAreaSpawnerCustomNpc(realm, box, template, overrides, allow_water);
		return struct{ obj := customresult.critter, config_error := customresult.config_error };
	endif
	var critterresult := TryPlaceAreaSpawnerCritter(realm, box, template, allow_water);
	return struct{ obj := critterresult.critter, config_error := critterresult.config_error };
endfunction


function TickAreaSpawnerEntry(realm, name, entry, box, prefix := "areaspawner", allow_water := 0)
	// prune serials that no longer resolve (death, capture, taming, GM removal, etc.)
	var alive := array{};
	var current_serials := entry.serials;
	foreach serial in current_serials
		if( serial && SystemFindObjectBySerial(serial) )
			alive.append(serial);
		endif
	endforeach
	if( alive.size() != current_serials.size() )
		AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" pruned "+CStr(current_serials.size()-alive.size())+" dead/gone serial(s), "+CStr(alive.size())+" remain");
	endif
	entry.serials := alive;

	if( entry.serials.size() >= entry.target )
		return entry;
	endif

	var clock := ReadGameClock();
	if( clock < entry.next_spawn_time )
		return entry;
	endif

	var template := ResolveAreaSpawnerTemplate(entry, prefix);
	// forbidden-template check only means anything for npc/group sources -
	// see IsAreaSpawnerTemplateForbidden's comment
	var isnpcsource := (entry.source == "group" || entry.source == "npc" || entry.source == "customnpc");
	if( !template || (isnpcsource && IsAreaSpawnerTemplateForbidden(template)) )
		AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" ("+entry.source+":"+CStr(entry.ref)+") could not resolve a template - forbidden or missing group/npc");
		entry.last_fail_time := clock;
		entry.next_spawn_time := clock + entry.frequency;
		// won't fix itself by retrying - the group/npc has to actually exist
		// in areagroups.cfg/npcdesc.cfg, or the entry has to be edited
		entry.config_error := "could not resolve a template for "+entry.source+":"+CStr(entry.ref)+" - missing/forbidden";
		return entry;
	endif

	var result := TryPlaceAreaSpawnerEntity(realm, box, entry.source, template, allow_water, entry.overrides);
	if( !result.obj )
		entry.last_fail_time := clock;
		entry.next_spawn_time := clock + entry.frequency;
		entry.config_error := result.config_error;

		// only a genuine "couldn't find room" failure counts toward the
		// pause threshold - a bad template is already handled (and won't
		// get any less bad by retrying) via config_error above
		if( !result.config_error )
			entry.fail_cycles := entry.fail_cycles + 1;
			if( entry.fail_cycles >= AREASPAWNER_ENTRY_FAIL_THRESHOLD )
				entry.paused_reason := "couldn't place '"+template+"' after "+CStr(entry.fail_cycles)+" cycles in a row - box may not suit this template's MoveMode";
				AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" pausing - "+entry.paused_reason);
			endif
		endif

		return entry;
	endif

	entry.config_error := "";
	entry.fail_cycles := 0;
	entry.paused_reason := "";
	TagAreaSpawnerObject(result.obj, realm, name, entry.id, prefix);
	entry.serials.append(result.obj.serial);
	entry.next_spawn_time := clock + entry.frequency;
	AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" now "+CStr(entry.serials.size())+"/"+CStr(entry.target));

	return entry;
endfunction


// Event/timer-driven, same idea as pkg/opt/spawnpoint: no global poll. A spawner
// sits idle once every entry is at target, and only wakes back up when something
// dies (RemoveAreaSpawnerSerial kicks the entry that lost a serial) or a GM
// action changes what's needed (Enable toggled on, entry added/edited). Each
// kick is a one-shot script (areaspawnerentrytick) that tries one spawn, then
// re-kicks itself (paced by the entry's own frequency) if still under target -
// it stops rescheduling itself the moment nothing is left to do.
function AreaSpawnerKickEntry(realm, name, prefix, entry_id, delay)
	AreaSpawnerDebug("kicking '"+name+"' ("+realm+") entry #"+CStr(entry_id)+" - firing in "+CStr(delay)+"s ["+prefix+"]");
	return Start_Script( ":areaspawner:areaspawnerentrytick", array{realm,name,prefix,entry_id,delay} );
endfunction


// Pure in-memory mark, no Start_Script - only actually marks "refilling" if
// no chain is already in flight for this entry (a death, or a hundred, while
// a chain is already running just adds to the deficit that chain will notice
// and keep trickling in on its own, rather than spinning up N parallel
// chains all doing the same job). Split out from the kick itself so callers
// can save the mark BEFORE starting the entrytick script - see
// AreaSpawnerMarkAndKickEntry for why that order matters.
function AreaSpawnerMarkEntryRefilling(byref record, entry_id)
	var idx := FindAreaSpawnerEntryIndex(record, entry_id);
	if( !idx )
		return 0;
	endif
	if( record.entries[idx].refilling )
		return 0;
	endif
	record.entries[idx].refilling := 1;
	return 1;
endfunction


// Marks + saves + kicks, in that order. The save has to happen BEFORE
// Start_Script, not after - areaspawnerentrytick loads its own fresh copy of
// this record the moment it runs, and if the "refilling" mark (or whatever
// else the caller already changed on `record`) hasn't hit disk yet, it can
// load stale data. Whether POL's scheduler could actually let the new script
// run before this caller finishes isn't something this comment can promise
// either way, so this just closes the window rather than relying on timing.
function AreaSpawnerMarkAndKickEntry(realm, name, prefix, byref record, entry_id, delay)
	if( !AreaSpawnerMarkEntryRefilling(record, entry_id) )
		return 0;
	endif
	SaveAreaSpawner(realm, name, record, 0, prefix);
	AreaSpawnerKickEntry(realm, name, prefix, entry_id, delay);
	return 1;
endfunction


// TickAreaSpawnerEntry paces itself with next_spawn_time = last attempt +
// frequency, set on both success AND failure - so a run of failed attempts
// (e.g. an invalid/zero box) leaves a cooldown sitting in the future. A kick
// that's meant to retry right now (re-enabling, or saving a just-fixed box)
// needs to clear that first, or it silently sits there doing nothing until
// the stale cooldown finally expires on its own.
//
// Also force-clears refilling. That flag is only ever supposed to come back
// down to 0 when areaspawnerentrytick itself runs to completion - if that
// script ever fails to actually start (a bad Start_Script call) or dies
// mid-run before reaching its own save/reschedule logic, refilling is left
// stuck at 1 forever with NOTHING able to recover it: AreaSpawnerMarkEntry-
// Refilling refuses to re-mark an already-refilling entry, so every future
// death, edit, Enable toggle, and even the coarse safety-net sweep would
// silently no-op on it indefinitely. Every caller of this function is
// exactly the moment a GM (or the safety net) is explicitly saying "make
// sure this is actually running" - the ideal, and only, place to break that
// stuck state loose again, at the small cost of possibly firing a second
// concurrent kick for an entry whose chain genuinely was still healthy and
// mid-Sleep (harmless - it just tries one extra placement).
function AreaSpawnerResetEntryCooldowns(byref record)
	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		record.entries[i].next_spawn_time := 0;
		record.entries[i].refilling := 0;
	endfor
	return 1;
endfunction


function AreaSpawnerKickAllEntries(realm, name, prefix, byref record, delay)
	// AreaSpawnerMarkAndKickEntry saves before every kick it actually fires,
	// so nothing extra is needed here - no leftover "unsaved mark" to catch
	// after the loop the way there used to be.
	var entries := record.entries;
	foreach entry in entries
		AreaSpawnerMarkAndKickEntry(realm, name, prefix, record, entry.id, delay);
	endforeach
	return 1;
endfunction


// Boot-time priming and the coarse safety-net sweep both just re-kick every
// entry of every enabled spawner - cheap no-ops for anything already at target
// (areaspawnerentrytick checks and doesn't reschedule), real work only for
// anything that's actually short. Also the one place a permanently-stuck
// refilling flag (see AreaSpawnerResetEntryCooldowns) gets a chance to
// recover on its own, without a GM having to notice and manually touch the
// spawner - this sweep exists specifically to catch things that drifted.
function AreaSpawnerKickAllEnabled(delay)
	var arealms := AreaSpawnerRealms();
	var kinds := AreaSpawnerKinds();
	var eventflags := array{ 0, 1 };
	foreach realm in arealms
		foreach kind in kinds
			foreach is_event in eventflags
				var prefix := AreaSpawnerKindPrefix(kind, is_event);
				var names := GetAreaSpawnerNames(realm, prefix);
				foreach name in names
					var record := LoadAreaSpawner(realm, name, prefix);
					if( record && record.enabled )
						AreaSpawnerResetEntryCooldowns(record);
						AreaSpawnerKickAllEntries(realm, name, prefix, record, delay);
					endif
				endforeach
			endforeach
		endforeach
	endforeach
	return 1;
endfunction


// Used by the staff login notice and the .aserror* commands - flags two
// distinct situations so staff know to go check: "disabled" (record.enabled=0,
// the whole spawner stopped) and "paused" (still enabled and mostly working,
// but one entry gave up after repeated placement failures - see
// AREASPAWNER_ENTRY_FAIL_THRESHOLD). A paused entry alone never sets
// record.enabled=0, so without this it would fail silently with nothing in
// the list gump to hint at it beyond opening Entries and reading each row.
//
// kindfilter/eventfilter narrow the sweep for the per-kind commands - pass
// "" / -1 (the defaults) to scan everything, matching the old no-argument
// behavior. kindfilter must be one of AreaSpawnerKinds(); eventfilter is
// 0 (regular only), 1 (event only), or -1 (both).
function ListAreaSpawnerAutoDisabled(kindfilter := "", eventfilter := -1)
	var found := array{};
	var arealms := AreaSpawnerRealms();
	var kinds := AreaSpawnerKinds();
	var eventflags := array{ 0, 1 };
	foreach realm in arealms
		foreach kind in kinds
			if( kindfilter && kind != kindfilter )
				continue;
			endif
			foreach is_event in eventflags
				if( eventfilter != -1 && is_event != eventfilter )
					continue;
				endif
				var prefix := AreaSpawnerKindPrefix(kind, is_event);
				var names := GetAreaSpawnerNames(realm, prefix);
				foreach name in names
					var record := LoadAreaSpawner(realm, name, prefix);
					if( !record )
						continue;
					endif
					if( !record.enabled && record.disabled_reason )
						found.append( struct{ realm := realm, name := name, prefix := prefix, kind := kind, is_event := is_event, status := "disabled", reason := record.disabled_reason } );
					elseif( record.enabled )
						var pausedreason := AreaSpawnerFindPausedReason(record);
						if( pausedreason )
							found.append( struct{ realm := realm, name := name, prefix := prefix, kind := kind, is_event := is_event, status := "paused", reason := pausedreason } );
						endif
					endif
				endforeach
			endforeach
		endforeach
	endforeach
	return found;
endfunction


function CountAreaSpawnerAutoDisabled()
	return ListAreaSpawnerAutoDisabled().size();
endfunction


function DespawnAllAreaSpawner(record)
	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		var serials := record.entries[i].serials;
		foreach serial in serials
			var obj := SystemFindObjectBySerial(serial);
			if( obj )
				AreaSpawnerDespawnObject(obj);
			endif
		endforeach
		record.entries[i].serials := array{};
	endfor
	return record;
endfunction


// Deliberately ignores record.enabled - Full Respawn is a manual, one-shot GM
// action, not the passive automatic tick, so it shouldn't be silently gated by
// the same toggle that governs whether the manager keeps this spawner topped up.
function FullRespawnAreaSpawner(realm, name, prefix := "areaspawner")
	AreaSpawnerDebug("full respawn starting for '"+name+"' ("+realm+") ["+prefix+"]");
	var record := LoadAreaSpawner(realm, name, prefix);
	if( !record )
		AreaSpawnerDebug("full respawn ABORTED - '"+name+"' ("+realm+") not found in datafile");
		return 0;
	endif

	if( !record.entries.size() )
		AreaSpawnerDebug("full respawn ABORTED - '"+name+"' ("+realm+") has no entries configured");
		return 0;
	endif

	if( !record.enabled )
		AreaSpawnerDebug("'"+name+"' ("+realm+") is not Enabled - full respawn will still place mobs, but the passive tick won't maintain them afterward");
	endif

	record := DespawnAllAreaSpawner(record);

	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		record.entries[i].target := record.entries[i].min + RandomInt(record.entries[i].max - record.entries[i].min + 1);
		record.entries[i].next_spawn_time := 0;
	endfor

	SaveAreaSpawner(realm, name, record, 0, prefix);

	var box := ResolveAreaSpawnerBox(realm, record);
	box := ClampBoxToRealm(realm, box);
	AreaSpawnerDebug("'"+name+"' ("+realm+") box after clamp: "+CStr(box.minx)+","+CStr(box.maxx)+","+CStr(box.miny)+","+CStr(box.maxy));
	if( box.minx > box.maxx || box.miny > box.maxy )
		AreaSpawnerDebug("full respawn ABORTED - '"+name+"' ("+realm+") box is degenerate after realm-bounds clamp");
		return 0;
	endif
	if( box.minx == box.maxx && box.miny == box.maxy && box.minx == 0 && box.miny == 0 )
		AreaSpawnerDebug("'"+name+"' ("+realm+") box looks unset (0,0,0,0) - did you Pick an area or enter coords?");
	endif

	var config_error := "";
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		var entry := record.entries[i];
		AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" ("+entry.source+":"+CStr(entry.ref)+") filling toward target="+CStr(entry.target));
		var isnpcsource := (entry.source == "group" || entry.source == "npc" || entry.source == "customnpc");
		while( entry.serials.size() < entry.target )
			var template := ResolveAreaSpawnerTemplate(entry, prefix);
			if( !template || (isnpcsource && IsAreaSpawnerTemplateForbidden(template)) )
				AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" giving up - could not resolve a template for source='"+entry.source+"' ref='"+CStr(entry.ref)+"' (missing group/npc in areagroups.cfg, or forbidden)");
				entry.last_fail_time := ReadGameClock();
				entry.config_error := "could not resolve a template for "+entry.source+":"+CStr(entry.ref)+" - missing/forbidden";
				config_error := entry.config_error;
				break;
			endif

			var result := TryPlaceAreaSpawnerEntity(realm, box, entry.source, template, record.allow_water, entry.overrides);
			if( !result.obj )
				AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" giving up - could not place '"+template+"' anywhere in the box");
				entry.last_fail_time := ReadGameClock();
				entry.config_error := result.config_error;
				if( result.config_error )
					config_error := result.config_error;
				endif
				break;
			endif

			entry.config_error := "";
			entry.fail_cycles := 0;
			entry.paused_reason := "";
			TagAreaSpawnerObject(result.obj, realm, name, entry.id, prefix);
			entry.serials.append(result.obj.serial);
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endwhile
		entry.next_spawn_time := ReadGameClock() + entry.frequency;
		record.entries[i] := entry;
	endfor

	// same reasoning as areaspawnerentrytick's zero-box case: a bad/missing
	// template won't fix itself by retrying, so surface it in the list
	// instead of leaving the spawner sitting "ON" and quietly short forever
	if( config_error )
		record.enabled := 0;
		record.disabled_reason := config_error;
		AreaSpawnerDebug("'"+name+"' ("+realm+") disabling - "+config_error);
	elseif( record.disabled_reason )
		// a previous run auto-disabled this (bad template, etc.) and this
		// run completed clean - whatever was broken got fixed, so clear the
		// stale flag instead of leaving it stuck OFF with an outdated reason
		AreaSpawnerDebug("'"+name+"' ("+realm+") re-enabling - full respawn succeeded, previous auto-disable reason no longer applies");
		record.enabled := 1;
		record.disabled_reason := "";
	endif

	SaveAreaSpawner(realm, name, record, 0, prefix);
	AreaSpawnerDebug("full respawn complete for '"+name+"' ("+realm+"): "+AreaSpawnerPopulationSummary(record));
	return 1;
endfunction


function DeleteAreaSpawner(realm, name, prefix := "areaspawner")
	AreaSpawnerDebug("deleting '"+name+"' ("+realm+") ["+prefix+"]");
	var record := LoadAreaSpawner(realm, name, prefix);
	if( record )
		record := DespawnAllAreaSpawner(record);
		SaveAreaSpawner(realm, name, record, 0, prefix);
	endif
	return DeleteAreaSpawnerRecord(realm, name, prefix);
endfunction


// Plain items (the "item" kind) still have no on-demand refill kick - nothing
// hooks generic item destruction the way scripts/misc/death.src hooks NPC
// death, so those only get topped up eventually via TickAreaSpawnerEntry's
// serial-liveness pruning, whenever something else kicks that entry (an
// edit, an Enable toggle, or the coarse AreaSpawnerManager safety-net sweep -
// AREASPAWNER_SAFETYNET_INTERVAL, currently 4 hours). Chest kind entries DO
// get an on-demand kick now - pkg/items/containers/container/canDestroy.src
// (every container's DestroyScript) and pkg/std/lockpicking/use/picklock.src
// (a legit pick) both call NotifyAreaSpawnerChestGone below.
function RemoveAreaSpawnerSerial(area_info, mobile_serial)
	if( !area_info || !area_info.name || !area_info.realm )
		return 0;
	endif

	var prefix := area_info.prefix ? area_info.prefix : "areaspawner";

	var record := LoadAreaSpawner(area_info.realm, area_info.name, prefix);
	if( !record )
		return 0;
	endif

	var idx := FindAreaSpawnerEntryIndex(record, area_info.entry_id);
	if( !idx )
		return 0;
	endif

	var kept := array{};
	var serials := record.entries[idx].serials;
	foreach serial in serials
		if( serial != mobile_serial )
			kept.append(serial);
		endif
	endforeach

	// .kill/.akill call this both explicitly and (via mob.kill() triggering
	// the organic death path in scripts/misc/death.src) a second time for the
	// same corpse - only act the first time a given serial is actually seen
	// gone, otherwise the duplicate would also schedule its own redundant
	// refill kick, roughly doubling the effective respawn rate.
	if( kept.size() == serials.size() )
		return 0;
	endif

	record.entries[idx].serials := kept;

	AreaSpawnerDebug("death report: '"+CStr(area_info.name)+"' entry #"+CStr(area_info.entry_id)+" lost serial "+CStr(mobile_serial)+", "+CStr(kept.size())+" remain");

	// This is the whole reason the passive side doesn't need to poll - a death
	// is exactly when a slot opens up, so kick a refill for just this entry
	// right here instead of waiting for some later sweep to notice. But if a
	// chain is already in flight for this entry (e.g. an .akill just killed
	// a dozen of them), don't start another - the existing chain re-checks
	// the real deficit every cycle regardless of how many died in the meantime.
	if( !record.enabled )
		AreaSpawnerDebug("death report: '"+CStr(area_info.name)+"' not Enabled - not kicking a refill");
		SaveAreaSpawner(area_info.realm, area_info.name, record, 0, prefix);
	elseif( AreaSpawnerMarkAndKickEntry(area_info.realm, area_info.name, prefix, record, area_info.entry_id, record.entries[idx].frequency) )
		// AreaSpawnerMarkAndKickEntry already saved record (with both the
		// pruned serials above and its own refilling flag) right before
		// kicking - saving again here would just be a redundant second
		// datafile write for the exact same data, on every single death
	else
		AreaSpawnerDebug("death report: '"+CStr(area_info.name)+"' entry #"+CStr(area_info.entry_id)+" already has a refill chain running - letting it continue");
		SaveAreaSpawner(area_info.realm, area_info.name, record, 0, prefix);
	endif

	return 1;
endfunction


// Called two places: pkg/items/containers/container/canDestroy.src (any
// areaspawner chest actually being destroyed - the generic DestroyScript
// hook every container in this repo already fires, same mechanism
// pkg/opt/spawnpoint relies on for its own chests via that same script's
// WarnSpawnPoint) and pkg/std/lockpicking/use/picklock.src's PickTreasureChest
// (a legit lockpick roll, fired at unlock time as a speed optimization -
// picking is always followed by a destroy 30s later anyway, this just
// avoids waiting for it). Both call sites are safe to fire twice for the
// same chest (unlock now, destroy later) because RemoveAreaSpawnerSerial's
// own duplicate guard (see its "kept.size() == serials.size()" check) makes
// the second call a no-op - unlike pkg/opt/spawnpoint's spawndeath.src,
// which is NOT duplicate-safe, which is why that system only ever gets
// notified once, at actual destroy time (see canDestroy.src).
//
// No-ops harmlessly for any chest this package didn't spawn (a spawnpoint
// chest, a player's own locked container, etc.) - those never have
// PROPID_MOBILE_AREASPAWNER set, so area_info comes back falsy.
function NotifyAreaSpawnerChestGone(chest)
	var area_info := GetObjProperty(chest, PROPID_MOBILE_AREASPAWNER);
	if( !area_info )
		return 0;
	endif
	return RemoveAreaSpawnerSerial(area_info, chest.serial);
endfunction
