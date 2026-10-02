// traveldebug.inc -- TEMPORARY console tracing (2026-10-01) for the report that an old rune
// recalls its holder into a house placed later on the same spot. It prints what the travel
// scripts see at a rune's destination and where the caster ends up.
//
// Callers: pkg/std/spells/mark.src, recall.src, gate.src and pkg/std/runebook/customspells.inc
// (CustomRecall, CustomGate). Set TRAVEL_DEBUG to 0 to silence it; delete this file and the
// "TEMP debug" lines in those four files once the cause is confirmed.

use uo;

const TRAVEL_DEBUG := 1;

function TravelDebugRef( obj )

	if( !obj )
		return "none";
	endif

	return Hex( obj.serial ) + " (objtype " + Hex( obj.objtype ) + ", base z " + obj.z + ")";

endfunction

function TravelDebugStand( st )

	if( !st )
		return "ERROR \"" + st.errortext + "\" = the script sees NO house";
	endif

	return "standing z " + st.z + ", house " + TravelDebugRef( st.multi );

endfunction

// The rune is being marked: where, and what the engine calls the house under the caster.
function TravelDebugMark( caster )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	Print( "[traveldbg] MARK by " + caster.name + " at " + caster.x + "," + caster.y + "," + caster.z + " " + caster.realm
		+ " | caster.multi " + TravelDebugRef( caster.multi ) );
	return 1;

endfunction

// Everything about the destination, before the script decides.
function TravelDebugDest( tag, caster, tox, toy, toz, torealm )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	Print( "[traveldbg] " + tag + " by " + caster.name + " (" + Hex( caster.serial ) + ", cmdlevel " + caster.cmdlevel + ") from "
		+ caster.x + "," + caster.y + "," + caster.z + " " + caster.realm + " to rune " + tox + "," + toy + "," + toz + " " + torealm );

	// what the script's own test returns (lowest standable surface at or above the rune's z)
	Print( "[traveldbg]   script's test, GetStandingHeight at rune z " + toz + ": " + TravelDebugStand( GetStandingHeight( tox, toy, toz, torealm ) ) );
	// the same lookup started 40 below the rune: what is really standing in that column
	Print( "[traveldbg]   same test from z " + (toz - 40) + ": " + TravelDebugStand( GetStandingHeight( tox, toy, toz - 40, torealm ) ) );

	// multis that have any component on this exact tile, at any height
	var here := ListMultisInBox( tox, toy, -128, tox, toy, 127, torealm );
	if( here.size() )
		foreach house in here
			Print( "[traveldbg]   multi with a piece on this tile: " + TravelDebugRef( house ) + " at " + house.x + "," + house.y );
		endforeach
	else
		Print( "[traveldbg]   no multi has a piece on this tile" );
	endif

	// house signs whose footage (the footprint lockdowns use) covers this x,y
	var sign_types := { 0xBCF, 0xBD1, 0xBD0, 0xBD2, 0x20BD1, 0x20BD2, 0x20BCF, 0x20BD0, 0x260FA, 0xFFF4 };
	var nearby := ListItemsNearLocation( tox, toy, LIST_IGNORE_Z, 40, torealm );
	var covering := 0;
	var other_signs := 0;
	foreach thing in nearby
		if( thing.objtype in sign_types )
			var footage := GetObjProperty( thing, "footage" );
			var hit := 0;
			if( footage )
				foreach box in footage
					if( tox >= box[1] && tox <= box[4] && toy >= box[2] && toy <= box[5] )
						hit := 1;
						var z_inside := 0;
						if( toz >= CInt( box[3] ) - 12 && toz <= box[6] )
							z_inside := 1;
						endif
						Print( "[traveldbg]   sign " + Hex( thing.serial ) + " at " + thing.x + "," + thing.y + "," + thing.z
							+ " owner " + Hex( CInt( GetObjProperty( thing, "ownerserial" ) ) ) + " house " + Hex( CInt( GetObjProperty( thing, "houseserial" ) ) )
							+ ": footage x " + box[1] + ".." + box[4] + " y " + box[2] + ".." + box[5] + " z " + (CInt( box[3] ) - 12) + ".." + box[6]
							+ " covers the rune's x,y; rune z inside that z range: " + z_inside );
					endif
				endforeach
			endif
			if( hit )
				covering := covering + 1;
			else
				other_signs := other_signs + 1;
			endif
		endif
	endforeach
	Print( "[traveldbg]   house signs covering this x,y: " + covering + " (other signs within 40 tiles: " + other_signs + ")" );

	return 1;

endfunction

// The script found a house: what the owner / friend tests answered.
function TravelDebugPerm( tag, caster, house, is_cowner, is_friend )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	Print( "[traveldbg] " + tag + " house found " + TravelDebugRef( house ) + ": Iscowner " + is_cowner + ", IsFriend(teleporter) " + is_friend
		+ " for " + caster.name );
	return 1;

endfunction

// 2026-10-02: what the NEW footprint test (FindHouseAtSpot / CanTravelIntoHouse) decided.
function TravelDebugHouse( tag, caster, house, allowed )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	Print( "[traveldbg] " + tag + " NEW house test: " + TravelDebugRef( house ) + " | " + caster.name + " allowed: " + allowed );
	return 1;

endfunction

// After the move: did it work and where is the caster now.
function TravelDebugLanded( tag, caster, moved )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	var result := "ok";
	if( !moved )
		result := "FAILED \"" + moved.errortext + "\"";
	endif
	Print( "[traveldbg] " + tag + " move " + result + " | caster now at " + caster.x + "," + caster.y + "," + caster.z + " " + caster.realm
		+ " | caster.multi " + TravelDebugRef( caster.multi ) );
	return 1;

endfunction

// Where the destination gate really sits (the rune asked for z toz).
function TravelDebugGate( tag, gate, toz )

	if( !TRAVEL_DEBUG )
		return 0;
	endif

	Print( "[traveldbg] " + tag + " destination gate at " + gate.x + "," + gate.y + "," + gate.z + " " + gate.realm + " (rune z " + toz
		+ ") | gate.multi " + TravelDebugRef( gate.multi ) );
	return 1;

endfunction
