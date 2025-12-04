// GAMEHELP.INC
//
// Creator: Myrathi <tjh@myrathi.co.uk>
// Created: 25-Apr-2001
use uo;
use basic;

var ascZ := CAsc("Z");

function GetPieceGump( piece_code, dictPieces )
	if ( !dictPieces.exists( piece_code ) )
		return -1;
	endif
	return dictPieces[ piece_code ];
endfunction

function SetupChess( byref zBoard )
	var zSetup := "RNBQKBNRPPPPPPPP................................pppppppprnbqkbnr";
	if ( typeof(zBoard)["String"] )
		zBoard := zSetup;
	endif
	return zSetup;
endfunction

// TOP   : White
// BOTTOM: Black
//
// Return: Move Ok? 1=y, 0=n
//
// TODO: "En Passant" (pawn-move 'in passing'), "Castling" (Rook/King swap)
//
function MakeChessMove( sqFrom, sqTo, byref zBoard )

	// Are both locations on the board?
	//
	if ( sqFrom < 1 || sqFrom > 64 || sqTo < 1 || sqTo > 64 )
		//return 0;
	endif

	// Is the board valid? (64 locs)
	//
	if ( zBoard.size() != 64 )
		//return 0;
	endif

	// Get the piece
	//
	var piece := zBoard[sqFrom];
	var dest := zBoard[sqTo];

	// Convert locations to {x,y} arrays (topleft origin)
	//
	var xyFrom := {((sqFrom-1)%8)+1,CInt((sqFrom-1)/8)+1};
	var xyTo := {((sqTo-1)%8)+1,CInt((sqTo-1)/8)+1};

	// Set piece-direction by piece-colour
	// (+1 = down the board, -1 = up the board)
	//
	var dir := 1, color := "W", destcol := "W";
	if ( CAsc(piece) > ascZ ) // lower case = black
		dir := -1;
		color := "B";
		piece := upper(piece);
	endif
	if ( CAsc(dest) > ascZ )
		destcol := "B";
		dest := upper(dest);
	endif

	// Validate move (by piece type and direction)

	// Is the destination piece the same color? Fail.
	if ( color == destcol )
		return 0;
	endif

	// Ensure path validity and non-blockage.
	case( piece )
		"P": // Pawn
			break;
		"K": // King
			break;
		"B": // Bishop
		"R": // Rook (castle)
		"Q": // Queen
			break;
		"N": // kNight (horse)
			break;
		default:
			// Board has an invalid piece on it or they're trying to move nothing? :)
			if ( piece != "." ) // invalid
				return -1;
			endif
			break;
	endcase

	// A-Ok, if we got here! Move piece and return OK.
	//
	zBoard[sqTo] := zBoard[sqFrom]; // need this to get original piece code
	zBoard[sqFrom] := ".";
	return 1;

endfunction
