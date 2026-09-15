// multicraft.inc -- shared "assemble several distinct fixed parts into one
// output" crafting gump engine (2026-09-15). Distinct from craftmenu.inc,
// which is built around ONE player-chosen material family (N Logs, N
// Ingots...) -- these recipes need several different specific items at once
// (e.g. Barrel Staves + Barrel Lid + Barrel Hoops), closer in shape to the
// old TryToMakePotionKeg target-based assembly than to a normal recipe.
//
// Any tool or station can open this gump against its own recipe cfg --
// pkg/items/toolbox/config/barrelparts.cfg is the first one. Cfg block shape:
//
// MultiRecipe <key>
// {
//     Name            Closed Barrel
//     ResultObjtype   0x0FAE      // OR MakeDeed 0xDEED10A -- exactly one
//     SkillId         11
//     Difficulty      60
//     CoSkillid       37          // optional
//     CoSkill         20          // optional
//     Part1Objtype    0x1EB1
//     Part1Qty        5
//     Part2Objtype    0x1DB8
//     Part2Qty        1
// }

use uo;
use cfgfile;

include ":mdgumps:gumps";
include "include/itemutil";
include "include/classes";
include "include/random";
include "include/difficulty";
include "include/resourcemanager";

const MC_MAX_PARTS		:= 5;
const MC_BTN_BASE		:= 910000;
const MC_ROW_HEIGHT		:= 58;
const MC_POINTS_MULTIPLIER	:= 10;

// Builds a resourcemanager-style request for a raw objtype (no real item
// instance needed) so part availability/consumption can draw from the Omega
// Cache the same way every other crafting recipe in this repo does -- see
// resourcemanager.inc's MakeBackpackRequest, which this mirrors. `cache` is
// threaded through by the caller (one dict per gump-build or per try-make
// pass) so FindAccessibleContainer's proximity/DataFile scan only runs once
// per player action instead of once per part -- same fix craftmenu.inc got
// this same day for the identical redundant-lookup problem.
function MultiCraft_MakeRequest( character, objtype, byref cache )
	var access;
	if( cache.Exists("access") )
		access := cache["access"];
	else
		access := 0;
		if( !GetObjProperty( character, "omegacache_no_autodraw" ) )
			access := FindAccessibleContainer( character, array{REMOVE_FROM_SECURE} );
		endif
		cache["access"] := access;
	endif

	var dataFileHandle := 0;
	var houseSerial := 0;
	if( access )
		dataFileHandle := access.df;
		if( access.house )
			houseSerial := access.house.serial;
		endif
	endif

	return struct{
		objtype              := CInt(objtype),
		key                  := 0,
		color                := 0,
		quality              := 1.0,
		preferredSourceOrder := array{ BACKPACK, OMEGA_CACHE },
		dataFileHandle       := dataFileHandle,
		houseSerial          := houseSerial,
		leaseKey             := 0
	};
endfunction

// Halves a part quantity during a Crafting Power Hour, same convention every
// other crafting recipe in this repo follows (carpentry.src, blacksmithgump.
// inc, the old TryToMakePotionKeg this replaces).
function MultiCraft_PHQty( character, qty )
	if( GetGlobalProperty("PHC") || GetObjProperty(character, "#PPHC") )
		return CInt(Ceil(qty/2));
	endif
	return qty;
endfunction

exported function MultiCraft_ShowGump( character, cfg, title )
	var keys := GetConfigStringKeys( cfg );
	var recipe_keys := array{};
	foreach key in keys
		if( cfg[key].Name )
			recipe_keys.append( key );
		endif
	endforeach

	if( !len(recipe_keys) )
		SendSysMessage( character, "There's nothing to build here yet." );
		return;
	endif

	var cache := dictionary;
	var gump := GFCreateGump( 50, 50 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9200, 420, CInt(70 + len(recipe_keys) * MC_ROW_HEIGHT) );
	GFHTMLArea( gump, 20, 15, 380, 20, "<CENTER><B>" + title + "</B></CENTER>" );

	var y := 45;
	var btn_map := dictionary;
	var idx := 1;
	foreach key in recipe_keys
		var element := cfg[key];
		var have_all := 1;
		var parts_text := "";
		var p;
		for( p := 1; p <= MC_MAX_PARTS; p := p + 1 )
			var pobjtype := CInt(GetConfigString(element, "Part" + p + "Objtype"));
			if( !pobjtype )
				continue;
			endif
			var pqty := MultiCraft_PHQty( character, CInt(GetConfigString(element, "Part" + p + "Qty")) );
			var have := CInt(GetAvailableResource(character, MultiCraft_MakeRequest(character, pobjtype, cache)).total);
			if( have < pqty )
				have_all := 0;
			endif
			if( parts_text != "" )
				parts_text := parts_text + ", ";
			endif
			parts_text := parts_text + GetItemDisplayName(pobjtype) + " (" + have + "/" + pqty + ")";
		endfor

		var namehtml;
		if( have_all )
			namehtml := "<BASEFONT COLOR=#FFFFFF><B>" + element.Name + "</B></BASEFONT>";
		else
			namehtml := "<BASEFONT COLOR=#999999>" + element.Name + "</BASEFONT>";
		endif

		GFHTMLArea( gump, 20, y, 300, 20, namehtml, 0, 0 );
		GFHTMLArea( gump, 20, y+18, 380, 34, "<BASEFONT COLOR=#AAAAAA>" + parts_text + "</BASEFONT>", 0, 0 );
		GFAddButton( gump, 355, y, 4005, 4007, GF_CLOSE_BTN, MC_BTN_BASE + idx );
		btn_map[MC_BTN_BASE + idx] := key;
		y := y + MC_ROW_HEIGHT;
		idx := idx + 1;
	endforeach

	var input := GFSendGump( character, gump );
	if( !input )
		return;
	endif
	var btn := CInt(input[0]);
	if( !btn )
		return;
	endif
	var chosen_key := btn_map[btn];
	if( !chosen_key )
		return;
	endif

	MultiCraft_TryMake( character, cfg, chosen_key );
