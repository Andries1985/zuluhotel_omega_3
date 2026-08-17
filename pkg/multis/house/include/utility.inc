use uo;
use os;

//include "setup";

const VIEW_SECURE := 2;
const ADD_TO_SECURE := 3;
const REMOVE_FROM_SECURE := 4;
const RECALL_TO := 5;
const GATE_TO := 6;
const GATE_FROM := 7;
const CO_OWNER := 8;

// House sign objtypes: the classic (:house:) sign graphics, plus custom
// housing's (:customhousing:) 0xFFF4 -- customhousing's signs carry the same
// "signserial"/"houseserial" properties (see customhousedeed.src) and share
// this package's :house:multiSign/method MethodScript, so they resolve the
// same way as any other house sign here.
var HOUSE_SIGN_OBJTYPES := {0xBCF, 0xBD1, 0xBD0, 0xBD2, 0x20BD1, 0x20BD2, 0x20BCF, 0x20BD0, 0x260FA, 0xFFF4};

function GetHouseSignFromObject(byref house_or_sign)
	if(!house_or_sign)
		return 0;
	endif

	if(house_or_sign.objtype in HOUSE_SIGN_OBJTYPES)
		return house_or_sign;
	endif

	var signserial := GetObjProperty(house_or_sign, "signserial");
	if(signserial && !signserial.errortext)
		var sign := SystemFindObjectBySerial(CInt(signserial));
		if(sign)
			return sign;
		endif
	endif

	foreach nearby in ListItemsNearLocation(house_or_sign.x, house_or_sign.y, house_or_sign.z, 18, house_or_sign.realm)
		if(nearby.objtype in HOUSE_SIGN_OBJTYPES)
			if(CInt(GetObjProperty(nearby, "houseserial")) == CInt(house_or_sign.serial))
				return nearby;
			endif
		endif
	endforeach

	return 0;
endfunction

//-------------------------------------------------- New Housing Functions ----------------------------------------------------------------\\

function GetCurrentHouseAtMobile(who)

	if(!who)
		return 0;
	endif

	var house := who.multi;
	if(house)
		return house;
	endif

	var standing_house := GetStandingHeight(who.x, who.y, who.z, who.realm).multi;
	if(standing_house)
		return standing_house;
	endif

	return 0;

endfunction

// Returns 1 if it's the owner of co-owner.
function IsCowner( who, house )
	if(!house)
		return 0;
	endif

	var sign := GetHouseSignFromObject(house);
	if(sign)
		if(sign.IsCoOwnerOrOwner(who))
			return 1;
		endif
	endif

	if (GetObjProperty(house, "ownerserial") == who.serial)
		return 1;
	endif
	
	var friends := GetObjProperty(house, "Friends");
	if(!friends || friends.errortext)
		return 0;
	endif
	
	foreach friend in friends
		if (friend[1] == who.serial)
			if (friend[CO_OWNER] == 1) // [8] is co-owner tag
				return 1;
			endif
		endif
	endforeach
	
	return 0;
	
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IsFriend() - Can pass a priv to check
// 
// Priv List:	2=View secures	3=Add to secures	4=Take from secures	5=Recall into house
//						6=Gate to house	7=Gate from house	8=Co-owner
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
function IsFriend( who, house, priv := 0 )
	if(!house)
		return 0;
	endif

	var sign := GetHouseSignFromObject(house);
	if(sign)
		if(priv == CO_OWNER)
			return sign.IsCoOwnerOrOwner(who);
		elseif(priv == VIEW_SECURE || priv == ADD_TO_SECURE || priv == REMOVE_FROM_SECURE)
			return sign.HasHousePermission(who, "secure");
		elseif(priv == RECALL_TO || priv == GATE_TO || priv == GATE_FROM)
			return sign.HasHousePermission(who, "teleporter");
		elseif(priv == 0)
			return sign.IsAFriend(who);
		else
			return sign.IsAFriend(who);
		endif
	endif

	var friends := GetObjProperty(house, "Friends");
	if(!friends || friends.errortext)
		return 0;
	endif
	
	foreach friend in friends
		if (friend[1] == who.serial) 		// He's a friend
			if (priv != 0) 								// a priv was passed, check it equals 1
				if (friend[priv] == 1)
					return 1;									// Priv is 1, accept
				else
					return 0;									// Priv is 0, deny
				endif
			endif
			return 1; 										// No more searching for a priv, return 1
		endif
	endforeach
	
	return 0;													// DENY!!
	
endfunction