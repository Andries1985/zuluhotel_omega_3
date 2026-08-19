use os;
use uo;
use cfgfile;
use datafile;
use util;

include "include/constants/propids";
include "include/constants/cfgfiles";
include "include/all";	// KillMobile
include ":areas:include/areapolicy";	// pulls in :datafile:datafile for DFOpenDataFile/DFFindElement/DFGetProp

const AREASPAWNER_CFG				:= ":areaspawner:areagroups";

const AREASPAWNER_ENTRY_MIN_CAP		:= 1;
const AREASPAWNER_ENTRY_MAX_CAP		:= 100;
const AREASPAWNER_PLACEMENT_MAX_TRIES		:= 20;
const AREASPAWNER_PLACEMENT_RETRY_DELAY	:= 100;	// ms between placement tries, keeps a bad box from hammering the shard

const AREASPAWNER_HOME_X			:= 6338;
const AREASPAWNER_HOME_Y			:= 1581;

const AREASPAWNER_DEBUG			:= 1;	// 1 = print debug to console, 0 = silent

function AreaSpawnerDebug(msg)
	if( AREASPAWNER_DEBUG )
		Print("[areaspawner] " + msg);
	endif
	return 1;
endfunction


function AreaSpawnerRealms()
	return array{ "britannia", "britannia_alt", "malas", "tokuno", "ilshenar", "termur" };
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


function GetAreaSpawnerDatafileName(realm)
	return ":areaspawner:areaspawner_"+Lower(realm);
endfunction


function OpenAreaSpawnerDataFile(realm, create := DF_CREATE)
	return DFOpenDataFile( GetAreaSpawnerDatafileName(realm), create );
endfunction


function GetAreaSpawnerNames(realm)
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE);
	if( !df || df == error )
		return array{};
	endif

	var names := DFGetElemNames(df);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm) );

	if( names == error )
		return array{};
	endif

	return names;
endfunction


function NewAreaSpawnerRecord(who)
	var record := struct;
	record.+enabled	:= 0;
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


function CreateAreaSpawner(realm, name, who)
	var df := OpenAreaSpawnerDataFile(realm, DF_CREATE);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_NO_CREATE);
	if( elem && elem != error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
		return 0;	// already exists
	endif

	elem := DFFindElement(df, name, DF_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
		return 0;
	endif

	elem.SetProp( "Data", NewAreaSpawnerRecord(who) );
	UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
	AreaSpawnerDebug("created '"+name+"' ("+realm+")"+(who ? " by "+who.name : ""));
	return 1;
endfunction


function LoadAreaSpawner(realm, name)
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_NO_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
		return 0;
	endif

	var record := DFGetProp(elem, "Data", DF_NO_CREATE);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm) );

	if( record == error )
		return 0;
	endif

	return record;
endfunction


function SaveAreaSpawner(realm, name, record, who)
	var df := OpenAreaSpawnerDataFile(realm, DF_CREATE);
	if( !df || df == error )
		return 0;
	endif

	var elem := DFFindElement(df, name, DF_CREATE);
	if( !elem || elem == error )
		UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
		return 0;
	endif

	record.updated_at := ReadGameClock();
	if( who )
		record.updated_by := who.name;
	endif

	elem.SetProp( "Data", record );
	UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
	return 1;
endfunction


function DeleteAreaSpawnerRecord(realm, name)
	var df := OpenAreaSpawnerDataFile(realm, DF_NO_CREATE);
	if( !df || df == error )
		return 0;
	endif

	df.DeleteElement(name);
	UnloadDataFile( GetAreaSpawnerDatafileName(realm) );
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


function AddAreaSpawnerEntry(byref record, source, ref, min_count, max_count, frequency)
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


function UpdateAreaSpawnerEntry(byref record, entry_id, source, ref, min_count, max_count, frequency)
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
	if( entry.target < min_count )
		entry.target := min_count;
	elseif( entry.target > max_count )
		entry.target := max_count;
	endif

	record.entries[idx] := entry;
	return record;
