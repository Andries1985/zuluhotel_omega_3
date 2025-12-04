use uo;
use os;

include "include/spelldata";
include "include/attributes";
include "include/dotempmods";
include "include/classes";
include "include/client";

function checkmymod(player)
	if(player.warmode && IsInSafeArea(player))
		SetObjProperty(player, "guardkill", 1);
		ApplyRawdamage(player, GetMaxHP(player) + 3);
	endif

	var allmods := GetObjProperty( player , "#mods" );
	if(allmods)
		var newmods := {};
		var i;
		for( i := 1 ; i <= len(allmods) ; i := i + 1 )
			if( ReadGameClock() >= allmods[i][3] )
				if(allmods[i][4])
					UndoTheMod( player , allmods[i][1] , allmods[i][2], allmods[i][4] );
				else
					UndoTheMod( player , allmods[i][1] , allmods[i][2] );
				endif
			else	
				newmods.append( allmods[i] );
			endif
		endfor
						
		if( len(newmods) )
			SetObjProperty( player , "#mods" , newmods );
		else
			EraseObjProperty( player , "#mods" );
		endif
	endif
    return;
endfunction