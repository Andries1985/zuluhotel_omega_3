// Ritual chant books: one generic item (Item 0xBA2C in config/itemdesc.cfg)
// covers every ritual. CreateRitualChantBookInContainer() bakes the specific
// graphic/color/RitualId into that one instance; the double-click handler
// (chantbook/use.src) reads RitualId back off the item and pulls that
// ritual's own Speech<n> lines straight out of rituals.cfg to render into a
// read-only book gump -- no per-ritual book content is hand-written anywhere.
//
// Gump art: 0xA9C is the large double-page book background used below.
// 0x761C (single page) is available for a ritual with only 1-2 lines if one
// ever needs the smaller layout instead.
use uo;
use os;
use cfgfile;

include ":mdgumps:gumps_ex";
include "../pkg/opt/rituals/include/settings";

const CHANTBOOK_OBJTYPE := 0xBA2C;
const CHANTBOOK_GUMP_DOUBLE := 0xA9C;
const CHANTBOOK_GUMP_SINGLE := 0x761C;

// 0xA9C's own reported width is 781, overall height ~500. Page rectangles
// below are back-calculated from a screenshot where the user marked the
// real page corners against a known Z-coordinate ruler (see
// textcmd/test/chantbookcalib.src) -- close, but still estimated from a
// screenshot, not measured art data, so treat as refinable.
const CHANTBOOK_PAGE_WIDTH := 781;
const CHANTBOOK_PAGE_HEIGHT := 500;

const CHANTBOOK_LEFT_X := 96;
const CHANTBOOK_LEFT_WIDTH := 244;
const CHANTBOOK_RIGHT_X := 434;
const CHANTBOOK_RIGHT_WIDTH := 261;
const CHANTBOOK_TOP_Y := 36; // one row lower than the confirmed page-top (20), per feedback
const CHANTBOOK_BOTTOM_Y := 457;
const CHANTBOOK_LINE_HEIGHT := 16;
const CHANTBOOK_NAV_Y := 465; // wooden-frame strip below the page, confirmed clear of the text area

// Graphic pools:
//  Pool A (0x7306-0x730F, 0xA75F, 0xA760) - the 10 lowest-risk rituals,
//  one graphic picked by hand per ritual. Originally untinted; now each has
//  its own unique ore/log color too (see RITUAL_ChantBookColor) so no book
//  is left as plain color 0.
//  Pool B (0xA92D, 0xA92F) - the 12 "physical/warding" rituals, one of the
//  two graphics per ritual, tinted with a cool ore-inspired hue (can be
//  refined later).
//  Perilous Theurgy and Racial Theurgy each get their own dedicated
//  graphic/color, matching an existing GM item's hue as requested.
function RITUAL_ChantBookGraphic(ritualid)

	if(ritualid == "DrawCircle" || ritualid == "UndoCircle")
		return 0x7307; // the one remaining unused Pool A graphic
	endif

	case(CInt(ritualid))
		131: return 0x730F; // Advanced Theurgy
		132: return 0x7306; // Attunement
		134: return 0x7309; // Consecration
		135: return 0x730C; // Create Focus
		138: return 0xA760; // Free Movement
		140: return 0x730A; // Mana Dissimal
		141: return 0x730E; // Mana Flux
		145: return 0x730D; // Protective Aura
		146: return 0xA75F; // Purification
		152: return 0x7308; // Venom Bane
		155: return 0x730B; // Demonstration

		133: return 0xA92F; // Blood Seeking
		136: return 0xA92F; // Cursing
		137: return 0xA92D; // Elemental Ward
		139: return 0xA92D; // Hardening
		143: return 0xA92D; // Physical Ward
		144: return 0xA92D; // Planard Ward
		147: return 0xA92F; // Quick Healing
		149: return 0xA92D; // Resilience
		150: return 0xA92F; // Spell Bouncing
		151: return 0xA92D; // Spell Warding
		153: return 0xA92F; // Venom Mastery
		154: return 0xA92F; // Vital Infusion

		142: return 0xA92F; // Perilous Theurgy
		148: return 0xA930; // Racial Theurgy

		default: return 0x7306;
	endcase

endfunction

