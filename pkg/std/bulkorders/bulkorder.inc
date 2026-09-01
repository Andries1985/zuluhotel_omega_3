// Bulk Order Deeds -- shared constants, CProp accessors, and per-craft
// metadata. Locked design: see memory project_bulk_order_deeds_plan and the
// published "Bulk Order Deeds" artifact for full rationale.

use uo;
use os;
use util;
use cfgfile;
use polsys;
use basicio;

include ":karmafame:karmafame";
include "include/objtype";
include "include/client";
include "include/random";

// ---- Debug logging (server console, not player-visible) ----
// Flip to 0 to silence once the system is confirmed working end to end.
const BOD_DEBUG_ENABLED := 1;

function BOD_Debug(msg)
	if( BOD_DEBUG_ENABLED )
		Print("[BOD] " + msg);
	endif
endfunction

// ---- Tier thresholds (flat gates, no skill-scaled odds past these) ----
const BOD_SMALL_SKILL_MIN   := 50;
const BOD_LARGE_SKILL_MIN   := 100;

// ---- Flat generation gates ----
// Exceptional-required roll is 1 in 6 (~16.7%), solved backward from the
// New Zulu Exceptional Large ~6 month target -- not independently picked.
const BOD_EXCEPTIONAL_CHANCE := 6;

// Large-share (chance a daily roll comes back Large instead of Small) --
// unused until Phase 3, kept here since it's part of the same locked gate.
const BOD_LARGE_SHARE_PCT    := 30;

// ---- Material rarity curve (Large only, from Phase 3) ----
// weight(material) = (1 - Difficulty/150)^2 + floor
const BOD_MATERIAL_FLOOR     := 1.19;

// ---- Reward constants ----
const BOD_POINTS_PER_SMALL      := 10;     // flat, Phase 4
const BOD_POINTS_PER_TOKEN      := 36;     // Phase 4
const BOD_LARGE_REWARD_MULT     := 3.0;    // Points/Gold only, Phase 3+
const BOD_GOLD_BASE             := 350;    // material-tiered crafts, x QualityMultiplier
const BOD_GOLD_FLAT             := 1000;   // Cooking/Alchemy, no material axis
const BOD_FAMEKARMA_MIN         := 1;
const BOD_FAMEKARMA_MAX         := 25;

// AwardFame/AwardKarma (pkg/opt/karmafame/karmafame.inc) divide the passed
// amount by 50 and clamp the result to 1-25 before applying it -- every
// existing craft's FAME_BONUS:=10 call only ever grants 1 fame because of
// this. The reward design's Fame/Karma budget is scaled to 1-25 (not the
// originally-drafted 10-500) specifically to fit this real clamp, discovered
// while wiring Phase 1 -- see memory.
const BOD_AWARDFAME_DIVISOR     := 50;

// ---- Daily issuance cap ----
const BOD_DAILY_CAP             := 5;   // per craft, per character

// ==========================================================================
// Reward math
// ==========================================================================

function BOD_QualityMultiplier(difficulty)
	// 1x - 10x, reused identically for Gold and Fame/Karma scaling.
	return 1.0 + (CDbl(difficulty)/150.0)*9.0;
endfunction

function BOD_FameKarmaBudget(difficulty)
	var budget := BOD_FAMEKARMA_MIN + Cint( (CDbl(difficulty)/150.0) * (BOD_FAMEKARMA_MAX - BOD_FAMEKARMA_MIN) );
	if( budget < BOD_FAMEKARMA_MIN )
		budget := BOD_FAMEKARMA_MIN;
	elseif( budget > BOD_FAMEKARMA_MAX )
		budget := BOD_FAMEKARMA_MAX;
	endif
	return budget;
endfunction

function BOD_GoldReward(difficulty, has_material_axis)
	if( !has_material_axis )
		return BOD_GOLD_FLAT;
	endif
	return Cint( BOD_GOLD_BASE * BOD_QualityMultiplier(difficulty) );
endfunction

