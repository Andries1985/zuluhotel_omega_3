// Shared runebook action logic used by both the standard runebook gump
// (runebook.src) and the Runic Atlas gump (runicatlas.src). Operates on the
// common "RuneDefs" array / "charges"+"maxcharges" ObjProperty data model
// used by every item in this package.

use uo;
use os;
use math;

include "customspells";
include "include/objtype";
include "include/realmcolors";


function UpdateName( runebook, charges )

	set_critical(1);

	var name := GetObjProperty( runebook, "name" );
	if( !name )
		name := "Runebook";
	endif

	runebook.name := name + " [charges: " + charges + "]";

	set_critical(0);

endfunction


function RenameBook( who, runebook )

	var name := SendTextEntryGump( who, "Enter the new name of the runebook. ", TE_CANCEL_ENABLE, TE_STYLE_NORMAL );
	if( name )
		SetObjProperty( runebook, "name", name );
		return 1;
	else
		return 0;
	endif

endfunction


function RecallWithCharges( who, runebook, byref charges, byref runes_def, index )

	if( !runes_def[index] )
		SendSysMessage( who, "Error 1: Rune not found." );
		return 0;
	endif

	var new_charges := Cint( GetObjProperty( runebook, "charges" ) );

	if( new_charges < 1 )
		SendSysMessage( who, "The runebook is out of charges.");
		SetObjProperty( runebook, "charges", 0 );
		IncRevision(runebook);
	else
		var tox := Cint(runes_def[index].tox);
		var toy := Cint(runes_def[index].toy);
		var toz := Cint(runes_def[index].toz);
		var torealm := runes_def[index].torealm;

		if( !tox and !toy and !toz and !torealm )
			SendSysMessage( who, "Error 2: Blank rune!" );
			DestroyRune( runebook, runes_def, index );
			return 0;
		endif

		new_charges := new_charges - 1;
		SetObjProperty( runebook, "charges", new_charges );
		IncRevision(runebook);
		charges	:= new_charges;
		SetObjProperty( who, "betatest", 1 );

		if( !CustomRecall( who, tox, toy, toz, torealm, 1 ) )
			return 0;
		endif
	endif

	return 1;

endfunction


function RecallWithoutCharges( who, runebook, byref runes_def, index )

	if( !runes_def[index] )
		SendSysMessage( who, "Error 1: Rune not found." );
		return 0;
	endif

	var tox := Cint(runes_def[index].tox);
	var toy := Cint(runes_def[index].toy);
	var toz := Cint(runes_def[index].toz);
	var torealm := runes_def[index].torealm;

	if( !tox and !toy and !toz and !torealm )
		SendSysMessage( who, "Error 2: Blank rune!" );
		DestroyRune( runebook, runes_def, index );
		return 0;
	endif

	if( !CustomRecall( who, tox, toy, toz, torealm ) )
		return 0;
	endif

	return 1;

endfunction


function GateWithoutCharges( who, runebook, byref runes_def, index, guild_only := 0 )
	if( !runes_def[index] )
		SendSysMessage( who, "Error 1: Rune not found." );
		return 0;
	endif

	var tox := Cint(runes_def[index].tox);
	var toy := Cint(runes_def[index].toy);
	var toz := Cint(runes_def[index].toz);
	var torealm := runes_def[index].torealm;

	if( !tox and !toy and !toz and !torealm )
		SendSysMessage( who, "Error 2: Blank rune!" );
		DestroyRune( runebook, runes_def, index );
		return 0;
	endif

    //A gate is about to be opened from this runebook.
    //sets its opengate prop to 1
    SetObjProperty(runebook, "opengate", 1);
	if( !CustomGate( who, tox, toy, toz, torealm, guild_only ) )
		SetObjProperty(runebook, "opengate", 0);
		return 0;
	endif
    //The gate is closed already. Sets opengate back to 0.
    SetObjProperty(runebook, "opengate", 0);
	return 1;
endfunction


function SetDefaultLocation( runebook, byref runes_def, index, defloc := 0 )

	var old_prio := set_priority(100);

	if( !runes_def[index] )
		set_priority(old_prio);
		return 0;
	endif

	var tox := Cint(runes_def[index].tox);
	var toy := Cint(runes_def[index].toy);
	var toz := Cint(runes_def[index].toz);
	var torealm := runes_def[index].torealm;

	if( !tox and !toy and !toz and !torealm )
		DestroyRune( runebook, runes_def, index );
		set_priority(old_prio);
		return 0;
	endif

	if( runes_def[defloc] )
		if( !ResetDefaultLocation( runebook, runes_def, defloc ) )
			set_priority(old_prio);
			return 0;
		endif
	endif

	SetObjProperty( runebook, "x", tox );
	SetObjProperty( runebook, "y", toy );
	SetObjProperty( runebook, "z", toz );
	SetObjProperty( runebook, "realm", torealm );
	runes_def[index].defloc := 1;

	set_priority(old_prio);
	return 1;

