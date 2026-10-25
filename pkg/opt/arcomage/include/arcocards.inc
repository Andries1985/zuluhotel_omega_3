// Arcomage rules: card data, rule sets, player state, card effects and the win check.
// 2026-10-23: new package. Numbers and rules follow the arcomage-hd remake (MIT code,
// github.com/arcomage/arcomage-hd); card names/text are our own (config/cards.cfg).
//
// A "side" is a dictionary: tower wall ore reagents recruits oreprod reagentprod recruitprod
// plus "hand" (array of card ids). Effects change a side through ArcoChangeStat so the
// floors hold everywhere: stats never drop below 0, production never below 1.
use uo;
use os;
use basic;
use basicio;
use cfgfile;
use math;
use util;

const ARCO_CARDS_CFG := ":arcomage:cards";
const ARCO_RULESETS_CFG := ":arcomage:rulesets";

// 2026-10-25: 1 = trace every game to the server console (AI options/choices, turn state).
// Off since 2026-10-25 (game confirmed working); set to 1 to trace games again.
const ARCO_DEBUG := 0;

const ARCO_RESULT_NONE := 0;
const ARCO_RESULT_A := 1;
const ARCO_RESULT_B := 2;
const ARCO_RESULT_TIE := 3;

// Filled on first use by ArcoGetCards() (an include's top-level var must stay a literal).
var arco_cards := 0;
var arco_pool := 0;

function ArcoGetCards()

	if( arco_cards )
		return arco_cards;
	endif

	var cfg := ReadConfigFile( ARCO_CARDS_CFG );
	var cards := dictionary;
	var pool := array;
	var keys := GetConfigIntKeys( cfg );
	foreach id in keys
		var elem := FindConfigElem( cfg, id );
		var card := struct;
		card.+id := id;
		card.+name := GetConfigString( elem, "Name" );
		card.+text := GetConfigString( elem, "Text" );
		card.+resource := Lower( GetConfigString( elem, "Resource" ) );
		card.+cost := CInt( GetConfigInt( elem, "Cost" ) );
		card.+special := 0;
		var special := GetConfigString( elem, "Special" );
		if( special )
			card.special := Lower( special );
		endif
		card.+playagain := CInt( GetConfigInt( elem, "PlayAgain" ) );
		card.+drawdiscard := CInt( GetConfigInt( elem, "DrawDiscard" ) );
		card.+undiscardable := CInt( GetConfigInt( elem, "Undiscardable" ) );
		card.+effects := array;
		var lines := GetConfigStringArray( elem, "Effect" );
		foreach line in lines
			var words := SplitWords( line );
			card.effects.append( struct{ "side" := Lower( words[1] ), "stat" := Lower( words[2] ), "amount" := CInt( words[3] ) } );
		endforeach
		cards[id] := card;

		var weight := CInt( GetConfigInt( elem, "Weight" ) );
		if( weight < 1 )
			weight := 1;
		endif
		var w;
		for( w := 1; w <= weight; w := w + 1 )
			pool.append( id );
		endfor
	endforeach

	arco_cards := cards;
	arco_pool := pool;
	return arco_cards;

endfunction

// 2026-10-25: moved here from match.src so tablewatch.src shares the same test.
const ARCO_TABLE_RANGE := 8;

// The player is still logged in, alive, and within range of the NPC.
function ArcoAtTable( who, npc_serial )

	if( !who )
		return 0;
	elseif( who.dead )
		return 0;
	endif
	var npc := SystemFindObjectBySerial( npc_serial );
	if( !npc )
		return 0;
	elseif( who.realm != npc.realm )
		return 0;
	endif
	return Distance( who, npc ) <= ARCO_TABLE_RANGE;

endfunction

function ArcoDebug( text )

	if( ARCO_DEBUG )
		Print( "[Arcomage] " + text );
	endif

endfunction

// One line of a side's numbers, for the debug trace.
function ArcoSideSummary( side )

	return "tower " + CStr( side["tower"] ) + " wall " + CStr( side["wall"] )
		+ " | ore " + CStr( side["ore"] ) + "+" + CStr( side["oreprod"] )
		+ " reagents " + CStr( side["reagents"] ) + "+" + CStr( side["reagentprod"] )
		+ " recruits " + CStr( side["recruits"] ) + "+" + CStr( side["recruitprod"] );

endfunction

function ArcoGetCard( id )

	var cards := ArcoGetCards();
	return cards[CInt( id )];

endfunction

// Weighted draw with replacement (the remake's randomWithProbs).
function ArcoDrawCardId()

	ArcoGetCards();
	return arco_pool[RandomInt( arco_pool.size() ) + 1];

endfunction

function ArcoGetRuleSet( ruleset_id )

	var cfg := ReadConfigFile( ARCO_RULESETS_CFG );
	var elem := FindConfigElem( cfg, CInt( ruleset_id ) );
	if( !elem )
		return error{ "errortext" := "No Arcomage rule set " + CStr( ruleset_id ) };
	endif

	var rules := struct;
	rules.+id := CInt( ruleset_id );
	rules.+npcname := GetConfigString( elem, "NpcName" );
	rules.+town := GetConfigString( elem, "Town" );
	rules.+original := GetConfigString( elem, "Original" );
	rules.+tier := GetConfigString( elem, "Tier" );
	rules.+title := GetConfigString( elem, "Title" );	// 2026-10-25
	if( !rules.title )
		rules.title := "Arcomage Player";
	endif
	rules.+tower := CInt( GetConfigInt( elem, "Tower" ) );
	rules.+wall := CInt( GetConfigInt( elem, "Wall" ) );
	rules.+oreprod := CInt( GetConfigInt( elem, "OreProd" ) );
	rules.+reagentprod := CInt( GetConfigInt( elem, "ReagentProd" ) );
	rules.+recruitprod := CInt( GetConfigInt( elem, "RecruitProd" ) );
	rules.+ore := CInt( GetConfigInt( elem, "Ore" ) );
	rules.+reagents := CInt( GetConfigInt( elem, "Reagents" ) );
	rules.+recruits := CInt( GetConfigInt( elem, "Recruits" ) );
	rules.+wintower := CInt( GetConfigInt( elem, "WinTower" ) );
	rules.+winresource := CInt( GetConfigInt( elem, "WinResource" ) );
	rules.+handsize := CInt( GetConfigInt( elem, "CardsInHand" ) );
	if( rules.handsize < 1 )
		rules.handsize := 5;
	endif
	return rules;

endfunction

function ArcoAllRuleSetIds()

	var cfg := ReadConfigFile( ARCO_RULESETS_CFG );
	var ids := GetConfigIntKeys( cfg );
	ids.sort();
	return ids;

endfunction

function ArcoStatNames()

	return array{ "tower", "wall", "ore", "reagents", "recruits", "oreprod", "reagentprod", "recruitprod" };

endfunction

function ArcoNewSide( rules )

	var side := dictionary;
	side["tower"] := rules.tower;
	side["wall"] := rules.wall;
	side["ore"] := rules.ore;
	side["reagents"] := rules.reagents;
	side["recruits"] := rules.recruits;
	// Production floors at 1 (the remake's minGeneratorIsOne), even in a rule set.
	side["oreprod"] := Max( 1, rules.oreprod );
	side["reagentprod"] := Max( 1, rules.reagentprod );
	side["recruitprod"] := Max( 1, rules.recruitprod );
	side["hand"] := array;
	return side;

endfunction

// A plain copy of the stats (no hand), for the AI's what-if runs.
function ArcoCopyStats( side )

	var copy := dictionary;
	var names := ArcoStatNames();
	foreach stat in names
		copy[stat] := CInt( side[stat] );
	endforeach
	return copy;

endfunction

function ArcoSetStat( byref side, stat, value )

	var lowest := 0;
	if( stat == "oreprod" || stat == "reagentprod" || stat == "recruitprod" )
		lowest := 1;
	endif
	value := CInt( value );
	if( value < lowest )
		value := lowest;
	endif
	side[stat] := value;

endfunction

function ArcoChangeStat( byref side, stat, diff )

	ArcoSetStat( side, stat, CInt( side[stat] ) + CInt( diff ) );

endfunction

// Damage hits the wall first; what the wall can't absorb comes off the tower.
function ArcoDamage( byref side, amount )

	var left := CInt( side["wall"] ) - amount;
	ArcoChangeStat( side, "wall", -amount );
	if( left < 0 )
		ArcoChangeStat( side, "tower", left );
	endif

endfunction

