// Generic quest state storage + quest-definition reads, shared by the
// journal gump, the quest-giver gump, and the death hook. Any future quest
// (not just chant books) reads/writes state through these functions instead
// of touching storage directly, so the storage shape can change in one place.
//
// Stored in a datafile (data/ds/questpkg/), keyed by character serial, rather
// than as a CProp on the character object.
use uo;
use os;
use cfgfile;
use datafile;

include "include/objtype";

const QUESTPKG_DATAFILE := ":questpkg:queststate";
const QUESTPKG_STATE_PROP := "QuestState";

function QP_OpenDataFile()

	var df := OpenDataFile(QUESTPKG_DATAFILE);
	if(!df)
		CreateDataFile(QUESTPKG_DATAFILE, flags := DF_KEYTYPE_STRING);
		df := OpenDataFile(QUESTPKG_DATAFILE);
	endif

	return df;
endfunction

function QP_GetState(who)

	var state := dictionary;

	var df := QP_OpenDataFile();
	if(!df)
		Print("[QuestPkg] QP_GetState: could not open/create "+QUESTPKG_DATAFILE);
		return state;
	endif

	var elem := df.FindElement(CStr(who.serial));
	if(elem)
		var saved := elem.GetProp(QUESTPKG_STATE_PROP);
		if(saved)
			state := saved;
		endif
	endif

	UnloadDataFile(QUESTPKG_DATAFILE);
	return state;
endfunction

function QP_SaveState(who, state)

	var df := QP_OpenDataFile();
	if(!df)
		Print("[QuestPkg] QP_SaveState: could not open/create "+QUESTPKG_DATAFILE);
		return 0;
	endif

	var key := CStr(who.serial);
	var elem := df.FindElement(key);
	if(!elem)
		elem := df.CreateElement(key);
	endif
	if(!elem)
		Print("[QuestPkg] QP_SaveState: could not create datafile element for "+key);
		UnloadDataFile(QUESTPKG_DATAFILE);
		return 0;
	endif

	elem.SetProp(QUESTPKG_STATE_PROP, state);

	UnloadDataFile(QUESTPKG_DATAFILE);
	return 1;
endfunction

// Quest id ranges (all quests share this one file/namespace, keep these in sync as new
// batches get added -- a repeated integer key silently shadows the earlier one):
//   1-99    existing/core (ritual quests currently occupy 2-5)
//   100-199 fishing (100-103 fish line, 104-107 crustacean line)
//   200+    reserved for whatever's next
function QP_GetQuestsCfg()

	var cfg := ReadConfigFile(":questpkg:quests");
	if(cfg.errortext)
		SysLog("Error::QP_GetQuestsCfg() - Unable to open [:questpkg:quests.cfg] -->"+cfg.errortext);
	endif

	return cfg;
endfunction

// Optional per-quest skill gate: SkillId (a SKILLID_* constant's numeric value) + MinSkill.
// A quest with neither field set always passes. Used to decide whether a giver OFFERS a
// fresh (not yet started) quest -- an already-active or ready-to-turn-in quest ignores this,
// so a skill dropping mid-quest can't strand a player partway through.
function QP_MeetsSkillGate(who, elem)

	var skill_id := CInt(GetConfigString(elem, "SkillId"));
	var min_skill := CInt(GetConfigString(elem, "MinSkill"));

	if(!skill_id or !min_skill)
		return 1;
	endif

	return GetEffectiveSkill(who, skill_id) >= min_skill;
endfunction

// Optional per-quest repeat cooldown, in seconds, checked against the timestamp
// QP_CompleteQuest stamps on turn-in. Same plain-ObjProperty + ReadGameClock() convention
// already proven reliable across restarts by powerhour/the BOD daily cap -- deliberately NOT
// "#"-prefixed.
function QP_CooldownRemaining(who, questid)

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate or !qstate["LastCompleted"])
		return 0;
	endif

	var elem := QP_GetQuestElem(questid);
	var cooldown := CInt(GetConfigString(elem, "RepeatCooldown"));
	if(!cooldown)
		return 0;
	endif

	var remaining := (CInt(qstate["LastCompleted"]) + cooldown) - ReadGameClock();
	if(remaining < 0)
		return 0;
	endif

	return remaining;
endfunction

function QP_IsQuestOnCooldown(who, questid)
	return QP_CooldownRemaining(who, questid) > 0;
endfunction

// Renders a cooldown-remaining second count as a short, rounded-up unit string for the
// quest-giver gump ("ready again in 3h").
function QP_FormatCooldown(seconds)

	if(seconds >= 86400)
		return CStr(CInt(seconds/86400)+1)+"d";
	elseif(seconds >= 3600)
		return CStr(CInt(seconds/3600)+1)+"h";
	else
		return CStr(CInt(seconds/60)+1)+"m";
	endif