endfunction


function ResetDefaultLocation( runebook, byref runes_def, index )

	var old_prio := set_priority(100);

	if( !runes_def[index] )
		set_priority(old_prio);
		return 0;
	endif

	EraseObjProperty( runebook, "x" );
	EraseObjProperty( runebook, "y" );
	EraseObjProperty( runebook, "z" );
	EraseObjProperty( runebook, "realm" );
	runes_def[index].defloc := 0;

	set_priority(old_prio);
	return 1;

endfunction


function DropRune( who, runebook, byref runes_def, index )
	set_critical(1);

	UpdateRunesDef( runebook, runes_def );

	if( !runes_def[index] )
		SendSysMessage( who, "Error 1: Rune not found." );
		set_critical(0);
		return 0;
	endif

	if( runes_def[index].defloc )
		ResetDefaultLocation( runebook, runes_def, index );
	endif


    //Checks if there is a gate opened sent from that runebook
    //this is a work around to prevent players from duping runes.
    if(GetObjProperty(runebook, "opengate") == 1)
        return 0;
    endif

	var rune := CreateItemInContainer( who.backpack, UOBJ_RUNE );
	if( !rune )
		SendSysMessage( who, "Error 2: Your backpack is full." );
		set_critical(0);
		return 0;
	endif

	var name := runes_def[index].name;
	var tox  := Cint(runes_def[index].tox);
	var toy  := Cint(runes_def[index].toy);
	var toz  := Cint(runes_def[index].toz);
	var torealm  := runes_def[index].torealm;

	if( !runes_def.erase(index) )
		SendSysMessage( who, "Error 3: Couldn't erase the rune data from the book." );
		DestroyItem( rune );
		set_critical(0);
		return 0;
	endif

	if( !tox and !toy and !toz and !torealm)
		SendSysMessage( who, "Error 4: Blank rune!" );
		DestroyItem( rune );
		set_critical(0);
		return 0;
	endif

	if( !name )
		name := "a marked recall rune";
	elseif( name != "a marked recall rune" )
		var splited := SplitWords( name );
		name := "A recall rune for";
		foreach part in splited
			name := name + " " + part;
		endforeach
	endif

	if( !SetObjProperty( rune, "x", tox ) and
	    !SetObjProperty( rune, "y", toy ) and
	    !SetObjProperty( rune, "z", toz ) and
		!SetObjProperty( rune, "realm", torealm ) )
	    	SendSysMessage( who, "Error 5: Couldn't copy the destination data on the rune!" );
	    	DestroyItem( rune );
	    	set_critical(0);
		return 0;
	endif

	SendSysMessage( who, "Rune " + name + " was successfuly dropped in your backpack." );

	rune.name := name;
	rune.color := GetRealmRuneColor( torealm );

	set_critical(0);
	return 1;

endfunction


function DestroyRune( runebook, byref runes_def, index )

	set_critical(1);

	UpdateRunesDef( runebook, runes_def );

	if( !runes_def[index] )
		set_critical(0);
		return 0;
	endif

	if( !runes_def.erase(index) )
		set_critical(0);
		return 0;
	endif

	set_critical(0);
	return 1;

endfunction


function UpdateRunesDef( runebook, byref runes_def )

	set_critical(1);

	var current_defs := GetObjProperty( runebook, "RuneDefs" );
	if( !current_defs )
		current_defs := array;
	endif

	runes_def := current_defs;

	set_critical(0);

endfunction


function GetSextantLatitude( y )

	var degrees := (y-1624) * 0.087890625;
	var minutes;
	var direction;

	if( degrees < 0 )
		direction := "N";
	else
		direction := "S";
	endif

	degrees := Abs( degrees );
	minutes := CInt((CInt((degrees-Floor(degrees))*100)*60)/100);

	return (Floor(degrees) + "o" + minutes + "'" + direction);

endfunction


function GetSextantLongitude( x )

	var degrees := (x-1323) * 0.0703125;
	var minutes;
	var direction;

	if( degrees < 0 )
		direction := "W";
	else
		direction := "E";
	endif

	degrees := Abs( degrees );
	minutes := CInt((CInt((degrees-Floor(degrees))*100)*60)/100);

	return (Floor(degrees) + "o" + minutes + "'" + direction);

endfunction
