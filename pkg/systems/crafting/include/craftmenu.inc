// craftmenu.inc -- shared OSI-style crafting gump (category list -> recipe
// detail -> material picker), extracted from pkg/std/bowcraft/bowcraft.src
// (2026-08-29) so every crafting skill can reuse the same browsing/picking
// gump instead of each reimplementing it. Visual chrome (tiled panels,
// checkered overlay, title bar) matches the reference OSI crafting-menu
// mockup at pkg/opt/alryc/textcmd/test/osibsgump.src -- 2026-08-30 restyle,
// see that file for the literal raw-gump-string layout this was matched to.
//
// This include owns ONLY the gump flow and material-resolution bookkeeping.
// It never touches CheckSkill/CreateItemInBackpack/ConsumeResource -- the
// actual craft-attempt logic stays local to each skill's own .src (see
// CraftLogRecipe/CraftAmmoRecipe/CraftLogBulk/CraftAmmoBulk in bowcraft.src
// for the reference pattern a new skill's crafting functions should follow).
//
// Recipe cfg schema (see pkg/std/bowcraft/bowcraft.cfg for a worked example):
//   Element <objtype>
//   {
//       Name              <display name>
//       Category          <free-text category, drives the category list>
//       Skill             <difficulty, 1-150 -- this shard's real skill cap>
//       Bulk              <optional: 1 if a "Make Bulk" option should show>
//       Material          <amount needed of slot 1>
//       MaterialType      <"Log", or any other family name recognised by
//                          CraftMenu_IsPickableFamily below, or a free label
//                          for a single fixed objtype (e.g. "Shaft")>
//       Material1Objtype  <slot 1's real objtype -- required unless
//                          MaterialType is a pickable family>
//       Material2/Material2Type/Material2Objtype  <optional second slot>
//       Material3/Material3Type/Material3Objtype  <optional third slot>
//       ResultObjtype     <optional: crafted objtype, if different from the
//                          Element's own key -- see bowcraft.cfg's "Improve"
//                          recipes>
//   }
//
// "Pickable family" material types are MaterialTypes that can resolve to
// more than one real objtype (e.g. any color of Log, or any kind of Hide), so
// they need a picker gump and a family-wide total instead of a single fixed
// objtype. "Log" (bowcraft) and "Hide" (tailoring) are implemented. Add new
// families by extending CraftMenu_IsPickableFamily and the five
// CraftMenu_*Family* dispatch functions below -- Ingot (blacksmithy/
// tinkering) is a known future addition, not yet implemented. Not every
// multi-color material needs a family: tailoring's Cloth is a single fixed
// objtype (UOBJ_MUTLI_CLOTH) whose crafted item just inherits the source
// item's own .color, exactly like Log does -- it uses a plain
// Material1Objtype slot, no family logic at all.
//
// PERFORMANCE: CraftMenu_ShowCategoryGump must call CraftMenu_CanMakeElement
// once per recipe in the whole cfg to decide what's visible. Each of those
// calls independently re-totals its material -- for a "pickable family"
// slot (Hide, Log) that means a full backpack scan plus a dozen-plus Omega
// Cache lookups, EVERY TIME, even though the total is identical for every
// recipe sharing that family. With ~130 Hide recipes and ~90 fixed-Cloth
// recipes (tailoring.cfg), that was measured taking several seconds to
// build one gump. CraftMenu_CanMakeElement/HasEnoughOfSlot/GetFamilyTotal/
// GetFixedMaterialTotal all take an optional `cache` dictionary (keyed by
// "family:<type>" or "fixed:<objtype>") -- CraftMenu_ShowCategoryGump builds
// one and passes it through its whole counting pass, so each distinct
// material is only ever totalled once per gump build. Pass 0 (the default)
// to skip caching for one-off calls (CraftMenu_ShowDetailGump only ever
// resolves the one recipe's own slots, so it doesn't need this).

use uo;
use os;
use cfgfile;
use basic;
use basicio;

include ":gumps:gumps";
include "include/client";
include "include/random";
include "include/objtype";
include "include/itemutil";
include "include/autoloop";
include "include/utility";
include "include/resourcemanager";

const CRAFTMENU_BTN_BACK           := 900001;
const CRAFTMENU_BTN_MAKE_MAX       := 900003;
const CRAFTMENU_BTN_MAKE_NUMBER    := 900004;
const CRAFTMENU_BTN_MAKE_NOW       := 900005;
const CRAFTMENU_BTN_MAKE_BULK      := 900006;
const CRAFTMENU_BTN_AUTOLOOP       := 900007; // NOTE: distinct from every per-craft .src extra_options sentinel (900002, 900021-900023 in use as of 2026-08-30) -- pick a new .src sentinel outside 900001-900012 and 900021-900023 to avoid colliding with this or future shared buttons.
const CRAFTMENU_BTN_PICK_MATERIAL  := 900010;
const CRAFTMENU_BTN_PICK_MATERIAL2 := 900011;
const CRAFTMENU_BTN_PICK_MATERIAL3 := 900012;
const CRAFTMENU_BTN_MATERIAL_BASE  := 800000; // + index into a family picker's option list
const CRAFTMENU_BTN_CATEGORY_BASE  := 910000; // + index into the categories list (sidebar click / bank switch)

// OSI-style chrome shared by every gump this include draws -- backdrop
// resizepic, the leather/stone tile used to fill panels, and a debug-print
// toggle (set 1 to get build-time timing in the server console).
const CRAFTMENU_BACKDROP  := 5054;
const CRAFTMENU_PANEL_BG  := 2624;
const CRAFTMENU_DEBUG_ON  := 1;

const CRAFTMENU_SIDEBAR_BANK_SIZE  := 10; // categories per sidebar page (row 11 reserved for << >> nav)
const CRAFTMENU_CATEGORY_PAGE_SIZE := 10; // recipes per selections-panel page (row 11 reserved for Prev/Next nav)

function CraftMenu_Debug( msg )
	if( CRAFTMENU_DEBUG_ON )
		Print( "[craftmenu] " + msg );
	endif
endfunction


// All gump text goes through these two rather than calling GFTextLine
// directly. Briefly tried routing this through GFHTMLArea (HTMLGump
// elements) to get real <center> support and see if it also explained a
// font mismatch against osibsgump's screenshot -- it didn't fix the font
// (that was never an HTML-vs-text issue), and on tailoring's 258-recipe/
// 33-page gump it blew the client's gump packet buffer ("Buffer length
// exceeded", gump wouldn't even open). Reverted to GFTextLine, which has no
// layout engine at all (fixed x,y, no centering) -- CraftMenu_CenteredText
// fakes centering by estimating the classic gump font's average character
// width. Good enough for short header labels; don't rely on it for
// long/variable-length strings.
const CRAFTMENU_APPROX_CHAR_WIDTH := 8;

function CraftMenu_Text( byref gump, x, y, width, text )
	GFTextLine( gump, x, y, 1153, text );
endfunction


function CraftMenu_CenteredText( byref gump, x, y, width, text )
	var offset := CInt( (width - len(text) * CRAFTMENU_APPROX_CHAR_WIDTH) / 2 );
	if( offset < 0 )
		offset := 0;
	endif
	GFTextLine( gump, x + offset, y, 1153, text );
endfunction


function CraftMenu_GetCategories( cfg )
	var cats := array{};
	var seen := dictionary;
	foreach key in GetConfigStringKeys( cfg )
		var cat := cfg[key].Category;
		if( cat and !seen[cat] )
			seen[cat] := 1;
			cats.append(cat);
		endif
	endforeach
	return cats;
endfunction


// ------------------------------------------------------------------
// Pickable-family dispatch. "Log" and "Hide" exist today.
// ------------------------------------------------------------------