// Splits a Fame/Karma budget randomly 20-80% between the two (never purely
// one or the other) and awards it through the real engine helpers.
function BOD_AwardFameKarma(byref who, difficulty)
	var budget := BOD_FameKarmaBudget(difficulty);
	var fame_share := (Random(61)+20) / 100.0;
	var fame_amt := Cint( budget * fame_share );
	var karma_amt := budget - fame_amt;

	if( fame_amt > 0 )
		AwardFame( who, fame_amt * BOD_AWARDFAME_DIVISOR );
	endif
	if( karma_amt > 0 )
		AwardKarma( who, karma_amt * BOD_AWARDFAME_DIVISOR );
	endif
endfunction

// ==========================================================================
// Per-craft metadata
// ==========================================================================

function BOD_CraftName(skillid)
	case(skillid)
		SKILLID_BLACKSMITHY: return "Blacksmithy";
		SKILLID_BOWCRAFT:    return "Bowcraft";
		SKILLID_CARPENTRY:   return "Carpentry";
		SKILLID_TAILORING:   return "Tailoring";
		SKILLID_TINKERING:   return "Tinkering";
		SKILLID_INSCRIPTION: return "Inscription";
		SKILLID_COOKING:     return "Cooking";
		SKILLID_ALCHEMY:     return "Alchemy";
		default: return "";
	endcase
endfunction

// Cooking/Alchemy have no material axis at all -- both crafts' materials are
// vendor-purchasable, so there's no rarity to gate on; both are a lighter
// supplement by design, not a full parallel chase system. Every other craft
// (including Inscription, whose "material" is really the intrinsic spell
// Circle of the item requested rather than a player-chosen crafting
// material) has one.
function BOD_HasMaterialAxis(skillid)
	case(skillid)
		SKILLID_BLACKSMITHY:
		SKILLID_BOWCRAFT:
		SKILLID_CARPENTRY:
		SKILLID_TAILORING:
		SKILLID_TINKERING:
		SKILLID_INSCRIPTION:
			return 1;
		default:
			return 0;
	endcase
endfunction

// Locked palette (2026-08-23): one hue family per craft, Small/Large hues
// drawn from two real named materials that craft already produces (Cooking/
// Alchemy/Inscription's are color-reference only -- those 3 have no real
// player-chosen material tier, see BOD_HasMaterialAxis).
function BOD_DeedColor(skillid, size)
	case(skillid)
		SKILLID_BLACKSMITHY:
			if( size ) return 2669; else return 1131; endif   // New Zulu / Darkpagan ore
		SKILLID_TAILORING:
			if( size ) return 1154; else return 1102; endif   // Silver Dragon / Wolf hide
		SKILLID_TINKERING:
			if( size ) return 2298; else return 1209; endif   // Destruction / Red Elven ore
		SKILLID_BOWCRAFT:
			if( size ) return 1287; else return 48;   endif   // Bloodhorse / Golden Reflections wood
		SKILLID_CARPENTRY:
			if( size ) return 1655; else return 2243; endif   // Stardust / Crystal wood
		SKILLID_COOKING:
			if( size ) return 2733; else return 44;   endif   // Lava / Bear hide
		SKILLID_INSCRIPTION:
			if( size ) return 2752; else return 2742; endif   // Liche hide / Anra ore
		SKILLID_ALCHEMY:
			if( size ) return 2734; else return 2130; endif   // Malachite / Old Britain ore
		default:
			return 0;
	endcase
endfunction

function BOD_CraftCfg()
	return ReadConfigFile(":bulkorders:bulkorders");
endfunction

function BOD_CraftCfgSection(skillid)
	var craftname := BOD_CraftName(skillid);
	if( !craftname )
		BOD_Debug("BOD_CraftCfgSection: no craft name for skillid " + skillid);
		return 0;
	endif
	var cfg := BOD_CraftCfg();
	if( !cfg )
		BOD_Debug("BOD_CraftCfgSection: bulkorders.cfg failed to load (ReadConfigFile returned falsy)!");
		return 0;
	endif
	var section := FindConfigElem( cfg, craftname );
	if( !section )
		BOD_Debug("BOD_CraftCfgSection: no BulkOrderCraft block found for key '" + craftname + "' -- check bulkorders.cfg");
	endif
	return section;
