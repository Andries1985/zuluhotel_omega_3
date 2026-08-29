// Bulk Order Deeds -- shared gump-drawing helpers used by the deed, book,
// and filter-preference gumps. The consumer must already `include
// ":mdgumps:gumps";` -- this file doesn't, to avoid forcing every file that
// pulls in general BOD helpers (bulkorder_generation.inc,
// bulkorder_matching.inc, the gm commands, etc.) to carry a gump dependency
// they don't need.

const BOD_FRAME_FX := 25;
const BOD_FRAME_FY := 10;
const BOD_FRAME_FW := 430;

// The real OSI parchment-deed recipe, captured byte-for-byte via
// pkg/opt/alryc/textcmd/test/osibulkgump.src: outer 5054 border, inner 2624
// tiled paper with a CheckerTrans region over it (the see-through "torn
// parchment" look), 10460 ornaments at each corner (offset -5 from the near
// edges, -25 from the far edges).
function BOD_DrawParchmentFrame(byref gump, fx, fy, fw, fh)
	GFResizePic(gump, fx, fy, 5054, fw, fh);
	GFPicTiled(gump, fx+8, fy+10, fw-17, fh-19, 2624);
	GFAddAlphaRegion(gump, fx+8, fy+10, fw-17, fh-19);
	GFGumpPic(gump, fx-5, fy-5, 10460);
	GFGumpPic(gump, fx+fw-25, fy-5, 10460);
	GFGumpPic(gump, fx-5, fy+fh-25, 10460);
	GFGumpPic(gump, fx+fw-25, fy+fh-25, 10460);
endfunction

// Linear membership check against a plain array (filter selections and
// GFSendGump's checked-checkbox `.keys` are both small plain arrays here,
// never more than a few dozen entries).
function BOD_ArrayContains(arr, val)
	if( !arr )
		return 0;
	endif
	foreach v in arr
		if( CInt(v) == CInt(val) )
			return 1;
		endif
	endforeach
	return 0;
endfunction
