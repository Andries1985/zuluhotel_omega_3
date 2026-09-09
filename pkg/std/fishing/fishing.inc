use uo;
use cfgfile;
use os;
use basicio;
use polsys;

include "include/client";
include "include/random";
include "include/itemutil";
include "include/attributes";
include "include/skillpoints";
include ":questpkg:questfishing";

//Dead code - superseded by PickCaughtFish()/ResolveFishCatch() below.
//function RandomFish(character)
//
//var currentskill := GetEffectiveSkill( character, SKILLID_FISHING);
//var thisone := Random(currentskill) + 1;
//var fishtype;
//
//if (thisone<50 )
//	fishtype := Random(UOBJ_FISH_END - UOBJ_FISH_START);
//	fishtype := UOBJ_FISH_START + fishtype;
//elseif (thisone<100)
//	fishtype := Random(UOBJ_SFISH_END - UOBJ_SFISH_START);
//	fishtype := UOBJ_SFISH_START + fishtype;
//else
//        fishtype := Random(9) + 0xA84;
//endif
//return fishtype;
//endfunction

function createfish( who,fish,fish_amount, equipped, deepwater := 1, tier := "Regular" )

	//A blade equipped for something unrelated shouldn't silently carve a rare/legendary
	//catch - players opt in via :togglerarecarve.
	var protect_rare := (tier == "Rare" or tier == "Legendary") and !GetObjProperty(who, "AutoCarveRareFish");

	if (equipped==1 or protect_rare or fish == 0xA84 or fish == 0xA85 or fish == 0xA86 or fish == 0xA87 or fish == 0xA88 or fish == 0xA89 or fish == 0xA8A or fish == 0xA8B or fish == 0xA8C )
		//Returned so ResolveFishCatch can read .desc for the catch message; 0 (below) means
		//it was carved into steaks instead.
		return CreateItemInBackpack(who, cint(fish), cint(fish_amount));
	else
		PlaySoundEffect(who, SFX_57);
		PerformAction( who, ANIM_SALUTE );
		var steaks_per_fish := 4;
		if (!deepwater)
			steaks_per_fish := 1;
		endif
		CreateItemInBackpack(who, UOBJ_FISHSTEAK, (steaks_per_fish * fish_amount));
		SendSysmessage(who, "You carve up the fish.");
		return 0;
	endif

endfunction

//Resolves the character's region via the engine (GetRegionNameAtLocation), then reads that
//region's Type directly from regions.cfg.
function IsFishingInDungeon(character)

	var region_name := GetRegionNameAtLocation(character.x, character.y, character.realm);
	var regions_cfg := ReadConfigFile("::regions/regions");
	var elem := FindConfigElem(regions_cfg, region_name);
	var region_type := "";
	if (elem)
		region_type := Lower(CStr(elem.Type));
	endif
	UnloadConfigFile("::regions/regions");

	return (region_type == "dungeon");

endfunction

//Safe area halves skill gain (checked first, overrides all), dungeon doubles it, deep water
//gives 1.5x, otherwise normal shore rate. in_safe_area/in_dungeon are cached once per fishing
//session by the caller; deep_water is refreshed every cast since a boat-anchored character can
//have the water under them shift between deep/shallow without ever moving themselves.
function GetFishingPointsMultiplier(deep_water, in_safe_area, in_dungeon)

	var mult := 1;

	if (in_safe_area)
		mult := 0.5;
	elseif (in_dungeon)
		mult := 2;
	elseif (deep_water)
		mult := 1.5;
	else
		mult := 1;
	endif

	return mult;

endfunction

//Same three buckets fish are catalogued under in catchtable.cfg/itemdesc.cfg's Location CProp.
//Dungeons beats Deep Water since the two are mutually exclusive by design.
function GetFishingLocation(deep_water, in_dungeon)

	var location;

	if (in_dungeon)
		location := "Dungeons";
	elseif (deep_water)
		location := "Deep Water";
	else
		location := "Shores";
	endif

	return location;

endfunction

