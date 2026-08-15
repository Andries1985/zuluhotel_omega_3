//include "include/wodinc";
//include ":customhousing:housefriends";
use uo;
use os;
use guilds;

include "housefriends";
include "include/objtype";
include ":house:multihouse_settings";
include "include/omegacache_utils";
include ":guilds:include/guildconstants";
include ":guilds:items/guildstone/companions";

Const HOUSE_LOCKDOWN_ITEM	:= 1;
Const HOUSE_RELEASE_ITEM		:= 2;
Const HOUSE_SECURE_CONTAINER	:= 3;
Const HOUSE_RELEASE_CONTAINER	:= 4;
Const HOUSE_RAISE_ITEM		:= 5;
Const HOUSE_LOWER_ITEM		:= 6;
Const HOUSE_DISPLAY_ITEM		:= 7;

// 2026-08-10: the old per-house-value account cap (HOUSE_VALUE_LIMIT=6,
// GetAccountHouseValue/HouseValueUnderLimit) was replaced by the unified,
// plain house-count cap shared across house/staticHousing/customhousing
// (:house:multihouse_settings -- GetAccountHouseCount/IsAccountUnderHouseCap/
// RegisterAccountHouseSign/UnregisterAccountHouseSign, MaxHousesPerAccount
// setting). It reuses the exact same "#housing_of_<acctname>" tracking
// property this package already wrote, just counting entries instead of
// summing per-house "housevalue" scores, so existing registrations carried
// over without a data migration.
//
// RecordHouseSignOwner (formerly here) is superseded by
// RegisterAccountHouseSign(); customeHouseDeed.src registers new houses
// synchronously at purchase time (no race with the sign's ControlScript),
// and signcontrol.src's SignListener registers pre-existing signs the first
// time they load under the new system.

// Returns the house serial linked to a custom house sign. Reads "houseserial"
// -- the same property name :house: housing uses for its own signs, and the
// one FindOmegaCacheHouseSign() (omegacache.inc) and :house:utility's
// GetHouseSignFromObject() already check -- falling back to the legacy
// "house_serial" property (customhousing's original, differently-named
// convention) for signs placed before this package started writing
// "houseserial" too. The fallback self-heals: the first time it's hit for a
// given sign, it backfills "houseserial" so every later read goes through
// the fast path and legacy "house_serial" reads/writes can eventually be
// dropped entirely once no un-migrated signs remain.
function ResolveCustomHouseSerial (sign)
	if (!sign)
		return 0;
	endif

	var hserial := GetObjProperty (sign, "houseserial");
	if (hserial)
		return CInt (hserial);
	endif

	var legacy := GetObjProperty (sign, "house_serial");
	if (legacy)
		SetObjProperty (sign, "houseserial", CInt (legacy));
		return CInt (legacy);
	endif

	return 0;
endfunction

///////////////////
//  2026-08-10: flat registry of every custom house sign, independent of the
//  per-account cap registry above (#housing_of_<acct>). Enumerating "every
//  account" to sweep for decayed houses isn't cheap/reliable, so this keeps
//  one simple global list instead. Used by decaywatcher.src.
///////////////////

function RegisterCustomHouseSign (sign)
	if (!sign)
		return;
	endif
	var existing := GetGlobalProperty ("#customhouse_all_signs");
	if (!existing)
		existing := array {};
	endif
	if (! (sign.serial in existing))
		existing.append (sign.serial);
		SetGlobalProperty ("#customhouse_all_signs", existing);
	endif
endfunction

function UnregisterCustomHouseSign (sign)
	if (!sign)
		return;
	endif
	var existing := GetGlobalProperty ("#customhouse_all_signs");
	if (!existing)
		return;
	endif
	var idx := sign.serial in existing;
	if (idx)
		existing.erase (idx);
		SetGlobalProperty ("#customhouse_all_signs", existing);
	endif
endfunction




///////////////////
//  2026-08-10: decay-timeout teardown for an abandoned custom house, called
//  by decaywatcher.src. Modeled directly on house's own timeout-demolish
//  (pkg/multis/house/multiSign/control.src, function demolish) -- terminal/
//  abandoned, so no deed refund, and Omega Cache containers are force-
//  cleared regardless of contents (unlike the player-triggered manual
//  redeed in sign.src, which blocks on non-empty cache instead).
///////////////////