function RITUAL_ChantBookColor(ritualid)

	if(ritualid == "DrawCircle")
		return 1468;
	elseif(ritualid == "UndoCircle")
		return 1469;
	endif

	case(CInt(ritualid))
		155: return 1972; // Demonstration

		// Pool B - cool ore-inspired hues, from this shard's own mining
		// itemdesc (pkg/std/mining/itemdesc.cfg).
		139: return 1111; // Hardening        - PlatinumOre
		149: return 1001; // Resilience       - SilverrockOre
		143: return 1105; // Physical Ward    - BlackdwarfOre
		137: return 1152; // Elemental Ward   - IcerockOre
		144: return 2730; // Planard Ward     - SpectralOre
		151: return 383;  // Spell Warding    - MysticOre
		133: return 1223; // Blood Seeking    - SpikeOre
		154: return 1247; // Vital Infusion   - AzuriteOre
		147: return 2760; // Quick Healing    - DripstoneOre
		136: return 633;  // Cursing          - UndeadOre
		150: return 1109; // Spell Bouncing   - OnyxOre
		153: return 2734; // Venom Mastery    - MalachiteOre

		// Matches Shield of Alryc's EnchantColor (pkg/systems/combat/config/itemdesc.cfg)
		142: return 2665; // Perilous Theurgy

		// Matches the ritual robe's Color / Bane of the Undead's EnchantColor
		148: return 2173; // Racial Theurgy

		// Formerly untinted "Pool A" -- every ritual needs a color, so these
		// 10 were given their own unique ore/log hue too (same source files
		// as Pool B above and pkg/std/lumberjacking/itemdesc.cfg), picked
		// loosely to fit each ritual's own flavor text.
		131: return 1917; // Advanced Theurgy - OrichalcumOre (alchemical gold)
		132: return 2301; // Attunement       - SilverleafLog (guardian spirit, silver)
		134: return 2299; // Consecration     - GoddessOre (four elements' blessing)
		135: return 2243; // Create Focus     - CrystalOre (stored mana crystal)
		138: return 1346; // Free Movement    - StormTealLog (wind/air)
		140: return 48;   // Mana Dissimal    - GoldenReflectionsLog (Air's gentler side)
		141: return 2298; // Mana Flux        - DestructionOre (chaos/calm paradox)
		145: return 2758; // Protective Aura  - PeachBlueOre (sheltering calm)
		146: return 2741; // Purification     - JadeLog (Water's cleansing flow)
		152: return 1131; // Venom Bane       - DarkpaganOre (venom warding)

		default: return 0;
	endcase

endfunction

function RITUAL_ChantBookTitle(ritualid)

	if(ritualid == "DrawCircle")
		return "Drawing the Circle";
	elseif(ritualid == "UndoCircle")
		return "Closing the Circle";
	endif

	case(CInt(ritualid))
		131: return "Advanced Theurgy";
		132: return "Attunement";
		133: return "Blood Seeking";
		134: return "Consecration";
		135: return "Create Focus";
		136: return "Cursing";
		137: return "Elemental Ward";
		138: return "Free Movement";
		139: return "Hardening";
		140: return "Mana Dissimal";
		141: return "Mana Flux";
		142: return "Perilous Theurgy";
		143: return "Physical Ward";
		144: return "Planard Ward";
		145: return "Protective Aura";
		146: return "Purification";
		147: return "Quick Healing";
		148: return "Racial Theurgy";
		149: return "Resilience";
		150: return "Spell Bouncing";
		151: return "Spell Warding";
		152: return "Venom Bane";
		153: return "Venom Mastery";
		154: return "Vital Infusion";
		155: return "Demonstration";
		default: return "Ritual";
	endcase

endfunction

