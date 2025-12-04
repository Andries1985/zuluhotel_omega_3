//this file part of pkg/opt/irc by sith

//constructs and returns an event object with members Type, Who, Value, and CmdLevel
function eventset()
	var ev := struct;
	ev.+type;
	ev.+value;
	ev.+who;
	ev.+cmdlevel;
	
	return ev;
endfunction

//constructs and returns an event object with members initialized
function eventcon(who, text)
	var ev := struct;
	ev.+type;
	ev.+value;
	ev.+who;
	ev.+cmdlevel;
	
	ev.type := "ircchat";
	ev.value := text;
	ev.who := who.name;
	ev.cmdlevel := who.cmdlevel;
	
	return ev;
endfunction