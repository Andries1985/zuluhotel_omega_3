use uo;
use os;

//include "include/spelldata";
include "include/itemutil";
include "include/client";
include ":staff:include/staff";
include "include/attributes";

const CLASSE_BONUS        		:= 1.5;
const CLASSEID_BARD       		:= "IsBard";
const CLASSEID_BLADESINGER 		:= "IsBladesinger";
const CLASSEID_CRAFTER    		:= "IsCrafter";
const CLASSEID_MAGE       		:= "IsMage";
const CLASSEID_MYSTIC_ARCHER	:= "IsMysticArcher";
const CLASSEID_PALADIN     		:= "IsPaladin";
const CLASSEID_RANGER     		:= "IsRanger";
const CLASSEID_THIEF      		:= "IsThief";
const CLASSEID_WARRIOR    		:= "IsWarrior";
const CLASSEID_POWERPLAYER		:= "IsPowerplayer";

const EACH_MUST_REPRESENT := 7.5;
const REPRESENT_LEVEL_MOD := 1.0;
const AVERAGE_SKILL       := 75;
const AVERAGE_LEVEL_MOD   := 15;
const BONUS_PER_LEVEL     := 0.25;
const SMALL_BONUS_PER_LEVEL := 0.15;

const RANGER_LEVEL_ARCHERY_RANGE_BONUS := 1; 		//1 range per level further range before 1/4 base damage done

const CRAFTER_REPAIR_DIFF_BONUS := 2.5; // used in util/repair.inc


function ClasseBonus( who , classeid )

	var level := CInt( GetObjProperty( who , classeid ) );
	// 2026-09-23: the paperdoll sweep (unequipRestrictedItems) used to run here on every lookup,
	// with two 5 ms pauses per worn item, from 127 call sites; it stays in AssignClasse,
	// the class-check loop and .showclasse, where the class is actually (re)assigned.
	if( level )
		return( 1.0 + BONUS_PER_LEVEL * level );
	else
		return( 1.0 );
	endif

endfunction


function ClasseBonusByLevel( level )

	return CDbl( 1.0 + ( BONUS_PER_LEVEL * CDbl(level) ) );

endfunction

function ClasseSmallBonusByLevel( level )

	return CDbl( 1.0 + ( SMALL_BONUS_PER_LEVEL * CDbl(level) ) );

endfunction


function ClasseBonusBySkillId( who, skillid )

	// 2026-09-23: the paperdoll sweep (unequipRestrictedItems) used to run here on every lookup,
	// with two 5 ms pauses per worn item, from 127 call sites; it stays in AssignClasse,
	// the class-check loop and .showclasse, where the class is actually (re)assigned.
	return( ClasseBonusByLevel( IsSpecialisedIn( who , skillid ) ) );

endfunction


function ByPassForMages( classeid )

	var fract_array := {};
	var restricted_types := {};

	case( classeid )
		CLASSEID_MAGE:		
					fract_array.append( GetLeatherArmorGraphics() );
					fract_array.append( GetStuddedLeatherArmorGraphics() );
					break;

	endcase

	foreach type in fract_array
		foreach graphic in type
			restricted_types.append( graphic );
		endforeach
	endforeach

	return restricted_types;

endfunction


