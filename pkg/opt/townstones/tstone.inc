// Shared helpers for the townstone package.
// Include with:  include ":townstone:tstone";
//
// Callers that use the election helpers (DeclareCandidate, CastVote,
// ResolveElection, CallElection, CallPoll) must declare the following
// file-scope vars in their .src:
//     city, election_active, poll_question, candidates,
//     population, votes, votepercent, timer, poll_result,
//     mayorserial, nextcitizenelection

use uo;
use os;
use basic;
use datafile;
use cfgfile;
use polsys;

include "include/math";
include "include/client";

const TOWNSTONE_DATAFILE       := ":townstone:townstone_data";
const TOWNSTONE_UPGRADES_CFG   := ":townstone:upgrades";
const TOWNSTONE_OBJTYPE        := 0xa320;
const VOTE_RUNE_OBJTYPE        := 0xa321;
const CHEQUE_OBJTYPE           := 0x14000;

const CITIZEN_ELECTION_COOLDOWN := (60 * 24 * 3600);
const ELECTION_DURATION         := (2 * 60);
const POLL_DURATION             := (7 * 24 * 3600);
const POLL_TIMER_EXTEND         := (6 * 3600);

const WATCHER_PROP_NAME         := "#TownstoneElectionWatcherRunning";
const TOWNLIST_GLOBAL           := "townlist";
const TOWNREALMS_GLOBAL         := "townrealms";


// ---------------------------------------------------------------------------
// Datafile / region key
// ---------------------------------------------------------------------------

function OpenTownstoneDataFile()
	var df := OpenDataFile(TOWNSTONE_DATAFILE);
	if(!df)
		CreateDataFile(TOWNSTONE_DATAFILE, flags := DF_KEYTYPE_STRING);
		df := OpenDataFile(TOWNSTONE_DATAFILE);
	endif
	return df;
endfunction

function MakeRegionKey(realm, region_name)
	return CStr(realm) + ":" + CStr(region_name);
endfunction

function GetStoneRealm(item)
	if(!item)
		return "";
	endif
	var stored := CStr(GetObjProperty(item, "realm"));
	if(stored)
		return stored;
	endif
	return CStr(item.realm);
endfunction

function GetStoneRegionName(item)
	if(!item)
		return "";
	endif
	return CStr(GetObjProperty(item, "regionname"));
endfunction


// ---------------------------------------------------------------------------
// Global town registry
// ---------------------------------------------------------------------------

function GetTownName(town_id)
	var list := GetGlobalProperty(TOWNLIST_GLOBAL);
	if(!list or !list[town_id])
		return "";
	endif
	return CStr(list[town_id]);
endfunction

function GetTownRealm(town_id)
	var list := GetGlobalProperty(TOWNREALMS_GLOBAL);
	if(!list or !list[town_id])
		return "";
	endif
	return CStr(list[town_id]);
endfunction

function FindTownIdByRegion(region_name, realm)
	var namelist := GetGlobalProperty(TOWNLIST_GLOBAL);
	var realmlist := GetGlobalProperty(TOWNREALMS_GLOBAL);
	if(!namelist or !namelist.size())
		return 0;
	endif

	var i;
	for(i := 1; i <= namelist.size(); i := i + 1)
		if(CStr(namelist[i]) != CStr(region_name))
			continue;
		endif
		if(!realm)
			return i;
		endif
		if(realmlist and realmlist[i] and CStr(realmlist[i]) == CStr(realm))
			return i;
		endif
		if(!realmlist or !realmlist[i])
			// Legacy entry with no realm recorded: accept as a match.
			return i;
		endif
	endfor
	return 0;
endfunction

function RegisterTown(region_name, realm)
	var existing := FindTownIdByRegion(region_name, realm);
	if(existing)
		// Make sure realm is stamped even for pre-existing entries.
		var realmlist := GetGlobalProperty(TOWNREALMS_GLOBAL);
		if(!realmlist)
			realmlist := {};
		endif
		if(!realmlist[existing])
			realmlist[existing] := CStr(realm);
			SetGlobalProperty(TOWNREALMS_GLOBAL, realmlist);
		endif
		return existing;
	endif

	var namelist := GetGlobalProperty(TOWNLIST_GLOBAL);
	if(!namelist)
		namelist := {};
	endif
	var realmlist2 := GetGlobalProperty(TOWNREALMS_GLOBAL);
	if(!realmlist2)
		realmlist2 := {};
	endif

	namelist.append(CStr(region_name));
	var new_id := namelist.size();
	realmlist2[new_id] := CStr(realm);
	SetGlobalProperty(TOWNLIST_GLOBAL, namelist);
	SetGlobalProperty(TOWNREALMS_GLOBAL, realmlist2);
	return new_id;