function CraftMenu_IsPickableFamily( materialType )
	return materialType == "Log" or materialType == "Hide" or materialType == "Cloth" or materialType == "Ingot";
endfunction


function CraftMenu_GetFamilyTotal( character, materialType, byref cache )
	var cacheKey := "family:" + materialType;
	if( cache.Exists(cacheKey) )
		return cache[cacheKey];
	endif

	var total := 0;
	if( materialType == "Log" )
		total := CraftMenu_GetLogFamilyTotal( character );
	elseif( materialType == "Hide" )
		total := CraftMenu_GetHideFamilyTotal( character );
	elseif( materialType == "Cloth" )
		total := CraftMenu_GetClothFamilyTotal( character );
	elseif( materialType == "Ingot" )
		total := CraftMenu_GetIngotFamilyTotal( character );
	endif

	cache[cacheKey] := total;
	return total;
endfunction


function CraftMenu_EnumerateFamilyOptions( character, materialType )
	if( materialType == "Log" )
		return CraftMenu_EnumerateLogOptions( character );
	elseif( materialType == "Hide" )
		return CraftMenu_EnumerateHideOptions( character );
	elseif( materialType == "Cloth" )
		return CraftMenu_EnumerateClothOptions( character );
	elseif( materialType == "Ingot" )
		return CraftMenu_EnumerateIngotOptions( character );
	endif
	return array{};
endfunction


function CraftMenu_FamilyOptionToRequest( character, materialType, opt )
	if( materialType == "Log" )
		return CraftMenu_LogOptionToRequest( character, opt );
	elseif( materialType == "Hide" )
		return CraftMenu_HideOptionToRequest( character, opt );
	elseif( materialType == "Cloth" )
		return CraftMenu_ClothOptionToRequest( character, opt );
	elseif( materialType == "Ingot" )
		return CraftMenu_IngotOptionToRequest( character, opt );
	endif
	return 0;
endfunction


function CraftMenu_FamilyOptionDisplayName( materialType, objtype )
	if( materialType == "Log" )
		var logs_cfg := ReadConfigFile( ":carpentry:carpentry" );
		var name := logs_cfg[objtype].Name;
		if( !name or name == "" )
			name := GetItemDisplayName(objtype);
		endif
		return name;
	elseif( materialType == "Hide" )
		var hides_cfg := ReadConfigFile( ":tailoring:tailoring" );
		var hname := hides_cfg[objtype].Name;
		if( !hname or hname == "" )
			hname := GetItemDisplayName(objtype);
		endif
		return hname;
	endif
	return GetItemDisplayName(objtype);
endfunction


// Blacksmithy/Tinkering-style material-quality scaling: the "Difficulty" a
// given family-member objtype contributes to a recipe's effective skill (see
// CraftMenu_CanMakeElement's header comment). Log/Ingot both keep their
// quality tables in their own craft's cfg -- the same files
// CraftMenu_FamilyOptionDisplayName above already reads names from -- so a
// caller with a "plain" family like Hide/Cloth just never sets qualityDivider
// and this is never consulted.
function CraftMenu_GetFamilyQualityDifficulty( materialType, objtype )
	if( materialType == "Log" )
		var logs_cfg := ReadConfigFile( ":carpentry:carpentry" );
		return CInt( logs_cfg[objtype].Difficulty );
	elseif( materialType == "Ingot" )
		var ingots_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );
		return CInt( ingots_cfg[objtype].Difficulty );
	endif
	return 0;
endfunction


function CraftMenu_ResolveFamilyDefault( character, materialType )
	if( materialType == "Log" )
		return CraftMenu_ResolveLogDefault( character );
	elseif( materialType == "Cloth" )
		return CraftMenu_ResolveClothDefault( character );
	elseif( materialType == "Hide" )
		return CraftMenu_ResolveHideDefault( character );
	elseif( materialType == "Ingot" )
		return CraftMenu_ResolveIngotDefault( character );
	endif

	// Any future family without its own "plain" default just takes
	// whichever option the player currently has the most of. On an exact
	// tie the first candidate in that family's enumeration order wins
	// (strict ">" below) -- confirmed 2026-08-30 (against Hide, before it
	// got its own plain-first default above) that this is why a pick can
	// shift after crafting: consuming the current winner down below an
	// exactly-tied runner-up hands the tie to that runner-up, which looks
	// like "random" flip-flopping but is really just a real stock change
	// reacted to correctly.
	var options := CraftMenu_EnumerateFamilyOptions( character, materialType );
	var best;
	var bestTotal := 0;
	foreach opt in options
		var req := CraftMenu_FamilyOptionToRequest( character, materialType, opt );
		var total := GetAvailableResource(character, req).total;
		if( total > bestTotal )
			bestTotal := total;
			best := req;
		endif
	endforeach
	return best;
endfunction


// ------------------------------------------------------------------
// Log family (bowcraft/carpentry raw material -- any color of Log).
// ------------------------------------------------------------------

function CraftMenu_GetLogFamilyTotal( character )
	var total := 0;
	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsLog(item) )
			total := total + item.amount;
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		total := total + GetStoredAmountByObjtype( access.df, UOBJ_LOGS, 0 );
		total := total + GetStoredAmountByObjtype( access.df, UOBJ_LOGS_GODDESS, 0 );
		var c;
		for( c := UOBJ_LOGS_START; c <= UOBJ_LOGS_END; c := c + 1 )
			total := total + GetStoredAmountByObjtype( access.df, c, 0 );
		endfor
	endif

	return total;
endfunction


function CraftMenu_EnumerateLogOptions( character )
	var options := array{};
	var seen := dictionary;

	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsLog(item) and !seen[CInt(item.objtype)] )
			seen[CInt(item.objtype)] := 1;
			options.append( struct{ objtype := CInt(item.objtype), item := item } );
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		var candidates := array{ UOBJ_LOGS, UOBJ_LOGS_GODDESS };
		var c;
		for( c := UOBJ_LOGS_START; c <= UOBJ_LOGS_END; c := c + 1 )
			candidates.append(c);
		endfor
		foreach ot in candidates
			if( !seen[CInt(ot)] )
				var amt := GetStoredAmountByObjtype( access.df, ot, 0 );
				if( amt > 0 )
					seen[CInt(ot)] := 1;
					options.append( struct{ objtype := CInt(ot), item := 0 } );
				endif
			endif
		endforeach
	endif

	return options;
endfunction


function CraftMenu_LogOptionToRequest( character, opt )
	if( opt.item )
		return MakeBackpackRequest( character, opt.item );
	endif
	return CraftMenu_GetMaterialRequest( character, opt.objtype );
endfunction


function CraftMenu_ResolveLogDefault( character )
	var plainRequest := CraftMenu_GetMaterialRequest( character, UOBJ_LOGS );
	if( GetAvailableResource(character, plainRequest).total > 0 )
		return plainRequest;
	endif

	var options := CraftMenu_EnumerateLogOptions( character );
	var best;
	var bestTotal := 0;
	foreach opt in options
		var req := CraftMenu_LogOptionToRequest( character, opt );
		var total := GetAvailableResource(character, req).total;
		if( total > bestTotal )
			bestTotal := total;
			best := req;
		endif
	endforeach
	return best;
endfunction


// ------------------------------------------------------------------
// Ingot family (blacksmithy/tinkering raw material -- any color of ore
// ingot, plus Gold/Silver/melted-copper-coin). Quality/difficulty scaling
// for this family is handled separately -- see CraftMenu_GetFamilyQuality-
// Difficulty and CraftMenu_CanMakeElement's qualityDivider parameter -- this
// section is only about which objtypes count and how much of each you have.
// ------------------------------------------------------------------

