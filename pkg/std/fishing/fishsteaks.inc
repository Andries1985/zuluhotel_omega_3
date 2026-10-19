// 2026-10-19: one steak rule shared by catch-time auto-carve (fishing.inc:createfish)
// and the knife (bladed.src:CarveFish). Location is the catchtable/itemdesc bucket
// ("Shores" | "Deep Water" | "Dungeons"); anything else (unset, quest fish) is 1.

function GetSteaksPerFish(location)

	if (location == "Dungeons")
		return 6;
	elseif (location == "Deep Water")
		return 4;
	endif

	return 1;

endfunction

// Rare 2x, Legendary 10x. Accepts the catch-tier word ("Rare") or the itemdesc
// FishTier text ("Rare Fish") so either source works.
function GetFishTierSteakMultiplier(tier)

	tier := CStr(tier);
	if (tier["Legendary"])
		return 10;
	elseif (tier["Rare"])
		return 2;
	endif

	return 1;

endfunction
