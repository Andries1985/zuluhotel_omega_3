include "include/areas";
include "include/npcboosts";

function main_AI_loop(next_wander := 0)

    var ev;
    var wanders := 60;
    var next_loot := readgameclock() + 20;

    EnableMainEvents();

    while (1)

	if(!GetObjProperty(me, "lbc") || GetObjProperty(me, "lbc") < ReadGameClock())
		SetObjProperty(me, "lbc", ReadGameClock()+5);
		checkmymod(me);
	endif
    
	if (readgameclock() > next_wander)
        	wander_around();
		next_wander := readgameclock()+1;
		wanders := wanders +1;
		if (wanders > 60)
			wanders :=0;
			ev := sleepmode();
    		endif
	else
		ev := os::wait_for_event( 3 );
	endif

        repeat

            case (ev.type)
                SYSEVENT_ENTEREDAREA:
                        if(!IsInSafeArea(me))
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
                        endif
                SYSEVENT_DAMAGED:
                SYSEVENT_ENGAGED:
                    if(!IsInSafeArea(me))
                        Fight( ev.source );
                    endif
                default:
		    process_noncombat_event(ev);
            endcase

        until (! (ev := os::wait_for_event(1)) );

   endwhile      

endfunction        

function look_around()
	return;
endfunction

function EnableMainEvents()

    cleareventque();
    DisableEvents( SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED );
    EnableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD );

endfunction

function DisableMainEvents()

    cleareventque();
    DisableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);

endfunction