function CraftMenu_IngotObjtypeCandidates()
	var candidates := array{ UOBJ_GOLD_INGOT, UOBJ_SILVER_INGOT, UOBJ_COIN_COPPER_INGOT };
	var c;
	for( c := UOBJ_INGOTS_START; c <= UOBJ_INGOTS_END; c := c + 1 )
		candidates.append(c);
	endfor
	for( c := UOBJ_INGOTS2_START; c <= UOBJ_INGOTS2_END; c := c + 1 )
		candidates.append(c);
	endfor
	return candidates;
endfunction


function CraftMenu_GetIngotFamilyTotal( character )
	var total := 0;
	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsIngot(item) )
			total := total + item.amount;
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_IngotObjtypeCandidates()
			total := total + GetStoredAmountByObjtype( access.df, ot, 0 );
		endforeach
	endif

	return total;
endfunction


function CraftMenu_EnumerateIngotOptions( character )
	var options := array{};
	var seen := dictionary;

	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsIngot(item) and !seen[CInt(item.objtype)] )
			seen[CInt(item.objtype)] := 1;
			options.append( struct{ objtype := CInt(item.objtype), item := item } );
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_IngotObjtypeCandidates()
			if( !seen[CInt(ot)] )
				var amt := GetStoredAmountByObjtype( access.df, ot, 0 );
				if( amt > 0 )
					seen[CInt(ot)] := 1;
					options.append( struct{ objtype := CInt(ot), item := 0 } );
				endif
			endif
		endforeach
	endif

	return options;
endfunction


function CraftMenu_IngotOptionToRequest( character, opt )
	if( opt.item )
		return MakeBackpackRequest( character, opt.item );
	endif
	return CraftMenu_GetMaterialRequest( character, opt.objtype );
endfunction


// Plain Iron first, since that's the "normal" material players expect by
// default; otherwise fall back to whichever ingot the player has the most
// of, same shape as CraftMenu_ResolveLogDefault.
function CraftMenu_ResolveIngotDefault( character )
	var plainRequest := CraftMenu_GetMaterialRequest( character, UOBJ_IRON_INGOT );
	if( GetAvailableResource(character, plainRequest).total > 0 )
		return plainRequest;
	endif

	var options := CraftMenu_EnumerateIngotOptions( character );
	var best;
	var bestTotal := 0;
	foreach opt in options
		var req := CraftMenu_IngotOptionToRequest( character, opt );
		var total := GetAvailableResource(character, req).total;
		if( total > bestTotal )
			bestTotal := total;
			best := req;
		endif
	endforeach
	return best;
endfunction


// ------------------------------------------------------------------
// Hide family (tailoring raw material -- Leather plus every monster hide).
// ------------------------------------------------------------------

// Every hide objtype IsHide() recognises: the contiguous UOBJ_HIDES_START..
// UOBJ_HIDES_END range plus the handful of named exceptions that fall outside
// it (see itemutil.inc's IsHide for why -- Orc/Terathan/Angel/Silver Dragon
// hides aren't contiguous with the rest).
function CraftMenu_HideObjtypeCandidates()
	var candidates := array{ UOBJ_HIDE1, UOBJ_HIDE2, 0x702f, 0xdc13, 0xdc0a, 0x7030, 0xff17, 0xff18 };
	var c;
	for( c := UOBJ_HIDES_START; c <= UOBJ_HIDES_END; c := c + 1 )
		candidates.append(c);
	endfor
	return candidates;
endfunction


function CraftMenu_GetHideFamilyTotal( character )
	var total := 0;
	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsHide(item.objtype) )
			total := total + item.amount;
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_HideObjtypeCandidates()
			total := total + GetStoredAmountByObjtype( access.df, ot, 0 );
		endforeach
	endif

	return total;
endfunction


function CraftMenu_EnumerateHideOptions( character )
	var options := array{};
	var seen := dictionary;

	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsHide(item.objtype) and !seen[CInt(item.objtype)] )
			seen[CInt(item.objtype)] := 1;
			options.append( struct{ objtype := CInt(item.objtype), item := item } );
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_HideObjtypeCandidates()
			if( !seen[CInt(ot)] )
				var amt := GetStoredAmountByObjtype( access.df, ot, 0 );
				if( amt > 0 )
					seen[CInt(ot)] := 1;
					options.append( struct{ objtype := CInt(ot), item := 0 } );
				endif
			endif
		endforeach
	endif

	return options;
endfunction


function CraftMenu_HideOptionToRequest( character, opt )
	if( opt.item )
		return MakeBackpackRequest( character, opt.item );
	endif
	return CraftMenu_GetMaterialRequest( character, opt.objtype );
endfunction


// Regular Leather (UOBJ_HIDE1, or UOBJ_HIDE2 -- both are just plain cured
// leather, not a monster-specific hide) first, since that's the "normal"
// material players expect by default; otherwise fall back to whichever
// hide the player has the most of, same shape as CraftMenu_ResolveLogDefault
// and CraftMenu_ResolveClothDefault.
function CraftMenu_ResolveHideDefault( character )
	var plainRequest := CraftMenu_GetMaterialRequest( character, UOBJ_HIDE1 );
	if( GetAvailableResource(character, plainRequest).total > 0 )
		return plainRequest;
	endif

	var altRequest := CraftMenu_GetMaterialRequest( character, UOBJ_HIDE2 );
	if( GetAvailableResource(character, altRequest).total > 0 )
		return altRequest;
	endif

	var options := CraftMenu_EnumerateHideOptions( character );
	var best;
	var bestTotal := 0;
	foreach opt in options
		var req := CraftMenu_HideOptionToRequest( character, opt );
		var total := GetAvailableResource(character, req).total;
		if( total > bestTotal )
			bestTotal := total;
			best := req;
		endif
	endforeach
	return best;
endfunction


// ------------------------------------------------------------------
// Cloth family (tailoring raw material -- every "real cloth" graphic, see
// IsCloth()/UOBJ_CLOTH_START..END in itemutil.inc/objtype.inc).
//
// Investigated 2026-08-30: Bolts of Cloth (0x0F95-0x0F9C, 8 color variants)
// always scissor down into the SAME single graphic -- plain Cloth,
// UOBJ_MUTLI_CLOTH/0x1765 -- regardless of which bolt color was cut (see
// scissors.src's UOBJ_BOLT_START/END branch). That's the only cloth
// graphic the shard's bolt-cutting economy actually produces, which is why
// tailoring.cfg's Cloth slots were hardcoded to 0x1765 alone. The other 11
// graphics in the family (0x175D-0x1768 minus 0x1765) are valid items a
// player could still have from elsewhere (loot/spawner/GM) -- scissors.src
// treats THOSE as already-finished cloth and shears them into Bandages,
// not more Cloth, so they were never reachable through normal play either
// way. Making Cloth a pickable family (like Log/Hide) means a player who
// does have one of those other graphics can spend it here instead of it
// being unusable, while 0x1765 still resolves as the default everyone
// with bolts will have.
// ------------------------------------------------------------------

function CraftMenu_ClothObjtypeCandidates()
	var candidates := array{};
	var c;
	for( c := UOBJ_CLOTH_START; c <= UOBJ_CLOTH_END; c := c + 1 )
		candidates.append(c);
	endfor
	return candidates;
endfunction


function CraftMenu_GetClothFamilyTotal( character )
	var total := 0;
	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsCloth(item) )
			total := total + item.amount;
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_ClothObjtypeCandidates()
			total := total + GetStoredAmountByObjtype( access.df, ot, 0 );
		endforeach
	endif

	return total;
endfunction