function EnumerateRestrictedItemTypesFromClasse( classeid )

	var fract_array := {};
	var restricted_types := {};

	case( classeid )
		CLASSEID_BARD:		
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					fract_array.append( GetGraphicsCrafter() );
					break;
		CLASSEID_BLADESINGER:
					fract_array.append( GetLeatherArmorGraphics() );
					fract_array.append( GetStuddedLeatherArmorGraphics() );
					fract_array.append( GetGraphicsBladesinger() );
					fract_array.append( GetShieldGraphics() );
					fract_array.append( GetGraphicsCrafter() );
					break;
		CLASSEID_CRAFTER:
					break;
		CLASSEID_MAGE:		
					fract_array.append( GetBoneArmorGraphics() );
					fract_array.append( GetRingmailArmorGraphics() );
					fract_array.append( GetChainmailArmorGraphics() );
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;
		CLASSEID_MYSTIC_ARCHER:		
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					fract_array.append( GetGraphicsCrafter() );
					break;
 		CLASSEID_PALADIN:
					fract_array.append( GetLeatherArmorGraphics() );
					fract_array.append( GetStuddedLeatherArmorGraphics() );
					fract_array.append( GetBoneArmorGraphics() );
					fract_array.append( GetRingmailArmorGraphics() );
					break;
		CLASSEID_RANGER:
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphics() );
					break;
		CLASSEID_THIEF:
					fract_array.append( GetBoneArmorGraphics() );
					fract_array.append( GetRingmailArmorGraphics() );
					fract_array.append( GetChainmailArmorGraphics() );
					fract_array.append( GetPlatemailArmorGraphics() );
					fract_array.append( GetShieldGraphicsThief() );
					break;

		CLASSEID_WARRIOR:
					fract_array.append( GetGraphicsCrafter() );
					break;

		CLASSEID_POWERPLAYER:
					fract_array.append( GetGraphicsCrafter() );
					break;
	endcase

	foreach type in fract_array
		foreach graphic in type
			restricted_types.append( graphic );
		endforeach
	endforeach

	return restricted_types;

endfunction


function GetClasseIdForSkill( who, skillid )

	// 2026-10-03: look at the classes the CHARACTER has, not at the first class that lists the skill.
	// The old lookup returned the first class in GetClasseIds() order whose list held the skill, so a
	// Warrior's Swordsmanship resolved to Bladesinger (level 0) and got no class skill-gain bonus,
	// failed-roll second chance or success bonus; only Healing and Wrestling counted. Of the classes
	// the character has a level in, the one with the highest level that lists the skill wins, and
	// 0 means none does. (This also covers the Thief/Powerplayer Throwing, Snooping and Detecting
	// Hidden special case that 3.1.1 handled by hand.)
	// 2026-10-04: Snooping and Detecting Hidden are free co-skills of the Thief now (not in its class list),
	// so the loop below cannot find them for a Thief. The committed lookup resolved them to the Thief and my
	// 2026-10-03 rewrite dropped that; restored so a Thief keeps the class bonus on both.
	if( skillid == SKILLID_DETECTINGHIDDEN or skillid == SKILLID_SNOOPING )
		if( CInt( GetObjProperty( who, CLASSEID_THIEF ) ) )
			return CLASSEID_THIEF;
		endif
	endif

	var best := 0;
	var bestlevel := 0;

	foreach id in GetClasseIds()
		var level := CInt( GetObjProperty( who, id ) );
		if( level > bestlevel )
			if( skillid in GetClasseSkills( id ) )
				// 2026-10-04: a Powerplayer keeps the old rule (decided): the class skill-gain bonus and
				// second roll only on Forensics, Throwing, Snooping and Detecting Hidden; its own x1.1 to
				// x1.3 at levels 3 to 5 in skillpoints.inc is its bonus on the rest.
				var allowed := 1;
				if( id == CLASSEID_POWERPLAYER )
					if( skillid != SKILLID_FORENSICS and skillid != SKILLID_THROWING and skillid != SKILLID_SNOOPING and skillid != SKILLID_DETECTINGHIDDEN )
						allowed := 0;
					endif
				endif
				if( allowed )
					best := id;
					bestlevel := level;
				endif
			endif
		endif
	endforeach

	return best;

endfunction


function GetClasseIds()

	return { 
			CLASSEID_BARD,
			CLASSEID_BLADESINGER,
			CLASSEID_CRAFTER,
			CLASSEID_MAGE,
			CLASSEID_MYSTIC_ARCHER,
			CLASSEID_PALADIN,
			CLASSEID_RANGER,
			CLASSEID_THIEF,
			CLASSEID_WARRIOR,
			CLASSEID_POWERPLAYER
			};

