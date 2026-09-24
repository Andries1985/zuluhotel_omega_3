////////////////////////////////////////////////////////////////////////////
// alchemyfunctions.inc
//
// DoAlchemy and everything else alchemy.src used to contain except
// use_mortar itself -- split out (crafting-station rollout, 2026-09-12) so a
// double-click entry point OTHER than a held Mortar and Pestle (an Alchemy
// station) can reach DoAlchemy. Include files can't contain a program block,
// which alchemy.src has (use_mortar), so this had to move into its own .inc
// instead of being included directly.
////////////////////////////////////////////////////////////////////////////
use uo;
use cfgfile;
use basic;
use os;

include "include/attributes";
include "include/client";
include "include/objtype";
include "include/classes";
include "include/itemutil";
include "include/autoloop";
include "include/resourcemanager";


const ALCH_SFX_GRIND        := 0x0243;
const ALCH_SFX_POUR         := 0x0241;
const SFX_BUBBLE       := 0x004f;

const APPEAR_AT        := -10;
const ALCH_POINT_MULTIPLIER := 15;
const NUM_PERFORM      := 4;
const ANIM_DELAY_MS    := 1500;

// 2026-09-15: package-qualified, NOT a bare name -- see the matching note in
// pkg/std/blacksmithy/blacksmithgump.inc's smith_cfg. A bare "alchemy"
// resolves against exec.prog()->pkg (the ACTUALLY EXECUTING program's
// package), which is wrong for any cross-package station caller of this
// shared .inc (e.g. alchemy_station_use.src's Thief-class dispatch) -- it
// silently returned an error/empty cfg for those, showing zero categories/
// recipes in the gump.
var conf := ReadConfigFile( ":alchemy:alchemy" );


// via_station (crafting-station rollout, 2026-09-12): a placed Alchemy
// station isn't "on the player" and has no meaningful backpack container to
// reserve, so both of those carried-tool-only checks are skipped when
// invoked from a station instead of a held Mortar and Pestle. Everything
// else (targeting a reagent, picking a potion) works identically either way.
exported function DoAlchemy( user, mortar, via_station := 0 )

	if( !via_station )
		if( !IsOnPlayer( user.backpack , mortar ) )
			SendSysmessage( user, "That item has to be in your backpack." );
			return;
		endif
	endif

	if( !ReserveItem( mortar ) )
		return;
	endif

	if( !via_station )
		if( !ReserveContainers( user, mortar.container ) )
			return;
		endif
	endif

	if( GetObjProperty( mortar, "contents" ) )
		TryToMakePotion( user, mortar );
		return;
	endif

	SendSysmessage( user, "What would you like to use that on?" );

	var use_on := Target( user, TGTOPT_CHECK_LOS );
	if( !use_on )
		return;
	endif

	if( !IsReagent( use_on ) )
		SendSysMessage( user, "That's not a reagent!" );
		return;
	endif

	if( !IsOnPlayer( user.backpack , use_on ) )
		SendSysmessage( user, "That item has to be in your backpack." );
		return;
	endif

	if( !ReserveItem( use_on ) )
		return;
	endif

	if( !ReserveContainers( user, use_on.container ) )
		return;
	endif

	var elem := conf[use_on.objtype];
	if( !elem )
		SendSysmessage( user, "You don't know any recipe that use this reagent." );
		return;
	endif

	var regRequest := MakeBackpackRequest( user, use_on );

	var possibles := GetConfigStringArray( elem, "potion" );
	if( !len(possibles) )
		SendSysmessage( user, "You don't know any recipe that use this reagent." );
		return;
	endif

	var menu := CreateMenu( "Select a potion to make" );
	var num_possibilities;
	var mage := GetObjProperty( user, CLASSEID_MAGE );

	foreach potion_type in possibles
		if( CanMake( user, potion_type, use_on, mage, usereg := 0, regRequest := regRequest ) )
			AddMenuItem( menu, conf[potion_type].objtype, potion_type );
			num_possibilities := 1;
		endif
	endforeach

	if( !num_possibilities )
		SendSysMessage( user, "You can't make any potions!" );
		return;
	endif

	var choice := SelectMenuItem2( user, menu ).index;
	if( !choice )
		return;
	endif

	TryToMakePotion( user, mortar, possibles[choice], use_on, mage, regRequest );

