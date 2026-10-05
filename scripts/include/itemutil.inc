use uo;
use os;
use cfgfile;	// 2026-10-04: added for GetEdibleObjtypes() reading config/food.cfg

//Removed Includes
//include "include/attributes";
//Move FitRequirements to wordofpowertele.src to avoid circular includes

//Needed Includes
include "include/objtype";
include "include/constants/settings";
include "include/constants/propids";

// 2026-10-04: commented out - dead code, no callers. Superseded by the
// resourcemanager.inc helpers (ConsumeResource / ConsumeFromBackpack).
// function ConsumeObjType( who , objtype , amount )
//
// 	var item := FindItemInContainer( who.backpack , objtype );
// 	if( !item )
// 		return 0;
// 	endif
// 	
// 	if( !ReserveItem( item ) )
// 		return 0;
// 	endif
// 	
// 	if( item.amount < amount )
// 		return 0;
// 	endif
// 	
// 	if( !SubtractAmount( item , amount ) )
// 		return 0;
// 	endif
// 	
// 	return 1;
// 	
// endfunction


// 2026-10-04: commented out - dead code, no callers, and it would fail anyway:
// there is no :summoning:itemappear script in the repo.
// function CreateItemAt( where , objtype , duration := -1 , amount := 1 , color := 0 , movable := 0 )
//
// 	var parms := {};
// 	parms[1] := where;
// 	parms[2] := objtype;
// 	parms[3] := duration;
// 	parms[4] := amount;
// 	parms[5] := color;
// 	parms[6] := movable;
// 	
// 	Start_Script( ":summoning:itemappear" , parms );
// 	
// endfunction


// 2026-10-04: commented out - dead code, no callers (see also
// PlayMagicCircleEffect below; between them they were the only users of
// :summoning:magiccircleappear).
// function CreateMagicCircleAround( who , duration := 0 , color := 0 , sfx := 0 )
//
// 	var parms := {};
// 	parms[1] := who;
// 	parms[2] := duration;
// 	parms[3] := color;
// 	parms[4] := sfx;
// 	
// 	Start_Script( ":summoning:magiccircleappear" , parms );
// 	
// endfunction


// 2026-10-04: commented out - dead code, no callers (see also
// PlayWaterfallEffect below; between them they were the only users of
// :summoning:waterfallappear).
// function CreateWaterfall( who , duration := 0 , color := 0 , sfx := 0 )
//
// 	var parms := {};
// 	parms[1] := who;
// 	parms[2] := duration;
// 	parms[3] := color;
// 	parms[4] := sfx;
//
// 	Start_Script( ":summoning:waterfallappear" , parms );
//
// endfunction

	
function DupeItem( item, newitem )

	newitem.graphic := item.graphic;
	newitem.movable	:= item.movable;
		
	if( item.name )		newitem.name 		:= item.name; 		endif
	if( item.maxhp_mod ) 	newitem.maxhp_mod 	:= item.maxhp_mod; 	endif
	if( item.ar_mod ) 	newitem.ar_mod 		:= item.ar_mod; 	endif
	if( item.dmg_mod ) 	newitem.dmg_mod 	:= item.dmg_mod; 	endif
	if( item.hitscript ) 	newitem.hitscript 	:= item.hitscript; 	endif
	if( item.quality ) 	newitem.quality 	:= item.quality; 	endif
	if( item.color ) 	newitem.color 		:= item.color; 		endif
	if( item.newbie ) 	newitem.newbie 		:= item.newbie; 	endif
	if( item.facing )	newitem.facing		:= item.facing;		endif
	if( item.hp ) 		newitem.hp 		:= item.hp; 		endif
	if( item.buyprice )	newitem.buyprice 	:= item.buyprice; 	endif
	if( item.decayat )	newitem.decayat		:= item.decayat;	endif
	// 2026-10-04: these four were dropped silently. usescript is the second
	// most script-assigned item member in the repo (36 sites), so a .iteminfo
	// clone used to lose its use script, its description and its price.
	if( item.usescript )	newitem.usescript	:= item.usescript;	endif
	if( item.desc )		newitem.desc		:= item.desc;		endif
	if( item.sellprice )	newitem.sellprice	:= item.sellprice;	endif
	if( item.amount > 1 )	newitem.amount		:= item.amount;		endif

	foreach prop in GetObjPropertyNames(item)
		SetObjProperty( newitem, prop, GetObjProperty( item , prop ) );
	endforeach

endfunction


function DestroyTheItem( who , item )

	// 2026-10-04: was a bare SetObjProperty( item, "Cursed", 3 ); the named
	// constant and SetCurseLevel() below say the same thing.
	if( GetCurseLevel( item ) )
		SetCurseLevel( item , SETTING_CURSE_LEVEL_REVEALED_CAN_UNEQUIP );
	endif
	
	if( who.backpack )
		MoveItemToContainer( item , who.backpack );
	endif
	
	DestroyItem( item );
	
endfunction


// 2026-10-05: commented out - no callers left. Its 23 call sites across 9 files
// were swapped to the engine call FindObjtypeInContainer, which does the same
// search in C++ and returns on the first hit instead of building a list of the
// whole container tree first. Both skip locked sub-containers, and a not-found
// error is falsy exactly like this function's unset return, so no caller changed
// shape. 27 other sites in the repo already used the engine call.
// function FindItemInContainer( container , objtype )
//
// 	var the_item;	
// 	foreach thing in EnumerateItemsInContainer( container );
// 		if( thing.objtype == objtype )
// 			the_item := thing;
// 			break;
// 		endif
// 	endforeach
// 	
// 	return the_item;
//
// endfunction


// 2026-10-04: commented out - dead code, no callers. Superseded by the engine
// call FindObjtypeInContainer( container, objtype, FINDOBJTYPE_ROOT_ONLY ).
// function FindRootItemInContainer( container , objtype )
//
// 	var the_item;
// 	
// 	foreach thing in EnumerateItemsInContainer( container );
// 		if( (thing.container == container) and (thing.objtype == objtype) )
// 			the_item := thing;
// 			break;
// 		endif
// 	endforeach
// 	
// 	return the_item;
//
// endfunction

