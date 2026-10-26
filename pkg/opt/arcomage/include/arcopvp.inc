// Arcomage player vs player: shared constants, unlocks, gumps and the per-seat board view.
// 2026-10-25: phase 2. The controller is pvpmatch.src; each player (and each watcher) has a
// pvpseat.src that shows gumps and reports clicks back as events. No wagers yet (phase 2b)
// and no PvP records.
use uo;
use os;

include ":arcomage:arcocards";
include ":arcomage:arcorecords";
include ":arcomage:arcogump";

// 2026-10-25: 1 = trace PvP games to the console ("[Arcomage PvP]"); separate from the solo
// ARCO_DEBUG so one can be on without the other. Set to 0 once PvP is confirmed working.
const ARCO_PVP_DEBUG := 1;

// Pid of the PvP controller a character is playing in, so .arcomage watch can find the game.
const ARCO_PVP_PROP := "#ArcomagePvp";

const ARCO_PVP_TURN_SECONDS := 120;      // timed mode, per action
const ARCO_PVP_MAX_TIMEOUTS := 3;        // in a row, then the game is forfeit
const ARCO_PVP_INVITE_SECONDS := 60;
const ARCO_PVP_CHALLENGE_RANGE := 3;     // tiles, to challenge someone with the deck
const ARCO_PVP_RANGE := 8;               // timed mode: tiles from your starting spot
const ARCO_PVP_WATCH_RANGE := 12;        // tiles from either player
const ARCO_PVP_MAX_WATCHERS := 6;

const ARCO_PVP_BTN_ACCEPT := 1;
const ARCO_PVP_BTN_DECLINE := 3;
const ARCO_PVP_BTN_CHALLENGE := 10;
const ARCO_PVP_RADIO_RULESET := 2000;   // + rule set id
const ARCO_PVP_RADIO_TIMED := 3001;
const ARCO_PVP_RADIO_UNTIMED := 3002;

function ArcoPvpDebug( text )

	if( ARCO_PVP_DEBUG )
		Print( "[Arcomage PvP] " + text );
	endif

endfunction

// A board-log line that also goes to the PvP trace.
function ArcoPvpLog( byref st, line )

	ArcoPvpDebug( "  match " + CStr( GetPid() ) + ": " + line );
	ArcoLog( st, line );

endfunction

// Live rule sets this character has beaten at any difficulty: the ones they may pick for PvP.
function ArcoBeatenRuleSets( who )

	var rec := ArcoGetRecord( who );
	var beaten := array;
	var ids := ArcoAllRuleSetIds();
	var diffs := ArcoDifficulties();
	foreach id in ids
		foreach difficulty in diffs
			if( ArcoGridWins( rec, id, difficulty ) )
				beaten.append( id );
				break;
			endif
		endforeach
	endforeach
	return beaten;

endfunction

function ArcoSameAccount( a, b )

	var acct_a := a.acct;
	var acct_b := b.acct;
	if( !acct_a || !acct_b )
		return 0;
	endif
	return acct_a.name == acct_b.name;

endfunction

function ArcoInBackpack( who, item )

	var holder := item.container;
	while( holder )
		if( holder.serial == who.backpack.serial )
			return 1;
		elseif( holder.isa( POLCLASS_MOBILE ) )
			return 0;
		endif
		holder := holder.container;
	endwhile
	return 0;

endfunction

function ArcoPvpModeText( timed )

	if( timed )
		return "Timed: " + CStr( ARCO_PVP_TURN_SECONDS ) + " seconds a turn. Out of time, a random card is discarded; "
			+ CStr( ARCO_PVP_MAX_TIMEOUTS ) + " in a row forfeit. Stay within " + CStr( ARCO_PVP_RANGE ) + " tiles of your spot.";
	endif
	return "Untimed: no clock, but taking a step from your spot forfeits the game.";

endfunction

