use uo;
use os;
use basic;
use cfgfile;
use polsys;

include "include/client";
include "include/objtype";
include "include/drinkpotion";
include "include/dotempmods";
include "include/classes";
include "include/itemutil";
include "include/string";
include "include/random";
include ":mdgumps:yesNo";
include "include/attributes";
include "include/difficulty";

const DIFF_MULTIPLIER	:= 20;
const POINT_MULTIPLIER	:= 20;
const KARMA_PENALTY_FOR_FOOD_POISONING := -50;

// Poisoning-as-crafting-skill additions.
const UOBJ_VIAL_OF_VENOM	:= 0x5722;
const UOBJ_TOXIN_FLASK		:= 0x182b;
const BREW_APPEAR_AT		:= -10;
const SFX_GRIND			:= 0x0243;
const SFX_POUR			:= 0x0241;

var madeperm;
var spec_lvl:=0;
var failedperm :=0;
var chancenum := 1;
var chance :=0;
var NOPicked :=0;


function ApplyPoison( character )

	var the_item;
	var potion;

	SendSysMessage( character , "Select a poison potion." );

	potion := Target( character , TGTOPT_CHECK_LOS );
	if( !potion )
		return;
	endif

	if( !Accessible( character , potion ) or !potion.movable )
		SendSysmessage( character , "You can't reach that!" );
		return;
	endif

	if( !IsInContainer( character.backpack , potion ) )
		SendSysmessage( character, "That item has to be in your backpack." );
		return;
	endif

	if( potion.graphic != UOBJ_POISON_POTION )
		SendSysmessage (character, "You must select a poison potion!");
		return;
	endif

	if( !ReserveItem( potion ) )
		return;
	endif

	var power := Getpower(character, potion);

	if (!power)
		SendSysMessage(character, "That potion is too advanced for you to work with.");
		return 0;
	endif

	SendSysmessage( character , "Select an object to which to apply this poison" );

	the_item := Target( character , TGTOPT_CHECK_LOS );
	if( !the_item )
		return;
	endif

	if( !Accessible( character , the_item ) or !the_item.movable )
		SendSysmessage( character , "You can't reach that!" );
		return;
	endif

	if( !ReserveItem( the_item ) )
		return;
	endif

	if( the_item.newbie )
		SendSysMessage( character, "You can't poison that." );
		return 0;
	endif

	if( the_item.hitscript == "blackrockscript" )
		SendSysMessage( character, "You can't poison that." );
		return 0;
	endif

	if( the_item.isa( POLCLASS_WEAPON ) )
		PoisonWeapon( character , potion , the_item );
	elseif( IsConsommable( the_item ) )
		PoisonFood( character , potion , the_item );
	elseif (the_item.objtype ==	3617) // bandage
		PoisonBandage(character, potion, the_item);
	else
		SendSysMessage( character , "You can't poison that." );
	endif

endfunction


// Need to be within 20 points to attempt
function Getpower(who, potion)

var power := Cint( GetPotionStrength( potion ) );
var diff  := Cint( power * DIFF_MULTIPLIER );

var poisoning_skill := GetEffectiveSkill( who , SKILLID_POISONING );
if( diff > 150 )
	diff := 150;
endif

if (poisoning_skill+20 < diff)
	SendSysMessage(who, "You Assess the poison at "+diff+" difficulty.");
	return 0;
endif

PrintTextAbovePrivate(who, "You Assess the poison at "+diff+" difficulty.", who);
return 1;

endfunction