//Filters catchtable.cfg down to the species eligible for "tier" at this location/realm/skill.
//Location/Realm are optional per species - unset matches anything. Returns 0 if nothing
//qualifies, otherwise a random pick's Objtype (Regular) or a random East/South pick
//(Rare/Legendary, which were built with both facings).
function PickFishFromTier(tier, location, realm, skill)

	var cfg := ReadConfigFile("catchtable");
	var keys := GetConfigStringKeys(cfg);
	var candidates := {};

	foreach key in keys
		var elem := cfg[key];
		if (elem.Tier == tier)
			if ( (!elem.Location or elem.Location == location) and
			     (!elem.Realm or elem.Realm == realm) and
			     (!elem.MinSkill or skill >= CInt(elem.MinSkill)) )
				candidates.append(elem);
			endif
		endif
	endforeach

	UnloadConfigFile("catchtable");

	if (!len(candidates))
		return 0;
	endif

	var chosen := candidates[Random(len(candidates))+1];
	var picked_objtype;

	if (chosen.ObjtypeEast)
		if (Random(2))
			picked_objtype := CInt(chosen.ObjtypeEast);
		else
			picked_objtype := CInt(chosen.ObjtypeSouth);
		endif
	else
		picked_objtype := CInt(chosen.Objtype);
	endif

	return picked_objtype;

endfunction

//Rolls Legendary (1/legendary_divisor), then Rare (1/rare_divisor), else Regular. Falls back a
//tier at a time if nothing's eligible at this location/realm/skill, rather than wasting the
//catch. Returns {objtype, tier} - tier is the tier actually resolved after any fallback, which
//the caller needs to size the bonus skill-point award.
function PickCaughtFish(character, location, rare_divisor, legendary_divisor)

	var skill := GetEffectiveSkill(character, SKILLID_FISHING);
	var realm := character.realm;

	var tier := "Regular";
	if (Random(legendary_divisor) == 0)
		tier := "Legendary";
	elseif (Random(rare_divisor) == 0)
		tier := "Rare";
	endif

	var fish := PickFishFromTier(tier, location, realm, skill);

	if (!fish and tier == "Legendary")
		tier := "Rare";
		fish := PickFishFromTier(tier, location, realm, skill);
	endif

	if (!fish and tier != "Regular")
		tier := "Regular";
		fish := PickFishFromTier(tier, location, realm, skill);
	endif

	return {fish, tier};

endfunction

