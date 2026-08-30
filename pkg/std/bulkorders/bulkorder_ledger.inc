// Bulk Order Deeds -- points ledger (Phase 4).
//
// A plain running balance, not a bank -- no decay, no spend-now-vs-bank
// choice. Shared across every craft and every character on the account
// (Points are the same flat/multiplier math regardless of which craft
// earned them, and the only thing they buy right now -- Omega Tokens -- is
// a universal currency, so a single shared pool avoids fragmenting it into
// 8 tiny per-craft balances a player would need to grind separately).

use datafile;

const BOD_POINTS_DATAFILE := ":bulkorders:bulkorder_points";

function BOD_OpenPointsDataFile()
	var df := OpenDataFile(BOD_POINTS_DATAFILE);
	if( !df )
		CreateDataFile(BOD_POINTS_DATAFILE, flags := DF_KEYTYPE_STRING);
		df := OpenDataFile(BOD_POINTS_DATAFILE);
	endif
	return df;
endfunction

function BOD_GetPoints(byref who)
	var df := BOD_OpenPointsDataFile();
	if( !df )
		return 0;
	endif
	var points := 0;
	var elem := df.FindElement(who.acctname);
	if( elem )
		points := CInt(elem.GetProp("Points"));
	endif
	UnloadDataFile(BOD_POINTS_DATAFILE);
	return points;
endfunction

function BOD_AddPoints(byref who, amount)
	if( amount <= 0 )
		return;
	endif
	var df := BOD_OpenPointsDataFile();
	if( !df )
		return;
	endif
	var elem := df.FindElement(who.acctname);
	if( !elem )
		elem := df.CreateElement(who.acctname);
	endif
	if( elem )
		elem.SetProp("Points", CInt(elem.GetProp("Points")) + amount);
	endif
	UnloadDataFile(BOD_POINTS_DATAFILE);
endfunction

// Returns 1 and deducts on success, 0 (no change) if the balance is short.
function BOD_SpendPoints(byref who, amount)
	var df := BOD_OpenPointsDataFile();
	if( !df )
		return 0;
	endif
	var elem := df.FindElement(who.acctname);
	var points := 0;
	if( elem )
		points := CInt(elem.GetProp("Points"));
	endif
	if( points < amount )
		UnloadDataFile(BOD_POINTS_DATAFILE);
		return 0;
	endif
	elem.SetProp("Points", points - amount);
	UnloadDataFile(BOD_POINTS_DATAFILE);
	return 1;
endfunction
