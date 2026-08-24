var GUILD_PRICE := 120000;
var TEXT_ENTRY_GUILDNAME := 1;
var TEXT_ENTRY_GUILDABV := 2;

CONST HEAR_ALL := 3;
CONST HEAR_OWN := 2;
CONST HEAR_ALLY := 1;
CONST HEAR_NONE := 0;

CONST TALK_ALL := 3;
CONST TALK_OWN := 2;
CONST TALK_ALLY := 1;
CONST TALK_NONE := 0;

const MAX_GUILD_NAME_SIZE:=45;
const MAX_GUILD_ABREV_SIZE:=6;
const MAX_GUILD_TITLE_SIZE:=20;

const REQUEST_DECLARE_WAR := "DW";
const REQUEST_END_WAR := "EW";
const REQUEST_FORM_ALLIANCE := "RA";

const SEND_SURRENDER := "S";

const STATUS_WAR_STARTING := "WS";
const STATUS_WAR_ENDING := "WE";
const STATUS_ALLIANCE_FORMING := "FA";
const STATUS_ALLIANCE_ENDING := "EA";

const GUILD_UNIFORM_PRICE := 150000;
const GUILD_COLOUR_TIME := 24; //hours
const GUILD_COLOUR_TIME_SET := "TimeGuildColour";
const GUILD_NAME_TIME := 7; // days
const GUILD_NAME_TIME_SET := "TimeGuildName";
const GUILD_UNIFORM := "GuildUniform";
const GUILD_COLOUR := "GuildColour";
const GUILD_NAME := "GuildName";
const GUILD_ABV := "GuildAbv";

const GUILD_MEMBER_TITLE := "GuildTitle";

// Guild-side property (parallel to "GuildHouse") holding the serial of the
// guild's placed guildstone item, if any.
const GUILD_STONE := "GuildStone";

const GUILDSTONE_OBJTYPE := 0x1A390;
const GUILDSTONE_DEFAULT_GRAPHIC := 0xed6;

// Doors swing/open into adjacent tiles, so keep the guildstone (and any
// statue-set companion pieces, which are wider) clear of them.
const GUILDSTONE_DOOR_PLACEMENT_DISTANCE := 2;
const GUILDSTONE_DOOR_SET_DISTANCE := 4;

// Statue-set guildstones (multi-piece companions) require a guild house at
// least Tower-sized. Tower (objtype 0x16069)'s own footprint is
// {x-10,y-6,x+11,y+5} -- 22 x 12 = 264 tiles -- per
// :house:multiDeed/use.src's GetHouseFootprintBounds(). Compared against
// each house's actual footprint (via GetMultiDimensions()) rather than a
// name/type whitelist, since regular housing alone has 150+ individually
// named house types and custom housing has none at all.
const GUILDSTONE_SET_MIN_HOUSE_AREA := 264;

// Guild membership ranks -- display/record-keeping only for now, no
// gameplay effect yet. Stored in the registry datafile (see guilds.inc's
// GetGuildMemberRanks()/SetGuildMemberRanks()), which is the sole source of
// truth for ranks -- unlike GuildHouse/GuildStone there's no guild CProp
// mirror, since nothing else in the codebase reads/writes rank yet.
const GUILD_RANK_LEADER := "Guild Leader";
const GUILD_RANK_OFFICER := "Officer";
const GUILD_RANK_VETERAN := "Veteran";
const GUILD_RANK_MEMBER := "Member";
const GUILD_RANK_RECRUIT := "Recruit";
const GUILD_DEFAULT_RANK := "Member";

const GUILD_MEMBER_RANKS_PROP := "MemberRanks";

// Single ordered list driving the Membership gump's rank columns (header
// labels, per-row radio buttons, and the button-id -> rank mapping read back
// in ApplyMembershipRankChanges) so all three always agree on which column
// is which rank instead of independently repeating the same order.
// GUILD_RANK_COLUMN_X is each entry's header-label x; radio buttons sit 25px
// right of their header, matching every other column in that gump.
var GUILD_ASSIGNABLE_RANKS := {GUILD_RANK_OFFICER, GUILD_RANK_VETERAN, GUILD_RANK_MEMBER, GUILD_RANK_RECRUIT};
var GUILD_RANK_COLUMN_X := {354, 438, 522, 606};

