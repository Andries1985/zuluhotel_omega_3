/*
        Maintened by *Edwards
        For FantasiaShard.com
        
        Edwards@FantasiaShard.com
        
        2009-06-04
        
        Last Modifications 2010-01-24
             * Changed order of param for CreateNewAccount()
             * Added signup_ip param to CreateNewAccount()

        Last Modifications 2010-03-01
             * Removed MaxAccountIPs()
             * Removed CheckForMaxClientsOnline()
*/
use uo;
use os;

include "include/security";
include ":accounts:settings";
include "include/time";
include ":datafile:datafile";

// Max numbers of characters an account can have.
CONST ACCT_MAX_CHARACTERS	:= 7;

// 2026-10-01: the login policy's numbers come from settings.cfg ("Settings" block); these
// are the fallbacks for a missing key.
CONST ACCT_DEFAULT_MAX_DISCORDS_ONLINE		:= 2;	// characters online per DiscordID
CONST ACCT_DEFAULT_MAX_ACCOUNTS_PER_DISCORD	:= 2;	// accounts one DiscordID may hold
CONST ACCT_DEFAULT_HOUSEHOLD_CAP		:= 2;	// DiscordIDs a new household lets share an IP

function ACCT_IsDebugEnabled()

	var settings := ACCT_GetSettingsCfgElem( "Settings" );
	if( settings.errortext )
		return 0;
	endif

	return CInt( settings.DebugAccountsPolicy ) ? 1 : 0;
endfunction

function ACCT_DebugPrint( message )

	if( ACCT_IsDebugEnabled() )
		var line := "[ACCTDBG] " + CStr( message );
		Print( line );
		SysLog( line );
	endif

	return 1;
endfunction

// 2026-10-01: one whole-number policy key from settings.cfg. A missing key, or a value
// under lowest, gives the fallback.
function ACCT_GetPolicySetting( key, fallback, lowest:=1 )

	var settings := ACCT_GetSettingsCfgElem( "Settings" );
	var value := GetConfigInt( settings, key );
	if( value == error )
		return fallback;
	endif

	value := CInt( value );
	if( value < lowest )
		return fallback;
	endif

	return value;
endfunction

function CreateNewAccount( username, password, discordID, pin, expansion:="" )

	// 2026-10-01: a DiscordID may hold only MaxAccountsPerDiscord accounts (settings.cfg).
	var limit_reason := ACCT_DiscordAccountLimitReason( discordID );
	if( limit_reason )
		return error{ "errortext" := limit_reason };
	endif

	var account := CreateAccount( username, password, 1 );
	if( account.errortext )
		return account;
	endif

	account := FindAccount(UserName);
	account.setprop("DiscordID", discordID);
	account.setprop("Pin", CInt(pin));
	account.setprop("Login", NowString()); // 2026-10-01: was "Login " with a trailing space, a property nothing reads
	account.setprop("Notes", "<none>");
	account.setprop("Release", 0);
	account.SetProp( "CreatedAt", NowString());

	if( expansion )
		account.Set_UO_Expansion( expansion );
	endif

	//account.SetProp( "SignupIP", signup_ip );
	ACCT_SetLastLogin( account );

	return account;
endfunction

function ACCT_SetLastLogin( account, time:=0 )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	if( !time )
		time := POLCore().systime;
	endif
    
	return account.SetProp( "LastLogin", time );
endfunction

function ACCT_GetLastLogin( account )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	return CInt( account.GetProp( "LastLogin" ));
endfunction

function ACCT_GetCharacters( account )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	var mobile_list := array;
	for i:=1 to ACCT_MAX_CHARACTERS
		var mobile := account.GetCharacter(i);
		if( mobile )
			mobile_list.Append( mobile );
		endif
		SleepMS(2);
	endfor

	return mobile_list;
endfunction

function ACCT_GetNumCharacters( account )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	var count := 0;
	for i:=1 to ACCT_MAX_CHARACTERS
		if( account.GetCharacter( i ))
			count := count+1;
		endif
		SleepMS(2);
	endfor

	return count;
endfunction

function ACCT_DeleteAllCharacters( account )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	var count := 0;
	for i:=1 to ACCT_MAX_CHARACTERS
		account.DeleteCharacter( i );
		SleepMS(2);
	endfor

	return count;
endfunction

function ACCT_DeleteAccount( account )

	if( TypeOf( account ) != "AccountRef" )
		account := FindAccount( account );
		if( account.errortext )
			return account;
		endif
	endif

	foreach package in ( POLCore().packages )
		Run_Script_To_Completion( ":"+package+":acctDelete/acctDelete", account );
		SleepMS(2);
	endforeach

	ACCT_DeleteAllCharacters( account );

	return account.Delete();