function GetPlatemailArmorGraphics()

	return { 0x1408, 0x1409, 0x140a, 0x140b, 0x140c,
		 0x140d, 0x140e, 0x140f, 0x1410, 0x1411,
		 0x1412, 0x1413, 0x1414, 0x1415, 0x1416,
		 0x1417, 0x1418, 0x1419, 0x141a, 0x1c04, 0x1c05,
		 0x1f0b };

	// 2026-10-04: 0x1f0b is the orc helm (also worn by KoboldOrchelm 0x32d,
	// same graphic) -- a metal helm that was in no family list, so classes
	// barred from plate could still wear one.
endfunction

function GetChainmailArmorGraphics()

	return { 0x13bb, 0x13bc, 0x13bd, 0x13be, 0x13bf,
		 0x13c0, 0x13c1, 0x13c2, 0x13c3, 0x13c4 };

endfunction

function GetRingmailArmorGraphics()

	return { 0x13e5, 0x13e6, 0x13e7, 0x13e8, 0x13e9,
		 0x13ea, 0x13eb, 0x13ec, 0x13ed, 0x13ee,
		 0x13ef, 0x13f0, 0x13f1, 0x13f2 };

endfunction

function GetBoneArmorGraphics()

	return { 0x144e, 0x144f, 0x1450, 0x1451, 0x1452,
		 0x1453, 0x1454, 0x1455, 0x1456, 0x1457 };

endfunction

function GetStuddedLeatherArmorGraphics()

	return { 0x13d4, 0x13d5, 0x13d6, 0x13d7, 0x13d8,
		 0x13d9, 0x13da, 0x13db, 0x13dc, 0x13dd,
		 0x13de, 0x13df, 0x13e0, 0x13e1, 0x13e2,
		 0x1c02, 0x1c0c };

	// 2026-10-04: 0x1c02 FemaleStudded and 0x1c0c StuddedBustier were missing,
	// so Bladesinger and Paladin could wear studded leather in female form.
	// Covers SylvianFemaleStudded 0x8249 and TourneyFemaleStudded 0x8275 too,
	// which share graphic 0x1c02.
endfunction

function GetLeatherArmorGraphics()

	return { 0x13c5, 0x13c6, 0x13c7, 0x13c8, 0x13c9,
		 0x13ca, 0x13cb, 0x13cc, 0x13cd, 0x13ce,
		 0x13cf, 0x13d0, 0x13d1, 0x13d2, 0x13d3,
		 0x1db9, 0x1dba, 0x1c06, 0x1c07, 0x1c0a,
		 0x1c0b, 0x1c00, 0x1c01, 0x1c08, 0x1c09,
		 0x141b, 0x141c, 0x1545, 0x1546, 0x1547,
		 0x1548, 0x1549, 0x154a, 0x154b, 0x154c };

	// 0x141b to 0x154c is masks
endfunction

function GetShieldGraphics()

	return { 0x1b72, 0x1b73, 0x1b74, 0x1b75, 0x1b76,
		 0x1b77, 0x1b78, 0x1b79, 0x1b7a, 0x1b7b, 0x982a,
		 0x1bc3, 0x1bc4 };

	// 2026-10-04: 0x1bc3 Chaosshield and 0x1bc4 Ordershield were in no list, so
	// every shield-restricted class could carry one. Adding the graphics also
	// covers Chaosshieldguard 0x86df, Ordershieldguard 0x86ef and TourneyShield
	// 0x8260, which reuse them -- and ebardshield 0x7ce2, which also uses
	// graphic 0x1bc3 and has no script that creates it anywhere in the repo.
endfunction

function GetShieldGraphicsThief()

	return { 0x1b72, 0x1b74, 0x1b75, 0x1b76,
		 0x1b77, 0x1b78, 0x1b79, 0x1b7a, 0x1b7b,
		 0x982a, 0x1bc3, 0x1bc4 };
	// 2026-10-05: 0x1bc3 Chaosshield and 0x1bc4 Ordershield added here too; they
	// were only in GetShieldGraphics, so thieves could still carry them (decided).

	// 2026-10-04: 0x982a NavarBloodyBarrier added -- it was restricted for every
	// other shield class but not for thieves. The buckler 0x1b73 stays off this
	// list deliberately: thieves keep it.
endfunction

function GetGraphicsCrafter()

	// This helper is referenced by classes restrictions; keep default empty set
	// until explicit crafter-only graphic restrictions are defined.
	return {};

endfunction

function GetGraphicsBladesinger()

	return { 0x1f02, 0x9B10 };
	
	// Cloak of Lettenhove, Grarks Shepherds Crook
endfunction

