use uo;
use os;
use basic;
use cfgfile;
use basicio;	// 2026-09-25: Print() is called in this file; was compiling only because an including script brought basicio

include ":datafile:datafile";
include "include/constants/cfgfiles";

const AREA_POLICY_DATAFILE_PREFIX := ":areas:area_policies_";
const AREA_POLICY_SCHEMA_VERSION := 2;
const AREA_POLICY_DEBUG := 0;
const AREA_POLICY_TRACE_RESOLVE := 0;
const AREA_POLICY_TRACE_HASPOLICY := 0;
const AREA_POLICY_META_ELEMENT := "__meta__";
const AREA_POLICY_MASK_PROP := "Mask";

// 2026-09-23: per-realm mask cache in a global property (in memory, saved with the
// world). GetPolicyMask used to open the realm datafile from disk on every area check
// (per victim of an area spell, per cast, per swing, per recall); now the datafile is
// read only when the property is missing, and every save below rewrites it.
const AREA_POLICY_CACHE_PREFIX := "AreaPolicyCache_";

// 2026-10-10: version marker next to the mask cache. RefreshRealmPolicyCache bumps
// AreaPolicyCacheVer_<realm> after every rewrite; a script keeps its own copy of the
// mask dictionary and re-reads the global property only when this number changes, so
// long-running scripts (NPC AI) never go stale and never unpack the dictionary per call.
const AREA_POLICY_CACHE_VER_PREFIX := "AreaPolicyCacheVer_";

// 2026-10-10: element positions in a parsed area entry (ParseAreaLine result flattened
// into an array, or the error ParseAreaLine returned for a bad line).
const AREA_IDX_MIN_X := 1;
const AREA_IDX_MAX_X := 2;
const AREA_IDX_MIN_Y := 3;
const AREA_IDX_MAX_Y := 4;
const AREA_IDX_ID := 5;

// 2026-10-10: per-script caches. Literals only at file scope (gotcha 25); every one is
// filled lazily by the getters below. areas.cfg is static while the server runs.
var gAreaParsedCache := 0;	// realm -> array of parsed entries
var gAreaParsedRealm := "";	// realm whose entries are in gAreaParsedList
var gAreaParsedList := 0;
var gAreaMaskRealm := "";	// realm whose mask dictionary copy is in gAreaMaskDict
var gAreaMaskVer := 0;		// AreaPolicyCacheVer_<realm> the copy was taken at
var gAreaMaskDict := 0;

const AREA_POLICY_GUARDED := 1;
const AREA_POLICY_NO_RECALL_TO := 2;
const AREA_POLICY_NO_RECALL_FROM := 4;
const AREA_POLICY_NO_MARKING := 8;
const AREA_POLICY_ANTI_MAGIC := 16;
const AREA_POLICY_FORBIDDEN := 32;
const AREA_POLICY_NO_LOOTING := 64;
const AREA_POLICY_SAFE_AREA := 128;
const AREA_POLICY_NO_PK := 256;
const AREA_POLICY_RP_AREA := 512;

function LoadRealmPolicies(realm)
	realm := Lower(realm);

	var init_result := EnsurePolicyDataInitialized(realm);
	if (init_result == error)
		return init_result;
	endif

	var area_lines := GetRealmAreaLines(realm);
	if (area_lines == error)
		return area_lines;
	endif

	var file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
	if (file_ref == error)
		return file_ref;
	endif

	var entries := dictionary;
	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
			return area_info;
		endif

		var mask := GetPolicyMaskFromElement(file_ref, area_info.area_id);
		entries.Insert(area_info.area_id, mask);
	endforeach

	var meta := LoadRealmMeta(file_ref);
	if (meta == error)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
		return meta;
	endif

	var policy_struct := CreatePolicyStruct(
		realm,
		meta.schema_version,
		meta.updated_at,
		meta.updated_by,
		entries
	);
	UnloadDataFile(GetRealmPolicyDescriptor(realm));
	return policy_struct;
endfunction

function SaveRealmPolicies(realm, policy_struct, updated_by := "")
	realm := Lower(realm);

	var init_result := EnsurePolicyDataInitialized(realm);
	if (init_result == error)
		return init_result;
	endif

	var area_lines := GetRealmAreaLines(realm);
	if (area_lines == error)
		return area_lines;
	endif

	var file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
	if (file_ref == error)
		return file_ref;
	endif

	var entries := GetPolicyEntriesDictionary(policy_struct);
	var updated_at := ReadGameClock();
	if (policy_struct && policy_struct.updated_at)
		updated_at := CInt(policy_struct.updated_at);
	endif
	if (!updated_by && policy_struct && policy_struct.updated_by)
		updated_by := CStr(policy_struct.updated_by);
	endif

	var changed_count := 0;
	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
			return area_info;
		endif

		var mask := 0;
		if (entries.Exists(area_info.area_id)) // 2026-10-10: was "in entries.Keys()", a linear scan per area
			mask := CInt(entries[area_info.area_id]);
		endif

		var set_result := SetPolicyMaskInternal(file_ref, area_info.area_id, mask, updated_at, updated_by);
		if (set_result == error)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
			return set_result;
		endif
		changed_count := changed_count + CInt(set_result); // 2026-10-10: 1 only when the stored mask really changed
	endforeach

	var prune_result := PruneStaleRealmPolicyEntries(realm, file_ref);
	if (prune_result == error)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
		return prune_result;
	endif

	// 2026-10-10: the realm stamp moves only when something was written or pruned
	if (changed_count > 0 || prune_result > 0)
		var meta_result := SaveRealmMeta(file_ref, updated_at, updated_by);
		if (meta_result == error)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
			return meta_result;
		endif
	endif

	if (IsAreaPolicyDebugEnabled(realm))
		AreaPolicyDebugPrint("SAVE realm="+realm+" descriptor="+GetRealmPolicyDescriptor(realm)+" pruned="+CStr(prune_result)+" changed="+CStr(changed_count));
	endif

	UnloadDataFile(GetRealmPolicyDescriptor(realm));
	// 2026-09-23; 2026-10-19: only when something was written or pruned, or the cache was never
	// built. A no-edit save used to bump the version and make every script reload its mask copy.
	var cache_missing := (Lower(TypeOf(GetGlobalProperty(AREA_POLICY_CACHE_PREFIX+realm))) != "dictionary");
	if (changed_count > 0 || prune_result > 0 || cache_missing)
		RefreshRealmPolicyCache(realm);
	endif
	return LoadRealmPolicies(realm);
endfunction

function BuildAreaKey(area_line)
	var area_info := ParseAreaLine(area_line);
	if (area_info == error)
		return area_info;
	endif

	return area_info.area_id;
endfunction

function GetPolicyMask(realm, area_key)
	realm := Lower(realm);
	area_key := CStr(area_key);

	// 2026-10-10: script-local copy of the mask dictionary, re-read only when the
	// version marker moved (see GetRealmMaskDict); was GetGlobalProperty (a full
	// dictionary unpack) on every call.
	var ready := GetRealmMaskDict(realm);
	if (ready == error)
		return ready;
	endif

	if (gAreaMaskDict.exists(area_key))
		return CInt(gAreaMaskDict[area_key]);
	endif

	return 0;
endfunction

// 2026-10-10: makes gAreaMaskDict hold the current mask dictionary of this realm.
// Returns 1, or the error from rebuilding the cache. The version is read BEFORE the
// dictionary, so a refresh landing in between can only make the next call reload.
function GetRealmMaskDict(realm)
	var ver := CInt(GetGlobalProperty(AREA_POLICY_CACHE_VER_PREFIX+realm));
	if (gAreaMaskDict)
		if (gAreaMaskRealm == realm)
			if (gAreaMaskVer == ver)
				return 1;
			endif
		endif
	endif

	// 2026-09-23: memory first; the datafile only when the cache has never been built
	var cache := GetGlobalProperty(AREA_POLICY_CACHE_PREFIX+realm);
	if (Lower(TypeOf(cache)) != "dictionary")
		cache := RefreshRealmPolicyCache(realm); // also fills the script-local copy
		if (cache == error)
			return cache;
		endif
		return 1;
	endif

	gAreaMaskDict := cache;
	gAreaMaskRealm := realm;
	gAreaMaskVer := ver;
	return 1;
endfunction

// 2026-09-23: rebuilds the realm cache from the datafile; called by every save and
// by GetPolicyMask when the global property is missing. Returns the dictionary.
function RefreshRealmPolicyCache(realm)
	realm := Lower(realm);

	var policies := LoadRealmPolicies(realm);
	if (policies == error)
		return policies;
	endif

	var entries := GetPolicyEntriesDictionary(policies);

	// 2026-10-10: dictionary first, then the version bump, so a reader that sees the new
	// number always finds the new dictionary. Critical so two refreshes cannot share a number.
	var was_critical := is_critical();
	set_critical(1);
	SetGlobalProperty(AREA_POLICY_CACHE_PREFIX+realm, entries);
	var new_ver := CInt(GetGlobalProperty(AREA_POLICY_CACHE_VER_PREFIX+realm)) + 1;
	SetGlobalProperty(AREA_POLICY_CACHE_VER_PREFIX+realm, new_ver);
	if (!was_critical)
		set_critical(0);
	endif

	gAreaMaskDict := entries;
	gAreaMaskRealm := realm;
	gAreaMaskVer := new_ver;
	return entries;
endfunction

function SetPolicyMask(realm, area_key, mask, updated_by := "")
	realm := Lower(realm);
	area_key := CStr(area_key);
	mask := CInt(mask);

	var init_result := EnsurePolicyDataInitialized(realm);
	if (init_result == error)
		return init_result;
	endif

	var file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
	if (file_ref == error)
		return file_ref;
	endif

	var set_result := SetPolicyMaskInternal(file_ref, area_key, mask, ReadGameClock(), updated_by);
	if (set_result == error)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
		return set_result;
	endif

	var meta_result := SaveRealmMeta(file_ref, ReadGameClock(), updated_by);
	if (meta_result == error)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
		return meta_result;
	endif

	if (IsAreaPolicyDebugEnabled(realm))
		AreaPolicyDebugPrint("SET realm="+realm+" area_id="+area_key+" mask="+CStr(mask));
	endif

	UnloadDataFile(GetRealmPolicyDescriptor(realm));
	RefreshRealmPolicyCache(realm); // 2026-09-23
	return mask;
endfunction

function ResolveAreaKeyAtLocation(x, y, realm)
	realm := Lower(realm);

	// 2026-10-10: walks the script-local parsed list (same first-match order as the old
	// per-call ReadConfigfile + ParseAreaLine pass) instead of re-parsing every line.
	var ready := SelectParsedRealmAreas(realm);
	if (ready == error)
		return ready;
	endif

	var px := CInt(x);
	var py := CInt(y);

	foreach area_info in gAreaParsedList
		if (area_info == error)
			return area_info;
		endif

		if (px >= area_info[AREA_IDX_MIN_X])
			if (px <= area_info[AREA_IDX_MAX_X])
				if (py >= area_info[AREA_IDX_MIN_Y])
					if (py <= area_info[AREA_IDX_MAX_Y])
						if (AREA_POLICY_TRACE_RESOLVE)
							if (IsAreaPolicyDebugEnabled(realm))
								AreaPolicyDebugPrint("RESOLVE_KEY realm="+realm+" x="+CStr(px)+" y="+CStr(py)+" area_id="+area_info[AREA_IDX_ID]);
							endif
						endif
						return area_info[AREA_IDX_ID];
					endif
				endif
			endif
		endif
	endforeach

	if (AREA_POLICY_TRACE_RESOLVE)
		if (IsAreaPolicyDebugEnabled(realm))
			AreaPolicyDebugPrint("RESOLVE_KEY realm="+realm+" x="+CStr(px)+" y="+CStr(py)+" area_id=<none>");
		endif
	endif

	return 0;
endfunction

// 2026-10-10: makes gAreaParsedList hold the parsed area entries of this realm, parsing
// the config lines once per script per realm. Returns 1, or the GetRealmAreaLines error.
// A line that fails to parse is stored as its error, so resolving still only fails when
// the walk reaches it, as before.
function SelectParsedRealmAreas(realm)
	if (gAreaParsedList)
		if (gAreaParsedRealm == realm)
			return 1;
		endif
	endif

	if (!gAreaParsedCache)
		gAreaParsedCache := dictionary;
	endif

	if (!gAreaParsedCache.exists(realm))
		var area_lines := GetRealmAreaLines(realm);
		if (area_lines == error)
			return area_lines;
		endif

		var parsed := {};
		foreach area_line in area_lines
			var area_info := ParseAreaLine(area_line);
			if (area_info == error)
				parsed.append(area_info);
			else
				parsed.append({area_info.min_x, area_info.max_x, area_info.min_y, area_info.max_y, area_info.area_id});
			endif
		endforeach
		gAreaParsedCache[realm] := parsed;
	endif

	gAreaParsedList := gAreaParsedCache[realm];
	gAreaParsedRealm := realm;
	return 1;