endfunction


// ---------------------------------------------------------------------------
// Region enumeration
// ---------------------------------------------------------------------------

function IsCityRegion(elem)
	if(!elem)
		return 0;
	endif
	var type_val := Lower(CStr(GetConfigString(elem, "Type")));
	if(type_val == "city")
		return 1;
	endif
	var city_flag := CInt(GetConfigString(elem, "City"));
	if(city_flag == 1)
		return 1;
	endif
	return 0;
endfunction

function GetRegionRealmFromElem(elem)
	if(!elem)
		return "";
	endif
	return Lower(CStr(GetConfigString(elem, "Realm")));
endfunction

function GetRegionRange(elem)
	if(!elem)
		return {};
	endif
	var coords := GetConfigStringArray(elem, "Range");
	if(!coords or coords.size() < 4)
		return {};
	endif
	return { CInt(coords[1]), CInt(coords[2]), CInt(coords[3]), CInt(coords[4]) };
endfunction

function IsPointInRange(x, y, range)
	if(!range or range.size() < 4)
		return 0;
	endif
	return (x >= range[1] and x <= range[3] and y >= range[2] and y <= range[4]);
endfunction

function IsRangeInsideRange(inner_range, outer_range)
	if(!inner_range or inner_range.size() < 4 or !outer_range or outer_range.size() < 4)
		return 0;
	endif
	return (inner_range[1] >= outer_range[1] and
			inner_range[2] >= outer_range[2] and
			inner_range[3] <= outer_range[3] and
			inner_range[4] <= outer_range[4]);
endfunction

// Returns an array of struct { name, realm, range } for every region that
// qualifies as a city (Type City or legacy City 1). Duplicate names across
// realms are all included; callers should disambiguate by realm.
function EnumerateCityRegions()
	UnloadConfigFile("regions");
	var regions_cfg := ReadConfigFile("::regions/regions");
	if(!regions_cfg)
		return {};
	endif

	var region_names := GetConfigStringKeys(regions_cfg);
	if(!region_names or !region_names.size())
		return {};
	endif

	var results := {};
	foreach region_name in region_names
		var elem := FindConfigElem(regions_cfg, region_name);
		if(!elem or !IsCityRegion(elem))
			continue;
		endif
		var entry := struct;
		entry.+name  := CStr(region_name);
		entry.+realm := GetRegionRealmFromElem(elem);
		entry.+range := GetRegionRange(elem);
		results.append(entry);
	endforeach
	return results;
endfunction


// ---------------------------------------------------------------------------
// Multi-realm townstone scanning
// ---------------------------------------------------------------------------

function EnumerateAllTownstones()
	var stones := {};
	var realmlist := Realms();
	if(!realmlist)
		return stones;
	endif

	var realm_names := realmlist.Keys();
	if(!realm_names or !realm_names.size())
		return stones;
	endif

	foreach realm_name in realm_names
		if(!realm_name)
			continue;
		endif
		var realm_data := realmlist[realm_name];
		if(!realm_data)
			continue;
		endif
		var w := CInt(realm_data.width) - 1;
		var h := CInt(realm_data.height) - 1;
		if(w < 0)
			w := 0;
		endif
		if(h < 0)
			h := 0;
		endif

		var found := ListItemsInBoxOfObjType(TOWNSTONE_OBJTYPE, 0, 0, -128, w, h, 127, realm_name);
		if(!found or !found.size())
			continue;
		endif
		foreach it in found
			if(it and it.objtype == TOWNSTONE_OBJTYPE)
				stones.append(it);
			endif
		endforeach
	endforeach
	return stones;
endfunction

function FindTownstoneByTownId(town_id)
	if(!town_id)
		return 0;
	endif

	var region_name := GetTownName(town_id);
	var realm := GetTownRealm(town_id);

	if(region_name and realm)
		var df := OpenTownstoneDataFile();
		if(df)
			var elem := df.FindElement(MakeRegionKey(realm, region_name));
			if(elem)
				var serial := CInt(elem.GetProp("stone_serial"));
				UnloadDataFile(TOWNSTONE_DATAFILE);
				if(serial)
					var stone := SystemFindObjectBySerial(serial);
					if(stone and stone.objtype == TOWNSTONE_OBJTYPE)
						return stone;
					endif
				endif
			else
				UnloadDataFile(TOWNSTONE_DATAFILE);
			endif
		endif
	endif

	foreach stone in EnumerateAllTownstones()
		if(CInt(GetObjProperty(stone, "town")) == CInt(town_id))
			return stone;
		endif
		if(region_name and CStr(GetObjProperty(stone, "regionname")) == CStr(region_name))
			if(!realm or CStr(GetStoneRealm(stone)) == CStr(realm))
				return stone;
			endif
		endif
	endforeach
	return 0;
endfunction


// ---------------------------------------------------------------------------
// Datafile state sync
// ---------------------------------------------------------------------------

function TouchTownstoneRegionEntry(item)
	if(!item)
		return;
	endif
	var region_name := GetStoneRegionName(item);
	var realm := GetStoneRealm(item);
	if(!region_name or !realm)
		return;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return;
	endif

	var key := MakeRegionKey(realm, region_name);
	var elem := df.FindElement(key);
	if(!elem)
		elem := df.CreateElement(key);
	endif
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return;
	endif

	var gold := CInt(elem.GetProp("gold"));
	if(gold < 0)
		gold := 0;
	endif
	var allow_upgrades := Lower(CStr(elem.GetProp("allow_upgrades")));
	if(allow_upgrades != "off")
		allow_upgrades := "on";
	endif
	var allow_donations := Lower(CStr(elem.GetProp("allow_donations")));
	if(allow_donations != "off")
		allow_donations := "on";
	endif

	var mayor_state := GetObjProperty(item, "mayor");
	if(!mayor_state)
		mayor_state := "No mayor";
	endif
	var mayorserial_state := CInt(GetObjProperty(item, "mayorserial"));
	if(mayorserial_state < 0)
		mayorserial_state := 0;
	endif
	var population_state := CInt(GetObjProperty(item, "population"));
	if(population_state < 0)
		population_state := 0;
	endif
	var election_state := CInt(GetObjProperty(item, "election_active"));
	if(election_state < 0)
		election_state := 0;
	endif
	var poll_state := GetObjProperty(item, "poll_question");
	if(!poll_state)
		poll_state := 0;
	endif
	var candidates_state := GetObjProperty(item, "candidates");
	if(!candidates_state)
		candidates_state := {};
	endif
	var votes_state := CInt(GetObjProperty(item, "votes"));
	if(votes_state < 0)
		votes_state := 0;
	endif
	var votepercent_state := CInt(GetObjProperty(item, "votepercent"));
	if(votepercent_state < 0)
		votepercent_state := 0;
	endif
	var timer_state := CInt(GetObjProperty(item, "timer"));
	if(timer_state < 0)
		timer_state := 0;
	endif
	var poll_result_state := GetObjProperty(item, "poll_result");
	if(!poll_result_state)
		poll_result_state := {};
	endif
	var next_election_state := CInt(GetObjProperty(item, "nextcitizenelection"));
	if(next_election_state < 0)
		next_election_state := 0;
	endif
	var citizens_state := GetObjProperty(item, "citizen");
	if(!citizens_state)
		citizens_state := {};
	endif

	elem.SetProp("gold", gold);
	elem.SetProp("town_id", CInt(GetObjProperty(item, "town")));
	elem.SetProp("stone_serial", item.serial);
	elem.SetProp("region_name", region_name);
	elem.SetProp("realm", realm);
	elem.SetProp("mayor", mayor_state);
	elem.SetProp("mayorserial", mayorserial_state);
	elem.SetProp("population", population_state);
	elem.SetProp("election_active", election_state);
	elem.SetProp("poll_question", poll_state);
	elem.SetProp("candidates", candidates_state);
	elem.SetProp("votes", votes_state);
	elem.SetProp("votepercent", votepercent_state);
	elem.SetProp("timer", timer_state);
	elem.SetProp("poll_result", poll_result_state);
	elem.SetProp("nextcitizenelection", next_election_state);
	elem.SetProp("citizen", citizens_state);
	elem.SetProp("allow_upgrades", allow_upgrades);
	elem.SetProp("allow_donations", allow_donations);
	UnloadDataFile(TOWNSTONE_DATAFILE);
