/////////////////////////////////////////////////////////////////////////////
// Shared logic for city-bound civilian NPCs.
/////////////////////////////////////////////////////////////////////////////

include "include/anchors";
include "include/eventid";
include "include/client";
include "include/random";

function SayRandomFromArray(arr)
	// 2026-09-23: was Random(10) over tables filled from index 0; arrays start at 1 here, so
	// the first line of each table was rejected and one draw in ten said an error value.
	var idx := Random(len(arr)) + 1;
	say(arr[idx]);
endfunction

var DISENGAGED_SAYINGS;
DISENGAGED_SAYINGS[1] := "Is it over?";
DISENGAGED_SAYINGS[2] := "Thank the heavens!";
DISENGAGED_SAYINGS[3] := "I survived... somehow.";
DISENGAGED_SAYINGS[4] := "That was too close!";
DISENGAGED_SAYINGS[5] := "I hope they're gone for good.";
DISENGAGED_SAYINGS[6] := "Back to safety at last.";
DISENGAGED_SAYINGS[7] := "I need a drink after that!";
DISENGAGED_SAYINGS[8] := "I thought I was done for.";
DISENGAGED_SAYINGS[9] := "Maybe I should lay low for a while.";
DISENGAGED_SAYINGS[10] := "Thank you, whoever helped me!";

var ENGAGED_SAYINGS;
ENGAGED_SAYINGS[1] := "Help! Help! I'm being attacked!";
ENGAGED_SAYINGS[2] := "Somebody stop this madness!";
ENGAGED_SAYINGS[3] := "Guards! Guards!";
ENGAGED_SAYINGS[4] := "Why are you doing this?";
ENGAGED_SAYINGS[5] := "Please, I mean no harm!";
ENGAGED_SAYINGS[6] := "No! Stay away from me!";
ENGAGED_SAYINGS[7] := "Mercy! Mercy!";
ENGAGED_SAYINGS[8] := "What did I do to deserve this?";
ENGAGED_SAYINGS[9] := "Leave me alone!";
ENGAGED_SAYINGS[10] := "I just want to live!";

var DAMAGED_SAYINGS;
DAMAGED_SAYINGS[1] := "Ouch! Heeelp!";
DAMAGED_SAYINGS[2] := "That hurts! Stop it!";
DAMAGED_SAYINGS[3] := "Aaaagh! Why?!";
DAMAGED_SAYINGS[4] := "By the virtues, that stings!";
DAMAGED_SAYINGS[5] := "No more, please!";
DAMAGED_SAYINGS[6] := "I'm bleeding!";
DAMAGED_SAYINGS[7] := "You fiend!";
DAMAGED_SAYINGS[8] := "Is this the end for me?";
DAMAGED_SAYINGS[9] := "I need a healer!";
DAMAGED_SAYINGS[10] := "Someone, save me!";

function FleeOpenDistance(opponent)
	var me := Self();
	case (Distance(me, opponent))
		10:
			return 1;
		default:
			TryOpenNearbyNpcDoors();
			RunAwayFrom(opponent);
			return 0;
	endcase
endfunction

function RunFromOpponent(opponent, HALT_THRESHOLD)
	TurnAwayFrom(opponent);
	DisableEvents(SYSEVENT_ENTEREDAREA + SYSEVENT_LEFTAREA);

	var waittime;
outer:
	while (opponent and !opponent.dead)
		if (!EnforceTownBounds(1))
			break outer;
		endif

		waittime := FleeOpenDistance(opponent) * 15;

		var ev := wait_for_event(waittime);
		case (ev.type)
			SYSEVENT_SPEECH:
				if (ev.text["hold"])
					say("Very well then.");
					break outer;
				endif

			SYSEVENT_DISENGAGED:
				if (ev.source.serial == opponent.serial)
					SayRandomFromArray(DISENGAGED_SAYINGS);
					break outer;
				endif

			SYSEVENT_ENGAGED:
			SYSEVENT_DAMAGED:
				if (ev.source)
					if (ev.source.serial != opponent.serial)
						SayRandomFromArray(ENGAGED_SAYINGS);
					else
						if (ev.type == SYSEVENT_DAMAGED) // 2026-09-23: was the word "damage"; event types are numbers
							SayRandomFromArray(DAMAGED_SAYINGS);
						endif
					endif
				endif

			SYSEVENT_OPPONENT_MOVED:

			SYSEVENT_ENTEREDAREA:
			SYSEVENT_LEFTAREA:
				// ignore
		endcase
	endwhile

	EnableEvents(SYSEVENT_ENTEREDAREA + SYSEVENT_LEFTAREA, HALT_THRESHOLD);
	SetWarMode(0);
endfunction

function singsongs(singer, character)
	var counting := 1;
	var songcfg := ReadConfigFile("songs");
	var maxsongs := FindConfigElem(songcfg, "songs");
	var numsongs := GetConfigInt(maxsongs, "number");
	var songnumb := Random(numsongs) + 1;
	var songelem := FindConfigElem(songcfg, songnumb);
	var midinum := GetConfigString(songelem, "midi");
	var number := GetConfigInt(songelem, "count");

	if (midinum)
		PlayMidi(singer, character, midinum);
	else
		PlaySFX(character);
	endif

	while (counting <= number)
		var linename := "line" + CInt(counting);
		var song := GetConfigString(songelem, linename);
		say(song);
		sleep(4);
		counting := counting + 1;
	endwhile
endfunction

function PlayMidi(singer, unused character, song_id)
	// 2026-09-23: the loop reused the listener's name for every online player and tested the
	// first listener's distance for all of them, so the tune went to the whole shard.
	var packet_id := "6D00" + song_id;
	foreach listener in EnumerateOnlineCharacters()
		if (listener.realm == singer.realm)
			if (Abs(singer.x - listener.x) <= 15 and Abs(singer.y - listener.y) <= 15)
				SendPacket(listener, packet_id);
			endif
		endif
	endforeach
endfunction

function PlaySFX(character)
	var randy := Random(4);

	case(randy)
		0: playsoundeffect(character, SFX_MUSIC_LUTE_GOOD);
		1: playsoundeffect(character, SFX_MUSIC_HARP_LAP_GOOD);
		2: playsoundeffect(character, SFX_MUSIC_DRUM_GOOD);
		3: playsoundeffect(character, SFX_MUSIC_TAMBORINE_GOOD);
	endcase
endfunction