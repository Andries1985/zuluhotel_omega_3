/*
        Donation box shared constants / helpers.
*/
use uo;
use os;

const DONATION_OBJTYPE      := 0xf703;
const DONATED_AT_PROP       := "donatedat";   // ReadGameClock() seconds when donated
const DONATED_BY_PROP       := "donatedby";   // donor serial (informational)
const DONATION_TTL_SECONDS  := 1209600;       // 14 days (2 weeks)
const DONATION_SWEEP_PERIOD := 3600;          // 1 hour
const DONATION_REGISTRY     := "#DonationBoxes";
const DONATION_SWEEP_DISABLED_PROP := "#DonationSweepDisabled";
const DONATION_DEBUG        := 1;             // 1 = print debug to console, 0 = silent

function DonationDebug(msg)
	if (DONATION_DEBUG)
		Print("[donationbox] " + msg);
	endif
	return 1;
endfunction

function DonationBoxList()
	var list := GetGlobalProperty(DONATION_REGISTRY);
	if (TypeOf(list) != "Array")
		list := array{};
	endif
	return list;
endfunction

function DonationBoxRegister(serial)
	var list := DonationBoxList();
	foreach existing in list
		if (existing == serial)
			return list;
		endif
	endforeach
	list.append(serial);
	SetGlobalProperty(DONATION_REGISTRY, list);
	return list;
endfunction

function DonationBoxUnregister(serial)
	var list := DonationBoxList();
	var kept := array{};
	foreach existing in list
		if (existing != serial)
			kept.append(existing);
		endif
	endforeach
	SetGlobalProperty(DONATION_REGISTRY, kept);
	return kept;
endfunction

function DonationSweepEnabled()
	return !GetGlobalProperty(DONATION_SWEEP_DISABLED_PROP);
endfunction

function SetDonationSweepEnabled(enabled)
	if (enabled)
		EraseGlobalProperty(DONATION_SWEEP_DISABLED_PROP);
	else
		SetGlobalProperty(DONATION_SWEEP_DISABLED_PROP, 1);
	endif
	return DonationSweepEnabled();
endfunction
