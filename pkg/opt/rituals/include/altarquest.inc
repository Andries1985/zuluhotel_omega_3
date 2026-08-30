// Ritual altar boss kills. A boss spawned by a ritual altar (see
// pkg/opt/rituals/altar/gump.inc, RitualAltar_Summon) is tagged with
// "QuestTrophyObjtype"/"QuestTrophyColor" (the latter = that ritual's own
// RITUAL_ChantBookColor, so the trophy always matches its reward book's tint)
// at spawn time, separately from the generic "QuestId"/"QuestObjIndex" tags
// TagNpcForQuest() already sets for the quest-objective advance
// (pkg/opt/questpkg/include/questdeath.inc). This drops a proof-of-kill
// trophy on the corpse -- NOT the actual chant book. The real reward comes
// from the quest's RewardItem (quests.cfg) on turn-in with Valthor, so
// killing the boss alone can't shortcut the trip back.
//
// The trophy objtype is generic/shared across every future altar boss (see
// Item 0xBA3F, pkg/opt/rituals/config/itemdesc.cfg), so the dropped item is
// also stamped with a "QuestId" CProp matching the corpse's own quest tag --
// that's what QP_FindTurnInItem() (pkg/opt/questpkg/include/queststate.inc)
// actually checks at turn-in, not just the objtype. Without it, a skull from
// an easy boss could otherwise be turned in for a harder boss's quest.
use uo;
use os;

function ProcessRitualBossTrophyDrop(corpse)

	var trophy_objtype := GetObjProperty(corpse, "QuestTrophyObjtype");
	if(!trophy_objtype)
		return 0;
	endif

	var trophy_name := GetObjProperty(corpse, "QuestTrophyName");
	var trophy_color := GetObjProperty(corpse, "QuestTrophyColor");
	var questid := GetObjProperty(corpse, "QuestId");

	Print("[RitualAltar] ProcessRitualBossTrophyDrop: corpse "+CStr(corpse.serial)+" tagged for trophy objtype 0x"+Hex(CInt(trophy_objtype)));

	// Cleared before creating the trophy so a duplicate death notification
	// (WarnManagerOfNpcDeath is known to sometimes fire twice per kill) can't
	// mint a second copy.
	EraseObjProperty(corpse, "QuestTrophyObjtype");
	EraseObjProperty(corpse, "QuestTrophyName");
	EraseObjProperty(corpse, "QuestTrophyColor");

	var trophy := CreateItemInContainer(corpse, CInt(trophy_objtype));
	if(!trophy)
		return 0;
	endif

	if(trophy_name)
		SetName(trophy, "Skull of "+trophy_name);
	endif
	if(trophy_color)
		trophy.color := CInt(trophy_color);
	endif
	if(questid)
		SetObjProperty(trophy, "QuestId", CInt(questid));
	endif

	return 1;
endfunction