// A short in-universe synopsis of what each ritual actually does, distilled
// from that ritual's own Speech/Print lines in rituals.cfg rather than
// asserting the exact underlying game mechanic (loot's enchant-slot rules
// are shard-internal bookkeeping, not lore) -- shown under "Directions:" in
// the chant book gump.
function RITUAL_ChantBookFlavor(ritualid)

	if(ritualid == "DrawCircle")
		return "The universal rite that opens every working -- calling on the cosmic energy and the four elements in turn to raise the circle any ritual must be cast within.";
	elseif(ritualid == "UndoCircle")
		return "The universal rite that closes every working -- giving thanks to the elements in turn and returning their gathered energy back to the world once the ritual is complete.";
	endif

	case(CInt(ritualid))
		131: return "Draws upon the alchemist's own reagents -- garlic, black pearl, blood moss, ginseng, mandrake, nightshade, sulfurous ash, and spider's silk -- weaving their combined virtues into the item to harness a deeper reservoir of magical power.";
		132: return "Calls forth a spirit from beyond the veil and binds it to the item as an eternal guardian, sworn to see it returned to its owner's hands no matter how far it strays.";
		133: return "Offers a weapon to the fury of Fire, sanctifying it in blood so that all felled by its edge become tribute to Fire's glory.";
		134: return "Passes an item through the blessing of all four elements in turn -- Earth, Water, Fire, and Air -- consecrating it and marking it as ready for further ritual work.";
		135: return "Calls upon the Earth to bless a crystal, filling it with stored mana that can later be drawn upon to power another ritual.";
		136: return "Channels the caster's own pain and anguish through Fire's vengeful flame, binding that darkness permanently into the item and blackening it with a lasting curse.";
		137: return "Wraps an item in the combined shelter of Earth, Fire, Air, and Water, warding its bearer against the fury of the elements.";
		138: return "Calls upon the great winds of Air to lift the item free of every shackle, granting its bearer freedom from that which would otherwise root them in place.";
		139: return "Draws on the eternal endurance of the mountains, hardening the item's substance until it can shrug off blows the way stone endures the ages.";
		140: return "Calls on Air's gentler side to grant the item's bearer a measure of peace from petty spells and magics that would otherwise trouble them.";
		141: return "Draws on the eye of the hurricane's paradox -- chaos and calm intertwined -- binding that raw, wild energy into the item.";
		142: return "Calls forth the living Shadow beyond the Void and unseals its long-forbidden power, grafting a fragment of that dark essence permanently into the item.";
		143: return "Draws on the mountains' unbroken endurance through war, peace, and the ages, transferring a portion of that unyielding resilience into the item.";
		144: return "Opens a gate to the worlds beyond and draws essence from both the brightest and darkest of them, binding that balance into the item.";
		145: return "Gathers Air's power into a dense, sheltering core, wrapping the item's bearer in a calm at the center of any storm of violence.";
		146: return "Calls upon Water's endless, purifying flow to wash every trace of impurity and curse from the item, cleansing it completely.";
		147: return "Draws on Water as the source of all life, granting the item a portion of its essence so that its bearer might heal more swiftly.";
		148: return "Summons the strongest of the abyss's vengeful spirits and binds their hatred to the item, turning old suffering into a weapon's edge.";
		149: return "Calls on the unmoving mountain's own sturdiness, granting the item a portion of Earth's raw physical toughness.";
		150: return "Weaves an unseen curtain of Air around the item, one that turns hostile magic back upon whoever cast it.";
		151: return "Raises the item above the notice of mortal spellcraft, letting its bearer walk through magical blazes untouched.";
		152: return "Raises a barrier against venom itself, sealing the item against the poison that fangs and stingers would otherwise deliver.";
		153: return "Twists venom's very purpose, turning what once killed into a ward that instead preserves the item's bearer from poison's touch.";
		154: return "Draws on the countless memories of all who have fallen and returned to the earth, binding a measure of their vitality into the item.";
		155: return "A harmless practice working -- every word and step rehearsed with nothing real bound and nothing at risk.";
		default: return "";
	endcase

endfunction

// Mirrors GetRitualItem()'s own TargOptions case block (pkg/opt/rituals/
// include/rituals.inc) so the chant book always names exactly what the
// ritual will actually accept, never something GetRitualItem would reject.
function RITUAL_TargOptionsWord(targoptions)

	case(targoptions)
		"crystal": return "Mana Crystals";
		"ritual": return "Ritual Robes and Ritual Staves";
		"jewel": return "Jewelry";
		"armor": return "Bone, Chainmail, Platemail, Leather, Ringmail, or Studded Leather Armor, and Shields";
		"hp": return "Any Item With Durability";
		"jewelArmor": return "Armor or Jewelry";
		"weapon": return "Weapons";
		"cloth": return "Clothing";
		"equippable": return "Any Equippable Item";
		"clothJewel": return "Clothing or Jewelry";
		"wearable": return "Armor, Clothing, or Jewelry";
		"weaponArmor": return "Weapons, Armor, or Jewelry";
		"cursed": return "Cursed Items";
		"none": return "Nothing -- This is a Practice Working";
		default: return "Any Item";
	endcase

endfunction

// `ritualid` is either a numeric ritual id (131-155) or one of the two
// special string keys "DrawCircle"/"UndoCircle" for the universal circle
// rites, which live in rituals.cfg under those string keys instead of a
// number. Stored on the item as-is (int or string) -- ShowRitualChantBookGump
// figures out which kind it's looking at the same way these tables do.
function CreateRitualChantBookInContainer(container, ritualid)

	if(CInt(ritualid))
		ritualid := CInt(ritualid);
	endif

	var book := CreateItemInContainer(container, CHANTBOOK_OBJTYPE);
	if(book.errortext)
		return book;
	endif

	book.graphic := RITUAL_ChantBookGraphic(ritualid);
	book.color := RITUAL_ChantBookColor(ritualid);
	SetObjProperty(book, "RitualId", ritualid);
	SetName(book, RITUAL_ChantBookTitle(ritualid)+" chant book");

	return book;