function CraftMenu_EnumerateClothOptions( character )
	var options := array{};
	var seen := dictionary;

	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( IsCloth(item) and !seen[CInt(item.objtype)] )
			seen[CInt(item.objtype)] := 1;
			options.append( struct{ objtype := CInt(item.objtype), item := item } );
		endif
	endforeach

	var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
	if( access )
		foreach ot in CraftMenu_ClothObjtypeCandidates()
			if( !seen[CInt(ot)] )
				var amt := GetStoredAmountByObjtype( access.df, ot, 0 );
				if( amt > 0 )
					seen[CInt(ot)] := 1;
					options.append( struct{ objtype := CInt(ot), item := 0 } );
				endif
			endif
		endforeach
	endif

	return options;
endfunction


function CraftMenu_ClothOptionToRequest( character, opt )
	if( opt.item )
		return MakeBackpackRequest( character, opt.item );
	endif
	return CraftMenu_GetMaterialRequest( character, opt.objtype );
endfunction


// Plain Cloth (0x1765) first, since that's what bolt-cutting actually
// produces; otherwise fall back to whichever cloth graphic the player has
// the most of, same shape as CraftMenu_ResolveLogDefault.
function CraftMenu_ResolveClothDefault( character )
	var plainRequest := CraftMenu_GetMaterialRequest( character, UOBJ_MUTLI_CLOTH );
	if( GetAvailableResource(character, plainRequest).total > 0 )
		return plainRequest;
	endif

	var options := CraftMenu_EnumerateClothOptions( character );
	var best;
	var bestTotal := 0;
	foreach opt in options
		var req := CraftMenu_ClothOptionToRequest( character, opt );
		var total := GetAvailableResource(character, req).total;
		if( total > bestTotal )
			bestTotal := total;
			best := req;
		endif
	endforeach
	return best;
endfunction


// Lists every option in the given family currently available (backpack +
// cache) with a total, lets the player pick which one to craft with.
function CraftMenu_PickFamilyMaterial( character, materialType )

	var options := CraftMenu_EnumerateFamilyOptions( character, materialType );

	if( len(options) == 0 )
		SendSysMessage( character, "You don't have any of that available." );
		return 0;
	endif

	const CRAFTMENU_FAMILY_PAGE_SIZE := 10;
	var total_options := len(options);
	var pageCount := CInt(Ceil( Cdbl(total_options) / CRAFTMENU_FAMILY_PAGE_SIZE ));
	if( pageCount < 1 )
		pageCount := 1;
	endif

	var gump := GFCreateGump(60, 60, 440, 460);
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, CRAFTMENU_BACKDROP, 440, 460 );
	GFPicTiled( gump, 10, 10, 420, 22, CRAFTMENU_PANEL_BG );
	GFPicTiled( gump, 10, 37, 420, 380, CRAFTMENU_PANEL_BG );
	GFAddAlphaRegion( gump, 10, 10, 420, 407 );
	CraftMenu_CenteredText( gump, 10, 13, 420, Upper("Choose a Material") );

	// Page 0 is chrome ONLY -- item rows always start at page 1, matching the
	// same convention resourcemanager.inc's SelectMaterialFromList uses (page
	// 0 renders underneath every other page, so a page's worth of item rows
	// on page 0 bleeds through and overlaps every other page at the same
	// coordinates).
	var page;
	for( page := 1; page <= pageCount; page := page + 1 )
		GFPage( gump, page );

		var startIdx := (page - 1) * CRAFTMENU_FAMILY_PAGE_SIZE;
		var endIdx := Min(startIdx + CRAFTMENU_FAMILY_PAGE_SIZE, total_options) - 1;
		var y := 45;
		var idx;
		for( idx := startIdx; idx <= endIdx; idx := idx + 1 )
			var opt := options[idx+1];
			var req := CraftMenu_FamilyOptionToRequest( character, materialType, opt );
			var amt := GetAvailableResource(character, req).total;
			var name := CraftMenu_FamilyOptionDisplayName( materialType, opt.objtype );
			GFAddButton( gump, 16, y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_MATERIAL_BASE + idx );
			CraftMenu_Text( gump, 50, y, 350, name + " (" + amt + ")" );
			y := y + 25;
		endfor

		if( page > 1 )
			GFAddButton( gump, 16, 385, 4014, 4016, GF_PAGE_BTN, page - 1 );
			CraftMenu_Text( gump, 50, 386, 150, "Previous Page" );
		endif
		if( page < pageCount )
			GFAddButton( gump, 220, 385, 4005, 4007, GF_PAGE_BTN, page + 1 );
			CraftMenu_Text( gump, 255, 386, 150, "Next Page" );
		endif
	endfor

	var result := GFSendGump( character, gump );
	if( !result )
		return 0;
	endif

	var btn := CInt(result[0]);
	if( btn < CRAFTMENU_BTN_MATERIAL_BASE )
		return 0;
	endif

	var picked_idx := btn - CRAFTMENU_BTN_MATERIAL_BASE;
	if( picked_idx < 0 or picked_idx >= total_options )
		return 0;
	endif

	return CraftMenu_FamilyOptionToRequest( character, materialType, options[picked_idx+1] );

endfunction


// ------------------------------------------------------------------
// Generic per-slot resolution (fixed-objtype slots + family slots alike).
// ------------------------------------------------------------------

function CraftMenu_GetMaterialRequest( character, objtype )
	var item := CraftMenu_FindTypeInBackpack( character, objtype );
	if( item )
		return MakeBackpackRequest( character, item );
	endif

	var dataFileHandle := 0;
	if( !GetObjProperty(character, "omegacache_no_autodraw") )
		var access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
		if( access )
			dataFileHandle := access.df;
		endif
	endif

	// No physical item to read .color from (cache-only) -- fall back to the
	// objtype's own registered default color instead of hardcoding 0, or a
	// colored material crafted purely from the cache loses its hue entirely.
	var default_color := 0;
	var desc_info := GetItemDescriptor(objtype);
	if( desc_info and desc_info.Color )
		default_color := CInt(desc_info.Color);
	endif

	return struct{
		objtype              := CInt(objtype),
		key                  := 0,
		color                := default_color,
		quality              := 0,
		preferredSourceOrder := array{ BACKPACK, OMEGA_CACHE },
		dataFileHandle       := dataFileHandle,
		houseSerial          := 0,
		leaseKey             := 0
	};
endfunction


function CraftMenu_FindTypeInBackpack( character, objtype )
	foreach item in EnumerateItemsInContainer( character.backpack, ENUMERATE_ROOT_ONLY )
		if( item.objtype == objtype )
			return item;
		endif
	endforeach
	return 0;
endfunction


function CraftMenu_GetFixedMaterialTotal( character, objtype, byref cache )
	var cacheKey := "fixed:" + CStr(CInt(objtype));
	if( cache.Exists(cacheKey) )
		return cache[cacheKey];
	endif

	var total := GetAvailableResource( character, CraftMenu_GetMaterialRequest(character, objtype) ).total;

	cache[cacheKey] := total;
	return total;
endfunction


function CraftMenu_HasEnoughOfSlot( character, materialType, amountNeeded, fixedObjtype, byref cache )
	if( CraftMenu_IsPickableFamily(materialType) )
		return CraftMenu_GetFamilyTotal(character, materialType, cache) >= amountNeeded;
	else
		return CraftMenu_GetFixedMaterialTotal(character, CInt(fixedObjtype), cache) >= amountNeeded;
	endif
endfunction


// DescribeSlot only ever resolves the one recipe's own slot (not a loop over
// many recipes), so it doesn't benefit from memoizing across calls -- it
// still needs to pass a cache dictionary through (CraftMenu_GetFixedMaterialTotal
// requires byref cache now, see the PERFORMANCE note at the top of this
// file), so it just uses a fresh, single-use one each time.
function CraftMenu_DescribeSlot( character, materialType, fixedObjtype, chosenRequest )
	if( chosenRequest )
		return GetItemDisplayName(CInt(chosenRequest.objtype)) + " (" + GetAvailableResource(character, chosenRequest).total + " available)";
	endif
	if( CraftMenu_IsPickableFamily(materialType) )
		return "not chosen yet (click to pick one)";
	endif
	var resolved := CInt(fixedObjtype);
	var scratch_cache := dictionary;
	return GetItemDisplayName(resolved) + " (" + CraftMenu_GetFixedMaterialTotal(character, resolved, scratch_cache) + " available)";
