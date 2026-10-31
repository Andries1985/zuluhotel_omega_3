include "include/client";
// 2026-10-29: :Events:bag only served the dead reward code; reward bags need random + the bank storage area (ZH2.5 1.1.5 port)
include "include/random";
include ":containers:storageAreas";
include "include/constants/locations";	// 2026-10-29: the bag is built at DEFAULT_LOCATION_ITEM_CREATION

function RainDownMoney(corpse)
  var tile, blocked;
  var i;
  // 2026-09-23: 137-685 a pile over 441 tiles keeps the total near the old 121-tile
  // 500-2499 (about 180,000 gold a champion); see the GetMapInfo fix below.
  for (i := -10; i <= 10 ; i := i + 1)
    var j;
    for (j := -10; j <= 10; j := j + 1)
      tile := GetMapInfo(corpse.x+i, corpse.y+j, corpse.realm); // 2026-09-23: the offsets were passed as map coordinates, so only the 121 tiles with both offsets >= 0 got gold
      if(tile)
		blocked := 0;
		var altar := ListItemsAtLocation( corpse.x+i, corpse.y+j, corpse.z, corpse.realm );
		foreach item in altar
			if(item.color == 2732)
				blocked := 1;
			endif
		endforeach
		if(!blocked)
			if(CheckLosBetween( corpse.x+i, corpse.y+j, corpse.z, corpse.x, corpse.y, corpse.z+5, corpse.realm) && CheckLosAt( corpse, corpse.x+i, corpse.y+j, corpse.z+5 ) )
				CreateItemAtLocation( corpse.x+i, corpse.y+j, corpse.z, "goldcoin", RandomInt(549)+137, corpse.realm);
				PlayStationaryEffect( corpse.x+i, corpse.y+j, corpse.z, 0x36b0, 7, 0, 1, corpse.realm);
			else
				CreateItemAtLocation( corpse.x, corpse.y, corpse.z, "goldcoin", RandomInt(549)+137, corpse.realm);
			endif
		else
			CreateItemAtLocation( corpse.x+i, corpse.y+j, corpse.z+6, "goldcoin", RandomInt(549)+137, corpse.realm);
			PlayStationaryEffect( corpse.x+i, corpse.y+j, corpse.z+6, 0x36b0, 7, 0, 1, corpse.realm);
		endif
        sleepms(50);
      endif
    endfor
  endfor
endfunction

// 2026-10-29: Champion reward bags (ZH2.5 1.1.5 port). When an altar's champion dies, every character with at
// least CHAMP_BAG_MIN_KILLS kills on the altar's tally (death.inc) gets a bag, provided their last kill came
// no more than CHAMP_CREDIT_WINDOW seconds before the newest kill on the tally: kills made at an altar that was
// then left to decay don't pay when someone else finishes it. The tiers add up, so a 1000-kill bag holds
// everything below it. Part of the tomes, scrolls and stat potions, and all the presents and vanity tokens,
// moved here from the champion's corpse (nlootgroup.cfg lootgroups 150 and 151).
const CHAMP_BAG_MIN_KILLS := 150;
const CHAMP_CREDIT_WINDOW := 7200;
const CHAMP_BAG_GRAPHIC := 0x0E76;
const CHAMP_REWARD_COLOR := 53;

function GiveRewardsToPlayers(altar)

	var players := GetObjProperty(altar, "players");
	if(TypeOf(players) != "Dictionary")
		return;
	endif

	// An offline player's bag goes to their bank box.
	set_script_option(SCRIPTOPT_CAN_ACCESS_OFFLINE_MOBILES, 1);

	// Entries are {kills, systime of the last kill}; one from before this port has no time and
	// can't show a recent kill.
	var newest := 0;
	foreach entry in players
		if(TypeOf(entry) == "Array")
			if(CInt(entry[2]) > newest)
				newest := CInt(entry[2]);
			endif
		endif
	endforeach

	var serials := players.keys();
	foreach serial in serials
		var entry := players[serial];
		// 2026-10-29: an entry from before this port is a bare kill count with no time; it is paid
		// without the time window, so a spawn running over the patch doesn't lose its earlier kills
		var kills := 0;
		var in_window := 0;
		if(TypeOf(entry) == "Array")
			kills := CInt(entry[1]);
			in_window := (newest - CInt(entry[2]) <= CHAMP_CREDIT_WINDOW);
		else
			kills := CInt(entry);
			in_window := 1;
		endif
		if(in_window and kills > 0)
			var who := SystemFindObjectBySerial(CInt(serial), SYSFIND_SEARCH_OFFLINE_MOBILES);
			if(who)
				if(kills >= CHAMP_BAG_MIN_KILLS)
					GiveChampionBag(who, kills);
				elseif(who.connected)
					SendSysMessage(who, "You slew " + kills + " of the champion's minions. It takes " + CHAMP_BAG_MIN_KILLS + " to earn a Champion Reward Bag.", FONT_NORMAL, CHAMP_REWARD_COLOR);
				endif
			endif
		endif
	endforeach

