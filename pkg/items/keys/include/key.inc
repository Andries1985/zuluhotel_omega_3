use uo;
use storage;

include ":itemUtils:itemdesc";
//include "../pkg/mobiles/npcs/include/NPCBackpacks";
include ":containers:storageAreas";
include ":containers:containers";
include "include/objtype";

function KP_ToggleLock( object, mobile:=0 )

	if( !mobile )
		if( object.locked )
			PrintTextAbove( object, "*unlocked*" );
			object.locked := 0;
		else
			PrintTextAbove( object, "*locked*" );
			object.locked := 1;
		endif
	else
		if( object.locked )
			PrintTextAbovePrivate( object, "*unlocked*", mobile );
			object.locked := 0;
		else
			PrintTextAbovePrivate( object, "*locked*", mobile );
			object.locked := 1;
		endif
	endif

	return 1;
endfunction

function KP_IsLockable( object )

	if( object.IsA( POLCLASS_ITEM ))
		object := object.objtype;
	endif

	return GetItemDescInfo( object ).Lockable;
endfunction

function KP_HasKeyForLock( container, lock_id )
	var held_item := 0;

	if( container.IsA( POLCLASS_MOBILE ) )
		held_item := container.GetGottenItem();
		container := container.backpack;
	endif

	if( lock_id.IsA( POLCLASS_ITEM ) )
		lock_id := GetObjProperty( lock_id, "LockID" );
	endif

	var inventory := EnumerateItemsInContainer( container );
	if( held_item.IsKey() )
		inventory.Append( held_item );
	elseif( held_item.IsKeyring() )
		inventory := inventory+EnumerateItemsInContainer( held_item );
	endif

	foreach item in ( inventory )
		SleepMS( 5 );
		if( item.IsKey() )
			if( item.KeyMatchesLock( lock_id ) )
				return 1;
			endif
		endif
	endforeach

	return 0;
endfunction

function AllocateLockID()

	var lock_id := GetGlobalProperty( "NextLockID" );
	if( !lock_id )
		lock_id := 1;
	endif

	SetGlobalProperty( "NextLockID", lock_id + 1 );

	return lock_id;
endfunction

function FindMyKey( serial )
	return CP_GetStorageAreaContainer( CStr(serial), CP_KEYRING_AREA, CP_CREATE );
endfunction

function FindExistingKey( serial )
	return CP_GetStorageAreaContainer( CStr(serial), CP_KEYRING_AREA, CP_NOCREATE );
endfunction

function DestroyMyKey( serial )
	return CP_RemoveStorageAreaContainer( CStr(serial), CP_KEYRING_AREA );
endfunction

function KP_DestroyStoredKeysForLockIDs( keyring, byref lock_ids )
	if( !keyring || !keyring.IsKeyring() )
		return 0;
	endif

	var key_storage := FindExistingKey( keyring.serial );
	if( !key_storage )
		return 0;
	endif

	var destroyed := 0;
	foreach stored_key in EnumerateItemsInContainer( key_storage )
		var lock_id := GetObjProperty( stored_key, "LockID" );
		if( lock_id && lock_id in lock_ids )
			if( DestroyItem( stored_key ) )
				destroyed := destroyed + 1;
			endif
		endif
	endforeach

	if( !EnumerateItemsInContainer( key_storage, ENUMERATE_ROOT_ONLY ).Size() )
		DestroyMyKey( keyring.serial );
	endif

	return destroyed;
endfunction

function KP_DestroyKeysForLockIDsInContainer( container, byref lock_ids )
	if( !container )
		return 0;
	endif

	var destroyed := 0;
	foreach thing in EnumerateItemsInContainer( container )
		if( thing.IsKeyring() )
			destroyed := destroyed + KP_DestroyStoredKeysForLockIDs( thing, lock_ids );
		endif

		var lock_id := GetObjProperty( thing, "LockID" );
		if( lock_id && lock_id in lock_ids )
			if( DestroyItem( thing ) )
				destroyed := destroyed + 1;
			endif
		endif
	endforeach

	return destroyed;
endfunction

function KP_DestroyOwnedKeysForLockIDs( owner, bank_region, byref lock_ids )
	if( !owner )
		return 0;
	endif

	var destroyed := KP_DestroyKeysForLockIDsInContainer( owner.backpack, lock_ids );
	// Destroy-only lookup -- never writes into the bankbox, so this should
	// never create one for an owner who doesn't already have it (matches the
	// CP_NOCREATE convention FindExistingKey() uses above, and this
	// function's own default parameter value).
	var bankbox := CP_GetStorageContainerForMobile( owner, bank_region, CP_NOCREATE );
	if( bankbox )
		destroyed := destroyed + KP_DestroyKeysForLockIDsInContainer( bankbox, lock_ids );
	endif

	return destroyed;
endfunction

/*
function Create_ChestKey( chest )

	var the_key := CreateItemInContainer( chest, 0x100E, 1 );
	if( the_key.errortext )
		PrintTextAbove( the_key, "Error creating key -->"+the_key.errortext );
		return 0;
	endif

	var ided := GetObjProperty( chest, "IDed" );
	if( ided )
		SetObjProperty( the_key, "IDed", ided );
	endif

	var lock_id := CreateLockID();

	SetObjProperty( the_key, "LockID", lock_id );
	SetObjProperty( chest, "LockID", lock_id );

	return 1;
endfunction
*/
function Create_ChestKey( chest )

	var the_key := CreateItemInContainer( chest, 0x100E, 1 );
	if( the_key.errortext )
		PrintTextAbove( the_key, "Error creating key -->"+the_key.errortext );
		return 0;
	endif

	var ided := GetObjProperty( chest, "IDed" );
	if( ided )
			SetObjProperty( the_key, "IDed", ided );
	endif

	var lock_id := CreateLockID();

	SetObjProperty( the_key,  "LockID", lock_id );
	SetObjProperty( chest, "LockID", lock_id );

	return 1;
endfunction

/*
 *
 * FixOldKeyRings( keyring )
 *
 * Parameters:
 * keyring - keyring obj ref
 *
 * Purpose:
 * Removes the storage areas for keyrings, moving all keys within a storage area
 * to the keyring container itself.
 *
 * Date: 2019-08-22	05:49:18
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Boolean - 1 on success, 0 on failure
 *
 
function DuplicateKeyRing( byref keyring )
	var failed := 0,
		LockID := 0,
		key_desc := 0,
		key := 0,
		on_ring := FindMyKey( keyring.serial );

	if( !on_ring )
		return 0;
	endif

	foreach otherkey in EnumerateItemsInContainer( on_ring )
		Sleepms( 5 );
		LockID := GetObjProperty( otherkey, "LockID" );
		if( LockID )
			key_desc := GetItemDescriptor( otherkey.objtype );
			key_desc.CProps.Insert( "LockID", LockID );
			key := CreateItemInContainer( keyring, key_desc, 1 );
			if( key )
				SetName( key, otherkey.desc );
				DestroyItem( otherkey );
			else
				failed := 1;
			endif
		endif
	endforeach

	if( !failed )
		DestroyMyKey( keyring.serial );
		return 1;
	endif

	return 0;
endfunction
*/