endfunction


// ---------------------------------------------------------------------------
// Bank features (gold, upgrades, donations)
// ---------------------------------------------------------------------------

function IsTownBankFeatureEnabled(realm, region_name, prop_name)
	if(!region_name or !realm)
		return 1;
	endif
	var df := OpenTownstoneDataFile();
	if(!df)
		return 1;
	endif
	var elem := df.FindElement(MakeRegionKey(realm, region_name));
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return 1;
	endif
	var val := Lower(CStr(elem.GetProp(prop_name)));
	UnloadDataFile(TOWNSTONE_DATAFILE);
	if(val == "off" or val == "0")
		return 0;
	endif
	return 1;
endfunction

function SetTownBankFeatureEnabled(realm, region_name, prop_name, enabled)
	if(!region_name or !realm)
		return;
	endif
	var df := OpenTownstoneDataFile();
	if(!df)
		return;
	endif
	var key := MakeRegionKey(realm, region_name);
	var elem := df.FindElement(key);
	if(!elem)
		elem := df.CreateElement(key);
	endif
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return;
	endif
	elem.SetProp(prop_name, enabled ? "on" : "off");
	UnloadDataFile(TOWNSTONE_DATAFILE);
endfunction

function IsTownUpgradesEnabled(realm, region_name)
	return IsTownBankFeatureEnabled(realm, region_name, "allow_upgrades");
endfunction

function IsTownDonationsEnabled(realm, region_name)
	return IsTownBankFeatureEnabled(realm, region_name, "allow_donations");
endfunction

function GetTownBankGold(realm, region_name)
	if(!region_name or !realm)
		return 0;
	endif
	var df := OpenTownstoneDataFile();
	if(!df)
		return 0;
	endif
	var elem := df.FindElement(MakeRegionKey(realm, region_name));
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return 0;
	endif
	var gold := CInt(elem.GetProp("gold"));
	UnloadDataFile(TOWNSTONE_DATAFILE);
	if(gold < 0)
		gold := 0;
	endif
	return gold;
endfunction

function SetTownBankGold(realm, region_name, new_amount)
	if(!region_name or !realm)
		return;
	endif
	if(new_amount < 0)
		new_amount := 0;
	endif
	var df := OpenTownstoneDataFile();
	if(!df)
		return;
	endif
	var key := MakeRegionKey(realm, region_name);
	var elem := df.FindElement(key);
	if(!elem)
		elem := df.CreateElement(key);
	endif
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return;
	endif
	elem.SetProp("gold", CInt(new_amount));
	UnloadDataFile(TOWNSTONE_DATAFILE);
endfunction

function DeleteTownstoneRegionData(realm, region_name)
	if(!region_name or !realm)
		return 0;
	endif
	var df := OpenTownstoneDataFile();
	if(!df)
		return 0;
	endif
	var key := MakeRegionKey(realm, region_name);
	var elem := df.FindElement(key);
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return 0;
	endif
	df.DeleteElement(key);
	UnloadDataFile(TOWNSTONE_DATAFILE);
	return 1;
endfunction


// ---------------------------------------------------------------------------
// Watcher lifecycle
// ---------------------------------------------------------------------------

function EnsureTownstoneElectionWatcher()
	if(GetGlobalProperty(WATCHER_PROP_NAME))
		return;
	endif
	Start_Script(":townstone:electionwatch");
endfunction

function BroadcastSystemLine(message)
	Broadcast(CStr(message), FONT_SHADOW, 25);
endfunction


// ---------------------------------------------------------------------------
// Formatting helpers
// ---------------------------------------------------------------------------

