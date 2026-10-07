//Edited August 25, 2024

use uo;
use os;
use npc;
use cfgfile;

include "include/attributes";
include "include/random";
include ":attributes:regen";
include ":attributes:npcvitals";	// 2026-09-30: HITSREGEN / MANAREGEN / STAMREGEN template fields

var modnpc := Self();

// 2026-09-30: the BaseStrmod/BaseIntmod/BaseDexmod CProps were folded into each template's
// STR/INT/DEX (and RunSpeed pinned to the old DEX), so there is no stat mod to apply here any
// more. The engine fills HP/Mana/Stam to max itself when an NPC spawns (npc.cpp).


// 2026-09-30: template regen comes from the HITSREGEN / MANAREGEN / STAMREGEN fields (points per
// minute, dice allowed; the old BaseHpRegen/BaseManaRegen CProps were converted x12/100 in the
// npcdesc reformat). GetNpcVitalSetting resolves the field x100 = the engine's hundredths per
// minute, caches it per NPC, and honours a tool override (SetNpcVitalOverride) first. -1 = no
// field: the engine default applies and any stale "template" entry is cleared.
ApplyTemplateRegen( modnpc, "HITSREGEN", VITALID_LIFE );
ApplyTemplateRegen( modnpc, "MANAREGEN", VITALID_MANA );
ApplyTemplateRegen( modnpc, "STAMREGEN", VITALID_STAMINA );

function ApplyTemplateRegen( who, field, vitalid )

	var setting := GetNpcVitalSetting( who, field );
	if( setting >= 0 )
		SetVitalRegenRateMod( who, vitalid, "template", setting - CORE_SETTINGS_DEFAULT_REGEN_RATE );
	else
		ClearVitalRegenRateMod( who, vitalid, "template" );
	endif

endfunction

if(1)
	var equip_cfg := ReadConfigFile(":*:equip");
	if( equip_cfg )
	var npccfg := readconfigfile("npcdesc");
		var equip_elem := FindConfigelem(equip_cfg, npccfg[modnpc.npctemplate].equip);
		if( equip_elem )
			EquipFromTemplate(modnpc, npccfg[modnpc.npctemplate].equip);
		endif
	endif
endif


if( !GetObjProperty( modnpc , "Karma" ) )
	var npc_cfg	:= ReadConfigFile( "::npcdesc" );
	var stat_tot	:= GetStrength(modnpc) + GetDexterity(modnpc) + GetIntelligence(modnpc);
	var ar	:= modnpc.ar + 3;
	var base	:= Cint(stat_tot * ar / 12);
	if( !base )
		base	:= 1;
	endif
	
	var multiply	:= 1;
	case( modnpc.script )
		"spellkillpcs":	
		"firebreather":	multiply := 1.2;
				break;	
		default:
	endcase
	
	base := Cint(base * multiply);
		
	var bonus := 0;
	foreach item in ListEquippedItems( modnpc )
		if( item.attribute )
			case( item.hitscript )
				":combat:mainhit":			break;
				":combat:banishscript":		bonus := 250;	break;
				":combat:poisonhit":			bonus := 100 + (GetObjProperty( item , "Poisonlvl" ) * 150);	break;
				":combat:piercingscript":		bonus := 500;
				":combat:voidscript":			bonus := 1000;
				":combat:lifedrainscript":	bonus := 250;
				":combat:manadrainscript":	bonus := 200;
				":combat:spellstrikescript":	bonus := 100 + (GetObjProperty( item , "EffectCircle" ) * 50);	break;
				default:
			endcase
		endif
	endforeach

	// 2026-09-30: the regen bonus (raw BaseHpRegen/BaseManaRegen CProp values, which put 46
	// templates straight at the 15,000 cap) is out of the formula -- npcdesc reformat decision R3.
	// Re-tune the affected templates when karma/fame is revisited.

	var possible_prots := { "PermPoisonImmunity",
			 	  "PermMagicImmunity",
			 	  "Permmr",
			 	  "FireProtection",
			 	  "WaterProtection",
			 	  "AirProtection",
			 	  "EarthProtection",
			 	  "FreeAction",
			 	  "NecroProtection",
			 	  "HolyProtection",
			 	  "PhysicalProtection",
			 	  "HealingIncrease" };
			 	  
	foreach prot in possible_prots
		if( GetObjProperty( modnpc , prot ) )
			bonus := bonus + (GetObjProperty( modnpc , prot ) * 100);
		endif
	endforeach
	
	base := base + bonus;
	if( base > 15000 )
		base := 15000;
	endif
	
	var karma := Cint( base * (Random(21)+90) / 100 );
	var famodnpc  := Cint( base * (Random(21)+90) / 100 * 0.875 );

	// 2026-09-30: was ".alignmodnpcnt" (a mangled "alignment") -- it never matched, so every NPC
	// fell through to the default (negative karma) branch, "alignment good" templates included.
	case( npc_cfg[modnpc.npctemplate].alignment )
		"evil":		karma := 0 - karma;	break;
		"good":		break;
		
		default:	karma := 0 - karma;	break;
	endcase
	
	SetObjProperty( modnpc , "Karma" , karma );
	SetObjProperty( modnpc , "Fame"  , famodnpc  );
endif


if( !GetObjProperty( modnpc , "AiSerial" ) or GetObjProperty( modnpc , "AiSerial" ) != modnpc.serial )
	SetObjProperty( modnpc , "AiSerial" , modnpc.serial );
endif


if( GetProperty( "Anchor" ) )
	// 2026-09-23: the live spawner (:spawnpoint:checkpoint) stores this as a six-slot array
	// {x, y, range, psub, z, realm}; only the treasure-map guardians use the named-field struct.
	// Reading array slots as struct fields gave 0s, and SetAnchor with a range of 0 switches the
	// anchor off, so spawned NPCs were never leashed to their point.
	var anchor := GetProperty( "Anchor" );
	if( anchor.x )
		SetAnchor( cint(anchor.x), cint(anchor.y), cint(anchor.range), cint(anchor.psub) );
	else
		SetAnchor( cint(anchor[1]), cint(anchor[2]), cint(anchor[3]), cint(anchor[4]) );
	endif
endif