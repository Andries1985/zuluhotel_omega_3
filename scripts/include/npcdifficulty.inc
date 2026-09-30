// npcdifficulty.inc -- per-NPC snoop / steal difficulty (2026-09-30).
//
// The snoopme / stealme values moved from template CProps to npcdesc.cfg template fields
// (npcdesc reformat). An instance ObjProperty of the same name still wins -- that is how the
// areaspawner custom-NPC override is stored, and any NPC spawned before the move keeps its old
// CProp -- then the template field. 0 = cannot be snooped / stolen from.

use uo;
use cfgfile;

var npcdifficulty_cfg := ReadConfigFile( "::npcdesc" );

function GetNpcDifficultyField( mob, field )

	var override := GetObjProperty( mob, field );
	if( override )
		return Cint( override );
	endif

	if( !mob.npctemplate )
		return 0;
	endif

	return Cint( GetConfigString( npcdifficulty_cfg[mob.npctemplate], field ) );

endfunction

function GetSnoopDifficulty( mob )

	return GetNpcDifficultyField( mob, "snoopme" );

endfunction

function GetStealDifficulty( mob )

	return GetNpcDifficultyField( mob, "stealme" );

endfunction