function FormatSecondsToDays(seconds_left)
	if(seconds_left <= 0)
		return "0";
	endif
	var days_left := CInt(seconds_left / 86400);
	if(mod(seconds_left, 86400))
		days_left := days_left + 1;
	endif
	return CStr(days_left);
endfunction

function FormatDuration(seconds_left)
	if(seconds_left <= 0)
		return "0m";
	endif
	var days := CInt(seconds_left / 86400);
	var remaining := seconds_left - (days * 86400);
	var hours := CInt(remaining / 3600);
	remaining := remaining - (hours * 3600);
	var minutes := CInt(remaining / 60);

	var parts := "";
	if(days > 0)
		parts := parts + CStr(days) + "d ";
	endif
	if(hours > 0 or days > 0)
		parts := parts + CStr(hours) + "h ";
	endif
	parts := parts + CStr(minutes) + "m";
	return parts;
endfunction

function FormatGoldAmount(amount)
	var numeric := CInt(amount);
	var sign := "";
	if(numeric < 0)
		sign := "-";
		numeric := 0 - numeric;
	endif

	var digits := CStr(numeric);
	if(Len(digits) <= 3)
		return sign + digits;
	endif

	var formatted := "";
	while(Len(digits) > 3)
		formatted := "," + digits[Len(digits) - 2, 3] + formatted;
		digits := digits[1, Len(digits) - 3];
	endwhile
	return sign + digits + formatted;
endfunction

function TruncateForGump(text, max_chars)
	var trimmed := CStr(text);
	if(!trimmed)
		return "";
	endif
	if(max_chars < 4)
		return trimmed;
	endif
	if(Len(trimmed) > max_chars)
		return trimmed[1, max_chars - 3] + "...";
	endif
	return trimmed;
endfunction


// ---------------------------------------------------------------------------
// Name suffix helpers ( "Alice of Britain" <-> "Alice" )
// ---------------------------------------------------------------------------

function StripExactSuffix(base_text, suffix)
	if(!base_text or !suffix)
		return base_text;
	endif
	var start_pos := Find(base_text, suffix, 1);
	if(!start_pos)
		return base_text;
	endif
	if((start_pos + Len(suffix) - 1) != Len(base_text))
		return base_text;
	endif
	if(start_pos <= 1)
		return base_text;
	endif
	return base_text[1, start_pos - 1];
endfunction

function StripTownSuffix(character_name, town_name)
	if(!character_name or !town_name)
		return character_name;
	endif
	var safe := StrReplace(town_name, "'", "");
	var stripped := StripExactSuffix(character_name, " of " + safe);
	if(stripped != character_name)
		return stripped;
	endif
	return StripExactSuffix(character_name, " of " + town_name);
endfunction


// ---------------------------------------------------------------------------
// Election logic (uses caller's file-scope vars; see header comment)
// ---------------------------------------------------------------------------

function CountVotes(item)
	var counts := {};
	var i;
	foreach rune in EnumerateItemsInContainer(item)
		if(rune.objtype == VOTE_RUNE_OBJTYPE)
			var opt := CInt(GetObjProperty(rune, "option"));
			if(!counts[opt])
				counts[opt] := 1;
			else
				counts[opt] := counts[opt] + 1;
			endif
		endif
	endforeach

	var max_votes := 0;
	var winner := 0;
	var tie := 0;
	for(i := 1; i <= counts.size(); i := i + 1)
		if(max_votes < counts[i])
			max_votes := CInt(counts[i]);
			winner := i;
			tie := 0;
		elseif(max_votes == CInt(counts[i]))
			tie := 1;
		endif
	endfor
	if(!tie)
		return winner;
	endif
	return 0;
endfunction

function GetTopTiedOptionNames(item)
	var counts := {};
	var max_votes := 0;
	var i;
	foreach rune in EnumerateItemsInContainer(item)
		if(rune.objtype == VOTE_RUNE_OBJTYPE)
			var opt := CInt(GetObjProperty(rune, "option"));
			if(!counts[opt])
				counts[opt] := 1;
			else
				counts[opt] := counts[opt] + 1;
			endif
			if(counts[opt] > max_votes)
				max_votes := counts[opt];
			endif
		endif
	endforeach

	if(max_votes <= 0)
		return "No options";
	endif

	var result_text := "";
	for(i := 1; i <= counts.size(); i := i + 1)
		if(counts[i] == max_votes)
			var option_name := CStr(candidates[(2 * i)]);
			if(!option_name)
				option_name := "Option " + CStr(i);
			endif
			if(result_text)
				result_text := result_text + ", " + option_name;
			else
				result_text := option_name;
			endif
		endif
	endfor
	if(!result_text)
		return "No options";
	endif
	return result_text;