function DemolishAbandonedCustomHouse (sign)
	if (!sign)
		return;
	endif
	var house := SystemFindObjectBySerial (ResolveCustomHouseSerial (sign));
	if (!house)
		UnregisterCustomHouseSign (sign);
		DestroyItem (sign);
		return;
	endif

	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (homeinfo)
		foreach thing in ListObjectsInBox (homeinfo[2], homeinfo[3], -128, homeinfo[4], homeinfo[5], 127, sign.realm)
			if (thing.objtype == OMEGACACHE_OBJTYPE)
				EraseObjProperty (thing, "houseserial");
				DestroyItem (thing);
			elseif (thing.objtype == GUILDSTONE_OBJTYPE)
				// The guildstone (and any statue-set companion pieces) can
				// only ever sit inside the guild's own house, so it can't
				// survive the house decaying away -- this path previously
				// didn't sweep for one at all, leaving it orphaned in place
				// once DestroyMulti ran below.
				DestroyGuildstoneCompanions (thing);
				foreach guild in ListGuilds ()
					if (guild.getprop (GUILD_STONE) == thing.serial)
						guild.eraseprop (GUILD_STONE);
						break;
					endif
				endforeach
				DestroyItem (thing);
			endif
		endforeach
	endif

	var demolished_acct := GetObjProperty (sign, "owneracct");
	if (!demolished_acct)
		demolished_acct := GetObjProperty (sign, "account");
	endif
	if (demolished_acct)
		UnregisterAccountHouseSign (sign, demolished_acct);
	endif
	UnregisterCustomHouseSign (sign);

	var owner := SystemFindObjectBySerial (CInt (GetObjProperty (sign, "ownerserial")), SYSFIND_SEARCH_OFFLINE_MOBILES);
	if (owner)
		ReleaseAll (owner, sign);
	endif

	var addons := GetObjProperty (house, "components");
	if (addons.size())
		foreach item in addons
			DestroyItem (SystemFindObjectBySerial (item));
		endforeach
	endif

	var house_serial := house.serial;
	DestroyMulti (house);

	// Cache containers above were force-cleared regardless of contents, so
	// nothing further can be recovered from this DataFile either way --
	// drop it instead of leaving an orphaned entry for a house that no
	// longer exists.
	DeleteOmegaCacheStore (house_serial);

	Print ("Custom house demolished due to decay: sign=" + sign.serial + " house=" + house_serial);
endfunction




///////////////////
//  2026-08-10: customhousing originally tracked ownership only as an
//  account-name string ("account"), with its own dictionary-based friend
//  system (housefriends). The shared :house: sign core (co-owners, bans,
//  per-friend permissions, IsOwner/IsFullHouseManager, the account house
//  cap's self-healing count) is serial-based ("ownerserial"/"owneracct"),
//  matching house/staticHousing.
//
//  Existing custom houses only have "account" set, with no character serial
//  recorded anywhere to promote from -- an account can have multiple
//  characters, so there's no safe way to guess which one is "the owner" from
//  data alone. Instead, this runs on every sign interaction: once the
//  character whose account matches the legacy "account" property actually
//  opens their house sign, THEY are recorded as the serial-based owner from
//  then on. Houses whose owner never logs back in stay on the legacy
//  "account" check (still honored everywhere below) until they do.
///////////////////

function MigrateCustomHouseSignOwnership (sign, character)
	if (GetObjProperty (sign, "ownerserial"))
		return;
	endif

	var legacy_account := Lower (GetObjProperty (sign, "account"));
	if (!legacy_account or Lower (character.acctname) != legacy_account)
		return;
	endif

	SetObjProperty (sign, "ownerserial", character.serial);
	SetObjProperty (sign, "owneracct", character.acctname);
	RegisterAccountHouseSign (sign, character.acctname);
endfunction




///////////////////
//  gets a default house price, based on its size
///////////////////

function GetDefaultHousePrice (housevalue)
	case (housevalue)
		2:
			return 90000;
		3:
			return 150000;
		4:
			return 400000;
		5:
			return 800000;
		default:
			return 40000;
	endcase
endfunction




///////////////////
//  determines the housevalue of a given deed
///////////////////

function GetHouseValueByDeed (deed)
	case (deed)
		0x16023: //tower
		0x16230: //customthreestoryhousedeed
			return 3;
		0x16024: //keep
		0x16025: //castle
			return 5;
		0x16026: //largepatiohousedeed
		0x16020: //largebrickhouse
		0x16021: //twostorywood
		0x16022: //twostorystone
		0x16229: //smalltowerdeed
		0x1622A: //sandstonepatiohousedeed
		0x1622B: //twostorylogcabindeed
		0x1622C: //twostoryvilladeed
		0x1622D: //largemarblepatiohousedeed
		0x1622E: //ranch house
		0x16231: //customtwostoryhousedeed
			return 2;
		default:
			return 1;
	endcase
endfunction




///////////////////
//  determines the housevalue of the given sign, whether its one of a static house or deedplaced
///////////////////

function GetHouseValueOfSign (sign)
	//static houses:
	// if (sign.objtype == 0x17060)
	// 	if (GetObjProperty (sign, "housevalue"))
	// 		return CINT (GetObjProperty (sign, "housevalue"));
	// 	endif
	// 	return 1;
	// endif

	// if (GetObjProperty (sign, "housevalue"))
	// 	return CINT (GetObjProperty (sign, "housevalue"));
	// endif
	//deed-placed houses:
	var house := SystemFindObjectBySerial (ResolveCustomHouseSerial (sign));
	var deed := GetDeedType (house);
	return GetHouseValueByDeed (deed);
endfunction




///////////////////
//  determines the deed objtype that corresponds to a given house type
///////////////////

function GetDeedType (house)
	var deed := 0;
	case (house.objtype)
		69732:	deed := 0x16019;
		69734:	deed := 0x1601A;
		69738:	deed := 0x1601B;
		69740:	deed := 0x1601C;
		69742:	deed := 0x1601D;
		69748:	deed := 0x16020;
		69750:	deed := 0x16021;
		69752:	deed := 0x16022;
		69754:	deed := 0x16023;
		69756:	deed := 0x16024;
		69758:	deed := 0x16025;
		69772:	deed := 0x16026;
		69792:	deed := 0x16227;
		69794:	deed := 0x16228;
		69784:	deed := 0x16229;
		69788:	deed := 0x1622A;
		69786:	deed := 0x1622B;
		69790:	deed := 0x1622C;
		69782:	deed := 0x1622D;
		69773:	deed := 0x1622E;
		69736:	deed := 0x1622F;
	endcase
	if (house.objtype >= 0x113ec and house.objtype <= 0x1140a)
		deed := 0x16231;
	elseif (house.objtype >= 0x11410 and house.objtype <= 0x11416)
		deed := 0x16231;
	elseif (house.objtype >= 0x1141c and house.objtype <= 0x11422)
		deed := 0x16231;
	elseif (house.objtype >= 0x11428 and house.objtype <= 0x1142e)
		deed := 0x16231;
	elseif (house.objtype >= 0x11435 and house.objtype <= 0x1143a)
		deed := 0x16231;
	elseif (house.objtype >= 0x1140b and house.objtype <= 0x1147d)
		deed := 0x16230;
	endif
	return deed;

endfunction




///////////////////
//  Determines the maximum number of lockdowns permitted in a house
///////////////////

function GetMaxLockdowns (sign)
	var hvalue := GetHouseValueOfSign (sign);
	return (hvalue * 750);
endfunction




///////////////////
//  returns the number of items locked down in the house
///////////////////

