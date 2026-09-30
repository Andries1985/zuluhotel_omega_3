// wfhvitals.inc -- Warrior for Hire vital ceilings.
//
// 2026-09-30: NPC max HP/Mana/Stam come from the template's HITS/MANA/STAM fields now, not from
// Str/Int/Dex, so a warrior's ceilings have to be re-stamped whenever its stats change (hire,
// hourly growth, High Priest restore). Rule: HP = 2 x base Str; Mana = base Int; Stam = base Dex.

use uo;

include "include/attributes";
include ":attributes:npcvitals";

const WFH_HITS_PER_STR := 2;

function SetWfhVitalCeilings( wfh )

	var basestr := GetStrength( wfh ) - GetStrengthMod( wfh );
	var baseint := GetIntelligence( wfh ) - GetIntelligenceMod( wfh );
	var basedex := GetDexterity( wfh ) - GetDexterityMod( wfh );

	SetNpcVitalOverride( wfh, "HITS", basestr * WFH_HITS_PER_STR );
	SetNpcVitalOverride( wfh, "MANA", baseint );
	SetNpcVitalOverride( wfh, "STAM", basedex );

endfunction

// Ceilings plus a full top-up: used where the warrior is (re)built from scratch.
function SetWfhVitalCeilingsAndFill( wfh )

	SetWfhVitalCeilings( wfh );
	SetHp( wfh, GetMaxHp( wfh ) );
	SetMana( wfh, GetMaxMana( wfh ) );
	SetStamina( wfh, GetMaxStamina( wfh ) );

endfunction
