// Arcomage gumps: setup (pick a difficulty), the board, resign confirm, result, and the
// .arcomage stats/leaderboard gump. 2026-10-23: new package.
use uo;
use os;
use basic;

include ":mdgumps:gumps_ex";
include ":arcomage:arcocards";
include ":arcomage:arcorecords";

const ARCO_BTN_OK := 1;
const ARCO_BTN_RESIGN := 2;
const ARCO_BTN_CANCEL := 3;
const ARCO_BTN_START := 10;
const ARCO_BTN_PLAY := 100;      // + hand slot
const ARCO_BTN_DISCARD := 200;   // + hand slot
const ARCO_RADIO_DIFF := 1000;   // + difficulty

const ARCO_HUE_TITLE := 53;
const ARCO_HUE_TEXT := 1152;
const ARCO_HUE_DIM := 999;
const ARCO_HUE_GOOD := 68;
const ARCO_HUE_BAD := 38;

// 2026-10-25: cards made taller (text was hidden behind the buttons) and room for 3 log lines.
// The keeps stand on the info panels' bottom edge and a bar at its goal is exactly as tall
// as the panels, whatever the goal; numbers and names sit under the bars.
const ARCO_BOARD_W := 900;
const ARCO_BOARD_H := 680;
const ARCO_PANEL_Y := 60;
const ARCO_PANEL_H := 205;
const ARCO_TOWER_BASE := 265;     // ARCO_PANEL_Y + ARCO_PANEL_H
const ARCO_TOWER_MAX_H := 205;    // ARCO_PANEL_H
const ARCO_BAR_NUMBER_Y := 269;
const ARCO_KEEP_NAME_Y := 289;
const ARCO_LOG_Y := 315;
const ARCO_LOG_LINES := 3;
const ARCO_CARD_W := 104;
const ARCO_CARD_H := 284;
const ARCO_CARD_GAP := 6;
const ARCO_CARD_Y := 380;

function ArcoResourceHtmlColor( resource, enabled := 1 )

	if( !enabled )
		return "#808080";
	endif
	case( resource )
		"ore":      return "#A8432F";
		"reagents": return "#2F5FA8";
		"recruits": return "#3C7D3F";
	endcase
	return "#333333";

endfunction

function ArcoResourceHue( resource )

	case( resource )
		"ore":      return 38;
		"reagents": return 89;
		"recruits": return 68;
	endcase
	return ARCO_HUE_TEXT;

endfunction

function ArcoResourceIcon( resource )

	case( resource )
		"ore":      return 0x19B9;   // pile of ore
		"reagents": return 0x0F7B;   // blood moss
		"recruits": return 0x13B9;   // sword
	endcase
	return 0x0EED;

endfunction

function ArcoCommas( number )

	var text := CStr( CInt( number ) );
	var joined := "";
	var count := 0;
	var i;
	for( i := len( text ); i >= 1; i := i - 1 )
		if( count == 3 )
			joined := "," + joined;
			count := 0;
		endif
		joined := text[i] + joined;
		count := count + 1;
	endfor
	return joined;

endfunction

// ---------------------------------------------------------------- setup

