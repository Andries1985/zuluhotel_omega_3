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
 * HE_GetSettingsCfgFile(engine_name)
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
function HE_GetSettingsCfgFile()
	UnloadConfigFile(":houseExtras:settings");
	var cfg := ReadConfigFile(":houseExtras:settings");

	if ( cfg.errortext )
		SysLog("Error::HE_GetSettingsCfgFile() - Unable to open [:houseExtras:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * HE_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by HE_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function HE_GetSettingsCfgElem(elem_name, byref cfg_file:=0)

  if ( !cfg_file )
		cfg_file := HE_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

  if ( elem.errortext )
		SysLog("Error::HE_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

function DemolishTeleporter(tele, who)

	var settings := HE_GetSettingsCfgElem("settings");
	var othertele := SystemFindObjectBySerial(GetObjProperty(tele, "Othertele"));
	if(!othertele)
		SendSysMessage(who, "I cannot locate the destination teleporter!", color := 33);
		SendSysMessage(who, "I will remove this teleporter anyway.", color := 33);
	endif
	var housesign := SystemFindObjectBySerial(GetObjProperty(tele, "signserial"));
	if(GetObjProperty(tele, "Primary"))
		if(settings.LimitHouseTeleporters)
			SetObjProperty(housesign, "numhteles", CInt(GetObjProperty(housesign, "numhteles")) - 1);
			if(GetObjProperty(housesign, "numhteles") < 0)
				SetObjProperty(housesign, "numhteles", 0);
			endif
		endif
	endif
	DestroyItem(tele);
	if(othertele)
		housesign := SystemFindObjectBySerial(GetObjProperty(othertele, "signserial"));
		if(GetObjProperty(othertele, "Primary"))
			if(settings.LimitHouseTeleporters)
				SetObjProperty(housesign, "numhteles", CInt(GetObjProperty(housesign, "numhteles")) - 1);
				if(GetObjProperty(housesign, "numhteles") < 0)
					SetObjProperty(housesign, "numhteles", 0);
				endif
			endif
		endif
		DestroyItem(othertele);
	endif
	SendSysMessage(who, "Removed.", color := 66);
	return;

endfunction