// 2026-10-04: commented out - dead code, no callers anywhere in the repo.
// function GetPossiblePropertyNames()
//
// 	return {	"AirProtection"		,
// 			"ArBonus"		,
// 			"AvailSkills"		,
// 			"BaseName"		,
// 			"BetweenUses"		,
// 			"bookcontent"		,
// 			"ByTrueMage"		,
// 			"CannotBeUncursed"	,
// 			"CastingOpts"		,
// 			"CastSpell"		,
// 			"ChanceOfEffect"	,
// 			"charges"		,
// 			"ChoppedBy"		,
// 			"Circle1"		,
// 			"Circle2"		,
// 			"Circle3"		,
// 			"Circle4"		,
// 			"CircleColor"		,
// 			"CircleDurationMs"	,
// 			"CircleSfx"		,
// 			"ColorMod"		,
// 			"ConfirmationMsg"	,
// 			"Consecrated"		,
// 			"CostPerCharge"		,
// 			"CreatureType"		,
// 			"Cursed"		,
// 			"DefaultDex"		,
// 			"DefaultLocation"	,
// 			"Delay"			,
// 			"DelayBetweenMs"	,
// 			"DestroyAfterUse"	,
// 			"dex"			,
// 			"DontAffectCaster"	,
// 			"EarthProtection"	,
// 			"EffectCircle"		,
// 			"EffectType"		,
// 			"ElementalSpell"	,
// 			"EnchantColor"		,
// 			"Enchanted"		,
// 			"EnchantedWith"		,
// 			"equipspell"		,
// 			"Expire"		,
// 			"ExpireNum"		,
// 			"FireProtection"	,
// 			"FixedDamages"		,
// 			"FreeAction"		,
// 			"Frequency"		,
// 			"GateDestX"		,
// 			"GateDestY"		,
// 			"GateDestZ"		,
// 			"GraphicMod"		,
// 			"Greater"		,
// 			"HealingIncrease"	,
// 			"HitscriptNumber"	,
// 			"HitWithSpell"		,
// 			"HolyProtection"	,
// 			"HpRegen"		,
// 			"HrEnd"			,
// 			"HrStart"		,
// 			"IDed"			,
// 			"int"			,
// 			"IsExceptional"		,
// 			"IsGMItem"		,
// 			"ItemAppear"		,
// 			"ItemAppearAmount"	,
// 			"ItemAppearColor"	,
// 			"ItemAppearDurationMs"	,
// 			"ItemAppearMovable"	,
// 			"keep"			,
// 			"KilledBy"		,
// 			"Lesser"		,
// 			"LootedBy"		,
// 			"LumberjackingBonus"	,
// 			"MagicImmunity"		,
// 			"MagicPenalty"		,
// 			"ManaCost"		,
// 			"ManaLevel"		,
// 			"ManaRegen"		,
// 			"Max"			,
// 			"maxcharges"		,
// 			"MaxStatMod"		,
// 			"MiningBonus"		,
// 			"name"			,
// 			"NameMod"		,
// 			"NecroProtection"	,
// 			"NoCheckLos"		,
// 			"noloot"		,
// 			"NormalRange"		,
// 			"OfBlackrock"		,
// 			"OnHitScript"		,
// 			"OnHitscriptNumber"	,
// 			"ownername"		,
// 			"ownerserial"		,
// 			"PermMagicCircle"	,
// 			"PermMagicImmunity"	,
// 			"Permmr"		,
// 			"PermPoisonImmunity"	,
// 			"PhysicalProtection"	,
// 			"PlanarSpell"		,
// 			"PlayAnimation"		,
// 			"Poisoned"		,
// 			"PoisonImmunity"	,
// 			"Poisonlvl"		,
// 			"PolymorphColor"	,
// 			"PolymorphDuration"	,
// 			"PolymorphInto"		,
// 			"PolymorphMod"		,
// 			"Powerlevel"		,
// 			"PowerWords"		,
// 			"ProtectedType"		,
// 			"Range"			,
// 			"ReagentsNeeded"	,
// 			"Rechargeable"		,
// 			"RequireNoCharges"	,
// 			"runedefs"		,
// 			"SlayType"		,
// 			"ShearingBonus"		,
// 			"skilladv"		,
// 			"skilladvamount"	,
// 			"SkillLevelMod"		,
// 			"SkillMultipliersMod"	,
// 			"SkillPoisoned"		,
// 			"SpecMult"		,
// 			"SpellCircle"		,
// 			"SpellDifficulty"	,
// 			"SpellEffectType"	,
// 			"SpellFx"		,
// 			"SpellFxLoop"		,
// 			"SpellFxSpeed"		,
// 			"SpellFxType"		,
// 			"SpellHealAmount"	,
// 			"SpellId"		,
// 			"SpellModAmount"	,
// 			"SpellModDuration"	,
// 			"SpellModId"		,
// 			"SpellPoisonLevel"	,
// 			"SpellRadius"		,
// 			"SpellReflect"		,
// 			"SpellSfx"		,
// 			"SpellTargetOption"	,
// 			"StamRegen"		,
// 			"StaticColor"		,
// 			"StaticName"		,
// 			"StaticCursedName"	,
// 			"StatLevelMod"		,
// 			"StayEquipped"		,
// 			"str"			,
// 			"SummonDelayBeforeMs"	,
// 			"SummonDelayBetweenMs"	,
// 			"SummonSfx"		,
// 			"SummonNpcTemplate"	,
// 			"TargetingType"		,
// 			"Template"		,
// 			"TriggerCircle"		,
// 			"Type"			,
// 			"WasCursed"		,
// 			"WaterProtection"	,
// 			"WordOfPower"		,
// 			"x"			,
// 			"y"			,
// 			"z"			};
// 			
// endfunction

function IsCarpet( item )

	if( item.objtype >= UOBJ_CARPET_START and item.objtype <= UOBJ_CARPET_END )
		return 1;
	else
		return 0;
	endif

endfunction

function IsCloth( item )

	if( item.objtype >= UOBJ_CLOTH_START and item.objtype <= UOBJ_CLOTH_END )
		return 1;
	else
		return 0;
	endif

endfunction

function IsConsommable( item )

	// 2026-10-04: was nineteen hardcoded objtype ranges that recognised only 79
	// of the 138 edible objtypes in config/food.cfg -- the fish steaks, the raw
	// fish, the fruit at 0x1a92-0x1a96 and the custom foods at 0x30128-0x3012b
	// among the misses, so none of them could be poisoned. It now reads
	// food.cfg, the same file the cooking package eats from, so the two can no
	// longer drift. The potion ranges the old list carried are kept below:
	// potions are not in food.cfg but poisoning a potion is intended.
	// 2026-10-05: drinks are not in food.cfg either (nothing eats them) but were
	// poisonable before the rewrite, as were onion rings and the alchemyplus
	// potion; the old ranges are restored below (decided 2026-10-05).
	var objtype := item.objtype;

	if( objtype in GetEdibleObjtypes() )
		return 1;
	elseif( objtype == 0x099b or objtype == 0x099f or objtype == 0x09c7 or objtype == 0x09c8 )	// bottles of liquor, ale, wine, jug of cider
		return 1;
	elseif( objtype >= 0x09ee and objtype <= 0x09f0 )	// mugs of ale
		return 1;
	elseif( objtype >= 0x1f7d and objtype <= 0x1f9e )	// glasses and pitchers of drink, milk, water
		return 1;
	elseif( objtype == 0xc950 or objtype == 0x7059 )	// onion rings, alchemyplus potion
		return 1;
	elseif( objtype >= 0xdc01 and objtype <= 0xdc03 )	// heal potions
		return 1;
	elseif( objtype >= 0xdc0b and objtype <= 0xdc16 )	// agility .. greater cure
		return 1;
	elseif( objtype >= 0xc900 and objtype <= 0xc947 )	// custom cooked dishes
		return 1;
	else
		return 0;
	endif
	
