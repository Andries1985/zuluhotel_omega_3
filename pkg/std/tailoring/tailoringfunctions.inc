/////////////////
// tailoringfunctions.inc
//
// RunTailoringGump + everything it calls -- split out of make_cloth_items.src
// (2026-09-20) so a double-click entry point OTHER than the Sewing Kit (the
// Autoloom's facing/animation wrapper in autoloom_use.src, and any other
// Tailoring station) can open the exact same gump. Include files can't
// contain a program block, which make_cloth_items.src has (use_sewing_kit),
// so this had to move into its own .inc instead of being included directly.
//
//Edited August 30, 2026 -- OSI-style crafting gump, ported onto the shared
//pkg/systems/crafting/include/craftmenu framework (see bowcraft.src for the
//reference pattern). Replaces the old upfront-target-a-material-then-
//CreateMenu flow: the Sewing Kit now opens the gump directly (auto-detecting
//Hide/Cloth from backpack + Omega Cache, with a picker for which hide to use,
//exactly like bowcraft's Log picker), matching how Bowcraft already works.
//Repair Item and Fortify a Hat are still available as gump Options, each
//doing its own follow-up targeting step just like bowcraft's Repair Item.
/////////////////////////////////////////////////////////////////////////////
//
//	Tailoring: Original Code by Racalac
//
/////////////////////////////////////////////////////////////////////////////

use uo;
use cfgfile;
use os;
use math;
use basicio;
use polsys;

include ":mdgumps:yesNo";
include "include/objtype";
include "include/client";
include "include/string";
include "include/classes";
include "include/itemutil";
include "include/random";
include "util/repair";
include "include/autoloop";
include "include/attributes";
include "include/difficulty";
include ":karmafame:karmafame";
include ":crafting:craftingfunctions";
include "include/resourcemanager";
include ":crafting:craftmenu";

const SCISSORS_SOUND    := 0x0249;
const DIVIDER           := 3;
const APPEAR_AT         := -10;
const POINTS_MULTIPLIER := 8;
const FAME_BONUS        := 10;

// Non-objtype button-id sentinels for the category gump's Options. All real
// recipe/material objtypes in tailoring.cfg are well under this.
const TL_BTN_REPAIR   := 900021;
const TL_BTN_FORTIFY  := 900022;

// 2026-09-20: package-qualified, NOT a bare name -- see blacksmithgump.inc's
// own note on this same gotcha. ReadConfigFile("tailoring") resolves relative
// to exec.prog()->pkg (the ACTUALLY EXECUTING program's package), not this
// .inc's home package, so a bare name would silently break for any caller
// living outside pkg/std/tailoring (e.g. a future cross-package station).
var tailor_cfg          := ReadConfigFile( ":tailoring:tailoring" );
var tailor_items        := ReadConfigFile( ":tailoring:itemdesc" );

// Autoloom graphics (2026-09-21) -- see autoloom_use.src for the full stored/
// open/weaving breakdown. Keyed on the station's OBJTYPE, which never changes,
// not its .graphic: the graphic is what these states rewrite, so sniffing it
// misread an East loom caught on any non-stored frame as South and then
// rewrote it to South art for good. Returns 0 for every other station.
function GetAutoloomGraphics( station )

	if( !station )
		return 0;
	endif

	case( CInt( station.objtype ) )
		0xA82D:	return struct{ anim_start := 0xA81D, open_graphic := 0xA82E, stored_graphic := 0xA82D };	// South
		0xA82F:	return struct{ anim_start := 0xA825, open_graphic := 0xA830, stored_graphic := 0xA82F };	// East
	endcase

	return 0;

endfunction

// No-op for every station except the Autoloom (safe to call unconditionally
// from the shared gump loop). Called inline by the use script's own thread,
// so only one script ever writes the graphic during a session -- the old
// fire-and-forget animate script raced the use script's revert and itself.
// 0xA81D/0xA825 are client-animated tiles (animdata), so ONE write starts the
// 8-frame weave loop; stepping frames server-side landed on 0xA81F/0xA829,
// which the client animates too, and flashed wrong-facing art.
function AutoloomSetWeaving( station, weaving )

	var loom := GetAutoloomGraphics( station );
	if( !loom )
		return;
	endif

	if( weaving )
		station.graphic := loom.anim_start;
	else
		station.graphic := loom.open_graphic;
	endif

endfunction

