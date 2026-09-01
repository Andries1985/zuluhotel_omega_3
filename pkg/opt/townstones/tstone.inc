use os;
use uo;

include "include/math";
include "include/client";

const CITIZEN_ELECTION_COOLDOWN := (60*24*3600); // 2 months in seconds
const ELECTION_DURATION := (7*24*3600); // 1 week in seconds
// Moved here 2026-09-01 alongside LoadPlayerTownsConfig -- every file that
// includes tstone.inc now compiles that function, so the constant it
// references has to be visible everywhere too, not just in the 2 files that
// used to define it locally.
const PLAYERTOWNS_CFG := ":townstone:playertowns";

function countvotes(item)

	var result := { };  //formato { n� votos}
	var i, voto;

	//conta os votos
	foreach runa in EnumerateItemsInContainer( item )
		if (runa.objtype == 0xa321)
			voto:= cint(GetObjProperty(runa, "opcao"));
			if (!result[voto])
				result[voto] := 1;
			else
				result[voto] := result[voto] +1;
			endif
		endif
	endforeach

	//verifica quem venceu

	var nvotos := 0;
	var  vencedor;
	var empate :=0;

	for(i:=1;i<=result.size(); i:=i+1)
		if ( nvotos < result[i] )
			nvotos := cint(result[i]) ;
			vencedor := i;
			empate := 0;
		elseif (  nvotos == cint(result[i]) )
			empate := 1;
		endif
	endfor

	if(!empate)
		return vencedor;
	else
		return 0;
	endif

endfunction

function GetTopTiedOptionNames(item)
	var counts := {};
	var maxvotes := 0;
	var i;

	foreach runa in EnumerateItemsInContainer(item)
		if(runa.objtype == 0xa321)
			var voto := CInt(GetObjProperty(runa, "opcao"));
			if(!counts[voto])
				counts[voto] := 1;
			else
				counts[voto] := counts[voto] + 1;
			endif

			if(counts[voto] > maxvotes)
				maxvotes := counts[voto];
			endif
		endif
	endforeach

	if(maxvotes <= 0)
		return "No options";
	endif

	var result_text := "";
	for(i := 1; i <= counts.size(); i := i + 1)
		if(counts[i] == maxvotes)
			var option_name := CStr(candidatos[(2 * i)]);
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


function SetVotepercentage(item)
	var retval := 0.00;
	retval := retval + cdbl(votos);
	retval := retval / cdbl(populacao);
	retval := retval * 100;
	retval := cint(retval);
	SetObjProperty(item, "votepercent", retval);
	return retval;
endfunction


function high(divided, divisor)
	var retval := divided/divisor;

	if(mod(divided, divisor))
		retval:= retval +1;
	endif
	return retval;
endfunction


function GetTownName(town)
	var namelist := GetGlobalProperty("townlist");
	var name := namelist[town];
	return name;
endfunction

function SyncTownstoneState(item)
	if(!item)
		return;
	endif

	var region_name := "";
	var town_id := CInt(GetObjProperty(item, "town"));
	if(town_id)
		region_name := GetTownName(town_id);
	endif

	if(!region_name)
		return;
	endif

	TouchTownstoneRegionEntry(item, region_name);
endfunction

function BroadcastSystemLine(message)
	Broadcast(CStr(message), FONT_SHADOW, 25);
endfunction

function EnsureTownstoneElectionWatcher()
	if(GetGlobalProperty("#TownstoneElectionWatcherRunning"))
		return;
	endif

	Start_Script(":townstone:electionwatch");
endfunction


function candidato(who, item)

	var i;
	if(who.serial == mayorserial)
		SendSysMessage(who, "You can't be candidate again");
		return 0;
	endif

	set_critical(1);
	for(i:=1;i<=candidatos.size();i:=i+2)
		if(candidatos[i] == who.serial)
			SendSysMessage(who, "You are already a candidate");
			set_critical(0);
			return 0;
		endif
	endfor

	candidatos.append(who.serial);
	candidatos.append(who.name);
	set_critical(0);
	SendSysMessage(who, "Now you are a candidate, there are  " + (candidatos.size()/2) + " others.");
	SetObjProperty(item, "candidatos", candidatos);
	SyncTownstoneState(item);

