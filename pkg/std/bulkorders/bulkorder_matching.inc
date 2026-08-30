// Bulk Order Deeds -- matching: filling a Small BOD, combining a completed
// Small into a Large's child slot, and turning either in.

include ":bulkorders:bulkorder";
include ":bulkorders:bulkorder_ledger";
include "include/objtype";

use cfgfile;

// Attempts to fill one unit of a Small BOD's requested quantity with a
// single crafted item. Consumes the item on success.
function BOD_TryFill(byref deed, byref item)
	var result := struct;
	result.ok := 0;

	if( !deed or deed.objtype != UOBJ_BULKORDER_DEED )
		BOD_Debug("BOD_TryFill: target isn't a bulk order deed");
		result.reason := "That isn't a bulk order deed.";
		return result;
	endif

	if( CInt(BOD_GetSize(deed)) )
		BOD_Debug("BOD_TryFill: deed is Large, not Small -- rejecting");
		result.reason := "Large orders are filled by combining completed Small deeds, not raw items.";
		return result;
	endif

	if( BOD_IsComplete(deed) )
		BOD_Debug("BOD_TryFill: deed already complete");
		result.reason := "This order is already complete.";
		return result;
	endif

	var skillid := CInt(BOD_GetCraft(deed));
	var expected := CInt(BOD_ExpectedFillObjtype(skillid, CInt(BOD_GetItemObjtype(deed))));
	if( !item )
		BOD_Debug("BOD_TryFill: no item targeted");
		result.reason := "That isn't what this order calls for.";
		return result;
	elseif( item.objtype != expected )
		BOD_Debug("BOD_TryFill: item objtype " + Hex(item.objtype) + " != expected " + Hex(expected));
		result.reason := "That isn't what this order calls for.";
		return result;
	endif

	if( CInt(BOD_GetExceptional(deed)) and !GetObjProperty(item, "IsExceptional") )
		BOD_Debug("BOD_TryFill: deed requires exceptional, item isn't");
		result.reason := "This order requires an exceptional piece.";
		return result;
	endif

	// A Small BOD carries one recorded material (BODFilledMaterial), set from
	// whichever item fills it first -- every subsequent item must match, so
	// the reward's quality bonus stays well-defined for the whole order.
	var material := CInt(GetObjProperty(item, "CraftedWith"));
	var existing := BOD_GetFilledMaterial(deed);
	if( existing and CInt(existing) != material )
		BOD_Debug("BOD_TryFill: material mismatch, deed has " + CInt(existing) + " item is " + material);
		result.reason := "This order requires " + BOD_MaterialName(skillid, CInt(existing)) + ".";
		return result;
	endif
	if( !existing )
		SetObjProperty(deed, "BODFilledMaterial", material);
	endif

	var filled := CInt(BOD_GetAmountFilled(deed)) + 1;
	SetObjProperty(deed, "BODAmountFilled", filled);
	DestroyItem(item);
	BOD_Debug("BOD_TryFill: filled " + filled + "/" + CInt(BOD_GetAmount(deed)));

	result.ok := 1;
	result.filled := filled;
	result.amount := CInt(BOD_GetAmount(deed));
	return result;
endfunction