endfunction

function ACCT_TrimDiscordID( byref discord_id )

	// 2026-09-09: an error{} struct (e.g. from GetProp() on a property that
	// was never set, like an old account with no HouseholdId) is truthy --
	// !discord_id doesn't catch it, and CStr()'ing it below would return the
	// literal "error{errortext=...}" text instead of "". Checked explicitly,
	// same fix as ACCT_GetAccountDiscordInfo's DiscordID guard.
	if( discord_id == error || !discord_id )
		return "";
	endif

	discord_id := CStr( discord_id );

	while( Len( discord_id ) > 0 )
		if( discord_id[1] == " " || discord_id[1] == "\t" || discord_id[1] == "\r" || discord_id[1] == "\n" )
			discord_id[1] := "";
		else
			break;
		endif
	endwhile

	while( Len( discord_id ) > 0 )
		if( discord_id[Len( discord_id )] == " " || discord_id[Len( discord_id )] == "\t" || discord_id[Len( discord_id )] == "\r" || discord_id[Len( discord_id )] == "\n" )
			discord_id[Len( discord_id )] := "";
		else
			break;
		endif
	endwhile

	return discord_id;
endfunction

function ACCT_NormalizeDiscordID( discord_id )

	var normalized := ACCT_TrimDiscordID( discord_id );
	if( !normalized )
		return "";
	endif

	return Lower( normalized );
endfunction

function ACCT_ResolveAccountRef( account_or_who )

	if( TypeOf( account_or_who ) == "AccountRef" )
		return account_or_who;
	elseif( TypeOf( account_or_who ) == "String" )
		return FindAccount( account_or_who );
	endif

	// Character/mobile path
	return account_or_who.acct;
endfunction

function ACCT_GetAccountDiscordInfo( account_or_who )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return account;
	endif

	// 2026-09-09: accounts that predate this property entirely (e.g. the
	// original bootstrap "admin" account) return the unresolved error{}
	// struct from GetProp, not an empty string -- CStr()'d that straight
	// into "DiscordIDRaw"/"DiscordIDNorm" as literal error{errortext=...}
	// text, surfaced in :alryc:testadminpanel's account browser. Guarded the
	// same way as every other error{}-from-GetProp case in this codebase.
	var discord_prop := account.GetProp( "DiscordID" );
	var discord_raw := "";
	if( discord_prop != error )
		discord_raw := CStr( discord_prop );
		if( discord_raw == "<empty string>" )
			discord_raw := "";
		endif
	endif

	var discord_norm := ACCT_NormalizeDiscordID( discord_raw );
	var household_id := ACCT_TrimDiscordID( account.GetProp( "HouseholdId" ));

	return dictionary
	{
		"AccountName" -> account.name,
		"DiscordIDRaw" -> discord_raw,
		"DiscordIDNorm" -> discord_norm,
		"HouseholdId" -> household_id
	};
endfunction

function ACCT_AccountHasStaffAccess( account_or_who )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return 0;
	endif

	// 2026-10-01: DefaultCmdLevel is a built-in account field holding a number, not a custom
	// property -- account.GetProp( "DefaultCmdLevel" ) never found anything, so this test
	// never matched. Only a real number counts, so nothing unexpected can mark everyone staff.
	var default_cmdlevel := account.defaultcmdlevel;
	if( TypeOf( default_cmdlevel ) == "Integer" )
		if( default_cmdlevel > 0 )
			return 1;
		endif
	endif

	for i := 1 to ACCT_MAX_CHARACTERS
		var chr := account.GetCharacter( i );
		if( !chr )
			continue;
		endif

		if( ACCT_IsStaffCmdLevel( chr.cmdlevel ))
			return 1;
		endif
		SleepMS(2);
	endfor

	return 0;
endfunction

function ACCT_IsStaffCmdLevel( cmdlevel )

	if( TypeOf( cmdlevel ) == "Error" )
		return 0;
	endif

	if( TypeOf( cmdlevel ) == "Integer" )
		return ( CInt( cmdlevel ) > 0 ) ? 1 : 0;
	endif

	cmdlevel := Lower( ACCT_TrimDiscordID( CStr( cmdlevel )));
	if( !cmdlevel || cmdlevel == "0" || cmdlevel == "player" )
		return 0;
	endif

	if( cmdlevel == "error" || cmdlevel == "error{}" )
		return 0;
	endif

	if( cmdlevel == "none" || cmdlevel == "<empty string>" || cmdlevel == "property not found" )
		return 0;
	endif

	if( cmdlevel == "error{ errortext = \"property not found\" }" )
		return 0;
	endif

	return 1;