//Shared by the pole (fishing.src) and the net (fishingnet.src) so both catch from the same
//tables - only rare_divisor/legendary_divisor differ between them. thepoints is whatever base
//skill-gain value the caller's own CheckSkill call used; the Rare/Legendary bonus award is
//sized relative to it (2x/5x), on top of the normal gain CheckSkill already granted.
//
//Preserves the old "magic" stat-boost fish (0xA84-0xA8C) as an independent roll ahead of the
//tiered tables, unchanged from its original odds.
//
//location is pre-computed by the caller (see PickCaughtFish); deep_water is passed separately
//since createfish()'s carve-into-steaks math cares specifically about deep water.
//
//big_lure: when set, a Big lure is charged on the catching gear -- rolls the trophy weight
//twice and keeps the higher result instead of replacing RollTrophyWeight()'s own Min() with
//a Max() (which tested too easy at 37% for the 180-stone quest threshold; best-of-two lands
//closer to 8%). Callers pre-adjust rare_divisor/legendary_divisor themselves for a charged
//Rare/Legendary lure (see fishing.src) rather than this function reading gear directly, so
//it stays usable by the net (fishingnet.src), which never carries a lure.
function ResolveFishCatch(character, fish_amount, equipped, deep_water, location, tool_bon, rare_divisor, legendary_divisor, thepoints, big_lure := 0)

	var currentskill := GetEffectiveSkill(character, SKILLID_FISHING);
	if (Random(currentskill) + 1 >= 100)
		if (tool_bon)
			fish_amount := fish_amount * tool_bon;
		endif
		createfish(character, Random(9) + 0xA84, fish_amount, equipped, deep_water);
		return;
	endif

	var caught := PickCaughtFish(character, location, rare_divisor, legendary_divisor);
	var caught_objtype := caught[1];
	var caught_tier := caught[2];

	if (!caught_objtype)
		//Safety net - shouldn't happen if catchtable.cfg covers every Location/Realm combo.
		SendSysmessage(character, "The fish slips off your hook!");
		return;
	endif

	if (caught_tier == "Regular")
		if (tool_bon)
			fish_amount := fish_amount * tool_bon;
		endif
	else
		fish_amount := 1;
	endif

	//0 if createfish() carved the catch into steaks instead of handing it over - no name
	//message in that case.
	var caughtitem := createfish(character, caught_objtype, fish_amount, equipped, deep_water, caught_tier);

	if (caught_tier == "Rare")
		AwardSkillPoints(character, SKILLID_FISHING, Cint(thepoints * 2));
		if (caughtitem)
			//Yellow hue (1731), matching RandomYellowHue()'s range in
			//pkg/utils/itemUtils/include/colors.inc.
			SendSysmessage(character, "You caught a rare "+caughtitem.desc+"!", 3, 1731);
		endif
	elseif (caught_tier == "Legendary")
		AwardSkillPoints(character, SKILLID_FISHING, Cint(thepoints * 5));
		if (caughtitem)
			//Orange hue (1531), matching RandomOrangeHue()'s range in
			//pkg/utils/itemUtils/include/colors.inc.
			SendSysmessage(character, "You caught a legendary "+caughtitem.desc+"!", 3, 1531);
		endif
	endif

	//Rare/Legendary catches are trophy-eligible (matches OSI's High Seas Taxidermy Kit rules -
	//trophy eligibility is species rarity, not an independent "big fish" roll). caughtitem is 0
	//if createfish() carved it into steaks instead (player opted into AutoCarveRareFish), in
	//which case there's nothing left to tag.
	var trophy_weight := 0;
	if (caughtitem and caught_tier != "Regular")
		trophy_weight := RollTrophyWeight();
		if (big_lure)
			trophy_weight := Max(trophy_weight, RollTrophyWeight());
		endif
		SetObjProperty(caughtitem, "TrophyWeight", trophy_weight);
	endif

	Print("[Fishing] "+character.name+" caught tier="+caught_tier+" objtype="+CStr(caught_objtype)+" weight="+CStr(trophy_weight)+" big_lure="+CStr(big_lure));

	//Counts toward any active "catch fish" quest objective regardless of tier/carve choice -
	//trophy_weight stays 0 (so weight-gated objectives correctly don't fire) unless a Rare/
	//Legendary catch was actually kept as an item above.
	QP_ProcessFishCatch(character, "CatchFish", caught_tier, trophy_weight);

endfunction

//OSI's historical fish-weight record range is 1-225 stones. Taking the min of two rolls skews
//the result toward the low end, so a near-record weight stays rare.
function RollTrophyWeight()
	return Min(Random(225)+1, Random(225)+1);
endfunction

//Looks up trophyart.cfg for a graphic matching base_graphic - the caught item's own .graphic
//(the shared body-shape art every species using that shape, east or south, is built from -
//e.g. 0x337D/0x337E for the "Fish9"/"Fish10" shape pair). Each confirmed base_graphic maps to
//exactly one matching trophy graphic, already facing-correct. Shapes without a confirmed
//dedicated trophy fall back to the single "Generic" pool of plaque graphics.
function PickTrophyGraphic(base_graphic)

	var cfg := ReadConfigFile("trophyart");
	var elem := FindConfigElem(cfg, base_graphic);
	var graphic;

	if (elem)
		graphic := CInt(elem.Trophy);
	else
		var choices := GetConfigStringArray(FindConfigElem(cfg, "Generic"), "Graphics");
		graphic := CInt(choices[Random(len(choices))+1]);
	endif

	UnloadConfigFile("trophyart");

	return graphic;

endfunction