endfunction

// Cached because IsConsommable is called per item while poisoning, and a file
// scope initialiser would re-read the config at the start of every one of the
// 77 scripts that include this file. Built on first use instead.
var _edible_objtypes := 0;

function GetEdibleObjtypes()

	if( _edible_objtypes )
		return _edible_objtypes;
	endif

	// GetConfigIntArray, not GetConfigStringArray: the engine parses these with
	// std::stoi( .., 0 ), so the "0x09d0" form in food.cfg comes back as a
	// number. foreach over a ConfigFile yields its named elements -- food.cfg's
	// four blocks (veggie, meat, all, cooked) hold every entry between them.
	var found := {};
	var cfg := ReadConfigFile( "::food" );
	if( cfg )
		foreach elem in cfg
			foreach entry in GetConfigIntArray( elem , "food" )
				found.append( entry );
			endforeach
		endforeach
	endif

	_edible_objtypes := found;
	return _edible_objtypes;

endfunction

// 2026-10-04: renamed from IsEquipped. It never only asked -- if the item is
// not already in hand 1 or 2 it calls EquipItem and returns that result, which
// is exactly what the lumberjacking and mining call sites rely on. The old name
// read like a test and would have caught the next caller out.
function EquipIfNeeded( who, what )

	var hand1	:= GetEquipmentByLayer( who , 0x01 );
	var hand2	:= GetEquipmentByLayer( who , 0x02 );
	
	if( hand1 == what or hand2 == what )
		return 1;
	elseif( !Accessible( who, what ) )
		return 0;
	else
		return EquipItem( who , what );
	endif

endfunction

function IsFish( theobj )

	// 2026-09-14: UOBJ_FISH_START/END and UOBJ_SFISH_START/END (0x09CC-0x09D0,
	// 0x0DD6-0x0DD9) are the OLD, now-dead placeholder fish range -- nothing
	// creates them anymore, but they're kept here in case any pre-rework
	// item still exists somewhere. Real fish now use the tiered catch system
	// in pkg/std/fishing/ (Regular/Rare/Legendary, 0x30300-0x30361) --
	// crustaceans (0x30362-0x3037D) are a separate trap-only system, not
	// carved by a blade, so deliberately not included here.
	if( (theobj >= UOBJ_FISH_START) and (theobj <= UOBJ_FISH_END) )
		return 1;
	elseif( (theobj >= UOBJ_SFISH_START) and (theobj <= UOBJ_SFISH_END) )
		return 1;
	elseif( (theobj >= 0x30300) and (theobj <= 0x30361) )
		return 1;
	else
		return 0;
	endif

endfunction

function IsIDable( item )

	if( item.graphic >= 0x182a and item.graphic <= 0x1848 )
		return 0;
	elseif( item.graphic >= 0x0efb and item.graphic <= 0x0f0e )
		return 0;
	elseif( item.graphic >= 0x100e and item.graphic <= 0x1013 )
		return 0;
	elseif( IsStackable( item, item ) )
		return 0;
	else
		return 1;
	endif

endfunction

// Root level only: an item inside a bag inside this container does NOT match.
// IsOnPlayer() below is the same search with recursion -- see the note there.
function IsInContainer( container , item )

   	foreach thing in EnumerateItemsInContainer( container, ENUMERATE_ROOT_ONLY )
		if( thing.serial == item.serial )
			return 1;
		endif
   	endforeach

   	return 0;

endfunction

function IsIngot( item )

	if(( item.objtype >= UOBJ_INGOTS_START  and item.objtype <= UOBJ_INGOTS_END  ) or
	   ( item.objtype >= UOBJ_INGOTS2_START and item.objtype <= UOBJ_INGOTS2_END ) or
	   ( item.objtype  == UOBJ_GOLD_INGOT ) or
	   ( item.objtype  == UOBJ_SILVER_INGOT ) or
	   ( item.objtype  == UOBJ_COIN_COPPER_INGOT ))
		return 1;
	else
		return 0;
	endif

endfunction

function IsLog( item )

	if(( item.objtype >= UOBJ_LOGS_START  and item.objtype <= UOBJ_LOGS_END ) or ( item.objtype == UOBJ_LOGS_GODDESS ) or ( item.objtype  == UOBJ_LOGS ))
		return 1;
	else
		return 0;
	endif

endfunction

