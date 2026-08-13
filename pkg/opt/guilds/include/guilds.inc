use util;
use guilds;
use polsys;
use datafile;
use basicio;
include ":house:utility";
// GetItemDescInfo() (used for house footprint sizing below) -- pulls the
// merged itemdesc.cfg across all packages, so it works uniformly for
// regular housing's ~150 individually named house types and custom
// housing's foundationWxH types alike.
//
// Deliberately NOT including ":itemutils:itemtypes" for door detection --
// it duplicates several function names (IsIngot/IsLog/IsOre/IsReagent/
// IsSign) already defined in the older, still widely-used
// scripts/include/itemutil.inc, which some other scripts pull in
// transitively alongside this file; including both causes a
// duplicate-function compile error. item.isa(POLCLASS_DOOR) below is a bare
// uo.em check instead, needing no extra include.
include ":itemutils:itemdesc";

include ":gumps:include/gumps";
include "guildconstants";
include ":guilds:items/guildstone/companions";

// guild.getprop() returns a truthy error struct (not a plain falsy value)
// for a property that was never set, so a bare "if(!guild.getprop(x))"
// check doesn't reliably detect "unset". Use this wherever an unset guild
// property should read back as a clean 0.
function GuildPropInt(guild, propname)
	var value := guild.getprop(propname);
	if(value == error)
		return 0;
	endif
	if(value && value.errortext)
		return 0;
	endif
	if(!value)
		return 0;
	endif
	return CInt(value);
endfunction

// External, durable mirror of each guild's house/guildstone serials, keyed
// by guild name. guild.getprop("GuildHouse")/.getprop(GUILD_STONE) remain
// the live values everything else reads; this datafile is a backup record
// kept in sync alongside them, not a second source of truth.
const GUILD_REGISTRY_DATAFILE := ":guilds:guild_registry";

function OpenGuildRegistryDataFile()
	var df := OpenDataFile(GUILD_REGISTRY_DATAFILE);
	if(!df)
		CreateDataFile(GUILD_REGISTRY_DATAFILE, DF_KEYTYPE_STRING);
		df := OpenDataFile(GUILD_REGISTRY_DATAFILE);
	endif
	return df;
endfunction

// Writes guild's current GuildHouse/GuildStone serials into its datafile
// entry (keyed by its current name), creating the entry if needed.
function SyncGuildRegistry(guild)
	var name := guild.getprop(GUILD_NAME);
	if(!name)
		return;
	endif

	var df := OpenGuildRegistryDataFile();
	if(!df)
		return;
	endif

	var elem := df.FindElement(name);
	if(!elem)
		elem := df.CreateElement(name);
	endif
	if(!elem)
		UnloadDataFile(GUILD_REGISTRY_DATAFILE);
		return;
	endif

	elem.SetProp("GuildHouse", GuildPropInt(guild, "GuildHouse"));
	elem.SetProp(GUILD_STONE, GuildPropInt(guild, GUILD_STONE));

	UnloadDataFile(GUILD_REGISTRY_DATAFILE);
endfunction

// Call when a guild is renamed, before old_name is overwritten on the guild
// itself, so the datafile entry moves (carrying every stored property,
// including MemberRanks, across) instead of leaving an orphan behind and
// silently losing anything not re-derived by SyncGuildRegistry().
function MoveGuildRegistryEntry(old_name, guild)
	if(old_name && old_name != guild.getprop(GUILD_NAME))
		var df := OpenGuildRegistryDataFile();
		if(df)
			var old_elem := df.FindElement(old_name);
			if(old_elem)
				var new_name := guild.getprop(GUILD_NAME);
				var new_elem := df.FindElement(new_name);
				if(!new_elem)
					new_elem := df.CreateElement(new_name);
				endif
				if(new_elem)
					foreach prop_name in (old_elem.PropNames())
						new_elem.SetProp(prop_name, old_elem.GetProp(prop_name));
					endforeach
				endif
				df.DeleteElement(old_name);
			endif
			UnloadDataFile(GUILD_REGISTRY_DATAFILE);
		endif
	endif

	SyncGuildRegistry(guild);
endfunction

// Returns {member_serial -> rank string} for guild, read from the registry
// datafile -- the sole source of truth for ranks (no guild CProp mirror).
function GetGuildMemberRanks(guild)
	var name := guild.getprop(GUILD_NAME);
	if(!name)
		return dictionary;
	endif

	var df := OpenGuildRegistryDataFile();
	if(!df)
		return dictionary;
	endif

	var ranks := dictionary;
	var elem := df.FindElement(name);
	if(elem)
		var stored := elem.GetProp(GUILD_MEMBER_RANKS_PROP);
		if(stored && !stored.errortext)
			ranks := stored;
		endif
	endif

	UnloadDataFile(GUILD_REGISTRY_DATAFILE);
	return ranks;
endfunction

// The guild leader always reads back as GUILD_RANK_LEADER, regardless of
// whatever (if anything) happens to be stored for their serial.
function GetGuildMemberRank(guild, member)
	if(GuildPropInt(guild, "GuildMaster") == member.serial)
		return GUILD_RANK_LEADER;
	endif

	var ranks := GetGuildMemberRanks(guild);
	var stored := ranks[CInt(member.serial)];
	if(!stored || stored.errortext)
		return GUILD_DEFAULT_RANK;
	endif

	return stored;
endfunction

function SetGuildMemberRanks(guild, ranks)
	var name := guild.getprop(GUILD_NAME);
	if(!name)
		return;
	endif

	var df := OpenGuildRegistryDataFile();
	if(!df)
		return;
	endif

	var elem := df.FindElement(name);
	if(!elem)
		elem := df.CreateElement(name);
	endif
	if(!elem)
		UnloadDataFile(GUILD_REGISTRY_DATAFILE);
		return;
	endif

	elem.SetProp(GUILD_MEMBER_RANKS_PROP, ranks);

	UnloadDataFile(GUILD_REGISTRY_DATAFILE);
endfunction

// Call when a guild is disbanded, to drop its datafile entry entirely.
function DeleteGuildRegistryEntry(guild)
	var name := guild.getprop(GUILD_NAME);
	if(!name)
		return;
	endif

	var df := OpenGuildRegistryDataFile();
	if(!df)
		return;
	endif

	if(df.FindElement(name))
		df.DeleteElement(name);
	endif

	UnloadDataFile(GUILD_REGISTRY_DATAFILE);
endfunction

function GuildRealmNameFromMapId(mapid)
	if (mapid == 0)
		return "britannia";
	elseif (mapid == 1)
		return "britannia_alt";
	elseif (mapid == 2)
		return "ilshenar";
	elseif (mapid == 3)
		return "malas";
	elseif (mapid == 4)
		return "tokuno";
	elseif (mapid == 5)
		return "termur";
	endif

	return 0;
endfunction

// Static houses (pkg/multis/staticHousing) share POLCLASS_HOUSE with
// regular/custom housing, so they can only be told apart via their sign's
// exported IsStaticHouseSign() method. Sign resolution itself is
// GetHouseSignFromObject() from :house:utility (included above) -- it
// already does the same house-itself/signserial/nearby-scan resolution a
// guilds-local copy would, custom housing signs included, so there's no
// need to duplicate it here.
function IsStaticHouse(house)
	var sign := GetHouseSignFromObject(house);
	if(!sign)
		return 0;
	endif

	return sign.IsStaticHouseSign();
endfunction

// Tests whether any door is within radius tiles of loc (an object with
// x/y/z/realm members, e.g. a placed item or a TargetCoordinates() result).
function IsNearDoor(loc, radius)
	foreach item in ListItemsNearLocation(loc.x, loc.y, loc.z, radius, loc.realm)
		if (item.isa(POLCLASS_DOOR))
			return 1;
		endif
	endforeach

	return 0;
endfunction

// Tile area of house's footprint, via its itemdesc.cfg MultiID. 0 if it
// can't be determined.
function GetHouseFootprintArea(house)
	if (!house)
		return 0;
	endif

	var info := GetItemDescInfo(CInt(house.objtype));
	if (!info || info.errortext || !info.MultiID)
		return 0;
	endif

	var dims := GetMultiDimensions(CInt(info.MultiID));
	if (!dims)
		return 0;
	endif

	var width := (CInt(dims.xmax) - CInt(dims.xmin)) + 1;
	var height := (CInt(dims.ymax) - CInt(dims.ymin)) + 1;

	return width * height;
endfunction

function IsHouseTowerSizeOrBigger(house)
	return GetHouseFootprintArea(house) >= GUILDSTONE_SET_MIN_HOUSE_AREA;
endfunction

function IsGuildHouseOwner(who, house)
	if (!house)
		return 0;
	endif

	if (CInt(GetObjProperty(house, "ownerserial")) == who.serial)
		return 1;
	endif

	if (GetObjProperty(house, "owneracct") == who.acctname)
		return 1;
	endif

	return 0;
endfunction

function GuildSafeObjProperty(obj, propname, fallback := "")
	var value := GetObjProperty(obj, propname);
	if(value == error)
		return fallback;
	endif
	if(value && value.errortext)
		return fallback;
	endif
	if(!value)
		return fallback;
	endif
	return value;
endfunction

function GetGuildHouseDisplayName(house)
	if(!house)
		return "Unknown house";
	endif

	var hname := GuildSafeObjProperty(house, "MultiName", "");
	if(hname != "")
		return hname;
	endif

	var htype := GuildSafeObjProperty(house, "MultiType", "");
	if(htype != "")
		return htype;
	endif

	return "House " + Hex(CInt(house.objtype));
endfunction

function GetOwnedGuildCandidateHouses(who)
	var owned := array;
	var realmslist := Realms();

	foreach realm in realmslist
		var realm_name := GuildRealmNameFromMapId(CInt(realm.mapid));
		if (!realm_name)
			continue;
		endif

		var realm_width := CInt(realm.width) - 1;
		var realm_height := CInt(realm.height) - 1;

		var found := ListMultisInBox(0, 0, -128, realm_width, realm_height, 127, realm_name);
		if (!found || !found.size())
			continue;
		endif

		foreach house in found
			if (!house || !house.isa(POLCLASS_HOUSE))
				continue;
			endif

			// Cheap ownership check first -- IsStaticHouse() resolves the
			// house's sign (up to an 18-tile item scan) and there's no
			// reason to pay that cost for houses the player doesn't even own.
			if (!IsGuildHouseOwner(who, house))
				continue;
			endif

			if (IsStaticHouse(house))
				continue;
			endif

			owned.append(house);
		endforeach
	endforeach

	return owned;
endfunction

/////////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// FindSelectedHouse() - Find the selected house to make the guild house.
//
// 50 is for the house that the player is standing in.  50+1 for 1st selection, 50+2
// for second selected etc.
//
// returns a house ref or 0
/////////////////////////////////////////////////////////////////////////////////////
function FindSelectedHouse(who, data);
	var standing_house := GetCurrentHouseAtMobile(who);
	var houses := GetOwnedGuildCandidateHouses(who);

	// Player selected 'standing in house' option
	if (50 in data.keys)
		if (!standing_house or !IsGuildHouseOwner(who, standing_house))
			return 2;
		elseif (IsStaticHouse(standing_house))
			return 3;
		else
			return standing_house;
		endif
	endif

	var i := 51;

	// Loop from 51 to 65.
	repeat
		if (i in data.keys)
			var idx := i - 50;
			if (idx >= 1 && idx <= houses.size())
				return houses[idx];
			endif
			return 0;
		endif
		i := i + 1;
	until (i >= 66);

	return 0;

endfunction

///////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// DisbandGuild() - Deletes a guild, deals with sorting everything
//
// TODO: * All requests/recruits/pending alliances/enemies
//	 controlled on the sending (the one being deleted) guild ref, not players
//	
//
//	Dreams 
//
//	Removed requests/pending alliances/wars
///////////////////////////////////////////////////////////////////////////////////
// DestroyGuildStone() - Destroys the guild's placed guildstone (and any
// statue-set companion pieces) and clears the GUILD_STONE property, if the
// guild has one. Used both on disband and whenever the guild loses/changes
// its house, since the stone can only ever sit inside the guild's own house.
function DestroyGuildStone(guild)

	var guildstone_serial := GuildPropInt(guild, GUILD_STONE);
	if(guildstone_serial)
		var guildstone := SystemFindObjectBySerial(guildstone_serial);
		if(guildstone)
			DestroyGuildstoneCompanions(guildstone);
			DestroyItem(guildstone);
		endif
	endif
	guild.eraseprop(GUILD_STONE);

endfunction

function DisbandGuild(guild)

	// remove colour from guild house
	var guildhouse_serial := guild.getprop("GuildHouse");
	var guildhouse := 0;
	if(guildhouse_serial)
		guildhouse := SystemFindObjectBySerial(CInt(guildhouse_serial));
	endif
	//SetGuildHouseColour(guildhouse, 0);

	// remove guild reference from the linked house if it still exists
	if(guildhouse)
		var linked_guild_id := GetObjProperty(guildhouse, "GuildHouse");
		if(CInt(linked_guild_id) == CInt(guild.guildid))
			EraseObjProperty(guildhouse, "GuildHouse");
		endif
	endif

	// also clear the guild-side house reference before destruction
	guild.eraseprop("GuildHouse");

	DestroyGuildStone(guild);

	DeleteGuildRegistryEntry(guild);

	// remove all players from guild. Remove players titles
	foreach person in (guild.members)
		SendSysMessage(person, "You are no longer in a guild.", 3, 67);
		RemoveMember(person);
	endforeach

	// remove applicants requests
		guild.eraseprop("JoinGuildRequest");
		guild.eraseprop("BanReq");

	// remove ally/enemy guilds
	foreach other in ListGuilds()
		ClearAllRequests(guild, other);
		guild.removeallyguild(other);
		guild.removeenemyguild(other);
		// This most likely isn't needed, just in case.
		other.removeallyguild(guild);
		other.removeenemyguild(guild);
	endforeach

	foreach person in EnumerateOnlineCharacters()
		SendSysMessage(person, guild.getprop("GuildName")+" has been disbanded!", 3, 43);
	endforeach

	DestroyGuild(guild);

endfunction

///////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// RemoveMember() - Removes specified member from guild
//
///////////////////////////////////////////////////////////////////////////////////
function RemoveMember(who)

	who.guild.removemember(who);
	who.title_guild := "";

	// unequip uniforms
	foreach item in ListEquippedItems( who )
		if (GetObjProperty(item, GUILD_UNIFORM) || GetObjProperty(item, "Entwined") )	
			MoveItemToContainer( item, who.backpack);
		endif

		if (GetObjProperty(item, "Entwined"))
			UnEntwine(who, item);
		endif

	endforeach

endfunction


///////////////////////////////////////////////////////////////////////////////////
// Toadstool
//
// RemoveGuildMaster() - Removes the guildmaster.  If he's online, asks him to choose
// a successor
//
///////////////////////////////////////////////////////////////////////////////////
function ChangeGuildMaster(who)

	var guild := who.guild;

	if (!who.connected)
		guild.setprop("GuildMaster", guild.members[1].serial);

		foreach member in (guild.members)
			if (member.serial != who.serial)
				SendSysMessage(member, member.name+" is now the Guild Master!", 3, 53);
			else
				SendSysMessage(member, "You are now the Guild Master!", 3, 53);
			endif
		endforeach
		return 1;
	else
		var gump := GFCreateGump(20, 20, 400, 350);
		GFResizePic(gump, 20, 20, 9260, 400, 350);
		GFResizePic(gump, 30, 30, 9350, 380, 290);
		GFResizePic(gump, 30, 321, 9350, 380, 39);

		GFResizePic(gump, 118, 35, 9200, 200, 25);
		GFTextLine(gump, 135, 37, 53, "Choose the new Guild Leader");

		GFTextLine(gump, 140, 330, 0, "Hand over guild leadership?");
		GFAddBUtton(gump, 50, 330, 2128, 2129, 1, 1000);
		GFAddBUtton(gump, 335, 330, 2119, 2120, 1, 0);

		GFTextline(gump, 100, 80, 0, "Choose a member at random");
		GFRadiobutton(gump, 60, 80, 210, 211, 1, 999); 

		var y := 120;
		var maxpage := 7;
		var count := 0;
		var page := 2;
		GFPage(gump, 1);
		GFTextLine(gump, 200, 295, 53, "Page 1");
		foreach person in (guild.members)
			if (count >= maxpage)
				GFAddButton(gump, 260, 296, 5601, 5605, 0, page);
				GFPage(gump, page);
				page := page + 1;
				GFTextLine(gump, 200, 295, 53, "Page "+(page-1));
				GFAddButton(gump, 163, 296, 5603, 5607, 0, page-2);
				y := 120;
			endif

			if (person != who)
				GFTextline(gump, 100, y, 0, person.name);
				GFRadiobutton(gump, 60, y, 210, 211, 0, _person_iter); 
				y := y + 25;
				count := count + 1;
			endif
		endforeach

		var data := GFSendGump(who, gump);

		if (data[0] == 1000)

			var newmaster;
			// selected a member
			if (data.keys[2] != 999)
				newmaster := guild.members[data.keys[2]];

			// selected random member
			elseif (data.keys[2] > 1)
				repeat
				newmaster := guild.members[RandomInt(guild.members.size())+1];
				until (newmaster.serial != who.serial);			
			endif

			guild.setprop("GuildMaster", newmaster.serial);
			SendSysMessage(who, newmaster.name+" is now the Guild Master.", 3, 43);

			foreach member in (guild.members)
				if (member.serial != who.serial && member.serial != newmaster.serial )
					SendSysMessage(member, newmaster.name+" is now the Guild Master!", 3, 53);
				endif
				if (member.serial == newmaster.serial)	
					SendSysMessage(member, "You are now the Guild Master!", 3, 53);
				endif
			endforeach
			return 1;
		endif
	endif

	return 0; 

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// NameUniform() - Names the uniform on creation
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function NameUniform(item)

	var name := SplitWords(item.desc);

	var newname := "";
	//not equipped
	if (!item.layer)
		foreach part in name
			if (_part_iter == 1 && lower(part) == "a")
				newname := "a Guild";
			elseif (_part_iter == 1 && lower(part) != "a")
				newname :=  "Guild "+part;
			else
				newname := newname+" "+part;
			endif
		endforeach
	SetName(item, newname);
	endif

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// EquipGuildUniform() - Set's the uniform colour on equip
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function EquipGuildUniform(who, item)

	if(GetObjProperty(item, "EquippedAlready"))
		return 1;
	endif

	var guild := who.guild;
	var colour := guild.getprop(GUILD_COLOUR);

	if (!guild)
		SendSysMessage(who, "You need a guild to wear a uniform!");
		return 0;
	elseif (!colour)
		SendSysMessage(who, "Your guild needs a guild colour before you can wear that!");
		return 0;
	endif

	PlaySoundEffectPrivate(who, 0x457, who);
	PlayObjectCenteredEffectEx( who, 14186, 10, 10,colour, 0, 7);
	item.color := colour;

	return 1;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnEquipGuildUniform() - Set's the uniform colour on unequip
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function UnEquipGuildUniform(who, item)

	if (GetObjProperty(item, "Entwined"))
		item.color := GetObjProperty(item, "OriginalColour");
		if (who.dead)
			UnEntwine(who, item);
		endif
		return 1;
	endif

	if (!GetObjProperty(item, GUILD_UNIFORM))
		return 1;
	endif

	item.color := 0;

	return 1;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// UnEntwine() - does just that
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function UnEntwine(who, uniform)

	var name := SplitWords(uniform.desc);
	var newname, count := 0;

	foreach thing in name
		if(count == 0)
			newname := thing;
			count := 1;
		elseif ( thing != "[Entwined]" )
			newname := newname+" "+thing;
		endif
	endforeach

	SetName(uniform, newname);
	uniform.color := GetObjProperty(uniform, "OriginalColour");

	var uni := SystemFindObjectBySerial(GetObjProperty(uniform, "Entwined"));

	if (!uni)
		Broadcast("This is odd, your uniform is gone!, page staff!",3, 33);
		syslog(who.name+"/"+who.serial+" This is odd, your uniform is gone!, page staff!");
		return 0;
	endif

	EraseObjProperty(uniform, "Entwined");
	EraseObjProperty(uniform, "OriginalColour");
	uniform.usescript := "";

	uni.movable := 1;
	uni.invisible := 0;
	uni.graphic := GetObjProperty(uni, "Graphic");
	MoveItemToContainer( uni, who.backpack );
	endfunction

	function UpdateUniformColour(guild)

	// unequip uniforms
	foreach who in (guild.members)
		foreach item in ListEquippedItems( who )
			if (GetObjProperty(item, GUILD_UNIFORM) || GetObjProperty(item, "Entwined") )	
				item.color := guild.getprop(GUILD_COLOUR);
			endif
		endforeach
	endforeach

endfunction

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dreams
//
// Guild Relation Stuff
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

function RequestStatus(who, thatguild, relation)

	var relationlist := thatguild.getprop(relation);
	var glist := who.guild.getprop(relation);

	if(!relationlist)
		relationlist := {};
	endif
	
	if(glist)
		foreach guild in glist
			if(guild == thatguild.getprop("GuildName"))
				SendSysMessage(who, "You already got a pending request from that guild.");
				return;
			endif
		endforeach
	endif
	relationlist.append(who.guild.getprop("GuildName"));
	thatguild.setprop(relation, relationlist);

endfunction

function CancelRequest(whoguild, thatguild, relation)

	var relationlist := thatguild.getprop(relation);
	var newlist := {};

	foreach guilds in relationlist
		if(guilds != whoguild.getprop("GuildName"))
			newlist.append(guilds);
		endif
	endforeach

	thatguild.setprop(relation, newlist);

endfunction

function ClearAllRequests(who, thatguild)

	CancelRequest(who.guild, thatguild, "RequestWar");
	CancelRequest(who.guild, thatguild, "RequestAlly");
	CancelRequest(who.guild, thatguild, "RequestPeace");
	CancelRequest(who.guild, thatguild, "RequestBreakup");

	CancelRequest(thatguild, who.guild, "RequestWar");
	CancelRequest(thatguild, who.guild, "RequestAlly");
	CancelRequest(thatguild, who.guild, "RequestPeace");
	CancelRequest(thatguild, who.guild, "RequestBreakup");

endfunction

function CheckStatus(whoguild, thatguild, relation)

	var CheckArray := thatguild.getprop(relation);

	if(CheckArray)
		foreach guilds in CheckArray
	    	if(guilds == whoguild.getprop("GuildName"))
				return 1;
			endif
		endforeach
	endif

	return 0;

endfunction

function WarAllyGuilds(who, ally := 0)

	var newlist := {};

	foreach member in ListGuilds()
		if(ally == 1)
			if(member.IsAllyGuild(who.guild))
				newlist.append(member);
			endif
		else
			if(member.IsEnemyGuild(who.guild))
				newlist.append(member);
			endif
		endif
	endforeach

	return newlist;

endfunction

function RelationStatusGuilds(whoguild, relation)

	var relationlist := whoguild.getprop(relation);
	var newlist := {};

	if(!relationlist)
		return 0;
	endif

	foreach member in relationlist
			newlist.append(member);
	endforeach

	return newlist;

endfunction


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dreams
//
// Join Requests / Acceptance of Requests
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
function ClearJoinGuildRequest(who)

	foreach guild in ListGuilds()
		if(guild.getprop("GuildName") == who.getprop("JoinGuildRequest"))
			var relationlist := guild.getprop("JoinGuildRequest");
			if(relationlist.size() <= 1)
				guild.eraseprop("JoinGuildRequest");
				who.eraseprop("JoinGuildRequest");
				return 0;
			endif

			var newlist := {};
			foreach member in relationlist
				if(member != who.serial)
					newlist.append(member);
				endif
			endforeach
			guild.setprop("JoinGuildRequest", newlist);
			who.eraseprop("JoinGuildRequest");
		endif
	endforeach

endfunction

function RequestJoinGuild(who, guild)

	var relationlist := guild.getprop("JoinGuildRequest");

	who.setprop("JoinGuildRequest", guild.getprop("GuildName"));
	
	if(!relationlist)
		relationlist := {};
	endif

	relationlist.append(who.serial);
	guild.setprop("JoinGuildRequest", relationlist);

endfunction

function BanReq(who, guild)

	var banlist := guild.getprop("BanReq");
	
	if(!banlist)
		banlist := {};
	endif

	banlist.append(who.serial);
	guild.setprop("BanReq", banlist);

endfunction