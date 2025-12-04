include "ai/main/npcinfo";
include "include/areas";
include "include/attributes";
include "include/npcboosts";


function main_AI_loop()

    var next_wander := ReadGameClock() + 10;
    var ev;
    var wanders := 120;


        look_around();


    EnableMainEvents();
 
    while (1)

	if(!GetObjProperty(me, "lbc") || GetObjProperty(me, "lbc") < ReadGameClock())
		SetObjProperty(me, "lbc", ReadGameClock()+5);
		checkmymod(me);
	endif
    
	if (readgameclock() > next_wander)
		wanders := wanders +1;
		next_wander := readgameclock()+120;
		if (wanders > 60)
			wanders :=0;
			ev := sleepmode();
	    	endif
	else
		ev := os::wait_for_event( 120 );
	endif

        repeat
	    if (ev)
            case (ev.type)
                SYSEVENT_ENTEREDAREA:
                    if ( (!ev.source.npctemplate) or (ev.source.script == "tamed") || GetObjProperty(ev.source, "NPCAttack"))
								var resProt := GetObjProperty(ev.source, "ResProtection");
								if(resProt)
									if(resProt < readgameclock())
										Fight( ev.source );
									endif
								else
										Fight( ev.source );
								endif
                    endif
                SYSEVENT_ENGAGED:
                SYSEVENT_DAMAGED:
                    if(!IsInSafeArea(me))
                        Fight( ev.source );
                    endif
            endcase
	    endif
        until (! (ev := os::wait_for_event(120)) );
   endwhile      
endfunction        

function look_around()
	return;
endfunction

function EnableMainEvents()

    DisableEvents( SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED );
    EnableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    DisableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);

endfunction
