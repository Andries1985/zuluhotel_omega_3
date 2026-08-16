use uo;
use cfgfile;
use os;

include "include/client";
include "include/random";
include "include/itemutil";
include "include/attributes";

function RandomFish(character)

var currentskill := GetEffectiveSkill( character, SKILLID_FISHING);
var thisone := Random(currentskill) + 1;
var fishtype;

if (thisone<50 )
	fishtype := Random(UOBJ_FISH_END - UOBJ_FISH_START);
	fishtype := UOBJ_FISH_START + fishtype;
elseif (thisone<100)
	fishtype := Random(UOBJ_SFISH_END - UOBJ_SFISH_START);
	fishtype := UOBJ_SFISH_START + fishtype;
else
        fishtype := Random(9) + 0xA84;
endif
return fishtype;
endfunction

function createfish( who,fish,fish_amount, equipped, deepwater := 1 )


	if (equipped==1 or fish == 0xA84 or fish == 0xA85 or fish == 0xA86 or fish == 0xA87 or fish == 0xA88 or fish == 0xA89 or fish == 0xA8A or fish == 0xA8B or fish == 0xA8C )
		CreateItemInBackpack(who, cint(fish), cint(fish_amount));
	else
		PlaySoundEffect(who, SFX_57);
		PerformAction( who, ANIM_SALUTE );
		var steaks_per_fish := 4;
		if (!deepwater)
			steaks_per_fish := 1;
		endif
		CreateItemInBackpack(who, UOBJ_FISHSTEAK, (steaks_per_fish * fish_amount));
		SendSysmessage(who, "You carve up the fish.");
	endif

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

//Depreciated Code
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