endfunction

// 2026-10-10: one resolve for callers that need the matched area AND its mask (and
// several policy bits from it). Returns 0 outside every box, an error when the realm or
// a reached line is bad, else a struct { area_id, mask }.
function ResolveAreaMatchAtLocation(x, y, realm)
	realm := Lower(realm);

	var area_key := ResolveAreaKeyAtLocation(x, y, realm);
	if (area_key == error)
		return area_key;
	elseif (!area_key)
		return 0;
	endif

	var mask := GetPolicyMask(realm, area_key);
	if (mask == error)
		return mask;
	endif

	var match := struct;
	match.+area_id := area_key;
	match.+mask := mask;
	return match;
endfunction

function ResolvePolicyAtLocation(x, y, realm)
	realm := Lower(realm);
	var area_key := ResolveAreaKeyAtLocation(x, y, realm);
	if (area_key == error)
		return area_key;
	elseif (!area_key)
		if (AREA_POLICY_TRACE_RESOLVE)
			if (IsAreaPolicyDebugEnabled(realm))
				AreaPolicyDebugPrint("RESOLVE_MASK realm="+realm+" x="+CStr(x)+" y="+CStr(y)+" mask=0 area_id=<none>");
			endif
		endif
		return 0;
	endif

	var mask := GetPolicyMask(realm, area_key);
	if (mask != error)
		if (AREA_POLICY_TRACE_RESOLVE)
			if (IsAreaPolicyDebugEnabled(realm))
				AreaPolicyDebugPrint("RESOLVE_MASK realm="+realm+" x="+CStr(x)+" y="+CStr(y)+" mask="+CStr(mask)+" area_id="+CStr(area_key));
			endif
		endif
	endif

	return mask;
endfunction

function HasPolicy(mask, policy_bit)
	mask := CInt(mask);
	policy_bit := CInt(policy_bit);

	if (policy_bit <= 0)
		return 0;
	endif

	// 2026-10-10: bitwise test; every policy_bit is a single power of two, so this equals
	// the old (mask / bit) % 2 and still returns exactly 0 or 1.
	var result := 0;
	if (mask & policy_bit)
		result := 1;
	endif
	if (AREA_POLICY_DEBUG)
		if (AREA_POLICY_TRACE_HASPOLICY)
			if (mask != 0 || result != 0)
				AreaPolicyDebugPrint("HAS_POLICY mask="+CStr(mask)+" bit="+CStr(policy_bit)+" result="+CStr(result));
			endif
		endif
	endif

	return result;
endfunction

function EnsurePolicyDataInitialized(realm)
	realm := Lower(realm);

	var file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
	if (file_ref == error)
		return file_ref;
	endif

	var meta_elem := DFFindElement(file_ref, AREA_POLICY_META_ELEMENT, DF_CREATE);
	if (meta_elem == error)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
		return meta_elem;
	endif

	if (meta_elem.GetProp("SchemaVersion") == error)
		meta_elem.SetProp("SchemaVersion", AREA_POLICY_SCHEMA_VERSION);
	endif
	if (meta_elem.GetProp("UpdatedAt") == error)
		meta_elem.SetProp("UpdatedAt", 0);
	endif
	if (meta_elem.GetProp("UpdatedBy") == error)
		meta_elem.SetProp("UpdatedBy", "");
	endif

	UnloadDataFile(GetRealmPolicyDescriptor(realm));
	return 1;
endfunction

