// npcdifficulty.inc -- per-NPC snoop / steal difficulty (2026-09-30).
//
// The snoopme / stealme values moved from template CProps to npcdesc.cfg template fields
// (npcdesc reformat). An instance ObjProperty of the same name still wins -- that is how the
// areaspawner custom-NPC override is stored, and any NPC spawned before the move keeps its old
// CProp -- then the template field. 0 = cannot be snooped / stolen from.

use uo;
use cfgfile;

var npcdifficulty_cfg := ReadConfigFile( "::npcdesc" );

// 2026-10-25: the instance value of a difficulty field, or -1 when the NPC has none. 0 is a real value
// (cannot be snooped / stolen from / provoked / peaced), which `if( override )` used to treat as unset
// (.newmobedit writes it; ZH2.5 1.1.5 port).
function GetNpcDifficultyOverride( mob, field )

	var override := GetObjProperty( mob, field );
	if( TypeOf( override ) == "Error" )
		return -1;
	endif

	return Cint( override );

endfunction

function GetNpcDifficultyField( mob, field )

	var override := GetNpcDifficultyOverride( mob, field );
	if( override >= 0 )
		return override;
	endif

	if( !mob.npctemplate )
		return 0;
	endif

	return GetConfigInt( npcdifficulty_cfg[mob.npctemplate], field );

endfunction

function GetSnoopDifficulty( mob )

	return GetNpcDifficultyField( mob, "snoopme" );

endfunction

function GetStealDifficulty( mob )

	return GetNpcDifficultyField( mob, "stealme" );

endfunction