// Shows the challenge gump. Returns the chosen difficulty, or 0 if the player declines.
function ArcoSetupGump( who, npc_name, rules, rec, default_difficulty := 90 )

	var gump := GFCreateGump( 80, 80, 470, 430 );
	GFResizePic( gump, 0, 0, 9270, 470, 430 );
	GFTextMid( gump, 0, 18, 470, ARCO_HUE_TITLE, npc_name + " plays Arcomage" );

	GFPage( gump, 1 );
	var info := "<BASEFONT COLOR=#FFFFFF>First to a tower of " + CStr( rules.wintower ) + ", or " + CStr( rules.winresource )
		+ " of any one resource, wins - or knock the enemy tower down to nothing.<BR>"
		+ "Both start with tower " + CStr( rules.tower ) + " and wall " + CStr( rules.wall )
		+ ", producing " + CStr( rules.oreprod ) + " ore, " + CStr( rules.reagentprod ) + " reagents and "
		+ CStr( rules.recruitprod ) + " recruits a turn. Hand: " + CStr( rules.handsize ) + " cards.</BASEFONT>";
	GFHTMLArea( gump, 25, 45, 420, 90, info );

	GFTextLine( gump, 25, 140, ARCO_HUE_TITLE, "Pick a difficulty" );
	GFTextLine( gump, 245, 140, ARCO_HUE_DIM, "Daily gold" );
	GFTextLine( gump, 370, 140, ARCO_HUE_DIM, "Your wins" );

	GFSetRadioGroup( gump, 1 );
	var y := 165;
	var diffs := ArcoDifficulties();
	foreach difficulty in diffs
		var checked := 0;
		if( difficulty == default_difficulty )
			checked := 1;
		endif
		GFRadioButton( gump, 25, y, 208, 209, checked, ARCO_RADIO_DIFF + difficulty );
		GFTextLine( gump, 55, y, ARCO_HUE_TEXT, "Difficulty " + CStr( difficulty ) );
		if( ArcoDailyGoldClaimed( who, difficulty ) )
			GFTextLine( gump, 245, y, ARCO_HUE_DIM, "claimed today" );
		else
			GFTextLine( gump, 245, y, ARCO_HUE_GOOD, ArcoCommas( ArcoDailyGoldAmount( difficulty ) ) + " gold" );
		endif
		var wins := ArcoGridWins( rec, rules.id, difficulty );
		if( wins )
			GFTextLine( gump, 370, y, ARCO_HUE_GOOD, CStr( wins ) );
		else
			GFTextLine( gump, 370, y, ARCO_HUE_DIM, "-" );
		endif
		y := y + 30;
	endforeach

	GFHTMLArea( gump, 25, 320, 420, 40, "<BASEFONT COLOR=#B0B0B0>Daily gold is paid for your first win at each difficulty per day, against anyone. 150 plays its best move every time; 30 plays at random.</BASEFONT>" );

	GFAddButton( gump, 25, 380, 4005, 4007, GF_CLOSE_BTN, ARCO_BTN_START );
	GFTextLine( gump, 60, 381, ARCO_HUE_TEXT, "Play" );
	GFAddButton( gump, 165, 380, 4011, 4013, GF_PAGE_BTN, 2 );
	GFTextLine( gump, 200, 381, ARCO_HUE_TEXT, "How to play" );
	GFAddButton( gump, 340, 380, 4017, 4019, GF_CLOSE_BTN, ARCO_BTN_CANCEL );
	GFTextLine( gump, 375, 381, ARCO_HUE_TEXT, "No thanks" );

	GFPage( gump, 2 );
	var rules_text := "<BASEFONT COLOR=#FFFFFF>"
		+ "<B>Your keep.</B> You have a tower and a wall. Damage hits the wall first; whatever the wall can't stop comes off the tower.<BR><BR>"
		+ "<B>Resources.</B> Your Mine makes Ore, your Grove makes Reagents and your Barracks make Recruits. "
		+ "At the start of each of your turns every producer adds its level to your stock.<BR><BR>"
		+ "<B>Your turn.</B> You draw a card, then play one card you can afford or discard one. "
		+ "Some cards let you play again. A card's cost is shown in its resource's colour.<BR><BR>"
		+ "<B>Winning.</B> Build your tower to the goal, stockpile the goal in any one resource, "
		+ "or bring the enemy tower down to 0. If both sides reach a goal on the same card, it's a draw.<BR><BR>"
		+ "<B>Leaving.</B> Walking away from the table or logging out forfeits the game."
		+ "</BASEFONT>";
	GFHTMLArea( gump, 25, 45, 420, 320, rules_text, 0, 1 );
	GFAddButton( gump, 25, 380, 4014, 4016, GF_PAGE_BTN, 1 );
	GFTextLine( gump, 60, 381, ARCO_HUE_TEXT, "Back" );

	var data := GFSendGump( who, gump );
	if( !data || data[0] != ARCO_BTN_START )
		return 0;
	endif

	foreach difficulty in diffs
		if( data[ARCO_RADIO_DIFF + difficulty] )
			return difficulty;
		endif
	endforeach
	return default_difficulty;