// Attempts to combine a completed Small BOD deed into one open slot of a
// Large BOD (exact item, exact material, exact quality -- OSI's real rule,
// minus quantity, which is deliberately dropped). Feeding a Small into a
// slot forfeits its own turn-in payout: that value folds into the Large's
// accrued Gold/Points instead, per the locked reward-floor rule.
function BOD_TryFillLargeSlot(byref large_deed, byref small_deed)
	var result := struct;
	result.ok := 0;

	if( !large_deed or large_deed.objtype != UOBJ_BULKORDER_DEED or !CInt(BOD_GetSize(large_deed)) )
		BOD_Debug("BOD_TryFillLargeSlot: target isn't a Large deed");
		result.reason := "That isn't a Large bulk order deed.";
		return result;
	endif
	if( !small_deed or small_deed.objtype != UOBJ_BULKORDER_DEED or CInt(BOD_GetSize(small_deed)) )
		BOD_Debug("BOD_TryFillLargeSlot: targeted item isn't a Small deed");
		result.reason := "That isn't a completed Small bulk order deed.";
		return result;
	endif
	if( BOD_IsComplete(large_deed) )
		BOD_Debug("BOD_TryFillLargeSlot: Large already complete");
		result.reason := "This order is already complete.";
		return result;
	endif
	if( !BOD_IsComplete(small_deed) )
		BOD_Debug("BOD_TryFillLargeSlot: Small isn't complete yet");
		result.reason := "That order isn't finished yet.";
		return result;
	endif

	var skillid := CInt(BOD_GetCraft(large_deed));
	if( CInt(BOD_GetCraft(small_deed)) != skillid )
		BOD_Debug("BOD_TryFillLargeSlot: craft mismatch, Large=" + BOD_CraftName(skillid) + " Small=" + BOD_CraftName(CInt(BOD_GetCraft(small_deed))));
		result.reason := "That deed is for a different craft.";
		return result;
	endif

	// The whole Large order shares one exceptional requirement across every
	// slot -- a normal Large accepts either quality, an exceptional-required
	// Large needs an exceptional Small.
	if( CInt(BOD_GetExceptional(large_deed)) and !CInt(BOD_GetExceptional(small_deed)) )
		BOD_Debug("BOD_TryFillLargeSlot: Large requires exceptional, Small isn't");
		result.reason := "This order requires exceptional pieces.";
		return result;
	endif

	var has_axis := BOD_HasMaterialAxis(skillid);
	if( has_axis and skillid != SKILLID_INSCRIPTION )
		if( CInt(BOD_GetFilledMaterial(small_deed)) != CInt(BOD_GetMaterial(large_deed)) )
			BOD_Debug("BOD_TryFillLargeSlot: material mismatch, Large wants " + Hex(CInt(BOD_GetMaterial(large_deed))) + " Small has " + Hex(CInt(BOD_GetFilledMaterial(small_deed))));
			result.reason := "That deed isn't made of the material this order calls for.";
			return result;
		endif
	endif

	var items := BOD_TemplateItems( BOD_GetTemplateKey(large_deed) );
	var filled := BOD_GetSlotsFilled(large_deed);
	var small_item := CInt(BOD_GetItemObjtype(small_deed));
	var slot := 0;
	var i;
	for( i := 1; i <= Len(items); i := i+1 )
		if( !CInt(filled[i]) and CInt(BOD_ExpectedFillObjtype(skillid, CInt(items[i]))) == small_item )
			slot := i;
			break;
		endif
	endfor

	if( !slot )
		BOD_Debug("BOD_TryFillLargeSlot: Small's item " + Hex(small_item) + " doesn't match any open slot in template " + CStr(BOD_GetTemplateKey(large_deed)));
		result.reason := "That deed doesn't fill any open slot in this order.";
		return result;
	endif

	filled[slot] := 1;
	SetObjProperty(large_deed, "BODSlotsFilled", filled);

	var small_difficulty := BOD_RewardDifficulty(small_deed);
	var small_gold := BOD_GoldReward(small_difficulty, has_axis);
	SetObjProperty(large_deed, "BODAccruedGold", CInt(GetObjProperty(large_deed, "BODAccruedGold")) + small_gold);
	SetObjProperty(large_deed, "BODAccruedPoints", CInt(GetObjProperty(large_deed, "BODAccruedPoints")) + BOD_POINTS_PER_SMALL);

	DestroyItem(small_deed);

	result.ok := 1;
	result.slot := slot;
	var filled_count := 0;
	foreach f in filled
		if( CInt(f) )
			filled_count := filled_count + 1;
		endif
	endforeach
	result.filled_count := filled_count;
	result.total_slots := Len(items);
	BOD_Debug("BOD_TryFillLargeSlot: filled slot " + slot + " (" + filled_count + "/" + result.total_slots + "), accrued gold now " + CInt(GetObjProperty(large_deed, "BODAccruedGold")));
	return result;
endfunction

// True if the given NPC's MerchantType is one this craft's BODs accept
// turn-in from (any matching NPC, not just the issuer).
function BOD_NPCAcceptsCraft(merchant_type, skillid)
	if( !merchant_type )
		BOD_Debug("BOD_NPCAcceptsCraft: merchant_type is empty/unset on this NPC (GetObjProperty(me,\"MerchantType\") returned nothing)");
		return 0;
	endif
	var section := BOD_CraftCfgSection(skillid);
	if( !section )
		return 0;   // BOD_CraftCfgSection already logged why
	endif
	var accepted_list := GetConfigStringArray(section, "NPCType");
	foreach accepted in accepted_list
		if( Lower(accepted) == Lower(merchant_type) )
			return 1;
		endif
	endforeach
	var accepted_str := "";
	foreach accepted in accepted_list
		if( accepted_str != "" )
			accepted_str := accepted_str + ", ";
		endif
		accepted_str := accepted_str + accepted;
	endforeach
	BOD_Debug("BOD_NPCAcceptsCraft: NPC MerchantType '" + merchant_type + "' not in " + BOD_CraftName(skillid) + "'s NPCType list [" + accepted_str + "]");
	return 0;
