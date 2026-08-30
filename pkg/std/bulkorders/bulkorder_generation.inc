// Bulk Order Deeds -- generation.
//
// Small BOD: craft + item + quantity + quality only, no material roll (that
// moved to Large-only, see memory project_bulk_order_deeds_plan). Large-tier
// generation (material curve, exceptional-across-a-shared-roll) is Phase 3.

include ":bulkorders:bulkorder";
include ":bulkorders:bulkorder_matching";
include "include/objtype";
include "include/attributes";

use cfgfile;
use basic;

// The set of objtypes excluded for a craft via bulkorders.cfg's ExcludeItem
// list (a dictionary used as a set, for O(1) membership checks).
function BOD_ExcludedItems(skillid)
	var excluded := dictionary;
	var section := BOD_CraftCfgSection(skillid);
	if( section )
		foreach x in (GetConfigIntArray(section, "ExcludeItem"))
			excluded[x] := 1;
		endforeach
	endif
	return excluded;
endfunction

// Builds the eligible Small-BOD item pool for a craft by reading its own
// recipe .cfg directly (never duplicated into bulkorders.cfg), filtered
// against that craft's ExcludeItem list. Each craft's recipe .cfg has a
// different shape, so the eligibility test differs per craft:
//   - Blacksmithy/Carpentry/Tailoring/Tinkering: objtype-keyed, a finished
//     recipe entry carries a Type field (raw-material entries carry
//     Difficulty instead, with no Type -- naturally filtered out).
//   - Bowcraft: objtype-keyed, filtered by Category in {Bows, Crossbows}
//     (Shafts/Ammo/Misc excluded structurally by category, not a list).
//   - Inscription: objtype-keyed, a real spell scroll carries a Circle
//     field (Runebook/gem stubs/enchant foci mixed into the same block
//     type have no Circle, so they're naturally filtered out too).
//   - Cooking: numbered recipe blocks (1..N), not objtype-keyed -- several
//     numbers share the same product_objtype at different skill tiers, so
//     the pool is de-duplicated.
//   - Alchemy: blocks keyed by potion name (a string), with an objtype
//     field -- string-keyed blocks are naturally separate from the file's
//     other int-keyed "Alchemy <reagent>" blocks (GetConfigStringKeys only
//     returns the potion-name keys).
function BOD_ItemPool(skillid)
	var pool := {};
	var recipe_cfg := BOD_RecipeCfg(skillid);
	if( !recipe_cfg )
		BOD_Debug("BOD_ItemPool(" + BOD_CraftName(skillid) + "): BOD_RecipeCfg returned falsy -- recipe .cfg failed to load");
		return pool;
	endif
	var excluded := BOD_ExcludedItems(skillid);

	case(skillid)
		SKILLID_BLACKSMITHY:
		SKILLID_CARPENTRY:
		SKILLID_TAILORING:
		SKILLID_TINKERING:
			foreach key in (GetConfigIntKeys(recipe_cfg))
				var elem := recipe_cfg[key];
				if( GetConfigInt(elem, "Type") and !excluded[key] )
					pool.Append(key);
				endif
				SleepMS(1);
			endforeach

		SKILLID_BOWCRAFT:
			foreach key in (GetConfigIntKeys(recipe_cfg))
				var elem := recipe_cfg[key];
				var category := Lower(GetConfigString(elem, "Category"));
				if( (category == "bows" or category == "crossbows") and !excluded[key] )
					pool.Append(key);
				endif
				SleepMS(1);
			endforeach

		SKILLID_INSCRIPTION:
			foreach key in (GetConfigIntKeys(recipe_cfg))
				var elem := recipe_cfg[key];
				if( GetConfigInt(elem, "Circle") and !excluded[key] )
					pool.Append(key);
				endif
				SleepMS(1);
			endforeach

		SKILLID_COOKING:
			var seen := dictionary;
			foreach key in (GetConfigIntKeys(recipe_cfg))
				var elem := recipe_cfg[key];
				var product := CInt(GetConfigInt(elem, "product_objtype"));
				if( product and !seen[product] and !excluded[product] )
					seen[product] := 1;
					pool.Append(product);
				endif
				SleepMS(1);
			endforeach

		SKILLID_ALCHEMY:
			foreach potionname in (GetConfigStringKeys(recipe_cfg))
				var elem := recipe_cfg[potionname];
				var product := CInt(GetConfigInt(elem, "objtype"));
				if( product and !excluded[product] )
					pool.Append(product);
				endif
				SleepMS(1);
			endforeach

			// pkg/opt/alchemyplus adds 17 more real, distinct potions
			// (itemtype 23-39 in alchemyplus.cfg) that alchemy.cfg has no
			// knowledge of at all -- they're not exposed via a clean cfg
			// field the way everything else here is (no "objtype"/"product"
			// field on these entries; the actual produced objtype only
			// exists as a hardcoded case(itemtype)/case(level) table in
			// alchemyplus.src/alchemyplus toad.src, keyed off the crafter's
			// mage-class level at craft time, not a player choice). Using
			// each family's base (or, where level 0 has no mapping, its
			// only/default) objtype as the one BOD-eligible representative,
			// same convention already used for every other Lesser/Regular/
			// Greater-style ladder in this pool. Totem and Elixer
			// (itemtype 36/37) are deliberately left out -- they're
			// mage-class-gated utility items (CLASSEID_MAGE level 4+), not
			// ordinary potions. Found 2026-08-29 answering "where are Grand
			// Mage Refresh/Elixir/Rebirth potions".
			var alchemyplus_extra := {
				0xff4e, 0xff52, 0xff59,             // Phandel's Fine/Fabulous/Fantastic Intellect
				0xff62, 0xff68, 0xff6e,              // Lesser/Regular/Greater Mego Invulnerability
				0xff74,                              // Grand Mage Refresh Elixir
				0xff7a, 0xff80,                       // Homeric Might / Greater Homeric Might
				0xff86,                              // Tamla Heal
				0xff8c, 0xff8e,                       // Taint's Minor/Major Transmutation
				0xff95,                              // Invisibility Potion
				0xff94,                              // Dispel Potion
				0xffa2                               // Rebirth Potion
			};
			foreach product in alchemyplus_extra
				if( !excluded[product] )
					pool.Append(product);
				endif
			endforeach

		default:
			BOD_Debug("BOD_ItemPool: no case branch wired for skillid " + skillid);
			return pool;
	endcase

	BOD_Debug("BOD_ItemPool(" + BOD_CraftName(skillid) + "): " + Len(pool) + " eligible items");
	return pool;
