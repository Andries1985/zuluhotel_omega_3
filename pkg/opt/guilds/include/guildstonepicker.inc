use uo;
use basic;
use cfgfile;

// Relies on the including script already having a working GF*/GFSendGump
// gump framework in scope (guilds.src pulls it in transitively via
// ":gumps:autoClose"), and DestroyGuildstoneCompanions()/
// SpawnGuildstoneSetCompanions() in scope (guilds.src pulls those in via
// ":guilds:include/guilds", which itself includes
// ":guilds:items/guildstone/companions" for DisbandGuild()'s cleanup).
// Deliberately does NOT re-include ":mdgumps:gumps", ":gumps:gumps", or
// "companions" here -- any of those already being present via the
// includer's own chain means a second copy would be a duplicate-function
// compile error.

const GS_CAT_SINGLES := 1;
const GS_CAT_SETS := 2;
const GS_CAT_COLOUR := 3;
const GS_CAT_RESET_COLOUR := 4;
const GS_CAT_BACK := 5;
const GS_CAT_CLOSE := 6;

const GS_PICK_PREV := 1;
const GS_PICK_NEXT := 2;
const GS_PICK_MENU := 3;
const GS_PICK_CLOSE := 4;

const GS_NAV_BACK := 1;
const GS_NAV_CLOSE := 2;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowGraphicCategoryMenu() - Walks the player through Singles/Statue Sets category selection
// and the matching picker. preview:=1 browses candidate graphics without ever touching the real
// guildstone item; preview:=0 applies each choice to item live as it's picked.
//
////////////////////////////////////////////////////////////////////////////////////////////////
function ShowGraphicCategoryMenu(who, item, preview := 0)

	while (1)
		var cat := ShowCategoryMenu(who, preview);

		if (cat == GS_CAT_SINGLES)
			var nav := ShowGuildstoneGraphicPicker(who, item, preview, 0);
			if (nav == GS_NAV_CLOSE)
				return GS_NAV_CLOSE;
			endif
		elseif (cat == GS_CAT_SETS)
			var nav := ShowGuildstoneGraphicPicker(who, item, preview, 1);
			if (nav == GS_NAV_CLOSE)
				return GS_NAV_CLOSE;
			endif
		elseif (cat == GS_CAT_COLOUR)
			ApplyGuildColourToGuildstone(who, item);
		elseif (cat == GS_CAT_RESET_COLOUR)
			ResetGuildstoneColour(who, item);
		elseif (cat == GS_CAT_BACK)
			return GS_NAV_BACK;
		else
			return GS_NAV_CLOSE;
		endif
	endwhile

endfunction

function ShowCategoryMenu(who, preview := 0)

	var gump := GFCreateGump();
	GFPage(gump, 0);

	// Matching/resetting the colour actually recolours the real guildstone,
	// so both are only offered in the guild master's real "Change" flow,
	// not the member-facing read-only preview -- two extra rows taller.
	var panel_height := 150;
	if (!preview)
		panel_height := 210;
	endif
	GFResizePic(gump, 50, 50, 39925, 280, panel_height);
	if (preview)
		GFTextLine(gump, 70, 68, 1097, "View Guildstone Graphic");
	else
		GFTextLine(gump, 70, 68, 1097, "Change Guildstone Graphic");
	endif

	GFAddButton(gump, 70, 100, 4005, 4007, GF_CLOSE_BTN, GS_CAT_SINGLES);
	GFTextLine(gump, 105, 102, 1097, "Singles");

	GFAddButton(gump, 70, 130, 4005, 4007, GF_CLOSE_BTN, GS_CAT_SETS);
	GFTextLine(gump, 105, 132, 1097, "Statue Sets");

	var back_y := 160;
	if (!preview)
		GFAddButton(gump, 70, 160, 4005, 4007, GF_CLOSE_BTN, GS_CAT_COLOUR);
		GFTextLine(gump, 105, 162, 1097, "Match Guild Colour");

		GFAddButton(gump, 70, 190, 4005, 4007, GF_CLOSE_BTN, GS_CAT_RESET_COLOUR);
		GFTextLine(gump, 105, 192, 1097, "Reset Statue Colour");

		back_y := 220;
	endif

	GFAddButton(gump, 70, back_y, 4005, 4007, GF_CLOSE_BTN, GS_CAT_BACK);
	GFTextLine(gump, 105, back_y + 2, 1097, "Back");

	GFAddButton(gump, 280, 60, 4017, 4019, GF_CLOSE_BTN, GS_CAT_CLOSE);

	var data := GFSendGump(who, gump);
	if (!data)
		return 0;
	endif

	return data[0];