// 2026-10-04: the landtile list below is regions/ore.cfg's Global LandTile
// block, grouped into ranges, with each entry's name from that file. It used to
// be eighteen hardcoded ranges covering 129 of the config's 199 tiles, so 70
// ore tiles answered "You can't mine or dig anything there." Delete a line to
// take a tile out of mining without touching the resource config.
function IsMinable( theobj , othertype )

	if( (othertype >= 0x053b) and (othertype <= 0x0553) and (othertype != 0x0550 ) )
	
		return 1;
		
	else
		if( ((theobj >= 0x00E4) and (theobj <= 0x00E7)) or   // rock
		    ((theobj >= 0x00F4) and (theobj <= 0x00F7)) or   // rock
		    ((theobj >= 0x0104) and (theobj <= 0x0107)) or   // rock
		    ((theobj >= 0x0110) and (theobj <= 0x0113)) or   // rock
		    ((theobj >= 0x0122) and (theobj <= 0x0125)) or   // rock
		    ((theobj >= 0x01D3) and (theobj <= 0x01DA)) or   // rock
		    ((theobj >= 0x021F) and (theobj <= 0x0230)) or   // rock
		    ((theobj >= 0x0235) and (theobj <= 0x0238)) or   // rock
		    ((theobj >= 0x0245) and (theobj <= 0x026D)) or   // cave
		    ((theobj >= 0x02BC) and (theobj <= 0x02CB)) or   // cave
		    ((theobj >= 0x063B) and (theobj <= 0x063E)) or   // cave
		    ((theobj >= 0x06CD) and (theobj <= 0x06D1)) or   // rock
		    ((theobj >= 0x06DA) and (theobj <= 0x06DD)) or   // rock
		    ((theobj >= 0x06EB) and (theobj <= 0x06F2)) or   // rock
		    ((theobj >= 0x06FB) and (theobj <= 0x06FE)) or   // rock
		    ((theobj >= 0x070D) and (theobj <= 0x0714)) or   // rock
		    ((theobj >= 0x071D) and (theobj <= 0x0720)) or   // rock
		    ((theobj >= 0x072B) and (theobj <= 0x0732)) or   // rock
		    ((theobj >= 0x073B) and (theobj <= 0x073E)) or   // rock
		    ((theobj >= 0x0749) and (theobj <= 0x0750)) or   // rock
		    ((theobj >= 0x0759) and (theobj <= 0x075C)) or   // rock
		    ((theobj >= 0x07C2) and (theobj <= 0x07C4)) or   // rock
		    ((theobj >= 0x07D1) and (theobj <= 0x07D4)) or   // rock
		    ((theobj >= 0x09EC) and (theobj <= 0x0A03)) )   // rock
			return 1;
		else
			return 0;
		endif
	endif
	
endfunction

function IsOre( item )

	if( item.objtype >= UOBJ_ORE_START  and item.objtype <= UOBJ_ORE_END )
		return 1;
	elseif( item.objtype >= UOBJ_ORE2_START and item.objtype <= UOBJ_ORE2_END ) 
		return 1;
	elseif( item.objtype >= UOBJ_ORE3_START and item.objtype <= UOBJ_ORE3_END ) 
		return 1;
	else
		return 0;
	endif

endfunction

function IsHide( objtype )

	// 2026-10-04: 0x702f and 0x7030 dropped from this line -- both already fall
	// inside UOBJ_HIDES_START..UOBJ_HIDES_END on the next branch.
	if( objtype == UOBJ_HIDE1 or objtype == UOBJ_HIDE2 or objtype == 0xdc13 or objtype == 0xdc0a or objtype == 0xff17 or objtype == 0xff18 )

		return 1;
	elseif( objtype >= UOBJ_HIDES_START and objtype <= UOBJ_HIDES_END )
		return 1;
	else
		return 0;
	endif

endfunction

function IsReagent( item )

	if( item.objtype >= UOBJ_REAGENT_START and item.objtype <= UOBJ_REAGENT_END )
		return 1;
	else
		return 0;
	endif
	
endfunction

// 2026-10-04: this list is regions/sand.cfg's Global landtile block, grouped
// into ranges, with each entry's name from that file. It used to be seven
// hardcoded ranges that matched the config only partly -- 44 configured sand
// tiles were rejected and 54 accepted tiles were not in the config at all.
// Note IsMinable is tested before IsSand in mining.src.
function IsSand( theobj )

	if( ((theobj >= 0x0016) and (theobj <= 0x003A)) or   // sand
	    ((theobj >= 0x0044) and (theobj <= 0x004B)) or   // sand
	    ((theobj >= 0x011E) and (theobj <= 0x0121)) or   // sand
	    ((theobj >= 0x0126) and (theobj <= 0x012D)) or   // sand
	    ((theobj >= 0x01A8) and (theobj <= 0x01AB)) or   // sand
	    ((theobj >= 0x01B9) and (theobj <= 0x01D1)) or   // sand
	    ((theobj >= 0x0282) and (theobj <= 0x0285)) or   // sand
	    ((theobj >= 0x028A) and (theobj <= 0x0291)) or   // sand
	    ((theobj >= 0x0335) and (theobj <= 0x033C)) or   // sand
	    ((theobj >= 0x0341) and (theobj <= 0x0344)) or   // sand
	    ((theobj >= 0x034D) and (theobj <= 0x0354)) or   // sand
	    ((theobj >= 0x0359) and (theobj <= 0x035C)) or   // sand
	    ((theobj >= 0x03B7) and (theobj <= 0x03BE)) or   // sand
	    ((theobj >= 0x03C7) and (theobj <= 0x03CA)) or   // sand
	    ((theobj >= 0x05A7) and (theobj <= 0x05B2)) or   // sand
	    ((theobj >= 0x064B) and (theobj <= 0x0652)) or   // sand
	    ((theobj >= 0x0657) and (theobj <= 0x065A)) or   // sand
	    ((theobj >= 0x0663) and (theobj <= 0x066A)) or   // sand
	    ((theobj >= 0x066F) and (theobj <= 0x0672)) or   // sand
	    ((theobj >= 0x07BD) and (theobj <= 0x07C1)) or   // sand
	    ((theobj >= 0x07C5) and (theobj <= 0x07D0)) )   // sand
		return 1;
	else
		return 0;
	endif

endfunction

function IsSwamp( theobjtype )

	if( (theobjtype >= 0x053b) and (theobjtype <= 0x0553) and (theobjtype != 0x0550 ) )
		return 0;
	endif
	if( ((theobjtype >= 0x240 ) and (theobjtype <= 0x244 )) or
	    (theobjtype == 0x250) or
	    ((theobjtype >= 0x3d65) and (theobjtype <= 0x3ef0)) )
		return 1;
	else
		return 0;
	endif
	
endfunction