endfunction

// Returns 0 on failure, or a struct: {craft, size, item, amount, exceptional}
function BOD_GenerateSmall(skillid)
	var pool := BOD_ItemPool(skillid);
	if( !pool or !Len(pool) )
		BOD_Debug("BOD_GenerateSmall(" + BOD_CraftName(skillid) + "): empty item pool, cannot generate an order");
		return 0;
	endif

	var order := struct;
	order.craft := skillid;
	order.size := 0;
	order.item := pool[ Random(Len(pool)) + 1 ];

	// Quantity is 10, 15, or 20 only -- 3 discrete values, not a range.
	case( Random(3) )
		0: order.amount := 10;
		1: order.amount := 15;
		2: order.amount := 20;
	endcase

	// Flat 1-in-6 (~16.7%), no skill scaling above the tier threshold.
	order.exceptional := ( Random(BOD_EXCEPTIONAL_CHANCE) == 0 );

	return order;
endfunction

// Returns 0 on failure, or a struct:
// {craft, size, templatekey, material, difficulty, exceptional}
function BOD_GenerateLarge(skillid)
	var templatekey := BOD_PickTemplate(skillid);
	if( !templatekey )
		BOD_Debug("BOD_GenerateLarge(" + BOD_CraftName(skillid) + "): BOD_PickTemplate returned nothing -- no templates found for this craft in bulkorder_templates.cfg");
		return 0;
	endif

	var order := struct;
	order.craft := skillid;
	order.size := 1;
	order.templatekey := templatekey;

	// Flat 1-in-6 (~16.7%), shared across every slot in the order.
	order.exceptional := ( Random(BOD_EXCEPTIONAL_CHANCE) == 0 );

	order.material := 0;
	order.difficulty := 0;
	if( BOD_HasMaterialAxis(skillid) )
		if( skillid == SKILLID_INSCRIPTION )
			// No player-chosen material at all -- the template's own
			// hardest Circle already fixed this order's difficulty.
			order.difficulty := BOD_InscriptionTemplateDifficulty(templatekey);
		else
			order.material := BOD_PickMaterial(skillid);
			order.difficulty := BOD_MaterialDifficulty(skillid, order.material);
		endif
	endif

	BOD_Debug("BOD_GenerateLarge(" + BOD_CraftName(skillid) + "): template=" + BOD_TemplateName(templatekey) + " material=" + Hex(order.material) + " difficulty=" + order.difficulty + " exceptional=" + order.exceptional);

	return order;