endfunction



function voteto(who, item, choose)
	//verifica se fulando jah votou
	foreach runa in EnumerateItemsInContainer( item )
		if (runa.objtype == 0xa321)
			if (GetObjProperty(runa, "serial") == who.acctname)
				SendSysMessage(who, "You have already voted.");
				return 0;
			endif
		endif
	endforeach

	//Cria 1 runa do voto.

	var voto := CreateItemInContainer( item, 0xa321, 1);
	SetObjProperty(voto, "serial", who.acctname);
	SetObjProperty(voto, "opcao", choose);
	votos := votos+1;
	SetObjProperty(item, "votos",  votos);

	SendSysMessage(who, "You just voted for " + candidatos[(2*choose)] );
	var current_votepercent := SetVotepercentage(item);
	SyncTownstoneState(item);

	if(pleibicito and current_votepercent >= 100)
		eleicao(item);
		var poll_result := GetObjProperty(item, "Pleibicito_Result");
		if(poll_result and poll_result.size() >= 2)
			SendSysMessage(who, "Poll ended: " + CStr(poll_result[2]));
		else
			SendSysMessage(who, "Poll ended.");
		endif
		return;
	endif

	if(timer < ReadGameClock())
		eleicao(item);
	endif

endfunction


function eleicao(item);
	var choose := countvotes(item);

	var type;
	if (eleicoes)
		type :=1;
	elseif(pleibicito)
		type :=2;
	else
		type :=0;
	endif

	case(type)
		1: if(votos < 3)
			BroadcastSystemLine("Election Result");
			BroadcastSystemLine("Town: " + city);
			BroadcastSystemLine("Winner: No winner (not enough votes)");
			SetObjProperty(item, "eleicao", 0);
			SetObjProperty(item, "candidatos", { });
			SetObjProperty(item, "votos", 0);
			SetObjProperty(item, "votepercent", 0);
			SetObjProperty(item, "timer", 0);
		elseif(choose)
			BroadcastSystemLine("Election Result");
			BroadcastSystemLine("Town: " + city);
			BroadcastSystemLine("Winner: " + candidatos[choose*2]);
			SetObjProperty(item, "mayor",  candidatos[choose*2]);
			SetObjProperty(item, "mayorserial",  candidatos[choose*2-1]);
			SetObjProperty(item, "eleicao", 0);
			SetObjProperty(item, "nextcitizenelection", ReadGameClock() + CITIZEN_ELECTION_COOLDOWN);
			SetObjProperty(item, "candidatos", { });
			SetObjProperty(item, "votos", 0);
			SetObjProperty(item, "votepercent", 0);
			SetObjProperty(item, "timer", 0);
		else
			BroadcastSystemLine("Election Result");
			BroadcastSystemLine("Town: " + city);
			BroadcastSystemLine("Winner: No winner (tie)");
			SetObjProperty(item, "eleicao", 0);
			SetObjProperty(item, "candidatos", { });
			SetObjProperty(item, "votos", 0);
			SetObjProperty(item, "votepercent", 0);
			SetObjProperty(item, "timer", 0);
			if(GetObjProperty(item, "mayorserial"))
				SetObjProperty(item, "nextcitizenelection", ReadGameClock() + CITIZEN_ELECTION_COOLDOWN);
			else
				SetObjProperty(item, "nextcitizenelection", 0);
			endif
		endif

		2: if(choose)
			BroadcastSystemLine("Poll Result");
			BroadcastSystemLine("Town: " + city + " | Question: " + CStr(pleibicito));
			BroadcastSystemLine("Result: " + candidatos[choose*2]);
			SetObjProperty(item, "Pleibicito_Result", {pleibicito, candidatos[choose*2]} );
			SetObjProperty(item, "pleibicito", 0);
			SetObjProperty(item, "candidatos", { });
			SetObjProperty(item, "votos", 0);
			SetObjProperty(item, "votepercent", 0);
			SetObjProperty(item, "timer", 0);
		else
			if(populacao > 0 and votos >= populacao)
				var tied_options := GetTopTiedOptionNames(item);
				BroadcastSystemLine("Poll Result");
				BroadcastSystemLine("Town: " + city + " | Question: " + CStr(pleibicito));
				BroadcastSystemLine("Result: Tie: " + tied_options);
				SetObjProperty(item, "Pleibicito_Result", {pleibicito, "Tie: " + tied_options});
				SetObjProperty(item, "pleibicito", 0);
				SetObjProperty(item, "candidatos", { });
				SetObjProperty(item, "votos", 0);
				SetObjProperty(item, "votepercent", 0);
				SetObjProperty(item, "timer", 0);
			else
				timer := ReadGameclock() + (3600*6);
				SetObjProperty(item, "timer", timer);
			endif
		endif

	endcase

	SyncTownstoneState(item);

endfunction



function ConvocarEleicoes(who, item)

	if(eleicoes or pleibicito)
		SendSysMessage(who, "Elections running.");
		return;
	endif

	if(who.serial != mayorserial)
		if(populacao < 3)
			SendSysMessage(who, "You need at least 3 town members before you can call an election.");
			return;
		endif

		if(mayorserial)
			var next_election_time := cint(GetObjProperty(item, "nextcitizenelection"));
			if(next_election_time and ReadGameClock() < next_election_time)
				SendSysMessage(who, "Citizens can only call elections every 2 months while a mayor exists.");
				return;
			endif
		endif
	endif

	foreach runa in EnumerateItemsInContainer( item )
		destroyitem(runa);
	endforeach

	SetObjProperty(item, "candidatos", { });
	SetObjProperty(item, "eleicao", 1);
	SetObjProperty(item, "votos", 0);
	SetObjProperty(item, "timer", ReadGameClock() + ELECTION_DURATION );
	SetObjProperty(item, "votepercent", 0);
	SyncTownstoneState(item);
	EnsureTownstoneElectionWatcher();

			BroadcastSystemLine("Elections for " + city + " are open!");

endfunction

function ConvocarPleibicito(who, item)

	if(eleicoes or pleibicito)
		SendSysMessage(who, "Elections running.");
		return;
	endif

	foreach runa in EnumerateItemsInContainer( item )
		destroyitem(runa);
	endforeach

	var pergunta, opcoes := { }, temp;

	pergunta := SendTextEntryGump( who, "What's the question? " , 50);
	if(!pergunta)
		SendSysMessage(who, "Cancelled");
		return;
	endif

	SendSysMessage(who, "Type the options, press ENTER to finish");

	repeat

	temp := SendTextEntryGump( who, "Enter Option " +(opcoes.size()/2+1) , 50);
	if(temp)
		opcoes.append(0);
		opcoes.append(temp);
	endif

	until(!temp);

	SetObjProperty(item, "candidatos", opcoes);
	SetObjProperty(item, "pleibicito", pergunta);
	SetObjProperty(item, "votos", 0);
	SetObjProperty(item, "timer", ReadGameClock() + (7*24*3600) );
	SetObjProperty(item, "votepercent", 0);
	SetObjProperty(item, "Pleibicito_Result", {});
	SyncTownstoneState(item);
	EnsureTownstoneElectionWatcher();

	BroadcastSystemLine("New poll for " + city );

endfunction


///////////////////////////////////////////////////////////////////////////
// 2026-09-01: consolidated here from up to 9 separate near-verbatim copies
// spread across electionwatch.src and textcmd/admin|player/*.src (the
// "duplicated helper family" flagged in the code review) -- each of these
// was previously pasted into every file that needed it, which had already
// caused one real behavioral drift (see ParseEnabled below). All 18 bodies
// below were diff-verified byte-identical across every copy before being
// consolidated, except TouchTownstoneRegionEntry and ParseEnabled, where
// the copies had drifted; the most complete/permissive variant of each was
// kept as canonical (noted on each). Callers now get these via
// `include ":townstone:tstone";` (package-qualified, since most callers are
// two directories deeper than this file -- a bare `include "tstone";` only
// resolves relative to the calling file's own directory, not the package).
///////////////////////////////////////////////////////////////////////////

function OpenTownstoneDataFile()
	var df := OpenDataFile(TOWNSTONE_DATAFILE);
	if(!df)
		CreateDataFile(TOWNSTONE_DATAFILE, flags := DF_KEYTYPE_STRING);
		df := OpenDataFile(TOWNSTONE_DATAFILE);
	endif
	return df;
endfunction

// Canonical version -- was electionwatch.src's/tstone.src's TouchTownstoneRegionEntry.
// tstone.src's copy was the superset (also preserves allow_upgrades/
// allow_donations on the element); electionwatch.src's copy silently
// omitted those two fields (harmless -- SetProp on other fields doesn't
// touch them -- but incomplete). Kept as the canonical version.
function TouchTownstoneRegionEntry(item, region_name)
	if(!region_name)
		return;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return;
	endif

	var elem := df.FindElement(region_name);
	if(!elem)
		elem := df.CreateElement(region_name);
	endif
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return;
	endif

	var gold := CInt(elem.GetProp("gold"));
	if(gold < 0)
		gold := 0;
	endif

	var mayor_state := GetObjProperty(item, "mayor");
	if(!mayor_state)
		mayor_state := "No mayor";
	endif
	var mayorserial_state := CInt(GetObjProperty(item, "mayorserial"));
	if(mayorserial_state < 0)
		mayorserial_state := 0;
	endif
	var populacao_state := CInt(GetObjProperty(item, "populacao"));
	if(populacao_state < 0)
		populacao_state := 0;
	endif
	var eleicao_state := CInt(GetObjProperty(item, "eleicao"));
	if(eleicao_state < 0)
		eleicao_state := 0;
	endif
	var pleibicito_state := GetObjProperty(item, "pleibicito");
	if(!pleibicito_state)
		pleibicito_state := 0;
	endif
	var candidatos_state := GetObjProperty(item, "candidatos");
	if(!candidatos_state)
		candidatos_state := {};
	endif
	var votos_state := CInt(GetObjProperty(item, "votos"));
	if(votos_state < 0)
		votos_state := 0;
	endif
	var votepercent_state := CInt(GetObjProperty(item, "votepercent"));
	if(votepercent_state < 0)
		votepercent_state := 0;
	endif
	var timer_state := CInt(GetObjProperty(item, "timer"));
	if(timer_state < 0)
		timer_state := 0;
	endif
	var poll_result_state := GetObjProperty(item, "Pleibicito_Result");
	if(!poll_result_state)
		poll_result_state := {};
	endif
	var next_election_state := CInt(GetObjProperty(item, "nextcitizenelection"));
	if(next_election_state < 0)
		next_election_state := 0;
	endif
	var citizen_state := GetObjProperty(item, "citizen");
	if(!citizen_state)
		citizen_state := {};
	endif
	var allow_upgrades_state := CStr(elem.GetProp("allow_upgrades"));
	if(Lower(allow_upgrades_state) != "off")
		allow_upgrades_state := "on";
	endif
	var allow_donations_state := CStr(elem.GetProp("allow_donations"));
	if(Lower(allow_donations_state) != "off")
		allow_donations_state := "on";
	endif

	elem.SetProp("gold", gold);
	elem.SetProp("town_id", CInt(GetObjProperty(item, "town")));
	elem.SetProp("stone_serial", item.serial);
	elem.SetProp("mayor", mayor_state);
	elem.SetProp("mayorserial", mayorserial_state);
	elem.SetProp("populacao", populacao_state);
	elem.SetProp("eleicao", eleicao_state);
	elem.SetProp("pleibicito", pleibicito_state);
	elem.SetProp("candidatos", candidatos_state);
	elem.SetProp("votos", votos_state);
	elem.SetProp("votepercent", votepercent_state);
	elem.SetProp("timer", timer_state);
	elem.SetProp("Pleibicito_Result", poll_result_state);
	elem.SetProp("nextcitizenelection", next_election_state);
	elem.SetProp("citizen", citizen_state);
	elem.SetProp("allow_upgrades", allow_upgrades_state);
	elem.SetProp("allow_donations", allow_donations_state);
	UnloadDataFile(TOWNSTONE_DATAFILE);
endfunction

function GetTownBankGold(region_name)
	if(!region_name)
		return 0;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return 0;
	endif

	var elem := df.FindElement(region_name);
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

function SetTownBankGold(region_name, new_amount)
	if(!region_name)
		return;
	endif

	if(new_amount < 0)
		new_amount := 0;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return;
	endif

	var elem := df.FindElement(region_name);
	if(!elem)
		elem := df.CreateElement(region_name);
	endif
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return;
	endif

	elem.SetProp("gold", CInt(new_amount));
	UnloadDataFile(TOWNSTONE_DATAFILE);
endfunction

function FormatGoldAmount(amount)
	var numeric_amount := CInt(amount);
	var sign := "";
	if(numeric_amount < 0)
		sign := "-";
		numeric_amount := 0 - numeric_amount;
	endif

	var digits := CStr(numeric_amount);
	if(len(digits) <= 3)
		return sign + digits;
	endif

	var formatted := "";
	while(len(digits) > 3)
		formatted := "," + digits[len(digits) - 2, 3] + formatted;
		digits := digits[1, len(digits) - 3];
	endwhile

	return sign + digits + formatted;
endfunction

function TruncateForGump(text, max_chars)
	var trimmed_text := CStr(text);
	if(!trimmed_text)
		return "";
	endif

	if(max_chars < 4)
		return trimmed_text;
	endif

	if(len(trimmed_text) > max_chars)
		return trimmed_text[1, max_chars - 3] + "...";
	endif

	return trimmed_text;
endfunction

function IsPointInRange(x, y, range)
	if(!range or range.size() < 4)
		return 0;
	endif

	return (x >= range[1] and x <= range[3] and y >= range[2] and y <= range[4]);
endfunction

function GetRegionRange(regions_cfg, region_name)
	if(!regions_cfg or !region_name)
		return {};
	endif

	var elem := regions_cfg[region_name];
	if(!elem)
		elem := FindConfigElem(regions_cfg, region_name);
	endif
	if(!elem)
		return {};
	endif

	var coords := GetConfigStringArray(elem, "Range");
	if(!coords or coords.size() < 4)
		return {};
	endif

	return { CInt(coords[1]), CInt(coords[2]), CInt(coords[3]), CInt(coords[4]) };
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

function SafeNonNegativeInt(raw_value)
	var value := CInt(raw_value);
	if(value < 0)
		value := 0;
	endif
	return value;
endfunction

// Canonical version -- was diverged between townbankstatus.src (accepted
// "off"/"0"/"no"/"false" as disabled) and playerruntowns.src (only "off"/"0").
// Kept the more permissive townbankstatus.src version as canonical; harmless
// today since every writer only ever stores "on"/"off", but the two readers
// disagreeing was a real, if currently dormant, inconsistency.
function ParseEnabled(raw_value)
	var value := Lower(CStr(raw_value));
	if(value == "off" or value == "0" or value == "no" or value == "false")
		return 0;
	endif
	return 1;
endfunction

function LoadPlayerTownsConfig()
	var cfg_map := dictionary;
	var cfg := ReadConfigFile(PLAYERTOWNS_CFG);
	if(!cfg)
		return cfg_map;
	endif

	var keys := GetConfigStringKeys(cfg);
	if(!keys or !keys.size())
		return cfg_map;
	endif

	var i;
	for(i := 1; i <= keys.size(); i := i + 1)
		var key := CStr(keys[i]);
		var elem := cfg[key];
		if(!elem)
			elem := FindConfigElem(cfg, key);
		endif
		if(!elem)
			continue;
		endif

		var region_name := Trim(CStr(GetConfigString(elem, "RegionName")));
		if(!region_name)
			region_name := Trim(CStr(key));
		endif
		if(!region_name)
			continue;
		endif

		var purchase_price := SafeNonNegativeInt(GetConfigInt(elem, "PurchasePrice"));
		var available := GetRuntimePlayerTownAvailableState(region_name, 1);

		cfg_map[Lower(region_name)] := { purchase_price, 0, available };
	endfor

	return cfg_map;
endfunction

function GetPlayerTownInfo(cfg_map, region_name)
	var info := { 0, 0, 1 };
	if(!cfg_map or !region_name)
		return info;
	endif

	var cfg_entry := cfg_map[Lower(CStr(region_name))];
	if(!cfg_entry)
		return info;
	endif

	info[1] := SafeNonNegativeInt(cfg_entry[1]);
	info[2] := GetRuntimePlayerTownPurchasedState(region_name, 0);
	info[3] := GetRuntimePlayerTownAvailableState(region_name, 1);

	return info;
endfunction

function GetRuntimePlayerTownPurchasedState(region_name, default_value)
	if(!region_name)
		return default_value;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return default_value;
	endif

	var elem := df.FindElement(region_name);
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return default_value;
	endif

	var raw := CStr(elem.GetProp("playertown_purchased"));
	UnloadDataFile(TOWNSTONE_DATAFILE);
	if(!raw)
		return default_value;
	endif

	return ParseEnabled(raw);
endfunction

function GetRuntimePlayerTownAvailableState(region_name, default_value)
	if(!region_name)
		return default_value;
	endif

	var df := OpenTownstoneDataFile();
	if(!df)
		return default_value;
	endif

	var elem := df.FindElement(region_name);
	if(!elem)
		UnloadDataFile(TOWNSTONE_DATAFILE);
		return default_value;
	endif

	var raw := CStr(elem.GetProp("playertown_available"));
	UnloadDataFile(TOWNSTONE_DATAFILE);
	if(!raw)
		return default_value;
	endif

	return ParseEnabled(raw);
endfunction

function GetTownRegionsFromDatafile()
	var df := OpenTownstoneDataFile();
	if(!df)
		return {};
	endif

	var keys := df.Keys();
	var region_list := {};
	if(keys)
		var key_idx;
		for(key_idx := 1; key_idx <= keys.size(); key_idx := key_idx + 1)
			var key_text := Trim(CStr(keys[key_idx]));
			if(Len(key_text) >= 1)
				region_list.append(key_text);
			endif
		endfor
	endif
	UnloadDataFile(TOWNSTONE_DATAFILE);

	return region_list;
endfunction

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

	var safe_town_name := StrReplace(town_name, "'", "");
	var stripped_name := StripExactSuffix(character_name, " of " + safe_town_name);
	if(stripped_name != character_name)
		return stripped_name;
	endif

	return StripExactSuffix(character_name, " of " + town_name);
endfunction

function GetCityRegionsFromConfig()
	UnloadConfigFile("regions");
	var regions_cfg := ReadConfigFile("::regions/regions");
	if(!regions_cfg)
		return {};
	endif

	var region_names := GetConfigStringKeys(regions_cfg);
	if(!region_names or !region_names.size())
		region_names := GetConfigStringArray(regions_cfg[1], "Region");
	endif

	if(!region_names or !region_names.size())
		return {};
	endif

	var city_regions := {};
	var region_idx;
	for(region_idx := 1; region_idx <= region_names.size(); region_idx := region_idx + 1)
		var region_name := CStr(region_names[region_idx]);
		var elem := regions_cfg[region_name];
		if(!elem)
			elem := FindConfigElem(regions_cfg, region_name);
		endif
		if(!elem)
			continue;
		endif

		var city_raw := GetConfigString(elem, "City");
		if(city_raw.errortext)
			continue;
		endif

		if(CInt(city_raw) != 1)
			continue;
		endif

		city_regions.append(CStr(region_name));
	endfor

	return city_regions;
endfunction

function GetCandidateTownRegions()
	var candidates := {};
	var seen := dictionary;

	var city_regions := GetCityRegionsFromConfig();
	if(city_regions and city_regions.size())
		var city_idx;
		for(city_idx := 1; city_idx <= city_regions.size(); city_idx := city_idx + 1)
			var city_name := Trim(CStr(city_regions[city_idx]));
			if(Len(city_name) < 1)
				continue;
			endif
			var city_key := Lower(city_name);
			if(seen[city_key])
				continue;
			endif
			seen[city_key] := 1;
			candidates.append(city_name);
		endfor
	endif

	var data_regions := GetTownRegionsFromDatafile();
	if(data_regions and data_regions.size())
		var data_idx;
		for(data_idx := 1; data_idx <= data_regions.size(); data_idx := data_idx + 1)
			var data_name := Trim(CStr(data_regions[data_idx]));
			if(Len(data_name) < 1)
				continue;
			endif
			var data_key := Lower(data_name);
			if(seen[data_key])
				continue;
			endif
			seen[data_key] := 1;
			candidates.append(data_name);
		endfor
	endif

	return candidates;
endfunction