endfunction


function DespawnAreaSpawnerEntryByIndex(byref record, idx)
	var serials := record.entries[idx].serials;
	foreach serial in serials
		var obj := SystemFindObjectBySerial(serial);
		if( obj )
			KillMobile(obj, 1);
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


function ResolveAreaSpawnerTemplate(entry)
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

	return CStr(entry.ref);
endfunction


function IsAreaSpawnerTemplateForbidden(template)
	var forbidden := GetGlobalProperty( PROPID_GLOBAL_SPAWNPOINTS_FORBIDDEN_TEMPLATES );
	if( !forbidden )
		return 0;
	endif
	return (Lower(template) in forbidden);
endfunction


function TagAreaSpawnerCritter(critter, realm, name, entry_id)
	var info := struct;
	info.+name	:= name;
	info.+realm	:= realm;
	info.+entry_id	:= entry_id;
	SetObjProperty( critter, PROPID_MOBILE_AREASPAWNER, info );
	SetObjProperty( critter, PROPID_MOBILE_SELF_SERIAL, critter.serial );
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
function TryPlaceAreaSpawnerCritter(realm, box, template)
	var critter := CreateNpcFromTemplate( template, AREASPAWNER_HOME_X, AREASPAWNER_HOME_Y, 0, 0, "britannia_alt" );
	if( !critter )
		AreaSpawnerDebug("CreateNpcFromTemplate FAILED for template '"+template+"' - bad/missing npc template?");
		return 0;
	endif
	critter.hidden := 1;

	var tries := 0;
	var placed := 0;
	var x;
	var y;
	var z;
	var standing;

	while( tries < AREASPAWNER_PLACEMENT_MAX_TRIES && !placed )
		tries := tries + 1;
		x := box.minx + RandomInt(box.maxx - box.minx + 1);
		y := box.miny + RandomInt(box.maxy - box.miny + 1);
		z := CInt( GetWorldHeight(x, y, realm) );
		standing := GetStandingHeight(x, y, z, realm);

		if( standing && !standing.multi )
			if( MoveObjectToLocation( critter, x, y, z, realm, MOVEOBJECT_FORCELOCATION ) )
				placed := 1;
			endif
		endif

		if( !placed )
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endif
	endwhile

	if( !placed )
		AreaSpawnerDebug("placement FAILED for template '"+template+"' after "+CStr(tries)+" tries, realm="+realm);
		KillMobile(critter, 1);
		return 0;
	endif

	AreaSpawnerDebug("placed '"+template+"' at "+CStr(x)+","+CStr(y)+" ("+realm+") after "+CStr(tries)+" tries");
	RestartScript(critter);
	critter.hidden := 0;
	return critter;
endfunction


function TickAreaSpawnerEntry(realm, name, entry, box)
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

	var template := ResolveAreaSpawnerTemplate(entry);
	if( !template || IsAreaSpawnerTemplateForbidden(template) )
		AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" ("+entry.source+":"+CStr(entry.ref)+") could not resolve a template - forbidden or missing group/npc");
		entry.last_fail_time := clock;
		entry.next_spawn_time := clock + entry.frequency;
		return entry;
	endif

	var critter := TryPlaceAreaSpawnerCritter(realm, box, template);
	if( !critter )
		entry.last_fail_time := clock;
		entry.next_spawn_time := clock + entry.frequency;
		return entry;
	endif

	TagAreaSpawnerCritter(critter, realm, name, entry.id);
	entry.serials.append(critter.serial);
	entry.next_spawn_time := clock + entry.frequency;
	AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" now "+CStr(entry.serials.size())+"/"+CStr(entry.target));

	return entry;
endfunction


function TickAreaSpawner(realm, name, record)
	if( !record.enabled )
		AreaSpawnerDebug("'"+name+"' ("+realm+") not Enabled - skipping tick");
		return record;
	endif

	var box := ResolveAreaSpawnerBox(realm, record);
	box := ClampBoxToRealm(realm, box);
	if( box.minx > box.maxx || box.miny > box.maxy )
		AreaSpawnerDebug("'"+name+"' ("+realm+") box is invalid after realm-bounds clamp - skipping tick");
		return record;
	endif

	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		record.entries[i] := TickAreaSpawnerEntry(realm, name, record.entries[i], box);
	endfor

	return record;
endfunction


function DespawnAllAreaSpawner(record)
	var i;
	for( i := 1; i <= record.entries.size(); i := i + 1 )
		var serials := record.entries[i].serials;
		foreach serial in serials
			var obj := SystemFindObjectBySerial(serial);
			if( obj )
				KillMobile(obj, 1);
			endif
		endforeach
		record.entries[i].serials := array{};
	endfor
	return record;
endfunction


// Deliberately ignores record.enabled - Full Respawn is a manual, one-shot GM
// action, not the passive automatic tick, so it shouldn't be silently gated by
// the same toggle that governs whether the manager keeps this spawner topped up.
function FullRespawnAreaSpawner(realm, name)
	AreaSpawnerDebug("full respawn starting for '"+name+"' ("+realm+")");
	var record := LoadAreaSpawner(realm, name);
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

	SaveAreaSpawner(realm, name, record, 0);

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

	for( i := 1; i <= record.entries.size(); i := i + 1 )
		var entry := record.entries[i];
		AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" ("+entry.source+":"+CStr(entry.ref)+") filling toward target="+CStr(entry.target));
		while( entry.serials.size() < entry.target )
			var template := ResolveAreaSpawnerTemplate(entry);
			if( !template || IsAreaSpawnerTemplateForbidden(template) )
				AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" giving up - could not resolve a template for source='"+entry.source+"' ref='"+CStr(entry.ref)+"' (missing group/npc in areagroups.cfg, or forbidden)");
				entry.last_fail_time := ReadGameClock();
				break;
			endif

			var critter := TryPlaceAreaSpawnerCritter(realm, box, template);
			if( !critter )
				AreaSpawnerDebug("'"+name+"' entry #"+CStr(entry.id)+" giving up - could not place '"+template+"' anywhere in the box");
				entry.last_fail_time := ReadGameClock();
				break;
			endif

			TagAreaSpawnerCritter(critter, realm, name, entry.id);
			entry.serials.append(critter.serial);
			SleepMs(AREASPAWNER_PLACEMENT_RETRY_DELAY);
		endwhile
		entry.next_spawn_time := ReadGameClock() + entry.frequency;
		record.entries[i] := entry;
	endfor

	SaveAreaSpawner(realm, name, record, 0);
	AreaSpawnerDebug("full respawn complete for '"+name+"' ("+realm+"): "+AreaSpawnerPopulationSummary(record));
	return 1;
endfunction


function DeleteAreaSpawner(realm, name)
	AreaSpawnerDebug("deleting '"+name+"' ("+realm+")");
	var record := LoadAreaSpawner(realm, name);
	if( record )
		record := DespawnAllAreaSpawner(record);
		SaveAreaSpawner(realm, name, record, 0);
	endif
	return DeleteAreaSpawnerRecord(realm, name);
endfunction


function RemoveAreaSpawnerSerial(area_info, mobile_serial)
	if( !area_info || !area_info.name || !area_info.realm )
		return 0;
	endif

	var record := LoadAreaSpawner(area_info.realm, area_info.name);
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
	record.entries[idx].serials := kept;

	AreaSpawnerDebug("death report: '"+CStr(area_info.name)+"' entry #"+CStr(area_info.entry_id)+" lost serial "+CStr(mobile_serial)+", "+CStr(kept.size())+" remain");
	SaveAreaSpawner(area_info.realm, area_info.name, record, 0);
	return 1;
endfunction