endfunction


function TryToMakePotion( user, mortar, potion := 0, reg := 0, mage := 0, regRequest := 0 )

	var bonus := 0;

	if( potion and reg )

		var difficulty := conf[potion].skill;
		var i;
		var product := conf[potion].objtype;

		if( mage )
			var itemcfg := ReadConfigFile( "itemdesc" );
			var base := itemcfg[product].Strength;
			bonus := CInt( base * ClasseBonus( user, CLASSEID_MAGE ) ) - base;
			if( !bonus )
				bonus := 1;
			endif
		endif

		if( !ReserveItem( reg ) )
			return;
		endif

		if( !regRequest )
			regRequest := MakeBackpackRequest( user, reg );
		endif

		var amount_needed := conf[potion].reagent_amount;
		if( mage )
			amount_needed := CInt(ceil( amount_needed / ClasseBonus( user, CLASSEID_MAGE ) ));
			if(GetGlobalProperty("PHC") || GetObjProperty(user, "#PPHC"))
				amount_needed := CInt(ceil(amount_needed/2.0)); // 2026-09-21: /2.0 (int/int rounds down)
			endif
			if( amount_needed < 1 )
				amount_needed := 1;
			endif
		endif

		LeaseResource( user, regRequest, amount_needed );

		SendSysmessage( user , "Difficulty: "+difficulty );

		AutoLoop_init( user );

		SendSysMessage( user, "You start grinding reagents..." );

		product := ReturnTruePotion(product, bonus);

		while( AutoLoop_more() && NOT user.dead)

			if( CanMake( user, potion, reg, mage, usereg := 1, regRequest := regRequest ) )

				for( i := 1 ; i <= NUM_PERFORM ; i := i + 1 )
					PlaySoundEffect( user, ALCH_SFX_GRIND );
					PerformAction( user, ANIM_SALUTE );
					Sleepms( ANIM_DELAY_MS );
				endfor

				if( CheckSkill( user, SKILLID_ALCHEMY, difficulty, difficulty * ALCH_POINT_MULTIPLIER ) )
					if( GetBottle( user.backpack, user, regRequest ) )
						// 2026-09-21: was CreateItemInContainer( mortar.container, ... ) with
						// the result ignored. A placed station has no container (the engine
						// returns 0 for a world item), so the create failed silently and the
						// reagents, bottle and skill roll were spent for nothing.
						if( !AlchemyCreatePotion( user, mortar, product ) )
							SendSysMessage( user, "Your backpack is full." );
							break;
						endif
						PlaySoundEffect( user, ALCH_SFX_POUR );
					else
						SendSysMessage( user, "With no bottles, you are forced to store the mixture in your mortar" );
						SetObjProperty( mortar, "contents", product );
						SendSysMessage( user, "You stop griding reagents." );
						ReleaseResourceLease(regRequest);
						AutoLoop_finish();
						return;
					endif
				else
					PlaySoundEffect( user, ALCH_SFX_POUR );
					PrintTextAbovePrivate( user, "You pour out the useless mixture", user );
				endif

			else

				break;

			endif

			if( !ExtendResourceLease(regRequest, user, amount_needed) )
				break;
			endif

		endwhile

		ReleaseResourceLease(regRequest);
		SendSysMessage( user, "You stop griding reagents." );
		AutoLoop_finish();

	else

		if( GetBottle( user.backpack, user, regRequest ) )
			var product := GetObjProperty( mortar, "contents" );
			if( mage )
				var itemcfg := ReadConfigFile( "itemdesc" );
				var base := itemcfg[product].Strength;
				bonus := CInt( base * ClasseBonus( user, CLASSEID_MAGE ) ) - base;
				if( !bonus )
					bonus := 1;
				endif
				product := ReturnTruePotion(product, bonus);
			endif

			// 2026-09-21: same station case as in the loop above -- and the stored
			// mixture is only erased once the potion really exists.
			if( !AlchemyCreatePotion( user, mortar, product ) )
				SendSysMessage( user, "Your backpack is full." );
				return;
			endif
			PlaySoundEffect( user, ALCH_SFX_POUR );
			SendSysMessage( user, "You pour the content of your mortar in a bottle." );
			EraseObjProperty( mortar, "contents" );
		else
			SendSysMessage( user, "You have no bottles to pour this into." );
			return;
		endif

	endif

