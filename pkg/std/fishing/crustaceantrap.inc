use uo;
use os;
use cfgfile;

include "include/random";
include "include/skillpoints";
include "fishing";
include ":questpkg:questfishing";

const UOBJ_CRUSTACEANTRAP      := 0x3037E;
const UOBJ_CRUSTACEANTRAP_BUOY := 0x3037F;

const TRAP_LEASH_DISTANCE    := 10;    // tiles from owner before the buoy is destroyed
const TRAP_MAX_LIFETIME      := 900;   // 15 minutes - buoy destroyed if never retrieved
const TRAP_BOB_INTERVAL      := 60;    // seconds per OSI "bob"
const TRAP_MAX_CATCH         := 5;     // OSI's per-trap cap
const TRAP_CATCH_CHANCE      := 35;    // % chance of a catch on a surviving bob
const TRAP_RARE_DIVISOR      := 500;   // matches fishing.inc's production Rare odds
const TRAP_LEGENDARY_DIVISOR := 5000;  // matches fishing.inc's production Legendary odds
const TRAP_SWEEP_PERIOD      := 5;     // seconds between sweeper ticks
const TRAP_MAX_PER_PLAYER    := 5;     // max simultaneously active traps per player

const TRAP_REGISTRY := "#CrustaceanTrapBuoys";

const PROP_TRAP_OWNER     := "TrapOwner";
const PROP_TRAP_PLACED_AT := "TrapPlacedAt";
const PROP_TRAP_LAST_BOB  := "TrapLastBob";
const PROP_TRAP_BOB_COUNT := "TrapBobCount";
const PROP_TRAP_CATCHES   := "TrapCatches";
//Parallel array to PROP_TRAP_CATCHES (same index per catch) - 1 where the Big lure was still
//active on the buoy at the exact bob that produced this catch, 0 otherwise. Lure charges are
//spent every bob regardless of catch outcome (crustaceansweeper.src's RollBob), so by the
//time a later bob catches something the lure may already be gone - this tag is what lets
//CollectTrap apply the weight-skew only to catches that actually happened while it was live,
//instead of re-deciding off whatever's left on the buoy (or nothing, if it's since expired).
const PROP_TRAP_BIGBOOST  := "TrapBigBoost";

//Global registry of every buoy currently deployed, mirroring :donationbox:include/
//donationbox.inc's DonationBoxList()/Register()/Unregister() pattern - lets the sweeper
//walk only active buoys instead of scanning the whole world every tick. Not trusted to
//survive an unclean shutdown (see crustaceansweeper.src's boot-time world scan) - this is
//purely a live-session performance aid.
function TrapBuoyList()
	var list := GetGlobalProperty(TRAP_REGISTRY);
	if (TypeOf(list) != "Array")
		list := array{};
	endif
	return list;
endfunction

function TrapBuoyRegister(serial)
	var list := TrapBuoyList();
	foreach existing in list
		if (existing == serial)
			return list;
		endif
	endforeach
	list.append(serial);
	SetGlobalProperty(TRAP_REGISTRY, list);
	return list;
endfunction

function TrapBuoyUnregister(serial)
	var list := TrapBuoyList();
	var kept := array{};
	foreach existing in list
		if (existing != serial)
			kept.append(existing);
		endif
	endforeach
	SetGlobalProperty(TRAP_REGISTRY, kept);
	return kept;
endfunction

//Counts how many currently-registered buoys belong to a given owner - used to cap the
//number of traps a single player can have active at once. Skips entries that no longer
//resolve to a real item (already-gone buoys the sweeper hasn't pruned yet).
function CountOwnedTraps(owner_serial)
	var count := 0;
	foreach serial in TrapBuoyList()
		var buoy := SystemFindObjectBySerial(CInt(serial));
		if (buoy and CInt(GetObjProperty(buoy, PROP_TRAP_OWNER)) == owner_serial)
			count := count + 1;
		endif
	endforeach
	return count;
endfunction

//Same shape as fishing.inc's PickFishFromTier(), reading crustaceantable.cfg instead.
function PickCrustaceanFromTier(tier, location, realm, skill)

	var cfg := ReadConfigFile("crustaceantable");
	var keys := GetConfigStringKeys(cfg);
	var candidates := {};

	foreach key in keys
		var elem := cfg[key];
		if (elem.Tier == tier)
			if ( (!elem.Location or elem.Location == location) and
			     (!elem.Realm or elem.Realm == realm) and
			     (!elem.MinSkill or skill >= CInt(elem.MinSkill)) )
				candidates.append(elem);
			endif
		endif
	endforeach

	UnloadConfigFile("crustaceantable");

	if (!len(candidates))
		return 0;
	endif

	var chosen := candidates[Random(len(candidates))+1];
	var picked_objtype;

	if (chosen.ObjtypeEast)
		if (Random(2))
			picked_objtype := CInt(chosen.ObjtypeEast);
		else
			picked_objtype := CInt(chosen.ObjtypeSouth);
		endif
	else
		picked_objtype := CInt(chosen.Objtype);
	endif

	return picked_objtype;

endfunction

//Same roll order/odds as fishing.inc's PickCaughtFish() - Legendary first, then Rare, else
//Regular, using the same divisors as the pole/net's production values by default. Falls
//back a tier at a time if nothing's eligible at this location/realm/skill. Returns
//{objtype, tier}.
//
//rare_divisor/legendary_divisor default to the trap's normal constants - the caller
//(crustaceansweeper.src) pre-adjusts them when the deployed buoy has a charged Rare/
//Legendary lure, same pattern fishing.src uses for the pole.
function PickCaughtCrustacean(character, location, rare_divisor := TRAP_RARE_DIVISOR, legendary_divisor := TRAP_LEGENDARY_DIVISOR)

	var skill := GetEffectiveSkill(character, SKILLID_FISHING);
	var realm := character.realm;

	var tier := "Regular";
	if (Random(legendary_divisor) == 0)
		tier := "Legendary";
	elseif (Random(rare_divisor) == 0)
		tier := "Rare";
	endif

	var caught := PickCrustaceanFromTier(tier, location, realm, skill);

	if (!caught and tier == "Legendary")
		tier := "Rare";
		caught := PickCrustaceanFromTier(tier, location, realm, skill);
	endif

	if (!caught and tier != "Regular")
		tier := "Regular";
		caught := PickCrustaceanFromTier(tier, location, realm, skill);
	endif

	return {caught, tier};

endfunction

//Hands the accumulated catches + the physical trap back to whoever is retrieving it, then
//tears down the buoy. Named/colored messages for Rare (yellow)/Legendary (orange) mirror
//fishing.inc's ResolveFishCatch() exactly.
function CollectTrap(character, buoy)

	var catches := GetObjProperty(buoy, PROP_TRAP_CATCHES);
	if (TypeOf(catches) != "Array")
		catches := array{};
	endif

	//Whether each catch gets the Big lure's weight-skew was already decided (and its charge
	//already spent) at the bob that caught it - see crustaceansweeper.src's RollBob - not
	//re-decided here, so a trap that's lost before collection can't leave the lure unspent.
	//Same index as catches; missing/wrong-length just means no boost (buoys deployed before
	//this property existed).
	var bigboosts := GetObjProperty(buoy, PROP_TRAP_BIGBOOST);
	if (TypeOf(bigboosts) != "Array")
		bigboosts := array{};
	endif

	var i;
	for (i := 1; i <= len(catches); i := i + 1)
		var objtype := catches[i];
		var big_boost := (i <= len(bigboosts) and CInt(bigboosts[i]) == 1);

		var caughtitem := CreateItemInBackpack(character, CInt(objtype), 1);
		var fishtier := GetObjProperty(caughtitem, "FishTier");
		var tier := "Regular";
		var weight := 0;

		if (fishtier == "Rare Crustacean")
			tier := "Rare";
			weight := RollTrophyWeight();
			if (big_boost)
				weight := Max(weight, RollTrophyWeight());
			endif
			SendSysmessage(character, "You find a rare "+caughtitem.desc+"!", 3, 1731);
			SetObjProperty(caughtitem, "TrophyWeight", weight);
		elseif (fishtier == "Legendary Crustacean")
			tier := "Legendary";
			weight := RollTrophyWeight();
			if (big_boost)
				weight := Max(weight, RollTrophyWeight());
			endif
			SendSysmessage(character, "You find a legendary "+caughtitem.desc+"!", 3, 1531);
			SetObjProperty(caughtitem, "TrophyWeight", weight);
		endif

		Print("[Fishing] "+character.name+" collected trap catch tier="+tier+" weight="+CStr(weight)+" big_boost="+CStr(big_boost));
		QP_ProcessFishCatch(character, "CatchCrustacean", tier, weight);
	endfor

	CreateItemInBackpack(character, UOBJ_CRUSTACEANTRAP, 1);

	if (len(catches))
		SendSysmessage(character, "You haul in the trap and collect "+len(catches)+" crustacean(s).");
	else
		SendSysmessage(character, "You haul in the trap, but it's empty.");
	endif

	TrapBuoyUnregister(buoy.serial);
	DestroyItem(buoy);

endfunction

//Used by the sweeper when a buoy is lost/expired instead of retrieved - no items are handed
//over, matching OSI's "lose the trap" outcome. Message is best-effort (skipped if the owner
//isn't resolvable, e.g. deleted account).
function DestroyTrap(buoy, owner, message)
	if (owner and message)
		SendSysmessage(owner, message);
	endif
	TrapBuoyUnregister(buoy.serial);
	DestroyItem(buoy);
endfunction