endfunction

// Builds the bag for this many kills, in the backpack of an online player or else in the bank box.
function GiveChampionBag(who, kills)

	var tomes := 0, scrolls := 0, stats := 0, tokens := 0, presents := 0;
	if(kills >= 150)
		scrolls := scrolls + 1;
		tokens := tokens + 1;
		presents := presents + 1;
	endif
	if(kills >= 300)
		scrolls := scrolls + 1;
		tokens := tokens + 1;
	endif
	if(kills >= 500)
		stats := stats + 1;
	endif
	if(kills >= 750)
		tomes := tomes + 1;
	endif
	if(kills >= 1000)
		tomes := tomes + 1;
		stats := stats + 1;
		tokens := tokens + 2;
		presents := presents + 1;
	endif

	// 2026-10-29: the bag is built in full at the offsite creation spot, then moved: to the backpack
	// when online and it fits, else to the bank box. Filling it inside the backpack lost whatever
	// went over the backpack's limits. A bag that fits nowhere stays offsite and is logged for staff.
	var bag := CreateItemAtLocation(DEFAULT_LOCATION_ITEM_CREATION_X, DEFAULT_LOCATION_ITEM_CREATION_Y, DEFAULT_LOCATION_ITEM_CREATION_Z, CHAMP_BAG_GRAPHIC, 1, DEFAULT_LOCATION_ITEM_CREATION_R);
	if(!bag)
		SysLog("Champion reward bag for " + who.name + " (" + Hex(who.serial) + ", " + kills + " kills) could not be created: " + bag.errortext);
		return 0;
	endif
	bag.name := "A Champion Reward Bag";
	bag.color := RandomInt(3000);

	var missing := 0;
	missing := missing + AddChampionReward(bag, "PowerTome", tomes);
	missing := missing + AddChampionReward(bag, "TranscendScroll", scrolls);
	missing := missing + AddChampionReward(bag, "StatPotion", stats);
	missing := missing + AddChampionReward(bag, "vanitytoken", tokens);
	var i;
	for(i := 1; i <= presents; i := i + 1)
		// The champions' old ChampionPresents group: gift1-3, equally likely.
		if(!CreateItemInContainer(bag, "gift" + (RandomInt(3) + 1), 1))
			missing := missing + 1;
		endif
	endfor
	if(missing)
		SysLog("Champion reward bag " + Hex(bag.serial) + " for " + who.name + " (" + Hex(who.serial) + ") is missing " + missing + " reward(s)");
	endif

	var in_bank := 0;
	var placed := 0;
	if(who.connected)
		placed := MoveItemToContainer(bag, who.backpack);
	endif
	if(!placed)
		// 2026-10-29: ZH3.0 has no util/bank FindBankBox; the bank box is the "World Bank" storage area
		var bankbox := CP_GetStorageContainerForMobile(who, CP_BANK_AREA, CP_CREATE);
		if(bankbox)
			placed := MoveItemToContainer(bag, bankbox);
			in_bank := 1;
		endif
	endif
	if(!placed)
		SysLog("Champion reward bag " + Hex(bag.serial) + " for " + who.name + " (" + Hex(who.serial) + ", " + kills + " kills) fits in neither backpack nor bank; left at the item creation spot");
		if(who.connected)
			SendSysMessage(who, "Your Champion Reward Bag could not be delivered. Please page staff.", FONT_NORMAL, CHAMP_REWARD_COLOR);
		endif
		return bag;
	endif

	if(who.connected)
		if(in_bank)
			SendSysMessage(who, "Your backpack is full, so your Champion Reward Bag was placed in your bank box.", FONT_NORMAL, CHAMP_REWARD_COLOR);
		else
			PlayRewardFX(who);
		endif
		SendSysMessage(who, "You slew " + kills + " of the champion's minions.", FONT_NORMAL, CHAMP_REWARD_COLOR);
	endif
	return bag;

endfunction

// One at a time, as the loot code does: these items don't stack.
// 2026-10-29: returns how many could not be created
function AddChampionReward(bag, objname, amount)

	var failed := 0;
	var i;
	for(i := 1; i <= amount; i := i + 1)
		if(!CreateItemInContainer(bag, objname, 1))
			failed := failed + 1;
		endif
	endfor
	return failed;

endfunction

function PlayRewardFX(who)
  SendSysMessage(who, "As the champion lets out its final breath, a curious bag begins to materialize in your backpack.", FONT_NORMAL, 53);
  PlayObjectCenteredEffect(who, 0x3735, 0x1, 20);
  PlaySoundEffect(who, 0x5D0);
endfunction
