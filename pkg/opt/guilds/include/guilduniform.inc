////////////////////////////////////////////////////////////////////////////////////////////////////////////
// guilduniform.inc - guild uniform colour handling for the equip/unequip control scripts.
//
// 2026-09-25: split out of guilds.inc. scripts/control/skilladvancerequip.src and skilladvancerunequip.src
// run for every equip and unequip on the shard (and once per equipped item at world load) and only need
// these three functions, so they include this file instead of guilds.inc and its housing/itemdesc/gump
// include tree. guilds.inc includes this file, so every other guild script still sees the functions.
// Keep this file free of file-scope work: anything here runs at the start of every equip and unequip.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
use uo;
use os;

include "guildconstants";

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EquipGuildUniform() - Set's the uniform colour on equip
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function EquipGuildUniform(who, item)

	if(GetObjProperty(item, "EquippedAlready"))
		return 1;
	endif

	var guild := who.guild;
	var colour := guild.getprop(GUILD_COLOUR);

	if (!guild)
		SendSysMessage(who, "You need a guild to wear a uniform!");
		return 0;
	elseif (!colour)
		SendSysMessage(who, "Your guild needs a guild colour before you can wear that!");
		return 0;
	endif

	PlaySoundEffectPrivate(who, 0x457, who);
	PlayObjectCenteredEffectEx( who, 14186, 10, 10,colour, 0, 7);
	item.color := colour;

	return 1;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnEquipGuildUniform() - Set's the uniform colour on unequip
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function UnEquipGuildUniform(who, item)

	if (GetObjProperty(item, "Entwined"))
		item.color := GetObjProperty(item, "OriginalColour");
		if (who.dead)
			UnEntwine(who, item);
		endif
		return 1;
	endif

	if (!GetObjProperty(item, GUILD_UNIFORM))
		return 1;
	endif

	item.color := 0;

	return 1;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnEntwine() - does just that
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function UnEntwine(who, uniform)

	var name := SplitWords(uniform.desc);
	var newname, count := 0;

	foreach thing in name
		if(count == 0)
			newname := thing;
			count := 1;
		elseif ( thing != "[Entwined]" )
			newname := newname+" "+thing;
		endif
	endforeach

	SetName(uniform, newname);
	uniform.color := GetObjProperty(uniform, "OriginalColour");

	var uni := SystemFindObjectBySerial(GetObjProperty(uniform, "Entwined"));

	if (!uni)
		Broadcast("This is odd, your uniform is gone!, page staff!",3, 33);
		syslog(who.name+"/"+who.serial+" This is odd, your uniform is gone!, page staff!");
		return 0;
	endif

	EraseObjProperty(uniform, "Entwined");
	EraseObjProperty(uniform, "OriginalColour");
	uniform.usescript := "";

	uni.movable := 1;
	uni.invisible := 0;
	uni.graphic := GetObjProperty(uni, "Graphic");
	MoveItemToContainer( uni, who.backpack );
endfunction
