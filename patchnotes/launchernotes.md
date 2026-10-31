# Latest Changes
Always check Discord announcements for all the patch notes.

---

## What Changed

Patch 3.1.5 is about getting credit for your kills and getting paid for champion spawns. Poison, poison fields, Vengeance and your pets now credit you for the kill. Champion spawns hand out Champion Reward Bags again to everyone who fought hard enough, the last wave can no longer end early, and champfire now hits everyone the same, with only elemental protection to stop it. Rangers tame, resurrect and heal their pets more reliably, every character can join a town and vote, Warriors for Hire fight the right enemy, monsters stop chasing ghosts, and a handful of smaller fixes and changes round it out.

---

## Kill Credit

- When your poison kills something, you get the kill. This covers poisoned weapons, poison-coated armour hitting back, Poison Field, poison spells and your pet's poison. Before, a poison death often counted for nobody.
- Vengeance kills now count for you, and so do kills by your pets and summons.
- A poisoning only takes the credit from someone else when it is stronger than the poison already on the target.
- Poisoning an innocent player flags you for as long as the poison works, just as hitting them does.
- Magusbane and Magehunter now shrug off poison, traps included. A Thief's poison still gets through.
- When you die, the game no longer remembers who killed you last time.

### Player Impact

- Kills by poison and Vengeance count toward champion spawns, quests and loot rights.

## Champion Spawns

- **Champion Reward Bags are back.** When a champion dies, every player with at least 150 kills at that altar gets a bag, as long as their last kill was within 2 hours of the newest one. Every character gets their own bag. More kills, more rewards:
  - 150 kills: 1 Transcend Scroll, 1 vanity token, 1 present
  - 300 kills: +1 Transcend Scroll, +1 vanity token
  - 500 kills: +1 stat potion
  - 750 kills: +1 Power Tome
  - 1000 kills: +1 Power Tome, +1 stat potion, +2 vanity tokens, +1 present
- If you are offline or your backpack is full, the bag goes to your bank box. If it fits in neither, you're told to page staff. Below 150 kills you're told how many you had.
- New command `.champkills` shows your kills at the altar you're fighting at, how many more you need for the next bigger bag, and warns you if your last kill is falling outside the 2-hour window. You're also told when you earn the bag and each time it grows.
- Your kill count now uses the killing blow, or failing that your last hit, so kills by poison, spells and pets count.
- The champion's corpse drops fewer Power Tomes, Transcend Scrolls and stat potions, and no vanity tokens or presents (those are in the bags now). Barracoon and Erebus drop magic items 85% of the time, Rikktor and Nyx 95%.
- When a wave stalls, the altar no longer adds a red skull and takes it away again. On the last wave that used to end the spawn early and summon the champion.

### Player Impact

- Fight the whole spawn to earn a bag; the more you kill, the better the bag.
- Champion corpses drop less loot, and the difference goes into the bags.

## Champfire

- Champfire is the elemental fireball that Rikktor, Nyx, The Summoner, the Soul Whisperers and Sutek throw at everyone nearby. Each cast rolls one of six elements and leaves burning fields of that element around you.
- It now hits for the same damage whoever casts it and whatever your class or Magic Resistance. Only your protection against the element it rolled reduces it. Until now, elemental protection never worked against champfire.

| Your protection in that element | Fireball | Each tick of the fields (every 5 seconds) |
|---|---|---|
| None | about 170 | about 42 |
| 50% | about 85 | about 21 |
| 95% | about 9 | about 2 |
| 100% or more | heals you | heals you |

- Cursed equipment still doubles the fireball.
- The fields really burn now. Because of a bug they hit you with your own skills and barely hurt. They now count as the champion's, and keep burning for their full minute after the champion dies.
- Champfire no longer trains Magic Resistance.

### Player Impact

- Mages and players with high Magic Resistance take far more than before. Warriors with no resist take much less from Rikktor and Nyx, and about the same from the others.
- Wear protection against the champion's elements: it is now your only defence. Step out of the fields.

## Warriors for Hire and Pets