endfunction

function UpdateVotePercent(item)
	var pct := 0.0;
	if(population > 0)
		pct := (CDbl(votes) / CDbl(population)) * 100.0;
	endif
	var v := CInt(pct);
	SetObjProperty(item, "votepercent", v);
	return v;
endfunction

function DeclareCandidate(who, item)
	var i;
	if(who.serial == mayorserial)
		SendSysMessage(who, "You can't be a candidate again.");
		return 0;
	endif

	set_critical(1);
	for(i := 1; i <= candidates.size(); i := i + 2)
		if(candidates[i] == who.serial)
			SendSysMessage(who, "You are already a candidate.");
			set_critical(0);
			return 0;
		endif
	endfor

	candidates.append(who.serial);
	candidates.append(who.name);
	set_critical(0);
	SendSysMessage(who, "You are now a candidate. Total: " + (candidates.size() / 2) + ".");
	SetObjProperty(item, "candidates", candidates);
	TouchTownstoneRegionEntry(item);
	return 1;
endfunction

function ResetElectionFields(item)
	SetObjProperty(item, "election_active", 0);
	SetObjProperty(item, "candidates", {});
	SetObjProperty(item, "votes", 0);
	SetObjProperty(item, "votepercent", 0);
	SetObjProperty(item, "timer", 0);
endfunction

function ResolveElection(item)
	var choice := CountVotes(item);

	var kind := 0;
	if(election_active)
		kind := 1;
	elseif(poll_question)
		kind := 2;
	endif

	case(kind)
		1:	if(votes < 3)
				BroadcastSystemLine("Election Result");
				BroadcastSystemLine("Town: " + city);
				BroadcastSystemLine("Winner: No winner (not enough votes)");
				ResetElectionFields(item);
			elseif(choice)
				BroadcastSystemLine("Election Result");
				BroadcastSystemLine("Town: " + city);
				BroadcastSystemLine("Winner: " + candidates[choice * 2]);
				SetObjProperty(item, "mayor", candidates[choice * 2]);
				SetObjProperty(item, "mayorserial", candidates[choice * 2 - 1]);
				SetObjProperty(item, "nextcitizenelection", ReadGameClock() + CITIZEN_ELECTION_COOLDOWN);
				ResetElectionFields(item);
			else
				BroadcastSystemLine("Election Result");
				BroadcastSystemLine("Town: " + city);
				BroadcastSystemLine("Winner: No winner (tie)");
				ResetElectionFields(item);
				if(GetObjProperty(item, "mayorserial"))
					SetObjProperty(item, "nextcitizenelection", ReadGameClock() + CITIZEN_ELECTION_COOLDOWN);
				else
					SetObjProperty(item, "nextcitizenelection", 0);
				endif
			endif

		2:	if(choice)
				BroadcastSystemLine("Poll Result");
				BroadcastSystemLine("Town: " + city + " | Question: " + CStr(poll_question));
				BroadcastSystemLine("Result: " + candidates[choice * 2]);
				SetObjProperty(item, "poll_result", { poll_question, candidates[choice * 2] });
				SetObjProperty(item, "poll_question", 0);
				SetObjProperty(item, "candidates", {});
				SetObjProperty(item, "votes", 0);
				SetObjProperty(item, "votepercent", 0);
				SetObjProperty(item, "timer", 0);
			else
				if(population > 0 and votes >= population)
					var tied := GetTopTiedOptionNames(item);
					BroadcastSystemLine("Poll Result");
					BroadcastSystemLine("Town: " + city + " | Question: " + CStr(poll_question));
					BroadcastSystemLine("Result: Tie: " + tied);
					SetObjProperty(item, "poll_result", { poll_question, "Tie: " + tied });
					SetObjProperty(item, "poll_question", 0);
					SetObjProperty(item, "candidates", {});
					SetObjProperty(item, "votes", 0);
					SetObjProperty(item, "votepercent", 0);
					SetObjProperty(item, "timer", 0);
				else
					timer := ReadGameClock() + POLL_TIMER_EXTEND;
					SetObjProperty(item, "timer", timer);
				endif
			endif
	endcase

	TouchTownstoneRegionEntry(item);
