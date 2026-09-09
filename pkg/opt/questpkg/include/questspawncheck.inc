// Shared "is this quest content?" detection, used by:
//  - pkg/opt/spawnpoint/spawnpoint.src, to keep quest altars/givers on the
//    dedicated "Quest Item"/"Quest NPC" spawnpoint types instead of the
//    generic ones (so .questmap's live scan below can rely on them always
//    being self-describing, no matter which spawnpoint placed them).
//  - pkg/opt/questpkg/include/questmapgump.inc, to discover quest content
//    live in the world with no hand-maintained quest->objtype/template
//    registry -- an altar's own RitualAltarQuestId CProp and a giver's own
//    QuestGiverTag CProp already say which quest they belong to; this just
//    reads them back, before OR after the object exists in the world.
use uo;
use cfgfile;
use polsys;
use basic;

// The rituals package's own reserved itemdesc range -- every ritual altar
// objtype lives here (pkg/opt/rituals/config/itemdesc.cfg). Scanning this
// range with GetItemDescriptor is a cheap, local, in-memory lookup (not a
// world query), so doing all 256 on every .questmap open is fine.
const QUESTSPAWN_ALTAR_OBJTYPE_MIN := 0xBA00;
const QUESTSPAWN_ALTAR_OBJTYPE_MAX := 0xBAFF;

// Returns the quest id baked into an objtype's own RitualAltarQuestId
// CProp, or 0 if it isn't a recognized quest altar objtype at all.
function QuestAltarQuestIdForObjtype(objtype)

	var desc := GetItemDescriptor(CInt(objtype));
	if(!desc || desc.errortext || !desc.CProps)
		return 0;
	endif

	return CInt(desc.CProps["RitualAltarQuestId"]);
endfunction

// Same, but resolves a spawnpoint-style Template field (an item Name string
// or a raw hex/decimal objtype string) to an objtype first.
function QuestAltarQuestIdForTemplate(template)

	var objtype := CInt(template);
	if(!objtype)
		objtype := GetObjtypeByName(template);
	endif
	if(!objtype)
		return 0;
	endif

	return QuestAltarQuestIdForObjtype(objtype);
endfunction

// Returns the QuestGiverTag CProp value baked into an npctemplate NAME
// (config/npcdesc.cfg), or 0 if it isn't a recognized quest-giver template.
// Same CProp-array-scan shape as FindMerchantType() in
// scripts/include/mrcspawn.inc -- npcdesc CProp lines aren't individually
// addressable by GetConfigString, only as one "CProp" array of raw
// "<name> <typed value>" strings that has to be split and matched by hand.
function QuestGiverTagForTemplate(template)

	var npcdesccfg := ReadConfigFile(":*:npcdesc");
	var elem := FindConfigElem(npcdesccfg, Lower(template));
	if(elem.errortext)
		return 0;
	endif

	foreach thing in GetConfigStringArray(elem, "CProp")
		var split := SplitWords(thing);
		if(Len(split) >= 2 && split[1] == "QuestGiverTag")
			return split[2] - split[2][1]; // strip the "s" type prefix
		endif
	endforeach

	return 0;
endfunction

// Every ritual altar objtype currently registered, keyed by quest id (an
// altar can have ew/ns facing variants -- both map to the same quest id,
// so each quest's value is an array). Built by scanning the reserved
// objtype range live every time this is called, so a newly added ritual
// altar just shows up -- no registry to keep in sync by hand.
function DiscoverQuestAltarObjtypes()

	var result := dictionary;
	var objtype;
	for(objtype := QUESTSPAWN_ALTAR_OBJTYPE_MIN; objtype <= QUESTSPAWN_ALTAR_OBJTYPE_MAX; objtype := objtype + 1)
		var questid := QuestAltarQuestIdForObjtype(objtype);
		if(!questid)
			continue;
		endif

		var list := result[questid];
		if(!list)
			list := array;
		endif
		list.append(objtype);
		result[questid] := list;
	endfor

	return result;
endfunction
