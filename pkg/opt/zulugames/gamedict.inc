// GAMEDICT.INC
//
// Creator: Myrathi <tjh@myrathi.co.uk>
// Created: 25-Apr-2001

const GD_KEY	:= 1;
const GD_VALUE	:= 2;

// Takes an array of key/value arrays and turns it into a dictionary
//
function gdMakeDict( zArray )
	var dTmp := dictionary;
	foreach pair in zArray
		dTmp[pair[GD_KEY]] := pair[GD_VALUE];
	endforeach
	return dTmp;
endfunction