endfunction

function ACCT_IsStaffCharacter( who )

	if( !who )
		return 0;
	endif

	if( ACCT_IsStaffCmdLevel( who.cmdlevel ))
		return 1;
	endif

	return ACCT_AccountHasStaffAccess( who.acct );
endfunction

function ACCT_GetHouseholdStore( create_if_missing:=1 )

	var store;
	if( create_if_missing )
		store := DFOpenDataFile( "AccountsHouseholds", DF_CREATE );
	else
		store := DFOpenDataFile( "AccountsHouseholds", DF_NO_CREATE );
	endif

	// Unloading here (before the caller ever uses the returned handle)
	// inverted the "open -> use -> unload last" convention every other
	// datafile consumer in this repo follows. Callers now unload it
	// themselves once they're done with it.
	return store;
endfunction

function ACCT_GetHouseholdById( household_id, create_if_missing:=0 )

	household_id := ACCT_TrimDiscordID( household_id );
	if( !household_id )
		return dictionary{ "errortext" -> "Missing household id." };
	endif

	var store := ACCT_GetHouseholdStore( create_if_missing );
	if( store.errortext )
		return store;
	endif

	var elem;
	if( create_if_missing )
		elem := DFFindElement( store, household_id, DF_CREATE );
	else
		elem := DFFindElement( store, household_id, DF_NO_CREATE );
	endif
	UnloadDataFile( "AccountsHouseholds" );
	return elem;
endfunction

function ACCT_HouseholdContainsDiscord( household_id, discord_norm )

	household_id := ACCT_TrimDiscordID( household_id );
	discord_norm := ACCT_NormalizeDiscordID( discord_norm );

	if( !household_id || !discord_norm )
		return 0;
	endif

	foreach acct_name in ( ListAccounts() )
		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var account_household := ACCT_TrimDiscordID( account.GetProp( "HouseholdId" ));
		if( account_household != household_id )
			continue;
		endif

		var account_discord_norm := ACCT_NormalizeDiscordID( account.GetProp( "DiscordID" ));
		if( account_discord_norm == discord_norm )
			return 1;
		endif

		SleepMS(2);
	endforeach

	return 0;
endfunction

function ACCT_FindDiscordHouseholdConflict( discord_norm, desired_household_id )

	discord_norm := ACCT_NormalizeDiscordID( discord_norm );
	desired_household_id := ACCT_TrimDiscordID( desired_household_id );

	if( !discord_norm || !desired_household_id )
		return 0;
	endif

	foreach acct_name in ( ListAccounts() )
		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var account_discord_norm := ACCT_NormalizeDiscordID( account.GetProp( "DiscordID" ));
		if( account_discord_norm != discord_norm )
			continue;
		endif

		var account_household := ACCT_TrimDiscordID( account.GetProp( "HouseholdId" ));
		if( !account_household )
			continue;
		endif

		if( account_household != desired_household_id )
			return dictionary
			{
				"ConflictHouseholdId" -> account_household,
				"AccountName" -> account.name
			};
		endif
	endforeach

	return 0;
endfunction

function ACCT_ConfirmDiscordHouseholdReassign( who, discord_norm, desired_household_id, context:="accounts", target_account_name:="" )

	var conflict := ACCT_FindDiscordHouseholdConflict( discord_norm, desired_household_id );
	if( !conflict )
		return 1;
	endif

	var from_household := CStr( conflict["ConflictHouseholdId"] );
	SendSysMessage( who, "WARNING: Discord " + discord_norm + " already exists in household " + from_household + ".", 3, 33 );

	var confirm := SendTextEntryGump( who, "Type YES to confirm reassignment:", TE_CANCEL_ENABLE, TE_STYLE_NORMAL, 8 );
	confirm := Lower( ACCT_TrimDiscordID( confirm ));
	if( confirm != "yes" )
		ACCT_DebugPrint( context + " reassignment denied for " + CStr( target_account_name ) + " from " + from_household + " to " + desired_household_id );
		SendSysMessage( who, "Reassignment cancelled.", 3, 33 );
		return 0;
	endif

	ACCT_DebugPrint( context + " reassignment confirmed for " + CStr( target_account_name ) + " from " + from_household + " to " + desired_household_id );
	return 1;
endfunction