endfunction


function GetClasseName( classeid )

        case( classeid )

                CLASSEID_BARD:			return( "Bard" );
				CLASSEID_BLADESINGER:	return( "Bladesinger" );
                CLASSEID_CRAFTER:		return( "Crafter" );
                CLASSEID_MAGE:			return( "Mage" );
				CLASSEID_MYSTIC_ARCHER:	return( "Mystic Archer" );
				CLASSEID_PALADIN:		return( "Paladin" );
                CLASSEID_RANGER:		return( "Ranger" );
                CLASSEID_THIEF:			return( "Thief" );
                CLASSEID_WARRIOR:		return( "Warrior" );
				CLASSEID_POWERPLAYER:	return( "Powerplayer" );
                default:		return( "INVALID" );
        endcase

endfunction


function GetClasseSkills( classeid )

	case( classeid )

		CLASSEID_BARD:			
				return { SKILLID_BEGGING	,
						 SKILLID_CARTOGRAPHY	,
						 SKILLID_ENTICEMENT	,
						 SKILLID_HERDING	,
						 SKILLID_MUSICIANSHIP	,
						 SKILLID_PEACEMAKING	,
						 SKILLID_PROVOCATION	,
						 SKILLID_TASTEID	};

		CLASSEID_BLADESINGER:			
				return { SKILLID_ANATOMY	,
						 SKILLID_ENTICEMENT	,
						 SKILLID_FENCING	,
						 SKILLID_MUSICIANSHIP	,
						 SKILLID_PEACEMAKING	,
						 SKILLID_PROVOCATION	,
						 SKILLID_SWORDSMANSHIP	,
						 SKILLID_TACTICS	};

		CLASSEID_CRAFTER:
				return { SKILLID_ARMSLORE	,
						 SKILLID_BLACKSMITHY	,
						 SKILLID_BOWCRAFT	,
						 SKILLID_CARPENTRY	,
						 SKILLID_LUMBERJACKING	,
						 SKILLID_MINING		,
						 SKILLID_TAILORING	,
						 SKILLID_TINKERING	};

		CLASSEID_MAGE:			
				return { SKILLID_ALCHEMY	,
						 SKILLID_EVALINT	,
						 SKILLID_INSCRIPTION	,
						 SKILLID_ITEMID		,
						 SKILLID_MAGERY		,
						 SKILLID_MEDITATION	,
						 SKILLID_MAGICRESISTANCE,
						 SKILLID_SPIRITSPEAK	};

		CLASSEID_MYSTIC_ARCHER:			
				return { SKILLID_ARCHERY	,
						 SKILLID_COOKING	,
						 SKILLID_EVALINT	,
						 SKILLID_FISHING	,
						 SKILLID_TRACKING	,
						 SKILLID_MAGERY		,
						 SKILLID_MAGICRESISTANCE,
						 SKILLID_MEDITATION	};

		CLASSEID_PALADIN:
				return { SKILLID_SWORDSMANSHIP	,
						 SKILLID_PARRY	,
						 SKILLID_TACTICS	,
						 SKILLID_MAGERY	,
						 SKILLID_EVALINT		,
						 SKILLID_MEDITATION	,
						 SKILLID_MAGICRESISTANCE,
						 SKILLID_MACEFIGHTING	};

		CLASSEID_RANGER:
				return { SKILLID_ANIMALLORE	,
						 SKILLID_TAMING		,
						 SKILLID_ARCHERY	,
						 SKILLID_CAMPING	,
						 SKILLID_COOKING	,
						 SKILLID_FISHING	,
						 SKILLID_TRACKING	,
						 SKILLID_VETERINARY };

		CLASSEID_THIEF:
				return { SKILLID_HIDING		,
						 SKILLID_LOCKPICKING	,
						 SKILLID_POISONING	,
						 SKILLID_REMOVETRAP	,
						 SKILLID_STEALING	,
						 SKILLID_STEALTH	,
						 SKILLID_THROWING	,
						 SKILLID_TACTICS	};

		CLASSEID_WARRIOR:		
				return { SKILLID_ANATOMY	,
						 SKILLID_FENCING	,
						 SKILLID_HEALING	,
						 SKILLID_MACEFIGHTING	,
						 SKILLID_PARRY		,
						 SKILLID_SWORDSMANSHIP	,
						 SKILLID_TACTICS	,
						 SKILLID_WRESTLING	};

		CLASSEID_POWERPLAYER:	
				return { SKILLID_ANATOMY			, //Warrior
						 SKILLID_FENCING			,
						 SKILLID_HEALING			,
						 SKILLID_MACEFIGHTING		,
						 SKILLID_PARRY				,
						 SKILLID_SWORDSMANSHIP		,
						 SKILLID_TACTICS			,
						 SKILLID_WRESTLING			,
						 SKILLID_DETECTINGHIDDEN	, //Thief
						 SKILLID_HIDING				,
						 SKILLID_LOCKPICKING		,
						 SKILLID_POISONING			,
						 SKILLID_REMOVETRAP			,
						 SKILLID_SNOOPING			,
						 SKILLID_STEALING			,
						 SKILLID_STEALTH			,
						 SKILLID_THROWING			,
						 SKILLID_ANIMALLORE			, //Ranger
						 SKILLID_TAMING				,
						 SKILLID_ARCHERY			,
						 SKILLID_CAMPING			,
						 SKILLID_COOKING			,
						 SKILLID_FISHING			,
						 SKILLID_TRACKING			,
						 SKILLID_VETERINARY 		,
						 SKILLID_ALCHEMY			, //Mage
						 SKILLID_EVALINT			,
						 SKILLID_INSCRIPTION		,
						 SKILLID_ITEMID				,
						 SKILLID_MAGERY				,
						 SKILLID_MEDITATION			,
						 SKILLID_MAGICRESISTANCE 	,
						 SKILLID_SPIRITSPEAK	 	,
						 SKILLID_ARMSLORE			, //Crafter
						 SKILLID_BLACKSMITHY		,
						 SKILLID_BOWCRAFT			,
						 SKILLID_CARPENTRY			,
						 SKILLID_LUMBERJACKING		,
						 SKILLID_MINING				,
						 SKILLID_TAILORING			,
						 SKILLID_TINKERING			,
						 SKILLID_BEGGING			, //Bard
						 SKILLID_CARTOGRAPHY		,
						 SKILLID_ENTICEMENT			,
						 SKILLID_HERDING			,
						 SKILLID_MUSICIANSHIP		,
						 SKILLID_PEACEMAKING		,
						 SKILLID_PROVOCATION		,
						 SKILLID_TASTEID 			,
						 SKILLID_FORENSICS			}; //Misc.

		default:		return {};
	endcase