endfunction

function QP_GetQuestElem(questid, byref cfg_file := 0)

	if(!cfg_file)
		cfg_file := QP_GetQuestsCfg();
	endif

	var elem := cfg_file[CInt(questid)];
	if(elem.errortext)
		SysLog("Error::QP_GetQuestElem() - Unable to find quest ["+CStr(questid)+"] -->"+elem.errortext);
	endif

	return elem;
endfunction

// Self-terminating count of ObjText<n> fields, same convention as
// rituals.inc's Speech<n> loop.
function QP_GetObjectiveCount(elem)

	var i := 1;
	while(GetConfigString(elem, "ObjText"+i))
		i := i + 1;
	endwhile

	return i - 1;
endfunction

function QP_IsQuestActive(who, questid)

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate)
		return 0;
	endif

	return qstate["Started"] && !qstate["Completed"];
endfunction

function QP_IsQuestCompleted(who, questid)

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate)
		return 0;
	endif

	return CInt(qstate["Completed"]) == 1;
endfunction

function QP_StartQuest(who, questid)

	if(QP_IsQuestActive(who, questid))
		Print("[QuestPkg] QP_StartQuest: quest "+CStr(questid)+" already active for "+who.name+", ignoring");
		return 0;
	endif

	var elem := QP_GetQuestElem(questid);
	if(elem.errortext)
		Print("[QuestPkg] QP_StartQuest: quest "+CStr(questid)+" not found in quests.cfg -->"+elem.errortext);
		return 0;
	endif

	if(QP_IsQuestCompleted(who, questid) && !CInt(GetConfigString(elem, "Repeatable")))
		Print("[QuestPkg] QP_StartQuest: quest "+CStr(questid)+" already completed and not repeatable for "+who.name+", ignoring");
		return 0;
	endif

	if(QP_IsQuestOnCooldown(who, questid))
		Print("[QuestPkg] QP_StartQuest: quest "+CStr(questid)+" still on cooldown for "+who.name+", ignoring");
		SendSysMessage(who, "That's not ready again yet.", 3, 73);
		return 0;
	endif

	if(!QP_MeetsSkillGate(who, elem))
		Print("[QuestPkg] QP_StartQuest: quest "+CStr(questid)+" skill gate not met by "+who.name+", ignoring");
		SendSysMessage(who, "You're not skilled enough for that yet.", 3, 73);
		return 0;
	endif

	Print("[QuestPkg] QP_StartQuest: starting quest "+CStr(questid)+" for "+who.name);

	var qstate := dictionary;
	qstate["Started"] := 1;
	qstate["Completed"] := 0;

	var objectives := dictionary;
	var i := 1;
	while(GetConfigString(elem, "ObjText"+i))
		objectives[i] := 0;
		i := i + 1;
	endwhile
	qstate["Objectives"] := objectives;
	qstate["Flags"] := dictionary;

	var state := QP_GetState(who);
	state[CInt(questid)] := qstate;
	QP_SaveState(who, state);

	SendSysMessage(who, "Quest accepted: "+GetConfigString(elem, "Name"), 3, 73);

	return 1;
endfunction

// Resets a quest back to never-started -- for when something outside the
// player's control went wrong (the boss didn't spawn, someone else killed
// it and looted the trophy, etc.). Does NOT mark the quest Completed, so a
// non-repeatable quest can still be re-accepted from its giver afterward;
// this is the only way to retry one.
function QP_AbandonQuest(who, questid)

	if(!QP_IsQuestActive(who, questid))
		Print("[QuestPkg] QP_AbandonQuest: quest "+CStr(questid)+" not active for "+who.name+", ignoring");
		return 0;
	endif

	var elem := QP_GetQuestElem(questid);

	var state := QP_GetState(who);
	state[CInt(questid)] := 0;
	QP_SaveState(who, state);

	SendSysMessage(who, "Quest abandoned: "+GetConfigString(elem, "Name"), 3, 73);

	return 1;
endfunction

// GM/test tool reset -- unlike QP_AbandonQuest (player self-service, only
// works on a still-active quest), this resets a quest back to never-started
// regardless of its current state, including a fully Completed,
// non-repeatable quest. Used by .queststate (pkg/opt/questpkg/textcmd/test)
// so testers can retry quest content without needing a fresh character.
function QP_ResetQuest(who, questid)

	var elem := QP_GetQuestElem(questid);

	var state := QP_GetState(who);
	state[CInt(questid)] := 0;
	QP_SaveState(who, state);

	Print("[QuestPkg] QP_ResetQuest: reset quest "+CStr(questid)+" for "+who.name);
	if(!elem.errortext)
		SendSysMessage(who, "Quest reset: "+GetConfigString(elem, "Name"), 3, 73);
	endif

	return 1;