// Top-level loop: category/selection gump -> detail gump -> back to top, until
// the player crafts something, picks Repair/Fortify, or closes out entirely.
// The gump flow itself lives in the shared craftmenu.inc -- this is just the
// tailoring-specific wiring: Repair Item / Fortify a Hat handling, and
// dispatching a completed pick into TryToMakeItem below.
//
// station (2026-09-20): optional, passed only by autoloom_use.src, so the
// loom weaves during every real Make action (AutoloomSetWeaving) instead of
// once at the very start of the whole gump session.
exported function RunTailoringGump( character, station := 0 )

	while( 1 )
		var extra_options := array{
			struct{ label := "Repair Item", value := TL_BTN_REPAIR },
			struct{ label := "Fortify a Hat", value := TL_BTN_FORTIFY }
		};
		var choice := CraftMenu_ShowCategoryGump( character, tailor_cfg, "Tailoring", SKILLID_TAILORING, APPEAR_AT, extra_options );
		if( !choice )
			return;
		endif

		if( choice == TL_BTN_REPAIR )
			SendSysmessage( character, "What would you like to repair?" );
			var use_on := Target( character, TGTOPT_CHECK_LOS );
			if( use_on and Accessible(character, use_on) )
				if( use_on.hp and use_on.quality and tailor_cfg[use_on.graphic] )
					repair_item( character, use_on, SKILLID_TAILORING );
				else
					SendSysMessage( character, "You can't repair that with this." );
				endif
			endif
			return;
		endif

		if( choice == TL_BTN_FORTIFY )
			RunFortifyHat( character );
			return;
		endif

		var element := tailor_cfg[choice];
		if( !element )
			continue;
		endif

		var detail := CraftMenu_ShowDetailGump( character, tailor_cfg, choice, SKILLID_TAILORING, "Tailoring", array{} );
		if( !detail )
			continue;
		endif

		// Bulk is a single roll, so one weave here covers it; TryToMakeItem
		// weaves per AutoLoop pass itself (passing station through).
		if( detail.mode == "bulk" )
			AutoloomSetWeaving( station, 1 );
			CraftClothBulk( character, choice, element, detail.slot1, detail.qty );
			AutoloomSetWeaving( station, 0 );
		else
			TryToMakeItem( character, detail.slot1, choice, station );
		endif
		return;
	endwhile

endfunction


// Fortify a Hat gump option -- targets the hat (must be an undamaged hat in
// the backpack), then hands off to FortifyHat below, which targets the helm.
function RunFortifyHat( character )

	SendSysmessage( character, "What hat would you like to fortify?" );
	var hat := Target( character, TGTOPT_CHECK_LOS );
	if( !hat )
		return;
	endif

	if( !Accessible( character, hat ) )
		SendSysMessage( character, "You can't reach that." );
		return;
	endif

	if( !ReserveItem(hat) )
		return;
	endif

	if( !IsInContainer( character.backpack, hat ) )
		SendSysmessage( character, "That item has to be in your backpack." );
		return;
	endif

	if( !hat.hp and hat.quality and tailor_items[hat.objtype] )
		hat.hp := 1;
	endif

	if( !IsHat(hat) )
		SendSysMessage( character, "That's not a hat." );
		return;
	endif

	if( hat.hp != hat.maxhp )
		SendSysMessage( character, "That hat needs to be undamaged to fortify a helm with it." );
		return;
	endif

	FortifyHat( character, hat );

endfunction


