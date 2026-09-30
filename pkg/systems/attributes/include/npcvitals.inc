// npcvitals.inc -- NPC template HITS/MANA/STAM lookup plus the per-instance override API.
//
// 2026-09-30: replaces the retired CustomHitsLevel/CustomManaLevel/CustomStaminaLevel CProps.
// Every NpcTemplate in config/npcdesc.cfg now carries explicit HITS/MANA/STAM fields (plain
// numbers or dice strings). The value resolved for an instance lives x100 (engine fixed-point)
// in the "RolledNpcVitals" dictionary CProp, keyed "HITS"/"MANA"/"STAM". That dictionary is both
// the dice-roll cache and the override slot: whatever is in it wins over the template field, so a
// tool that wants to raise a boss's ceiling just writes the entry (SetNpcVitalOverride).
// 2026-09-30: the same lookup serves the HITSREGEN / MANAREGEN / STAMREGEN fields (points per
// minute, decimals allowed): x100 is exactly the engine's hundredths-per-minute regen unit, and
// modsetup.inc applies it as the "template" regen-rate mod.

use uo;
use cfgfile;
use util;          // RandomDiceRoll
use vitals;        // RecalcVitals

include "include/attributes";

const NPCVITALS_CACHE_CPROP := "RolledNpcVitals";

var npcvitals_cfg := ReadConfigFile( "::npcdesc" );

// x100 value for this instance: cached/override entry first, else the template field (a dice
// string is rolled once and cached so it never drifts). Returns -1 when the template has no such
// field -- distinct from an explicit 0.
function GetNpcVitalSetting( who, vitalfield )

	var cache := GetObjProperty( who, NPCVITALS_CACHE_CPROP );
	if( !cache )
		cache := dictionary;
	endif

	if( cache.exists( vitalfield ) )
		return Cint( cache[vitalfield] );
	endif

	var raw := GetConfigString( npcvitals_cfg[who.npctemplate], vitalfield );
	if( !raw )
		return -1;
	endif

	var value;
	if( Find( raw, "d", 1 ) )
		value := RandomDiceRoll( raw );
		if( value.errortext )
			value := 0;
		endif
		value := Cint( value ) * 100;
	else
		value := Cint( CDbl( raw ) * 100 );	// 2026-09-30: decimals allowed (HITSREGEN 2.5 -> 250)
	endif

	cache.insert( vitalfield, value );
	SetObjProperty( who, NPCVITALS_CACHE_CPROP, cache );

	return value;

endfunction

// Per-instance ceiling in real points (300 = 300 HP). Recalcs the vital; callers fill the
// current value themselves (SetHp/SetMana/SetStamina) if they want it topped up.
function SetNpcVitalOverride( who, vitalfield, points )

	var cache := GetObjProperty( who, NPCVITALS_CACHE_CPROP );
	if( !cache )
		cache := dictionary;
	endif

	cache.insert( vitalfield, Cint( CDbl( points ) * 100 ) );	// 2026-09-30: decimals allowed
	SetObjProperty( who, NPCVITALS_CACHE_CPROP, cache );

	return RecalcVitals( who );

endfunction

// The instance's resolved value in real points, or -1 if nothing has been resolved yet.
function GetNpcVitalOverride( who, vitalfield )

	var cache := GetObjProperty( who, NPCVITALS_CACHE_CPROP );
	if( cache and cache.exists( vitalfield ) )
		return Cint( cache[vitalfield] ) / 100;
	endif

	return -1;

endfunction

// Drops the instance entry so the next calc re-reads (and re-rolls) the template field.
function ClearNpcVitalOverride( who, vitalfield )

	var cache := GetObjProperty( who, NPCVITALS_CACHE_CPROP );
	if( !cache or !cache.exists( vitalfield ) )
		return 0;
	endif

	cache.erase( vitalfield );
	if( cache.size() )
		SetObjProperty( who, NPCVITALS_CACHE_CPROP, cache );
	else
		EraseObjProperty( who, NPCVITALS_CACHE_CPROP );
	endif

	return RecalcVitals( who );

endfunction

// Raw template field as written in npcdesc.cfg ("300", "1d20+180"), or "" if absent.
function GetNpcVitalTemplateValue( who, vitalfield )

	var raw := GetConfigString( npcvitals_cfg[who.npctemplate], vitalfield );
	if( !raw )
		return "";
	endif

	return raw;

endfunction