endfunction

// ---------------------------------------------------------------- board

function ArcoBarHeight( value, goal )

	value := CInt( value );
	if( value <= 0 )
		return 0;
	endif
	if( goal < 1 )
		goal := 1;
	endif
	var h := CInt( value * ARCO_TOWER_MAX_H / goal );
	if( h > ARCO_TOWER_MAX_H )
		h := ARCO_TOWER_MAX_H;
	elseif( h < 10 )
		h := 10;
	endif
	return h;

endfunction

function ArcoDrawBar( byref gump, x, width, value, goal, gump_id )

	var h := ArcoBarHeight( value, goal );
	if( h > 0 )
		GFResizePic( gump, x, ARCO_TOWER_BASE - h, gump_id, width, h );
	endif
	GFTextMid( gump, x - 10, ARCO_BAR_NUMBER_Y, width + 20, ARCO_HUE_TEXT, CStr( CInt( value ) ) );

endfunction

function ArcoDrawSidePanel( byref gump, x, y, title, side )

	GFResizePic( gump, x, y, 9200, 190, ARCO_PANEL_H );
	GFTextMid( gump, x, y + 10, 190, ARCO_HUE_TITLE, title );

	var rows := array{
		array{ "Tower", side["tower"], ARCO_HUE_TEXT },
		array{ "Wall", side["wall"], ARCO_HUE_TEXT },
		array{ "Mine", side["oreprod"], ArcoResourceHue( "ore" ) },
		array{ "Ore", side["ore"], ArcoResourceHue( "ore" ) },
		array{ "Grove", side["reagentprod"], ArcoResourceHue( "reagents" ) },
		array{ "Reagents", side["reagents"], ArcoResourceHue( "reagents" ) },
		array{ "Barracks", side["recruitprod"], ArcoResourceHue( "recruits" ) },
		array{ "Recruits", side["recruits"], ArcoResourceHue( "recruits" ) }
	};
	var row_y := y + 36;
	var i;
	for( i := 1; i <= rows.size(); i := i + 1 )
		GFTextLine( gump, x + 20, row_y, rows[i][3], rows[i][1] );
		GFTextRight( gump, x + 170, row_y, rows[i][3], CStr( CInt( rows[i][2] ) ) );
		row_y := row_y + 20;
		if( i == 2 )
			row_y := row_y + 4;
		endif
	endfor

endfunction

// affordable colours the name and cost; can_play / can_discard add the buttons.
function ArcoDrawCard( byref gump, x, slot, card, affordable, can_play, can_discard )

	var y := ARCO_CARD_Y;
	GFResizePic( gump, x, y, 9350, ARCO_CARD_W, ARCO_CARD_H );

	var color := ArcoResourceHtmlColor( card.resource, affordable );
	GFHTMLArea( gump, x + 4, y + 6, ARCO_CARD_W - 8, 36, "<CENTER><BASEFONT COLOR=" + color + "><B>" + card.name + "</B></BASEFONT></CENTER>" );
	GFTilePic( gump, x + 30, y + 44, ArcoResourceIcon( card.resource ) );
	GFHTMLArea( gump, x + 4, y + 92, ARCO_CARD_W - 8, 18, "<CENTER><BASEFONT COLOR=" + color + ">" + CStr( card.cost ) + " " + ArcoResourceLabel( card.resource ) + "</BASEFONT></CENTER>" );
	GFHTMLArea( gump, x + 4, y + 112, ARCO_CARD_W - 8, 128, "<BASEFONT COLOR=#2A2A2A>" + card.text + "</BASEFONT>" );

	if( can_play )
		GFAddButton( gump, x + 8, y + 244, 2117, 2118, GF_CLOSE_BTN, ARCO_BTN_PLAY + slot );
		GFTextLine( gump, x + 26, y + 241, 0, "Play" );
	endif
	if( can_discard )
		GFAddButton( gump, x + 8, y + 262, 2117, 2118, GF_CLOSE_BTN, ARCO_BTN_DISCARD + slot );
		GFTextLine( gump, x + 26, y + 259, 0, "Discard" );
	endif

