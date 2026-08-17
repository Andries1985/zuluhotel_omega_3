//
// helper functions for static housing
//
//
// A lot of these are good candidates to be moved to the sign method script
//
use cfgfile;
include ":statichousing:include/settings";

function STHIncDebug(msg)
	if(STH_IsDebugEnabled())
		Print("[STATIC-HOUSING-DEBUG][include/staticHousing] " + msg);
	endif
endfunction

function IsLocationWithinStaticHouseSign(sign, x, y, z)
	if(!sign)
		return 0;
	endif

	var footage := GetObjProperty(sign, "footage");
	if(!footage)
		return 0;
	endif

	foreach thing in footage
		if((x >= thing[1] && x <= thing[4]) &&
		   (y >= thing[2] && y <= thing[5]) &&
		   (z >= thing[3] && z <= thing[6]))
			return 1;
		endif
	endforeach

	return 0;
endfunction


///////////////////
// Tries to find a house sign that covers the given location
// Note: Makes NO check to determine ownership, co-ownership or fiend status
// of the house in question.
// Returns objref to the sign.
///////////////////
function FindStaticHouseForLocation (x, y, z, map)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, map)
		case (item.objtype)
			0x0BD1:
			0x0BCF:
			0x0BD0:
			0x0BD2:
			0x20BD1:
			0x20BCF:
			0x20BD0:
			0x20BD2:
			0x260FA:
				if (IsLocationWithinStaticHouseSign (item, x, y, z))
					STHIncDebug("FindStaticHouseForLocation hit sign=" + item.serial + " @" + x + "," + y + "," + z + " realm=" + map);
					return item;
				endif
		endcase
	endforeach
	STHIncDebug("FindStaticHouseForLocation no sign @" + x + "," + y + "," + z + " realm=" + map);
	return 0;
endfunction

/////////////////////////////////////////////////////////////////////////////
// footage { nwx, nwy, lowerz, sex, sey, upperz }
///////////////////
///////////////////////////////////////////////////////////////////
// IsInside
// Tests to see if the item is inside and if not alerts 'owner'
//
///////////////////////////////////////////////////////////////////

function IsItemInsideHouse(byref owner, sign, item)
	sign := FindStaticHouseForLocation(item.x, item.y, item.z, item.realm);
	if(!sign)
		STHIncDebug("IsItemInsideHouse failed: no sign for item=" + item.serial + " @" + item.x + "," + item.y + "," + item.z + " realm=" + item.realm);
		SendSysMessage(owner, "That is not inside your home.", 0x3, 0x21);
		return 0;
	endif
	footage := GetObjProperty (sign, "footage");
	var chk := 0;
	foreach thing in footage
		if ( (item.realm == sign.realm) &&
			(item.x >= thing[1] && item.x <= thing[4]) &&
			(item.y >= thing[2] && item.y <= thing[5]) &&
			(item.z >= thing[3] && item.z <= thing[6]) )
			chk := 1;
			break;
		endif
		sleepms(5);
		if(chk)
			break;
		endif
		sleepms(5);
	endforeach
	if(chk == 0)
		STHIncDebug("IsItemInsideHouse failed: item=" + item.serial + " outside sign=" + sign.serial);
		SendSysMessage(owner, "That is not inside your home.", 0x3, 0x21);
	else
		STHIncDebug("IsItemInsideHouse success: item=" + item.serial + " inside sign=" + sign.serial);
	endif
	return chk;
endfunction



