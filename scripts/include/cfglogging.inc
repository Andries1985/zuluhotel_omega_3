//This function requires the calling script to use the cfgfile escript module.
//logToCFGfile is for logging arbitrary data to an easily-findable location in pol093's outdated
//  environment.  It takes 4 arguments:  type and where should be strings, key can be whatever you
//  like, and propstruct must be an array of at least one property-value struct pair.
//sith

var logitem := array;

function logToCFGfile ( proparray, key, whichlog, eventtype )
	AppendConfigFileElem( whichlog, eventtype, key, proparray );
endfunction