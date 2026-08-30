// The paperdoll quest journal -- one place every quest's progress is
// tracked, opened from scripts/misc/questbutton.src. Reads purely from
// QuestState via queststate.inc, so it needs no per-quest code.
use uo;
use os;
use cfgfile;

include ":mdgumps:gumps_ex";
include ":questpkg:queststate";
include ":questpkg:questpkg_gumpstyle";

const QJ_BTN_CLOSE := 0;
const QJ_BTN_STOPTRACK := 1;
const QJ_TRACK_OFFSET := 1000;
const QJ_ABANDON_OFFSET := 2000;

function ShowQuestJournal(who)

	Print("[QuestPkg] ShowQuestJournal: opening for "+who.name);

	while(1)
		var active_ids := QP_GetActiveQuestIds(who);
		var completed_ids := QP_GetCompletedQuestIds(who);

		var gump := GFCreateGump();
		GFPage(gump, 0);
		QuestPkg_DrawOsiFrame(gump);
		GFTextLine(gump, 130, 48, 1152, "Quest Journal");

		var y := 90;

		if(!active_ids.size() && !completed_ids.size())
			GFTextLine(gump, 95, y, 999, "You have no active quests.");
			y := y + 25;
		endif

		var trackable := dictionary;
		var abandonable := dictionary;

		foreach questid in active_ids
			var elem := QP_GetQuestElem(questid);
			if(elem.errortext)
				continue;
			endif

			var quest_label := GetConfigString(elem, "Name");
			if(QP_IsQuestReadyToTurnIn(who, questid))
				quest_label := quest_label + " (ready to turn in)";
			endif
			GFTextLine(gump, 95, y, 1152, quest_label);
			y := y + 20;

			var state := QP_GetState(who);
			var qstate := state[CInt(questid)];
			var objectives := qstate["Objectives"];

			var i := 1;
			while(GetConfigString(elem, "ObjText"+i))
				var need := CInt(GetConfigString(elem, "ObjCount"+i));
				var current := CInt(objectives[i]);
				GFTextLine(gump, 115, y, 999, GetConfigString(elem, "ObjText"+i)+" ("+CStr(current)+"/"+CStr(need)+")");
				y := y + 20;
				i := i + 1;
			endwhile

			if(QP_IsQuestReadyToTurnIn(who, questid))
				GFTextLine(gump, 115, y, 88, "Return to the quest giver to turn this in.");
				y := y + 20;
			endif

			if(GetConfigString(elem, "TrackX"))
				var retval := QJ_TRACK_OFFSET + CInt(questid);
				GFAddButton(gump, 115, y, 4005, 4007, GF_CLOSE_BTN, retval);
				GFTextLine(gump, 150, y+2, 999, "Track");
				trackable[retval] := CInt(questid);
				y := y + 25;
			endif

			var abandon_retval := QJ_ABANDON_OFFSET + CInt(questid);
			GFAddButton(gump, 115, y, 4005, 4007, GF_CLOSE_BTN, abandon_retval);
			GFTextLine(gump, 150, y+2, 33, "Abandon");
			abandonable[abandon_retval] := CInt(questid);
			y := y + 25;

			y := y + 10;
		endforeach

		foreach questid in completed_ids
			var elem := QP_GetQuestElem(questid);
			if(elem.errortext)
				continue;
			endif
			GFTextLine(gump, 95, y, 88, GetConfigString(elem, "Name")+" (completed)");
			y := y + 20;
		endforeach

		GFAddButton(gump, 95, y+15, 4005, 4007, GF_CLOSE_BTN, QJ_BTN_STOPTRACK);
		GFTextLine(gump, 130, y+17, 999, "Stop Tracking");

		GFAddButton(gump, 310, 70, 12012, 12014, GF_CLOSE_BTN, QJ_BTN_CLOSE);

		var data := GFSendGump(who, gump);
		if(!data || data[0] == QJ_BTN_CLOSE)
			return 0;
		endif

		if(data[0] == QJ_BTN_STOPTRACK)
			SendQuestArrow(who, -1, -1);
			continue;
		endif

		var abandon_questid := abandonable[data[0]];
		if(abandon_questid)
			QP_AbandonQuest(who, abandon_questid);
			continue;
		endif

		var tracked_questid := trackable[data[0]];
		if(tracked_questid)
			var elem := QP_GetQuestElem(tracked_questid);
			SendQuestArrow(who, CInt(GetConfigString(elem, "TrackX")), CInt(GetConfigString(elem, "TrackY")));
		endif
	endwhile

endfunction