endfunction


function GetGeneralSkills()

	return { SKILLID_BEGGING,
		 SKILLID_ARMSLORE,
		 SKILLID_MAGICRESISTANCE,
		 SKILLID_ARCHERY,
		 SKILLID_HIDING,
		 SKILLID_TACTICS};

endfunction


function GetMaxCircle( who )

	foreach classeid in GetClasseIds()
		if( GetObjProperty( who , classeid ) )
			return GetMaxCircleForClasse( classeid );
		endif
	endforeach

	return 0;

endfunction


function GetMaxCircleForClasse( classeid )

	case( classeid )
		CLASSEID_BARD:			return 1;
		CLASSEID_BLADESINGER:	return 1;
		CLASSEID_CRAFTER:		return 7;
		CLASSEID_MAGE:			return 0;
		CLASSEID_MYSTIC_ARCHER:	return 0;
		CLASSEID_PALADIN:		return 0;
		CLASSEID_RANGER:		return 5;
		CLASSEID_THIEF:			return 5;
		CLASSEID_WARRIOR:		return 1;
		CLASSEID_POWERPLAYER:	return 0;
		default:		return 0;
	endcase

endfunction


function GetSkillPointsMultiplier( who, skillid )

	return ClasseBonusByLevel( IsSpecialisedIn( who, skillid ) );

