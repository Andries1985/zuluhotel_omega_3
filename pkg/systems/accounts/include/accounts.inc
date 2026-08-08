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

function CreateNewAccount( username, password, discordID, pin, expansion:="" )

	var account := CreateAccount( username, password, 1 );
	if( account.errortext )
		return account;
	endif

	account := FindAccount(UserName);
	account.setprop("DiscordID", discordID);
	account.setprop("Pin", CInt(pin));
	account.setprop("Login ", NowString());
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

	if( !discord_id )
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

	var discord_raw := CStr( account.GetProp( "DiscordID" ));
	if( discord_raw == "<empty string>" )
		discord_raw := "";
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

	var default_cmdlevel := account.GetProp( "DefaultCmdLevel" );
	if( ACCT_IsStaffCmdLevel( default_cmdlevel ))
		return 1;
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

	UnloadDataFile( "AccountsHouseholds" );
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

	if( create_if_missing )
		return DFFindElement( store, household_id, DF_CREATE );
	endif

	return DFFindElement( store, household_id, DF_NO_CREATE );
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

function ACCT_GetHouseholdForDiscord( discord_norm )

	discord_norm := ACCT_NormalizeDiscordID( discord_norm );
	if( !discord_norm )
		return dictionary{ "errortext" -> "Missing discord id." };
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

		var household_id := ACCT_TrimDiscordID( account.GetProp( "HouseholdId" ));
		if( !household_id )
			return dictionary{ "errortext" -> "No household assigned." };
		endif

		return ACCT_GetHouseholdById( household_id, 0 );
	endforeach

	return dictionary{ "errortext" -> "Discord id not found in accounts." };
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

	var members := household.GetProp( "Members" );
	if( TypeOf( members ) != "Array" )
		members := array;
	endif
	if( !members.contains( discord_norm ))
		members.append( discord_norm );
	endif

	household.SetProp( "Members", members );
	if( CInt( household.GetProp( "HouseholdCap" )) < 1 )
		household.SetProp( "HouseholdCap", 1 );
	endif
	if( actor_name )
		household.SetProp( "UpdatedBy", actor_name );
	else
		household.SetProp( "UpdatedBy", "system" );
	endif

	account.SetProp( "DiscordIDNorm", discord_norm );
	account.SetProp( "HouseholdId", household_id );

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
	var household := ACCT_GetHouseholdById( household_id, 0 );
	if( household.errortext )
		account.EraseProp( "HouseholdId" );
		return dictionary
		{
			"HouseholdId" -> household_id,
			"DiscordIDNorm" -> discord_norm,
			"MemberCount" -> 0
		};
	endif

	var members := household.GetProp( "Members" );
	if( TypeOf( members ) != "Array" )
		members := array;
	endif

	for i := 1 to members.size()
		if( members[i] == discord_norm )
			members.erase( i );
			break;
		endif
	endfor

	household.SetProp( "Members", members );
	if( actor_name )
		household.SetProp( "UpdatedBy", actor_name );
	else
		household.SetProp( "UpdatedBy", "system" );
	endif
	account.EraseProp( "HouseholdId" );

	return dictionary
	{
		"HouseholdId" -> household_id,
		"DiscordIDNorm" -> discord_norm,
		"MemberCount" -> members.size()
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

function ACCT_IsCharacterSerialCurrentlyOnline( serial )

	if( !serial )
		return 0;
	endif

	foreach online_char in EnumerateOnlineCharacters()
		if( online_char.serial == serial )
			return 1;
		endif
		SleepMS(2);
	endforeach

	return 0;
endfunction

function ACCT_IsAccountCurrentlyOnline( account_name )

	account_name := Lower( ACCT_TrimDiscordID( account_name ));
	if( !account_name )
		return 0;
	endif

	foreach online_char in EnumerateOnlineCharacters()
		if( Lower( CStr( online_char.acct.name )) == account_name )
			return 1;
		endif
		SleepMS(2);
	endforeach

	return 0;
endfunction

function ACCT_ClearPendingLoginReservation( account_or_who )

	var account := ACCT_ResolveAccountRef( account_or_who );
	if( account.errortext )
		return 0;
	endif

	account.EraseProp( "#PendingPolicyLoginUntil" );
	account.EraseProp( "#PendingPolicyLoginDiscord" );
	account.EraseProp( "#PendingPolicyLoginIP" );

	return 1;
endfunction

function ACCT_CountPendingPolicyByDiscord( discord_norm, now_systime, byref pending_ips )

	discord_norm := ACCT_NormalizeDiscordID( discord_norm );
	if( !discord_norm )
		pending_ips := array;
		return 0;
	endif

	pending_ips := array;
	var pending_count := 0;

	foreach acct_name in ( ListAccounts() )
		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var account_discord_norm := ACCT_NormalizeDiscordID( account.GetProp( "DiscordID" ));
		if( account_discord_norm != discord_norm )
			continue;
		endif

		var pending_until := CInt( account.GetProp( "#PendingPolicyLoginUntil" ));
		// systime resets to 0 on server restart. A real reservation is set to
		// now_systime + 180, so anything more than ~5 minutes in the future is
		// a stale prop from a previous uptime and must be discarded.
		if( pending_until < now_systime || pending_until > now_systime + 300 )
			account.EraseProp( "#PendingPolicyLoginUntil" );
			account.EraseProp( "#PendingPolicyLoginDiscord" );
			account.EraseProp( "#PendingPolicyLoginIP" );
			continue;
		endif

		pending_count := pending_count + 1;
		var pending_ip := ACCT_TrimDiscordID( account.GetProp( "#PendingPolicyLoginIP" ));
		if( pending_ip && !pending_ips.contains( pending_ip ))
			pending_ips.append( pending_ip );
		endif
		SleepMS(2);
	endforeach

	return pending_count;
endfunction

function ACCT_CountPendingPolicyByIP( player_ip, now_systime, byref pending_discords )

	player_ip := ACCT_TrimDiscordID( player_ip );
	if( !player_ip )
		pending_discords := array;
		return 0;
	endif

	pending_discords := array;
	var pending_count := 0;

	foreach acct_name in ( ListAccounts() )
		var account := FindAccount( acct_name );
		if( account.errortext )
			continue;
		endif

		var pending_until := CInt( account.GetProp( "#PendingPolicyLoginUntil" ));
		// systime resets to 0 on server restart; bound the future window to
		// catch stale props from a previous uptime.
		if( pending_until < now_systime || pending_until > now_systime + 300 )
			account.EraseProp( "#PendingPolicyLoginUntil" );
			account.EraseProp( "#PendingPolicyLoginDiscord" );
			account.EraseProp( "#PendingPolicyLoginIP" );
			continue;
		endif

		var account_pending_ip := ACCT_TrimDiscordID( account.GetProp( "#PendingPolicyLoginIP" ));
		if( account_pending_ip != player_ip )
			continue;
		endif

		var account_discord_norm := ACCT_NormalizeDiscordID( account.GetProp( "DiscordID" ));
		if( !account_discord_norm )
			continue;
		endif

		if( !pending_discords.contains( account_discord_norm ))
			pending_discords.append( account_discord_norm );
			pending_count := pending_count + 1;
		endif
		SleepMS(2);
	endforeach

	return pending_count;
endfunction

function ACCT_AcquirePolicyLoginGate()
	var spins := 0;
	while( CInt( GetGlobalProperty( "acct_policy_login_gate" )) == 1 )
		spins := spins + 1;
		if( spins >= 240 )
			SetGlobalProperty( "acct_policy_login_gate", 0 );
			break;
		endif
		SleepMS(25);
	endwhile

	SetGlobalProperty( "acct_policy_login_gate", 1 );
	return 1;
endfunction

function ACCT_ReleasePolicyLoginGate()
	SetGlobalProperty( "acct_policy_login_gate", 0 );
	return 1;
endfunction

function ACCT_CheckAccountLoginPolicy( account_or_who, ip )

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

	if( !ACCT_AcquirePolicyLoginGate() )
		return 0;
	endif

	var now_systime := POLCore().systime;
	var player_ip := CStr( ip );

	// Clear any prior reservation belonging to this same account BEFORE counting
	// pending peers, so the candidate is never counted as its own peer. We then
	// re-establish the reservation only if the policy allows the login.
	account.EraseProp( "#PendingPolicyLoginUntil" );
	account.EraseProp( "#PendingPolicyLoginDiscord" );
	account.EraseProp( "#PendingPolicyLoginIP" );

	var snapshot := ACCT_CountOnlineByDiscordAndIP();
	var pending_ips := array;
	var pending_count := ACCT_CountPendingPolicyByDiscord( discord_norm, now_systime, pending_ips );

	var live_online_count := 0;
	if( snapshot["DiscordCharCount"].exists( discord_norm ))
		live_online_count := snapshot["DiscordCharCount"][discord_norm];
	endif

	var settings := ACCT_GetSettingsCfgElem( "Settings" );
	var max_discords_online := GetConfigInt( settings, "MaxDiscordsOnline" );
	if( max_discords_online < 1 )
		max_discords_online := 2;
	endif

	// Cap is on count of concurrent logins per DiscordID, not on IP spread.
	// One person legitimately playing two characters from two devices is allowed.
	// effective_total includes the candidate (+1).
	var effective_total := live_online_count + pending_count + 1;

	ACCT_DebugPrint( "Account preflight counts for " + account.name + " discord=" + discord_norm + " live_online_count=" + CStr( live_online_count ) + " pending_others=" + CStr( pending_count ) + " effective_total=" + CStr( effective_total ) + " cap=" + CStr( max_discords_online ));

	if( live_online_count >= max_discords_online || effective_total > max_discords_online )
		ACCT_ReleasePolicyLoginGate();
		ACCT_DebugPrint( "Disconnecting: account preflight denied for " + account.name + " discord=" + discord_norm );
		return 0;
	endif

	// Policy passed — register the reservation as a concurrency guard for any
	// other login that races in before this one reaches the world.
	account.SetProp( "#PendingPolicyLoginUntil", now_systime + 180 );
	account.SetProp( "#PendingPolicyLoginDiscord", discord_norm );
	account.SetProp( "#PendingPolicyLoginIP", player_ip );

	ACCT_ReleasePolicyLoginGate();
	return 1;
endfunction

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
	var now_systime := POLCore().systime;

	// This account is now reaching the world. Its own auth-time reservation
	// is stale (it was set with the auth-time IP which may differ from the
	// in-game IP, e.g. 127.0.0.1 vs the client's LAN IP). Clear it BEFORE
	// building the snapshot so we don't double-count this account as both
	// live and pending, and do NOT re-set a new pending reservation — the
	// live snapshot is the source of truth from here on.
	ACCT_ClearPendingLoginReservation( who );

	var snapshot := ACCT_CountOnlineByDiscordAndIP();
	if( snapshot.errortext )
		ACCT_DebugPrint( "Disconnecting: unable to build online snapshot." );
		SysLog( "LoginAccount() Error: unable to build online snapshot." );
		return 0;
	endif

	var ip_to_discords := snapshot["IPToDiscords"];
	var discord_to_ips := snapshot["DiscordToIPs"];
	var discord_char_count := snapshot["DiscordCharCount"];

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
	var effective_discord_ip_set := dictionary{};
	if( discord_to_ips.exists( discord_norm ))
		var existing_ips := discord_to_ips[discord_norm];
		foreach existing_ip in ( existing_ips )
			effective_discord_ip_set[CStr( existing_ip )] := 1;
		endforeach
	endif

	var pending_ips := array;
	var pending_same_discord_count := ACCT_CountPendingPolicyByDiscord( discord_norm, now_systime, pending_ips );
	foreach pending_ip in ( pending_ips )
		effective_discord_ip_set[CStr( pending_ip )] := 1;
	endforeach

	var effective_discord_ips := array;
	foreach ip_key in ( effective_discord_ip_set )
		effective_discord_ips.append( ip_key );
	endforeach

	var effective_discord_char_count := 0;
	if( discord_char_count.exists( discord_norm ))
		effective_discord_char_count := discord_char_count[discord_norm];
	endif
	effective_discord_char_count := effective_discord_char_count + pending_same_discord_count;

	ACCT_DebugPrint( "Policy counts for " + who.name + " discord=" + discord_norm + " ip=" + player_ip + " online_count=" + CStr( effective_discord_char_count ) + " pending_count=" + CStr( pending_same_discord_count ) + " ip_count=" + CStr( effective_discord_ips.size() ));

	var settings_policy := ACCT_GetSettingsCfgElem( "Settings" );
	var max_discords_online := GetConfigInt( settings_policy, "MaxDiscordsOnline" );
	if( max_discords_online < 1 )
		max_discords_online := 2;
	endif

	// Same DiscordID may not appear from more than one IP at a time.
	if( effective_discord_ips.size() > 1 )
		ACCT_ClearPendingLoginReservation( who );
			ACCT_DebugPrint( "Disconnecting: DiscordID " + discord_norm + " is active from multiple IPs (" + CStr( effective_discord_ips ) + ")." );
			SysLog( "LoginAccount() Error: discord active from multiple IPs for " + who.name );
			return 0;
	endif

	if( effective_discord_char_count > max_discords_online )
		ACCT_ClearPendingLoginReservation( who );
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
	var ip_unique_discord_count := 0;
	foreach ip_discord_key in ( ip_discord_set )
		ip_unique_discord_count := ip_unique_discord_count + 1;
	endforeach

	if( !household_id )
		if( ip_unique_discord_count > 1 )
			ACCT_ClearPendingLoginReservation( who );
			ACCT_DebugPrint( "Disconnecting: non-household account cannot share IP with another DiscordID." );
			SysLog( "LoginAccount() Error: non-household IP share denied for " + who.name );
			return 0;
		endif
		ACCT_DebugPrint( "Allow login for " + who.name + " discord=" + discord_norm + " ip=" + player_ip + " (non-household single-discord ip)" );
		return 1;
	endif

	var household := ACCT_GetHouseholdById( household_id, 0 );
	if( household.errortext )
		ACCT_ClearPendingLoginReservation( who );
		ACCT_DebugPrint( "Disconnecting: household record missing for " + who.name + " (" + household_id + ")." );
		SysLog( "LoginAccount() Error: household record missing for " + who.name );
		return 0;
	endif

	var household_cap := CInt( household.GetProp( "HouseholdCap" ));
	if( household_cap < 1 )
		household_cap := 1;
	endif

	if( ip_unique_discord_count > household_cap )
		ACCT_ClearPendingLoginReservation( who );
		ACCT_DebugPrint( "Disconnecting: household cap exceeded for " + who.name + " cap=" + CStr( household_cap ) + "." );
		SysLog( "LoginAccount() Error: household cap exceeded for " + who.name );
		return 0;
	endif

	foreach ip_discord in ( ip_discord_set )
		if( !ACCT_HouseholdContainsDiscord( household_id, ip_discord ))
			ACCT_ClearPendingLoginReservation( who );
			ACCT_DebugPrint( "Disconnecting: mixed-household IP use denied for " + who.name + "." );
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

function ACCT_FindValidPlayer(player)

	var playerserial := 0;
	var acctlist := ListAccounts();
	var i := 1;

	// Stuff for verifying that the player exists
	foreach acctnam in acctlist
		var acctref := FindAccount(acctnam);
		for ( i := 1; i<=ACCT_MAX_CHARACTERS; i:=i+1 )
			var playername := CStr(acctref.GetCharacter(i).name);
			if(!playername)
				continue;
			endif
			if ( Lower(playername) == Lower(player) )
				playerserial := acctref.GetCharacter(i).serial;
				break;
			endif
		endfor
	endforeach

  return playerserial;

endfunction