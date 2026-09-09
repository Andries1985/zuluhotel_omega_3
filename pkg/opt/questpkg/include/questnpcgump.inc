// Per-NPC quest-giver interaction. Any quest-giver NPC's AI script just
// needs to call HandleQuestGiverSpeech(me, event) from its SYSEVENT_SPEECH
// case and set a "QuestGiverTag" CProp matching a quest's Giver field in
// quests.cfg -- the gump/offer/accept flow itself is fully generic.
use uo;
use os;
use cfgfile;

include ":mdgumps:gumps_ex";
include ":questpkg:queststate";
include ":questpkg:questpkg_gumpstyle";

const QNPC_BTN_CLOSE := 0;
const QNPC_BTN_ACCEPT := 1;
const QNPC_BTN_DECLINE := 2;
const QNPC_SELECT_OFFSET := 1000;

function HandleQuestGiverSpeech(me, event)

	if(event.text["quest"] || event.text["Quest"] || event.text["QUEST"])
		Print("[QuestPkg] HandleQuestGiverSpeech: "+event.source.name+" said 'quest' to "+me.name);
		TurnToward(event.source);
		ShowQuestGiverGump(me, event.source);
	endif

	return 1;
endfunction

function ShowQuestGiverGump(npc, who)

	var giver_tag := GetObjProperty(npc, "QuestGiverTag");
	if(!giver_tag)
		Print("[QuestPkg] ShowQuestGiverGump: "+npc.name+" has no QuestGiverTag CProp, nothing to offer");
		return 0;
	endif

	var cfg := QP_GetQuestsCfg();
	var ids := GetConfigIntKeys(cfg);

	while(1)
		var gump := GFCreateGump();
		GFPage(gump, 0);
		QuestPkg_DrawOsiFrame(gump);
		GFTextLine(gump, 130, 48, 1152, npc.name);

		var y := 90;
		var offered := 0;
		var selectable := dictionary;

		foreach questid in ids
			var elem := QP_GetQuestElem(questid, cfg);
			if(elem.errortext || GetConfigString(elem, "Giver") != giver_tag)
				continue;
			endif

			offered := 1;

			if(QP_IsQuestCompleted(who, questid) && !CInt(GetConfigString(elem, "Repeatable")))
				GFTextLine(gump, 95, y, 999, GetConfigString(elem, "Name")+" (completed)");
			elseif(!QP_IsQuestActive(who, questid) && !QP_IsQuestReadyToTurnIn(who, questid) && !QP_MeetsSkillGate(who, elem))
				GFTextLine(gump, 95, y, 999, GetConfigString(elem, "Name")+" (requires "+CStr(CInt(GetConfigString(elem, "MinSkill")))+" skill)");
			elseif(!QP_IsQuestActive(who, questid) && !QP_IsQuestReadyToTurnIn(who, questid) && QP_IsQuestOnCooldown(who, questid))
				GFTextLine(gump, 95, y, 999, GetConfigString(elem, "Name")+" (ready again in "+QP_FormatCooldown(QP_CooldownRemaining(who, questid))+")");
			else
				var retval := QNPC_SELECT_OFFSET + CInt(questid);
				GFAddButton(gump, 95, y, 4005, 4007, GF_CLOSE_BTN, retval);
				selectable[retval] := CInt(questid);

				var label := GetConfigString(elem, "Name");
				if(QP_IsQuestReadyToTurnIn(who, questid))
					label := label + " (ready to turn in)";
				elseif(QP_IsQuestActive(who, questid))
					label := label + " (in progress)";
				elseif(QP_IsQuestCompleted(who, questid))
					label := label + " (completed - repeatable)";
				endif
				GFTextLine(gump, 130, y+2, 1152, label);
			endif

			y := y + 30;
		endforeach

		if(!offered)
			GFTextLine(gump, 95, y, 999, "I have nothing for you right now.");
		endif

		GFAddButton(gump, 310, 70, 4017, 4019, GF_CLOSE_BTN, QNPC_BTN_CLOSE);

		var data := GFSendGump(who, gump);
		if(!data || data[0] == QNPC_BTN_CLOSE)
			return 0;
		endif

		var picked_questid := selectable[data[0]];
		if(!picked_questid)
			return 0;
		endif

		if(QP_IsQuestReadyToTurnIn(who, picked_questid))
			ShowQuestTurnInGump(who, picked_questid, cfg);
		elseif(QP_IsQuestActive(who, picked_questid))
			ShowQuestProgressGump(who, picked_questid, cfg);
		else
			ShowQuestOfferGump(who, picked_questid, cfg);
		endif
	endwhile

