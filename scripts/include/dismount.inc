use uo;
use os;
use basic;

include "include/myutil";
include "include/attributes";
include "include/client";
include "include/constants/locations";

//function returns 0 if the animal couldn't be created and/or
//if the mount wasn't destroyed

function dismount( me, mount := 0 )

	set_critical(1);

	if( !mount )
		if( !me )
			set_critical(0);
			return 0;
		else
			if( !me.corpseType )
				mount := GetEquipmentByLayer( me, LAYER_MOUNT );
			else
				mount := FindObjtypeInContainer(me, 0x1f021, FINDOBJTYPE_ROOT_ONLY);
			endif
		endif
	endif

	if( !mount )
		set_critical(0);
		return 0;
	endif

	var critter := GetObjProperty( mount, "npctemplate" );
	if( !critter )
		critter := "horse";
	endif

	var animal := CreateNpcFromTemplate(critter, me.x, me.y, me.z, 0, me.realm);
	if( !animal )
		// Death tiles can fail NPC spawn; fallback to relocation coords.
		animal := CreateNpcFromTemplate(
			critter,
			DEFAULT_LOCATION_NPC_RELOCATION_X,
			DEFAULT_LOCATION_NPC_RELOCATION_Y,
			DEFAULT_LOCATION_NPC_RELOCATION_Z,
			0,
			DEFAULT_LOCATION_NPC_RELOCATION_R
		);
	endif
	if( !animal )
		Print( "dismount(): failed to spawn mount npc template '"+CStr(critter)+"' near "+CStr(me.x)+","+CStr(me.y)+","+CStr(me.z)+" in realm "+CStr(me.realm) );
		set_critical(0);
		return 0;
	endif

	SetObjProperty(animal, "nocut", 1);
	SetObjProperty(animal, "noloot", 1);

	if( GetObjProperty( me, "DonatorMounted") )
		SetObjProperty( animal, "DonatorMount", 1);
		SetObjProperty( me, "DMountSerial", animal.serial);
	endif

	animal.color := mount.color;
	SetObjProperty( animal, "color", mount.color );

	if (GetObjProperty(mount, "petname"))
		animal.name := GetObjProperty(mount, "petname");
	endif

	if (Getobjproperty(mount,"pethp"))
		SetHP(animal,  Getobjproperty(mount,"pethp"));
	endif
	
	if (Getobjproperty(mount,"petmana"))
		SetMana(animal, Getobjproperty(mount,"petmana"));
	endif

	if (Getobjproperty(mount,"petstamina"))
		SetStamina(animal, Getobjproperty(mount,"petstamina"));
	endif

	if (Getobjproperty(mount,"DMountStoneSerial"))
		SetObjProperty(animal,"DMountStoneSerial", Getobjproperty(mount,"DMountStoneSerial"));
	endif

	if( me )
		if( me.corpseType )
			SetObjProperty(animal, "master", me.serial );
			var master := SystemFindObjectBySerial( CInt(GetObjProperty( me, "whose" )) );
			if( master )
				animal.script := "tamed";
				animal.setmaster( master );
			endif
			MoveObjectToLocation( animal, me.x, me.y, me.z, me.realm, MOVEOBJECT_FORCELOCATION );
			animal.facing := me.facing;
			RestartScript(animal);
		elseif( !me.npctemplate )
			SetObjProperty( animal, "master", me.serial );
			animal.setmaster( me );
			animal.script := "tamed";
			MoveObjectToLocation( animal, me.x, me.y, me.z, me.realm, MOVEOBJECT_FORCELOCATION );
			animal.facing := me.facing;
			RestartScript(animal);
		endif
	else
		SetObjProperty(animal,"killme",1);
	endif



	if (DestroyItem( mount ))
		set_critical(0);
		return 1;
	else
		set_critical(0);
		return 0;
	endif

endfunction