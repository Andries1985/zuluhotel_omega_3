// Arcomage records, leaderboard and daily gold. 2026-10-23: new package.
//
// Records are per character, in the datafile :arcomage:records keyed by serial:
//   Name, Wins, Losses, Ties, and Grid = dictionary "<ruleset>-<difficulty>" -> wins.
// Daily gold is per account: the account property ArcomageGoldDays holds, per difficulty,
// the server date ("YYYY-MM-DD", server local time) it was last paid. One paid win per
// difficulty per server day, against any NPC.
use uo;
use os;
use basic;
use basicio;
use datafile;
use util;

const ARCO_RECORDS_DF := ":arcomage:records";
const ARCO_GOLD_DAYS_PROP := "ArcomageGoldDays";
const ARCO_GOLD_PER_POINT := 100;
const ARCO_GOLD_OBJTYPE := 0x0EED;

function ArcoOpenRecords()

	var df := OpenDataFile( ARCO_RECORDS_DF );
	if( !df )
		CreateDataFile( ARCO_RECORDS_DF, DF_KEYTYPE_STRING );
		df := OpenDataFile( ARCO_RECORDS_DF );
	endif
	return df;

endfunction

function ArcoGridKey( ruleset_id, difficulty )

	return CStr( CInt( ruleset_id ) ) + "-" + CStr( CInt( difficulty ) );

endfunction

// result: "win", "loss" or "tie".
function ArcoRecordResult( who, ruleset_id, difficulty, result )

	var df := ArcoOpenRecords();
	if( !df )
		Print( "[Arcomage] Could not open " + ARCO_RECORDS_DF );
		return 0;
	endif

	var key := CStr( who.serial );
	var elem := df.FindElement( key );
	if( !elem )
		elem := df.CreateElement( key );
	endif

	elem.SetProp( "Name", who.name );
	var grid, grid_key;
	case( result )
		"win":
			elem.SetProp( "Wins", CInt( elem.GetProp( "Wins" ) ) + 1 );
			grid := elem.GetProp( "Grid" );
			if( !grid )
				grid := dictionary;
			endif
			grid_key := ArcoGridKey( ruleset_id, difficulty );
			grid[grid_key] := CInt( grid[grid_key] ) + 1;
			elem.SetProp( "Grid", grid );
		"loss":
			elem.SetProp( "Losses", CInt( elem.GetProp( "Losses" ) ) + 1 );
		"tie":
			elem.SetProp( "Ties", CInt( elem.GetProp( "Ties" ) ) + 1 );
	endcase

	UnloadDataFile( ARCO_RECORDS_DF );
	return 1;

endfunction

function ArcoEmptyRecord( name := "" )

	return struct{ "name" := name, "wins" := 0, "losses" := 0, "ties" := 0, "grid" := dictionary, "beaten" := 0 };

endfunction

function ArcoRecordFromElem( elem )

	var rec := ArcoEmptyRecord( elem.GetProp( "Name" ) );
	rec.wins := CInt( elem.GetProp( "Wins" ) );
	rec.losses := CInt( elem.GetProp( "Losses" ) );
	rec.ties := CInt( elem.GetProp( "Ties" ) );
	var grid := elem.GetProp( "Grid" );
	if( grid )
		rec.grid := grid;
	endif

	// Distinct NPCs beaten at any difficulty.
	var beaten := dictionary;
	var grid_keys := rec.grid.keys();
	foreach grid_key in grid_keys
		if( CInt( rec.grid[grid_key] ) > 0 )
			var parts := SplitWords( grid_key, "-" );
			beaten[parts[1]] := 1;
		endif
	endforeach
	rec.beaten := beaten.size();
	return rec;

endfunction

function ArcoGetRecord( who )

	var df := ArcoOpenRecords();
	if( !df )
		return ArcoEmptyRecord( who.name );
	endif

	var elem := df.FindElement( CStr( who.serial ) );
	var rec;
	if( elem )
		rec := ArcoRecordFromElem( elem );
	else
		rec := ArcoEmptyRecord( who.name );
	endif

	UnloadDataFile( ARCO_RECORDS_DF );
	return rec;

endfunction

function ArcoGridWins( rec, ruleset_id, difficulty )

	return CInt( rec.grid[ArcoGridKey( ruleset_id, difficulty )] );

endfunction

// Top `limit` characters by wins (fewer losses breaks a tie).
function ArcoLeaderboard( limit := 20 )

	var board := array;
	var df := ArcoOpenRecords();
	if( !df )
		return board;
	endif

	var keys := df.Keys();
	foreach key in keys
		var elem := df.FindElement( key );
		if( elem )
			var rec := ArcoRecordFromElem( elem );
			if( rec.wins > 0 )
				board.append( rec );
			endif
		endif
	endforeach
	UnloadDataFile( ARCO_RECORDS_DF );

	// Insertion sort: wins descending, then losses ascending.
	var i, j, moving;
	for( i := 2; i <= board.size(); i := i + 1 )
		moving := board[i];
		j := i - 1;
		while( j >= 1 )
			if( !ArcoRanksAbove( moving, board[j] ) )
				break;
			endif
			board[j + 1] := board[j];
			j := j - 1;
		endwhile
		board[j + 1] := moving;
	endfor

	while( board.size() > limit )
		board.erase( board.size() );
	endwhile
	return board;

endfunction

function ArcoRanksAbove( a, b )

	if( a.wins != b.wins )
		return a.wins > b.wins;
	endif
	return a.losses < b.losses;

endfunction

function ArcoToday()

	return StrFormatTime( "%Y-%m-%d", CInt( polcore().systime ) );

endfunction

function ArcoDailyGoldClaimed( who, difficulty )

	var acct := who.acct;
	if( !acct )
		return 1;
	endif
	var days := acct.GetProp( ARCO_GOLD_DAYS_PROP );
	if( !days )
		return 0;
	endif
	var claimed := days[CStr( CInt( difficulty ) )];
	if( !claimed )
		return 0;
	endif
	return claimed == ArcoToday();

endfunction

function ArcoDailyGoldAmount( difficulty )

	return CInt( difficulty ) * ARCO_GOLD_PER_POINT;

endfunction

// Pays the daily gold for a win at `difficulty` if this account hasn't had it today.
// Returns the amount paid, or 0. The day is marked only after the gold exists.
function ArcoPayDailyGold( who, difficulty )

	if( ArcoDailyGoldClaimed( who, difficulty ) )
		return 0;
	endif

	var amount := ArcoDailyGoldAmount( difficulty );
	var gold := CreateItemInContainer( who.backpack, ARCO_GOLD_OBJTYPE, amount );
	if( !gold )
		gold := CreateItemAtLocation( who.x, who.y, who.z, ARCO_GOLD_OBJTYPE, amount, who.realm );
		if( !gold )
			Print( "[Arcomage] Could not create " + CStr( amount ) + " gold for " + who.name + ": " + gold.errortext );
			return 0;
		endif
		SendSysMessage( who, "Your pack is full; the gold lies at your feet.", 3, 73 );
	endif

	var acct := who.acct;
	var days := acct.GetProp( ARCO_GOLD_DAYS_PROP );
	if( !days )
		days := dictionary;
	endif
	days[CStr( CInt( difficulty ) )] := ArcoToday();
	acct.SetProp( ARCO_GOLD_DAYS_PROP, days );
	return amount;

endfunction