endfunction

// ==========================================================================
// Issuance (daily cap is per craft, per character -- see memory)
// ==========================================================================

// Returns the number of still-live issuances counted against today's cap,
// pruning anything older than 86400 seconds as it goes.
//
// Stored as a plain array of timestamps, NOT a dictionary keyed by
// timestamp -- two issuances landing in the same ReadGameClock() tick would
// collide on an identical dictionary key and silently overwrite each other,
// undercounting today's total and letting one extra order through before
// the cap trips (confirmed live: BOD_DAILY_CAP=5 but 6 got issued).
//
// Per-character property via GetObjProperty/SetObjProperty on the mobile --
// NOT global, and critically NOT "#"-prefixed. powerhour's weekly-cooldown
// property (pph_use_time/pph_use_weekday in
// pkg/opt/powerhour/textcmd/player/setph.src) uses this exact idiom -- plain
// GetObjProperty/SetObjProperty on the character, no "#" -- and is confirmed
// to survive restarts correctly. Every "#"-prefixed property in that same
// file (#PPHH, #PPHC, #PPHS, #SettingPH, #PPHPid) is a short-lived runtime
// flag that gets explicitly EraseObjProperty'd before the script ends, so
// none of them were ever actually relied on to survive a restart -- they
// were never real evidence either way. The BOD daily cap previously used a
// "#"-prefixed key ("#bod_daily_...") and did NOT reliably survive a
// restart (confirmed live, including immediately after a manual .savenow).
// Dropping the "#" to match the one idiom in this codebase actually proven
// to persist a per-character cooldown across a restart.
function BOD_CheckDailyCap(byref who, skillid)
	var key := "bod_daily_" + BOD_CraftName(skillid);
	var issued := GetObjProperty(who, key);
	if( !issued )
		BOD_Debug("BOD_CheckDailyCap(" + BOD_CraftName(skillid) + "): raw read of '" + key + "' -- unset/falsy (TypeOf=" + TypeOf(issued) + ")");
	else
		BOD_Debug("BOD_CheckDailyCap(" + BOD_CraftName(skillid) + "): raw read of '" + key + "' -- TypeOf=" + TypeOf(issued) + " Len=" + Len(issued));
	endif
	if( !issued )
		issued := {};
	elseif( TypeOfInt(issued) == OT_DICTIONARY )
		// One-time migration: property was stored under the old
		// dictionary-keyed-by-timestamp format before this fix. Convert to
		// the new plain-array format so already-recorded issuances today
		// (yours included) still count correctly.
		var migrated := {};
		foreach ts in (issued.keys())
			migrated.Append(ts);
		endforeach
		issued := migrated;
	endif
	var kept := {};
	foreach ts in issued
		if( ts + 86400 >= ReadGameClock() )
			kept.Append(ts);
		endif
	endforeach
	SetObjProperty(who, key, kept);
	BOD_Debug("BOD_CheckDailyCap(" + BOD_CraftName(skillid) + "): " + who.name + " has " + Len(kept) + "/" + BOD_DAILY_CAP + " used today (pruned " + (Len(issued) - Len(kept)) + " expired)");
	return Len(kept);
endfunction

function BOD_RecordIssuance(byref who, skillid)
	var key := "bod_daily_" + BOD_CraftName(skillid);
	var issued := GetObjProperty(who, key);
	if( !issued )
		issued := {};
	endif
	issued.Append(ReadGameClock());
	SetObjProperty(who, key, issued);
	BOD_Debug("BOD_RecordIssuance(" + BOD_CraftName(skillid) + "): " + who.name + " now at " + Len(issued) + " issuances today");
endfunction