// 2026-10-04: this list is regions/wood.cfg's Global ObjType block, grouped
// into ranges, with each entry's name from that file. It used to be four
// hardcoded ranges covering 208 of the config's 574 objtypes, so two thirds of
// the trees had wood units allocated and regrowing that no axe could harvest.
// Delete any line here to make that kind of tree unchoppable without touching
// the resource config.
function IsTree( theobj )

	if( ((theobj >= 0x0C5E) and (theobj <= 0x0C60)) or   // vines
	    ((theobj >= 0x0C99) and (theobj <= 0x0C9D)) or   // small palm
	    (theobj == 0x0C9E) or                            // o'hii tree
	    (theobj == 0x0CA6) or                            // ponytail palm
	    (theobj == 0x0CA8) or                            // small banana tree
	    (theobj == 0x0CA9) or                            // snake plant
	    ((theobj >= 0x0CAA) and (theobj <= 0x0CAB)) or   // banana tree
	    (theobj == 0x0CC8) or                            // juniper bush
	    (theobj == 0x0CC9) or                            // spider tree
	    ((theobj >= 0x0CCA) and (theobj <= 0x0CCD)) or   // tree
	    ((theobj >= 0x0CCE) and (theobj <= 0x0CCF)) or   // leaves
	    (theobj == 0x0CD0) or                            // tree
	    ((theobj >= 0x0CD1) and (theobj <= 0x0CD2)) or   // leaves
	    (theobj == 0x0CD3) or                            // tree
	    ((theobj >= 0x0CD4) and (theobj <= 0x0CD5)) or   // leaves
	    (theobj == 0x0CD6) or                            // cedar tree
	    (theobj == 0x0CD7) or                            // cedar needles
	    (theobj == 0x0CD8) or                            // cedar tree
	    (theobj == 0x0CD9) or                            // cedar needles
	    (theobj == 0x0CDA) or                            // oak tree
	    ((theobj >= 0x0CDB) and (theobj <= 0x0CDC)) or   // oak leaves
	    (theobj == 0x0CDD) or                            // oak tree
	    ((theobj >= 0x0CDE) and (theobj <= 0x0CDF)) or   // oak leaves
	    (theobj == 0x0CE0) or                            // walnut tree
	    ((theobj >= 0x0CE1) and (theobj <= 0x0CE2)) or   // walnut leaves
	    (theobj == 0x0CE3) or                            // walnut tree
	    ((theobj >= 0x0CE4) and (theobj <= 0x0CE5)) or   // walnut leaves
	    (theobj == 0x0CE6) or                            // willow tree
	    ((theobj >= 0x0CE7) and (theobj <= 0x0CE8)) or   // willow leaves
	    ((theobj >= 0x0CE9) and (theobj <= 0x0CEA)) or   // sapling
	    ((theobj >= 0x0CEB) and (theobj <= 0x0CF2)) or   // vines
	    ((theobj >= 0x0CF3) and (theobj <= 0x0CF7)) or   // fallen log
	    (theobj == 0x0CF8) or                            // cypress tree
	    ((theobj >= 0x0CF9) and (theobj <= 0x0CFA)) or   // cypress leaves
	    (theobj == 0x0CFB) or                            // cypress tree
	    ((theobj >= 0x0CFC) and (theobj <= 0x0CFD)) or   // cypress leaves
	    (theobj == 0x0CFE) or                            // cypress tree
	    ((theobj >= 0x0CFF) and (theobj <= 0x0D00)) or   // cypress leaves
	    (theobj == 0x0D01) or                            // cypress tree
	    ((theobj >= 0x0D02) and (theobj <= 0x0D03)) or   // cypress leaves
	    ((theobj >= 0x0D41) and (theobj <= 0x0D44)) or   // tree
	    ((theobj >= 0x0D45) and (theobj <= 0x0D53)) or   // leaves
	    ((theobj >= 0x0D54) and (theobj <= 0x0D56)) or   // vines
	    ((theobj >= 0x0D57) and (theobj <= 0x0D5B)) or   // tree
	    ((theobj >= 0x0D5C) and (theobj <= 0x0D69)) or   // leaves
	    ((theobj >= 0x0D6A) and (theobj <= 0x0D6D)) or   // vines
	    ((theobj >= 0x0D6E) and (theobj <= 0x0D72)) or   // tree
	    ((theobj >= 0x0D73) and (theobj <= 0x0D7F)) or   // leaves
	    ((theobj >= 0x0D80) and (theobj <= 0x0D83)) or   // vines
	    ((theobj >= 0x0D84) and (theobj <= 0x0D86)) or   // tree
	    ((theobj >= 0x0D87) and (theobj <= 0x0D90)) or   // leaves
	    ((theobj >= 0x0D91) and (theobj <= 0x0D93)) or   // vines
	    (theobj == 0x0D94) or                            // apple tree
	    (theobj == 0x0D95) or                            // leaves
	    (theobj == 0x0D96) or                            // apple tree
	    (theobj == 0x0D97) or                            // fall leaves
	    (theobj == 0x0D98) or                            // apple tree
	    (theobj == 0x0D99) or                            // leaves
	    (theobj == 0x0D9A) or                            // apple tree
	    (theobj == 0x0D9B) or                            // fall leaves
	    (theobj == 0x0D9C) or                            // peach tree
	    (theobj == 0x0D9D) or                            // leaves
	    (theobj == 0x0D9E) or                            // peach tree
	    (theobj == 0x0D9F) or                            // fall leaves
	    (theobj == 0x0DA0) or                            // peach tree
	    (theobj == 0x0DA1) or                            // leaves
	    (theobj == 0x0DA2) or                            // peach tree
	    (theobj == 0x0DA3) or                            // fall leaves
	    (theobj == 0x0DA4) or                            // pear tree
	    (theobj == 0x0DA5) or                            // leaves
	    (theobj == 0x0DA6) or                            // pear tree
	    (theobj == 0x0DA7) or                            // fall leaves
	    (theobj == 0x0DA8) or                            // pear tree
	    (theobj == 0x0DA9) or                            // leaves
	    (theobj == 0x0DAA) or                            // pear tree
	    (theobj == 0x0DAB) or                            // fall leaves
	    ((theobj >= 0x1B1F) and (theobj <= 0x1B26)) or   // leaves
	    ((theobj >= 0x224A) and (theobj <= 0x224B)) or   // willow tree
	    ((theobj >= 0x224C) and (theobj <= 0x224D)) or   // oak tree
	    ((theobj >= 0x309C) and (theobj <= 0x30A5)) or   // tree
	    ((theobj >= 0x30A6) and (theobj <= 0x30BC)) or   // leaves
	    ((theobj >= 0x30BD) and (theobj <= 0x30C7)) or   // tree
	    ((theobj >= 0x30C8) and (theobj <= 0x30D3)) or   // leaves
	    ((theobj >= 0x30D4) and (theobj <= 0x30DE)) or   // tree
	    (theobj == 0x3123) or                            // peach tree
	    (theobj == 0x3124) or                            // apple tree
	    (theobj == 0x36A0) or                            // leaves
	    ((theobj >= 0x39A3) and (theobj <= 0x39D3)) or   // tree
	    ((theobj >= 0x39D5) and (theobj <= 0x3A54)) or   // tree
	    ((theobj >= 0x3A9C) and (theobj <= 0x3AEF)) or   // tree
	    (theobj == 0xA65C) or                            // o'hii tree
	    ((theobj >= 0xA65D) and (theobj <= 0xA65F)) or   // leaves
	    ((theobj >= 0xA660) and (theobj <= 0xA661)) or   // oak leaves
	    ((theobj >= 0xA662) and (theobj <= 0xA663)) or   // walnut leaves
	    (theobj == 0xA664) or                            // willow leaves
	    ((theobj >= 0xA665) and (theobj <= 0xA668)) or   // leaves
	    (theobj == 0xA669) or                            // o'hii tree
	    ((theobj >= 0xA66A) and (theobj <= 0xA66C)) or   // leaves
	    ((theobj >= 0xA66D) and (theobj <= 0xA66E)) or   // oak leaves
	    ((theobj >= 0xA66F) and (theobj <= 0xA670)) or   // walnut leaves
	    (theobj == 0xA671) or                            // willow leaves
	    (theobj == 0xA676) or                            // o'hii tree
	    ((theobj >= 0xA677) and (theobj <= 0xA679)) or   // leaves
	    ((theobj >= 0xA67A) and (theobj <= 0xA67B)) or   // oak leaves
	    ((theobj >= 0xA67C) and (theobj <= 0xA67D)) or   // walnut leaves
	    (theobj == 0xA67E) or                            // willow leaves
	    ((theobj >= 0xA6F1) and (theobj <= 0xA6F2)) or   // cypress tree
	    ((theobj >= 0xA6FE) and (theobj <= 0xA6FF)) or   // cypress tree
	    ((theobj >= 0xA708) and (theobj <= 0xA709)) or   // cedar tree
	    (theobj == 0xA70A) or                            // o'hii tree
	    ((theobj >= 0xA70B) and (theobj <= 0xA70D)) or   // tree
	    ((theobj >= 0xA70E) and (theobj <= 0xA70F)) or   // oak tree
	    ((theobj >= 0xA710) and (theobj <= 0xA711)) or   // walnut tree
	    (theobj == 0xA712) or                            // willow tree
	    ((theobj >= 0xA713) and (theobj <= 0xA716)) )   // cypress tree
		return 1;
	else
		return 0;
	endif
		