endfunction

// The recipe .cfg this craft's item pool is drawn from -- one per craft,
// never duplicated into bulkorders.cfg.
function BOD_RecipeCfg(skillid)
	case(skillid)
		SKILLID_BLACKSMITHY: return ReadConfigFile(":blacksmithy:blacksmithy");
		SKILLID_BOWCRAFT:    return ReadConfigFile(":bowcraft:bowcraft");
		SKILLID_CARPENTRY:   return ReadConfigFile(":carpentry:carpentry");
		SKILLID_TAILORING:   return ReadConfigFile(":tailoring:tailoring");
		SKILLID_TINKERING:   return ReadConfigFile(":tinkering:tinker");
		SKILLID_INSCRIPTION: return ReadConfigFile(":inscription:inscription");
		SKILLID_COOKING:     return ReadConfigFile(":cooking:cooking");
		SKILLID_ALCHEMY:     return ReadConfigFile(":alchemy:alchemy");
		default: return 0;
	endcase
endfunction

// Every real material .cfg that might hold a Difficulty/Quality entry for a
// material objtype this craft could have used. Most crafts draw from exactly
// one; Carpentry and Tinkering can craft from more than one raw-material
// family (wood/metal/cloth), so their filled material's objtype is looked up
// across each candidate in turn -- real material objtypes don't collide
// across families, so whichever cfg actually contains the key wins.
function BOD_MaterialCfgCandidates(skillid)
	case(skillid)
		SKILLID_BLACKSMITHY: return { ":blacksmithy:blacksmithy" };
		SKILLID_BOWCRAFT:    return { ":carpentry:carpentry" };
		SKILLID_TAILORING:   return { ":tailoring:tailoring" };
		SKILLID_CARPENTRY:   return { ":carpentry:carpentry", ":blacksmithy:blacksmithy", ":tailoring:tailoring" };
		SKILLID_TINKERING:   return { ":carpentry:carpentry", ":blacksmithy:blacksmithy" };
		default: return {};
	endcase
endfunction

// Material objtypes this craft's ExcludeMaterial list rules out (e.g.
// Blacksmithy's Gold entry -- a real named material.cfg row, but not a
// real BOD material tier), same "ExcludeItem" pattern BOD_ExcludedItems
// uses for the item pool.
function BOD_ExcludedMaterials(skillid)
	var excluded := dictionary;
	var section := BOD_CraftCfgSection(skillid);
	if( section )
		foreach x in (GetConfigIntArray(section, "ExcludeMaterial"))
			excluded[x] := 1;
		endforeach
	endif
	return excluded;
endfunction

// Real Difficulty (0-150ish) for a material objtype this craft actually
// produced an item from -- 0 if not found (e.g. the craft has no material
// data for that family, like Tinkering's glass/clay items).
function BOD_MaterialDifficulty(skillid, mat_key)
	if( !mat_key )
		return 0;
	endif
	foreach cfgname in (BOD_MaterialCfgCandidates(skillid))
		var cfg := ReadConfigFile(cfgname);
		if( cfg )
			var elem := cfg[mat_key];
			if( elem )
				return CInt(GetConfigInt(elem, "Difficulty"));
			endif
		endif
	endforeach
	return 0;
endfunction

// A material's real display name (e.g. "New Zulu") lives in its craft's own
// material .cfg, not necessarily in itemdesc.cfg's generic per-graphic
// Name/Desc (which is often shared across every color of the same raw
// ingot/log/hide) -- read it directly rather than misusing
// BOD_ItemDisplayName.
function BOD_MaterialName(skillid, mat_key)
	if( !mat_key )
		return "";
	endif
	foreach cfgname in (BOD_MaterialCfgCandidates(skillid))
		var cfg := ReadConfigFile(cfgname);
		if( cfg )
			var elem := cfg[mat_key];
			if( elem )
				var name := GetConfigString(elem, "Name");
				if( name and name != "" )
					return name;
				endif
				return BOD_ItemDisplayName(mat_key);
			endif
		endif
	endforeach
	return BOD_ItemDisplayName(mat_key);