endfunction

function RITUAL_FacingWord(facing)

	case(CInt(facing))
		0: return "North";
		1: return "Northeast";
		2: return "East";
		3: return "Southeast";
		4: return "South";
		5: return "Southwest";
		6: return "West";
		7: return "Northwest";
		default: return "North";
	endcase

endfunction

function RITUAL_PositionWord(x_mod, y_mod)

	if(!CInt(x_mod) && !CInt(y_mod))
		return "Stand at the circle's north point.";
	endif

	return "Stand at X:"+CStr(CInt(x_mod))+" Y:"+CStr(CInt(y_mod))+" from the circle's north point.";
endfunction

// True if every one of this ritual's lines shares the same Facing/Xmod/
// Ymod -- true for every real ritual currently in rituals.cfg (all of them
// are Facing 4/South with no Xmod/Ymod at all), so the chant book can show
// one shared instruction instead of repeating it once per line.
function RITUAL_LinesShareDirection(elem, total)

	var facing := GetConfigInt(elem, "Facing1");
	var x_mod := GetConfigInt(elem, "Xmod1");
	var y_mod := GetConfigInt(elem, "Ymod1");

	var i;
	for(i:=2; i<=total; i+=1)
		if(GetConfigInt(elem, "Facing"+i) != facing || GetConfigInt(elem, "Xmod"+i) != x_mod || GetConfigInt(elem, "Ymod"+i) != y_mod)
			return 0;
		endif
	endfor

	return 1;
endfunction