// Issues a fresh BOD deed into who's backpack -- Small below 100 skill,
// otherwise a flat 30% chance of Large (falling back to Small the rest of
// the time, both flat gates with no skill scaling above the tier
// thresholds). Returns a struct: {ok, reason} on failure, {ok, deed} on
// success.
function BOD_IssueDeed(byref who, skillid)
	var result := struct;
	result.ok := 0;

	var eff_skill := GetEffectiveSkill(who, skillid);
	BOD_Debug("BOD_IssueDeed(" + BOD_CraftName(skillid) + "): " + who.name + " has effective skill " + eff_skill);

	if( eff_skill < BOD_SMALL_SKILL_MIN )
		BOD_Debug("BOD_IssueDeed: skill " + eff_skill + " < BOD_SMALL_SKILL_MIN(" + BOD_SMALL_SKILL_MIN + ")");
		result.reason := "You aren't skilled enough for a bulk order yet.";
		return result;
	endif

	var used_today := BOD_CheckDailyCap(who, skillid);
	if( used_today >= BOD_DAILY_CAP )
		BOD_Debug("BOD_IssueDeed: daily cap hit (" + used_today + "/" + BOD_DAILY_CAP + ")");
		result.reason := "I don't have any more work for you today. Come back tomorrow.";
		return result;
	endif

	var order := 0;
	if( eff_skill >= BOD_LARGE_SKILL_MIN and Random(100) < BOD_LARGE_SHARE_PCT )
		BOD_Debug("BOD_IssueDeed: rolling Large");
		order := BOD_GenerateLarge(skillid);
		if( !order )
			BOD_Debug("BOD_IssueDeed: BOD_GenerateLarge failed, falling back to Small");
		endif
	endif
	if( !order )
		order := BOD_GenerateSmall(skillid);
	endif
	if( !order )
		BOD_Debug("BOD_IssueDeed: BOD_GenerateSmall also failed -- no eligible items for this craft");
		result.reason := "I don't have any work for you right now.";
		return result;
	endif

	var deed := CreateItemInContainer( who.backpack, UOBJ_BULKORDER_DEED, 1 );
	if( !deed )
		result.reason := "Your backpack is full.";
		return result;
	endif

	deed.color := BOD_DeedColor(skillid, order.size);
	SetObjProperty(deed, "BODCraft", skillid);
	SetObjProperty(deed, "BODSize", order.size);
	SetObjProperty(deed, "BODExceptional", order.exceptional);

	if( order.size )
		SetObjProperty(deed, "BODTemplateKey", order.templatekey);
		SetObjProperty(deed, "BODMaterial", order.material);
		SetObjProperty(deed, "BODEffectiveDifficulty", order.difficulty);
		SetObjProperty(deed, "BODAccruedGold", 0);
		SetObjProperty(deed, "BODAccruedPoints", 0);

		var items := BOD_TemplateItems(order.templatekey);
		var slots := {};
		foreach unused_item in items
			slots.Append(0);
		endforeach
		SetObjProperty(deed, "BODSlotsFilled", slots);

		deed.name := "a " + BOD_CraftName(skillid) + " Large bulk order deed (" + BOD_TemplateName(order.templatekey) + ")";
	else
		SetObjProperty(deed, "BODItemObjtype", order.item);
		SetObjProperty(deed, "BODAmount", order.amount);
		SetObjProperty(deed, "BODAmountFilled", 0);
		deed.name := "a " + BOD_CraftName(skillid) + " bulk order deed";
	endif

	BOD_RecordIssuance(who, skillid);

	result.ok := 1;
	result.deed := deed;
	return result;
endfunction

// Called from scripts/ai/merchant.src's SYSEVENT_SPEECH hook when a player
// says "bulk order" to a matching craft NPC.
function BOD_HandleRequestSpeech(byref npc, byref player, merchant_type)
	BOD_Debug("BOD_HandleRequestSpeech: " + player.name + " asked " + npc.name + " (MerchantType=" + CStr(merchant_type) + ") for a bulk order");
	var skillid := BOD_SkillIdForNPCType(merchant_type);
	if( !skillid )
		PrintTextAbovePrivate(npc, "I don't have any bulk orders for you.", player, color := 33);
		return;
	endif

	var result := BOD_IssueDeed(player, skillid);
	if( !result.ok )
		PrintTextAbovePrivate(npc, result.reason, player, color := 33);
		return;
	endif

	PrintTextAbovePrivate(npc, "Here is a bulk order. Bring me the finished order when you're done.", player, color := 88);
endfunction