function ACCT_UpsertHouseholdKnownIP( household_id, ip )

	household_id := ACCT_TrimDiscordID( household_id );
	ip := ACCT_TrimDiscordID( ip );

	if( !household_id || !ip )
		return dictionary{ "errortext" -> "Missing household id or ip." };
	endif

	var household := ACCT_GetHouseholdById( household_id, 1 );
	if( household.errortext )
		return household;
	endif

	var known_ips := household.GetProp( "KnownIPs" );
	if( TypeOf( known_ips ) != "Array" )
		known_ips := array;
	endif

	if( !known_ips.contains( ip ))
		known_ips.append( ip );
		household.SetProp( "KnownIPs", known_ips );
		household.SetProp( "UpdatedAt", NowString() );
	endif

	return 1;
endfunction

function ACCT_AssignAccountToHousehold( account_or_who, household_id, actor_name:="" )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return account;
	endif

	household_id := ACCT_TrimDiscordID( household_id );
	if( !household_id )
		return dictionary{ "errortext" -> "Missing household id." };
	endif

	var info := ACCT_GetAccountDiscordInfo( account );
	if( info.errortext )
		return info;
	endif

	var discord_norm := info["DiscordIDNorm"];
	if( !discord_norm )
		return dictionary{ "errortext" -> "Account has no DiscordID." };
	endif

	var household := ACCT_GetHouseholdById( household_id, 1 );
	if( household.errortext )
		return household;
	endif

	if( CInt( household.GetProp( "HouseholdCap" )) < 1 )
		// 2026-10-01: a new household lets two players share an IP (was 1, which let nobody).
		household.SetProp( "HouseholdCap", ACCT_GetPolicySetting( "DefaultHouseholdCap", ACCT_DEFAULT_HOUSEHOLD_CAP ));
	endif
	if( actor_name )
		household.SetProp( "UpdatedBy", actor_name );
	else
		household.SetProp( "UpdatedBy", "system" );
	endif

	var previous_household := ACCT_TrimDiscordID( info["HouseholdId"] );

	account.SetProp( "DiscordIDNorm", discord_norm );
	account.SetProp( "HouseholdId", household_id );

	// 2026-10-01: the member list is rebuilt from the accounts, so it cannot drift from
	// them; a household this account just left is rebuilt too.
	var members := ACCT_SyncHouseholdMembers( household_id );
	if( previous_household )
		if( previous_household != household_id )
			ACCT_SyncHouseholdMembers( previous_household );
		endif
	endif

	return dictionary
	{
		"HouseholdId" -> household_id,
		"DiscordIDNorm" -> discord_norm,
		"MemberCount" -> members.size(),
		"HouseholdCap" -> CInt( household.GetProp( "HouseholdCap" ))
	};
endfunction

function ACCT_RemoveAccountFromHousehold( account_or_who, actor_name:="" )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return account;
	endif

	var info := ACCT_GetAccountDiscordInfo( account );
	if( info.errortext )
		return info;
	endif

	var household_id := ACCT_TrimDiscordID( info["HouseholdId"] );
	if( !household_id )
		return dictionary{ "errortext" -> "Account is not in a household." };
	endif

	var discord_norm := info["DiscordIDNorm"];
	account.EraseProp( "HouseholdId" );

	// 2026-10-01: the member list is rebuilt from the accounts still in the household.
	// Erasing this DiscordID from it was wrong while the player's other account stayed in.
	var members := ACCT_SyncHouseholdMembers( household_id );

	var household := ACCT_GetHouseholdById( household_id, 0 );
	if( household )
		if( !household.errortext )
			if( actor_name )
				household.SetProp( "UpdatedBy", actor_name );
			else
				household.SetProp( "UpdatedBy", "system" );
			endif
		endif
	endif

	return dictionary
	{
		"HouseholdId" -> household_id,
		"DiscordIDNorm" -> discord_norm,
		"MemberCount" -> members.size()
	};
endfunction

// 2026-10-01: rebuilds a household's member list (DiscordIDs) from the accounts that carry
// its id, so the record cannot drift from the accounts. Returns the list.
function ACCT_SyncHouseholdMembers( household_id )

	var member_set := dictionary{};

	household_id := ACCT_TrimDiscordID( household_id );
	if( !household_id )
		return array;
	endif

	var acct_names := ListAccounts();
	foreach acct_name in acct_names
		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var info := ACCT_GetAccountDiscordInfo( account );
		if( info.errortext )
			continue;
		endif

		if( info["HouseholdId"] != household_id )
			continue;
		endif

		var member_discord := info["DiscordIDNorm"];
		if( member_discord )
			member_set[member_discord] := 1;
		endif
	endforeach

	// keys(), not a foreach over the dictionary: that would hand back the values.
	var members := member_set.keys();

	var household := ACCT_GetHouseholdById( household_id, 0 );
	if( household )
		if( !household.errortext )
			household.SetProp( "Members", members );
		endif
	endif

	return members;