endfunction


// If the player never opened the picker for this slot, resolve a sensible
// default automatically: for a family slot, defer to
// CraftMenu_ResolveFamilyDefault; for everything else there's only one
// possible objtype anyway.
function CraftMenu_ResolveDefaultSlot( character, materialType, fixedObjtype )
	if( !CraftMenu_IsPickableFamily(materialType) )
		return CraftMenu_GetMaterialRequest( character, CInt(fixedObjtype) );
	endif
	return CraftMenu_ResolveFamilyDefault( character, materialType );
endfunction


// ------------------------------------------------------------------
// Category + recipe-detail gumps.
// ------------------------------------------------------------------

// Reads the display name for a skill id out of :shilhook:skillsdef (the same
// data source starteqp.inc's GetSkillName uses) -- kept local instead of
// including starteqp.inc so this stays a self-contained one-line lookup
// rather than pulling in that file's other, unrelated content.
function CraftMenu_GetSkillName( skillid )
	var skills_cfg := ReadConfigFile( ":shilhook:skillsdef" );
	var name := skills_cfg[skillid].name;
	if( !name or name == "" )
		return "Skill " + CStr(CInt(skillid));
	endif
	return name;
endfunction


// Same behavior as typing ".autoloop" with no argument (see
// scripts/textcmd/player/autoloop.src) -- always prompts, since a gump
// button click never carries an amount the way ".autoloop 5" would. Every
// craft menu offers this as its first Options entry so a player who has
// never configured autoloop isn't stuck getting the "how many loops?" gump
// on every single Make click.
function CraftMenu_ConfigureAutoloop( character )
	const CRAFTMENU_AUTOLOOP_MAX := 9999;

	var number := CInt( SendTextEntryGump(character, "How many? [0-"+CRAFTMENU_AUTOLOOP_MAX+"]", TE_CANCEL_ENABLE, TE_STYLE_NORMAL, 4) );

	if( number > CRAFTMENU_AUTOLOOP_MAX )
		number := CRAFTMENU_AUTOLOOP_MAX;
	elseif( number < 0 )
		number := 0;
	endif

	SetObjProperty( character, "autoloop", number );

	if( number == 0 )
		SendSysMessage( character, "Autoloop configured to ask you how many loops to do.", 3, 89 );
	elseif( number == 1 )
		SendSysMessage( character, "Autoloop configured to loop 1 time.", 3, 89 );
	else
		SendSysMessage( character, "Autoloop configured to loop " + number + " times.", 3, 89 );
	endif
endfunction


// True if the player currently has enough skill and enough of every required
// material (summed across the whole family for a pickable-family slot, since
// we haven't asked which specific one yet) to make at least 1 of this
// element. appear_at is the (usually negative) skill-visibility margin below
// the recipe's own Skill difficulty at which it starts showing up. `cache`
// is a dictionary the caller keeps across a whole loop of these calls (see
// the PERFORMANCE note at the top of this file) -- it's byref because this
// dialect copies structs/dictionaries into a call by value otherwise, which
// would silently throw away every memoized total on return.
//
// CoSkillid/CoSkill (optional) add a second skill requirement, gated the
// same way as the primary Skill -- see bowcraft.cfg's Fire Bow/Ice Bow/
// Thunder Crossbow (Magery, to imbue the elemental reagent) for the
// reference example. Unlike carpentry.cfg's CoSkillid/CoSkill/CoMaterial,
// there's no separate co-material check here -- the "second thing" is
// already whatever the recipe's own Material2/Material3 slot models.
//
// qualityDivider/materialQualityDifficulty (optional, both default off)
// support Blacksmithy/Tinkering/Carpentry-style material-quality scaling,
// where a recipe gets HARDER the rarer the material family-member you use
// (see blacksmithy.cfg's ore Difficulty table). Since the category list is
// built before the player has picked a specific ingot/log, visibility is
// judged against whichever family-member CraftMenu_ResolveDefaultSlot would
// pick right now -- the caller (CraftMenu_ShowCategoryGump) resolves that
// ONCE per MaterialType per gump build (not once per recipe) and passes the
// resulting Difficulty number in here. This only ever applies to the
// PRIMARY slot -- CoSkillid/CoSkill (the co-material's skill gate) is always
// a flat requirement, not scaled by the co-material's own quality, even for
// Carpentry's Ingot/Cloth/Bone co-materials. The actual crafting roll still
// uses whichever material the player really ends up with, computed locally
// in that skill's own .src via CraftMenu_GetFamilyQualityDifficulty.
function CraftMenu_CanMakeElement( character, key, cfg, skill_id, appear_at, byref cache, qualityDivider := 0, materialQualityDifficulty := 0 )

	var effective_skill := GetEffectiveSkill( character, skill_id );
	var diff := cfg[key].Skill;
	if( qualityDivider > 0 )
		diff := diff + Cint( materialQualityDifficulty / qualityDivider );
	endif
	if( diff < 1 )
		diff := 1;
	elseif( diff > 150 )
		diff := 150;
	endif
	if( effective_skill < ( diff + appear_at ) )
		return 0;
	endif

	if( cfg[key].CoSkillid )
		var co_diff := cfg[key].CoSkill;
		if( co_diff < 1 )
			co_diff := 1;
		elseif( co_diff > 150 )
			co_diff := 150;
		endif
		if( GetEffectiveSkill( character, CInt(cfg[key].CoSkillid) ) < ( co_diff + appear_at ) )
			return 0;
		endif
	endif

	if( !CraftMenu_HasEnoughOfSlot( character, cfg[key].MaterialType, cfg[key].Material, cfg[key].Material1Objtype, cache ) )
		return 0;
	endif
	if( cfg[key].Material2Type )
		if( !CraftMenu_HasEnoughOfSlot( character, cfg[key].Material2Type, cfg[key].Material2, cfg[key].Material2Objtype, cache ) )
			return 0;
		endif
	endif
	if( cfg[key].Material3Type )
		if( !CraftMenu_HasEnoughOfSlot( character, cfg[key].Material3Type, cfg[key].Material3, cfg[key].Material3Objtype, cache ) )
			return 0;
		endif
	endif

	return 1;

endfunction


// Redraws the left "CATEGORIES" sidebar for one bank of up to
// CRAFTMENU_SIDEBAR_BANK_SIZE categories, plus prev/next-bank icon buttons
// (bottom-left/bottom-right of the panel, no text label -- the matching
// icon-only page buttons on the Selections side already make the meaning
// obvious) if there's more than one bank.
//
// Category and bank-nav buttons both close the gump with a
// CRAFTMENU_BTN_CATEGORY_BASE+index sentinel instead of client-side paging --
// see CraftMenu_ShowCategoryGump's header comment for why (only ONE
// category's own pages are ever built into a single gump send now, so
// switching categories is a fresh server round-trip, not a page flip).
// Switching banks jumps to the first category in the target bank.
function CraftMenu_DrawSidebarBank( byref gump, categories, bank, bank_count, bank_size )

	CraftMenu_CenteredText( gump, 10, 39, 200, Upper("Categories") );

	var start_idx := bank * bank_size;
	var end_idx := Min(start_idx + bank_size, len(categories)) - 1;
	var row_y := 60;
	var idx;
	for( idx := start_idx; idx <= end_idx; idx := idx + 1 )
		GFAddButton( gump, 15, row_y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_CATEGORY_BASE + idx );
		CraftMenu_Text( gump, 50, row_y, 145, categories[idx+1] );
		row_y := row_y + 20;
	endfor

	if( bank_count > 1 )
		var nav_y := 280;
		if( bank > 0 )
			var prev_first_idx := (bank-1) * bank_size;
			GFAddButton( gump, 15, nav_y, 4014, 4016, GF_CLOSE_BTN, CRAFTMENU_BTN_CATEGORY_BASE + prev_first_idx );
		endif
		if( bank < bank_count - 1 )
			var next_first_idx := (bank+1) * bank_size;
			GFAddButton( gump, 178, nav_y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_CATEGORY_BASE + next_first_idx );
		endif
	endif

