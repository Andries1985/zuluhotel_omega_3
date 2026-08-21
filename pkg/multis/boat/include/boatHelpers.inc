use uo;
use boat;

include ":keys:key";

/*
	These helpers exist because config/boats.cfg (stock polserver engine data,
	not something this package customizes) only assigns a Tillerman+Hold to
	the classic small/medium/long/dragon boats (MultiID 0x0-0x17). Row Boat
	gets a Tiller instead, and every galleon/Britannia hull only gets a Wheel
	- none of those newer families ever get a Tillerman, Tiller, or Hold.

	GetBoatController/GetBoatHolds let the rest of the package treat every
	ship family the same way instead of assuming boat.tillerman/boat.hold
	always exist.

	Expects the real underlying multi object - i.e. the "boat" parameter as
	received inside multi/listener.src, or multi.multi when called from
	boat/use.src right after Multi_MakeMulti().
*/

// A missing extobj role (e.g. boat.tiller on a ship with no Tiller component)
// does not reliably come back as 0 or an error value - observed in testing to
// come back as a live-looking item reference with objtype 0 and an
// uninitialized serial. Treat objtype 0 as invalid too, not just falsy/error.
function IsValidBoatItem( item )
	if( !item || item.errortext )
		return 0;
	endif
	if( CInt( item.objtype ) == 0 )
		return 0;
	endif
	return 1;
endfunction

// Whichever item currently acts as this boat's steering/ownership control point.
//
// .tillerman/.hold/.portplank/.starboardplank are real, engine-exposed dot
// properties - the four roles the classic 0x0-0x17 boats have always had as
// hardcoded C++ fields. Every newer role (Tiller, Wheel, Hull, Rope,
// Storage, ...) is NOT exposed as a dot property at all - boat.tiller and
// boat.wheel return a garbage placeholder object regardless of whether that
// component actually exists on the multi (confirmed in testing: a Wheel
// item that was verifiably present and clickable still came back invalid
// via boat.wheel). Those have to be found by walking boat.components
// directly and matching on objtype, same as GetBoatHolds already does for
// Storage.
function GetBoatController( boat )

	var ctrl := boat.tillerman;
	if( IsValidBoatItem( ctrl ) )
		return ctrl;
	endif

	// This is called every tick from the listener's main loop - avoid
	// re-scanning boat.components on every call by caching the resolved
	// item's serial on the boat itself once found.
	var cached_serial := CInt( GetObjProperty( boat, "ControllerSerial" ) );
	if( cached_serial )
		ctrl := SystemFindObjectBySerial( cached_serial );
		if( IsValidBoatItem( ctrl ) )
			return ctrl;
		endif
	endif

	foreach item in ( boat.components )
		if( item.objtype == 0x1F017 || item.objtype == 0x1F015 ) // Tiller (Row Boat) or Wheel (galleons/Britannia)
			SetObjProperty( boat, "ControllerSerial", item.serial );
			return item;
		endif
		SleepMS(2);
	endforeach

	return 0;
endfunction

// A specific, deliberately-chosen deck offset (dx,dy from the controller) for
// ship types where the generic nearby-wheel search lands somewhere ugly
// (e.g. the default graphic looked wrong sitting right next to the wheel/
// mast). Marked in-game with a placement blocker and read back from the
// save file. Returns 0 for any ship type without an override, meaning "use
// the generic nearby-wheel search instead."
function GetPreferredHoldOffset( ship_type )
	if( ship_type == 0x16058 ) // Orc Ship - near the stern/wheel, 3 tiles east.
		return Struct{ "dx":=3, "dy":=0 };
	elseif( ship_type == 0x16059 ) // Gargoyle Ship - near the stern/wheel, 4 tiles east.
		return Struct{ "dx":=4, "dy":=0 };
	elseif( ship_type == 0x1605A ) // Tokuno Ship - right at the wheel's own tile.
		return Struct{ "dx":=0, "dy":=0 };
	endif

	return 0;
endfunction

// Picks a deck tile to place the escript-created custom hold on, near the
// controller (wheel/tiller - always a real clickable deck spot in x/y)
// rather than boat.x/boat.y (the multi's anchor tile, which is not
// necessarily open deck - confirmed in testing: placing directly at
// boat.x/boat.y/boat.z+1 landed the hold half-inside the mast and
// half-underground). Avoids any tile already occupied by another component.
//
// Z is boat.z (the multi's own root/waterline), not any elevated deck
// height - confirmed in testing that this graphic (0x5C2D) is drawn with a
// height offset baked into the sprite itself and pokes up to the correct
// deck level on its own, the same way native boat components do (their
// items also sit at boat.z server-side despite rendering up on deck).
function FindHoldSpot( boat, controller )

	var deck_z := boat.z;

	var preferred := GetPreferredHoldOffset( controller.GetShipType() );
	if( preferred )
		return Struct{ "x":=controller.x + preferred.dx, "y":=controller.y + preferred.dy, "z":=deck_z };
	endif

	var occupied := array{};
	foreach item in ( boat.components )
		occupied.append( CStr(item.x) + "," + CStr(item.y) );
		SleepMS(1);
	endforeach

	var offsets := array{
		Struct{"dx":=0,"dy":=2}, Struct{"dx":=0,"dy":=-2},
		Struct{"dx":=2,"dy":=0}, Struct{"dx":=-2,"dy":=0},
		Struct{"dx":=1,"dy":=1}, Struct{"dx":=-1,"dy":=1},
		Struct{"dx":=1,"dy":=-1}, Struct{"dx":=-1,"dy":=-1}
	};

	foreach off in ( offsets )
		var x := controller.x + off.dx;
		var y := controller.y + off.dy;
		var key := CStr(x) + "," + CStr(y);

		var is_occupied := 0;
		foreach spot in ( occupied )
			if( spot == key )
				is_occupied := 1;
				break;
			endif
			SleepMS(1);
		endforeach

		if( !is_occupied )
			return Struct{ "x":=x, "y":=y, "z":=deck_z };
		endif
		SleepMS(1);
	endforeach

	// Every nearby offset was occupied - fall back to right on top of the
	// controller's own tile.
	return Struct{ "x":=controller.x, "y":=controller.y, "z":=deck_z };