function PoisonWeapon( character , potion , the_item )

	var power := Cint( GetPotionStrength( potion ) );
	var diff  := Cint( power * DIFF_MULTIPLIER );
	var poisoning_skill := GetEffectiveSkill( character , SKILLID_POISONING );

	var cp := GetObjProperty(the_item, "HitscriptNumber" );
	if(cp == 38)
		SendSysMessage(character, "Can't poison this.");
		return;
	endif

	if( GetObjProperty( character , "cwarring" ) and GetObjProperty(the_item,"OwnerName") != character.name )
		SendSysMessage(character, "You can only poison your own weapons in color wars!");
		return;
	endif

	if( GetObjProperty( the_item , "IsGMItem" ) )
		diff := diff + 35;
	elseif (the_item.dmg_mod)
		diff := diff + the_item.dmg_mod;
	endif

	if( diff > 150 )
		diff := 150;
	endif
		sendDiff( character , diff );

	if (poisoning_skill+20 < diff)
		SendSysMessage(character, "The required difficulty of poisoning the selected weapon is too high for you to attempt.");
		return 0;
	endif

	SubtractAmount( potion , 1 );
	CreateItemInBackpack( character , UOBJ_EMPTY_BOTTLE , 1 );

		if( poisoning_skill < 50 )
			power := power - 2;
		elseif( poisoning_skill < 75 )
			power := power - 1;
		elseif( poisoning_skill > 149 )
			power := power + 2;
		elseif( poisoning_skill > 135 )
			power := power + 1;
		endif

		if( power < 1 )
			power := 1;
		endif

		if( CheckSkill( character , SKILLID_POISONING , diff , diff * POINT_MULTIPLIER ) )
			if( GetObjProperty( character , CLASSEID_THIEF ) )
				power := power + 1;
			endif

			if( power > 6 )
				power := 6;
			endif

			var charges := CInt( GetObjProperty( the_item, "PoisonCharges" ));
			if( !charges )
				charges := 1;
			else
				charges += 1;
			endif

			var level := GetClasseLevel(character);
			if( GetObjProperty( character , CLASSEID_THIEF ) )
				charges += level;
			endif

			PlaySoundEffect( character , SFX_248 );
			SetObjProperty( the_item, "PoisonCharges", charges );
			SetObjProperty( the_item , "SkillPoisoned" , power );
			SendSysMessage( character , "You successfully apply the poison on the " + TruncateArticle(the_item.desc) + "." );
			IncRevision(the_item);
		else
			poisoning_skill := poisoning_skill - Cint( diff / 2 );
			if( GetObjProperty( character , CLASSEID_THIEF ) )
				poisoning_skill := poisoning_skill * CLASSE_BONUS;
			endif

			if( Random(100) > poisoning_skill )
				SetPoison( character , power , "You fail and poison yourself!" );
			else
				SendSysMessage( character , "You fail to apply the poison properly" );
			endif
		endif

endfunction

function PoisonBandage( who , potion , bandage )

	if( GetObjProperty( bandage , "Poisoned" ) )
		SendSysMessage( who , "This is already poisoned." );
		return;
	endif

	var power := GetPotionStrength( potion );
	var diff  := power * (DIFF_MULTIPLIER-5);

	if( diff > 120 )
		diff := 120;
	endif

	SubtractAmount( potion , 1 );
	CreateItemInBackpack( who , UOBJ_EMPTY_BOTTLE , 1 );

	if( CheckSkill( who , SKILLID_POISONING , diff , diff * POINT_MULTIPLIER ) )
		if( GetObjProperty( who , CLASSEID_THIEF ) )
			power := Cint( power * ClasseBonus( who , CLASSEID_THIEF ) );
		endif

		if( power > 7 )
			power := 7;
		endif

		PlaySoundEffect( who , SFX_248 );
		SetObjProperty( bandage , "Poisoned" , power );
		if (bandage.amount == 1)
			SendSysMessage( who , "You successfully poisoned a " + TruncateArticle(bandage.desc) + "." );
		else
			SendSysMessage( who , "You successfully poisoned " + TruncateArticle(bandage.desc) + "." );
		endif
		var color, name;

		case (power)
			1:	color := 61;	name := "weakly poisoned bandage";
			2:	color := 65;	name := "mildly poisoned bandage";
			3:	color := 64;	name := "moderately poisoned bandage";
			4: 	color := 63;	name := "heavily poisoned bandage";
			5:	color := 62;	name := "massively poisoned bandage";
			6:	color := 67;	name := "severely poisoned bandage";
			7:	color := 68;	name := "severely poisoned bandage";
		endcase

		SetName(bandage, name);
		bandage.color := color;

	else
		var poisoning_skill := GetEffectiveSkill( who , SKILLID_POISONING ) - Cint( diff / 2 );
		if( GetObjProperty( who , CLASSEID_THIEF ) )
			poisoning_skill := poisoning_skill * ClasseBonus( who , CLASSEID_THIEF );
		endif

		if( Random(100) > poisoning_skill )
			SetPoison( who , power , "You fail and poison yourself!" );
		else
			SendSysMessage( who , "You fail to apply the poison properly" );
		endif
	endif

endfunction

