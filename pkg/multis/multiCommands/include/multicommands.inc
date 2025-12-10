use uo;

function Multi_DestroyMulti( multi )

	return DestroyMulti( multi );
endfunction

function Multi_GetTarget( mobile, name, flags:=0, dist:=20 )

	var targ := TargetMultiPlacement( mobile, name, flags );
	if( !targ )
		SendSysMessage( mobile, "Cancelled."+targ.errortext );
		return 0;
	elseif( !CheckLosAt( mobile, targ.x, targ.y, targ.z ) && flags != CRMULTI_IGNORE_ALL )
		SendSysMessage( mobile, "You do not have line of sight to there." );
		return 0;
	elseif( CoordinateDistance( mobile.x, mobile.y, targ.x, targ.y ) > dist )
		SendSysMessage( mobile, "That is too far away." );
		return 0;
	endif

	return targ;
endfunction

function Multi_MakeMulti( mobile, name, targ, flags:=0 )

	var multi := CreateMultiAtLocation( targ.x, targ.y, targ.z, name, flags, targ.realm );
	if( multi.errortext )
		SendSysMessage( mobile, "Error creating multi "+name+" -->"+multi.errortext );
                return 0;
	endif

	return multi;
endfunction

//$Id: settings.inc 373 2006-06-17 18:27:33Z austinheilman $

/*===============================================================
* Current Version
* SETTINGS.INC - v1.0
* Updated 9/27/2005 2:54PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * SH_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :staticHousing:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function SH_GetSettingsCfgFile()
	UnloadConfigFile(":house:settings");
	var cfg := ReadConfigFile(":house:settings");

	if ( cfg.errortext )
		SysLog("Error::SH_GetSettingsCfgFile() - Unable to open [:house:settings] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * SH_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by SH_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function SH_GetSettingsCfgElem(elem_name, byref cfg_file:=0)

  if ( !cfg_file )
		cfg_file := SH_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

  if ( elem.errortext )
		SysLog("Error::SH_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction