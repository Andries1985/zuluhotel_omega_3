// regen.inc — stacking-safe regen-rate modifier API, ported from ModernDistro's
// pkg/systems/attributes/include/regen.inc pattern (AP_GetVitalRegenRateMod /
// AP_SetVitalRegenRateMod), adapted to this shard's VITALID_*/CProp conventions.
//
// Replaces the old flat "last SetXRegenRate() call wins" model: instead of one
// scalar CProp that every caller overwrites, each vital gets a dictionary of
// independently-named contributions (poison, item bonuses, NPC template
// bonuses...) that sum together to produce the actual regen rate. Two effects
// can now coexist without one clobbering the other's contribution.

include "include/attributes";   // VITALID_LIFE/MANA/STAMINA, RecalcVitalsIfOnline

const CPROP_NAME_PREFIX_REGEN_RATE_MODS := "RegenRateModsFor";

function GetVitalRegenRateModDict( who, vitalid )
	var dict := GetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid);
	if (!dict)
		return dictionary;
	endif
	return dict;
endfunction

function GetVitalRegenRateMod( who, vitalid )      // sum of every named entry
	var dict := GetVitalRegenRateModDict(who, vitalid);
	var total := 0;
	foreach modname in (dict.keys())
		total := total + Cint(dict[modname]);
	endforeach
	return total;
endfunction

// Clamps a final, already-summed regen rate to the engine's own accepted
// range -- a single huge template contribution (e.g. BaseHpRegen 60000 on an
// npcdesc.cfg template, *12 = 720000) would otherwise be handed to the
// engine unclamped; the old SetHpRegenRate/etc. this replaced always clamped
// before storing, so this preserves that guarantee at the one point all
// three vitals' regen rate is finally computed.
function ClampRegenRate( rate )
	if( rate < CORE_SETTINGS_MIN_REGEN_RATE )
		return CORE_SETTINGS_MIN_REGEN_RATE;
	elseif( rate > CORE_SETTINGS_MAX_REGEN_RATE )
		return CORE_SETTINGS_MAX_REGEN_RATE;
	endif
	return rate;
endfunction

function SetVitalRegenRateMod( who, vitalid, modname, amount )
	var dict := GetVitalRegenRateModDict(who, vitalid);
	dict.insert(modname, Cint(amount));
	SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid, dict);
	return RecalcVitalsIfOnline(who);
endfunction

function ClearVitalRegenRateMod( who, vitalid, modname )
	var dict := GetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid);
	if (!dict || !dict.exists(modname))
		return 0;
	endif
	dict.erase(modname);
	if (dict.size())
		SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid, dict);
	else
		EraseObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid);
	endif
	return RecalcVitalsIfOnline(who);
endfunction

function ClearAllVitalRegenRateMods( who, vitalid )
	EraseObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE_MODS+vitalid);
	return RecalcVitalsIfOnline(who);
endfunction
