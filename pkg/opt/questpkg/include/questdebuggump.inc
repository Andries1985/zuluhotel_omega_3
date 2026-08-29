// GM/test tool: view a character's status on every quest in quests.cfg, and
// reset any of them, so testers can retry quest content (e.g. Quest 2's
// altar/turn-in flow, which is otherwise non-repeatable) without needing a
// fresh character each time. See pkg/opt/questpkg/textcmd/test/queststate.src.
use uo;
use os;
use cfgfile;

include ":mdgumps:gumps_ex";
include ":questpkg:queststate";
include ":questpkg:questpkg_gumpstyle";

const QD_BTN_CLOSE := 0;
const QD_RESET_OFFSET := 1000;

function ShowQuestDebugGump(gm, subject)

	while(1)
		var cfg := QP_GetQuestsCfg();
		var ids := GetConfigIntKeys(cfg);

		var gump := GFCreateGump();
		GFPage(gump, 0);
		QuestPkg_DrawOsiFrame(gump);
		GFTextLine(gump, 130, 48, 1152, "Quest State: "+subject.name);

		var y := 90;
		var resettable := dictionary;

		foreach questid in ids
			var elem := QP_GetQuestElem(questid, cfg);
			if(elem.errortext)
				continue;
			endif

			var status := "Not started";
			if(QP_IsQuestCompleted(subject, questid))
				status := "Completed";
			elseif(QP_IsQuestReadyToTurnIn(subject, questid))
				status := "Ready to turn in";
			elseif(QP_IsQuestActive(subject, questid))
				status := "In progress";
			endif

			GFTextLine(gump, 98, y, 999, CStr(questid)+": "+GetConfigString(elem, "Name")+" ("+status+")");
			y := y + 20;

			var has_state := QP_IsQuestActive(subject, questid) || QP_IsQuestCompleted(subject, questid);
			if(has_state)
				var state := QP_GetState(subject);
				var qstate := state[CInt(questid)];
				var objectives := qstate["Objectives"];

				var i := 1;
				while(GetConfigString(elem, "ObjText"+i))
					var need := CInt(GetConfigString(elem, "ObjCount"+i));
					var current := CInt(objectives[i]);
					GFTextLine(gump, 115, y, 88, GetConfigString(elem, "ObjText"+i)+" ("+CStr(current)+"/"+CStr(need)+")");
					y := y + 18;
					i := i + 1;
				endwhile

				var retval := QD_RESET_OFFSET + CInt(questid);
				GFAddButton(gump, 115, y, 4005, 4007, GF_CLOSE_BTN, retval);
				GFTextLine(gump, 150, y+2, 33, "Reset");
				resettable[retval] := CInt(questid);
				y := y + 22;
			endif

			y := y + 10;
		endforeach

		GFAddButton(gump, 310, 70, 12012, 12014, GF_CLOSE_BTN, QD_BTN_CLOSE);

		var data := GFSendGump(gm, gump);
		if(!data || data[0] == QD_BTN_CLOSE)
			return 0;
		endif

		var reset_questid := resettable[data[0]];
		if(reset_questid)
			QP_ResetQuest(subject, reset_questid);
		endif
	endwhile

endfunction