function PruneStaleRealmPolicyEntries(realm, file_ref := 0)
	realm := Lower(realm);

	var opened_here := 0;
	if (!file_ref)
		file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
		if (file_ref == error)
			return file_ref;
		endif
		opened_here := 1;
	endif

	var area_lines := GetRealmAreaLines(realm);
	if (area_lines == error)
		if (opened_here)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
		endif
		return area_lines;
	endif

	var valid_ids := dictionary;
	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			if (opened_here)
				UnloadDataFile(GetRealmPolicyDescriptor(realm));
			endif
			return area_info;
		endif
		valid_ids.Insert(area_info.area_id, 1);
	endforeach

	var removed := 0;
	var keys := file_ref.Keys();
	if (keys == error)
		if (opened_here)
			UnloadDataFile(GetRealmPolicyDescriptor(realm));
		endif
		return keys;
	endif

	foreach elem_key in keys
		var key := CStr(elem_key);
		if (key == AREA_POLICY_META_ELEMENT)
			continue;
		endif
		if (!valid_ids.Exists(key)) // 2026-10-10: was "in valid_ids.Keys()"
			file_ref.DeleteElement(key);
			removed := removed + 1;
		endif
	endforeach

	if (IsAreaPolicyDebugEnabled(realm) && removed > 0)
		AreaPolicyDebugPrint("PRUNE realm="+realm+" removed="+CStr(removed));
	endif

	if (opened_here)
		UnloadDataFile(GetRealmPolicyDescriptor(realm));
	endif
	if (removed > 0 && opened_here)
		RefreshRealmPolicyCache(realm); // 2026-09-23: keep the in-memory cache in step
	endif
	return removed;
endfunction

function GetRealmAreaLines(realm)
	realm := Lower(realm);

	var areas_cfg := ReadConfigfile(CFGFILE_AREAS);
	var realm_elem := areas_cfg[realm];
	if (!realm_elem)
		return error{"errortext":="Unknown areas realm: "+realm};
	endif

	var area_lines := GetConfigStringArray(realm_elem, "Area");
	if (!area_lines)
		return error{"errortext":="No areas found for realm: "+realm};
	endif

	return area_lines;
endfunction

function ParseAreaLine(area_line)
	var parts := SplitWords(area_line);
	if (!parts || parts.size() < 6)
		return error{"errortext":="Invalid area definition: "+CStr(area_line)};
	endif

	var area_info := struct;
	area_info.+min_x := CInt(parts[1]);
	area_info.+max_x := CInt(parts[2]);
	area_info.+min_y := CInt(parts[3]);
	area_info.+max_y := CInt(parts[4]);
	area_info.+area_id := ParseAreaIdToken(parts[5]);
	area_info.+name := BuildAreaNameAndId(parts);

	if (!area_info.name)
		return error{"errortext":="Invalid area name in definition: "+CStr(area_line)};
	endif
	if (!area_info.area_id)
		return error{"errortext":="Missing id= in area definition: "+CStr(area_line)};
	endif

	return area_info;
endfunction

function BuildAreaName(parts)
	var area_info := struct;
	area_info.+min_x := 0;
	area_info.+max_x := 0;
	area_info.+min_y := 0;
	area_info.+max_y := 0;
	area_info.+area_id := "";
	return BuildAreaNameAndId(parts);
endfunction

function BuildAreaNameAndId(parts)
	var area_name := "";
	var i;
	for (i := 6; i <= parts.size(); i := i + 1)
		var token := CStr(parts[i]);

		if (area_name)
			area_name := area_name+" ";
		endif
		area_name := area_name+token;
	endfor

	return area_name;
endfunction

function ParseAreaIdToken(token)
	var raw_token := CStr(token);
	var lower_token := Lower(raw_token);
	if (Len(lower_token) < 4)
		return "";
	endif
	if (SubStr(lower_token, 1, 1) != "i")
		return "";
	endif
	if (SubStr(lower_token, 2, 1) != "d")
		return "";
	endif
	if (SubStr(lower_token, 3, 1) != "=")
		return "";
	endif

	var raw_id := "";
	var i;
	for (i := 4; i <= Len(raw_token); i := i + 1)
		raw_id := raw_id + SubStr(raw_token, i, 1);
	endfor

	return NormalizeAreaId(raw_id);
endfunction

function NormalizeAreaId(area_id)
	var normalized := Lower(CStr(area_id));
	var normalized_id := "";
	var i;
	for (i := 1; i <= Len(normalized); i := i + 1)
		var ch := SubStr(normalized, i, 1);
		if ((ch >= "a" && ch <= "z") || (ch >= "0" && ch <= "9"))
			normalized_id := normalized_id+ch;
		endif
	endfor

	if (!normalized_id)
		normalized_id := "area";
	endif

	return normalized_id;
endfunction

function CreatePolicyStruct(realm, schema_version, updated_at, updated_by, entries)
	var policy_struct := struct;
	policy_struct.+realm := realm;
	policy_struct.+schema_version := CInt(schema_version);
	policy_struct.+updated_at := CInt(updated_at);
	policy_struct.+updated_by := CStr(updated_by);
	policy_struct.+entries := entries;
	return policy_struct;