endfunction

// Generic per-quest, per-character one-shot flags (e.g. "already summoned
// this cycle's boss") -- reset automatically whenever the quest is (re)started
// via QP_StartQuest, so content scripts don't need their own separate
// datafile/CProp bookkeeping just to rate-limit something once per accept.
function QP_SetQuestFlag(who, questid, flagname, value := 1)

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate)
		return 0;
	endif

	var flags := qstate["Flags"];
	flags[flagname] := value;
	qstate["Flags"] := flags;
	state[CInt(questid)] := qstate;
	QP_SaveState(who, state);

	return 1;
endfunction

function QP_GetQuestFlag(who, questid, flagname)

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate)
		return 0;
	endif

	var flags := qstate["Flags"];
	return flags[flagname];
endfunction

// Bumps one objective's progress (default +1) if the quest is active on
// `who`. Idempotent against being called more than once for the same kill --
// once an objective's counter reaches its target, further calls are no-ops
// rather than over-counting, since WarnManagerOfNpcDeath-style hooks in this
// codebase are known to sometimes fire twice for the same death.
function QP_AdvanceObjective(who, questid, obj_index, amount := 1)

	if(!QP_IsQuestActive(who, questid))
		Print("[QuestPkg] QP_AdvanceObjective: quest "+CStr(questid)+" is not active for "+who.name+", ignoring");
		return 0;
	endif

	var elem := QP_GetQuestElem(questid);
	if(elem.errortext)
		Print("[QuestPkg] QP_AdvanceObjective: quest "+CStr(questid)+" not found in quests.cfg -->"+elem.errortext);
		return 0;
	endif

	var need := CInt(GetConfigString(elem, "ObjCount"+CInt(obj_index)));
	if(!need)
		Print("[QuestPkg] QP_AdvanceObjective: quest "+CStr(questid)+" has no ObjCount"+CStr(obj_index)+", ignoring");
		return 0;
	endif

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	var objectives := qstate["Objectives"];

	var current := CInt(objectives[CInt(obj_index)]);
	if(current >= need)
		Print("[QuestPkg] QP_AdvanceObjective: objective "+CStr(obj_index)+" on quest "+CStr(questid)+" already at target ("+CStr(current)+"/"+CStr(need)+"), no-op");
		return 0;
	endif

	current := current + amount;
	if(current > need)
		current := need;
	endif
	objectives[CInt(obj_index)] := current;
	qstate["Objectives"] := objectives;
	state[CInt(questid)] := qstate;
	QP_SaveState(who, state);

	Print("[QuestPkg] QP_AdvanceObjective: quest "+CStr(questid)+" objective "+CStr(obj_index)+" now "+CStr(current)+"/"+CStr(need)+" for "+who.name);
	SendSysMessage(who, "Quest progress: "+GetConfigString(elem, "ObjText"+CInt(obj_index))+" ("+CStr(current)+"/"+CStr(need)+")", 3, 73);

	var obj_total := QP_GetObjectiveCount(elem);
	var all_done := 1;
	var j;
	for(j := 1; j <= obj_total; j := j + 1)
		if(CInt(objectives[j]) < CInt(GetConfigString(elem, "ObjCount"+j)))
			all_done := 0;
			break;
		endif
	endfor

	if(all_done)
		Print("[QuestPkg] QP_AdvanceObjective: all objectives satisfied for quest "+CStr(questid)+", ready to turn in");
		SendSysMessage(who, "All objectives complete for \""+GetConfigString(elem, "Name")+"\" -- return to the quest giver to turn it in.", 3, 73);
	endif

	return 1;
endfunction

// Finds a "TurnInItem" (quests.cfg) the player is carrying that's actually
// tagged for THIS quest -- items of that objtype from a different quest
// (e.g. an easier altar's trophy sharing the same generic skull objtype)
// don't count, since only the "QuestId" CProp set at drop time (see
// pkg/opt/rituals/include/altarquest.inc) proves which quest it's for.
function QP_FindTurnInItem(who, questid, turnin_objtype)

	foreach item in EnumerateItemsInContainer(who.backpack)
		if(item.objtype == turnin_objtype && CInt(GetObjProperty(item, "QuestId")) == CInt(questid))
			return item;
		endif
	endforeach

	return 0;
endfunction