endfunction


function CanMake( user, potion, reg, mage, usereg := 0, regRequest := 0 )

	var skillneeded:= conf[potion].skill;
	var amount_needed:= conf[potion].reagent_amount;

	if( mage )
		amount_needed := CInt(ceil( amount_needed / ClasseBonus( user, CLASSEID_MAGE ) ));
		//Require half resources during Crafting PH
		if(GetGlobalProperty("PHC") || GetObjProperty(user, "#PPHC"))
			amount_needed := CInt(ceil(amount_needed/2.0)); // 2026-09-21: /2.0 (int/int rounds down)
		endif
		if( amount_needed < 1 )
			amount_needed := 1;
		endif
	endif

	if( GetEffectiveSkill( user, SKILLID_ALCHEMY ) >= ( skillneeded + APPEAR_AT ) )
		var avail := 0;
		if( regRequest )
			avail := GetAvailableResource(user, regRequest).total;
		else
			avail := reg.amount;
		endif
		if( avail >= amount_needed )
			if( usereg > 0 )
				if( regRequest )
					ConsumeResource( user, regRequest, amount_needed );
				else
					SubtractAmount( reg, amount_needed );
				endif
			endif
			return 1;
		endif
	endif
	return 0;

endfunction


function GetPotionName( product )

	var alchemcfg := ReadConfigFile(":alchemy:itemdesc");
	return alchemcfg[product].sname;

endfunction


// 2026-09-21: where a finished potion goes. A carried mortar's own container
// (the bag it sits in) as before; a placed Alchemy station has no container,
// so its potions go to the brewer's backpack. Returns the item or 0.
function AlchemyCreatePotion( user, mortar, product )
	if( mortar.container )
		return CreateItemInContainer( mortar.container, product, 1 );
	endif
	return CreateItemInBackpack( user, product, 1 );
endfunction


// find a bottle in backpack and reserve it
function GetBottle( conts, user, parentRequest := 0 )
	var bottle := FindItemInContainer( conts , UOBJ_EMPTY_BOTTLE );
	if(bottle)
		if( !ReserveContainers( user, bottle.container ) )
			return 0;
		endif
		if(ReserveItem(bottle))
			SubtractAmount( bottle, 1 );
			return 1;
		endif
	endif

	var dataFileHandle := 0;
	var houseSerial := 0;
	if(parentRequest and parentRequest.dataFileHandle)
		dataFileHandle := parentRequest.dataFileHandle;
		houseSerial := parentRequest.houseSerial;
	else
		var access := FindAccessibleContainer(user, array{REMOVE_FROM_SECURE});
		if(access)
			dataFileHandle := access.df;
			if(access.house)
				houseSerial := access.house.serial;
			endif
		endif
	endif

	if(dataFileHandle)
		var bottleReq := struct{ objtype := CInt(UOBJ_EMPTY_BOTTLE), key := 0, color := 0, quality := 0, preferredSourceOrder := array{ BACKPACK, OMEGA_CACHE }, dataFileHandle := dataFileHandle, houseSerial := houseSerial, leaseKey := 0 };
		if(GetAvailableResource(user, bottleReq).total >= 1)
			if(ConsumeResource(user, bottleReq, 1))
				return 1;
			endif
		endif
	endif
	return 0;