endfunction

// The boat's own LockID, read off one of its ropes rather than stored
// anywhere centrally - ropes are always present and always get theirs set at
// placement time (boat/use.src), regardless of ship family or how old the
// boat is. Used to lock a custom hold that's being created outside of
// initial placement (i.e. a replacement for one that went missing some other
// way than DestroyCustomBoatHold) to the same key as the rest of the boat,
// instead of leaving it unlocked.
function GetBoatLockId( boat )
	foreach item in ( boat.components )
		if( item.objtype == 0x1F014 )
			var lock_id := GetObjProperty( item, "LockID" );
			if( lock_id )
				return lock_id;
			endif
		endif
		SleepMS(1);
	endforeach

	return 0;
endfunction

// Every cargo container this boat has, regardless of ship family.
// Classic boats: the single native Hold.
// Britannia: every native Storage crate (there are several per ship).
// Row Boat: none, by design.
// Orc/Gargoyle/Tokuno: neither role exists in config/boats.cfg, so a single
// container is created once and tracked via RegisterItemWithBoat so it
// travels with the ship; its serial is saved on the boat so it survives restarts.
function GetBoatHolds( boat )

	var holds := array{};

	var native_hold := boat.hold;
	if( IsValidBoatItem( native_hold ) )
		holds.append( native_hold );
		return holds;
	endif

	foreach item in ( boat.components )
		if( item.objtype == 0x1F01A )
			// itemdesc.cfg's default Graphic for this objtype is 1 (an
			// essentially invisible placeholder) - the engine creates native
			// Storage components straight from that with no override, same
			// as it would for the escript-created custom hold if we didn't
			// set one. Match the custom hold's graphic here too.
			item.graphic := 0x5C2D;
			holds.append( item );
		endif
		SleepMS(2);
	endforeach

	if( holds.Size() > 0 )
		return holds;
	endif

	var controller := GetBoatController( boat );

	// Row Boat is the one ship family that's meant to have no cargo hold at
	// all - it only gets a Tiller in config/boats.cfg, same as Orc/Gargoyle/
	// Tokuno, but unlike them it shouldn't fall back to a custom-created one.
	if( IsValidBoatItem( controller ) && controller.GetShipType() == 0x1605B )
		return holds; // Empty.
	endif

	var hold_serial := CInt( GetObjProperty( boat, "CustomHoldSerial" ) );
	var custom_hold := 0;
	if( hold_serial )
		custom_hold := SystemFindObjectBySerial( hold_serial );
	endif

	if( !custom_hold || custom_hold.errortext )
		var spot := Struct{ "x":=boat.x, "y":=boat.y, "z":=boat.z+1 };
		if( IsValidBoatItem( controller ) )
			spot := FindHoldSpot( boat, controller );
		endif
		custom_hold := CreateItemAtLocation( spot.x, spot.y, spot.z, 0x1F01A, 1, boat.realm );
		if( custom_hold.errortext )
			return holds; // Empty - creation failed.
		endif

		custom_hold.graphic := 0x5C2D;
		RegisterItemWithBoat( boat, custom_hold );
		SetObjProperty( boat, "CustomHoldSerial", custom_hold.serial );

		var lock_id := GetBoatLockId( boat );
		if( lock_id )
			custom_hold.locked := 1;
			SetObjProperty( custom_hold, "LockID", lock_id );
		endif
	endif

	holds.append( custom_hold );
	return holds;
endfunction

// Destroys the escript-created hold (galleons only - Storage/Hold components
// are native and get cleaned up by DestroyMulti itself). Call before
// DestroyMulti so it isn't orphaned in the world.
//
// Also destroys the owner's key for this hold's LockID, wherever they're
// keeping it (backpack or bank) - self-contained so this stays correct no
// matter what calls it, not just DryDock (which already separately sweeps
// every hold's LockID before calling this - that sweep will simply find
// nothing left the second time here, which is harmless).
function DestroyCustomBoatHold( boat )

	var hold_serial := CInt( GetObjProperty( boat, "CustomHoldSerial" ) );
	if( !hold_serial )
		return 1;
	endif

	var custom_hold := SystemFindObjectBySerial( hold_serial );
	if( custom_hold && !custom_hold.errortext )
		var lock_id := GetObjProperty( custom_hold, "LockID" );
		if( lock_id )
			var controller := GetBoatController( boat );
			var owner := 0;
			if( IsValidBoatItem( controller ) )
				owner := controller.GetOwner();
			endif
			if( owner )
				KP_DestroyOwnedKeysForLockIDs( owner, CP_DEFAULT_AREA, {lock_id} );
			endif
		endif

		DestroyItem( custom_hold );
	endif

	EraseObjProperty( boat, "CustomHoldSerial" );
	return 1;
endfunction