endfunction

// 2026-10-25: one board for solo, both PvP seats and watchers. view is a struct (ArcoNewView):
//   title, rules, left_name, right_name, left_keep, right_keep (labels under the keeps),
//   log (array of lines), discard_mode, can_act (cards get Play/Discard), show_hand,
//   resign (Resign button), closable, status (a highlighted line, "" for none).
// left is drawn as the viewer's own side; its hand is shown when show_hand is set.
function ArcoNewView( rules, title, left_name, right_name )

	return struct{ "title" := title, "rules" := rules, "left_name" := left_name, "right_name" := right_name,
		"left_keep" := left_name, "right_keep" := right_name, "log" := array, "discard_mode" := 0,
		"can_act" := 0, "show_hand" := 0, "resign" := 0, "closable" := 0, "status" := "" };

endfunction

function ArcoBoardGump( view, left, right )

	var rules := view.rules;
	var gump := GFCreateGump( 20, 20, ARCO_BOARD_W, ARCO_BOARD_H );
	if( !view.closable )
		GFClosable( gump, 0 );
	endif
	GFResizePic( gump, 0, 0, 9270, ARCO_BOARD_W, ARCO_BOARD_H );

	GFTextMid( gump, 0, 14, ARCO_BOARD_W, ARCO_HUE_TITLE, view.title );
	GFTextMid( gump, 0, 34, ARCO_BOARD_W, ARCO_HUE_DIM, "Goal: tower " + CStr( rules.wintower ) + ", or " + CStr( rules.winresource ) + " of any resource" );
	if( view.resign )
		GFAddButton( gump, ARCO_BOARD_W - 50, 14, 4017, 4019, GF_CLOSE_BTN, ARCO_BTN_RESIGN );
		GFTextRight( gump, ARCO_BOARD_W - 56, 15, ARCO_HUE_DIM, "Resign" );
	endif

	ArcoDrawSidePanel( gump, 15, ARCO_PANEL_Y, view.left_name, left );
	ArcoDrawSidePanel( gump, ARCO_BOARD_W - 205, ARCO_PANEL_Y, view.right_name, right );

	// Keeps: the left side's tower and wall, then the right side's, as a share of the tower goal.
	ArcoDrawBar( gump, 330, 50, left["tower"], rules.wintower, 9350 );
	ArcoDrawBar( gump, 392, 20, left["wall"], rules.wintower, 9200 );
	ArcoDrawBar( gump, 488, 20, right["wall"], rules.wintower, 9200 );
	ArcoDrawBar( gump, 520, 50, right["tower"], rules.wintower, 9350 );
	GFTextMid( gump, 310, ARCO_KEEP_NAME_Y, 120, ARCO_HUE_TEXT, view.left_keep );
	GFTextMid( gump, 470, ARCO_KEEP_NAME_Y, 120, ARCO_HUE_TEXT, view.right_keep );

	// The last few things that happened, newest at the bottom, then the highlighted line.
	var prompt := view.status;
	if( view.can_act && view.discard_mode )
		prompt := "Choose a card to discard.";
	endif
	var log_lines := view.log;
	var first := log_lines.size() - ARCO_LOG_LINES + 1;
	if( prompt != "" )
		first := first + 1;
	endif
	if( first < 1 )
		first := 1;
	endif
	var line_y := ARCO_LOG_Y;
	var i;
	for( i := first; i <= log_lines.size(); i := i + 1 )
		GFTextMid( gump, 0, line_y, ARCO_BOARD_W, ARCO_HUE_TEXT, log_lines[i] );
		line_y := line_y + 14;
	endfor
	if( prompt != "" )
		GFTextMid( gump, 0, line_y, ARCO_BOARD_W, ARCO_HUE_TITLE, prompt );
	endif

	if( !view.show_hand )
		return gump;
	endif
	var hand := left["hand"];
	var count := hand.size();
	var x := CInt( ( ARCO_BOARD_W - count * ( ARCO_CARD_W + ARCO_CARD_GAP ) + ARCO_CARD_GAP ) / 2 );
	for( i := 1; i <= count; i := i + 1 )
		var card := ArcoGetCard( hand[i] );
		var affordable := ArcoCanAfford( left, card );
		var can_play := 0;
		var can_discard := 0;
		if( view.can_act )
			if( !view.discard_mode )
				if( affordable )
					can_play := 1;
				endif
			endif
			if( !card.undiscardable )
				can_discard := 1;
			endif
		endif
		ArcoDrawCard( gump, x, i, card, affordable, can_play, can_discard );
		x := x + ARCO_CARD_W + ARCO_CARD_GAP;
	endfor

	return gump;