endfunction

// Validates every companion piece of elem's statue set -- not just the
// anchor tile -- against the same guild-house-footprint and door-distance
// rules the anchor itself already has to satisfy. Without this, only the
// anchor's own position was checked, so a set with a wide piece offset could
// land a companion outside the house or right up against a door even though
// the anchor tile itself passed both checks.
function IsGuildstoneSetPlacementValid(who, item, elem, piece_count)
	var house := ResolveGuildHouse(who.guild);
	var piece;
	for (piece := 2; piece <= piece_count; piece := piece + 1)
		var dx := GetConfigInt(elem, "Piece" + piece + "DX");
		var dy := GetConfigInt(elem, "Piece" + piece + "DY");

		var piece_loc := struct;
		piece_loc.x := item.x + dx;
		piece_loc.y := item.y + dy;
		piece_loc.z := item.z;
		piece_loc.realm := item.realm;

		if (!IsInsideGuildHouse(house, piece_loc))
			SendSysMessage(who, "That statue set would place a piece outside your guild house.", 1, 33);
			return 0;
		endif

		if (IsNearDoor(piece_loc, GUILDSTONE_DOOR_SET_DISTANCE))
			SendSysMessage(who, "That statue set would place a piece too close to a door.", 1, 33);
			return 0;
		endif
	endfor

	return 1;
