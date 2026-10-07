// housetravel.inc -- "is this spot inside somebody's house, and may this character travel there?"
// for Recall, Gate, Mark, Teleport, Earth Portal and the runebook (2026-10-02).
//
// Why: the travel scripts used GetStandingHeight( x, y, runeZ ).multi. The engine answers that
// with the lowest surface AT OR ABOVE the rune's z, while MoveObjectToLocation drops the caster
// onto the nearest surface BELOW it. A rune marked in an old house at a height above the floor
// of a house placed there later therefore found "no house" and landed its holder inside
// (confirmed in game 2026-10-01: small tower rune z 26 -> stranger's small house floor z 7).
// The same test also saw nothing on a castle courtyard (no house piece under the tile) and in
// a static house (not a multi at all). The footprint test below ignores z.

use uo;
use os;

include "include/objtype";
include "../pkg/multis/house/include/utility";	// HOUSE_SIGN_OBJTYPES, GetHouseSignFromObject, IsCowner, IsFriend

// A sign can sit a whole house-width from the far corner of its footprint (castles are 31 x 32).
const HOUSETRAVEL_SIGN_RANGE := 34;

// The house covering a spot, or 0. Returns the house SIGN when a footprint matched, otherwise
// the multi; IsCowner / IsFriend accept either.
//   1. a house sign whose "footage" box covers x,y -- every housing package, static houses
//      included; z is ignored on purpose. A sign with no owner (a static house for sale)
//      protects nothing.
//   2. the engine's own test at this height, as the scripts always did -- keeps steps, porches
//      and boats behaving exactly as before.
//   3. a house that has no sign, or whose sign carries no footage: any house multi with a
//      piece on this tile, at any height.
// scan_signs 0 skips step 1 (used for NPC Teleport casts, which fire often).
function FindHouseAtSpot( x, y, z, realm, scan_signs := 1 )

	if( scan_signs )
		var nearby := ListItemsNearLocation( x, y, LIST_IGNORE_Z, HOUSETRAVEL_SIGN_RANGE, realm );
		foreach thing in nearby
			if( thing.objtype in HOUSE_SIGN_OBJTYPES )
				if( CInt( GetObjProperty( thing, "ownerserial" ) ) )
					var footage := GetObjProperty( thing, "footage" );
					if( footage )
						foreach box in footage
							if( x >= box[1] && x <= box[4] && y >= box[2] && y <= box[5] )
								return thing;
							endif
						endforeach
					endif
				endif
			endif
		endforeach
	endif

	var standing_multi := GetStandingHeight( x, y, z, realm ).multi;
	if( standing_multi )
		return standing_multi;
	endif

	var on_tile := ListMultisInBox( x, y, -128, x, y, 127, realm );
	foreach piece_owner in on_tile
		if( piece_owner.isa( POLCLASS_HOUSE ) )
			var its_sign := GetHouseSignFromObject( piece_owner );
			if( !its_sign )
				return piece_owner;
			endif
			// a sign that carries no footprint cannot answer step 1: the piece on the tile decides
			if( !GetObjProperty( its_sign, "footage" ) )
				return piece_owner;
			endif
		endif
	endforeach

	return 0;

endfunction

// 1 when "who" may recall / gate / mark at a spot covered by "house" (0 = no house there):
// owner, co-owner, or the house's Teleporter permission (friend or guild rank). A banned
// character is refused by HasHousePermission. priv is RECALL_TO / GATE_TO / GATE_FROM.
function CanTravelIntoHouse( who, house, priv )

	if( !house )
		return 1;
	endif

	if( IsCowner( who, house ) )
		return 1;
	endif

	if( IsFriend( who, house, priv ) )
		return 1;
	endif

	return 0;

endfunction

// Removes every entry of a runebook / runic atlas that points at this exact spot, and clears
// the book's default location when it is that spot. Returns how many entries were removed.
function RemoveRunebookEntryAt( book, x, y, z, realm )

	set_critical(1);

	var removed := 0;
	var defs := GetObjProperty( book, "RuneDefs" );
	if( defs )
		var i;
		for( i := defs.size(); i >= 1; i := i - 1 )
			var entry := defs[i];
			if( CInt( entry.tox ) == x && CInt( entry.toy ) == y && CInt( entry.toz ) == z )
				if( entry.torealm == realm )
					defs.erase( i );
					removed := removed + 1;
				endif
			endif
		endfor
		if( removed )
			SetObjProperty( book, "RuneDefs", defs );
		endif
	endif

	// the book's own x/y/z/realm are its default location -- what a spell cast on the book uses
	if( CInt( GetObjProperty( book, "x" ) ) == x && CInt( GetObjProperty( book, "y" ) ) == y && CInt( GetObjProperty( book, "z" ) ) == z )
		if( GetObjProperty( book, "realm" ) == realm )
			EraseObjProperty( book, "x" );
			EraseObjProperty( book, "y" );
			EraseObjProperty( book, "z" );
			EraseObjProperty( book, "realm" );
			removed := removed + 1;
		endif
	endif

	if( removed )
		IncRevision( book );
	endif

	set_critical(0);
	return removed;

endfunction

// A travel spell was refused because the destination is inside a house the caster may not
// enter (decision 2026-10-02: the rune is destroyed, as forbidden areas already do). A loose
// rune is destroyed; a runebook or atlas loses that entry. A boat is not a house: nothing is
// destroyed for it.
function DestroyRefusedRune( caster, cast_on, house, tox, toy, toz, torealm )

	if( house.isa( POLCLASS_BOAT ) )
		return 0;
	endif

	if( cast_on.objtype == UOBJ_RUNE )
		DestroyItem( cast_on );
		SendSysMessage( caster, "The rune crumbles." );
		return 1;
	endif

	if( cast_on.objtype == 0x7100 || cast_on.objtype == UOBJ_RUNIC_ATLAS_EW || cast_on.objtype == UOBJ_RUNIC_ATLAS_NS )
		if( RemoveRunebookEntryAt( cast_on, tox, toy, toz, torealm ) )
			SendSysMessage( caster, "That location has been removed from the book." );
			return 1;
		endif
	endif

	return 0;

endfunction