function ArcoProduce( byref side )

	ArcoChangeStat( side, "ore", side["oreprod"] );
	ArcoChangeStat( side, "reagents", side["reagentprod"] );
	ArcoChangeStat( side, "recruits", side["recruitprod"] );

endfunction

function ArcoCanAfford( side, card )

	return CInt( side[card.resource] ) >= card.cost;

endfunction

// Pays for and resolves a card for "me" against "foe". The caller removes it from the hand.
function ArcoPlayCard( byref me, byref foe, card )

	ArcoChangeStat( me, card.resource, -card.cost );
	if( card.special )
		ArcoApplySpecial( card.special, me, foe );
		return 1;
	endif

	var effects := card.effects;
	foreach eff in effects
		if( eff.side == "self" )
			ArcoApplyOp( me, eff );
		else
			ArcoApplyOp( foe, eff );
		endif
	endforeach
	return 1;

endfunction

function ArcoApplyOp( byref side, eff )

	if( eff.stat == "damage" )
		ArcoDamage( side, eff.amount );
	else
		ArcoChangeStat( side, eff.stat, eff.amount );
	endif

endfunction

// The 15 conditional cards (config/cards.cfg "Special"), written as the remake's effects.
function ArcoApplySpecial( special, byref me, byref foe )

	var my_wall, top, took_reagents, took_ore;
	case( special )
		"richvein":
			if( me["oreprod"] < foe["oreprod"] )
				ArcoChangeStat( me, "oreprod", 2 );
			else
				ArcoChangeStat( me, "oreprod", 1 );
			endif
		"borrowedplans":
			if( me["oreprod"] < foe["oreprod"] )
				ArcoSetStat( me, "oreprod", foe["oreprod"] );
			endif
		"deepfound":
			if( me["wall"] == 0 )
				ArcoChangeStat( me, "wall", 6 );
			else
				ArcoChangeStat( me, "wall", 3 );
			endif
		"springflood":
			if( foe["wall"] < me["wall"] )
				ArcoChangeStat( foe, "recruitprod", -1 );
				ArcoChangeStat( foe, "tower", -2 );
			elseif( foe["wall"] == me["wall"] )
				ArcoChangeStat( foe, "recruitprod", -1 );
				ArcoChangeStat( foe, "tower", -2 );
				ArcoChangeStat( me, "recruitprod", -1 );
				ArcoChangeStat( me, "tower", -2 );
			else
				ArcoChangeStat( me, "recruitprod", -1 );
				ArcoChangeStat( me, "tower", -2 );
			endif
		"garrison":
			ArcoChangeStat( me, "recruits", 6 );
			ArcoChangeStat( me, "wall", 6 );
			if( me["recruitprod"] < foe["recruitprod"] )
				ArcoChangeStat( me, "recruitprod", 1 );
			endif
		"turncoat":
			my_wall := me["wall"];
			ArcoSetStat( me, "wall", foe["wall"] );
			ArcoSetStat( foe, "wall", my_wall );
		"equalmeasure":
			top := Max( me["reagentprod"], foe["reagentprod"] );
			ArcoSetStat( me, "reagentprod", top );
			ArcoSetStat( foe, "reagentprod", top );
		"bagofreagents":
			if( me["tower"] < foe["tower"] )
				ArcoChangeStat( me, "tower", 2 );
			else
				ArcoChangeStat( me, "tower", 1 );
			endif
		"chainlightning":
			if( me["tower"] > foe["wall"] )
				ArcoChangeStat( foe, "tower", -8 );
			else
				ArcoDamage( foe, 8 );
			endif
		"harpy":
			if( foe["wall"] == 0 )
				ArcoDamage( foe, 10 );
			else
				ArcoDamage( foe, 6 );
			endif
		"poisonfield":
			if( foe["wall"] > 0 )
				ArcoDamage( foe, 10 );
			else
				ArcoDamage( foe, 7 );
			endif
		"unicorn":
			if( me["reagentprod"] > foe["reagentprod"] )
				ArcoDamage( foe, 12 );
			else
				ArcoDamage( foe, 8 );
			endif
		"longbowmen":
			if( me["wall"] > foe["wall"] )
				ArcoChangeStat( foe, "tower", -6 );
			else
				ArcoDamage( foe, 6 );
			endif
		"thievesguild":
			// The thief gains half (rounded up) of what the enemy actually loses.
			took_reagents := Min( CInt( foe["reagents"] ), 10 );
			took_ore := Min( CInt( foe["ore"] ), 5 );
			ArcoChangeStat( me, "reagents", CInt( ( took_reagents + 1 ) / 2 ) );
			ArcoChangeStat( me, "ore", CInt( ( took_ore + 1 ) / 2 ) );
			ArcoChangeStat( foe, "reagents", -10 );
			ArcoChangeStat( foe, "ore", -5 );
		"spearman":
			if( me["wall"] > foe["wall"] )
				ArcoDamage( foe, 3 );
			else
				ArcoDamage( foe, 2 );
			endif
		default:
			Print( "[Arcomage] Unknown card special '" + CStr( special ) + "'" );
	endcase