endfunction


function GetStatPointsMultiplier( who , stat )

	var level   := HaveStatAffinity( who , stat );
	var mult    := 1.0;

	if( level )
		mult  := ClasseBonusByLevel( level );
	else
		level := HaveStatDifficulty( who , stat );
		if( level )
			mult := CDbl( 1.0 / ClasseBonusByLevel( level ) );
		endif
	endif

	return mult;

endfunction


function HaveInvalidSkillEnchantmentForClasse( item , classeid )

	var ret	:= 0;
	var skillnum	:= GetObjProperty( item , "skilladv" );

	// 2026-10-03: the General-skills exemption never worked (and/or precedence: for any non-zero
	// skill the old condition was always true), so it is commented out rather than fixed. A skill
	// enchant outside the class skills is refused, as it effectively was.
	//if( (skillnum) or (skillnum == 0) and !(skillnum in GetGeneralSkills()) )
	if( (skillnum) or (skillnum == 0) )
		if( !(skillnum in GetClasseSkills( classeid )) )
			ret := 1;
		endif
	endif

	if( classeid == CLASSEID_RANGER && skillnum == SKILLID_FORENSICS )
		ret := 0;
	endif

	if( classeid == CLASSEID_THIEF && skillnum == SKILLID_DETECTINGHIDDEN )
		ret := 0;
	endif

	if( classeid == CLASSEID_THIEF && skillnum == SKILLID_SNOOPING )
		ret := 0;
	endif

	if( classeid == CLASSEID_CRAFTER && skillnum == SKILLID_MAGERY )
		ret := 0;
	endif

	if( classeid == CLASSEID_CRAFTER && skillnum == SKILLID_MUSICIANSHIP )
		ret := 0;
	endif

	return ret;

endfunction


function HaveRestrictedModEnchantmentForClasse( item , classeid )

	var ret := 0;

	case( classeid )
		CLASSEID_BARD:			break;
		CLASSEID_BLADESINGER:	break;
		CLASSEID_CRAFTER:		break;
		CLASSEID_MAGE:			break;
		CLASSEID_MYSTIC_ARCHER:	break;
		CLASSEID_PALADIN:		break;
		CLASSEID_RANGER:		break;
		CLASSEID_THIEF:			break;
		CLASSEID_WARRIOR:		break;
		CLASSEID_POWERPLAYER:	break;
	endcase

	//Warrior original restrictions
	/*if( GetObjProperty( item , "MagicImmunity" ) )
		ret := 1;
	elseif( GetObjProperty( item , "SpellReflect" ) )
		ret := 1;
	elseif( GetObjProperty( item , "PermMagicImmunity" ) )
		ret := 1;
	elseif( GetObjProperty( item , "Permmr" ) )
		ret := 1;
	endif
	break;
	*/

	return ret;

endfunction


// 2026-10-07: stat affinity reviewed per class and switched on for ALL classes (it was
// Warrior/Crafter only). Each class now has exactly one faster stat: Str for Crafter and
// Warrior, Dex for Bard, Bladesinger, Thief and Ranger, Int for Mage, Paladin and Mystic
// Archer. Bard's old Int affinity was dropped so no class gains two stats faster.
function HaveStatAffinity( who , stat )

	var classeids;

	case( stat )
		"str":	classeids := {  
					CLASSEID_CRAFTER,
					CLASSEID_WARRIOR };	
					break;

		"dex":	classeids := {  
					CLASSEID_BARD,
					CLASSEID_BLADESINGER,
					CLASSEID_THIEF,
					CLASSEID_RANGER };	
					break;

		"int":	classeids := {  
					CLASSEID_MAGE,
					CLASSEID_PALADIN,
					CLASSEID_MYSTIC_ARCHER };	
					break;

		default:	classeids := {};
	endcase

	foreach id in classeids
		var level := CInt( GetObjProperty( who , id ) );

		if( level )
			return level;
		endif
	endforeach

	return 0;