function PoisonFood( character , potion , the_item )

	if( GetObjProperty( the_item , "Poisoned" ) )
		SendSysMessage( character , "This is already poisoned." );
		return;
	endif

	var power := GetPotionStrength( potion );
	var diff  := power * (DIFF_MULTIPLIER-5);

	if( diff > 120 )
		diff := 120;
	endif

	AwardKarma( character , KARMA_PENALTY_FOR_FOOD_POISONING );
	SubtractAmount( potion , 1 );
	CreateItemInBackpack( character , UOBJ_EMPTY_BOTTLE , 1 );

	if( CheckSkill( character , SKILLID_POISONING , diff , diff * POINT_MULTIPLIER ) )
		if( GetObjProperty( character , CLASSEID_THIEF ) )
			power := Cint( power * ClasseBonus( character , CLASSEID_THIEF ) );
		endif

		if( power > 7 )
			power := 7;
		endif

		PlaySoundEffect( character , SFX_248 );
		SetObjProperty( the_item , "Poisoned" , power );
		SendSysMessage( character , "You successfully poisoned this " + TruncateArticle(the_item.desc) + "." );
	else
		var poisoning_skill := GetEffectiveSkill( character , SKILLID_POISONING ) - Cint( diff / 2 );
		if( GetObjProperty( character , CLASSEID_THIEF ) )
			poisoning_skill := poisoning_skill * ClasseBonus( character , CLASSEID_THIEF );
		endif

		if( Random(100) > poisoning_skill )
			SetPoison( character , power , "You fail and poison yourself!" );
		else
			SendSysMessage( character , "You fail to apply the poison properly" );
		endif
	endif

endfunction

function chanceperm(character, potion,the_item)

	madeperm :=0;
	failedperm:=0;
	var power := Cint( GetPotionStrength( potion ) );
	var diff  := Cint( power * DIFF_MULTIPLIER );
	var poisoning_skill := GetEffectiveSkill( character , SKILLID_POISONING );

		chance := randomint(300)+1;
		if(chance < (chancenum + 1))   // 1% - nonspec  2% spec
			if( CheckSkill( character , SKILLID_POISONING , diff , diff * POINT_MULTIPLIER ) )
				if( YesNo( character , "Make Perm?" ) )
					PlaySoundEffect( character , SFX_248 );
					the_item.color := 1169;
					madeperm :=1;
					the_item.hitscript :=":combat:thiefpoisonhit";
//					the_item.setprop("Poisonlvl",3);
					SetObjProperty(the_item,"Poisonlvl",3);
					SetObjProperty(the_item,"Thiefpermweapon",1);
					SendSysMessage( character , "You Poisoned the " + TruncateArticle(the_item.desc) + " so well, it is permanent." );
					the_item.name :="Poisoned "+the_item.name;
//					the_item.controlscript :="::applyhitscript";
//					the_item.Multiplier:=0.5;

					EraseObjProperty( the_item , "SkillPoisoned");
				else
					NOPicked :=1;
				endif
			else
				failedperm := 1;
				poisoning_skill := poisoning_skill - Cint( diff / 2 );
				if( GetObjProperty( character , CLASSEID_THIEF ) )
					poisoning_skill := poisoning_skill * CLASSE_BONUS;
				endif

				if( Random(100) > poisoning_skill )
					SetPoison( character , power , "You fail and poison yourself!" );
				else
					SendSysMessage( character , "You fail to apply the poison properly" );
				endif

			endif
		endif
endfunction


