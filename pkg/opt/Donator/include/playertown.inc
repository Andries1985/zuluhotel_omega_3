// playertown.inc -- shared by the four donator mount stones.
//
// 2026-09-22: IsInPlayerCity() used to search 250 tiles for objtype 0x7566
// "city stones" carrying MinX/MaxX/MinY/MaxY properties -- an item this repo
// never had, so player towns never counted and only IsInACity()'s hardcoded
// Britannia city boxes did. A player town is now a City region (regions.cfg)
// that has a town stone registered in the townstone datafile; standing in
// that region counts.
use uo;
use datafile;

const PLAYERTOWN_DATAFILE := ":townstone:townstone_data";

function IsInPlayerCity( who )
	var region_name := GetRegionName( who );
	if( !region_name )
		return 0;
	endif

	var df := OpenDataFile( PLAYERTOWN_DATAFILE );
	if( !df )
		return 0;
	endif

	var in_town := 0;
	var elem := df.FindElement( region_name );
	if( elem )
		var stone := SystemFindObjectBySerial( CInt( elem.GetProp( "stone_serial" ) ) );
		if( stone )
			in_town := 1;
		endif
	endif
	UnloadDataFile( PLAYERTOWN_DATAFILE );
	return in_town;
endfunction
