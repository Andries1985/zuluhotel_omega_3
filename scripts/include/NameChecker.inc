use uo;
use os;

include ":gumps:include/gumps";
include ":gumps:include/gumps_ex";

var blacklist := {"developer", "admin", "gm", "cn", "dev", "seer", "coun", "counselor", "game master", "gamemaster", "administrator", "dev"};
var bLTowns := {"vesper", "minoc", "ocllo", "occlo", "cove", "jhelom", "nujel'm", "britain", "trinsic", "moonglow", "elven town", "nujelm", "magincia", "buccaneer's den", "serpent's hold", "skara brae", "yew", "buccaneers den", "serpents hold"};
var chars := {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "};

var MAX_LENGTH := 19;
var MIN_LENGTH := 3;

Set_Script_Option(SCRIPTOPT_NO_RUNAWAY, 1);
// This checks proposed names as well as current names on each log in.
// who is not passed on proposed names, but is on log in to check 
// another name doesn't exist
// Staff don't have the check, and arn't checked against
function CheckName(name, who := 0)

if (who.cmdlevel > 0)
	return 1;
endif

if(GetObjProperty( who, "IsRPer" ) || GetObjProperty( who, "WasRPer" ))
	return 1;
endif

if (len(name) > MAX_LENGTH && !GetObjProperty(who, "town"))
	return {0, "too long"};
elseif (len(name) < MIN_LENGTH)
	return {0, "too short"};
endif

foreach thing in blacklist
	if (find(lower(name), thing, 0))
		return {0, "blacklist"};
	endif
endforeach

if(!GetObjProperty(who, "town"))
	foreach thing in bLTowns
		if (find(lower(name), thing, 0))
			return {0, "bLTowns"};
		endif
	endforeach
endif

var valid := 1;

var i := 1;
repeat

if (lower(name[i]) in chars)
	valid := 1;
else
	valid := 0;

endif
i := i + 1;
until (i > len(name) || valid == 0);

if (valid == 0)
	return {0, "illegal characters"};
endif;

var accounts := ListAccounts();
i := 1;
var unique := 1;

foreach account in accounts
	account := FindAccount(account);
		repeat
			if (name == account.getcharacter(i).name)
				// this will always be true if who is not passed.  If who is NOT passed it a name
				// that player want to CHANGE his name to.  But obviously it exists
				if (who.serial != account.getcharacter(i).serial)
					unique := 0;
					break;
				endif
			endif
			i := i + 1;
		until(i >= 6);
		i := 1;
endforeach

if (unique == 0)
	return {0, "duplicate"};
endif

return 1;

endfunction