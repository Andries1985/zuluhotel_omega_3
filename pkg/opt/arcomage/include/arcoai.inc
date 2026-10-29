// Arcomage computer player. 2026-10-23: port of the arcomage-hd remake's AI (MIT code,
// src/ai/main.ts, coefs.ts, getMaxScore.ts). It looks one move ahead: each card is
// tried on a copy of both sides and scored as (own gains - enemy gains), each stat
// weighted by the remake's coefficients. Level 0 always takes the best score;
// levels 1-3 add more and more random noise; level 4 picks any legal move.
// A card that wins outright (without also losing) is always played at once.
use uo;
use os;
use util;

include ":arcomage:arcocards";

const ARCO_COEF_ORE := 1.0;
const ARCO_COEF_REAGENTS := 1.28;
const ARCO_COEF_RECRUITS := 1.05;
const ARCO_COEF_PROD := 18.0;
const ARCO_COEF_TOWER := 3.67;
const ARCO_COEF_WALL := 2.02;
const ARCO_COEF_PLAYAGAIN := 46.0;
const ARCO_COEF_DRAWDISCARD := 27.6;
const ARCO_COEF_UNDISCARDABLE := 2.03;
const ARCO_COEF_USEDISCARD := 2.18;
const ARCO_COEF_TIEBONUS := 999.0;
const ARCO_COEF_LOSEPENALTY := 999.0;
const ARCO_COEF_ATTACK := 1.08;

function ArcoStatCoef( stat )

	case( stat )
		"ore":         return ARCO_COEF_ORE;
		"reagents":    return ARCO_COEF_REAGENTS;
		"recruits":    return ARCO_COEF_RECRUITS;
		"oreprod":     return ARCO_COEF_ORE * ARCO_COEF_PROD;
		"reagentprod": return ARCO_COEF_REAGENTS * ARCO_COEF_PROD;
		"recruitprod": return ARCO_COEF_RECRUITS * ARCO_COEF_PROD;
		"tower":       return ARCO_COEF_TOWER;
		"wall":        return ARCO_COEF_WALL;
	endcase
	return 0.0;

endfunction

// Sum of `dice` uniform rolls, each 0 .. max/dice: a bell curve on 0 .. max centred on max/2.
function ArcoWeightedRandom( max_value, dice )

	var total := 0.0;
	var d;
	for( d := 1; d <= dice; d := d + 1 )
		total := total + RandomFloat( CDbl( max_value ) / dice );
	endfor
	return total;

endfunction

// Picks the AI's action. Returns struct{ index (1-based hand slot), play (1 play / 0 discard) },
// or 0 when there is no legal move at all (the AI resigns).
// discard_mode: after a draw-discard card the player may only discard.
function ArcoAIChoose( me, foe, rules, level, discard_mode := 0 )

	var hand := me["hand"];
	var stats := ArcoStatNames();
	var options := array;
	var i;

	for( i := 1; i <= hand.size(); i := i + 1 )
		var card := ArcoGetCard( hand[i] );
		var canuse := 0;
		if( !discard_mode )
			if( ArcoCanAfford( me, card ) )
				canuse := 1;
			endif
		endif
		var candiscard := 1;
		if( card.undiscardable )
			candiscard := 0;
		endif

		var me_after := ArcoCopyStats( me );
		var foe_after := ArcoCopyStats( foe );
		ArcoPlayCard( me_after, foe_after, card );

		var wins_now := ArcoHasWon( me_after, foe_after, rules );
		var loses_now := ArcoHasWon( foe_after, me_after, rules );
		if( canuse && wins_now && !loses_now )
			ArcoDebug( "AI level " + CStr( level ) + ": " + card.name + " wins outright, playing it" );
			return struct{ "index" := i, "play" := 1 };
		endif

		var score := 0.0;
		foreach stat in stats
			var coef := ArcoStatCoef( stat );
			score := score + ( me_after[stat] - me[stat] ) * coef;
			score := score - ( foe_after[stat] - foe[stat] ) * coef * ARCO_COEF_ATTACK;
		endforeach

		if( card.playagain )
			score := score + ARCO_COEF_PLAYAGAIN;
		endif
		if( card.drawdiscard )
			score := score + ARCO_COEF_DRAWDISCARD;
		endif
		if( card.undiscardable )
			score := score + ARCO_COEF_UNDISCARDABLE;
		endif
		if( wins_now && loses_now )
			score := score + ARCO_COEF_TIEBONUS;
		endif
		if( !wins_now && loses_now )
			score := score - ARCO_COEF_LOSEPENALTY;
		endif

		var line := "  " + card.name + " (" + CStr( card.cost ) + " " + card.resource + ")";
		if( canuse )
			options.append( struct{ "index" := i, "play" := 1, "score" := score, "name" := card.name } );
			line := line + " play " + CStr( CInt( score ) );
		else
			line := line + " can't play";
		endif
		if( candiscard )
			options.append( struct{ "index" := i, "play" := 0, "score" := -( score / ARCO_COEF_USEDISCARD ), "name" := card.name } );
			line := line + ", discard " + CStr( CInt( -( score / ARCO_COEF_USEDISCARD ) ) );
		endif
		ArcoDebug( line );
	endfor

	if( !options.size() )
		ArcoDebug( "AI level " + CStr( level ) + ": no legal move, concedes" );
		return 0;
	endif

	var best := ArcoPickByLevel( options, level );
	if( best.play )
		ArcoDebug( "AI level " + CStr( level ) + " chose: play " + best.name );
	else
		ArcoDebug( "AI level " + CStr( level ) + " chose: discard " + best.name );
	endif
	return struct{ "index" := best.index, "play" := best.play };

endfunction

function ArcoPickByLevel( options, level )

	if( level >= 4 )
		// 2026-10-25: the remake picks from every legal move alike, and every card can be
		// discarded but only some played, so it nearly always discarded. Now it plays a random
		// affordable card 3 times in 4, and otherwise discards a random card.
		var plays := array;
		var discards := array;
		foreach opt in options
			if( opt.play )
				plays.append( opt );
			else
				discards.append( opt );
			endif
		endforeach
		if( plays.size() )
			if( RandomInt( 4 ) || !discards.size() )
				return plays[RandomInt( plays.size() ) + 1];
			endif
		endif
		return discards[RandomInt( discards.size() ) + 1];
	endif

	var adjusted := array;
	if( level == 3 )
		var top := options[1].score;
		var bottom := options[1].score;
		foreach opt in options
			if( opt.score > top )
				top := opt.score;
			endif
			if( opt.score < bottom )
				bottom := opt.score;
			endif
		endforeach
		foreach opt in options
			adjusted.append( opt.score + ( top - bottom ) * ( ArcoWeightedRandom( 100, 2 ) - 50.0 ) / 50.0 );
		endforeach
	else
		foreach opt in options
			case( level )
				1:  adjusted.append( opt.score * ArcoWeightedRandom( 100, 8 ) / 50.0 );
				2:  adjusted.append( opt.score * ArcoWeightedRandom( 100, 2 ) / 50.0 );
				default: adjusted.append( opt.score );
			endcase
		endforeach
	endif

	var best_index := 1;
	var i;
	for( i := 2; i <= adjusted.size(); i := i + 1 )
		if( adjusted[i] > adjusted[best_index] )
			best_index := i;
		endif
	endfor
	return options[best_index];

endfunction
