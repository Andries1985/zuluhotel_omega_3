use npc;
use cfgfile;

//include "../pkg/mobiles/npcs/include/NPCBackpacks";
include "include/random";


var speech_cfg := ReadConfigFile("speechgroup");

function process_text(inspeech, mood, templatename)
	var retval, wordgroup, elem, tmpword;
	var words := SplitWords(inspeech);
	elem := speech_cfg[templatename];

	foreach word in words
			if(mood && elem)
				wordgroup := GetConfigInt(elem, mood);
			if (!wordgroup)
				wordgroup := GetConfigInt(elem, "default");
			endif
			if (!wordgroup)
				wordgroup := templatename;
			endif
			elem := speech_cfg[wordgroup];
			tmpword := GetConfigString(elem,  word );
			if (tmpword) 
				retval := tmpword;
			endif
			endif
	endforeach

	if (!retval)
			retval := GetConfigString(elem, "default" );
	endif

	if ( (!retval) or (retval == "5") )
			retval := "";
	endif

	return retval;

endfunction

function TakeItem(me, you, it, speech)

	TurnToward(you);
	if (!getobjproperty(me,"serial"))
		setobjproperty(me, "serial", me.serial);
	endif
	//var mypack := FindMyPack(me.serial);

	var whatdesc := "";
	if (!len(it.name))
    	whatdesc := it.desc;
    else
		whatdesc := it.name;
    endif

	var keyitem := lower("keyitem" + whatdesc);
	var returnthing := lower("return" + whatdesc);

	var reply := GetObjProperty( me, keyitem );
	if(reply)
		TurnToward (you );
		PrintTextAbovePrivate(me, reply, you);
	else
		if(speech)
			if( GetObjproperty(it,"slave") )
				TurnToward( you );
				GiveQuestieDirections( you, it );
			else
				TurnToward( you );
				PrintTextAbovePrivate( me, "Eh? What's this?", you );
			endif
		endif
		MoveItemToContainer( it, you.backpack );
		return;
	endif

	// var gavething := 0;
	// Foreach item in EnumerateItemsInContainer(mypack)
	// 	var returnit := GetObjProperty(item,"returnfor");
	// 	if (returnit == returnthing)
	// 		gavething := 1;
	// 		EraseObjProperty(item,"returnfor");
	// 		MoveItemToContainer(item, you.backpack);
	// 	endif
	// endforeach

	// if (!me.backpack)
	// 	var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75, 1, me.realm);
	// 	if (!EquipItem(me, newbackpack))
	// 		return;
	// 	endif
	// endif

	// Foreach item in EnumerateItemsInContainer(me.backpack)
	// 	var returnit := GetObjProperty(item,"returnfor");
	// 	if (returnit == returnthing)
	// 		gavething := 1;
	// 		EraseObjProperty(item,"returnfor");
	// 		MoveItemToContainer(item, you.backpack);
	// 	endif
	// endforeach

	// if (!gavething)
	// 	return;
	// endif

	var newresponse := GetObjProperty( me, "done"+keyitem);
	if (newresponse)
		SetObjProperty( me, keyitem, newresponse);
		EraseObjProperty( me, "done"+keyitem);
	else
		EraseObjProperty(me,keyitem);
	endif

endfunction

function check_speech(me, speaker, text, speech)

	//Print"Speech check: " + text + " from " + speaker.name + " to " + me.name );
	//Print"Speech property is: " + speech );

	if (speaker.cmdlevel && speaker.concealed)
		//Print("Speaker is a concealed admin, exiting speech check.");
		return;
	endif

	if (BarkToKeyWord(me, text, speaker))
		//Print("Barked to keyword, exiting speech check.");
		return;
	endif

	if (DefaultHello(me, speaker, text))
		//Print("Default hello processed, exiting speech check.");
		return;
	endif

	if (!speech)
		//Print("No speech property set, exiting speech check.");
		return;
	endif

	if (Random(20) != 1 )
		//Print("Random check failed, exiting speech check.");
		return;
	endif

	If ( GetObjProperty(me,"nofriends") )
		//Print("NPC set to nofriends, exiting speech check.");
		return;
	endif

	//Print("Check Speech Passed checks, processing friend speech.");

	var myfriends := { };
	var i := 0;

    for( i := 1 ; i <= 3 ; i := i+1 )	
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			//Print("Empty friend slot " + i );

			break;
		elseif (myfriends[i] == speaker.name)
			if ( (i == 1) or (Random(2)==1) )
				TurnToward( speaker );
				//Print("Recognized friend " + speaker.name + " in slot " + i );
				GreetFriend( me, text, speaker, i );
				return;
			else
				TurnToward( speaker );
				//Print("Choosing to chat about friend " + speaker.name + " in slot " + i );
				ChatAboutFriends(me, text, speaker, i-1, speech);
				return;
			endif
		endif
	endfor

	//the speaker is not a friend yet
	var maxfriend := i - 1;

	if (i > 3)
		//too many friends, pick a slot at random
		i := Random(3)+1;
		maxfriend := 3;
	endif

	//check loyalty to the friend in that friendslot
	var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty:=0;
	endif

	if ( Random(20) <= (loyalty+10) )
		//don't ditch the old friend
		TurnToward( speaker );
		i := Random(maxfriend)+1;
		ChatAboutFriends(me, text, speaker, i, speech);
		return;
	endif

	var nexthello := GetObjProperty(me,"nexthello");
	if (!nexthello)
		nexthello := ReadGameClock();
	endif
	if ( nexthello <= ReadGameClock() )
		//make a new friend
		TurnToward( speaker );
		SetObjProperty(me,"friend"+i,speaker.name);
		SetObjProperty(me,"loyalty"+i,1);
		SetObjProperty(me,"nexthello",ReadGameClock()+300);
	else
		TurnToward( speaker );
		i := Random(maxfriend)+1;
		ChatAboutFriends(me, text,speaker, i, speech);
	endif