endfunction

function ArcoConfirmResign( who )

	var gump := GFCreateGump( 300, 250, 300, 120 );
	GFResizePic( gump, 0, 0, 9270, 300, 120 );
	GFTextMid( gump, 0, 20, 300, ARCO_HUE_TITLE, "Resign this game?" );
	GFTextMid( gump, 0, 42, 300, ARCO_HUE_DIM, "It counts as a loss." );
	GFAddButton( gump, 40, 75, 4005, 4007, GF_CLOSE_BTN, ARCO_BTN_OK );
	GFTextLine( gump, 75, 76, ARCO_HUE_TEXT, "Resign" );
	GFAddButton( gump, 170, 75, 4017, 4019, GF_CLOSE_BTN, ARCO_BTN_CANCEL );
	GFTextLine( gump, 205, 76, ARCO_HUE_TEXT, "Keep playing" );

	var data := GFSendGump( who, gump );
	return ( data && data[0] == ARCO_BTN_OK );

endfunction

function ArcoResultGump( who, headline, reason, gold, rec )

	var gump := GFCreateGump( 200, 150, 400, 200 );
	GFResizePic( gump, 0, 0, 9270, 400, 200 );
	GFTextMid( gump, 0, 20, 400, ARCO_HUE_TITLE, headline );
	GFTextMid( gump, 0, 50, 400, ARCO_HUE_TEXT, reason );
	if( gold )
		GFTextMid( gump, 0, 76, 400, ARCO_HUE_GOOD, "You win " + ArcoCommas( gold ) + " gold." );
	endif
	GFTextMid( gump, 0, 106, 400, ARCO_HUE_DIM, "Your record: " + CStr( rec.wins ) + " wins, " + CStr( rec.losses ) + " losses, " + CStr( rec.ties ) + " draws" );
	GFAddButton( gump, 175, 145, 4005, 4007, GF_CLOSE_BTN, ARCO_BTN_OK );
	GFTextLine( gump, 210, 146, ARCO_HUE_TEXT, "OK" );
	GFSendGump( who, gump );

endfunction

// ---------------------------------------------------------------- stats (.arcomage)