- A Warrior for Hire sent against another player's warrior now fights that warrior, not its owner. It also never turns on its own master in the middle of a fight.
- **Sparring stays on you.** When you spar with your own warrior, it now keeps fighting you until you say "stop", even if something else attacks it. Anything that hurts your idle warrior and counts as yours, including your area spells, fields and your pet's poison, still starts a spar.
- Your pets, summons and Warriors for Hire take the same melee damage cut against players that you do. Their melee on other players does half of what it did. Their poison is unchanged.
- Some summoned creatures skipped the PvP damage cut on their spells entirely. They now take it like other pets.
- A tamed pet's mass attacks (such as a water dragon's) no longer hit you, your party or your party's pets.

### Player Impact

- Warrior-versus-warrior fights stay between the warriors.

## Rangers

- Taming and resurrecting pets get easier with every ranger level: 2 points off the difficulty per level. A level 6 ranger at 150 skill tames a 150 creature about 55% of the time (80% with the ranger's second try), where a classless tamer gets 25%. Calming a dragon can finally succeed.
- A ranger's bandage on a pet now fully heals it 3% of the time per level (18% at level 6). It used to be about 8% at every level, so level 1-2 rangers get it a little less often.
- Rangers can bandage pets and other players right after eating a magic fish.
- Each kind of magic fish has its own cooldown for rangers, so you can eat a heal, a cure and an invisibility fish back to back. The same kind still has to wait.

### Player Impact

- Rangers spend less time re-taming and resurrecting big pets, and fish no longer get in the way of bandages.

## Towns

- Every character can now be a citizen of a town, even when another character on the same account already is. Each citizen counts toward the town's population and each one votes.
- Leaving a town only removes the character who left.

### Player Impact

- Bring all your characters into your town. Votes cast before the patch still count once per account until that vote ends.

## Monsters

- Monsters no longer get stuck chasing a ghost or someone hidden until they break. They drop out of combat instead.
- Boss mass attacks no longer hit town NPCs, friendly creatures or anyone standing in a safe area.
- Custom monsters from spawnpoints come with their gear and their backpack items again.

### Player Impact

- Fewer frozen or broken monsters after a fight.

## Other Changes

- `.setph` won't let you start a personal powerhour of the same type as the server-wide one that is running or about to start, so you don't waste it.
- Player vendors in cities now cost 50,000 gold a month (was 40,000). Wages come out of the gold your vendor holds; if it runs short the unpaid wages build up as debt, and after two months in debt the vendor leaves. Keep your city vendors stocked with gold.
- Level 6 bards now have an 8% chance to turn a level 6 treasure map into a level 7 chest (was 7%). Levels 1 to 5 get 1% per level, one point less than before.
- New command `.arrows` counts the arrows, fire arrows, ice arrows, bolts and thunder bolts in your backpack.
- Some items that should stack now do.
- The Omega Cache item list no longer bogs down when you store a lot of items.
- Fire, poison, paralysis and energy fields now count as their caster's from the moment they appear. A field with no caster no longer blames whoever stepped into it first for everyone else's damage.
- **Damage you do to yourself is no longer reduced by the PvP damage cut.** Fire traps, a cursed weapon hurting its wielder, a Wrath of God that turns on you and fields with no caster now hit you for their full damage, about two-thirds more than before.
- Behind the scenes, fields that run out mid-tick no longer cause errors. No direct gameplay change expected.

---

## Summary

- Poison, poison fields, Vengeance and pets credit you for the kill; poisoning an innocent flags you; an old killer is no longer remembered.
- Champion Reward Bags are back from 150 kills, with tiers up to 1000; `.champkills` shows your count; corpses drop less, and the last wave can't end early.
- Rangers tame and resurrect more easily with level, fully heal pets more often at high level, and fish don't block their bandages.
- Every character can be a town citizen and vote.
- Champfire does the same damage to everyone, and only elemental protection reduces it.
- Warriors for Hire fight the right warrior; pets and hirelings take your PvP melee cut.
- Traps and other damage you do to yourself now hit for full damage.
- Monsters stop chasing ghosts; custom spawnpoint monsters are dressed again.
- `.setph` refuses a type that's already running; city vendors cost 50,000 a month; level 6 bards get 8% at a level 7 chest; new `.arrows`; more items stack; the Omega Cache list is faster.

Thanks for playing Zuluhotel Omega 3.