// station (2026-09-20): optional, only the Autoloom passes it -- the loom
// weaves for the length of each AutoLoop pass's _Play_Sound delay.
function TryToMakeItem( character, resourceRequest, item, station := 0 )

	var resource_diff := tailor_cfg[resourceRequest.objtype].Difficulty;

	var material;
	var itemname;
	var give_deed;
	var deedname;

	var element := FindConfigElem( tailor_cfg, item );
	if( element )
		material := element.Material;
		deedname := element.Name;
	else
		return;
	endif

	var skill := element.Skill + CInt( resource_diff / DIVIDER );
	if( skill < 1 )
		skill := 1;
	elseif( skill > 150 )
		skill := 150;
	endif
	var points	:= CInt( (skill + element.Material) * POINTS_MULTIPLIER );

	//Require half resources during Crafting PH
	// 2026-09-21: /2.0, not /2 -- int/int is whole-number division in the engine,
	// so Ceil had nothing left to round: 1 became 0 (a free craft) and 25 became 12.
	if(GetGlobalProperty("PHC") || GetObjProperty(character, "#PPHC"))
		material := CInt(Ceil(material/2.0));
	endif

	var excep_ch := 10;
	var excep_diff	:= skill;

	if( GetObjProperty( character, CLASSEID_CRAFTER ) )
		excep_ch := Cint( excep_ch * ClasseBonus( character, CLASSEID_CRAFTER ) );
		excep_diff += 20;
	else
		excep_diff += 40;
	endif

	if( excep_diff < 90 )
		excep_diff := 90;
	endif

	LeaseResource( character, resourceRequest, material );

	// Loop count was already fixed by CraftMenu_ShowDetailGump (AutoLoop_initFixed
	// for Make Max/Now, or AutoLoop_init with a capped maxx for Make Number) --
	// do NOT call AutoLoop_init again here, it would overwrite that count.
	while( AutoLoop_more() && ( GetAvailableResource(character, resourceRequest).total >= material ) && NOT character.dead)

		sendDiff( character , skill );

		itemname := element.Name;
		deedname := element.Name;
		give_deed := element.MakeDeed;

		AutoloomSetWeaving( station, 1 );
		_Play_Sound( character );
		AutoloomSetWeaving( station, 0 );

		if( CheckSkill( character, SKILLID_TAILORING, skill, points ) )

			var theitem;
			var consume_amount := material;

			// 2026-09-21: consume BEFORE creating, and honour the result. The
			// item used to be created first with ConsumeResource's return value
			// ignored, so materials lifted out of the pack during _Play_Sound
			// above gave a free item.
			if( !ConsumeResource( character, resourceRequest, consume_amount ) )
				break;
			endif

			if( item == UOBJ_BANDAGE )
				// 2026-09-21: one bandage per pass at the cfg rate, honouring the
				// Make Now/Number/Max count. Used to convert ALL available cloth
				// (backpack + Omega Cache) at 4:1 on the first success -- Make
				// Bulk and scissors remain the fast whole-batch paths. Colour
				// goes on the descriptor, not after creation, so dyed-cloth
				// bandages still merge into one stack instead of piling up
				// as singles.
				var bandage_desc := GetItemDescriptor( CInt(item) );
				bandage_desc.Color := resourceRequest.color;
				theitem := CreateItemInBackpack( character, bandage_desc, 1 );
				if( theitem )
					SendSysmessage( character, "You create the item and place it in your pack." );
				endif
			else

				var cprops := GetConfigStringArray( tailor_cfg[resourceRequest.objtype], "CProp" );

				var randomchance := Random(150);

				if( GetObjProperty(character, "ForceExceptional") or (( randomchance < excep_ch ) && CheckSkill( character, SKILLID_TAILORING, excep_diff, 0 )) )

					//Made something Exceptional
					if( give_deed )
						var product_desc := GetItemDescriptor(CInt(give_deed));
						ApplyMaterialPropertiesDeed(product_desc, resourceRequest, 1, character, SKILLID_TAILORING, deedname);
						theitem := CreateItemInBackpack( character , product_desc );
						itemname := GetObjProperty( theitem , "DeedName" );
						SetName( theitem, itemname );
						IncRevision(theitem);
					else
						var product_desc := GetItemDescriptor(CInt(item));
						ApplyMaterialProperties(product_desc, resourceRequest, 1, character, SKILLID_TAILORING);
						theitem := CreateItemInBackpack( character , product_desc );
					endif

					// 2026-09-21: only on a real item -- a full backpack used to
					// award the fame and announce success for an item never made.
					if( theitem )
						if( excep_diff >= 130 )
							SendSysmessage( character, "This masterpiece will make you famous around the world." );
							AwardFame( character, FAME_BONUS );
						elseif( GetObjProperty( character, CLASSEID_CRAFTER ) and excep_diff >= 110 )
							SendSysmessage( character, "This masterpiece will make you famous around the world." );
							AwardFame( character, FAME_BONUS );
						endif
						SendSysmessage( character, "You make an exceptional item and put it in your pack" );
					endif
				else
					//Failed to make Exceptional, make normal item
					if( give_deed )
						var product_desc := GetItemDescriptor(CInt(give_deed));
						ApplyMaterialPropertiesDeed(product_desc, resourceRequest, 0, character, SKILLID_TAILORING, deedname);
						theitem := CreateItemInBackpack( character , product_desc );
						itemname := GetObjProperty( theitem , "DeedName" );
						SetName( theitem, itemname );
						IncRevision(theitem);
					else
						var product_desc := GetItemDescriptor(CInt(item));
						ApplyMaterialProperties(product_desc, resourceRequest, 0, character, SKILLID_TAILORING);
						theitem := CreateItemInBackpack( character , product_desc );
					endif
					if( theitem )
						SendSysmessage( character, "You create the item and place it in your pack." );
					endif
				endif

				if( Len( cprops ) && ( theitem.equipscript == "::skilladvancerequip" ) )
					foreach cprop in cprops
						var splited := SplitWords( cprop );
						var prop_name := splited[1];
						var prop_mod  := Cint(splited[2]);
						if( prop_name and prop_mod )
							var presently := GetObjProperty( theitem, prop_name );
							if( !presently )
								presently := 0;
							endif

							if( presently and abs(prop_mod) > abs(presently) )
							// magic penalty works normally
								if (prop_name != "MagicPenalty")
									prop_mod := 0 - presently;
								endif
							endif

							var resulting := presently + prop_mod;
							if( resulting > 0 and prop_name == "DefaultDex" )
								resulting := 0;
							endif
							if( resulting || resulting < 0  )
								SetObjProperty( theitem, prop_name, resulting );
							elseif( presently )

							// Toadstool - 21/04/2012 - Due to a core bug with items (just armour that i know of)
							// getting back their basic props on server restart. Set deleted ones to 0
								SetObjProperty( theitem, prop_name, 0);
							endif
						endif
					endforeach
				endif
			endif

			// 2026-09-21: was consume + consume-again + return -- a double charge
			// for zero items, and the return skipped ReleaseResourceLease below
			// (Omega Cache lease left dangling for its TTL). Materials were
			// already charged once above, same as carpentry.src on a full pack;
			// break so the lease release + AutoLoop_finish after the loop run.
			if( !theitem )
				SendSysmessage( character, "Your backpack is full." );
				break;
			endif

			theitem.hp := theitem.maxhp;
			theitem.color := resourceRequest.color;
			IncRevision(theitem);
		else
			var tail := GetEffectiveSkill( character, SKILLID_TAILORING );
			var destroy := 75 - CInt( tail / 2 );
			if( GetObjProperty( character, CLASSEID_CRAFTER ) )
				destroy := Cint( destroy / ClasseBonus( character, CLASSEID_CRAFTER ) );
			endif

			if( destroy > 50 )
				destroy	:= 50;
			endif

			destroy	:= Cint( material * destroy / 100 );
			if( destroy < 1 )
				destroy	:= 1;
			endif

			ConsumeResource( character, resourceRequest, destroy );
			SendSysmessage( character, "You destroyed some resources." );

		endif

		if( !ExtendResourceLease(resourceRequest, character, material) )
			break;
		endif

	endwhile

	ReleaseResourceLease(resourceRequest);
	AutoLoop_finish();