endfunction

// 2026-10-01: names of the accounts whose DiscordID is discord_norm, leaving out
// ignore_account_name.
function ACCT_ListAccountsWithDiscord( discord_norm, ignore_account_name:="" )

	var holder_names := array;

	discord_norm := ACCT_NormalizeDiscordID( discord_norm );
	if( !discord_norm )
		return holder_names;
	endif

	ignore_account_name := Lower( CStr( ignore_account_name ));

	var acct_names := ListAccounts();
	foreach acct_name in acct_names
		if( Lower( CStr( acct_name )) == ignore_account_name )
			continue;
		endif

		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var info := ACCT_GetAccountDiscordInfo( account );
		if( info.errortext )
			continue;
		endif

		if( info["DiscordIDNorm"] == discord_norm )
			holder_names.append( CStr( acct_name ));
		endif
	endforeach

	return holder_names;
endfunction

// 2026-10-01: "" when discord_id may take one more account, otherwise the reason it may
// not. The limit is MaxAccountsPerDiscord in settings.cfg; 0 there switches it off.
function ACCT_DiscordAccountLimitReason( discord_id, ignore_account_name:="" )

	var discord_norm := ACCT_NormalizeDiscordID( discord_id );
	if( !discord_norm )
		return "";
	endif

	var max_accounts := ACCT_GetPolicySetting( "MaxAccountsPerDiscord", ACCT_DEFAULT_MAX_ACCOUNTS_PER_DISCORD, 0 );
	if( max_accounts < 1 )
		return "";
	endif

	var holder_names := ACCT_ListAccountsWithDiscord( discord_norm, ignore_account_name );
	if( holder_names.size() < max_accounts )
		return "";
	endif

	var name_list := "";
	foreach holder_name in holder_names
		if( name_list )
			name_list := name_list + ", ";
		endif
		name_list := name_list + holder_name;
	endforeach

	return "DiscordID " + discord_norm + " already has " + CStr( holder_names.size() ) + " account(s): " + name_list + " (limit " + CStr( max_accounts ) + ").";
endfunction

// 2026-10-01: sets or changes the DiscordID of an existing account (.setdiscord). The
// account stays in its household, and that household's member list follows.
function ACCT_SetAccountDiscordID( account_or_who, new_discord_id )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return account;
	endif

	var new_raw := ACCT_TrimDiscordID( new_discord_id );
	var new_norm := ACCT_NormalizeDiscordID( new_raw );
	if( !new_norm )
		return dictionary{ "errortext" -> "Missing DiscordID." };
	endif

	var info := ACCT_GetAccountDiscordInfo( account );
	if( info.errortext )
		return info;
	endif

	var old_norm := info["DiscordIDNorm"];
	if( old_norm == new_norm )
		return dictionary{ "errortext" -> "Account " + account.name + " already has DiscordID " + new_norm + "." };
	endif

	var limit_reason := ACCT_DiscordAccountLimitReason( new_norm, account.name );
	if( limit_reason )
		return dictionary{ "errortext" -> limit_reason };
	endif

	account.SetProp( "DiscordID", new_raw );

	var household_id := ACCT_TrimDiscordID( info["HouseholdId"] );
	if( household_id )
		account.SetProp( "DiscordIDNorm", new_norm );
		ACCT_SyncHouseholdMembers( household_id );
	endif

	return dictionary
	{
		"AccountName" -> account.name,
		"OldDiscordIDNorm" -> old_norm,
		"DiscordIDNorm" -> new_norm,
		"HouseholdId" -> household_id
	};
endfunction

function ACCT_CountOnlineByDiscordAndIP( include_staff:=0 )

	// Build the three indexes as flat locals first. In eScript, reading a
	// nested dictionary out of an outer dictionary returns a value that does
	// not write back when mutated, so we must accumulate locally and assemble
	// the snapshot at the end.
	var ip_to_discords := dictionary{};
	var discord_to_ips := dictionary{};
	var discord_char_count := dictionary{};

	foreach player in EnumerateOnlineCharacters()
		if( !include_staff && ACCT_IsStaffCharacter( player ))
			continue;
		endif

		var account_info := ACCT_GetAccountDiscordInfo( player );
		if( account_info.errortext )
			continue;
		endif

		var discord_norm := account_info["DiscordIDNorm"];
		if( !discord_norm )
			continue;
		endif

		var player_ip := CStr( player.ip );

		var ip_discords := array;
		if( ip_to_discords.exists( player_ip ))
			ip_discords := ip_to_discords[player_ip];
		endif
		if( !ip_discords.contains( discord_norm ))
			ip_discords.append( discord_norm );
		endif
		ip_to_discords[player_ip] := ip_discords;

		var discord_ips := array;
		if( discord_to_ips.exists( discord_norm ))
			discord_ips := discord_to_ips[discord_norm];
		endif
		if( !discord_ips.contains( player_ip ))
			discord_ips.append( player_ip );
		endif
		discord_to_ips[discord_norm] := discord_ips;

		var current_count := 0;
		if( discord_char_count.exists( discord_norm ))
			current_count := CInt( discord_char_count[discord_norm] );
		endif
		discord_char_count[discord_norm] := current_count + 1;

		SleepMS(2);
	endforeach

	var snapshot := dictionary{};
	snapshot["IPToDiscords"] := ip_to_discords;
	snapshot["DiscordToIPs"] := discord_to_ips;
	snapshot["DiscordCharCount"] := discord_char_count;
	return snapshot;