endfunction

function GetPolicyEntriesDictionary(policy_struct)
	if (policy_struct && policy_struct.entries && Lower(TypeOf(policy_struct.entries)) == "dictionary")
		return policy_struct.entries;
	endif

	return dictionary;
endfunction

function OpenAreaPolicyFile(realm, create := DF_CREATE)
	realm := Lower(realm);
	var descriptor := GetRealmPolicyDescriptor(realm);
	var file_ref := DFOpenDataFile(descriptor, create);
	if (file_ref == error)
		return file_ref;
	endif

	return file_ref;
endfunction

function GetRealmPolicyDescriptor(realm)
	return AREA_POLICY_DATAFILE_PREFIX+Lower(realm);
endfunction

function GetPolicyMaskFromElement(file_ref, area_id)
	var elem := DFFindElement(file_ref, CStr(area_id), DF_NO_CREATE);
	if (elem == error)
		return elem;
	elseif (!elem)
		return 0;
	endif

	var prop_val := DFGetProp(elem, AREA_POLICY_MASK_PROP, DF_NO_CREATE);
	if (prop_val == error)
		return 0;
	endif

	return CInt(prop_val);
endfunction

// 2026-10-10: returns 1 when it wrote the element, 0 when the stored mask already
// matched (UpdatedAt/UpdatedBy are left alone), or the error. A missing element with
// mask 0 is not created: readers already treat "no element" as 0.
function SetPolicyMaskInternal(file_ref, area_id, mask, updated_at, updated_by)
	mask := CInt(mask);
	var elem := DFFindElement(file_ref, CStr(area_id), DF_NO_CREATE);
	if (elem == error)
		return elem;
	elseif (elem)
		var current := DFGetProp(elem, AREA_POLICY_MASK_PROP, DF_NO_CREATE);
		if (current != error)
			if (CInt(current) == mask)
				return 0;
			endif
		endif
	else
		if (mask == 0)
			return 0;
		endif
		elem := DFFindElement(file_ref, CStr(area_id), DF_CREATE);
		if (elem == error)
			return elem;
		endif
	endif

	elem.SetProp(AREA_POLICY_MASK_PROP, mask);
	elem.SetProp("UpdatedAt", CInt(updated_at));
	elem.SetProp("UpdatedBy", CStr(updated_by));
	return 1;
endfunction

function LoadRealmMeta(file_ref)
	var meta_elem := DFFindElement(file_ref, AREA_POLICY_META_ELEMENT, DF_NO_CREATE);
	var meta := struct;
	meta.+schema_version := AREA_POLICY_SCHEMA_VERSION;
	meta.+updated_at := 0;
	meta.+updated_by := "";

	if (!meta_elem || meta_elem == error)
		return meta;
	endif

	var schema_val := DFGetProp(meta_elem, "SchemaVersion", DF_NO_CREATE);
	if (schema_val != error)
		meta.schema_version := CInt(schema_val);
	endif

	var updated_at_val := DFGetProp(meta_elem, "UpdatedAt", DF_NO_CREATE);
	if (updated_at_val != error)
		meta.updated_at := CInt(updated_at_val);
	endif

	var updated_by_val := DFGetProp(meta_elem, "UpdatedBy", DF_NO_CREATE);
	if (updated_by_val != error)
		meta.updated_by := CStr(updated_by_val);
	endif

	return meta;
endfunction

function SaveRealmMeta(file_ref, updated_at, updated_by)
	var meta_elem := DFFindElement(file_ref, AREA_POLICY_META_ELEMENT, DF_CREATE);
	if (meta_elem == error)
		return meta_elem;
	endif

	meta_elem.SetProp("SchemaVersion", AREA_POLICY_SCHEMA_VERSION);
	meta_elem.SetProp("UpdatedAt", CInt(updated_at));
	meta_elem.SetProp("UpdatedBy", CStr(updated_by));
	return 1;
endfunction

function IsAreaPolicyDebugEnabled(realm)
	realm := CStr(realm);
	if (!AREA_POLICY_DEBUG)
		return 0;
	endif

	return 1;
endfunction

function AreaPolicyDebugPrint(msg)
	Print("[areapolicy-debug] "+msg);
	return 1;
endfunction
