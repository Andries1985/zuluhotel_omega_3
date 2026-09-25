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

// 2026-09-15: package-qualified, NOT a bare name. ReadConfigFile("blacksmithy")
// resolves relative to exec.prog()->pkg -- the ACTUALLY EXECUTING program's own
// package, not this .inc file's home package. That's fine for callers living in
// pkg/std/blacksmithy itself (make_blacksmith_items.src), but this .inc is also
// included cross-package (e.g. pkg/items/forge/use.src's Bellows animate
// script, and every Blacksmithy station's Script field) -- for those, a bare
// "blacksmithy" was resolving against THEIR package (forge, etc.), which has
// no blacksmithy.cfg, silently returning an error/empty cfg object. That's why
// the Blacksmithy gump opened fine (NearForgeAndAnvil doesn't touch this var)
// but showed zero categories/recipes for every station except ones that
// happen to live in pkg/std/blacksmithy itself.
var smith_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );

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

		// Smelting recipes are the only ones with a ResultObjtype field (their
		// recipe key is the ORE's objtype, not the product -- see the SMELTING
		// section header in blacksmithy.cfg) -- route those into the Mining-
		// gated MakeSmeltedIngots instead of the normal Blacksmithy-gated path.
		if( element.ResultObjtype )
			// detail.qty is only set by "Make Bulk" (which smelting recipes
			// don't offer -- no Bulk flag); Max/Now/Number all go through
			// AutoLoop_initFixed/AutoLoop_init instead, which stash the real
			// requested count in AL_number (include/autoloop.inc).
			var smelt_qty := detail.qty;
			if( !smelt_qty )
				smelt_qty := AL_number;
			endif
			MakeSmeltedIngots( character, detail.slot1, choice, smelt_qty );
		else
			MakeBlacksmithItems( character, detail.slot1, detail.slot2, choice );
		endif
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
	// 2026-09-21: /2.0, not /2 -- int/int is whole-number division in the engine,
	// so Ceil had nothing left to round: 1 became 0 (a free craft) and 25 became 12.
	if(GetGlobalProperty("PHC") || GetObjProperty(character, "#PPHC"))
		material := Cint(Ceil(material/2.0));
		if( boneRequest )
			boneMaterial := Cint(Ceil(boneMaterial/2.0));
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

			// 2026-09-21: consume first and honour the result (see
			// HasCraftingResources in resourcemanager.inc). ConsumeResource's
			// result used to be ignored, so ingots lifted out of the pack during
			// the anvil delay above still produced the item.
			if( !HasCraftingResources( character, ingotRequest, material, boneRequest, boneMaterial ) )
				SendSysMessage( character, "You no longer have the materials for that." );
				break;
			endif
			if( !ConsumeResource( character , ingotRequest , material ) )
				break;
			endif
			if( boneRequest )
				if( !ConsumeResource( character , boneRequest , boneMaterial ) )
					break;
				endif
			endif

			var is_excep := 0;
			if ( GetObjProperty(character, "ForceExceptional") or ((Random(100) < excep_ch) && CheckSkill( character, SKILLID_BLACKSMITHY, excep_diff, 0)) )
				is_excep := 1;
			endif
			ApplyMaterialProperties(product_desc, ingotRequest, is_excep, character, SKILLID_BLACKSMITHY);

			// 2026-09-21: the create was never checked -- on a full backpack a Make
			// Max run kept charging ingots every pass while announcing success.
			// Charged once (CheckSkill already paid the gain), then stop.
			var product := CreateItemInBackpack( character , product_desc );
			if( !product )
				SendSysmessage( character , "Your backpack is full." );
				break;
			endif

			if( is_excep )
				SendSysmessage( character , "You make an exceptional item and put it in your pack." );
				if( excep_diff >= 130 )
					SendSysmessage( character , "This masterpiece will make you famous around the world." );
					AwardFame( character , FAME_BONUS );
				elseif( GetObjProperty( character , CLASSEID_CRAFTER ) and excep_diff >= 110 )
					SendSysmessage( character , "This masterpiece will make you famous around the world." );
					AwardFame( character , FAME_BONUS );
				endif
			else
				SendSysmessage( character , "You create the item and place it in your pack." );
			endif

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


// Smelts ore into its matching ingot as ONE batch, not MakeBlacksmithItems's
// per-unit AutoLoop. Smelting never grants Mining skill on this shard --
// matches smelting.src's original CheckSkill(...,0) call (0 points = no
// gain) -- so there's no mechanical reason to pace it out one unit at a
// time with a sound/sleep between each. This instead mirrors how the
// ORIGINAL hand-smelt flow already worked: SmeltOre rolls ONCE against the
// whole ore stack (GetAmount(ore)), not once per unit -- qty here is just
// however many the player chose via Make Max/Number/Now instead of always
// being "the whole stack". On success the full qty converts; on failure,
// only 1 ore is lost (same as the original), not the whole batch, so a bad
// roll doesn't wipe out everything in one go.
//
// The success roll stays on Mining -- SKILLID_MINING against the ore's own
// SmeltDiff (stored in the recipe's CoSkill field) -- instead of
// Blacksmithy. CoSkillid/CoSkill on an ordinary recipe only gates
// visibility, it doesn't change which skill the actual craft roll uses, so
// this needed its own completion function rather than reusing
// MakeBlacksmithItems as-is.
function MakeSmeltedIngots( character, oreRequest, choice, qty )

	var element := smith_cfg[choice];
	if( !element )
		return 0;
	endif

	if( qty < 1 )
		SendSysMessage( character, "Nevermind." );
		return 0;
	endif

	var resultObjtype := CInt(element.ResultObjtype);
	var difficulty := CInt(element.CoSkill);
	var orename := element.MaterialType;

	var available := GetAvailableResource(character, oreRequest).total;
	if( qty > available )
		qty := available;
	endif
	// 2026-09-21: the engine refuses to create more than 60000 of anything in
	// one call -- a bigger batch used to be charged in full and make nothing.
	if( qty > 60000 )
		qty := 60000;
	endif
	if( qty < 1 )
		SendSysMessage( character, "You don't have enough " + orename + "." );
		return 0;
	endif

	LeaseResource( character, oreRequest, qty );

	sendDiff( character, difficulty );
	PlaySoundEffect(character, SFX_ANVIL);
	Sleep(2);

	if( CheckSkill( character, SKILLID_MINING, difficulty, 0 ) )
		// 2026-09-21: only create if the ore was really charged -- the result
		// used to be ignored, so ore lifted out of the pack during the Sleep(2)
		// above still produced the whole ingot batch. ConsumeResource tells the
		// player itself when it comes up short.
		if( ConsumeResource( character, oreRequest, qty ) )
			var ingots := CreateItemInBackpack( character, resultObjtype, qty );
			if( !ingots )
				ingots := CreateItemAtLocation( character.x, character.y, character.z, resultObjtype, qty, character.realm );
				if( ingots )
					SendSysmessage( character, "You create " + qty + " ingots, but as your backpack is full, you place them by your feet." );
				endif
			else
				SendSysmessage( character, "You create " + qty + " ingots and place them in your pack." );
			endif
		endif
	else
		ConsumeResource( character, oreRequest, 1 );
		SendSysmessage( character, "You destroy some " + orename + "." );
	endif

	ReleaseResourceLease( oreRequest );

endfunction