endfunction

function ShowQuestOfferGump(who, questid, byref cfg_file := 0)

	var elem := QP_GetQuestElem(questid, cfg_file);
	if(elem.errortext)
		return 0;
	endif

	var gump := GFCreateGump();
	GFPage(gump, 0);
	QuestPkg_DrawOsiFrame(gump);
	GFTextLine(gump, 130, 48, 1152, GetConfigString(elem, "Name"));

	var y := 90;
	GFDrawWrappedText(gump, 95, y, GFWordWrap(GetConfigString(elem, "Description"), 300), 999);

	y := y + 15;
	GFAddButton(gump, 95, y, 12000, 12002, GF_CLOSE_BTN, QNPC_BTN_ACCEPT);
	GFAddButton(gump, 313, y, 12018, 12020, GF_CLOSE_BTN, QNPC_BTN_DECLINE);

	var data := GFSendGump(who, gump);
	if(data && data[0] == QNPC_BTN_ACCEPT)
		Print("[QuestPkg] ShowQuestOfferGump: "+who.name+" accepted quest "+CStr(questid));
		QP_StartQuest(who, questid);
	else
		Print("[QuestPkg] ShowQuestOfferGump: "+who.name+" declined/closed quest "+CStr(questid)+" offer");
	endif

	return 1;
endfunction

function ShowQuestTurnInGump(who, questid, byref cfg_file := 0)

	var elem := QP_GetQuestElem(questid, cfg_file);
	if(elem.errortext)
		return 0;
	endif

	var gump := GFCreateGump();
	GFPage(gump, 0);
	QuestPkg_DrawOsiFrame(gump);
	GFTextLine(gump, 130, 48, 1152, GetConfigString(elem, "Name")+" (ready to turn in)");

	var y := 90;
	GFDrawWrappedText(gump, 95, y, GFWordWrap("You've completed every objective. Turn it in?", 300), 999);

	y := y + 15;
	GFAddButton(gump, 95, y, 12000, 12002, GF_CLOSE_BTN, QNPC_BTN_ACCEPT);
	GFAddButton(gump, 313, y, 12018, 12020, GF_CLOSE_BTN, QNPC_BTN_DECLINE);

	var data := GFSendGump(who, gump);
	if(data && data[0] == QNPC_BTN_ACCEPT)
		Print("[QuestPkg] ShowQuestTurnInGump: "+who.name+" turned in quest "+CStr(questid));
		QP_CompleteQuest(who, questid, elem);
	endif

	return 1;
endfunction

function ShowQuestProgressGump(who, questid, byref cfg_file := 0)

	var elem := QP_GetQuestElem(questid, cfg_file);
	if(elem.errortext)
		return 0;
	endif

	var state := QP_GetState(who);
	var qstate := state[CInt(questid)];
	var objectives := qstate["Objectives"];

	var gump := GFCreateGump();
	GFPage(gump, 0);
	QuestPkg_DrawOsiFrame(gump);
	GFTextLine(gump, 130, 48, 1152, GetConfigString(elem, "Name")+" (in progress)");

	var y := 90;
	var i := 1;
	while(GetConfigString(elem, "ObjText"+i))
		var need := CInt(GetConfigString(elem, "ObjCount"+i));
		var current := CInt(objectives[i]);
		GFTextLine(gump, 95, y, 999, GetConfigString(elem, "ObjText"+i)+" ("+CStr(current)+"/"+CStr(need)+")");
		y := y + 25;
		i := i + 1;
	endwhile

	GFAddButton(gump, 310, 70, 4017, 4019, GF_CLOSE_BTN, QNPC_BTN_CLOSE);

	GFSendGump(who, gump);

	return 1;
endfunction