endfunction


// Category + selection gump. Returns the chosen Element's objtype (key into
// cfg), the .value of whichever extra_options entry was clicked, or 0 on
// cancel. extra_options is an array of struct{label, value} rendered in the
// OPTIONS footer panel (pass array{} for none) -- value is returned
// verbatim, so the caller's sentinel just needs to not collide with any real
// recipe-key objtype in cfg.
//
// Both panels paginate independently: a category with more recipes than fit
// on one page gets Previous/Next Page buttons (tailoring's banner/gonfalon
// buckets need this); a cfg with more categories than fit in one sidebar
// gets << Categories / Categories >> bank navigation (tailoring's 17
// categories need this -- 11 was the most the reference OSI mockup ever had
// to show, so this is real pagination the mockup itself never demonstrated).
//
// PERFORMANCE/SCALE: only the CURRENTLY SELECTED category's own pages are
// ever built into one gump send -- switching categories or banks closes the
// gump and rebuilds+resends for the new selection (see the outer while loop
// below and CraftMenu_DrawSidebarBank's CRAFTMENU_BTN_CATEGORY_BASE
// buttons), rather than baking every category's every page into one giant
// client-side-flippable gump. Carpentry's 368 recipes across 20 categories
// (49 total pages) hit "Buffer length exceeded" under the old all-in-one-
// gump approach; bounding each send to one category's own page count (worst
// case ~11 pages for its biggest category) fixed it, and scales to however
// many categories/recipes a future craft adds without hitting the same wall
// again.
function CraftMenu_ShowCategoryGump( character, cfg, title, skill_id, appear_at, extra_options, qualityDivider := 0 )

	var build_start := ReadGameClock();
	CraftMenu_Debug( "ShowCategoryGump(" + title + ") starting" );

	var categories := CraftMenu_GetCategories( cfg );
	var all_keys := GetConfigStringKeys( cfg );
	var total_cache := dictionary;

	// Blacksmithy/Tinkering/Carpentry-style quality scaling (qualityDivider >
	// 0): each distinct MaterialType's Difficulty is resolved against its
	// current default pick ONCE for this whole gump build (not once per
	// recipe) -- see CraftMenu_CanMakeElement's header comment for why. This
	// only ever covers the PRIMARY slot -- CoSkillid/CoSkill is always flat,
	// not scaled by the co-material's own quality.
	var quality_by_type := dictionary;
	if( qualityDivider > 0 )
		foreach key in all_keys
			var qualKey := cfg[key].MaterialType;
			if( qualKey and !quality_by_type.Exists(qualKey) )
				var defaultReq := CraftMenu_ResolveDefaultSlot( character, qualKey, cfg[key].Material1Objtype );
				var qualDiff := 0;
				if( defaultReq )
					qualDiff := CraftMenu_GetFamilyQualityDifficulty( qualKey, CInt(defaultReq.objtype) );
				endif
				quality_by_type[qualKey] := qualDiff;
			endif
		endforeach
	endif

	// Counting pass: figure out which recipes are craftable per category
	// (and therefore how many pages each category needs), and which sidebar
	// bank each category belongs to. Cheap -- server-side only, no gump
	// packet bytes -- so doing this for every category up front (even ones
	// the player never visits this gump-open) is fine. total_cache makes
	// every recipe sharing the same family/fixed material get totalled
	// exactly once for this whole pass, not once per recipe (see the
	// PERFORMANCE note at the top of this file).
	var cat_keys := array{};
	var cat_page_counts := array{};
	var cat_bank := array{};
	var cat_idx := 0;
	foreach cat in categories
		cat_idx := cat_idx + 1;
		var keys := array{};
		foreach key in all_keys
			var matQual := 0;
			if( qualityDivider > 0 )
				matQual := quality_by_type[cfg[key].MaterialType];
			endif
			if( cfg[key].Category == cat and CraftMenu_CanMakeElement(character, key, cfg, skill_id, appear_at, total_cache, qualityDivider, matQual) )
				keys.append(key);
			endif
		endforeach

		var pageCount;
		if( len(keys) == 0 )
			pageCount := 1;
		else
			pageCount := CInt(Ceil( Cdbl(len(keys)) / CRAFTMENU_CATEGORY_PAGE_SIZE ));
		endif

		cat_keys.append(keys);
		cat_page_counts.append(pageCount);
		cat_bank.append( CInt(Floor( Cdbl(cat_idx-1) / CRAFTMENU_SIDEBAR_BANK_SIZE )) );
	endforeach

	CraftMenu_Debug( "ShowCategoryGump(" + title + ") counting pass done in " + (ReadGameClock()-build_start)
		+ "s -- categories=" + len(categories) + " recipes=" + len(all_keys) + " materials_totalled=" + total_cache.Size() );

	var total_categories := len(categories);
	var bank_count;
	if( total_categories == 0 )
		bank_count := 1;
	else
		bank_count := CInt(Ceil( Cdbl(total_categories) / CRAFTMENU_SIDEBAR_BANK_SIZE ));
	endif

	var current_cat_idx := 1; // 1-based index into categories/cat_keys/etc.

	while( 1 )

		// Gump is 655 wide (was 530, +125) so the Selections panel has room
		// for the longest recipe name this framework has to accommodate --
		// Carpentry's "Large Forge Deed [Bellows Both Ends]" (~36 chars,
		// ~288px at CRAFTMENU_APPROX_CHAR_WIDTH) -- without crowding the
		// Skill column. See the Name/Skill column comments below for the
		// resulting widths/positions.
		var gump := GFCreateGump(50, 50, 655, 497);

		// -- Static chrome (drawn once, underneath every LOCAL page of the
		// currently selected category): backdrop, title bar, both panel
		// backgrounds, the checkered overlay, and the OPTIONS footer.
		//
		// The Categories/Selections panels are 20px taller than they used to
		// be (270 instead of 250) to make room for a full buffer row above
		// their page-nav buttons, and the whole OPTIONS block is shifted
		// down that same 20px to make room -- its own content start moves up
		// within the block by roughly that same amount, so this is a wash
		// for OPTIONS and nets out to the same total gump height. See
		// CraftMenu_DrawSidebarBank and the Selections page-nav buttons
		// below for the matching nav_y.
		GFResizePic( gump, 0, 0, CRAFTMENU_BACKDROP, 655, 497 );
		GFPicTiled( gump, 10, 10, 635, 22, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 10, 37, 200, 270, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 215, 37, 430, 270, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 10, 312, 150, 45, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 165, 312, 480, 45, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 10, 362, 635, 125, CRAFTMENU_PANEL_BG );
		GFAddAlphaRegion( gump, 10, 10, 635, 477 );
		CraftMenu_CenteredText( gump, 10, 13, 635, Upper(title) + " MENU" );
		CraftMenu_CenteredText( gump, 10, 314, 150, Upper("Options") );

		// Autoloop is always the first Options entry, on every craft's gump --
		// see CraftMenu_ConfigureAutoloop's header comment for why.
		var opt_y := 367;
		GFAddButton( gump, 15, opt_y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_AUTOLOOP );
		CraftMenu_Text( gump, 50, opt_y, 150, "Set Autoloop Count" );
		opt_y := opt_y + 25;

		foreach opt in extra_options
			GFAddButton( gump, 15, opt_y, 4005, 4007, GF_CLOSE_BTN, CInt(opt.value) );
			CraftMenu_Text( gump, 50, opt_y, 150, opt.label );
			opt_y := opt_y + 25;
		endforeach

		// -- Per-page content: only the SELECTED category's own pages.
		var keys := array{};
		var pageCount := 1;
		var bank := 0;
		if( total_categories > 0 )
			keys := cat_keys[current_cat_idx];
			pageCount := cat_page_counts[current_cat_idx];
			bank := cat_bank[current_cat_idx];
		endif
		var total_keys := len(keys);

		var p;
		for( p := 0; p < pageCount; p := p + 1 )
			GFPage( gump, p + 1 );

			CraftMenu_DrawSidebarBank( gump, categories, bank, bank_count, CRAFTMENU_SIDEBAR_BANK_SIZE );

			CraftMenu_CenteredText( gump, 215, 39, 430, Upper("Selections") );
			CraftMenu_CenteredText( gump, 565, 39, 75, Upper("Skill") );

			if( total_keys == 0 )
				CraftMenu_Text( gump, 220, 60, 415, "Nothing craftable in this category yet." );
			else
				var startIdx := p * CRAFTMENU_CATEGORY_PAGE_SIZE;
				var endIdx := Min(startIdx + CRAFTMENU_CATEGORY_PAGE_SIZE, total_keys) - 1;
				var row_y := 60;
				var idx;
				for( idx := startIdx; idx <= endIdx; idx := idx + 1 )
					var key := keys[idx+1];
					GFAddButton( gump, 220, row_y, 4005, 4007, GF_CLOSE_BTN, Cint(key) );
					// 300px fits "Large Forge Deed [Bellows Both Ends]" (the
					// longest recipe name this framework has to accommodate,
					// ~36 chars) without reaching into the Skill column.
					CraftMenu_Text( gump, 255, row_y, 300, cfg[key].Name );
					// Compact skill-requirement readout in place of OSI's
					// per-row "item properties" button (osibsgump.src's
					// 480,y 4011/4012 button) -- we don't have per-item
					// property data to preview, but the skill needed is
					// useful to see without opening the detail gump.
					// Recipes with a real CoSkillid (e.g. bowcraft's
					// elemental bows needing Magery -- see bowcraft.cfg)
					// show both requirements; everything else just shows
					// the one Skill value. When quality scaling is on, this
					// is adjusted the same way CraftMenu_CanMakeElement's
					// visibility check was, against the current default
					// material -- picking a rarer one in the detail gump
					// can still push the real requirement higher than shown.
					var displaySkill := CInt(cfg[key].Skill);
					if( qualityDivider > 0 )
						displaySkill := displaySkill + Cint( quality_by_type[cfg[key].MaterialType] / qualityDivider );
						if( displaySkill > 150 )
							displaySkill := 150;
						endif
					endif
					var skill_readout := CStr(displaySkill);
					if( cfg[key].CoSkillid )
						skill_readout := skill_readout + " : " + CStr(CInt(cfg[key].CoSkill));
					endif
					// Column sized/positioned off "150 : 150" (the biggest
					// this text ever gets, since 150 is this shard's real
					// skill cap -- see project_zh_skill_cap) so it sits
					// flush against the panel's right edge, leaving the
					// Name column beside it as much room as possible.
					CraftMenu_Text( gump, 565, row_y, 75, skill_readout );
					row_y := row_y + 20;
				endfor
			endif

			// Icon-only nav, bottom corners of the panel -- no text label
			// needed, matches the sidebar bank-nav buttons. y matches
			// CraftMenu_DrawSidebarBank's nav_y (280) so both panels leave
			// the same buffer row above their nav row. Local page numbers
			// (1..pageCount) since this send only ever contains one
			// category's pages now.
			if( p > 0 )
				GFAddButton( gump, 220, 280, 4014, 4016, GF_PAGE_BTN, p );
			endif
			if( p < pageCount - 1 )
				GFAddButton( gump, 605, 280, 4005, 4007, GF_PAGE_BTN, p + 2 );
			endif
		endfor

		CraftMenu_Debug( "ShowCategoryGump(" + title + ") built category " + current_cat_idx + " (" + pageCount + " pages) in "
			+ (ReadGameClock()-build_start) + "s, sending..." );

		var result := GFSendGump( character, gump );
		if( !result )
			return 0;
		endif

		var btn := CInt(result[0]);
		if( btn >= CRAFTMENU_BTN_CATEGORY_BASE and btn < CRAFTMENU_BTN_CATEGORY_BASE + total_categories )
			current_cat_idx := (btn - CRAFTMENU_BTN_CATEGORY_BASE) + 1;
		elseif( btn == CRAFTMENU_BTN_AUTOLOOP )
			CraftMenu_ConfigureAutoloop( character );
			// Stay on the same category/bank -- configuring autoloop isn't a
			// pick, just a settings tweak mid-browse.
		else
			return btn;
		endif

	endwhile

