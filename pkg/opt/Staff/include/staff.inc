use uo;
use os;
use datafile;

include "include/time";

//////////////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// Logoff() - Records when the staff member logs off
//
//////////////////////////////////////////////////////////////////////////////////////////
function LogCommand(who, command, targ := 0, other := 0)

var timeanddate := SplitWords(NowString());
var date := timeanddate[1];
var time := timeanddate[2];
// 2026-10-25: NPCs have no account; CStr() of that error named a datafile the world save can't write (ZH2.5 1.1.5 port)
if (!who.acctname)
	return 0;
endif
var filename := CStr(who.acctname);
var file := OpenDataFile(":staff:"+filename);
if (!file)
	CreateDataFile( ":staff:"+filename, flags := DF_KEYTYPE_STRING );
	file := OpenDataFile(":staff:"+filename);	// 2026-10-25: re-open after creating, or a new account's first line was lost (as logspeech, 2026-09-22)
endif
if (!file)
	return 0;
endif

var currentelement := file.FindElement(date);

if (!currentelement)
	currentelement := file.CreateElement(date);
endif

/* if (!currentelement)
	SendSysMessage(who, "There was a fatal error on your account. You are banned until it's sorted. Contact a dev!", 3, 33);
	sleep(5);
	who.acct.ban();
	DisconnectClient(who);
endif */

var settingprop := "["+time+" - "+who.x+","+who.y+","+who.z+"]"; // set log with time date, 

command := SplitWords(command, "/");
command := command[len(command)];
var toset := "CMD: "+command;

if (targ != 0)
	toset := toset+" - TGT: "+targ;
endif

if (other != 0)
	toset := toset+" - OTHER: "+other;
endif

currentelement.SetProp(settingprop, toset);

UnloadDataFile(":staff:"+filename);

SetObjProperty(who, "#LogXYZ", {who.x, who.y, who.z});

endfunction

//////////////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// Logoff() - Records when the staff member logs off
//
//////////////////////////////////////////////////////////////////////////////////////////
function LogAction(who, action)

var timeanddate := SplitWords(NowString());
var date := timeanddate[1];
var time := timeanddate[2];
// 2026-10-25: NPCs have no account; CStr() of that error named a datafile the world save can't write (ZH2.5 1.1.5 port)
if (!who.acctname)
	return 0;
endif
var filename := CStr(who.acctname);
var file := OpenDataFile(":staff:"+filename);
if (!file)
	CreateDataFile( ":staff:"+filename, flags := DF_KEYTYPE_STRING );
	file := OpenDataFile(":staff:"+filename);	// 2026-10-25: re-open after creating, or a new account's first line was lost (as logspeech, 2026-09-22)
endif
if (!file)
	return 0;
endif

var currentelement := file.FindElement(date);

if (!currentelement)
	currentelement := file.CreateElement(date);
endif

var settingprop := "["+time+" - "+who.x+","+who.y+","+who.z+"]"; // set log with time date,

currentelement.SetProp(settingprop, action);

UnloadDataFile(":staff:"+filename);

SetObjProperty(who, "#LogXYZ", {who.x, who.y, who.z});

endfunction
