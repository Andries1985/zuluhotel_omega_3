// GM tool: list every quest in quests.cfg with its approximate world
// location and a Go button that teleports the GM straight there -- for
// quickly finding/returning to where a quest's content (altars, givers)
// needs to be set up.
//
// Locations are found LIVE every time this gump opens, not read from a
// hand-maintained config field -- an altar already knows which quest it
// belongs to via its own RitualAltarQuestId CProp, and a quest-giver NPC
// already knows via its own QuestGiverTag CProp (see
// pkg/opt/questpkg/include/questspawncheck.inc for both lookups). So if an
// altar gets destroyed, it just drops off this list on the next open; place
// a fresh one anywhere and it shows back up -- nothing to edit here or in
// quests.cfg either way.
//
// See pkg/opt/questpkg/textcmd/gm/questmap.src.
use uo;
use os;
use cfgfile;
use polsys;

include ":mdgumps:gumps_ex";
include ":questpkg:queststate";
include ":questpkg:questpkg_gumpstyle";
include ":questpkg:questspawncheck";

const QM_BTN_CLOSE := 0;
const QM_BTN_REFRESH := 1;
const QM_GO_OFFSET := 1000;

// Realms() returns each realm as a struct keyed by mapid, not name -- same
// hand-rolled mapid->name table already duplicated in
// pkg/opt/alryc/textcmd/test/testadminpanel.src and gotomulti.src, since
// there's no direct .name field to read instead.
function QM_RealmNameFromMapId(mapid)

	case(CInt(mapid))
		0: return "britannia";
		1: return "britannia_alt";
		2: return "ilshenar";
		3: return "malas";
		4: return "tokuno";
		5: return "termur";
		default: return 0;
	endcase

endfunction

// First live item of any of `objtypes` found in any realm, or 0.
function QM_FindLiveAltarLocation(objtypes)

	foreach objtype in objtypes
		foreach realm in Realms()
			var realm_name := QM_RealmNameFromMapId(realm.mapid);
			if(!realm_name)
				continue;
			endif

			var found := ListItemsInBoxOfObjType(CInt(objtype), 0, 0, -128, CInt(realm.width)-1, CInt(realm.height)-1, 127, realm_name);
			if(found && found.size())
				var item := found[1];
				var loc := struct;
				loc.+x := item.x;
				loc.+y := item.y;
				loc.+z := item.z;
				loc.+realm := realm_name;
				return loc;
			endif

			SleepMS(1);
		endforeach
	endforeach

	return 0;
endfunction

// Every live mobile anywhere carrying a QuestGiverTag CProp, keyed by that
// tag -- one pass across every realm rather than one scan per quest.
function QM_ScanLiveGivers()

	var result := dictionary;

	foreach realm in Realms()
		var realm_name := QM_RealmNameFromMapId(realm.mapid);
		if(!realm_name)
			continue;
		endif

		var mobiles := ListMobilesInBox(0, 0, -127, CInt(realm.width)-1, CInt(realm.height)-1, 128, realm_name);
		foreach mobile in mobiles
			var tag := GetObjProperty(mobile, "QuestGiverTag");
			if(!tag || result[tag])
				continue;
			endif

			var loc := struct;
			loc.+x := mobile.x;
			loc.+y := mobile.y;
			loc.+z := mobile.z;
			loc.+realm := realm_name;
			result[tag] := loc;
		endforeach

		SleepMS(1);
	endforeach

	return result;
endfunction

function ShowQuestMapGump(gm)

	while(1)
		SendSysMessage(gm, "Scanning the world for quest content...", 3, 73);

		var altar_objtypes := DiscoverQuestAltarObjtypes();
		var giver_locations := QM_ScanLiveGivers();

		var cfg := QP_GetQuestsCfg();
		var ids := GetConfigIntKeys(cfg);

		var gump := GFCreateGump();
		GFPage(gump, 0);
		QuestPkg_DrawOsiFrame(gump);
		GFTextLine(gump, 130, 48, 1152, "Quest Locations");

		var y := 90;
		var goable := dictionary;
		var loc_cache := dictionary;

		foreach questid in ids
			var elem := QP_GetQuestElem(questid, cfg);
			if(elem.errortext)
				continue;
			endif

			GFTextLine(gump, 98, y, 999, CStr(questid)+": "+GetConfigString(elem, "Name")+" ("+GetConfigString(elem, "GiverName")+")");
			y := y + 18;

			var loc_text := GetConfigString(elem, "ObjText1");
			if(!loc_text)
				loc_text := "(no location hint)";
			endif
			GFTextLine(gump, 115, y, 88, loc_text);
			y := y + 18;

			// Prefer a live altar (the concrete summon site); fall back to
			// the giver NPC's own position if no altar is up yet.
			var loc := 0;
			var altar_types := altar_objtypes[CInt(questid)];
			if(altar_types)
				loc := QM_FindLiveAltarLocation(altar_types);
			endif
			if(!loc)
				var giver_tag := GetConfigString(elem, "Giver");
				if(giver_tag)
					loc := giver_locations[giver_tag];
				endif
			endif

			if(loc)
				var retval := QM_GO_OFFSET + CInt(questid);
				GFAddButton(gump, 115, y, 4005, 4007, GF_CLOSE_BTN, retval);
				GFTextLine(gump, 150, y+2, 33, "Go");
				goable[retval] := CInt(questid);
				loc_cache[CInt(questid)] := loc;
			else
				GFTextLine(gump, 115, y, 33, "(not placed yet)");
			endif

			y := y + 25;
		endforeach

		GFAddButton(gump, 200, 70, 4005, 4007, GF_CLOSE_BTN, QM_BTN_REFRESH);
		GFTextLine(gump, 225, 72, 999, "Refresh");
		GFAddButton(gump, 310, 70, 12012, 12014, GF_CLOSE_BTN, QM_BTN_CLOSE);

		var data := GFSendGump(gm, gump);
		if(!data || data[0] == QM_BTN_CLOSE)
			return 0;
		endif

		if(data[0] == QM_BTN_REFRESH)
			continue;
		endif

		var go_questid := goable[data[0]];
		if(go_questid)
			var loc := loc_cache[go_questid];
			var elem := QP_GetQuestElem(go_questid, cfg);
			MoveObjectToLocation(gm, CInt(loc.x), CInt(loc.y), CInt(loc.z), loc.realm, MOVEOBJECT_FORCELOCATION);
			SendSysMessage(gm, "Teleported to: "+GetConfigString(elem, "Name"), 3, 73);
		endif
	endwhile

endfunction