endfunction

function IsWater( theobj )

	if( theobj.item )
		if( (theobj.objtype >=0x346E) and (theobj.objtype <= 0x3485) )
			return 1;
		elseif( (theobj.objtype >= 0x3494) and (theobj.objtype <= 0x34AB) )
			return 1;
		elseif( (theobj.objtype >= 0x34B8) and (theobj.objtype <= 0x34CA) )
			return 1;
		elseif( (theobj.objtype >= 0x1796) and (theobj.objtype <= 0x17B2) )
			return 1;
		endif
	endif
	
	var info := GetMapInfo( cint(theobj.x) , cint(theobj.y), theobj.realm );
	var tile := info.landtile;

	if( tile >= 0x04c and tile <= 0x06f )
		return 1;
	elseif( tile >= 0x0a8 and tile <= 0x0ab )
		return 1;
	elseif( tile == 0x0136 or tile == 0x0137 )
		return 1;
	elseif( tile >= 0x3ff0 and tile <= 0x3ff3 )
		return 1;
	endif
    	
	return 0;

endfunction

// 2026-10-04: commented out - dead code, no callers. Hardcoded 1500ms variant
// of the (also dead) CreateMagicCircleAround above.
// function PlayMagicCircleEffect( who ) 
//
// 	var parms := {};
// 	parms[1] := who;
// 	parms[2] := 1500;
// 	parms[3] := 0;
// 	parms[4] := 0;
// 	
// 	Start_Script( ":summoning:magiccircleappear" , parms );
// 	
// endfunction

// 2026-10-04: commented out - dead code, no callers. Hardcoded 5000ms/sfx 0x218
// variant of the (also dead) CreateWaterfall above.
// function PlayWaterfallEffect( who )
//
// 	var parms := {};
// 	parms[1] := who;
// 	parms[2] := 5000;
// 	parms[3] := 0;
// 	parms[4] := 0x218;
// 	
// 	Start_Script( ":summoning:waterfallappear" , parms );
// 	
// endfunction

// 2026-10-04: commented out - dead code, no callers. Nothing bulk-clears these
// hitscript cprops any more; .iteminfo erases them one at a time.
// function ResetAllHitscriptPropsExcep( item , excep := "" )
//
// 	var cprops := {	"ChanceOfEffect",
// 				"EffectCircle"	,
// 				"HitWithSpell"	,
// 				"Poisonlvl"	,
// 				"Powerlevel"	,
// 				"ProtectedType"	,
// 				"SlayType"	};
// 				
// 	foreach cprop in cprops
// 		if( cprop != excep )
// 			EraseObjProperty( item , cprop );
// 		endif
// 	endforeach
// 	
// endfunction