endfunction

function ArcoHasWon( side, other, rules )

	if( other["tower"] <= 0 )
		return 1;
	elseif( side["tower"] >= rules.wintower )
		return 1;
	elseif( side["ore"] >= rules.winresource || side["reagents"] >= rules.winresource || side["recruits"] >= rules.winresource )
		return 1;
	endif
	return 0;

endfunction

// ARCO_RESULT_A / _B / _TIE, or ARCO_RESULT_NONE while the game goes on.
function ArcoCheckVictory( side_a, side_b, rules )

	var a_won := ArcoHasWon( side_a, side_b, rules );
	var b_won := ArcoHasWon( side_b, side_a, rules );
	if( a_won && b_won )
		return ARCO_RESULT_TIE;
	elseif( a_won )
		return ARCO_RESULT_A;
	elseif( b_won )
		return ARCO_RESULT_B;
	endif
	return ARCO_RESULT_NONE;

endfunction

// Difficulty (30/60/90/120/150, shown like a skill) -> the remake's AI level (4 .. 0).
function ArcoDifficulties()

	return array{ 30, 60, 90, 120, 150 };

endfunction

function ArcoDifficultyToLevel( difficulty )

	case( CInt( difficulty ) )
		150: return 0;
		120: return 1;
		90:  return 2;
		60:  return 3;
	endcase
	return 4;

endfunction

function ArcoStatLabel( stat )

	case( stat )
		"tower":       return "Tower";
		"wall":        return "Wall";
		"oreprod":     return "Mine";
		"reagentprod": return "Grove";
		"recruitprod": return "Barracks";
	endcase
	return ArcoResourceLabel( stat );

endfunction

// "Tower +4, Recruits -3" for what changed between two copies of a side ("" if nothing).
// The card's cost is added back first, so paying for a card isn't listed as an effect.
function ArcoChangeText( before, after, cost_resource := "", cost := 0 )

	var parts := array;
	var names := ArcoStatNames();
	foreach stat in names
		var diff := CInt( after[stat] ) - CInt( before[stat] );
		if( stat == cost_resource )
			diff := diff + cost;
		endif
		if( diff > 0 )
			parts.append( ArcoStatLabel( stat ) + " +" + CStr( diff ) );
		elseif( diff < 0 )
			parts.append( ArcoStatLabel( stat ) + " " + CStr( diff ) );
		endif
	endforeach

	var joined := "";
	foreach part in parts
		if( joined != "" )
			joined := joined + ", ";
		endif
		joined := joined + part;
	endforeach
	return joined;

endfunction

// 2026-10-25: the board log names whose numbers moved ("Gorn's Tower +4; your Tower -2")
// instead of quoting the card text, whose "You"/"Enemy" read backwards when the NPC played it.
function ArcoEffectText( card, actor_owner, other_owner, actor_before, actor_after, other_before, other_after )

	var mine := ArcoChangeText( actor_before, actor_after, card.resource, card.cost );
	var theirs := ArcoChangeText( other_before, other_after );
	if( mine == "" && theirs == "" )
		return "no effect.";
	elseif( theirs == "" )
		return actor_owner + " " + mine + ".";
	elseif( mine == "" )
		return other_owner + " " + theirs + ".";
	endif
	return actor_owner + " " + mine + "; " + other_owner + " " + theirs + ".";

endfunction

function ArcoResourceLabel( resource )

	case( resource )
		"ore":      return "Ore";
		"reagents": return "Reagents";
		"recruits": return "Recruits";
	endcase
	return CStr( resource );

endfunction