endfunction


// Bulk version of TryToMakeItem, mirroring Bowcraft's CraftLogBulk (shafts/
// arrows): one CheckSkill roll for the whole player-chosen quantity instead
// of one roll per unit, then create/consume the entire batch in one shot.
// Crafting-station rollout, 2026-09-12 -- added for Bandages specifically
// (the only Tailoring recipe with Bulk 1 so far), but works for any single-
// material Tailoring recipe.
function CraftClothBulk( character, item, element, resourceRequest, qty )

	// 2026-09-21: the engine refuses to create more than 60000 of anything in
	// one call -- a bigger batch used to be charged in full and make nothing.
	if( qty > 60000 )
		qty := 60000;
	endif

	var resource_diff := tailor_cfg[resourceRequest.objtype].Difficulty;
	var material := CInt(element.Material) * qty;

	if(GetGlobalProperty("PHC") || GetObjProperty(character, "#PPHC"))
		material := CInt(Ceil(material/2.0)); // 2026-09-21: /2.0, see TryToMakeItem
	endif

	var skill := element.Skill + CInt( resource_diff / DIVIDER );
	if( skill < 1 )
		skill := 1;
	elseif( skill > 150 )
		skill := 150;
	endif

	// Points computed off one unit's worth, not the scaled batch total -- a
	// bulk click is only worth the same skill gain as crafting a single item.
	var points := CInt( (skill + element.Material) * POINTS_MULTIPLIER );

	_Play_Sound( character );

	if( CheckSkill( character, SKILLID_TAILORING, skill, points ) )
		// 2026-09-21: consume first and honour the result, same reasoning as
		// TryToMakeItem -- this used to create the batch, then consume with the
		// return value ignored.
		if( !ConsumeResource( character, resourceRequest, material ) )
			return;
		endif

		// Colour goes on the descriptor so dyed-cloth output only merges into a
		// same-coloured stack. Creating plain and recolouring afterwards used to
		// repaint an existing plain stack the new batch had merged into.
		var bulk_desc := GetItemDescriptor( CInt(item) );
		bulk_desc.Color := resourceRequest.color;
		var theitem := CreateItemInBackpack( character, bulk_desc, qty );
		if( theitem )
			theitem.hp := theitem.maxhp;
			IncRevision(theitem);
			SendSysmessage( character, "You place " + qty + " " + element.Name + " in your pack." );
		else
			// Used to charge the whole batch here and say nothing.
			SendSysmessage( character, "Your backpack is full." );
		endif
	else
		var tail := GetEffectiveSkill( character, SKILLID_TAILORING );
		var destroy := 75 - CInt( tail / 2 );
		if( GetObjProperty( character, CLASSEID_CRAFTER ) )
			destroy := Cint( destroy / ClasseBonus( character, CLASSEID_CRAFTER ) );
		endif
		if( destroy > 50 )
			destroy := 50;
		endif
		destroy := Cint( material * destroy / 100 );
		if( destroy < 1 )
			destroy := 1;
		endif
		ConsumeResource( character, resourceRequest, destroy );
		SendSysmessage( character, "You destroyed some resources." );
	endif