endfunction

// Every real material choice available to this craft (objtype + display
// name), for the Small BOD material picker gump -- same "material entry,
// not a recipe" test BOD_PickMaterial uses (Large's weighted roll), just
// enumerated flat instead of weighted, and de-duplicated across candidate
// cfgs (Carpentry/Tinkering draw from more than one).
function BOD_MaterialChoices(skillid)
	var seen := dictionary;
	var excluded := BOD_ExcludedMaterials(skillid);
	var choices := {};
	foreach cfgname in (BOD_MaterialCfgCandidates(skillid))
		var cfg := ReadConfigFile(cfgname);
		if( cfg )
			foreach key in (GetConfigIntKeys(cfg))
				var elem := cfg[key];
				if( !GetConfigInt(elem, "Type") and !seen[key] and !excluded[key] )
					seen[key] := 1;
					var choice := struct;
					choice.objtype := key;
					choice.name := GetConfigString(elem, "Name");
					choices.Append(choice);
				endif
				SleepMS(1);
			endforeach
		endif
	endforeach
	return choices;
endfunction

// Inscription has no player-chosen crafting material at all -- every scroll
// intrinsically belongs to a spell Circle, and circles.cfg already carries a
// real Difficulty (0-150ish) per circle on the same scale as every other
// craft's material table, so that substitutes directly.
function BOD_InscriptionItemDifficulty(itemtype)
	var insc_cfg := BOD_RecipeCfg(SKILLID_INSCRIPTION);
	var circles_cfg := ReadConfigFile(":*:circles");
	if( !insc_cfg or !circles_cfg or !itemtype )
		return 0;
	endif
	var elem := insc_cfg[itemtype];
	if( !elem )
		return 0;
	endif
	var circle := CInt(GetConfigInt(elem, "Circle"));
	if( !circle )
		return 0;
	endif
	var circle_elem := circles_cfg[circle];
	if( !circle_elem )
		return 0;
	endif
	return CInt(GetConfigInt(circle_elem, "Difficulty"));
endfunction

// The real Difficulty value that should drive this completed order's reward
// scaling. Small: material-tiered crafts read the material the player
// actually used (BODFilledMaterial); Inscription reads the requested item's
// own Circle instead, since it has no player-chosen material. Large: fixed
// at generation time (BODEffectiveDifficulty), since a Large rolls its own
// material/template difficulty upfront rather than inheriting it from a
// player's later choices.
function BOD_RewardDifficulty(byref deed)
	var skillid := CInt(BOD_GetCraft(deed));
	if( !BOD_HasMaterialAxis(skillid) )
		return 0;
	endif
	if( CInt(BOD_GetSize(deed)) )
		return CInt(GetObjProperty(deed, "BODEffectiveDifficulty"));
	endif
	if( skillid == SKILLID_INSCRIPTION )
		return BOD_InscriptionItemDifficulty( CInt(BOD_GetItemObjtype(deed)) );
	endif
	return BOD_MaterialDifficulty( skillid, CInt(BOD_GetFilledMaterial(deed)) );
endfunction

// ==========================================================================
// Material rarity curve (Large only)
// ==========================================================================

// weight(material) = (1 - Difficulty/150)^2 + floor -- harder materials are
// monotonically rarer; floor keeps the rarest tier reachable in finite time.
function BOD_MaterialWeight(difficulty)
	var x := 1.0 - (CDbl(difficulty)/150.0);
	return (x*x) + BOD_MATERIAL_FLOOR;
endfunction