endfunction

function GreetFriend(me, text, friend, friendnum)

	text := text;
	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	var nexthello := GetObjProperty(me,"nexthello");

	if (!loyalty)
		//Print("No loyalty found, setting to 0.");
		loyalty := 0;
	endif

	if (!nexthello)
		//Print("No nexthello found, setting to current game clock.");
		nexthello := ReadGameClock();
	endif

	if (loyalty >= 3)
		friend := "My "+ GetFriendTitle(loyalty) + friend;
		//Print("Friend title added, greeting " + friend );
	endif

	//Print("Greet Friend Checks Passed");

	var chatcfg := ReadConfigFile("::friendchat");
	var elem := FindConfigElem( chatcfg, "talktofriend" );
	var spamlist := GetConfigStringArray( elem, "say" );
	var spamline := spamlist[ Random(len(spamlist))+1 ];

	spamline["friendname"] := friend.name;

	//Print("Greeting friend, saying: " + spamline );
	
	PrintTextAbovePrivate(me, spamline, friend);

	if ( Random(8) == 1 )
		loyalty := loyalty + 1;
		if (loyalty > 9)
			loyalty := 9;
		endif
	endif

	if ( nexthello <= ReadGameClock() )
		SetObjProperty(me,"nexthello",ReadGameClock()+600);
		SetObjProperty(me,"loyalty"+friendnum,loyalty);
	endif

endfunction

function ChatAboutFriends(me, text, speaker, friendnum, speech)

	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	if (!loyalty)
		loyalty := 0;
	endif
	var friend := GetObjProperty(me,"friend"+friendnum);

	if ( (!friend ) or ( Random(3)==1 ) )
		//Print("No friend found or random chat failed, defaulting speech.");
		PrintTextAbovePrivate(me, process_text( text, "default", speech ), speaker);
		return;
	endif

	if (loyalty >= 3)
		friend := "My "+ GetFriendTitle(loyalty) + friend;
		//Print("Friend title added, chatting about " + friend );
	endif

	var chatcfg := ReadConfigFile("::friendchat");
	var elem := FindConfigElem( chatcfg, "talkaboutfriends" );
	var spamlist := GetConfigStringArray( elem, "say" );
	var spamline := spamlist[ Random(len(spamlist))+1 ];

	spamline["charname"] := speaker.name;
	spamline["friendname"] := friend;
	
	//Print("Chatting about friend, saying: " + spamline );
	PrintTextAbovePrivate(me, spamline, speaker);

endfunction

function GetFriendTitle(loyalty)

	case (loyalty)
	3:
	4:
		return "Friend, ";
	5: 
		return "Good Friend, ";
	6: 
		return "Great Friend, ";
	7: 
		return "Old Friend, ";
	8: 
		return "Dear Friend, ";
	9: 
		return "Dearest Friend, ";
	default: 
		return "";
	endcase

endfunction

function YellToFriend(me, friend)

	if (friend.npctemplate)
		return;
	endif

	var myfriends := { };
	var i := 0;
	var afriend := 0;

        for( i := 1 ; i <= 5 ; i := i + 1 )	
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			break;
		elseif (myfriends[i] == friend.name)
			afriend := 1;
			break;
		endif
	endfor

	if (!afriend)
		return;
	endif

	TurnToward(friend);
	var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty := 0;
	endif

	var friendname := friend.name + "!";

	if (loyalty >= 3)
		friendname := "My "+ GetFriendTitle(loyalty) + friendname;
	endif

	TurnToward ( friend );
	PrintTextAbovePrivate(me, friendname, friend);