// Page 1-2: your wins per NPC per difficulty; page 3: the shard leaderboard.
function ArcoStatsGump( who )

	var rec := ArcoGetRecord( who );
	var board := ArcoLeaderboard( 20 );
	var ids := ArcoAllRuleSetIds();
	var diffs := ArcoDifficulties();
	var per_page := 15;
	var pages := CInt( ( ids.size() + per_page - 1 ) / per_page );

	var gump := GFCreateGump( 60, 60, 560, 470 );
	GFResizePic( gump, 0, 0, 9270, 560, 470 );
	GFTextMid( gump, 0, 14, 560, ARCO_HUE_TITLE, "Arcomage" );
	GFTextMid( gump, 0, 34, 560, ARCO_HUE_TEXT, who.name + ": " + CStr( rec.wins ) + " wins, " + CStr( rec.losses ) + " losses, "
		+ CStr( rec.ties ) + " draws - " + CStr( rec.beaten ) + " of " + CStr( ids.size() ) + " players beaten" );

	var page, row, i, y;
	for( page := 1; page <= pages; page := page + 1 )
		GFPage( gump, page );
		GFTextLine( gump, 20, 62, ARCO_HUE_DIM, "Opponent" );
		var col_x := 300;
		foreach difficulty in diffs
			GFTextLine( gump, col_x, 62, ARCO_HUE_DIM, CStr( difficulty ) );
			col_x := col_x + 50;
		endforeach

		y := 84;
		for( row := 1; row <= per_page; row := row + 1 )
			i := ( page - 1 ) * per_page + row;
			if( i > ids.size() )
				break;
			endif
			var rules := ArcoGetRuleSet( ids[i] );
			GFTextLine( gump, 20, y, ARCO_HUE_TEXT, rules.npcname + " (" + rules.town + ")" );
			col_x := 300;
			foreach difficulty in diffs
				var wins := ArcoGridWins( rec, ids[i], difficulty );
				if( wins )
					GFTextLine( gump, col_x, y, ARCO_HUE_GOOD, CStr( wins ) );
				else
					GFTextLine( gump, col_x, y, ARCO_HUE_DIM, "-" );
				endif
				col_x := col_x + 50;
			endforeach
			y := y + 22;
		endfor

		if( page > 1 )
			GFAddButton( gump, 20, 425, 4014, 4016, GF_PAGE_BTN, page - 1 );
			GFTextLine( gump, 55, 426, ARCO_HUE_TEXT, "Back" );
		endif
		if( page < pages )
			GFAddButton( gump, 200, 425, 4005, 4007, GF_PAGE_BTN, page + 1 );
			GFTextLine( gump, 235, 426, ARCO_HUE_TEXT, "More" );
		endif
		GFAddButton( gump, 400, 425, 4011, 4013, GF_PAGE_BTN, pages + 1 );
		GFTextLine( gump, 435, 426, ARCO_HUE_TEXT, "Leaderboard" );
	endfor

	GFPage( gump, pages + 1 );
	GFTextLine( gump, 20, 62, ARCO_HUE_DIM, "#" );
	GFTextLine( gump, 50, 62, ARCO_HUE_DIM, "Player" );
	GFTextLine( gump, 290, 62, ARCO_HUE_DIM, "Wins" );
	GFTextLine( gump, 360, 62, ARCO_HUE_DIM, "Losses" );
	GFTextLine( gump, 440, 62, ARCO_HUE_DIM, "Beaten" );
	y := 84;
	if( !board.size() )
		GFTextLine( gump, 50, y, ARCO_HUE_DIM, "Nobody has won a game yet." );
	endif
	for( i := 1; i <= board.size(); i := i + 1 )
		var hue := ARCO_HUE_TEXT;
		if( board[i].name == who.name )
			hue := ARCO_HUE_TITLE;
		endif
		GFTextLine( gump, 20, y, hue, CStr( i ) );
		GFTextLine( gump, 50, y, hue, CStr( board[i].name ) );
		GFTextLine( gump, 290, y, hue, CStr( board[i].wins ) );
		GFTextLine( gump, 360, y, hue, CStr( board[i].losses ) );
		GFTextLine( gump, 440, y, hue, CStr( board[i].beaten ) );
		y := y + 16;
	endfor
	GFAddButton( gump, 20, 425, 4014, 4016, GF_PAGE_BTN, 1 );
	GFTextLine( gump, 55, 426, ARCO_HUE_TEXT, "My record" );

	GFSendGump( who, gump );

endfunction
