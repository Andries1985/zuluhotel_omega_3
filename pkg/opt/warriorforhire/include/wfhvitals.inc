// wfhvitals.inc -- Warrior for Hire vitals.
//
// 2026-10-10: a warrior's vitals work like a player's: max HP = Strength, max Mana = Intelligence,
// max Stamina = Dexterity, buffs included. A Bless or the Staff of Nagash raises the stat and the
// vital together and both drop back when the buff ends. The rule lives in the vital hooks
// (pkg/systems/attributes/hooks/vitalInit.src, UsesPlayerVitals), which skip the template HITS /
// MANA / STAM lookup for the warriorforhire template, so nothing here has to keep it in step.
// Was (2026-09-30): fixed ceilings stamped through SetNpcVitalOverride, HP = 2 x base Str, mods
// excluded, re-stamped on hire, growth and every restore.

use uo;

include "include/attributes";
include ":attributes:npcvitals";

// Hire, hourly growth and High Priest restores call this after setting stats. It drops any
// fixed ceiling an older warrior still carries from the 2026-09-30 rule and recalculates.
function SetWfhVitalCeilings( wfh )

	ClearNpcVitalOverride( wfh, "HITS" );
	ClearNpcVitalOverride( wfh, "MANA" );
	ClearNpcVitalOverride( wfh, "STAM" );
	RecalcVitals( wfh );

endfunction

// Ceilings plus a full top-up: used where the warrior is (re)built from scratch.
function SetWfhVitalCeilingsAndFill( wfh )

	SetWfhVitalCeilings( wfh );
	SetHp( wfh, GetMaxHp( wfh ) );
	SetMana( wfh, GetMaxMana( wfh ) );
	SetStamina( wfh, GetMaxStamina( wfh ) );

endfunction