// The challenger's choices. Returns struct{ ruleset, timed } or 0.
function ArcoPvpSetupGump( who, opponent_name, ruleset_ids )

	var rows := CInt( ( ruleset_ids.size() + 1 ) / 2 );
	var height := 190 + rows * 24;
	var gump := GFCreateGump( 80, 60, 600, height );
	GFResizePic( gump, 0, 0, 9270, 600, height );
	GFTextMid( gump, 0, 18, 600, ARCO_HUE_TITLE, "Challenge " + opponent_name + " to Arcomage" );
	GFTextLine( gump, 25, 46, ARCO_HUE_DIM, "Whose rules? (card players you have beaten)" );

	GFSetRadioGroup( gump, 1 );
	var i;
	for( i := 1; i <= ruleset_ids.size(); i := i + 1 )
		var rules := ArcoGetRuleSet( ruleset_ids[i] );
		var col := ( i - 1 ) % 2;
		var row := CInt( ( i - 1 ) / 2 );
		var x := 25 + col * 285;
		var y := 72 + row * 24;
		var checked := 0;
		if( i == 1 )
			checked := 1;
		endif
		GFRadioButton( gump, x, y, 208, 209, checked, ARCO_PVP_RADIO_RULESET + rules.id );
		GFTextLine( gump, x + 28, y, ARCO_HUE_TEXT, rules.npcname + " (" + CStr( rules.wintower ) + " / " + CStr( rules.winresource ) + ")" );
	endfor

	var y2 := 80 + rows * 24;
	GFSetRadioGroup( gump, 2 );
	GFRadioButton( gump, 25, y2, 208, 209, 1, ARCO_PVP_RADIO_TIMED );
	GFTextLine( gump, 53, y2, ARCO_HUE_TEXT, "Timed (" + CStr( ARCO_PVP_TURN_SECONDS ) + " seconds a turn)" );
	GFRadioButton( gump, 310, y2, 208, 209, 0, ARCO_PVP_RADIO_UNTIMED );
	GFTextLine( gump, 338, y2, ARCO_HUE_TEXT, "Untimed (don't move)" );

	GFHTMLArea( gump, 25, y2 + 30, 550, 40, "<BASEFONT COLOR=#B0B0B0>Walking away, logging out or resigning loses the game. Goal shown as tower / resource.</BASEFONT>" );
	GFAddButton( gump, 25, height - 45, 4005, 4007, GF_CLOSE_BTN, ARCO_PVP_BTN_CHALLENGE );
	GFTextLine( gump, 60, height - 44, ARCO_HUE_TEXT, "Challenge" );
	GFAddButton( gump, 440, height - 45, 4017, 4019, GF_CLOSE_BTN, ARCO_PVP_BTN_DECLINE );
	GFTextLine( gump, 475, height - 44, ARCO_HUE_TEXT, "Cancel" );

	var data := GFSendGump( who, gump );
	if( !data || data[0] != ARCO_PVP_BTN_CHALLENGE )
		return 0;
	endif
	var choice := struct{ "ruleset" := CInt( ruleset_ids[1] ), "timed" := 1 };
	foreach id in ruleset_ids
		if( data[ARCO_PVP_RADIO_RULESET + id] )
			choice.ruleset := id;
		endif
	endforeach
	if( data[ARCO_PVP_RADIO_UNTIMED] )
		choice.timed := 0;
	endif
	return choice;

endfunction

// The invitation, shown by the challenged player's seat. Returns 1 to accept.
function ArcoPvpInviteGump( who, challenger_name, rules, timed )

	var gump := GFCreateGump( 200, 150, 440, 250 );
	GFResizePic( gump, 0, 0, 9270, 440, 250 );
	GFTextMid( gump, 0, 18, 440, ARCO_HUE_TITLE, challenger_name + " challenges you to Arcomage" );
	var info := "<BASEFONT COLOR=#FFFFFF>Rules: " + rules.npcname + "'s. First to a tower of " + CStr( rules.wintower )
		+ " or " + CStr( rules.winresource ) + " of any resource wins. Hand: " + CStr( rules.handsize ) + " cards.<BR><BR>"
		+ ArcoPvpModeText( timed ) + "<BR><BR>A coin flip decides who goes first. Answer within "
		+ CStr( ARCO_PVP_INVITE_SECONDS ) + " seconds.</BASEFONT>";
	GFHTMLArea( gump, 25, 45, 390, 145, info );
	GFAddButton( gump, 25, 205, 4005, 4007, GF_CLOSE_BTN, ARCO_PVP_BTN_ACCEPT );
	GFTextLine( gump, 60, 206, ARCO_HUE_TEXT, "Accept" );
	GFAddButton( gump, 300, 205, 4017, 4019, GF_CLOSE_BTN, ARCO_PVP_BTN_DECLINE );
	GFTextLine( gump, 335, 206, ARCO_HUE_TEXT, "Decline" );

	var data := GFSendGump( who, gump );
	return ( data && data[0] == ARCO_PVP_BTN_ACCEPT );

endfunction

function ArcoPvpResultGump( who, headline, reason )

	var gump := GFCreateGump( 200, 150, 400, 170 );
	GFResizePic( gump, 0, 0, 9270, 400, 170 );
	GFTextMid( gump, 0, 20, 400, ARCO_HUE_TITLE, headline );
	GFHTMLArea( gump, 25, 50, 350, 60, "<CENTER><BASEFONT COLOR=#FFFFFF>" + reason + "</BASEFONT></CENTER>" );
	GFAddButton( gump, 175, 120, 4005, 4007, GF_CLOSE_BTN, ARCO_BTN_OK );
	GFTextLine( gump, 210, 121, ARCO_HUE_TEXT, "OK" );
	GFSendGump( who, gump );

endfunction

// The board one seat sees. st is the controller's "board" event; seat 1 or 2 is a player
// (own side on the left, hand shown, cards live on their turn), seat 0 a watcher (seat 1 on
// the left, no hands, closable).
function ArcoPvpBoardFor( st, seat )

	var names := st.names;
	var sides := st.sides;
	var title := "Arcomage - " + names[1] + " vs " + names[2] + " - " + st.rules.npcname + "'s rules";
	var view, left, right;
	if( seat )
		view := ArcoNewView( st.rules, title, names[seat], names[3 - seat] );
		view.left_keep := "Your keep";
		view.show_hand := 1;
		view.resign := 1;
		left := sides[seat];
		right := sides[3 - seat];
		if( st.active == seat )
			view.can_act := 1;
			view.discard_mode := st.discard_mode;
			view.status := "Your turn.";
			if( st.timed )
				view.status := "Your turn - " + CStr( Max( 0, st.deadline - ReadGameClock() ) ) + " seconds left.";
			endif
		else
			view.status := "Waiting for " + names[st.active] + "...";
		endif
	else
		view := ArcoNewView( st.rules, title + " (watching)", names[1], names[2] );
		view.closable := 1;
		view.status := names[st.active] + "'s turn.";
		left := sides[1];
		right := sides[2];
	endif
	view.log := st.log;
	return ArcoBoardGump( view, left, right );

endfunction
