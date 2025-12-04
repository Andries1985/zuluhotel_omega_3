use cfgfile;

include "include/client";
include "include/objtype";
include "include/dotempmods";
include "include/random";

const DRINK_SOUND	:= 0x0032;
const DELAY_BETWEEN_DRINK	:= 10;
const DURATION_MULTIPLIER	:= 120;

function CheckIfCanDrink( character )

	// Potion prop is removed on logout, so if it exists AND if it's greater than readgameclock dont drink
	if( GetObjProperty( character , "#potion" ) && GetObjProperty( character , "#potion" ) > ReadGameClock() )
		SendSysmessage( character , "You must wait until you can drink another potion!" );
		return 0;
	else
		SetObjProperty( character , "#potion" , ReadGameClock() + DELAY_BETWEEN_DRINK );
		return 1;
	endif
		
endfunction


function CanDrink( character )

	EraseObjProperty( character , "#potion" );
		
endfunction


function EmptyBottle( character, potion )

	ReserveItem( potion );
	PlaySoundEffect( character , DRINK_SOUND );
	CreateItemInBackpack( character , UOBJ_EMPTY_BOTTLE , 1 );
	SubtractAmount(potion, 1); 
	ReleaseItem( potion );
	
endfunction


function GetPotionStrength( potion ) 
	
	var itemcfg := ReadConfigFile( ":alchemy:itemdesc" );

	if( potion.objtype == 0x7059 || (potion.objtype >= 0xff4e && potion.objtype <= 0xff95))
		return GetObjProperty( potion ,"strength" );
	else
		var power := Cint( itemcfg[potion.objtype].Strength );
		if( !power )
			return 0;
		endif
		
		if( GetObjProperty( potion , "ByTrueMage" ) )
			power := power + Cint(GetObjProperty( potion , "ByTrueMage" ));
		endif
		
		return power;
	endif

endfunction


function IsPoison( potion )

	return GetObjProperty( potion , "Poisoned" );
		
endfunction


function DoPoison( character , potion )

	var plvl := IsPoison( potion );
	
	if( Checkskill( character , Cint(SKILLID_TASTEID) , Cint(plvl*15) ,  Cint(plvl*15*10) ) )
		SendSysMessage( character , "That potion is poisonous!" );
		return;
	endif

	EmptyBottle( character , potion );

	SetPoison( character , plvl , "You've been poisoned!" );

endfunction