endfunction

function MultiCraft_TryMake( character, cfg, key )
	var element := cfg[key];
	if( !element )
		return 0;
	endif

	var cache := dictionary;
	var requests := array{};
	var quantities := array{};
	var p;
	for( p := 1; p <= MC_MAX_PARTS; p := p + 1 )
		var pobjtype := CInt(GetConfigString(element, "Part" + p + "Objtype"));
		if( !pobjtype )
			continue;
		endif
		var pqty := MultiCraft_PHQty( character, CInt(GetConfigString(element, "Part" + p + "Qty")) );
		var req := MultiCraft_MakeRequest( character, pobjtype, cache );
		if( CInt(GetAvailableResource(character, req).total) < pqty )
			SendSysMessage( character, "You don't have enough " + GetItemDisplayName(pobjtype) + "." );
			return 0;
		endif
		requests.append( req );
		quantities.append( pqty );
	endfor

	if( !len(requests) )
		return 0;
	endif

	var skillid := CInt(element.SkillId);
	var difficulty := CInt(element.Difficulty);
	var coskillid := CInt(element.CoSkillid);
	var coskill := CInt(element.CoSkill);
	var points := Cint( difficulty * MC_POINTS_MULTIPLIER );

	var excep_ch := 10;
	var excep_diff := difficulty;
	if( GetObjProperty(character, CLASSEID_CRAFTER) )
		excep_ch := Cint( excep_ch * ClasseBonus(character, CLASSEID_CRAFTER) );
		excep_diff := excep_diff + 20;
	else
		excep_diff := excep_diff + 40;
	endif

	foreach req in requests
		LeaseResource( character, req, quantities[_req_iter] );
	endforeach

	sendDiff( character, difficulty );
	Sleep(1);

	var success := CheckSkill( character, skillid, difficulty, points );
	if( success and coskillid )
		success := CheckSkill( character, coskillid, coskill, 0 );
	endif

	if( !success )
		// Destroy a skill-scaled percentage of EVERY leased part, not just
		// the first -- matches carpentry.src/blacksmithgump.inc's failure
		// model instead of dumping the whole loss onto one ingredient.
		var effskill := GetEffectiveSkill( character, skillid );
		var destroy := 75 - Cint( effskill / 2 );
		if( GetObjProperty(character, CLASSEID_CRAFTER) )
			destroy := Cint( destroy / ClasseBonus(character, CLASSEID_CRAFTER) );
		endif
		if( destroy > 50 )
			destroy := 50;
		endif

		foreach req in requests
			var qty := quantities[_req_iter];
			var lost := Cint( qty * destroy / 100 );
			if( lost < 1 )
				lost := 1;
			endif
			ConsumeResource( character, req, lost );
		endforeach
		SendSysMessage( character, "You fail and waste some materials." );
	else
		foreach req in requests
			ConsumeResource( character, req, quantities[_req_iter] );
		endforeach

		var resultobjtype := CInt(element.ResultObjtype);
		var makedeed := CInt(element.MakeDeed);
		var theitem;

		var is_exceptional := (GetObjProperty(character, "ForceExceptional")
			or ((Random(100) < excep_ch) and CheckSkill(character, skillid, excep_diff, 0)));

		if( makedeed )
			var product_desc := GetItemDescriptor( makedeed );
			product_desc.deedname := CStr(element.Name);
			product_desc.CProps.insert( "BaseName", CStr(element.Name) );
			product_desc.CProps.insert( "CraftedWith", CInt(requests[1].objtype) );
			if( is_exceptional )
				product_desc.CProps.insert( "IsExceptional", 1 );
				product_desc.CProps.insert( "CraftedBy", character.name );
				product_desc.deedname := "Exceptional " + product_desc.deedname;
			endif
			product_desc.CProps.insert( "DeedName", product_desc.deedname );
			theitem := CreateItemInBackpack( character, product_desc );
			if( theitem )
				SetName( theitem, GetObjProperty(theitem, "DeedName") );
			endif
		else
			theitem := CreateItemInBackpack( character, resultobjtype );
			if( theitem )
				SetObjProperty( theitem, "CraftedWith", CInt(requests[1].objtype) );
				if( is_exceptional )
					SetObjProperty( theitem, "IsExceptional", 1 );
					SetObjProperty( theitem, "CraftedBy", character.name );
				endif
				theitem.hp := theitem.maxhp;
			endif
		endif

		if( !theitem )
			SendSysMessage( character, "Your backpack is full." );
		else
			if( is_exceptional )
				SendSysMessage( character, "You create an exceptional " + element.Name + "." );
			else
				SendSysMessage( character, "You create " + element.Name + "." );
			endif
			IncRevision( theitem );
		endif
	endif

	foreach req in requests
		ReleaseResourceLease( req );
	endforeach

	return 1;
endfunction
