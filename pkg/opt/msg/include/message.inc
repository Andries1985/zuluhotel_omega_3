use uo;
use os;
use datafile;

function SendMessage(sender, receiver, msg := 0)


var recmsg := GetObjProperty(receiver, "#Messaging");
var sendmsg := GetObjProperty(sender, "#Messaging");

if ( (sendmsg[2]+2) > ReadGameClock() )
	SendSysmessage(sender, "You can only send messages once every 2 seconds.");
	return 0;
endif

if (!msg)
	msg := RequestInput(sender, sender.backpack, "What message would you like to send to "+receiver.name+"?");
endif

if (!msg)
	SendSysMessage(sender, "Canceled.");
	return 0;
endif

if (sendmsg[1] == "off")
	SendSysMessage(sender, "You currently have messages turned off.");
	return 0;
elseif (recmsg[1] == "off")
	SendSysMessage(sender, receiver.name+" has messaging turned off.");
	return 0;
endif

if (receiver.connected)
	SendSysMessage(receiver, sender.name+" : "+msg, 3, 18);
	SendSysMessage(sender, "Message sent to "+receiver.name+".");
	RecordMessage(sender, receiver, msg);
	sendmsg[2] := ReadGameClock();
	SetObjProperty(sender, "#Messaging", sendmsg);
else
	SendSysMessage(sender, receiver.name+" is offline, canceled.");
	return 0;
endif

endfunction

function RecordMessage(who, towho, msg)

var timeanddate := SplitWords(NowString());
var date := timeanddate[1];
var time := timeanddate[2];
var entryname := CStr(who.acctname+"_"+who.serial);
var file := OpenDataFile(":msg:messages");

if (!file)
	CreateDataFile( ":msg:messages", flags := DF_KEYTYPE_STRING );
endif

var currentelement := file.FindElement(entryname);

if (!currentelement)
	currentelement := file.CreateElement(entryname);
endif

var entries := currentelement.PropNames();

var number := len(entries);

currentelement.SetProp(CStr(number), {who.serial, date+" "+time, "Sent", towho.name, msg});

 // status, lasttimesent, lastmsgreceivedfrom
var  messaging := GetObjProperty(who, "#Messaging");
messaging[2] := ReadGameClock();
SetObjProperty(who, "#Messaging", messaging);

// The receivers logging
entryname := CStr(towho.acctname+"_"+towho.serial);

currentelement := file.FindElement(entryname);

if (!currentelement)
	currentelement := file.CreateElement(entryname);
endif

entries := currentelement.PropNames();

number := len(entries);
	
currentelement.SetProp(CStr(number), {towho.serial, date+" "+time, "From", who.name, msg});

messaging := GetObjProperty(towho, "#Messaging");
if (!messaging)
	messaging := {"on", 0, who.serial};
else
	messaging[3] := who.serial;
endif
SetObjProperty(towho, "#Messaging", messaging);

if (!GetObjProperty(towho, "#MessageAlert"))
	Start_Script(":msg:msgalert", towho);
endif

UnloadDataFile(":msg:messages");

endfunction
	