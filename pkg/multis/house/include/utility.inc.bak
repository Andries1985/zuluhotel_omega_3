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

//-------------------------------------------------- New Housing Functions ----------------------------------------------------------------\\

// Returns 1 if it's the owner of co-owner.
function IsCowner( who, house )

	if (GetObjProperty(house, "ownerserial") == who.serial)
		return 1;
	endif
	
	var friends := GetObjProperty(house, "Friends");
	
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

	var friends := GetObjProperty(house, "Friends");
	
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