/////////////////
// blacksmithgump.inc
//
// RunBlacksmithGump -- the Blacksmithy category/detail gump loop -- split out
// of make_blacksmith_items.src (crafting-station rollout, 2026-09-12) so any
// double-click entry point OTHER than the hand hammer (Blacksmithy stations,
// and the Large Forge's Bellows animate script in pkg/items/forge/use.src)
// can open the exact same gump. Include files can't contain a program block,
// which make_blacksmith_items.src has (use_hammer), so this had to move into
// its own .inc instead of being included directly.
////////////////

use uo;
use cfgfile;

include "util/repair";
include "include/objtype";
include "include/difficulty";
include "include/resourcemanager";
include "include/classes";
include "include/itemutil";
include "include/autoloop";
include ":karmafame:karmafame";
include ":crafting:craftingfunctions";
include ":crafting:craftmenu";

const DIVIDER		:= 3;
const APPEAR_AT		:= -10;

// Non-objtype button-id sentinel for the category gump's Options. All real
// recipe/ore objtypes in blacksmithy.cfg are well under this.
const BS_BTN_REPAIR	:= 900021;

const POINTS_MULTIPLIER	:= 8;
const FAME_BONUS	:= 10;

var smith_cfg := ReadConfigFile( "blacksmithy" );

// Top-level loop: category/selection gump -> detail gump -> back to top,
// until the player crafts something, picks Repair, or closes out entirely.
// The gump flow itself lives in the shared craftmenu.inc -- this is just the
// blacksmithy-specific wiring: Repair Item handling, and dispatching a
// completed pick into MakeBlacksmithItems (make_blacksmith_items.src).
exported function RunBlacksmithGump( character )

	while( 1 )
		var extra_options := array{
			struct{ label := "Repair Item", value := BS_BTN_REPAIR }
		};
		var choice := CraftMenu_ShowCategoryGump( character, smith_cfg, "Blacksmithy", SKILLID_BLACKSMITHY, APPEAR_AT, extra_options, DIVIDER );
		if( !choice )
			return;
		endif

		if( choice == BS_BTN_REPAIR )
			SendSysmessage( character, "What would you like to repair?" );
			var use_on := Target( character, TGTOPT_CHECK_LOS );
			if( use_on and Accessible(character, use_on) )
				if( use_on.hp and use_on.quality and smith_cfg[use_on.graphic] )
					repair_item( character, use_on, SKILLID_BLACKSMITHY );
				else
					SendSysMessage( character, "You can't repair that with this." );
				endif
			endif
			return;
		endif

		var element := smith_cfg[choice];
		if( !element )
			continue;
		endif

		var detail := CraftMenu_ShowDetailGump( character, smith_cfg, choice, SKILLID_BLACKSMITHY, "Blacksmithy", array{}, DIVIDER );
		if( !detail )
			continue;
		endif

		MakeBlacksmithItems( character, detail.slot1, detail.slot2, choice );
		return;
	endwhile

endfunction