endfunction


function HaveStatDifficulty( who , stat )

	var classeids, level;

	case( stat )
		"str":
			classeids := {
				CLASSEID_BARD,
				CLASSEID_BLADESINGER,
				CLASSEID_MAGE,
				CLASSEID_MYSTIC_ARCHER,
				CLASSEID_THIEF,
				CLASSEID_RANGER };
				break;

		"dex":
			classeids := {
				CLASSEID_MAGE,
				CLASSEID_PALADIN };
				break;

		"int":
			classeids := {
				CLASSEID_CRAFTER,
				CLASSEID_WARRIOR,
				CLASSEID_THIEF,
				CLASSEID_RANGER,
				CLASSEID_BLADESINGER };
				break;

		default:	classeids := {};
	endcase

	foreach id in classeids
		level := CInt( GetObjProperty( who , id ) );
		if( level )
			return level;
		endif
	endforeach

	return 0;

endfunction


function IsSpecialisedIn( who, skillid )

	var classeid := GetClasseIdForSkill( who, skillid );
	if( classeid )
		return CInt( GetObjProperty( who, classeid ) );
	endif

	return 0; // 2026-10-03: explicit, so "no class for this skill" can never read as a truthy default

endfunction


function IsBard( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_BARD ) );
endfunction

function IsBladesinger( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_BLADESINGER ) );
endfunction

function IsCrafter( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_CRAFTER ), 0, 1, 1 );
endfunction

function IsMage( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_MAGE ) );
endfunction

function IsMysticArcher( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_MYSTIC_ARCHER ) );
endfunction

function IsPaladin( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_PALADIN ) );
endfunction

function IsRanger( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_RANGER ), 1, 0, 0 );
endfunction

function IsThief( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_THIEF ), 0, 0, 0, 1, 1 );
endfunction

function IsWarrior( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_WARRIOR ) );
endfunction

function IsPowerplayer( who )
	return IsFromThatClasse( who , GetClasseSkills( CLASSEID_POWERPLAYER ) );
endfunction


function IsFromThatClasse( who , classe_skills , forensicsallowed := 0, mageryallowed := 0, musicianshipallowed := 0, detectinghiddenallowed := 0, snoopingallowed := 0 )

	var classe	:= 0;
	var total	:= 0;
	var number := Len( classe_skills );
	var amount := 0;

	for i := 0 to SKILLID__HIGHEST
		amount := GetEffectiveSkill( who , i );

		//These skills are free coskills for certain classes and don't dilute their spec calculation
		if( !( (forensicsallowed and i == SKILLID_FORENSICS)
			or (mageryallowed and i == SKILLID_MAGERY)
			or (musicianshipallowed and i == SKILLID_MUSICIANSHIP)
			or (detectinghiddenallowed and i == SKILLID_DETECTINGHIDDEN)
			or (snoopingallowed and i == SKILLID_SNOOPING)
		 	) )
			total := total + amount;
		endif

		if( i in classe_skills )
			classe := classe + amount;
		endif
	endfor

	// 2026-10-04: was a hard-coded 3675 (75 x 49 skills, before Throwing made it 50). The all-skills
	// class (Powerplayer, the only list with more than 20 skills) now follows the length of its list:
	// level 1 = 75 x number, then 15 x number more per level (3750, 4500, 5250, 6000, 6750, 7500).
	if( number > 20 and classe >= AVERAGE_SKILL * number )
		var level     := 1;
		var average_t := CInt( (AVERAGE_SKILL + AVERAGE_LEVEL_MOD) * number );

		while( ( classe >= average_t ) and ( level < 6 ) )
			level     := level + 1;
			average_t := CInt( average_t + AVERAGE_LEVEL_MOD * number );
		endwhile
		unequipRestrictedItems(who);
		return level;
	elseif ( classe < AVERAGE_SKILL * number )	
		return 0;
	elseif( classe < CInt(total * number * EACH_MUST_REPRESENT * 0.01) )
		return 0;
	else
		var level     := 1;
		var represent := EACH_MUST_REPRESENT + REPRESENT_LEVEL_MOD;
		var percent   := CInt( total * number * represent * 0.01 );
		var average_t := CInt( (AVERAGE_SKILL + AVERAGE_LEVEL_MOD) * number );

		while( (classe >= average_t) and (classe >= percent) )
			level     := level + 1;
			represent := CDbl( represent + REPRESENT_LEVEL_MOD );
			percent   := CInt( total * number * represent * 0.01 );
			average_t := CInt( average_t + AVERAGE_LEVEL_MOD * number );
		endwhile
		unequipRestrictedItems(who);
		return level;
	endif