function GetLockdowns (sign)
	var itemlist := dictionary;
	foreach item in ListObjectsInBox (sign.x-32, sign.y-32, -128, sign.x+32, sign.y+32, +127, sign.realm)
		if (!item.movable)
			if ( GetObjProperty( item, "lockeddown" ) == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
			if ( GetObjProperty( item, "gmlockeddown" ) == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
		endif
	endforeach

	Setobjproperty (sign, "lockdowns", itemlist.size() );
	return itemlist.size();
endfunction




///////////////////
//  determines the maximum allowed number of secures for the house
///////////////////

function GetMaxSecures( sign )
	var hvalue := GetHouseValueOfSign (sign);
	return (hvalue * 10);
endfunction
  

///////////////////
//  counts the number of secured items in the house
///////////////////

function GetSecures (sign)
	var itemlist := dictionary;
	foreach item in ListObjectsInBox (sign.x-32, sign.y-32, -128, sign.x+32, sign.y+32, +127, sign.realm)
		if (!item.movable)
			if ( GetObjProperty (item, "secured") == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
		endif
	endforeach

	Setobjproperty (sign, "secures", itemlist.size() );
	return itemlist.size();
endfunction




// 2026-08-10: the old "lastvisit"-based RefreshHouse(sign)/LongTermRefresh()
// here tracked decay separately from the actual "decay" cprop the sign's
// ControlScript checks (signcontrol.src) -- customhousing's decay/abandon
// system was fully disabled at the time (its check loop was commented out),
// so this mismatch didn't matter. Decay is real again now, wired through
// the shared :house: sign core (sign.RefreshHouse(), used above and by
// handle_speech()), so both call sites were repointed there instead of this
// now-dead pair.

///////////////////
//  refreshes the house for a longer period of time
///////////////////

function LongTermRefresh (sign, owner)
	sign.RefreshHouse();
	SendSysMessage (owner, "Your house has been refreshed.");
endfunction




///////////////////
//  releases all locked down and secured items in the house.  Used when the house is destroyed
///////////////////

// 2026-08-13: was routing each item through run_script_to_completion(
// ":housing:lockunlock", parms) -- there is no ":housing:" package anywhere
// in this repo (only house/customhousing/statichousing), so that call
// always failed to resolve and no item was ever actually released before
// the caller destroys the house/multi right after this returns. Rewritten
// to release items inline instead, mirroring how :house:'s own demolish()
// (multiSign/use.src) does it: just clear the marking CProp and restore
// movable/usescript directly, no external script involved.
function ReleaseAll (owner, sign)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	foreach item in ListObjectsInBox (homeinfo[2], homeinfo[3], -128, homeinfo[4], homeinfo[5], +127, sign.realm)
		if (!item.movable and item.isa (POLCLASS_ITEM))
			if (!IsGMLockedDownItem (owner, item))
				if (GetObjProperty (item, "displaycontainer") == sign.serial)
					item.movable := 1;
					item.decayat := ReadGameClock() + 1;
					EraseObjProperty (item, "displaycontainer");
					EraseObjProperty (item, "lockeddown");
				elseif (GetObjProperty (item, "secured") == sign.serial)
					item.movable := 1;
					item.decayat := ReadGameClock() + 1;
					var oldscript := GetObjProperty (item, "oldscript");
					if (oldscript == error)
						oldscript := "";
					endif
					item.usescript := oldscript;
					EraseObjProperty (item, "secured");
					EraseObjProperty (item, "oldscript");
				elseif (GetObjProperty (item, "lockeddown") == sign.serial)
					item.movable := 1;
					item.decayat := ReadGameClock() + 1;
					EraseObjProperty (item, "lockeddown");
				endif
			endif
		endif
	endforeach

	EraseObjProperty (sign, "lockdowns");
	EraseObjProperty (sign, "secures" );
endfunction




///////////////////
//  releases a locked down or secured item
///////////////////

function ReleaseLockedDownItem (owner, item)
	if (GetObjProperty (item, "olditem"))
		if (!ReserveItem (item))
			return;
		endif
		var oldtype := GetObjProperty (item, "olditem");
		var olditem := CreateItemAtLocation (item.x, item.y, item.z, oldtype, 1, item.realm);
		olditem.name := item.name;
		olditem.color := item.color;
		olditem.graphic := item.graphic;
		olditem.movable := 1;
		if (getobjproperty (item, "lockid"))
			setobjproperty(olditem, "lockid", getobjproperty (item, "lockid") );
		endif
		PrinttextAbovePrivate (olditem, "Released!", owner);
		DestroyItem (item);
	else
		item.movable := 1;
		PrinttextAbovePrivate (item, "Released!", owner );
		ReleaseItem (item);
		if (item.decayat)
			item.decayat := ReadGameClock() + 600;
		endif
	endif
endfunction




///////////////////
//  allows the owner to pick a new owner for the house
///////////////////

function ChangeOwner (who, sign)
	SendSysMessage (who, "Transfer ownership to whom?");
	var newowner := Target (who);
	if (!newowner or !newowner.acctname)
		SendSysMessage (who, "Canceled.");
		return;
	endif

	if ((newowner.cmdlevel == 0) and !IsAccountUnderHouseCap (newowner.acctname))
		SendSysMessage (who, "That person already owns the maximum number of houses allowed on their account!");
		return;
	endif

	if (who and who.acctname)
		UnregisterAccountHouseSign (sign, who.acctname);
	endif
	EraseObjProperty (sign, "housefriends");
	SetObjProperty (sign, "account", newowner.acctname);
	SetObjProperty (sign, "owneracct", newowner.acctname);
	SetObjProperty (sign, "ownerserial", newowner.serial);
	RegisterAccountHouseSign (sign, newowner.acctname);

	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (homeinfo)
		homeinfo[1] := newowner.serial;
		SetObjProperty (sign, "homeinfo", homeinfo);
	endif

	// 2026-08-13: previously only set on the multi when homeinfo was absent
	// (the else-branch above) -- when homeinfo was present (the normal
	// case), the house multi's own ownerserial/owneracct were left pointing
	// at the old owner. guilds.inc's IsGuildHouseOwner()/
	// GetOwnedGuildCandidateHouses() read those properties directly off the
	// multi (not resolved via the sign), so a house transferred through
	// here couldn't be selected as a guild house by its new owner. Always
	// mirror both onto the multi now, regardless of homeinfo.
	var house := SystemFindObjectBySerial (ResolveCustomHouseSerial (sign));
	if (house)
		SetObjProperty (house, "ownerserial", newowner.serial);
		SetObjProperty (house, "owneracct", newowner.acctname);
	endif

	SetObjProperty (sign, "lastownername", newowner.name);

	SendSysMessage (who, "You are no longer the owner of this house!");
	SendSysMessage (newowner, "You are now the owner of this house!");
	SendSysMessage (newowner, "You should change the locks soon.");
endfunction




///////////////////
//  Finds any GM lockeddown items in the house and refunds 1/2 the purchase price to the houseowner
///////////////////

function IsGMLockedDownItem (owner, item)
	if (!GetObjProperty (item, "gmlockeddown"))
		return 0;
	endif
	
	var price := GetObjProperty (item, "price");
	if (!price)
		DestroyItem (item);
		return 1;
	endif
	
	price := CINT (price/2);

	CreateItemInContainer (owner.backpack, UOBJ_GOLD_COIN, price);
	SendSysMessage (owner, "You recieve " + price + " gold back for " + item.desc);
	DestroyItem (item);
	return 1;
endfunction




///////////////////
//  handles speech events that the sign hears
///////////////////

function handle_speech (event, sign)
	sign.RefreshHouse();

	var text := lower( event.text );
	if (text["i wish to lock this down"] or text["i wish to release this"])
		set_critical (1);
		LockdownItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to secure this"] or text["i wish to unsecure this"])
		set_critical (1);
		SecureItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to display this"] or text["i wish to remove this from display"])
		set_critical (1);
		DisplayContainer (sign, event.source);
		set_critical (0);
	elseif (text["i wish to raise this"])
		set_critical (1);
		RaiseItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to lower this"])
		set_critical (1);
		LowerItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to refresh this house"])
		LongTermRefresh (sign, event.source);
	endif

endfunction




///////////////////
//  allows the character to lock down items
///////////////////

function LockdownItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_LOCKDOWN_ITEM;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
// allows the player to secure containers
///////////////////

function SecureItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_SECURE_CONTAINER;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
//  allows the house owner to raise the z position of items in the house
///////////////////

function RaiseItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_RAISE_ITEM;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
//  allows the house owner to lower the z position of items in the house
///////////////////

function LowerItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_LOWER_ITEM;
	start_script (":housing:lockunlock", parms);

endfunction




///////////////////
//  Puts a container on display
///////////////////

function DisplayContainer (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_DISPLAY_ITEM;
	start_script (":housing:lockunlock", parms);

endfunction



function IsInside (owner, sign, item)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if ( (item.realm == sign.realm) and
	     (item.x >= homeinfo[2] and item.x <= homeinfo[4]) and
	     (item.y >= homeinfo[3] and item.y <= homeinfo[5]) )
		return 1;
	else
		SendSysmessage (owner, "That is not inside your house." );
		return 0;
	endif
endfunction




///////////////////
//  tries to find a house sign near the character that belongs to that character
///////////////////

function FindHouseSign (character)
	foreach item in ListObjectsInBox (character.x-32, character.y-32, -128, character.x+32, character.y+32, +127, character.realm)
		case (item.objtype)
			0x17060:
			0x0bd2:
			0x0bd0:
				if (item.IsAFriend (character) or character.cmdlevel)
					if (IsInsideTheHouse (character, item) )
						return item;
					endif
				endif
		endcase
	endforeach
	return 0;
endfunction




///////////////////
//  determines if the character is within the area the house covers
///////////////////

function IsInsideTheHouse (character, sign)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return;
	endif

	if ( (character.realm == sign.realm) and
		 (character.x >= homeinfo[2] and character.x <= homeinfo[4]) and
	     (character.y >= homeinfo[3] and character.y <= homeinfo[5]) )
		return 1;
	else
		return 0;
	endif
endfunction




///////////////////
//  tries to find a house sign that covers the given location
///////////////////

function FindHouseSignAtLocation (x, y, realm)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, realm)
		case (item.objtype)
			0x17060:
			0x0bd2:
			0x0bd0:
				if (IsLocationWithinHouseSign (item, x, y, realm))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction




///////////////////
//  determines if the given sign covers the given X-Y location
///////////////////

function IsLocationWithinHouseSign (sign, x, y, rlm)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return 0;
	endif

	if ( (x >= homeinfo[2] and x <= homeinfo[4]) and
	     (y >= homeinfo[3] and y <= homeinfo[5]) and
	     sign.realm == rlm )
		return 1;
	else
		return 0;
	endif
endfunction