// True once every ObjCount<n> has been met AND (if the quest declares a
// TurnInItem) the player is still carrying the correctly-tagged item, but
// the quest hasn't been handed back to its giver yet (QP_CompleteQuest not
// yet called) -- the gate between "grinding objectives" and "turn it in" in
// the quest-giver gump.
function QP_IsQuestReadyToTurnIn(who, questid)

	if(!QP_IsQuestActive(who, questid))
		return 0;
	endif

	var elem := QP_GetQuestElem(questid);
	if(elem.errortext)
		return 0;
	endif

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	var objectives := qstate["Objectives"];

	var obj_total := QP_GetObjectiveCount(elem);
	var i;
	for(i := 1; i <= obj_total; i := i + 1)
		if(CInt(objectives[i]) < CInt(GetConfigString(elem, "ObjCount"+i)))
			return 0;
		endif
	endfor

	var turnin_objtype := CInt(GetConfigString(elem, "TurnInItem"));
	if(turnin_objtype && !QP_FindTurnInItem(who, questid, turnin_objtype))
		return 0;
	endif

	return 1;
endfunction

function QP_CompleteQuest(who, questid, byref elem_in := 0)

	var elem := elem_in;
	if(!elem)
		elem := QP_GetQuestElem(questid);
	endif
	if(elem.errortext)
		return 0;
	endif

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	if(!qstate || CInt(qstate["Completed"]) == 1)
		Print("[QuestPkg] QP_CompleteQuest: quest "+CStr(questid)+" not active or already completed for "+who.name+", ignoring");
		return 0;
	endif

	// Re-checked here (not just at QP_IsQuestReadyToTurnIn time) since this is
	// the actual point of no return -- if the player dropped/lost the tagged
	// item between opening the turn-in gump and clicking Accept, the quest
	// must not complete.
	var turnin_objtype := CInt(GetConfigString(elem, "TurnInItem"));
	var turnin_item := 0;
	if(turnin_objtype)
		turnin_item := QP_FindTurnInItem(who, questid, turnin_objtype);
		if(!turnin_item)
			SendSysMessage(who, "You don't have proof of that anymore.", 3, 73);
			return 0;
		endif
	endif

	Print("[QuestPkg] QP_CompleteQuest: completing quest "+CStr(questid)+" for "+who.name);
	qstate["Completed"] := 1;
	qstate["LastCompleted"] := ReadGameClock();
	state[CInt(questid)] := qstate;
	QP_SaveState(who, state);

	if(turnin_item)
		DestroyItem(turnin_item);
	endif

	var reward_gold := CInt(GetConfigString(elem, "RewardGold"));
	if(reward_gold)
		CreateItemInContainer(who.backpack, UOBJ_GOLD_COIN, reward_gold);
	endif

	var reward_item := CInt(GetConfigString(elem, "RewardItem"));
	if(reward_item)
		CreateItemInContainer(who.backpack, reward_item);
	endif

	SendSysMessage(who, "Quest complete: "+GetConfigString(elem, "Name"), 3, 73);

	var reward_text := GetConfigString(elem, "RewardText");
	if(reward_text)
		SendSysMessage(who, reward_text, 3, 73);
	endif

	return 1;
endfunction

// True if `npc` (identified by its "QuestGiverTag" CProp) offers at least one
// quest that `who` has not completed yet -- used to decide whether an idling
// quest giver should shout a reminder at a nearby player.
function QP_GiverHasUndoneQuest(npc, who)

	var giver_tag := GetObjProperty(npc, "QuestGiverTag");
	if(!giver_tag)
		return 0;
	endif

	var cfg := QP_GetQuestsCfg();
	var ids := GetConfigIntKeys(cfg);

	foreach questid in ids
		var elem := QP_GetQuestElem(questid, cfg);
		if(elem.errortext || GetConfigString(elem, "Giver") != giver_tag)
			continue;
		endif

		if(!QP_IsQuestCompleted(who, questid))
			return 1;
		endif
	endforeach

	return 0;
endfunction

function QP_GetActiveQuestIds(who)

	var state := QP_GetState(who);
	var ids := array;
	foreach questid in (state.keys())
		var qstate := state[questid];
		if(!qstate)
			continue;
		endif
		if(qstate["Started"] && !qstate["Completed"])
			ids.Append(CInt(questid));
		endif
	endforeach

	return ids;
endfunction

function QP_GetCompletedQuestIds(who)

	var state := QP_GetState(who);
	var ids := array;
	foreach questid in (state.keys())
		var qstate := state[questid];
		if(!qstate)
			continue;
		endif
		if(CInt(qstate["Completed"]) == 1)
			ids.Append(CInt(questid));
		endif
	endforeach

	return ids;
endfunction