endfunction

function ACCT_IsCharacterCurrentlyOnline( character )

	if( !character )
		return 0;
	endif

	foreach online_char in EnumerateOnlineCharacters()
		if( online_char.serial == character.serial )
			return 1;
		endif
		SleepMS(2);
	endforeach

	return 0;
endfunction

// Login screen (packet 0x80, hook/onLogin.src). 2026-10-01: refuses only the certain case --
// the DiscordID already has its full number of characters online and none of them is on
// this account, so this login could only add one more. Everything else is decided on
// entering the world (ACCT_CheckForMaxClientsOnline_NewEngine) from the characters actually
// online. The 3-minute "pending login" stamp this used to set is gone: it fed the
// login-screen address into the one-IP rule and held a slot after an abandoned login.
function ACCT_CheckAccountLoginPolicy( account_or_who )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return 0;
	endif

	if( ACCT_AccountHasStaffAccess( account ))
		ACCT_DebugPrint( "Allow login for staff account " + account.name );
		return 1;
	endif

	var info := ACCT_GetAccountDiscordInfo( account );
	if( info.errortext )
		return 0;
	endif

	var discord_norm := info["DiscordIDNorm"];
	if( !discord_norm )
		return 0;
	endif

	var account_name := Lower( CStr( account.name ));
	var live_online_count := 0;
	var account_online := 0;

	var online_chars := EnumerateOnlineCharacters();
	foreach player in online_chars
		if( ACCT_IsStaffCharacter( player ))
			continue;
		endif

		var player_info := ACCT_GetAccountDiscordInfo( player );
		if( player_info.errortext )
			continue;
		endif

		if( player_info["DiscordIDNorm"] != discord_norm )
			continue;
		endif

		live_online_count := live_online_count + 1;
		if( Lower( CStr( player_info["AccountName"] )) == account_name )
			account_online := 1;
		endif
	endforeach

	var max_discords_online := ACCT_GetPolicySetting( "MaxDiscordsOnline", ACCT_DEFAULT_MAX_DISCORDS_ONLINE );

	ACCT_DebugPrint( "Login screen counts for " + account.name + " discord=" + discord_norm + " online=" + CStr( live_online_count ) + " account_online=" + CStr( account_online ) + " cap=" + CStr( max_discords_online ));

	if( live_online_count >= max_discords_online )
		// With a character of this account online the login may be a reconnect to it,
		// which adds nothing; the world-entry check sorts that out.
		if( !account_online )
			ACCT_DebugPrint( "Login screen refusal for " + account.name + ": discord=" + discord_norm + " already has " + CStr( live_online_count ) + " character(s) online on its other account." );
			return 0;
		endif
	endif

	return 1;
endfunction