// Rolls one material for a Large BOD via the locked quadratic rarity curve,
// across every real material entry this craft's material cfg(s) define.
// Returns the material's real objtype, or 0 if none found.
function BOD_PickMaterial(skillid)
	var keys := {};
	var weights := {};
	var total := 0.0;
	var excluded := BOD_ExcludedMaterials(skillid);

	foreach cfgname in (BOD_MaterialCfgCandidates(skillid))
		var cfg := ReadConfigFile(cfgname);
		if( cfg )
			foreach key in (GetConfigIntKeys(cfg))
				var elem := cfg[key];
				if( !GetConfigInt(elem, "Type") and !excluded[key] )   // material entry, not a recipe
					var w := BOD_MaterialWeight( CInt(GetConfigInt(elem, "Difficulty")) );
					keys.Append(key);
					weights.Append(w);
					total := total + w;
				endif
				SleepMS(1);
			endforeach
		endif
	endforeach

	if( !Len(keys) or total <= 0.0 )
		BOD_Debug("BOD_PickMaterial(" + BOD_CraftName(skillid) + "): no material entries found across candidate cfgs -- check BOD_MaterialCfgCandidates");
		return 0;
	endif

	var roll := (CDbl(Random(1000000)) / 1000000.0) * total;
	var cumulative := 0.0;
	var i;
	for( i := 1; i <= Len(keys); i := i+1 )
		cumulative := cumulative + weights[i];
		if( roll <= cumulative )
			return keys[i];
		endif
	endfor
	return keys[Len(keys)];
endfunction

// ==========================================================================
// Large BOD Templates catalog (locked, 93 templates across 8 crafts)
// ==========================================================================

function BOD_TemplatesCfg()
	return ReadConfigFile(":bulkorders:bulkorder_templates");
endfunction

function BOD_TemplatesForCraft(skillid)
	var craftname := BOD_CraftName(skillid);
	var keys := {};
	if( !craftname )
		return keys;
	endif
	var cfg := BOD_TemplatesCfg();
	foreach key in (GetConfigStringKeys(cfg))
		var elem := cfg[key];
		if( GetConfigString(elem, "Craft") == craftname )
			keys.Append(key);
		endif
		SleepMS(1);
	endforeach
	return keys;
endfunction

function BOD_TemplateItems(templatekey)
	if( !templatekey )
		return {};
	endif
	var cfg := BOD_TemplatesCfg();
	var elem := cfg[templatekey];
	if( !elem )
		return {};
	endif
	return GetConfigIntArray(elem, "Item");
endfunction

function BOD_TemplateName(templatekey)
	var cfg := BOD_TemplatesCfg();
	var elem := cfg[templatekey];
	if( !elem )
		return CStr(templatekey);
	endif
	return GetConfigString(elem, "Name");
endfunction

// Inscription templates have no player-chosen material, so their "rarity"
// comes from the hardest spell Circle in the set -- reused both to weight
// which template a Large rolls and as that Large's stored
// BODEffectiveDifficulty for reward scaling.
function BOD_InscriptionTemplateDifficulty(templatekey)
	var items := BOD_TemplateItems(templatekey);
	var maxdiff := 0;
	foreach item in items
		var d := BOD_InscriptionItemDifficulty(CInt(item));
		if( d > maxdiff )
			maxdiff := d;
		endif
	endforeach
	return maxdiff;
endfunction

// Rolls one template for a Large BOD. Every craft draws uniformly (the
// template only fixes *which* items -- material/exceptional roll
// separately); Inscription is the one exception, since it has no separate
// material roll at all -- its templates themselves are weighted by the same
// quadratic curve, using each template's hardest Circle as its Difficulty.
function BOD_PickTemplate(skillid)
	var templates := BOD_TemplatesForCraft(skillid);
	if( !Len(templates) )
		BOD_Debug("BOD_PickTemplate(" + BOD_CraftName(skillid) + "): 0 templates found -- check bulkorder_templates.cfg's Craft field matches BOD_CraftName exactly");
		return 0;
	endif

	if( skillid == SKILLID_INSCRIPTION )
		var weights := {};
		var total := 0.0;
		foreach key in templates
			var w := BOD_MaterialWeight( BOD_InscriptionTemplateDifficulty(key) );
			weights.Append(w);
			total := total + w;
		endforeach
		var roll := (CDbl(Random(1000000)) / 1000000.0) * total;
		var cumulative := 0.0;
		var i;
		for( i := 1; i <= Len(templates); i := i+1 )
			cumulative := cumulative + weights[i];
			if( roll <= cumulative )
				return templates[i];
			endif
		endfor
		return templates[Len(templates)];
	endif

	return templates[ Random(Len(templates)) + 1 ];
