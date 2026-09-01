// Shared realm -> hue mapping for runes: keeps the Runic Atlas rune-list
// display (pkg/std/runebook/runicatlas.src) and physical rune items (marked
// via pkg/std/spells/mark.src, or dropped out of a book via
// pkg/std/runebook/runebookactions.inc) showing the same color for the same
// realm. Hue values aren't from a verified chart -- tweak here if any of them
// don't render as expected in-game; this is the one place they're defined.

function GetRealmRuneColor( realm )

	var r := Lower( CStr(realm) );

	case( r )
		"britannia":		return 0x51;
		"britannia_alt":	return 0xA;
		"ilshenar":			return 0x501;
		"malas":			return 0x44E;
		"tokuno":			return 52;
		"termur":			return 0x66D;
		default:			return 0x2C;
	endcase

endfunction