// Entering the world (scripts/misc/logon.src and reconnect.src, through ACCT_AdmitCharacter).
// The rules, per DiscordID: staff are exempt and never counted; at most MaxDiscordsOnline
// characters online; all of them from one IP; and no other DiscordID on that IP unless every
// DiscordID there has an account in this account's household.
function ACCT_CheckForMaxClientsOnline_NewEngine( who )

	if( ACCT_IsStaffCharacter( who ))
		ACCT_DebugPrint( "Allow login for staff character " + who.name );
		return 1;
	endif

	var account_info := ACCT_GetAccountDiscordInfo( who );
	if( account_info.errortext )
		ACCT_DebugPrint( "Disconnecting: unable to resolve account info for " + who.name );
		SysLog( "LoginAccount() Error: unable to resolve account info for " + who.name );
		return 0;
	endif

	var discord_norm := account_info["DiscordIDNorm"];
	if( !discord_norm )
		ACCT_DebugPrint( "Disconnecting: missing DiscordID for " + who.name );
		SysLog( "LoginAccount() Error: missing DiscordID for " + who.name );
		return 0;
	endif

	var household_id := account_info["HouseholdId"];
	var player_ip := CStr( who.ip );

	// 2026-10-01: judged only from the characters that are online right now. The logon
	// scripts run one login at a time, start to finish, so this snapshot is exact. The
	// "pending login" stamps that used to be mixed in carried the login-screen address
	// (127.0.0.1 against the LAN address on the host machine) and tripped the one-IP rule.
	var snapshot := ACCT_CountOnlineByDiscordAndIP();
	if( snapshot.errortext )
		ACCT_DebugPrint( "Disconnecting: unable to build online snapshot." );
		SysLog( "LoginAccount() Error: unable to build online snapshot." );
		return 0;
	endif

	var ip_to_discords := snapshot["IPToDiscords"];
	var discord_to_ips := snapshot["DiscordToIPs"];
	var discord_char_count := snapshot["DiscordCharCount"];

	// The engine attaches the character to its client before the logon script runs, so it
	// is normally in the snapshot already; it is added here if it is not.
	var already_online := ACCT_IsCharacterCurrentlyOnline( who );

	if( !already_online )
		if( !ip_to_discords.exists( player_ip ))
			ip_to_discords.insert( player_ip, array );
		endif
		var ip_discords_add := ip_to_discords[player_ip];
		if( !ip_discords_add.contains( discord_norm ))
			ip_discords_add.append( discord_norm );
		endif
		ip_to_discords.insert( player_ip, ip_discords_add );

		if( !discord_to_ips.exists( discord_norm ))
			discord_to_ips.insert( discord_norm, array );
		endif
		var discord_ips_add := discord_to_ips[discord_norm];
		if( !discord_ips_add.contains( player_ip ))
			discord_ips_add.append( player_ip );
		endif
		discord_to_ips.insert( discord_norm, discord_ips_add );

		if( !discord_char_count.exists( discord_norm ))
			discord_char_count.insert( discord_norm, 0 );
		endif
		discord_char_count.insert( discord_norm, discord_char_count[discord_norm] + 1 );
	endif

	// Use a dict-as-set keyed by CStr(ip) for true value-equality dedup;
	// Array.contains has been observed to miss duplicates when player.ip
	// returns instances that don't compare equal as Array elements.
	var discord_ip_set := dictionary{};
	if( discord_to_ips.exists( discord_norm ))
		var existing_ips := discord_to_ips[discord_norm];
		foreach existing_ip in ( existing_ips )
			discord_ip_set[CStr( existing_ip )] := 1;
		endforeach
	endif
	// keys(), not a foreach over the dictionary: that would hand back the values.
	var discord_ips := discord_ip_set.keys();

	var discord_online_count := 0;
	if( discord_char_count.exists( discord_norm ))
		discord_online_count := CInt( discord_char_count[discord_norm] );
	endif

	ACCT_DebugPrint( "Policy counts for " + who.name + " discord=" + discord_norm + " ip=" + player_ip + " online_count=" + CStr( discord_online_count ) + " ips=" + CStr( discord_ips ));

	var max_discords_online := ACCT_GetPolicySetting( "MaxDiscordsOnline", ACCT_DEFAULT_MAX_DISCORDS_ONLINE );

	// Same DiscordID may not appear from more than one IP at a time.
	if( discord_ips.size() > 1 )
		ACCT_DebugPrint( "Disconnecting: DiscordID " + discord_norm + " is active from multiple IPs (" + CStr( discord_ips ) + ")." );
		SysLog( "LoginAccount() Error: discord active from multiple IPs for " + who.name );
		return 0;
	endif

	if( discord_online_count > max_discords_online )
		ACCT_DebugPrint( "Disconnecting: DiscordID " + discord_norm + " exceeded " + CStr( max_discords_online ) + " online chars." );
		SysLog( "LoginAccount() Error: discord exceeded cap for " + who.name );
		return 0;
	endif

	if( !ip_to_discords.exists( player_ip ))
		ACCT_DebugPrint( "Allow login for " + who.name + " discord=" + discord_norm + " ip=" + player_ip + " (first discord on ip snapshot)" );
		return 1;
	endif

	var ip_discords := ip_to_discords[player_ip];

	// Dedup by value-equality (Array.contains is unreliable here, same gotcha
	// as the IP set above). Count unique DiscordIDs sharing this IP.
	var ip_discord_set := dictionary{};
	foreach ip_discord in ( ip_discords )
		ip_discord_set[CStr( ip_discord )] := 1;
	endforeach
	var ip_discord_list := ip_discord_set.keys();
	var ip_unique_discord_count := ip_discord_list.size();

	if( !household_id )
		if( ip_unique_discord_count > 1 )
			ACCT_DebugPrint( "Disconnecting: non-household account cannot share IP with another DiscordID (" + CStr( ip_discord_list ) + ")." );
			SysLog( "LoginAccount() Error: non-household IP share denied for " + who.name );
			return 0;
		endif
		ACCT_DebugPrint( "Allow login for " + who.name + " discord=" + discord_norm + " ip=" + player_ip + " (non-household single-discord ip)" );
		return 1;
	endif

	// 2026-10-01: a household with no record, or no cap on its record, gets the default
	// cap. (The old "household record missing" refusal could never fire: a missing record
	// comes back as 0, not as an error.)
	var household_cap := 0;
	var household := ACCT_GetHouseholdById( household_id, 0 );
	if( household )
		if( !household.errortext )
			household_cap := CInt( household.GetProp( "HouseholdCap" ));
		endif
	endif
	if( household_cap < 1 )
		household_cap := ACCT_GetPolicySetting( "DefaultHouseholdCap", ACCT_DEFAULT_HOUSEHOLD_CAP );
	endif

	if( ip_unique_discord_count > household_cap )
		ACCT_DebugPrint( "Disconnecting: household cap exceeded for " + who.name + " cap=" + CStr( household_cap ) + " discords on ip=" + CStr( ip_discord_list ) + "." );
		SysLog( "LoginAccount() Error: household cap exceeded for " + who.name );
		return 0;
	endif

	// 2026-10-01: walk the DiscordIDs themselves. This loop used to run over the set
	// dictionary, which hands back its values (all 1), so no household member ever passed.
	foreach ip_discord in ( ip_discord_list )
		if( !ACCT_HouseholdContainsDiscord( household_id, ip_discord ))
			ACCT_DebugPrint( "Disconnecting: mixed-household IP use denied for " + who.name + ": discord=" + CStr( ip_discord ) + " has no account in household " + CStr( household_id ) + "." );
			SysLog( "LoginAccount() Error: mixed-household IP use denied for " + who.name );
			return 0;
		endif
		SleepMS(2);
	endforeach

	var known_ip_result := ACCT_UpsertHouseholdKnownIP( household_id, player_ip );
	if( known_ip_result.errortext )
		ACCT_DebugPrint( "Household KnownIP update warning: " + known_ip_result.errortext );
	endif

	ACCT_DebugPrint( "Allow login for " + who.name + " discord=" + discord_norm + " household=" + CStr( household_id ) + " ip=" + player_ip );

	return 1;