endfunction

// ==========================================================================
// Carpentry deed-acceptance
// ==========================================================================

// Carpentry's crafting pipeline creates the placement DEED directly in the
// backpack for any recipe with a MakeDeed field (ApplyMaterialPropertiesDeed
// -- see pkg/systems/crafting/include/craftingfunctions.inc), not the
// recipe's own objtype. A BOD requesting such a recipe must therefore match
// against the deed's real objtype, not the recipe key -- every other craft
// returns the requested item unchanged.
function BOD_ExpectedFillObjtype(skillid, requested_item)
	if( skillid == SKILLID_CARPENTRY )
		var cfg := BOD_RecipeCfg(skillid);
		if( cfg )
			var elem := cfg[requested_item];
			if( elem )
				var deedtype := CInt(GetConfigInt(elem, "MakeDeed"));
				if( deedtype )
					return deedtype;
				endif
			endif
		endif
	endif
	return requested_item;
endfunction

// ==========================================================================
// CProp accessors (deed instances)
// ==========================================================================

function BOD_GetCraft(byref deed)
	return GetObjProperty(deed, "BODCraft");
endfunction

function BOD_GetSize(byref deed)
	return GetObjProperty(deed, "BODSize");
endfunction

function BOD_GetItemObjtype(byref deed)
	return GetObjProperty(deed, "BODItemObjtype");
endfunction

function BOD_GetAmount(byref deed)
	return GetObjProperty(deed, "BODAmount");
endfunction

function BOD_GetAmountFilled(byref deed)
	return GetObjProperty(deed, "BODAmountFilled");
endfunction

function BOD_GetExceptional(byref deed)
	return GetObjProperty(deed, "BODExceptional");
endfunction

function BOD_GetFilledMaterial(byref deed)
	return GetObjProperty(deed, "BODFilledMaterial");
endfunction

function BOD_GetTemplateKey(byref deed)
	return GetObjProperty(deed, "BODTemplateKey");
endfunction

function BOD_GetMaterial(byref deed)
	return GetObjProperty(deed, "BODMaterial");
endfunction

function BOD_GetSlotsFilled(byref deed)
	return GetObjProperty(deed, "BODSlotsFilled");
endfunction

function BOD_IsComplete(byref deed)
	if( CInt(BOD_GetSize(deed)) )
		var filled := BOD_GetSlotsFilled(deed);
		if( !filled or !Len(filled) )
			return 0;
		endif
		foreach f in filled
			if( !CInt(f) )
				return 0;
			endif
		endforeach
		return 1;
	endif
	return ( CInt(BOD_GetAmountFilled(deed)) >= CInt(BOD_GetAmount(deed)) );
endfunction

// A small local stand-in for omegacache.inc's GetItemDisplayName() -- not
// reused directly because omegacache.inc drags in :mdgumps:gumps /
// :mdgumps:gumps_ex, already in scope here (the deed gump is built on the
// same library), and a second include would be a duplicate-function
// compile error.
function BOD_ItemDisplayName(objtype)
	var desc := GetItemDescriptor(objtype);
	if( !desc )
		return Hex(objtype);
	endif
	if( desc.Desc and desc.Desc != "" )
		return desc.Desc;
	elseif( desc.Name and desc.Name != "" )
		return desc.Name;
	endif
	return Hex(objtype);
endfunction