//Consumes a Rare/Legendary catch (fish or crustacean) tagged with TrophyWeight by
//ResolveFishCatch()/CollectTrap() and creates a wall-mount trophy in its place, carrying the
//weight, species, and angler's name for the tooltip block added to
//pkg/packethooks/megacliloc/itemdata.src. The trophy is hued to match the specific fish caught.
function CreateFishTrophy(character, caught_item)

	var weight := GetObjProperty(caught_item, "TrophyWeight");
	var species := caught_item.desc;
	var graphic := PickTrophyGraphic(caught_item.graphic);

	var trophy := CreateItemInBackpack(character, graphic, 1);
	trophy.name := species + " Trophy";
	trophy.desc := species + " Trophy";
	trophy.color := caught_item.color;
	SetObjProperty(trophy, "TrophyWeight", weight);
	SetObjProperty(trophy, "TrophySpecies", species);
	SetObjProperty(trophy, "TrophyAngler", character.name);

	//Renaming/recoloring an item after creation doesn't push a fresh MegaCliloc tooltip on its
	//own (see the "only sent if this value has changed" note in toolTips.src) - bump the
	//revision so the client actually re-fetches the new name/weight/angler text.
	IncRevision(trophy);

	DestroyItem(caught_item);
	SendSysmessage(character, "You mount the "+species+" as a trophy!");

endfunction

function whatequipped(who, pole)

var item := GetEquipmentByLayer( who, 2 );
var object1:=item.graphic;

item := GetEquipmentByLayer( who, 1 );
var object2:=item.graphic;
var hands:=0;

if (object1==0xDBF) hands := 1; endif
if (object1==0x26BA) hands := 1; endif

if (object2==0xF51 or object2==0x13f6) hands := 2; endif

if (hands==0 and pole<>666)
if (equipitem(who,pole)) hands:=1; endif
endif

return hands;

endfunction

function check_for_water (theobj)
	//static water tiles
	if( (theobj.objtype >=0x346E) and (theobj.objtype <= 0x3485) )
		return 1;
	elseif( (theobj.objtype >= 0x3494) and (theobj.objtype <= 0x34AB) )
		return 1;
	elseif( (theobj.objtype >= 0x34B8) and (theobj.objtype <= 0x34BB) )
		return 1;
	elseif( (theobj.objtype >= 0x34BD) and (theobj.objtype <= 0x34C0) )
		return 1;
	elseif( (theobj.objtype >= 0x34C2) and (theobj.objtype <= 0x34C5) )
		return 1;
	elseif( (theobj.objtype >= 0x34C7) and (theobj.objtype <= 0x34CA) )
		return 1;
	elseif( (theobj.objtype >= 0x1796) and (theobj.objtype <= 0x17B2) )
		return 1;
	elseif(IsWater(theobj))
		return 1;
	else
		return 0;
	endif
endfunction

function IsWaterTile(x, y, realm)
	var info := GetMapInfo(x, y, realm);
	var tile := info.landtile;

	if( tile >= 0x04c and tile <= 0x06f )
		return 1;
	elseif( tile >= 0x0a8 and tile <= 0x0ab )
		return 1;
	elseif( tile == 0x0136 or tile == 0x0137 )
		return 1;
	elseif( tile >= 0x3ff0 and tile <= 0x3ff3 )
		return 1;
	endif

	return 0;
endfunction

function IsPlankObjtype(objtype)
	if( (objtype >= 0x1CB and objtype <= 0x1CE) or
	    (objtype == 0x3A5) or
	    (objtype >= 0x3AA and objtype <= 0x3B0) or
	    (objtype >= 0x4A1 and objtype <= 0x4A4) or
	    (objtype >= 0x4C6 and objtype <= 0x4E1) or
	    (objtype == 0x7C9) or
	    (objtype >= 0x7CC and objtype <= 0x7D0) or
	    (objtype == 0x3E84) or (objtype == 0x3E86) or (objtype == 0x3E89) or
	    (objtype >= 0x3ED3 and objtype <= 0x3ED5) )
		return 1;
	endif
	return 0;
