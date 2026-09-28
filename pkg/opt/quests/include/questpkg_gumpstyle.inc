// QuestPkg -- shared gump-drawing helpers used by the quest journal and
// quest-giver gumps. The consumer must already `include ":mdgumps:gumps_ex";`
// -- this file doesn't, matching the pattern in bulkorder_gumpstyle.inc.

// The real OSI quest gump recipe (gump 0x325/0x320), captured byte-for-byte
// via pkg/opt/alryc/textcmd/test/osiquestgump.src and osiquestnpcgump.src --
// both share the identical outer chrome: a 400x460 parchment panel (5124)
// framed by ornate left/right borders (10460), a hanging left-margin ribbon
// tab (9263 + its 10411/10402 accents), top/bottom banner strips
// (10250/10101) with corner caps (10306/10304), a right-edge binding (2605),
// outer corner accents (10421/10420/10441), and small title-area icons
// (9005/1417/5545) either side of the header divider (9101).
function QuestPkg_DrawOsiFrame(byref gump)
	(gump.layout).Append("gumppictiled 50 20 400 460 5124");
	(gump.layout).Append("gumppictiled 50 29 30 450 10460");
	(gump.layout).Append("gumppictiled 34 140 17 339 9263");
	(gump.layout).Append("gumppic 48 135 10411");
	(gump.layout).Append("gumppic -16 285 10402");
	(gump.layout).Append("gumppic 0 10 10421");
	(gump.layout).Append("gumppic 25 0 10420");
	(gump.layout).Append("gumppictiled 83 15 350 15 10250");
	(gump.layout).Append("gumppic 34 479 10306");
	(gump.layout).Append("gumppic 442 479 10304");
	(gump.layout).Append("gumppictiled 51 479 392 17 10101");
	(gump.layout).Append("gumppictiled 415 29 44 450 2605");
	(gump.layout).Append("gumppictiled 415 29 30 450 10460");
	(gump.layout).Append("gumppic 370 50 1417");
	(gump.layout).Append("gumppic 379 60 5545");
	(gump.layout).Append("gumppic 425 0 10441");
	(gump.layout).Append("gumppic 90 33 9005");
	(gump.layout).Append("gumppictiled 130 65 175 1 9101");
endfunction