endfunction

// Turns in a completed BOD for its Points + Gold + Fame/Karma reward.
// Destroys the deed on success.
function BOD_TurnIn(byref deed, byref who, merchant_type)
	var result := struct;
	result.ok := 0;

	if( !deed or deed.objtype != UOBJ_BULKORDER_DEED )
		BOD_Debug("BOD_TurnIn: not a bulk order deed");
		result.reason := "That isn't a bulk order deed.";
		return result;
	endif

	if( !BOD_IsComplete(deed) )
		BOD_Debug("BOD_TurnIn: deed not complete yet");
		result.reason := "This order isn't finished yet.";
		return result;
	endif

	var skillid := CInt(BOD_GetCraft(deed));
	if( !BOD_NPCAcceptsCraft(merchant_type, skillid) )
		BOD_Debug("BOD_TurnIn: NPC (MerchantType=" + CStr(merchant_type) + ") doesn't accept " + BOD_CraftName(skillid) + " turn-ins");
		result.reason := "I don't handle that kind of order.";
		return result;
	endif

	var has_material_axis := BOD_HasMaterialAxis(skillid);
	var difficulty := BOD_RewardDifficulty(deed);

	var gold;
	var points;
	if( CInt(BOD_GetSize(deed)) )
		// Large: must always outpay cashing in its consumed Smalls piecemeal.
		gold := Cint( BOD_LARGE_REWARD_MULT * CInt(GetObjProperty(deed, "BODAccruedGold")) );
		points := Cint( BOD_LARGE_REWARD_MULT * CInt(GetObjProperty(deed, "BODAccruedPoints")) );
	else
		gold := BOD_GoldReward(difficulty, has_material_axis);
		points := BOD_POINTS_PER_SMALL;
	endif

	CreateItemInContainer( who.backpack, UOBJ_GOLD_COIN, gold );
	BOD_AwardFameKarma( who, difficulty );
	BOD_AddPoints( who, points );

	var size_label := "Small";
	if( CInt(BOD_GetSize(deed)) )
		size_label := "Large";
	endif
	BOD_Debug("BOD_TurnIn: " + who.name + " turned in a " + BOD_CraftName(skillid) + " " + size_label + " for " + gold + " gold, " + points + " points (difficulty=" + difficulty + ")");

	DestroyItem(deed);

	result.ok := 1;
	result.gold := gold;
	result.points := points;
	return result;
endfunction

// All 8 crafts, wired as of Phase 2.
function BOD_AllCraftSkillIds()
	return {
		SKILLID_BLACKSMITHY, SKILLID_BOWCRAFT, SKILLID_CARPENTRY, SKILLID_TAILORING,
		SKILLID_TINKERING, SKILLID_INSCRIPTION, SKILLID_COOKING, SKILLID_ALCHEMY
	};
endfunction

// Reverse-maps an NPC's MerchantType to the craft it accepts BOD issuance/
// turn-in for, or 0 if none of the wired crafts claim that NPC type.
function BOD_SkillIdForNPCType(merchant_type)
	BOD_Debug("BOD_SkillIdForNPCType: checking merchant_type='" + CStr(merchant_type) + "'");
	if( !merchant_type )
		BOD_Debug("BOD_SkillIdForNPCType: merchant_type is falsy, bailing out");
		return 0;
	endif
	foreach skillid in BOD_AllCraftSkillIds()
		if( BOD_NPCAcceptsCraft(merchant_type, skillid) )
			BOD_Debug("BOD_SkillIdForNPCType: matched " + BOD_CraftName(skillid));
			return skillid;
		endif
	endforeach
	BOD_Debug("BOD_SkillIdForNPCType: no craft claimed merchant_type '" + CStr(merchant_type) + "'");
	return 0;
endfunction

// Called from scripts/ai/merchant.src's SYSEVENT_ITEM_GIVEN hook when a
// player hands a bulk order deed to a matching craft NPC.
function BOD_HandleTurnIn(byref npc, byref player, byref deed, merchant_type)
	var result := BOD_TurnIn(deed, player, merchant_type);
	if( !result.ok )
		PrintTextAbovePrivate(npc, result.reason, player, color := 33);
		MoveItemToContainer(deed, player.backpack);
		return;
	endif

	PrintTextAbovePrivate(npc, "Well done! Here is " + result.gold + " gold and " + result.points + " bulk order points for your trouble.", player, color := 88);
	PlaySoundEffect(npc, 0x38);
endfunction