function GetCurseLevel( byref item )

	return Cint(GetObjProperty( item, PROPID_ITEM_CURSED ));
	
endfunction

function SetCurseLevel( byref item, byref level )

	level := Cint(level);
	if( level != SETTING_CURSE_LEVEL_NOT_CURSED )
		return SetObjProperty( item, PROPID_ITEM_CURSED, level );
	else
		return EraseObjProperty( item, PROPID_ITEM_CURSED );
	endif
	
endfunction

// Recursive: matches an item nested in a sub-container as well. Despite the
// name it takes any container, and it is the deep counterpart of
// IsInContainer() above, which is root level only. 2026-10-04: neither name
// said which depth it used, and the 33 call sites between them were written
// against whichever one the author happened to pick.
function IsOnPlayer( container , item )

   	foreach thing in EnumerateItemsInContainer( container )
		if( thing.serial == item.serial )
			return 1;
		endif
   	endforeach

   	return 0;

endfunction

function ReserveContainers( who, container )

	while(container != who.backpack)	
		if(!ReserveItem( container ))
   			return 0;
		endif
		container := container.container;
	endwhile
	return 1;

endfunction

function IsMetalEquipment(item)

	var metalstuff := {0x0f45, 0x0f47, 0x0f49, 0x0f4b, 0x0f4e, 0x0f51,
	0x0f5c, 0x0f5e, 0x0f60, 0x0f63, 0x13af, 0x13b5, 0x13b9, 0x13bb, 0x13bf,
	0x13c3, 0x13ec, 0x13ee, 0x13f0, 0x13f2, 0x13fa, 0x13fe, 0x1401, 0x1402,
	0x1404, 0x1406, 0x1408, 0x140a, 0x140c, 0x140e, 0x1410, 0x1411, 0x1412,
	0x1413, 0x1414, 0x1415, 0x1438, 0x143a, 0x143c, 0x143e, 0x1440, 0x1442,
	0x144E, 0x144F, 0x1450, 0x1451, 0x1452, 0x151A, 0x156A, 0x1b72,	0x1b73,
	0x1b74, 0x1b76, 0x1b7b, 0x1c04, 0x1f0b, 0x27AB, 0x982a};

	if (item.graphic in metalstuff)
		return 1;
	else
		return 0;
	endif

endfunction

function IsWoodEquipment(item)

	// 2026-10-04: five graphics were listed twice (0x0f4f, 0x13fd, 0x1403,
	// 0x6050, 0xb201) and the six archery weapons added 2026-08-23 were absent,
	// so a Refining Varnish refused them. Deduplicated, and the six added.
	var woodstuff := {0x0DF0, 0x0E81, 0x0E8A, 0x13B2, 0x13b3, 0x13F9, 0x1B78,
	0x1B79, 0x1B7A, 0x0f4f, 0x13fd, 0x1403, 0x6050, 0xb201,
	0xa915, 0x26c2, 0x26c3, 0x27a5, 0x2d1e, 0xb4dc};
	// Shortbow, CompositeBow, RepeatingCrossbow, Yumi,
	// ElvenCompositeLongbow, SkullCrossbow

	if (item.graphic in woodstuff)
		return 1;
	else
		return 0;
	endif

endfunction

function IsLeatherEquipment(item)

	// 2026-10-04: every tailoring item whose MaterialType is Hide now matches.
	// The Shoes category and the Leather Misc masks were missing, so a Refining
	// Compound refused them even though GetLeatherArmorGraphics already counted
	// the masks as leather armour -- the two lists disagreed with each other.
	var leatherstuff := {0x13C5, 0x13C6, 0x13C7, 0x13CB, 0x13CC, 0x13D4, 0x13D5,
	0x13D6, 0x13DA, 0x13DB, 0x1C00, 0x1C02, 0x1C06, 0x1C08, 0x1C0A, 0x1C0C, 0x1DB9
	//0x170B, 0x170D, 0x170F, 0x1711,
	//0x141B, 0x141C, 0x1545, 0x1546, 0x1547, 0x1548, 0x1549, 0x154A, 0x154B, 0x154C
	};
	// LeatherBoots, Sandals, Shoes, ThighBoots,
	// then the orc / bear / deer / tribal / voodoo masks and their flip graphics

	if (item.graphic in leatherstuff)
		return 1;
	else
		return 0;
	endif

endfunction

function IsSign (item)
	// 2026-10-04: 0xbd0 and 0xbd2 dropped from the case -- both already fall
	// inside the 0xba3-0xc0e range below.
	case (item.objtype)
		0xb95:
		0xb96:
		0xc43:
		0xc44:
		0x1f28:
		0x1f29:
		0x17060:
		0x1708a:
			return 1;
	endcase
	if (item.objtype >= 0xba3 and item.objtype <= 0xc0e)
		return 1;
	elseif (item.objtype >= 0x1297 and item.objtype <= 0x129e)
		return 1;
	endif
	return 0;
endfunction

function IsJewel(item)

	case (item.graphic)
		0x1085:
		0x1086:
		0x1087:
		0x1088:
		0x1089:
		0x108A:
		0x1F05:
		0x1F06:
		0x1F07:
		0x1F08:
		0x1F09:
		0x1F0A:
		0x5015:
			return 1;
	endcase

	// 2026-10-04: 0x1F05-0x1F0A added. tinker.cfg makes twelve jewellery pieces
	// and only the first six were listed, so the beaded necklace, bracelet2,
	// earrings2, necklace3, ring2 and silver necklace were invisible to the
	// ritual jewel checks, to vitalInfusion's skilladv read and to the tooltip.
	return 0;
endfunction

function IsCBoost(item)
	if ((item.objtype >= 0x30381) && (item.objtype <= 0x30384))
		return 1;
	endif
	return 0;
endfunction