endfunction

function BarkToKeyWord(me, text,speaker)

	//Print("Entering BarkToKeyWord with text: " + text + " from " + speaker.name + " to " + me.name );
	var words := SplitWords(text);
	//Print("Checking keywords for speech: " + words );
	var tmpword;
	var theword;
	var resp := 0;
	foreach word in words
		tmpword := GetObjProperty( me, "keyword"+ lower(word));
		if (tmpword)
			TurnToward( speaker );
			PrintTextAbovePrivate(me, tmpword, speaker);
			theword := lower(word);
		  	resp := 1;
			//Print("Barked to keyword, exiting BarkToKeyWord check.");
			break;
		endif
	endforeach

	if (!resp)
		return 0;
	endif

	// if (!getobjproperty(me,"serial"))
	// 	setobjproperty(me, "serial", me.serial);
	// endif
	// var mypack := FindMyPack(me.serial);

	 var returnthing := "return" + theword;
	 var doneword := "done" + theword;
	 var keyword := "keyword" + theword;

	// var gavething := 0;
	// Foreach item in EnumerateItemsInContainer(mypack)
	// 	var returnit := GetObjProperty(item,"returnfor");
	// 	if (returnit == returnthing)
	// 		gavething := 1;
	// 		EraseObjProperty(item,"returnfor");
	// 		MoveItemToContainer(item, speaker.backpack);
	// 	endif
	// endforeach

	// if (!me.backpack)
	// 	var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75, 1, me.realm);
	// 	if (!EquipItem(me, newbackpack))
	// 		return;
	// 	endif
	// endif

	// Foreach item in EnumerateItemsInContainer(me.backpack)
	// 	var returnit := GetObjProperty(item,"returnfor");
	// 	if (returnit == returnthing)
	// 		gavething := 1;
	// 		EraseObjProperty(item,"returnfor");
	// 		MoveItemToContainer(item, speaker.backpack);
	// 	endif
	// endforeach

	// if (!gavething)
	// 	return 1;
	// endif

	var newresponse := GetObjProperty( me, doneword);
	//Print("New response for keyword " + theword + " is: " + newresponse );
	if (newresponse)
		//Print("Setting new response for keyword " + theword );
		SetObjProperty( me, keyword, newresponse);
		EraseObjProperty( me, doneword);
	else
		//Print("Erasing keyword property for " + theword );
		EraseObjProperty( me, keyword);
		RemoveFromArray(me, keyword);
	endif

	return 1;

endfunction

function DefaultHello(me, speaker, text)

	text := text;
	if ( GetObjProperty(me,"#lastgreet") )
		//Print( "Has last greet property." );
		if ( ReadGameClock() > GetObjProperty(me,"#lastgreet") )
			BarkToKeyWord( me, "hi", speaker );
			SetObjProperty( me, "#lastgreet", ReadGameClock()+30 );
			return 1;
		else
			return 0;
		endif
	else
		//Print("No last greet property." );
		BarkToKeyWord( me, "hi", speaker );
		SetObjProperty( me, "#lastgreet", ReadGameClock()+30 );
		return 1;
	endif

endfunction

function GiveQuestieDirections(you, it)
	var me := self();

	var questie := SystemFindObjectBySerial(cint(GetObjproperty(it,"slave")));
	if (!questie)
		PrintTextAbovePrivate(me, "I don't think you'll ever find who you are looking for.", it);
		return;
	endif

	var ns_desc := "";
	var ew_desc := "";
	var dir;

	if (questie.y < you.y)
        	ns_desc := "north";
	elseif  (questie.y > you.y)
		ns_desc := "south";
	endif

	if (questie.x < you.x)
		ew_desc := "west";
	elseif  (questie.x > you.x)
		ew_desc := "east";
	endif

	if (ns_desc == "" and ew_desc == "")
		dir := "nearby";
	else 
		dir := ns_desc + ew_desc ;
	endif

	PrintTextAbovePrivate(me, "Looking for " + questie.name + "?  I think they are " + dir, it );

endfunction

function RemoveFromArray( me, keyword )

	var newarray := {};
	var mykeywords := {};
	if ( GetObjProperty(me,"mykeywords") )
		mykeywords := GetObjProperty(me,"mykeywords");
	endif

	foreach word in mykeywords
		if ( word != keyword )
			newarray[ len(newarray)+1 ] := word;
		endif
	endforeach

	SetObjProperty(me,"mykeywords", newarray);

endfunction
