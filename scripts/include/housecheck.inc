use uo;

include "include/objtype";
include "include/spelldata";
include "include/areas";
include "include/client";
include ":housing:utility";

function Housecheck(character)
	var multi := character.multi;
	if(multi)
		if( !Iscowner( character, multi ) )
			if( !IsFriend( character, multi ) )
				return 0;
			else
				return 1;
			endif
		else
			return 1;
		endif
	endif

endfunction  