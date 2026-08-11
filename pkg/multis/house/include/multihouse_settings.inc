use cfgfile;
use os;
use uo;

UnloadConfigFile( ":house:settings" );
var	housing_cfg := ReadConfigFile(":house:settings");
if(!housing_cfg)
	Broadcast("No housing settings file found.");
endif

function get_abandon_time()

var	elem := FindConfigElem( housing_cfg, "Settings" );
var	decayperiod := GetConfigInt( elem, "Abandoned_Timeout" );

return decayperiod;

endfunction




///////////////////
//  maximum number of houses (of any package) a single account may own at once
///////////////////

function get_max_houses_per_account()
	var elem := FindConfigElem( housing_cfg, "Settings" );
	var maxhouses := GetConfigInt( elem, "MaxHousesPerAccount" );
	if (!maxhouses)
		maxhouses := 2;
	endif
	return maxhouses;
endfunction




///////////////////
//  registers a house sign against its owning account's global house-count
//  list. Shared across house, staticHousing, and customhousing so the account
//  cap is enforced the same way no matter which package the house belongs to.
///////////////////

function RegisterAccountHouseSign(sign, acctname)
	if (!acctname or !sign)
		return;
	endif
	acctname := Lower(acctname);
	var propname := "#housing_of_" + acctname;
	var existing := GetGlobalProperty(propname);
	if (!existing)
		existing := array{};
	endif
	if (!(sign.serial in existing))
		existing.append(sign.serial);
		SetGlobalProperty(propname, existing);
	endif
endfunction




///////////////////
//  removes a house sign from its owning account's global house-count list
///////////////////

function UnregisterAccountHouseSign(sign, acctname)
	if (!acctname or !sign)
		return;
	endif
	acctname := Lower(acctname);
	var propname := "#housing_of_" + acctname;
	var existing := GetGlobalProperty(propname);
	if (!existing)
		return;
	endif
	var idx := sign.serial in existing;
	if (idx)
		existing.erase(idx);
		SetGlobalProperty(propname, existing);
	endif
endfunction




///////////////////
//  counts how many houses (of any package) an account currently owns,
//  self-healing the tracking list against destroyed signs or stale entries
//  left behind by a transfer that didn't own its unregister step.
///////////////////

function GetAccountHouseCount(acctname)
	if (!acctname)
		return 0;
	endif
	acctname := Lower(acctname);
	var propname := "#housing_of_" + acctname;
	var existing := GetGlobalProperty(propname);
	if (!existing or !len(existing))
		return 0;
	endif

	var live := array{};
	foreach signserial in existing
		var candidate := SystemFindObjectBySerial(signserial);
		if (candidate)
			// 2026-08-10: also accept the legacy "account" cprop (customhousing
			// only) as a fallback -- a custom house registered under the old
			// system has "account" but not "owneracct" until its owner opens
			// the sign at least once (MigrateCustomHouseSignOwnership). Without
			// this fallback, the first house-count check for that account
			// permanently drops a still-valid, still-owned house from the
			// registry, undercounting toward MaxHousesPerAccount.
			var candidate_acct := GetObjProperty(candidate, "owneracct");
			if (!candidate_acct)
				candidate_acct := GetObjProperty(candidate, "account");
			endif
			if (Lower(candidate_acct) == acctname)
				live.append(signserial);
			endif
		endif
	endforeach

	if (len(live) != len(existing))
		SetGlobalProperty(propname, live);
	endif

	return len(live);
endfunction




///////////////////
//  determines if an account can register one more house without exceeding
//  the configured MaxHousesPerAccount cap
///////////////////

function IsAccountUnderHouseCap(acctname)
	return GetAccountHouseCount(acctname) < get_max_houses_per_account();
endfunction

