// Catch-side quest-objective hook. Parallel to questdeath.inc's kill hook, but evaluated
// against the catching player's own active quests instead of a pre-tagged spawn -- there's
// no NPC instance to tag in advance, since every cast/bob is a fresh random roll from
// catchtable.cfg/crustaceantable.cfg rather than a specific placed mob.
//
// A quest objective opts into this by setting ObjKind<n> to "CatchFish" or "CatchCrustacean"
// in quests.cfg (see the field docs at the top of that file), instead of being a kill quest.
use uo;
use os;

include ":questpkg:queststate";

// kind must be "CatchFish" or "CatchCrustacean", matching ObjKind<n> exactly.
// tier is "Regular"/"Rare"/"Legendary". weight is 0 unless a TrophyWeight was actually
// rolled for this catch (only Rare/Legendary catches ever roll one).
function QP_ProcessFishCatch(character, kind, tier, weight := 0)

	var ids := QP_GetActiveQuestIds(character);
	if(!len(ids))
		return 0;
	endif

	var cfg := QP_GetQuestsCfg();

	Print("[QuestFishing] "+character.name+" kind="+kind+" tier="+tier+" weight="+CStr(weight)+" checking "+CStr(len(ids))+" active quest(s)");

	foreach questid in ids
		var elem := QP_GetQuestElem(questid, cfg);
		if(elem.errortext)
			continue;
		endif

		var obj_total := QP_GetObjectiveCount(elem);
		var i;
		for(i := 1; i <= obj_total; i := i + 1)
			var obj_kind := GetConfigString(elem, "ObjKind"+i);
			if(obj_kind == kind)
				var obj_tier := GetConfigString(elem, "ObjTier"+i);
				var obj_min_weight := CInt(GetConfigString(elem, "ObjMinWeight"+i));
				var tier_ok := (!obj_tier or obj_tier == "Any" or obj_tier == tier);
				var weight_ok := (!obj_min_weight or weight >= obj_min_weight);

				if(tier_ok and weight_ok)
					Print("[QuestFishing] advancing quest "+CStr(questid)+" objective "+CStr(i)+" for "+character.name);
					QP_AdvanceObjective(character, questid, i);
				endif
			endif
		endfor
	endforeach

	return 1;

endfunction
