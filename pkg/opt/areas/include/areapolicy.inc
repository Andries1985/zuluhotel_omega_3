use uo;
use os;
use basic;
use cfgfile;

include ":datafile:datafile";
include "include/constants/cfgfiles";

const AREA_POLICY_DATAFILE_PREFIX := ":areas:area_policies_";
const AREA_POLICY_SCHEMA_VERSION := 2;
const AREA_POLICY_DEBUG := 0;
const AREA_POLICY_TRACE_RESOLVE := 0;
const AREA_POLICY_TRACE_HASPOLICY := 0;
const AREA_POLICY_META_ELEMENT := "__meta__";
const AREA_POLICY_MASK_PROP := "Mask";

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
			return area_info;
		endif

		var mask := GetPolicyMaskFromElement(file_ref, area_info.area_id);
		entries.Insert(area_info.area_id, mask);
	endforeach

	var meta := LoadRealmMeta(file_ref);
	if (meta == error)
		return meta;
	endif

	return CreatePolicyStruct(
		realm,
		meta.schema_version,
		meta.updated_at,
		meta.updated_by,
		entries
	);
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

	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			return area_info;
		endif

		var mask := 0;
		if (area_info.area_id in entries.Keys())
			mask := CInt(entries[area_info.area_id]);
		endif

		var set_result := SetPolicyMaskInternal(file_ref, area_info.area_id, mask, updated_at, updated_by);
		if (set_result == error)
			return set_result;
		endif
	endforeach

	var prune_result := PruneStaleRealmPolicyEntries(realm, file_ref);
	if (prune_result == error)
		return prune_result;
	endif

	var meta_result := SaveRealmMeta(file_ref, updated_at, updated_by);
	if (meta_result == error)
		return meta_result;
	endif

	if (IsAreaPolicyDebugEnabled(realm))
		AreaPolicyDebugPrint("SAVE realm="+realm+" descriptor="+GetRealmPolicyDescriptor(realm)+" pruned="+CStr(prune_result));
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

	var file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
	if (file_ref == error)
		return file_ref;
	endif

	return GetPolicyMaskFromElement(file_ref, area_key);
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
		return set_result;
	endif

	var meta_result := SaveRealmMeta(file_ref, ReadGameClock(), updated_by);
	if (meta_result == error)
		return meta_result;
	endif

	if (IsAreaPolicyDebugEnabled(realm))
		AreaPolicyDebugPrint("SET realm="+realm+" area_id="+area_key+" mask="+CStr(mask));
	endif

	return mask;
endfunction

function ResolveAreaKeyAtLocation(x, y, realm)
	realm := Lower(realm);

	var area_lines := GetRealmAreaLines(realm);
	if (area_lines == error)
		return area_lines;
	endif

	var point := struct;
	point.+x := CInt(x);
	point.+y := CInt(y);

	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			return area_info;
		endif

		if (point.x >= area_info.min_x)
			if (point.x <= area_info.max_x)
				if (point.y >= area_info.min_y)
					if (point.y <= area_info.max_y)
						if (IsAreaPolicyDebugEnabled(realm) && AREA_POLICY_TRACE_RESOLVE)
							AreaPolicyDebugPrint("RESOLVE_KEY realm="+realm+" x="+CStr(point.x)+" y="+CStr(point.y)+" area_id="+area_info.area_id);
						endif
						return area_info.area_id;
					endif
				endif
			endif
		endif
	endforeach

	if (IsAreaPolicyDebugEnabled(realm) && AREA_POLICY_TRACE_RESOLVE)
		AreaPolicyDebugPrint("RESOLVE_KEY realm="+realm+" x="+CStr(point.x)+" y="+CStr(point.y)+" area_id=<none>");
	endif

	return 0;
endfunction

function ResolvePolicyAtLocation(x, y, realm)
	realm := Lower(realm);
	var area_key := ResolveAreaKeyAtLocation(x, y, realm);
	if (area_key == error)
		return area_key;
	elseif (!area_key)
		if (IsAreaPolicyDebugEnabled(realm) && AREA_POLICY_TRACE_RESOLVE)
			AreaPolicyDebugPrint("RESOLVE_MASK realm="+realm+" x="+CStr(x)+" y="+CStr(y)+" mask=0 area_id=<none>");
		endif
		return 0;
	endif

	var mask := GetPolicyMask(realm, area_key);
	if (mask != error)
		if (IsAreaPolicyDebugEnabled(realm) && AREA_POLICY_TRACE_RESOLVE)
			AreaPolicyDebugPrint("RESOLVE_MASK realm="+realm+" x="+CStr(x)+" y="+CStr(y)+" mask="+CStr(mask)+" area_id="+CStr(area_key));
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

	var result := CInt(CInt(mask / policy_bit) % 2);
	if (AREA_POLICY_DEBUG && AREA_POLICY_TRACE_HASPOLICY && (mask != 0 || result != 0))
		AreaPolicyDebugPrint("HAS_POLICY mask="+CStr(mask)+" bit="+CStr(policy_bit)+" result="+CStr(result));
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

	return 1;
endfunction

function PruneStaleRealmPolicyEntries(realm, file_ref := 0)
	realm := Lower(realm);

	if (!file_ref)
		file_ref := OpenAreaPolicyFile(realm, DF_CREATE);
		if (file_ref == error)
			return file_ref;
		endif
	endif

	var area_lines := GetRealmAreaLines(realm);
	if (area_lines == error)
		return area_lines;
	endif

	var valid_ids := dictionary;
	foreach area_line in area_lines
		var area_info := ParseAreaLine(area_line);
		if (area_info == error)
			return area_info;
		endif
		valid_ids.Insert(area_info.area_id, 1);
	endforeach

	var removed := 0;
	var keys := file_ref.Keys();
	if (keys == error)
		return keys;
	endif

	foreach elem_key in keys
		var key := CStr(elem_key);
		if (key == AREA_POLICY_META_ELEMENT)
			continue;
		endif
		if (!(key in valid_ids.Keys()))
			file_ref.DeleteElement(key);
			removed := removed + 1;
		endif
	endforeach

	if (IsAreaPolicyDebugEnabled(realm) && removed > 0)
		AreaPolicyDebugPrint("PRUNE realm="+realm+" removed="+CStr(removed));
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

function SetPolicyMaskInternal(file_ref, area_id, mask, updated_at, updated_by)
	var elem := DFFindElement(file_ref, CStr(area_id), DF_CREATE);
	if (elem == error)
		return elem;
	endif

	elem.SetProp(AREA_POLICY_MASK_PROP, CInt(mask));
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