// Handles both plain items (boneRequest = 0) and bone armor (boneRequest set
// from the recipe's Material2 "Bone" slot) -- the two used to be separate
// functions (MakeBlacksmithItems/MakeBoneItems) differing only by whether a
// second resource gets leased/consumed alongside the ingots.
function MakeBlacksmithItems( character, ingotRequest, boneRequest, product_type )

	var element := FindConfigElem( smith_cfg, product_type );
	if( !element )
		return 0;
	endif

	var orediff := CraftMenu_GetFamilyQualityDifficulty( "Ingot", CInt(ingotRequest.objtype) );
	var orename := smith_cfg[ingotRequest.objtype].Name;
	if( !orename or orename == "" )
		orename := GetItemDisplayName(CInt(ingotRequest.objtype));
	endif
	if( GetEffectiveSkill( character , SKILLID_BLACKSMITHY ) < orediff )
		sendDiff( character , orediff );
		SendSysmessage( character, "You aren't skilled enough to craft anything from " + orename + " ingots yet." );
		return;
	endif

	var material := element.Material;
	var boneMaterial := element.Material2;

	//Require half resources during Crafting PH
	if(GetGlobalProperty("PHC") || GetObjProperty(character, "#PPHC"))
		material := Cint(Ceil(material/2));
		if( boneRequest )
			boneMaterial := Cint(Ceil(boneMaterial/2));
		endif
	endif

	var skill := element.Skill + Cint( orediff / DIVIDER );
	if( skill < 1 )
		skill := 1;
	elseif( skill > 150 )
		skill := 150;
	endif

	var points	:= Cint( (skill + element.Material) * POINTS_MULTIPLIER );
	// Calculate Exceptional dificulty
	var excep_ch	:= 10;
	var excep_diff	:= skill;
	if( GetObjProperty( character , CLASSEID_CRAFTER ) )
		excep_ch	:= Cint( excep_ch * ClasseBonus( character , CLASSEID_CRAFTER ) );
		excep_diff	+= 20;
	else
		excep_diff	+= 40;
	endif
	if( excep_diff < 90 )
		excep_diff := 90;
	endif

	LeaseResource( character , ingotRequest , material );
	if( boneRequest )
		LeaseResource( character , boneRequest , boneMaterial );
	endif

	// Loop count was already fixed by CraftMenu_ShowDetailGump
	// (AutoLoop_initFixed for Make Max/Now, or AutoLoop_init with a capped
	// maxx for Make Number) -- do NOT call AutoLoop_init again here, it
	// would overwrite that count (e.g. turning Make Now's fixed 1 back into
	// an "ask every time" prompt).
	while( AutoLoop_more() && (GetAvailableResource(character, ingotRequest).total >= material)
		and (!boneRequest or GetAvailableResource(character, boneRequest).total >= boneMaterial) and !character.dead)

		sendDiff( character , skill );

		PlaySoundEffect(character, SFX_ANVIL);
		Sleep (2);
		PlaySoundEffect(character, SFX_ANVIL);
		Sleep (2);
		if( !GetObjProperty( character, CLASSEID_CRAFTER ))
			PlaySoundEffect(character, SFX_ANVIL);
			Sleep (2);
		endif

		var product_desc := GetItemDescriptor(CInt(product_type));
		if( CheckSkill( character , SKILLID_BLACKSMITHY , skill , points ) )

			if( boneRequest )
				product_desc.CProps.insert("OnHitScript","Undead");
				product_desc.CProps.insert("ProtectedType",":combat:raceresistonhit");
				product_desc.CProps.insert("OnHitscriptNumber",32);
			endif

			if ( GetObjProperty(character, "ForceExceptional") or ((Random(100) < excep_ch) && CheckSkill( character, SKILLID_BLACKSMITHY, excep_diff, 0)) )

				ApplyMaterialProperties(product_desc, ingotRequest, 1, character, SKILLID_BLACKSMITHY);

				SendSysmessage( character , "You make an exceptional item and put it in your pack." );
				if( excep_diff >= 130 )
					SendSysmessage( character , "This masterpiece will make you famous around the world." );
					AwardFame( character , FAME_BONUS );
				elseif( GetObjProperty( character , CLASSEID_CRAFTER ) and excep_diff >= 110 )
					SendSysmessage( character , "This masterpiece will make you famous around the world." );
					AwardFame( character , FAME_BONUS );
				endif
			else
				//Made a normal item
				ApplyMaterialProperties(product_desc, ingotRequest, 0, character, SKILLID_BLACKSMITHY);
				SendSysmessage( character , "You create the item and place it in your pack." );
			endif

			ConsumeResource( character , ingotRequest , material );
			if( boneRequest )
				ConsumeResource( character , boneRequest , boneMaterial );
			endif
			var product := CreateItemInBackpack( character , product_desc );
			product.hp := product.maxhp;
			IncRevision( product );
			if(GetObjProperty(product, "IsExceptional"))
				if(product.hitscript)
					//was *12.. changed to *8
					product.speed_mod := Cint(ClasseBonus(character, CLASSEID_CRAFTER)*8); //18 at 6, 15 at 5 etc
				elseif(product.ar)
					// maybe something for armor too?
				endif
			endif
		else
			var bs 	:= GetEffectiveSkill( character , SKILLID_BLACKSMITHY );
			var destroy	:= 75 - Cint( bs / 2 );
			if( GetObjProperty( character , CLASSEID_CRAFTER ) )
				destroy := Cint( destroy / ClasseBonus( character , CLASSEID_CRAFTER ) );
			endif

			if( destroy > 50 )
				destroy	:= 50;
			endif

			destroy	:= Cint( material * destroy / 100 );
			if( destroy < 1 )
				destroy	:= 1;
			endif
			ConsumeResource( character , ingotRequest , destroy );
			if( boneRequest )
				ConsumeResource( character , boneRequest , destroy );
				SendSysmessage( character , "You destroyed " + destroy + " " + orename + " ingots and " + destroy + " bones." );
			else
				SendSysmessage( character , "You destroyed " + destroy + " " + orename + " ingots." );
			endif
		endif

		if( !ExtendResourceLease( ingotRequest , character , material ) )
			break;
		endif
		if( boneRequest and !ExtendResourceLease( boneRequest , character , boneMaterial ) )
			break;
		endif
	endwhile

	if( GetAvailableResource(character, ingotRequest).total < material )
		SendSysMessage(character, "You don't have enough " + orename + " ingots to continue.");
	endif
	ReleaseResourceLease( ingotRequest );
	if( boneRequest )
		ReleaseResourceLease( boneRequest );
	endif
	AutoLoop_finish();

endfunction
