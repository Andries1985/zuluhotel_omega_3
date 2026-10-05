// ipban.inc -- the IP ban list check used at login (scripts/misc/logon.src, reconnect.src) and
// by .ipban (pkg/opt/ipban/textcmd/admin/ipban.src).
//
// 2026-10-04: rewritten. The command saves bans in the datafile "bannedips" but the login
// scripts read ":ipban:bannedips", a different file, so no ban was ever found. The old check
// also only recognised a range (an address with a 255 octet): a ban on one exact address
// never matched. A banned character was also announced as arrived before it was kicked.
//
// A ban is four octets. An octet of 255 and everything after it matches any address
// ("65.5.255.255" bans 65.5.*.*, "255.255.255.255" bans everyone, as .ipban always said).
// Any other octet has to match exactly.

use uo;
use os;
use datafile;

const IPBAN_DATAFILE := "bannedips";

function IPBan_Matches( ban, address )

	var ban_octets := SplitWords( CStr( ban ), "." );
	var ip_octets := SplitWords( CStr( address ), "." );
	if( ip_octets.size() < 4 || ban_octets.size() < 1 )
		return 0;
	endif

	var pos := 1;
	while( pos <= ban_octets.size() && pos <= 4 )
		if( CInt( ban_octets[pos] ) == 255 )
			return 1;
		endif
		if( CInt( ban_octets[pos] ) != CInt( ip_octets[pos] ) )
			return 0;
		endif
		pos := pos + 1;
	endwhile

	// every octet that was given matched; a full four-octet ban is an exact match
	return ( ban_octets.size() >= 4 );

endfunction

function IPBan_IsBanned( address )

	var ipdfile := OpenDataFile( IPBAN_DATAFILE );
	if( !ipdfile )
		return 0;
	endif

	var bannedele := ipdfile.FindElement( "BannedIPS" );
	if( !bannedele )
		UnloadDataFile( IPBAN_DATAFILE );
		return 0;
	endif

	var iplist := bannedele.PropNames();
	UnloadDataFile( IPBAN_DATAFILE );

	foreach ban in iplist
		if( IPBan_Matches( ban, address ) )
			return 1;
		endif
	endforeach

	return 0;

endfunction

// Returns 1 when the character may come in. A banned address tells the online staff and
// disconnects the client, and returns 0: the caller stops before anything is announced.
function IPBan_AdmitCharacter( who )

	if( !IPBan_IsBanned( who.ip ) )
		return 1;
	endif

	foreach person in EnumerateOnlineCharacters()
		if( person.cmdlevel > 1 )
			SendSysMessage( person, "Account: " + who.acctname, 3, 53 );
			SendSysMessage( person, "Character: " + who.name, 3, 53 );
			SendSysMessage( person, "Tried logging in but is IP banned. Consider account ban?", 3, 43 );
		endif
	endforeach

	DisconnectClient( who );
	return 0;

endfunction