endfunction


// Detail page for one recipe: shows required materials with picker buttons,
// then Make Max/Make Number/Make Now/(Make Bulk). skill_label is used only
// for the "<skill_label> skill needed"/"Your <skill_label>" text lines.
// extra_notes is an array of extra strings shown in the NOTES row (pass
// array{} for none -- e.g. bowcraft's Log-only maker's-mark/color notices).
//
// Returns 0 if the player backed out, or a struct{mode, qty, slot1, slot2,
// slot3, maxCraftable} once they committed to crafting -- mode is "max",
// "number", "now", or "bulk"; qty is only meaningful for "bulk" (AutoLoop is
// already initialized with the right count for the other three modes before
// this returns). The caller is responsible for dispatching mode/slot* into
// its own crafting-execution functions.
function CraftMenu_ShowDetailGump( character, cfg, objtype, skill_id, skill_label, extra_notes, qualityDivider := 0 )

	var element := cfg[objtype];
	if( !element )
		return 0;
	endif

	// Pre-resolve a default for every slot up front (instead of only when
	// Make is clicked) so the description line shows a real material and
	// count right away -- e.g. "Log (140 available)" -- rather than
	// "not chosen yet" for slots the player almost never needs to hand-pick.
	// The Pick button still works for overriding to a specific variant.
	var slot1Request := CraftMenu_ResolveDefaultSlot( character, element.MaterialType, element.Material1Objtype );
	var slot2Request;
	if( element.Material2Type )
		slot2Request := CraftMenu_ResolveDefaultSlot( character, element.Material2Type, element.Material2Objtype );
	endif
	var slot3Request;
	if( element.Material3Type )
		slot3Request := CraftMenu_ResolveDefaultSlot( character, element.Material3Type, element.Material3Objtype );
	endif

	while( 1 )

		var gump := GFCreateGump(50, 50, 530, 460);
		GFResizePic( gump, 0, 0, CRAFTMENU_BACKDROP, 530, 460 );
		GFPicTiled( gump, 10, 10, 510, 22, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 10, 37, 510, 280, CRAFTMENU_PANEL_BG );
		GFPicTiled( gump, 10, 322, 510, 118, CRAFTMENU_PANEL_BG );
		GFAddAlphaRegion( gump, 10, 10, 510, 430 );
		CraftMenu_CenteredText( gump, 10, 13, 510, element.Name );

		// Quality scaling (Blacksmithy/Tinkering): the real requirement
		// depends on whichever material is CURRENTLY resolved in slot1 --
		// recomputed every redraw since Pick can change it mid-gump.
		var displaySkillNeeded := CInt(element.Skill);
		if( qualityDivider > 0 and slot1Request )
			var qualDiff := CraftMenu_GetFamilyQualityDifficulty( element.MaterialType, CInt(slot1Request.objtype) );
			displaySkillNeeded := displaySkillNeeded + Cint( qualDiff / qualityDivider );
			if( displaySkillNeeded > 150 )
				displaySkillNeeded := 150;
			endif
		endif

		var y := 42;
		CraftMenu_Text( gump, 16, y, 480, skill_label + " skill needed: " + displaySkillNeeded );
		y := y + 20;
		CraftMenu_Text( gump, 16, y, 480, "Your " + skill_label + ": " + GetEffectiveSkill(character, skill_id) );
		y := y + 20;

		if( element.CoSkillid )
			var co_skill_name := CraftMenu_GetSkillName( CInt(element.CoSkillid) );
			// CoSkill is always flat -- not scaled by the co-material's own
			// quality, unlike the primary skill above.
			CraftMenu_Text( gump, 16, y, 480, co_skill_name + " skill also needed: " + element.CoSkill );
			y := y + 20;
			CraftMenu_Text( gump, 16, y, 480, "Your " + co_skill_name + ": " + GetEffectiveSkill(character, CInt(element.CoSkillid)) );
			y := y + 20;
		endif

		CraftMenu_Text( gump, 16, y+10, 200, Upper("Materials") );
		y := y + 30;

		var slot1Name := CraftMenu_DescribeSlot( character, element.MaterialType, element.Material1Objtype, slot1Request );
		GFAddButton( gump, 16, y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_PICK_MATERIAL );
		CraftMenu_Text( gump, 50, y, 460, element.MaterialType + " (" + element.Material + ") -- " + slot1Name );
		y := y + 22;

		if( element.Material2Type )
			var slot2Name := CraftMenu_DescribeSlot( character, element.Material2Type, element.Material2Objtype, slot2Request );
			GFAddButton( gump, 16, y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_PICK_MATERIAL2 );
			CraftMenu_Text( gump, 50, y, 460, element.Material2Type + " (" + element.Material2 + ") -- " + slot2Name );
			y := y + 22;
		endif

		if( element.Material3Type )
			var slot3Name := CraftMenu_DescribeSlot( character, element.Material3Type, element.Material3Objtype, slot3Request );
			GFAddButton( gump, 16, y, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_PICK_MATERIAL3 );
			CraftMenu_Text( gump, 50, y, 460, element.Material3Type + " (" + element.Material3 + ") -- " + slot3Name );
			y := y + 22;
		endif

		if( len(extra_notes) > 0 )
			CraftMenu_Text( gump, 16, y+12, 200, Upper("Notes") );
			var note_y := y + 32;
			foreach note in extra_notes
				CraftMenu_Text( gump, 16, note_y, 480, note );
				note_y := note_y + 20;
			endforeach
		endif

		GFAddButton( gump, 16, 340, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_BACK );
		CraftMenu_Text( gump, 50, 341, 100, "Back" );
		GFAddButton( gump, 170, 340, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_MAKE_MAX );
		CraftMenu_Text( gump, 204, 341, 140, "Make Max" );
		GFAddButton( gump, 330, 340, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_MAKE_NUMBER );
		CraftMenu_Text( gump, 364, 341, 150, "Make Number" );
		GFAddButton( gump, 16, 365, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_MAKE_NOW );
		CraftMenu_Text( gump, 50, 366, 100, "Make Now" );

		if( element.Bulk )
			GFAddButton( gump, 170, 365, 4005, 4007, GF_CLOSE_BTN, CRAFTMENU_BTN_MAKE_BULK );
			CraftMenu_Text( gump, 204, 366, 140, "Make Bulk" );
		endif

		var result := GFSendGump( character, gump );
		if( !result )
			return 0;
		endif

		var btn := CInt(result[0]);

		if( btn == CRAFTMENU_BTN_BACK )
			return 0;

		elseif( btn == CRAFTMENU_BTN_PICK_MATERIAL )
			if( CraftMenu_IsPickableFamily(element.MaterialType) )
				var picked := CraftMenu_PickFamilyMaterial( character, element.MaterialType );
				if( picked )
					slot1Request := picked;
				endif
			else
				SendSysMessage( character, "There's only one kind of that -- nothing to choose." );
			endif

		elseif( btn == CRAFTMENU_BTN_PICK_MATERIAL2 )
			if( CraftMenu_IsPickableFamily(element.Material2Type) )
				var picked2 := CraftMenu_PickFamilyMaterial( character, element.Material2Type );
				if( picked2 )
					slot2Request := picked2;
				endif
			else
				SendSysMessage( character, "There's only one kind of that -- nothing to choose." );
			endif

		elseif( btn == CRAFTMENU_BTN_PICK_MATERIAL3 )
			if( CraftMenu_IsPickableFamily(element.Material3Type) )
				var picked3 := CraftMenu_PickFamilyMaterial( character, element.Material3Type );
				if( picked3 )
					slot3Request := picked3;
				endif
			else
				SendSysMessage( character, "There's only one kind of that -- nothing to choose." );
			endif

		elseif( btn == CRAFTMENU_BTN_MAKE_MAX or btn == CRAFTMENU_BTN_MAKE_NUMBER or btn == CRAFTMENU_BTN_MAKE_NOW or btn == CRAFTMENU_BTN_MAKE_BULK )

			if( !slot1Request )
				slot1Request := CraftMenu_ResolveDefaultSlot( character, element.MaterialType, element.Material1Objtype );
			endif
			if( element.Material2Type and !slot2Request )
				slot2Request := CraftMenu_ResolveDefaultSlot( character, element.Material2Type, element.Material2Objtype );
			endif
			if( element.Material3Type and !slot3Request )
				slot3Request := CraftMenu_ResolveDefaultSlot( character, element.Material3Type, element.Material3Objtype );
			endif

			if( !slot1Request or (element.Material2Type and !slot2Request) or (element.Material3Type and !slot3Request) )
				SendSysMessage( character, "You don't have the materials to make that." );
				return 0;
			endif

			var maxCraftable := GetAvailableResource(character, slot1Request).total / element.Material;
			if( element.Material2Type )
				maxCraftable := Min( maxCraftable, GetAvailableResource(character, slot2Request).total / element.Material2 );
			endif
			if( element.Material3Type )
				maxCraftable := Min( maxCraftable, GetAvailableResource(character, slot3Request).total / element.Material3 );
			endif
			maxCraftable := CInt(maxCraftable);

			if( maxCraftable < 1 )
				SendSysMessage( character, "You don't have enough materials to make even one." );
				return 0;
			endif

			if( btn == CRAFTMENU_BTN_MAKE_BULK )
				var qty := CInt( SendTextEntryGump( character, "Make how many in one go? [0-"+maxCraftable+"]", TE_CANCEL_ENABLE, TE_STYLE_NORMAL, 5 ) );
				if( qty < 1 )
					SendSysMessage( character, "Nevermind." );
					return 0;
				elseif( qty > maxCraftable )
					qty := maxCraftable;
				endif

				return struct{
					mode         := "bulk",
					qty          := qty,
					slot1        := slot1Request,
					slot2        := slot2Request,
					slot3        := slot3Request,
					maxCraftable := maxCraftable
				};
			endif

			var mode := "now";
			if( btn == CRAFTMENU_BTN_MAKE_MAX )
				AutoLoop_initFixed( character, maxCraftable );
				mode := "max";
			elseif( btn == CRAFTMENU_BTN_MAKE_NUMBER )
				AutoLoop_init( character, maxCraftable );
				mode := "number";
			else
				AutoLoop_initFixed( character, 1 );
			endif

			return struct{
				mode         := mode,
				qty          := 0,
				slot1        := slot1Request,
				slot2        := slot2Request,
				slot3        := slot3Request,
				maxCraftable := maxCraftable
			};

		endif

	endwhile

endfunction