endfunction

function CastVote(who, item, choice)
	foreach rune in EnumerateItemsInContainer(item)
		if(rune.objtype == VOTE_RUNE_OBJTYPE and GetObjProperty(rune, "serial") == who.acctname)
			SendSysMessage(who, "You have already voted.");
			return 0;
		endif
	endforeach

	var vote_rune := CreateItemInContainer(item, VOTE_RUNE_OBJTYPE, 1);
	SetObjProperty(vote_rune, "serial", who.acctname);
	SetObjProperty(vote_rune, "option", choice);
	votes := votes + 1;
	SetObjProperty(item, "votes", votes);

	SendSysMessage(who, "You just voted for " + candidates[(2 * choice)] + ".");
	var current_pct := UpdateVotePercent(item);
	TouchTownstoneRegionEntry(item);

	if(poll_question and current_pct >= 100)
		ResolveElection(item);
		var result := GetObjProperty(item, "poll_result");
		if(result and result.size() >= 2)
			SendSysMessage(who, "Poll ended: " + CStr(result[2]));
		else
			SendSysMessage(who, "Poll ended.");
		endif
		return 1;
	endif

	if(timer > 0 and timer < ReadGameClock())
		ResolveElection(item);
	endif
	return 1;
endfunction

function CallElection(who, item)
	if(election_active or poll_question)
		SendSysMessage(who, "An election or poll is already running.");
		return;
	endif

	if(who.serial != mayorserial)
		if(population < 3)
			SendSysMessage(who, "You need at least 3 town members before you can call an election.");
			return;
		endif
		if(mayorserial)
			var next_time := CInt(GetObjProperty(item, "nextcitizenelection"));
			if(next_time and ReadGameClock() < next_time)
				SendSysMessage(who, "Citizens can only call elections every 2 months while a mayor exists.");
				return;
			endif
		endif
	endif

	foreach rune in EnumerateItemsInContainer(item)
		DestroyItem(rune);
	endforeach

	SetObjProperty(item, "candidates", {});
	SetObjProperty(item, "election_active", 1);
	SetObjProperty(item, "votes", 0);
	SetObjProperty(item, "timer", ReadGameClock() + ELECTION_DURATION);
	SetObjProperty(item, "votepercent", 0);
	TouchTownstoneRegionEntry(item);
	EnsureTownstoneElectionWatcher();

	BroadcastSystemLine("Elections for " + city + " are open!");
endfunction

function CallPoll(who, item)
	if(election_active or poll_question)
		SendSysMessage(who, "An election or poll is already running.");
		return;
	endif

	foreach rune in EnumerateItemsInContainer(item)
		DestroyItem(rune);
	endforeach

	var question := SendTextEntryGump(who, "What's the question?", 50);
	if(!question)
		SendSysMessage(who, "Cancelled.");
		return;
	endif

	SendSysMessage(who, "Type each option. Press Enter on an empty line to finish.");

	var options := {};
	var entry;
	repeat
		entry := SendTextEntryGump(who, "Enter Option " + (options.size() / 2 + 1), 50);
		if(entry)
			options.append(0);
			options.append(entry);
		endif
	until(!entry);

	if(!options.size())
		SendSysMessage(who, "No options were entered. Cancelled.");
		return;
	endif

	SetObjProperty(item, "candidates", options);
	SetObjProperty(item, "poll_question", question);
	SetObjProperty(item, "votes", 0);
	SetObjProperty(item, "timer", ReadGameClock() + POLL_DURATION);
	SetObjProperty(item, "votepercent", 0);
	SetObjProperty(item, "poll_result", {});
	TouchTownstoneRegionEntry(item);
	EnsureTownstoneElectionWatcher();

	BroadcastSystemLine("New poll for " + city + ".");
endfunction