endfunction


function _Play_Sound( character )

	PlaySoundEffect( character, SCISSORS_SOUND );
	Sleep(1);
	PlaySoundEffect( character, SCISSORS_SOUND );
	Sleep(2);
	if( !GetObjProperty( character, CLASSEID_CRAFTER ))
		PlaySoundEffect( character , SCISSORS_SOUND );
		Sleep (2);
	endif

endfunction

function FortifyHat( who, hat )

	if (GetObjProperty(hat, "FortifiedWith"))
		SendSysMessage(who, "This hat is already fortified.");
		return 0;
	endif

	if (GetObjProperty(hat, "Entwined") || GetObjProperty(hat,"GuildUniform") )
		SendSysMessage(who, "You must unentwine the hat to do that.");
		return 0;
	endif

	SendSysMessage( who, "Fortify this hat with what?" );
	var helm := Target( who, TGTOPT_CHECK_LOS );
	if( !helm )
		return;
	endif

	if( !IsHelm(helm) )
		SendSysMessage( who, "That's not a helmet." );
		return;
	endif

	if( GetObjProperty( helm , "Cursed" ) )
		SendSysMessage( who, "That helmet seems to be cursed. Purify it first." );
		return;
	endif

	if( !ReserveItem(helm) )
		return 0;
	endif

	if( !IsInContainer( who.backpack, helm ) )
		SendSysmessage( who, "The helm has to be in your backpack." );
		return;
	endif

	// this part most probably isn't needed, but havnt tested the above. just incase :P
	if (GetObjProperty(helm, "Entwined") || GetObjProperty(helm,"GuildUniform") )
		SendSysMessage(who, "You must unentwine the helm to do that.");
		return 0;
	endif

	_Play_Sound( who );

	if( !CheckSkill( who, SKILLID_TAILORING, -1, 250 ) )
		SendSysMessage( who, "You fail and ruin both, the hat and the helm." );
		DestroyItem( hat );
		DestroyItem( helm );
		return;
	endif

	SetObjProperty(helm, "FortifiedWith", hat.desc);
	hat.quality := 2.0;
	DupeItem( hat, helm );
	SetName( helm, "a fortified " + TruncateArticle(hat.desc) );
	helm.hp := helm.maxhp;
	DestroyItem( hat );
	IncRevision(helm);

endfunction


function IsHat( what )

	case( what.graphic )
		0x153F:
		0x1545:
		0x1546:
		0x1547:
		0x1548:
		0x1549:
		0x154a:
		0x154b:
		0x154c:
		0x141b:
		0x141c:
		0x1540:
		0x1543:
		0x1544:
		0x1713:
		0x1714:
		0x1715:
		0x1716:
		0x1717:
		0x1718:
		0x1719:
		0x171A:
		0x171B:
		0x171C:		return 1;

		default:	return 0;
	endcase

endfunction


function IsHelm( what )

	case (what.graphic)
		0x13BB:
		0x13C0:
		0x1408:
		0x1409:
		0x140A:
		0x140B:
		0x140C:
		0x140D:
		0x140E:
		0x140F:
		0x1412:
		0x1419:
		0x1451:
		0x1456:
		0x1DB9:
		0x1DBA:		return 1;
				break;

		default:	return 0;
	endcase

endfunction