endfunction


function IsProhibitedByClasse( who , item )

	var ret := 0;

	if( GetObjProperty( who , CLASSEID_BARD	) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_BARD ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_BARD ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_BARD ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_BLADESINGER	) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_BLADESINGER ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_BLADESINGER ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_BLADESINGER ) )
			ret := 1;
		elseif( GetObjProperty( item , "int" ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_CRAFTER ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_CRAFTER ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_CRAFTER ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_CRAFTER ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_MAGE ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_MAGE ) )
			ret := 1;
		elseif( GetObjProperty( item , "str" ) )
			ret := 1;
		elseif( GetObjProperty( item , "dex" ) )
			ret := 1;
		elseif( GetObjProperty( item , "ar" ) or GetObjProperty( item , "ArBonus" ) ) // 2026-10-03: ArBonus is the property enchanted armor really carries
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_MAGE ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_MAGE ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_MYSTIC_ARCHER ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_MYSTIC_ARCHER ) )
			ret := 1;
		elseif( GetObjProperty( item , "str" ) )
			ret := 1;
		elseif( GetObjProperty( item , "ar" ) or GetObjProperty( item , "ArBonus" ) ) // 2026-10-03: ArBonus is the property enchanted armor really carries
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_MYSTIC_ARCHER ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_MYSTIC_ARCHER ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_PALADIN ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_PALADIN ) )
			ret := 1;
		elseif( GetObjProperty( item , "dex" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_PALADIN ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_PALADIN ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_RANGER ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_RANGER ) )
			ret := 1;
		elseif( GetObjProperty( item , "str" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_RANGER ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_RANGER ) )	// 2026-10-05: every other class tests the objtype too; without it the Navar Bloody Barrier (objtype 0x982a, graphic 0x2B01) was never barred for Rangers
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_THIEF ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_THIEF ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_THIEF ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_THIEF ) )
			ret := 1;
		elseif( GetObjProperty( item , "int" ) )
			ret := 1;
		endif
	elseif( GetObjProperty( who , CLASSEID_WARRIOR ) )
		if( HaveInvalidSkillEnchantmentForClasse( item , CLASSEID_WARRIOR ) )
			ret := 1;
		elseif( GetObjProperty( item , "int" ) )
			ret := 1;
		elseif( item.graphic in EnumerateRestrictedItemTypesFromClasse( CLASSEID_WARRIOR ) )
			ret := 1;
		elseif( item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_WARRIOR ) )
			ret := 1;
		elseif( HaveRestrictedModEnchantmentForClasse( item , CLASSEID_WARRIOR ) )
			ret := 1;
		endif
	endif

	return ret;

endfunction


