use uo;
use os;
use storage;

include "include/objtype";
include "include/client";

function RemoveCamouflage( thief )

 	thief.color := getobjproperty(thief, "cmfc");
	thief.graphic := getobjproperty(thief, "cmfg");
	thief.name := getobjproperty(thief, "cmfn");

	eraseobjproperty(thief, "cmfc");
	eraseobjproperty(thief, "cmfg");
	eraseobjproperty(thief, "cmfn");
	

	var storage := findsa(thief);
	var hair := getequipmentbylayer(thief, LAYER_HAIR );
	destroyitem(hair);

	hair := getequipmentbylayer(thief, LAYER_BEARD );
	destroyitem(hair);

	var itm;

	foreach itm in enumerateitemsincontainer(storage)
	     equipitem(thief, itm);
	endforeach

endfunction
 
 
function FindSA( character )
    var worldbank := disguisesa();

    var bank_obj_name := "Disguises of " + character.serial;
    var bankbox := FindRootItemInStorageArea( worldbank, bank_obj_name );

    if (!bankbox)
        bankbox := CreateRootItemInStorageArea( worldbank, bank_obj_name, UOBJ_BANKBOX );
    endif

        // should never happen.  FIXME make sure and document that fact
    if (!bankbox)
        print( "Unable to find or create disguise area for " + character.serial );
    endif
    
    return bankbox;
endfunction 
 
 
 

function DisguiseSA()
    var bank := FindStorageArea( "Disguise Storage" );
    if (!bank)
        bank := CreateStorageArea( "Disguise Storage" );
    endif

    if (!bank)
        print( "Unable to open or create disguise storage!" );
    endif
    return bank;
endfunction 