use uo;

include "include/objtype";
include "include/spelldata";
include "include/areas";
include "include/client";
//include ":housing:utility";
include "../pkg/multis/house/include/utility";

function Housecheck(character)
	var house := GetCurrentHouseAtMobile(character);
	if(!house)
		return 0;
	endif

	var ownerserial := GetObjProperty(house, "ownerserial");
	if(ownerserial && !ownerserial.errortext && CInt(ownerserial) == character.serial)
		return 1;
	endif

	var owneracct := GetObjProperty(house, "owneracct");
	if(owneracct && !owneracct.errortext && owneracct == character.acctname)
		return 1;
	endif

	if(IsCowner(character, house))
		return 1;
	endif

	if(IsFriend(character, house))
		return 1;
	endif

	// Newer housing stores affiliations on sign-side friend/co-owner lists.
	var signserial := GetObjProperty(house, "signserial");
	if(signserial && !signserial.errortext)
		var sign := SystemFindObjectBySerial(CInt(signserial));
		if(sign)
			if(sign.IsFullHouseManager(character) || sign.IsAFriend(character))
				return 1;
			endif
		endif
	endif

	return 0;

endfunction  