endfunction

function IsNearDockPlanks(x, y, realm, radius)
	foreach item in ListItemsNearLocation(x, y, LIST_IGNORE_Z, radius, realm)
		if(IsPlankObjtype(item.objtype))
			return 1;
		endif
	endforeach

	foreach item in ListStaticsNearLocation(x, y, LIST_IGNORE_Z, radius, 0, realm)
		if(IsPlankObjtype(item.objtype))
			return 1;
		endif
	endforeach

	return 0;
endfunction

function IsDeepWater(x, y, realm, radius := 16)
	var r, dx, dy;

	if(!IsWaterTile(x, y, realm))
		return 0;
	endif

	if(IsNearDockPlanks(x, y, realm, radius))
		return 0;
	endif

	for(r:=1; r<=radius; r:=r+1)
		for(dx:=-r; dx<=r; dx:=dx+1)
			if(!IsWaterTile(x+dx, y-r, realm)) return 0; endif
			if(!IsWaterTile(x+dx, y+r, realm)) return 0; endif
		endfor
		for(dy:=-r+1; dy<=r-1; dy:=dy+1)
			if(!IsWaterTile(x-r, y+dy, realm)) return 0; endif
			if(!IsWaterTile(x+r, y+dy, realm)) return 0; endif
		endfor
	endfor

	return 1;
endfunction

//Deprecated code
/*
function povernut(ch, us)
    var xd, yd;

    xd := ch.x - us.x;
    yd := ch.y - us.y;
	var signt;


if ((xd == 0) or (yd == 0))
	if (xd == 0)
		if(yd >= 0)
			ch.facing:=0;
		else
			ch.facing:=4;
		endif
	else
		if(xd >= 0)
			ch.facing:=6;
		else
			ch.facing:=2;
		endif
	endif
else
    if (xd < 0)
	if (yd < 0)
		signt := 1;
	else
		signt := 2;
	endif
    else
	if (yd < 0)
		signt := 3;
	else
		signt := 4;
	endif
    endif
	case (signt)
        1: if ( Tan(-xd, -yd)>=0.4142)
		if (Tan(-xd, -yd)<=2.4142)
			ch.facing:=3;
		else
			ch.facing:=2;
		endif
	   else
			ch.facing:=4;
	   endif
        2: if ( Tan(-xd, yd)>=0.4142)
		if (Tan(-xd, yd)<=2.4142)
			ch.facing:=1;
		else
			ch.facing:=2;
		endif
	   else
			ch.facing:=0;
	   endif
        3: if ( Tan(xd, -yd)>=0.4142)
		if (Tan(xd, -yd)<=2.4142)
			ch.facing:=5;
		else
			ch.facing:=6;
		endif
	   else
			ch.facing:=4;
	   endif
        4: if ( Tan(xd, yd)>=0.4142)
		if (Tan(xd, yd)<=2.4142)
			ch.facing:=7;
		else
			ch.facing:=6;
		endif
	   else
			ch.facing:=0;
	   endif

 	endcase

endif
endfunction

function CreateSosBottle(char)

var cfg := ReadConfigFile("sosarea");
var maxI := GetConfigMaxIntKey(cfg);
var sosnum := Random(maxI)+1;
if ( sosnum > 10 )
	sosnum := 10;
endif
var elem := cfg[Random(sosnum)];
var X := cint(elem.x)+Random(cint(elem.range)*2) - cint(elem.range);
var Y := cint(elem.y)+Random(cint(elem.range)*2) - cint(elem.range);
var color := cint(elem.color);

// var Z := GetWorldHeight(X,Y);

// syslog(X+" "+Y+" "+Z);

var bottle :=CreateItemInContainer(char.backpack, 0x9600);

SetObjProperty(bottle, "tileX", X);
SetObjProperty(bottle, "tileY", Y);
SetObjProperty(bottle, "colormes", color);
// SetObjProperty(bottle, "tileZ", (Z+1));

UnloadConfigFile( "sosarea");

endfunction
*/