///////////////////////////////////////////////////////////////////////
// Poisoning-as-crafting-skill: brew poison potions with the Toxin Flask.
// Mirrors alchemy.src's TryToMakePotion()/CanMake() -- same recipe
// shape, same skill-check math, just SKILLID_POISONING instead of
// SKILLID_ALCHEMY. Thief gets the same ByTrueMage-style bonus Mage gets
// in Alchemy, via ByTrueThief instead, same ClasseBonus() formula.
///////////////////////////////////////////////////////////////////////
// via_station (crafting-station rollout, 2026-09-12): a placed Alchemy
// station isn't "on the player", so the carried-Toxin-Flask-only check is
// skipped when invoked from a station instead of a held flask -- brewing
// still needs Nightshade and an empty bottle either way.
exported function BrewPoison( character, flask, via_station := 0 )

	if( !via_station )
		if( !IsOnPlayer( character.backpack , flask ) )
			SendSysmessage( character, "That item has to be in your backpack." );
			return;
		endif
	endif

	if( !ReserveItem( flask ) )
		return;
	endif

	var nightshade := FindItemInContainer( character.backpack, UOBJ_NIGHTSHADE );
	if( !nightshade )
		SendSysMessage( character, "You need Nightshade to brew poison." );
		return;
	endif

	if( !ReserveItem( nightshade ) )
		return;
	endif

	var poisoncfg := ReadConfigFile( "poisoning" );
	var tiers  := { "Lesser_Poison", "Poison", "Greater_Poison", "Deadly_Poison" };
	var labels := { "Lesser Poison", "Poison", "Greater Poison", "Deadly Poison" };

	var menu := CreateMenu( "Select a poison to brew" );
	var num_possibilities := 0;

	foreach tier in tiers
		if( CanBrewPoison( character, poisoncfg, tier, nightshade.amount ) )
			AddMenuItem( menu, poisoncfg[tier].objtype, labels[_tier_iter] );
			num_possibilities := 1;
		endif
	endforeach

	if( !num_possibilities )
		SendSysMessage( character, "You don't have the skill or enough Nightshade to brew any poison yet." );
		return;
	endif

	var choice := SelectMenuItem2( character, menu ).index;
	if( !choice )
		return;
	endif

	var tier          := tiers[choice];
	var difficulty    := poisoncfg[tier].skill;
	var product       := poisoncfg[tier].objtype;
	var amount_needed := poisoncfg[tier].reagent_amount;

	var thief := GetObjProperty( character, CLASSEID_THIEF );
	var bonus := 0;

	if( thief )
		var itemcfg := ReadConfigFile( ":alchemy:itemdesc" );
		var base := itemcfg[product].Strength;
		bonus := CInt( base * ClasseBonus( character, CLASSEID_THIEF ) ) - base;
		if( !bonus )
			bonus := 1;
		endif
	endif

	SubtractAmount( nightshade, amount_needed );

	SendSysMessage( character, "You start brewing the poison..." );
	PlaySoundEffect( character, SFX_GRIND );
	PerformAction( character, ANIM_SALUTE );
	SleepMS( 1500 );

	if( CheckSkill( character, SKILLID_POISONING, difficulty, difficulty * POINT_MULTIPLIER ) )
		product := ReturnThiefPotion( product, bonus );
		if( FindAndConsumeBottle( character ) )
			CreateItemInContainer( character.backpack, product, 1 );
			PlaySoundEffect( character, SFX_POUR );
			SendSysMessage( character, "You successfully brew the poison." );
		else
			SendSysMessage( character, "You have no empty bottles to pour this into." );
		endif
	else
		PlaySoundEffect( character, SFX_POUR );
		SendSysMessage( character, "You fail to brew the poison, wasting the reagents." );
	endif

endfunction


function CanBrewPoison( character, poisoncfg, tier, available )

	var skillneeded    := poisoncfg[tier].skill;
	var amount_needed  := poisoncfg[tier].reagent_amount;

	if( GetEffectiveSkill( character, SKILLID_POISONING ) < ( skillneeded + BREW_APPEAR_AT ) )
		return 0;
	endif

	if( available < amount_needed )
		return 0;
	endif

	return 1;

endfunction


function FindAndConsumeBottle( character )

	var bottle := FindItemInContainer( character.backpack, UOBJ_EMPTY_BOTTLE );
	if( bottle )
		SubtractAmount( bottle, 1 );
		return 1;
	endif

	return 0;

endfunction


// Same shape as alchemy.src's ReturnTruePotion(), same POL093 stacking
// workaround (a raw CProp doesn't survive stacking, hence one objtype
// per bonus level) -- carries ByTrueThief instead of ByTrueMage.
function ReturnThiefPotion( type, level )

	if( !level || level < 1 )
		return type;
	endif

	case( type )
		0xdc05: // Poison
			case( level )
				1: type := 0x7af8;
				default: type := 0x7af9;
			endcase

		0xdc06: // Greater Poison
			case( level )
				1: type := 0x7afa;
				2: type := 0x7afb;
				3: type := 0x7afc;
				4: type := 0x7afd;
				default: type := 0x7afe;
			endcase

		0xdc17: // Deadly Poison
			case( level )
				1: type := 0x7aff;
				2: type := 0x7b00;
				3: type := 0x7b01;
				4: type := 0x7b02;
				5: type := 0x7b03;
				default: type := 0x7b04;
			endcase

		default: // Lesser Poison has no bonus tier, same as Mage's version
	endcase

	return type;

endfunction
