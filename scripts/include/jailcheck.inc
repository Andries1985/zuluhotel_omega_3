use uo;

include "include/constants/locations";

// 2026-09-23: the jail moved to britannia_alt (DEFAULT_LOCATION_JAIL_* in locations.inc);
// this used to test the old-map box 5272-5310/1160-1190 on britannia, which no jailed
// player has stood in since. One box around the constants, 40 tiles each way, shared
// with the skill-gain block in skillpoints.inc.
const JAIL_BOX_RADIUS := 40;

function Jailcheck(who)

	if( who.realm != DEFAULT_LOCATION_JAIL_R )
		return 0;
	endif
	if( who.x < DEFAULT_LOCATION_JAIL_X - JAIL_BOX_RADIUS or who.x > DEFAULT_LOCATION_JAIL_X + JAIL_BOX_RADIUS )
		return 0;
	endif
	if( who.y < DEFAULT_LOCATION_JAIL_Y - JAIL_BOX_RADIUS or who.y > DEFAULT_LOCATION_JAIL_Y + JAIL_BOX_RADIUS )
		return 0;
	endif

	return 1;
endfunction  