endfunction

function CheckForMaxClientsOnline( who )

	// Phase 2 cutover: new engine is authoritative.
	return ACCT_CheckForMaxClientsOnline_NewEngine( who );
endfunction

// 2026-10-01: the world-entry gate. scripts/misc/logon.src and reconnect.src call it first
// thing, so a refused character is disconnected before it is announced or any player script
// starts (the check used to run later, from this package's own logon.src). Returns 1 when
// the character may stay.
function ACCT_AdmitCharacter( who )

	if( CheckForMaxClientsOnline( who ))
		return 1;
	endif

	SysLog( "LoginAccount() Error: " + who.name + " (account " + who.acctname + ") refused on entering the world, client disconnected." );
	DisconnectClient( who );
	return 0;
endfunction

// 2026-10-01: staff commands take an account name so they work on an offline account; with
// no name they fall back to targeting a character. Returns the AccountRef, or 0 after
// telling the staff member what went wrong. picked_mobile comes back as the targeted
// character, or 0 when the account was named.
function ACCT_PickAccountForCommand( who, account_name, prompt, byref picked_mobile )

	picked_mobile := 0;

	account_name := ACCT_TrimDiscordID( account_name );
	if( account_name )
		var named_account := FindAccount( account_name );
		if( !named_account )
			SendSysMessage( who, "There is no account named " + account_name + "." );
			return 0;
		endif
		return named_account;
	endif

	SendSysMessage( who, prompt );
	var targetted := Target( who );
	if( !targetted )
		SendSysMessage( who, "Cancelled." );
		return 0;
	endif
	if( !targetted.acctname )
		SendSysMessage( who, "That is not a player character." );
		return 0;
	endif

	var targetted_account := FindAccount( targetted.acctname );
	if( !targetted_account )
		SendSysMessage( who, "Unable to resolve account." );
		return 0;
	endif

	picked_mobile := targetted;
	return targetted_account;
endfunction
