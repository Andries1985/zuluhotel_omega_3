// Shared player-vendor escrow helpers

use os;
use uo;
use util;
use datafile;

const PLAYERVENDOR_ESCROW_DF_PREFIX := "merchantescrow_";

function BuildEscrowLegacyDatafileName(player_obj)
	if(!player_obj)
		return "";
	endif

	var name := player_obj.name;
	var sanitized := StrReplace(name, " ", "_");
	return PLAYERVENDOR_ESCROW_DF_PREFIX + sanitized + "_" + player_obj.serial;
endfunction

function BuildEscrowDatafileName(player_obj)
	if(!player_obj)
		return "";
	endif

	var serial_name := PLAYERVENDOR_ESCROW_DF_PREFIX + CStr(player_obj.serial);
	var serial_df := OpenDataFile(serial_name);
	if(serial_df)
		UnloadDataFile(serial_name);
		return serial_name;
	endif

	var legacy_name := BuildEscrowLegacyDatafileName(player_obj);
	var legacy_df := OpenDataFile(legacy_name);
	if(legacy_df)
		UnloadDataFile(legacy_name);
		return legacy_name;
	endif

	return serial_name;
endfunction
