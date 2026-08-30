// Ritual altar reagent-check/summon gump. Framed with the shared OSI quest
// gump chrome (QuestPkg_DrawOsiFrame, pkg/opt/questpkg/include/questpkg_gumpstyle.inc)
// so it matches Valthor's own gumps. This engine has no drag-into-gump
// mechanic, so reagents are supplied via a target-cursor "Offer" flow
// (closes the gump, prompts a target, reopens) -- the same shape Bulk Order
// Deeds already use. Offered amounts are tracked per-player via the generic
// quest Flags mechanism (QP_SetQuestFlag/QP_GetQuestFlag,
// pkg/opt/questpkg/include/queststate.inc) -- the same one "AltarUsed"
// already uses -- so progress is naturally per-player and resets
// automatically on abandon/re-accept.
use uo;
use os;

include ":mdgumps:gumps_ex";
include ":questpkg:questpkg_gumpstyle";
include ":questpkg:questdeath";
include "include/objtype";
include "include/constants/locations";
include "../pkg/opt/rituals/include/chantbook";

const RA_GUMP_WRAP_WIDTH := 312;
const RA_BTN_CLOSE := 0;
const RA_BTN_SUMMON := 1;
const RA_OFFER_OFFSET := 100;

// Every ritual altar quest currently has exactly one kill objective; revisit
// if a future ritual ever needs more than one.
const RA_QUEST_OBJ_INDEX := 1;

// Small local name lookup instead of pulling in :omegacache: (which drags
// the OLD :gumps: package into scope and collides with :mdgumps:, already
// used here) -- add a case here as new reagents get used by future altars.
function RitualAltar_ReagentName(objtype)

	case(CInt(objtype))
		UOBJ_BLOOD_MOSS: return "Bloodmoss";
		UOBJ_PIG_IRON: return "Pig Iron";
		default: return "Hex 0x"+Hex(CInt(objtype));
	endcase

endfunction

function RitualAltar_ReagentCount(config_item)

	var i := 1;
	while(GetObjProperty(config_item, "RitualAltarReagent"+i))
		i := i + 1;
	endwhile

	return i - 1;
endfunction

function RitualAltar_OfferReagent(who, config_item, questid, slot)

	var required_objtype := CInt(GetObjProperty(config_item, "RitualAltarReagent"+slot));
	var required_amt := CInt(GetObjProperty(config_item, "RitualAltarReagentAmt"+slot));
	var offered_amt := CInt(QP_GetQuestFlag(who, questid, "Reagent"+slot+"Offered"));

	var still_needed := required_amt - offered_amt;
	if(still_needed <= 0)
		return 1;
	endif

	SendSysMessage(who, "Target the "+RitualAltar_ReagentName(required_objtype)+" to offer.", 3, 73);
	var target_item := Target(who);

	if(!target_item || !target_item.objtype)
		SendSysMessage(who, "Nothing offered.");
		return 0;
	endif

	if(target_item.objtype != required_objtype)
		SendSysMessage(who, "That's not what this altar needs.");
		return 0;
	endif

	if(target_item.container != who.backpack)
		SendSysMessage(who, "That must be in your own backpack.");
		return 0;
	endif

	var consume := CInt(target_item.amount);
	if(consume > still_needed)
		consume := still_needed;
	endif

	SubtractAmount(target_item, consume);
	offered_amt := offered_amt + consume;
	QP_SetQuestFlag(who, questid, "Reagent"+slot+"Offered", offered_amt);

	SendSysMessage(who, "Offered "+CStr(consume)+" "+RitualAltar_ReagentName(required_objtype)+" ("+CStr(offered_amt)+"/"+CStr(required_amt)+").", 3, 73);

	return 1;
endfunction

