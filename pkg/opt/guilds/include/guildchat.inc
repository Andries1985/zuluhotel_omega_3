use uo;
use os;
use guilds;

include "guildconstants";

function chatally(who, msg)

if (!who.guild)
	return 0;
endif

if (!CheckSquelch(who))
	return 0;
endif

var guild := who.guild;
msg := "[A] "+who.name+" ["+guild.getprop("GuildAbv")+"]: "+msg;
var personchat;

foreach person in (guild.members)
	personchat := GetObjProperty(person, "GuildChat");
	if (personchat[1] != HEAR_NONE)
		SendSysMessage(person, msg, 3, 68);
	endif
endforeach

foreach allyguild in (guild.allyguilds)
	foreach person in (allyguild.members)
		personchat := GetObjProperty(person, "GuildChat");
		if (personchat[1] != HEAR_NONE && personchat[1] != HEAR_OWN)
			SendSysMessage(person, msg, 3, 1176);
		endif
	endforeach
endforeach

	foreach player in EnumerateOnlineCharacters()
		if( player.cmdlevel >= 3)
			if(GetObjProperty(player, "EarsEverywhere"))
				SendSysMessage(player, msg, 3, 56);
			endif
		endif
	endforeach
endfunction

function chatown(who, msg)

if (!who.guild)
	return 0;
endif

if (!CheckSquelch(who))
	return 0;
endif

var guild := who.guild;
msg := "[O] "+who.name+" ["+guild.getprop("GuildAbv")+"]: "+msg;
var personchat;

foreach person in (guild.members)
	personchat := GetObjProperty(person, "GuildChat");
	if (personchat[1] != HEAR_NONE)
		SendSysMessage(person, msg, 3, 68);
	endif
endforeach

	foreach player in EnumerateOnlineCharacters()
		if( player.cmdlevel >= 3)
			if(GetObjProperty(player, "EarsEverywhere"))
				SendSysMessage(player, msg, 3, 56);
			endif
		endif
	endforeach
endfunction

function chatall(who, msg)

if (!who.guild)
	return 0;
endif

if (!CheckSquelch(who))
	return 0;
endif

var guild := who.guild;
msg := who.name+" ["+guild.getprop("GuildAbv")+"]: "+msg;
var personchat;

foreach otherguild in (ListGuilds())
	foreach person in (otherguild.members)
		personchat := GetObjProperty(person, "GuildChat");
		if (guild.ismember(person))
			if (personchat[1] != HEAR_NONE)
				SendSysMessage(person, msg, 3, 68);
			endif
		elseif (otherguild.isallyguild(guild))
			if (personchat[1] != HEAR_NONE && personchat[1] != HEAR_OWN)
				SendSysMessage(person, msg, 3, 1176);
			endif
		else
			if (personchat[1] == HEAR_ALL)
				SendSysMessage(person, msg, 3, 1585);
			endif
		endif
	endforeach
endforeach

endfunction

function CheckSquelch(who)

var squelch:= GetObjProperty(who, "SilenceOpts")[3];
if (squelch)
	if (squelch == 1)
		SendSysMessage(who, "Your guildchat priveleges have been revoked until further notice");
		return 0;

	// Not in yet, add optional timer in .silence
	elseif (ReadGameClock() < squelch)
		SendSysMessage(who, "You cannot guildchat for another "+CInt((squelch - ReadGameClock()) / 60)+" minutes");
		return 0;
	endif

endif
	return 1;

endfunction