// Guild house permissions -- per-rank secure/teleporter/lockdown/manager
// access to the guild's own house, mirroring :house:'s friend permission
// system. Stored in the registry datafile (GetGuildHousePerms()/
// SetGuildHousePerms() in guilds.inc), read directly (no shared include) by
// :house:multiSign/method.src's HasHousePermission() -- see that function's
// GuildRankHasHousePermission() for the consuming side. These 4 string
// values are the actual perm_key strings HasHousePermission() expects; keep
// them in sync with that file if ever renamed.
const GUILD_HOUSE_PERM_SECURE := "secure";
const GUILD_HOUSE_PERM_TELEPORTER := "teleporter";
const GUILD_HOUSE_PERM_LOCKDOWN := "lockdown";
const GUILD_HOUSE_PERM_MANAGER := "house_manager";

const GUILD_HOUSE_PERMS_PROP := "HousePerms";

// Unlike GUILD_ASSIGNABLE_RANKS (Membership gump, leader excluded -- fixed,
// uncheckable there), the Guild Management gump gives the leader its own
// normal, editable row: the current guild master isn't guaranteed to be the
// house's actual owner if leadership changed hands since the house was set,
// so without a configurable row a new leader could be locked out.
var GUILD_HOUSE_PERM_RANKS := {GUILD_RANK_LEADER, GUILD_RANK_OFFICER, GUILD_RANK_VETERAN, GUILD_RANK_MEMBER, GUILD_RANK_RECRUIT};

// x position, header label, and stored-flag key for each of the 8
// permission checkboxes, in column order -- one shared source for the gump's
// headers, its checkboxes, and ApplyGuildHousePermsChanges()'s unpacking, so
// the three can't independently drift out of sync (the button-id -> rank
// mistake this exact pattern was introduced to prevent, see
// GUILD_ASSIGNABLE_RANKS above).
// 2026-08-13: shifted left from {270, 350, ... 830} -- the last column sat
// right under the panel's right-side dragon decoration (x=838 in
// ShowGuildHouseManagementGump), running "Manager" off the edge.
var GUILD_HOUSE_PERM_COLUMN_X := {215, 295, 375, 455, 535, 615, 695, 775};
var GUILD_HOUSE_PERM_COLUMN_LABEL := {"View", "Add", "Remove", "Recall", "Gate To", "Gate From", "Lockdown", "Manager"};
var GUILD_HOUSE_PERM_COLUMN_KEY := {GUILD_HOUSE_PERM_SECURE, GUILD_HOUSE_PERM_SECURE, GUILD_HOUSE_PERM_SECURE, GUILD_HOUSE_PERM_TELEPORTER, GUILD_HOUSE_PERM_TELEPORTER, GUILD_HOUSE_PERM_TELEPORTER, GUILD_HOUSE_PERM_LOCKDOWN, GUILD_HOUSE_PERM_MANAGER};

// guild clothes
var CLOTHING := {0x1EFD, 0x1EFF, 0x1F01, 0x1F03, 0x1F7B, 0x1F9F, 0x1FA1, 0x1FFD, 0x1FFE, 0x1515, 0x1516, 0x1517, 0x152E, 0x1537, 0x1539, 0x153B, 0x153D, 0x1541, 0x153F, 0x1543, 0x141b, 0x1545, 0x1547, 0x1549, 0x154c, 0x1713, 0x1714, 0x1715, 0x1716, 0x1717, 0x1718, 0x1719, 0x171A, 0x171B, 0x171C, 0x170B, 0x170D, 0x170F, 0x1711};

// Guild colour picker (SetGuildColour() in guilds.src) now offers the full
// dyeable hue range 1000-2999, minus a handful of bands that are skin tones,
// near-duplicates, or otherwise not worth offering -- pruned by hand
// 2026-08-23. Swatches are shown GUILD_COLOUR_PER_PAGE at a time, each page
// its own gump send (a single gump covering all ~1300 swatches at once has
// broken the server before).
var GUILD_COLOUR_EXCLUDE_RANGES := {
	{1000,1082}, {1102,1149}, {1201,1254}, {1301,1354}, {1401,1454},
	{1501,1554}, {1601,1654}, {1702,1781}, {1801,1899}, {1901,1908},
	{2001,2018}, {2101,2130}, {2201,2224}, {2300,2318}, {2401,2430}
};

const GUILD_COLOUR_PER_PAGE := 150;

function IsGuildColourExcluded(hue)
	foreach range in GUILD_COLOUR_EXCLUDE_RANGES
		if (hue >= range[1] && hue <= range[2])
			return 1;
		endif
	endforeach
	return 0;
endfunction

function BuildGuildColours()
	var colours := array;
	var hue;
	for (hue := 1000; hue <= 2999; hue := hue + 1)
		if (!IsGuildColourExcluded(hue))
			colours.append(hue);
		endif
	endfor
	return colours;
endfunction

var COLOURS := BuildGuildColours();