function RitualAltar_Summon(who, config_item, questid)

	var boss_template := GetObjProperty(config_item, "RitualAltarBossTemplate");
	var trophy_objtype := CInt(GetObjProperty(config_item, "RitualAltarTrophyObjtype"));

	SendSysMessage(who, "The altar drinks in the offering, and the ground begins to churn...", 3, 73);
	Print("[RitualAltar] "+who.name+" activated ritual altar "+CStr(config_item.serial)+" (quest "+CStr(questid)+")");

	// CreateNPCFromTemplate rejects the target tile outright if it considers it
	// an invalid spawn location (e.g. too tight against the altar/cemetery
	// geometry) -- same problem spawnpoint.inc's CreateSpawnPointNpc solves by
	// creating at the shard's known-safe DEFAULT_LOCATION_NPC_RELOCATION_* spot
	// (with MOVEOBJECT_FORCELOCATION) and then relocating the finished NPC to
	// the real spot, where MoveObjectToLocation's own FORCELOCATION flag is
	// more permissive than creation-time placement.
	var boss := CreateNPCFromTemplate(boss_template, DEFAULT_LOCATION_NPC_RELOCATION_X, DEFAULT_LOCATION_NPC_RELOCATION_Y, DEFAULT_LOCATION_NPC_RELOCATION_Z, 0, DEFAULT_LOCATION_NPC_RELOCATION_R, MOVEOBJECT_FORCELOCATION);
	if(!boss)
		SendSysMessage(who, "Something has gone wrong -- the altar's power fizzles.");
		Print("[RitualAltar] CreateNPCFromTemplate failed for "+boss_template);
		return 0;
	endif

	if(!MoveObjectToLocation(boss, config_item.x, config_item.y, config_item.z+5, config_item.realm, MOVEOBJECT_FORCELOCATION))
		Print("[RitualAltar] MoveObjectToLocation to altar site failed for "+boss_template+" (serial "+CStr(boss.serial)+") -- left at the default relocation spot");
		SendSysMessage(who, "The altar's power falters -- whatever answered the call couldn't take shape here.");
	endif

	TagNpcForQuest(boss, questid, RA_QUEST_OBJ_INDEX);
	SetObjProperty(boss, "QuestTrophyObjtype", trophy_objtype);
	SetObjProperty(boss, "QuestTrophyName", boss.name);
	SetObjProperty(boss, "QuestTrophyColor", RITUAL_ChantBookColor(GetObjProperty(config_item, "RitualAltarRitualId")));

	QP_SetQuestFlag(who, questid, "AltarUsed", 1);

	return 1;
endfunction

function ShowRitualAltarGump(who, config_item, questid)

	var ritualid := GetObjProperty(config_item, "RitualAltarRitualId");
	var total := RitualAltar_ReagentCount(config_item);

	while(1)
		var gump := GFCreateGump();
		GFPage(gump, 0);
		QuestPkg_DrawOsiFrame(gump);
		GFTextLine(gump, 130, 48, 1152, RITUAL_ChantBookTitle(ritualid));

		var y := 90;
		var flavor_text := RITUAL_ChantBookFlavor(ritualid);
		if(flavor_text)
			GFDrawWrappedText(gump, 98, y, GFWordWrap(flavor_text, RA_GUMP_WRAP_WIDTH), 90000);
			y := y + 10;
		endif

		var all_met := 1;
		var offerable := dictionary;

		var i;
		for(i := 1; i <= total; i := i + 1)
			var reagent_objtype := CInt(GetObjProperty(config_item, "RitualAltarReagent"+i));
			var reagent_need := CInt(GetObjProperty(config_item, "RitualAltarReagentAmt"+i));
			var reagent_have := CInt(QP_GetQuestFlag(who, questid, "Reagent"+i+"Offered"));

			GFTextLine(gump, 98, y, 999, RitualAltar_ReagentName(reagent_objtype)+": "+CStr(reagent_have)+"/"+CStr(reagent_need));

			if(reagent_have < reagent_need)
				all_met := 0;
				var retval := RA_OFFER_OFFSET + i;
				GFAddButton(gump, 300, y, 4005, 4007, GF_CLOSE_BTN, retval);
				GFTextLine(gump, 325, y+2, 999, "Offer");
				offerable[retval] := i;
			else
				GFTextLine(gump, 325, y+2, 88, "(complete)");
			endif

			y := y + 22;
		endfor

		y := y + 10;
		if(all_met)
			GFAddButton(gump, 98, y, 4005, 4007, GF_CLOSE_BTN, RA_BTN_SUMMON);
			GFTextLine(gump, 133, y+2, 1152, "Summon");
		else
			GFTextLine(gump, 98, y, 999, "Bring the required offerings, then use Offer to give them.");
		endif

		GFAddButton(gump, 310, 70, 12012, 12014, GF_CLOSE_BTN, RA_BTN_CLOSE);

		var data := GFSendGump(who, gump);
		if(!data || data[0] == RA_BTN_CLOSE)
			return 0;
		endif

		if(data[0] == RA_BTN_SUMMON)
			if(all_met)
				RitualAltar_Summon(who, config_item, questid);
			endif
			return 1;
		endif

		var offer_slot := offerable[data[0]];
		if(offer_slot)
			RitualAltar_OfferReagent(who, config_item, questid, offer_slot);
		endif
	endwhile

endfunction
