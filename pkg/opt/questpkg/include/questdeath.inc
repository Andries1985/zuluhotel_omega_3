// Generic "an NPC that mattered to a quest just died" hook. Called directly
// from scripts/misc/death.src's npcdeath program, the same way
// pkg/opt/champspawns/include/death.inc's ProcessChampionCorpse is already
// wired in there -- a boss/mob only needs the two tag CProps set at spawn
// time (TagNpcForQuest), never a bespoke per-NPC death script.
use uo;
use os;

include ":questpkg:queststate";

const QUESTPKG_TAG_QUESTID := "QuestId";
const QUESTPKG_TAG_OBJIDX := "QuestObjIndex";

function TagNpcForQuest(npc, questid, obj_index)

	SetObjProperty(npc, QUESTPKG_TAG_QUESTID, CInt(questid));
	SetObjProperty(npc, QUESTPKG_TAG_OBJIDX, CInt(obj_index));

	return 1;
endfunction

function ProcessQuestKill(corpse)

	var questid := CInt(GetObjProperty(corpse, QUESTPKG_TAG_QUESTID));
	if(!questid)
		return 0;
	endif

	Print("[QuestPkg] ProcessQuestKill: corpse "+CStr(corpse.serial)+" tagged for quest "+CStr(questid));

	var obj_index := CInt(GetObjProperty(corpse, QUESTPKG_TAG_OBJIDX));
	if(!obj_index)
		Print("[QuestPkg] ProcessQuestKill: no QuestObjIndex tag on corpse, aborting");
		return 0;
	endif

	var killer_serial := GetObjProperty(corpse, "KilledBySerial");
	if(!killer_serial)
		Print("[QuestPkg] ProcessQuestKill: no KilledBySerial on corpse, aborting");
		return 0;
	endif

	var killer := SystemFindObjectBySerial(killer_serial, SYSFIND_SEARCH_OFFLINE_MOBILES);
	if(!killer)
		Print("[QuestPkg] ProcessQuestKill: killer serial "+CStr(killer_serial)+" not found, aborting");
		return 0;
	endif

	Print("[QuestPkg] ProcessQuestKill: advancing quest "+CStr(questid)+" objective "+CStr(obj_index)+" for "+killer.name);
	QP_AdvanceObjective(killer, questid, obj_index);

	return 1;
endfunction