endfunction

///////////////////////////////////////////////////////////////////////
// Toadstool - 19/04/2012
//
// ReturnTruePotion() - Workaround stacking script for POL093
//			Each type of potion can have a 'ByTrueMage' prop
//			This prop is destroyed when stack with other
//			potions with different props.
//			Had to create new potions for EVERY single possible
//			potion combination up to level 5 mage...
//
// Returns: an objtype
//
//////////////////////////////////////////////////////////////////////////
function ReturnTruePotion(type, level)

// If 0 is passed as level then we return original potion type.
// As 0 level means it doesn't get a power bonus
if (!level || level < 1)
	return type;
endif


case (type)
//HEAL
		0xDC02:
		case (level)
			1: type := 0xff19;
			2: type := 0xff1a;
			3: type := 0xff1b;
			default: type := 0xff1b; // Only situation when default is called is if the level is HIGHER, so it's maxed at level 5 mage atm
		endcase

//GHEAL
		0xDC03:
		case (level)
			1: type := 0xff1c;
			2: type := 0xff1d;
			3: type := 0xff1e;
			5: type := 0xff1f;
			6: type := 0xff20;
			default: type := 0xff20;
		endcase

//Poison
		0xdc05:
		case (level)
			1: type := 0xff22;
			2: type := 0xff23;
			default: type := 0xff23;
		endcase

//GPoison
		0xdc06:
		case (level)
			1: type := 0xff24;
			2: type := 0xff25;
			3: type := 0xff26;
			4: type := 0xff27;
			5: type := 0xff28;
			default: type := 0xff28;
		endcase

//DPoison
		0xdc17:
		case (level)
			1: type := 0xff29;
			2: type := 0xff2a;
			3: type := 0xff2b;
			5: type := 0xff2c;
			6: type := 0xff2d;
			default: type := 0xff2d;
		endcase

//ExPotion
		0xdc09:
		case (level)
			1: type := 0xff2f;
			2: type := 0xff30;
			3: type := 0xff31;
			default: type := 0xff31;
		endcase

//GExPotion
		0xdc07:
		case (level)
			1: type := 0xff32;
			2: type := 0xff33;
			3: type := 0xff34;
			5: type := 0xff35;
			6: type := 0xff36;
			default: type := 0xff36;
		endcase

//Agility
		0xdc0c:
		case (level)
			1: type := 0xff38;
			2: type := 0xff39;
			default: type := 0xff39;
		endcase

//GAgility
		0xdc0d:
		case (level)
			1: type := 0xff3a;
			2: type := 0xff3b;
			3: type := 0xff3c;
			default: type := 0xff3c;
		endcase

//FullRefresh
		0xdc0f:
		case (level)
			1: type := 0xff3d;
			2: type := 0xff3e;
			3: type := 0xff3f;
			default: type := 0xff3f;
		endcase

//Strength
		0xdc11:
		case (level)
			1: type := 0xff40;
			2: type := 0xff41;
			default: type := 0xff41;
		endcase

//GStrength
		0xdc12:
		case (level)
			1: type := 0xdc12;
			2: type := 0xdc12;
			3: type := 0xdc12;
			default: type := 0xdc12;
		endcase

//Cure
		0xdc15:
		case (level)
			1: type := 0xff45;
			2: type := 0xff46;
			3: type := 0xff47;
			default: type := 0xff47;
		endcase

//GCure
		0xdc16:
		case (level)
			1: type := 0xff48;
			2: type := 0xff49;
			3: type := 0xff4a;
			5: type := 0xff4b;
			6: type := 0xff4c;
			default: type := 0xff4c;
		endcase

		default: //no error

endcase

return type;


endfunction