// Reads RitualId off `book` (an int for a real ritual, or one of the
// special strings "DrawCircle"/"UndoCircle" for the two universal circle
// rites), pulls its Speech<n>/Facing<n>/Xmod<n>/Ymod<n> fields straight out
// of rituals.cfg, and renders them as plain black GFTextLine rows
// (GFWordWrap does the wrapping by hand -- GFHTMLArea was rendering washed
// out and could scroll off the visible page). Left page: Directions/Lore/
// Suitable Vessels. Right page: "Incantations in Order:" followed by the
// words themselves, unnumbered.
//
// GFPage(gump, 0) is the client's "always visible regardless of active
// page" background layer (see GFPage()'s own doc comment), so the
// background art and the whole left column are drawn there ONCE -- they
// then stay visible no matter which incantation page is active. Real
// navigable pages start at 1 (books used to start at page 0 for this,
// which meant the actual first page of incantations never displayed by
// default -- that was the "opens on page 2" bug). Short rituals
// (Consecration's 4 lines) fit page 1 alone with no page-turn buttons at
// all -- right-click closes it like any other gump. Longer rituals (up to
// 16 lines) spill onto additional page-turn spreads, sized by each entry's
// actual wrapped line count rather than a guess.
function ShowRitualChantBookGump(who, book)

	var ritualid := GetObjProperty(book, "RitualId");
	if(!ritualid)
		SendSysMessage(who, "This book has no ritual bound to it.");
		return 0;
	endif

	var cfg_key := ritualid;
	if(CInt(ritualid))
		cfg_key := CInt(ritualid);
	endif

	var elem := RITUAL_GetSettingsCfgElem(cfg_key);
	if(elem.errortext)
		SendSysMessage(who, "That ritual's chant text couldn't be found.");
		return 0;
	endif

	var title := RITUAL_ChantBookTitle(ritualid);

	var incantations := array;
	var i := 1;
	while(GetConfigString(elem, "Speech"+i))
		incantations.Append("\""+GetConfigString(elem, "Speech"+i)+"\"");
		i := i + 1;
	endwhile

	var total := Len(incantations);

	// One paragraph when every line shares the same Facing/Xmod/Ymod (every
	// real ritual), or one paragraph PER LINE when they don't (DrawCircle/
	// UndoCircle) -- kept as separate array entries, not one concatenated
	// string, so each numbered line always starts on its own row instead of
	// running together as one wrapped block of prose.
	var directions_paragraphs := array;
	if(total > 0 && RITUAL_LinesShareDirection(elem, total))
		directions_paragraphs.Append("Face "+RITUAL_FacingWord(GetConfigInt(elem, "Facing1"))+". "+RITUAL_PositionWord(GetConfigInt(elem, "Xmod1"), GetConfigInt(elem, "Ymod1")));
	else
		var j;
		for(j:=1; j<=total; j+=1)
			// UndoCircle's steps 7-9 repeat the same facing/position as earlier
			// steps (all Face South, circle center) -- skip them here so the
			// Directions list doesn't list the same instruction 4 times.
			if(ritualid == "UndoCircle" && (j == 7 || j == 8 || j == 9))
				continue;
			endif
			directions_paragraphs.Append(CStr(j)+". Face "+RITUAL_FacingWord(GetConfigInt(elem, "Facing"+j))+". "+RITUAL_PositionWord(GetConfigInt(elem, "Xmod"+j), GetConfigInt(elem, "Ymod"+j)));
		endfor
	endif

	var gump := GFCreateGump();

	// Page 0: background + the entire left column, drawn once, always visible.
	GFPage(gump, 0);
	GFGumpPic(gump, 0, 0, CHANTBOOK_GUMP_DOUBLE);

	var ly := CHANTBOOK_TOP_Y;
	GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, title);
	ly := ly + (CHANTBOOK_LINE_HEIGHT * 2);

	GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, "Directions:");
	ly := ly + CHANTBOOK_LINE_HEIGHT;
	foreach para in directions_paragraphs
		foreach line in (GFWordWrap(para, CHANTBOOK_LEFT_WIDTH))
			GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, line);
			ly := ly + CHANTBOOK_LINE_HEIGHT;
		endforeach
	endforeach

	var flavor_text := RITUAL_ChantBookFlavor(ritualid);
	if(flavor_text)
		ly := ly + CHANTBOOK_LINE_HEIGHT;
		GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, "Lore:");
		ly := ly + CHANTBOOK_LINE_HEIGHT;
		foreach line in (GFWordWrap(flavor_text, CHANTBOOK_LEFT_WIDTH))
			GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, line);
			ly := ly + CHANTBOOK_LINE_HEIGHT;
		endforeach
	endif

	// DrawCircle/UndoCircle have no TargOptions at all -- they don't
	// enchant an item, so "Suitable Vessels" doesn't apply and is skipped
	// entirely rather than shown with a "nothing" placeholder.
	var targ_options := GetConfigString(elem, "TargOptions");
	if(targ_options)
		ly := ly + CHANTBOOK_LINE_HEIGHT;
		GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, "Suitable Vessels:");
		ly := ly + CHANTBOOK_LINE_HEIGHT;
		foreach line in (GFWordWrap(RITUAL_TargOptionsWord(targ_options), CHANTBOOK_LEFT_WIDTH))
			GFTextLine(gump, CHANTBOOK_LEFT_X, ly, 0, line);
			ly := ly + CHANTBOOK_LINE_HEIGHT;
		endforeach
	endif

	// Pages 1+: the incantations, paginated by real wrapped height.
	var page := 1;
	var entry_idx := 1;
	var page_started := 0;

	while(entry_idx <= total || !page_started)
		GFPage(gump, page);
		page_started := 1;

		var y := CHANTBOOK_TOP_Y;
		if(page == 1)
			GFTextLine(gump, CHANTBOOK_RIGHT_X, y, 0, "Incantations in Order:");
			y := y + (CHANTBOOK_LINE_HEIGHT * 2);
		endif

		while(entry_idx <= total)
			var inc_lines := GFWordWrap(incantations[entry_idx], CHANTBOOK_RIGHT_WIDTH);
			var block_height := (Len(inc_lines)+1) * CHANTBOOK_LINE_HEIGHT; // +1 blank row between entries

			if((y + block_height) > CHANTBOOK_BOTTOM_Y && y > CHANTBOOK_TOP_Y)
				break; // doesn't fit -- leave it for the next page
			endif

			foreach line in inc_lines
				GFTextLine(gump, CHANTBOOK_RIGHT_X, y, 0, line);
				y := y + CHANTBOOK_LINE_HEIGHT;
			endforeach
			y := y + CHANTBOOK_LINE_HEIGHT;

			entry_idx := entry_idx + 1;
		endwhile

		if(page > 1)
			GFAddButton(gump, CHANTBOOK_LEFT_X, CHANTBOOK_NAV_Y, 4014, 4016, GF_PAGE_BTN, page-1);
		endif
		if(entry_idx <= total)
			GFAddButton(gump, CHANTBOOK_RIGHT_X+CHANTBOOK_RIGHT_WIDTH-16, CHANTBOOK_NAV_Y, 4005, 4007, GF_PAGE_BTN, page+1);
		endif

		page := page + 1;
	endwhile

	GFSendGump(who, gump);

	return 1;
endfunction