function AssignClasse( character )

	var t;

	foreach classe in GetClasseIds()
		EraseObjProperty( character , classe );
		if(classe == "IsRanger")
			t := IsFromThatClasse( character , GetClasseSkills(classe), 1, 0, 0, 0, 0 );
		elseif(classe == "IsCrafter")
			t := IsFromThatClasse( character , GetClasseSkills(classe), 0, 1, 1, 0, 0 );
		elseif(classe == "IsThief")
			t := IsFromThatClasse( character , GetClasseSkills(classe), 0, 0, 0, 1, 1 );
		else
			t := IsFromThatClasse( character , GetClasseSkills( classe ), 0, 0, 0, 0, 0 );
		endif
		if( t )
			SetObjProperty( character , classe , t );
			unequipRestrictedItems(character);
		endif
	endforeach

endfunction

// This function gets the weapons that certains classes can use. uses graphics
// It runs a seperate "canhit" script based on the players class level..
function ClassWeapons(classString)

	case (classString)
		// Thief no longer gated here as of the Throwing rework -- Thief deals
		// normal damage with any weapon now, same as Warrior/Ranger/Powerplayer.

		//Shepherds Crook, bow, heavy crossbow, crossbow, Yumi, Composite Bow, Repeating Crossbow, Lajatang
		"IsBard": return {0x0e81, 0x13B2, 0x13fd, 0x0f4f, 0x27a5, 0x26c2, 0x26c3, 0x27a7};

		//Black Staff, Gnarled Staff, QuarterStaff, Lajatang
		"IsMage": return {0x0df0, 0x13f9, 0x0e8a, 0x27a7};
		default:
	endcase

	return 0;

endfunction


function GetClass(who)

	foreach key in GetClasseIds()
		if (GetObjProperty(who, key))
			return key; // returns "Isranger" etc
		endif
	endforeach

	return 0; //no class found

endfunction


function GetClasseSkillTotal(who, aclass)

	var classtotal:=0;
	var skills:=GetClasseSkills(aclass);
	for i:= 0 to SKILLID__HIGHEST
		if (i in skills)
			classtotal:=classtotal+ CDbl(GetEffectiveSkillBaseValue(who, i)) / 10;
		endif
	endfor

return classtotal;

endfunction

// Returns class level (regardless of class)
function GetClasseLevel(who)

	var characterClass := GetClass(who);
	if (characterClass)
		return GetObjProperty(who, characterClass);
	else
		return 0;
	endif

endfunction


function unequipRestrictedItems(who)

	var IsRPer := GetObjProperty( who, "IsRPer" );

	if(!who.cmdlevel)
		if(!IsRPer)
			foreach item in ListEquippedItems( who )
				var isRperItem := GetObjProperty(item, "IsRPer");
				
				SleepMS(5);

				if(isRperItem)
					MoveItemToContainer( item, who.backpack);
					SendSysMessage(who, "You are carrying items intended for [RPer]'s. If we see you with an item equipped that you shouldn't have, you will be jailed without questions.", FONT_NORMAL, 2595 );
					// Broadcast to staff
					var msg := "Warning: " + who.name + " was carrying a RPer flagged item: " + item.name + " with serial: " + item.serial + " illegally.";
					foreach staff in EnumerateOnlineCharacters()
						if( staff.cmdlevel >= 1 )
							SendSysMessage( staff, msg, 3, 2581 );
						endif
					endforeach
				endif

				SleepMS(5);

				if(IsProhibitedByClasse( who , item ))
					MoveItemToContainer( item, who.backpack);
					SendSysMessage(who, "Illegal items for your class found equipped on you, if we see you with an item equipped that you shouldn't have, you will be jailed without questions.", FONT_NORMAL, 2595);
					LogCommand(who, GetProcess(GetPid()).name, 0, item.desc+"/"+item.serial);
				endif
			endforeach
		else
			foreach item in ListEquippedItems( who )

				SleepMS(5);

				if(IsProhibitedByClasse( who , item ))
					MoveItemToContainer( item, who.backpack);
					SendSysMessage(who, "Illegal items for your class found equipped on you, if we see you with an item equipped that you shouldn't have, you will be jailed without questions.", FONT_NORMAL, 2595);
				endif
			endforeach
		endif
	endif

	return;

endfunction