endfunction

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ShowGuildstoneGraphicPicker() - Shared browse/apply loop for both the Singles and Statue Sets
// categories (is_set:=0/1). They differ only in which config file/field they read, the header
// text, and what "applying a pick" means (a plain graphic swap for singles vs. destroy +
// revalidate + respawn companions for sets) -- everything else (gump layout, prev/next
// wraparound, Back/Close dispatch) is identical, so it's one function branching on is_set
// instead of two ~90-line near-twins.
//
////////////////////////////////////////////////////////////////////////////////////////////////
function ShowGuildstoneGraphicPicker(who, item, preview, is_set)

	// Statue sets spawn companion pieces around item, so keep them further
	// from doors than a lone guildstone needs to be, and require a big
	// enough guild house to have room for them. Checked once up front
	// (neither item's position nor the guild's house change while
	// browsing) and only when actually applying -- preview mode never
	// touches the real guildstone, so it's still fine to browse sets
	// regardless of door distance or house size there.
	if (is_set && !preview)
		if (IsNearDoor(item, GUILDSTONE_DOOR_SET_DISTANCE))
			SendSysMessage(who, "The guildstone is too close to a door to become a statue set.", 1, 33);
			return GS_NAV_BACK;
		endif

		if (!IsHouseTowerSizeOrBigger(ResolveGuildHouse(who.guild)))
			SendSysMessage(who, "Your guild house must be Tower-sized or bigger to use a statue set guildstone.", 1, 33);
			return GS_NAV_BACK;
		endif
	endif

	var cfg_name := ":guilds:guildstonegraphics";
	var graphic_field := "Graphic";
	var kind_label := "guildstone graphic";
	if (is_set)
		cfg_name := ":guilds:guildstonesets";
		graphic_field := "Piece1Graphic";
		kind_label := "guildstone statue set";
	endif

	var cfg := ReadConfigFile(cfg_name);
	if (!cfg)
		SendSysMessage(who, "The "+kind_label+" list could not be loaded.", 1, 33);
		return GS_NAV_BACK;
	endif

	var keys := GetConfigIntKeys(cfg);
	var total := keys.size();
	if (total < 1)
		SendSysMessage(who, "No "+kind_label+"s are configured.", 1, 33);
		return GS_NAV_BACK;
	endif

	var index;
	if (is_set)
		index := FindGuildstoneConfigIndex(cfg, total, "SetNumber", GetObjProperty(item, "GSSetNumber"));
	else
		index := FindGuildstoneConfigIndex(cfg, total, "Graphic", item.graphic);
	endif
	if (!index)
		index := 1;
	endif

	while (1)
		var elem := FindConfigElem(cfg, index);
		var name := GetConfigString(elem, "Name");
		var graphic := GetConfigInt(elem, graphic_field);

		var gump := GFCreateGump();
		GFPage(gump, 0);

		GFResizePic(gump, 50, 50, 302, 320, 230);

		GFAddButton(gump, 70, 60, 4005, 4007, GF_CLOSE_BTN, GS_PICK_MENU);
		GFTextLine(gump, 105, 62, 1097, "Back");

		GFAddButton(gump, 340, 60, 4017, 4019, GF_CLOSE_BTN, GS_PICK_CLOSE);

		if (is_set)
			var piece_count := GetConfigInt(elem, "PieceCount");
			GFTextLine(gump, 70, 90, 1097, "Statue Set (" + index + " / " + total + ")");
			GFTextLine(gump, 70, 195, 1097, "Name: " + name + " (" + piece_count + " pieces)");
		else
			GFTextLine(gump, 70, 90, 1097, "Guildstone Graphic (" + index + " / " + total + ")");
			GFTextLine(gump, 70, 195, 1097, "Name: " + name);
		endif

		if (preview)
			GFTilePic(gump, 260, 110, graphic);
		endif

		GFAddButton(gump, 70, 240, 5603, 5607, GF_CLOSE_BTN, GS_PICK_PREV);
		GFAddButton(gump, 300, 240, 5601, 5605, GF_CLOSE_BTN, GS_PICK_NEXT);

		var data := GFSendGump(who, gump);
		if (!data)
			return GS_NAV_CLOSE;
		endif

		case (data[0])
			GS_PICK_PREV:
				index := index - 1;
				if (index < 1)
					index := total;
				endif
			GS_PICK_NEXT:
				index := index + 1;
				if (index > total)
					index := 1;
				endif
			GS_PICK_MENU:
				return GS_NAV_BACK;
			GS_PICK_CLOSE:
				return GS_NAV_CLOSE;
			default:
				return GS_NAV_CLOSE;
		endcase

		if (!preview)
			var new_elem := FindConfigElem(cfg, index);
			if (is_set)
				var new_piece_count := GetConfigInt(new_elem, "PieceCount");
				if (IsGuildstoneSetPlacementValid(who, item, new_elem, new_piece_count))
					DestroyGuildstoneCompanions(item);
					item.graphic := GetConfigInt(new_elem, "Piece1Graphic");
					SpawnGuildstoneSetCompanions(item, new_elem, new_piece_count);
				endif
			else
				DestroyGuildstoneCompanions(item);
				item.graphic := GetConfigInt(new_elem, "Graphic");
			endif
		endif
	endwhile

endfunction

// Recolours the guildstone (and any statue-set companion pieces) to match
// the guild's own colour. Only reachable from the real "Change Guildstone"
// flow (see ShowCategoryMenu's preview guard), so item is always the guild's
// actual guildstone here, never a preview target.
function ApplyGuildColourToGuildstone(who, item)

	var colour := GuildPropInt(who.guild, GUILD_COLOUR);
	if (!colour)
		SendSysMessage(who, "Your guild does not have a guild colour yet.", 1, 33);
		return;
	endif

	item.color := colour;
	SetGuildstoneCompanionsColour(item, colour);

	SendSysMessage(who, "The guildstone has been recoloured to match your guild.", 3, 67);

endfunction

// Clears any colour applied via ApplyGuildColourToGuildstone(), on the
// guildstone and all its statue-set companion pieces alike.
function ResetGuildstoneColour(who, item)

	item.color := 0;
	SetGuildstoneCompanionsColour(item, 0);

	SendSysMessage(who, "The guildstone's colour has been reset.", 3, 67);

endfunction

// Shared by both pickers -- finds the config entry whose integer field
// matches value (item.graphic for singles, GSSetNumber for sets), so the
// picker can open on the guildstone's current graphic/set instead of always
// starting at entry 1.
function FindGuildstoneConfigIndex(cfg, total, field, value)

	if (!value)
		return 0;
	endif

	var i;
	for (i := 1; i <= total; i := i + 1)
		var elem := FindConfigElem(cfg, i);
		if (GetConfigInt(elem, field) == CInt(value))
			return i;
		endif
	endfor

	return 0;

endfunction
