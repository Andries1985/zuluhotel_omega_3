# Latest Changes
Always check Discord announcements for all the patch notes.

---

## What Changed

Patch 3.1.3 completes the ritual quests: every one of the 24 rituals can now be learned through a quest, with five new quest-givers and fourteen new bosses. A wave of new dungeons opens up to reach them, many teleporters now land where they should, and ritual enchantments combine more freely. Monster health was moved to a new system that keeps almost every creature exactly as tough as before, and hired warriors get sturdier. Logging in was repaired as well: households work, and the password lockout now holds. Skills set to decrease now drop gently, and your client's own skill arrows finally count. A round of fixes follows for every class, for crafting, spells and songs, traps, the banker, player vendors, guild locks, house redeeding and ritual mana crystals, and banned IP addresses are now really refused. Gathering opens up as well: a great deal more of the map can be chopped, mined and dug than the resource lists ever let you reach. Refining accepts the newer bows and leather footwear, half the tinker jewellery list stops being ignored, many more foods can be poisoned, and four items that were slipping past the class armour rules are now refused like the rest of their kind. Warriors, Bards, Thieves, Crafters and Rangers now identify items with their own class skill.

---

## Ritual Quests: All 24 Rituals

- Every ritual now has a quest that rewards its chant book. Before this patch only 10 of the 24 did.
- Five new quest-givers offer the 14 new quests. Say "quest" to one of them to see what they have.
  - **Cinderwarden Rudyom** at Faymoor: four bosses down Alryc's fire dungeons, from the Brimstone Catacombs to the Inferno.
  - **Inquisitor Erethian** near Bayfort: two bosses in the Necropolis of Nagash.
  - **Stonewarden Thragg** in Mountain Town: four great dragons in Rikktor's Lair.
  - **Warden Julia** in Erilyn: three ogre chieftains in the Bonecrusher Stronghold who stole chant books without knowing what they are.
  - **Kalabar** in the Sunken City: the last ritual, Cursing, guarded by Sutek, the Serpent's Fang, deep in Tartarus.
- Each quest works like the existing ones. Offer the listed reagents at the ritual altar, defeat the boss you summon, and bring its trophy back for gold and the chant book.
- Some new altars ask for Brimstone, Dragon's Blood, Volcanic Ash, Daemon Bone, Executioner's Cap or Bone.
- The fire dungeon, Necropolis and dragon bosses are endgame fights with 25,000 to 90,000 hit points. The Bonecrusher ogres are lighter, from 10,000 to 16,000.
- Sutek has 80,000 hit points. As he takes damage he opens portals, and up to three random elite bosses can join the fight.
- Two familiar quest-givers have new names. Valthor is now **Arch-Mage Mariah** in Dracham, and Ysolde is now **Frostkeeper Jaana** in Winterwyn. Their quests are unchanged.
- Quest progress from before this patch is not carried over: the shard is in beta, so every quest line starts fresh.
- Quest-givers are protected from snooping and stealing.

### Player Impact

- Every ritual can be earned through play now.
- Five new quest-givers to find, and fourteen new bosses to beat.

## Ritual Enchantments Combine More Freely

- Blood Seeking, Hardening, Resilience and Vital Infusion can now be performed on an item that already has Fury, a Slayer property or a poison effect.
- Perilous Theurgy and Racial Theurgy can now be performed on an item that already has a damage, armor, durability or stat bonus.
- An item still holds only one on-hit effect such as Fury, Slayer or poison. A ritual still refuses an item whose bonus of that kind is already at the most that ritual can give.

### Player Impact

- More combinations are possible on a single weapon or piece of armor. Items that rituals used to refuse can be enchanted now.

## New Dungeons and Teleporters

- New areas are reachable by teleporter:
  - The Ziggurat of Quetzalcohuatl's four lower levels and its Burial Chamber.
  - The Crimson Crucible, two levels.
  - Drakon's Reach.
  - Hasseth's Necropolis, reached from Hasseth's Fortress.
  - The Bonecrusher Stronghold, three levels and a Throneroom, entered from the Ogre Cave.
  - The Necropolis of Nagash, reached from the Great Pyramid, with its own Boss Room.
  - A Secret Retreat, with teleporters between its rooms.
  - A chain of fire dungeons: Aetherfall Bridge to the Brimstone Catacombs of Alryc, then Alryc's Magma Halls, Hellfire Abyss and Inferno.
- The teleporter from the Black City of the Damned to Tartarus used to send you back to the tile you were standing on. It now takes you to Tartarus.
- Teleporters at the Sunken City, Faymoor Island to the Solen Hive, Runebound Sanctum, and two Sosaria entrances to a mine and a cave now land you on the right tile.
- Some misplaced teleporters around the mines, the Underworld and Wayfarer's Abyss were moved a tile or two.
- **Ter Mur's teleporter links to the main lands are switched off for now,** including the exit that worked before. The rest never worked.
- The scorched and ruined ground in the new dungeons, plus some new snow terrain, now blocks or allows walking according to the game's own tile data.
- Ten new elven and wall-set door styles open, close and lock like normal doors.
- Stray doors left inside the rebuilt Quetzalcohuatl and Bonecrusher areas are gone.

### Player Impact

- A dozen new areas to explore.
- Fewer teleporters that drop you in the wrong spot or loop you back.
- No teleporter route in or out of Ter Mur this patch.

## Monsters and NPCs

- Monster health, mana and stamina now come from a fixed value per creature. For nearly every creature the numbers are exactly the same as before; the exceptions are the three below, the Earthbook elemental summons and the mage's Homunculus.
- Three creatures got tougher: pixies go from 100 to 225 hit points, dryads from 130 to 225, and fairies from 100 to 400.
- Earthbook elemental summons now scale with your class level, from about 400 to 2,000 hit points (200 with no class level) instead of a flat 1,000. The Homunculus has a fixed 750 hit points instead of 250 plus its mage's Strength.
- Creatures raised with Animate Dead now have their full health, mana and stamina; before they had half.
- A monster's maximum health, mana and stamina no longer rise or fall with Strength, Intelligence or Dexterity changes. Weakening or cursing a monster still lowers its stats, but no longer shrinks its health bar.
- Newly spawned monsters start at full health, mana and stamina.
- When two elite Chaos creatures merge, the Lord that appears is now its own creature. It has 20,000 hit points and its own loot table. A Lord that splits turns back into two of the original creature.
- Killing a good-aligned NPC no longer earns you karma. Before, every NPC counted as evil, so killing the good ones rewarded karma like killing a monster. Now it earns none and can cost you karma. Fame is not affected by this change, and creatures spawned before the update keep their old karma until they are replaced.
- Town guards no longer spawn mounted: the "Virtue Guard" variant with a shield, sword and horse is gone.
- Every creature's definition was reorganised into one standard layout. Apart from the points below, how creatures spawn, fight and drop loot is unchanged, with one side effect: the orc brute and the rhinoceros beetle now have 50 fire protection instead of 20.
- Summoned elementals (the Earthbook summons) regenerate only 5 hit points a minute on their own, well under a normal creature's 12 (they had an oddly tiny 2.4 before). Keep them alive with heals or resummon them.
- Peacemaking and Enticement now roll against their own per-creature difficulty instead of borrowing Provocation's. Today the numbers are identical, so nothing changes until a creature gets tuned.
- The Terathan Matriarch now has the Evaluating Intelligence skill she was always meant to have, and the Bewitched Peasant spawns with its gear (it used to appear naked).
- A creature's own regeneration no longer counts toward the karma and fame you earn for killing it; a couple of dozen fast-regenerating monsters are worth a little less.

### Player Impact

- Monsters hit as hard and last as long as before, apart from the three fairy-folk, the elemental summons, the Homunculus and animated dead noted above.
- Curses and weakening effects no longer cut a monster's health.
- Think twice before killing good-aligned townsfolk, guards and gentle creatures such as pixies, unicorns and wisps.
- Heal or resummon elemental summons; they still barely regenerate on their own.
- A few fast-regenerating monsters give slightly less karma and fame.

## Warrior for Hire

- A newly hired warrior starts with 150 hit points, double the 75 it had before. It also starts with 50 mana and 50 stamina.
- As your warrior's Strength grows, its maximum health grows by 2 per point. Mana and stamina grow with Intelligence and Dexterity.
- A warrior restored by the High Priest comes back with health, mana and stamina matching its stats, fully topped up.

### Player Impact

- Hired warriors are twice as sturdy from day one and keep growing with their Strength.

## Houses: Recall, Gate and Mark

- An old rune can no longer take you into someone else's house. Before, a rune marked inside a house (for example on an upper floor of a tower) kept working after that house was gone and another player built on the same spot, landing the rune's holder inside the new house.
- Recall, Gate, the runebook and Mark now treat a house's whole marked area as that house, at any height: floors, upper storeys, castle courtyards and owned static houses alike.
- Who may travel in is unchanged: the owner, co-owners, and friends or guild ranks holding the house's Teleporter permission.
- If you use a rune that leads into a house you may not enter, the spell is refused and **the rune is destroyed**. In a runebook, that entry is removed from the book.
- Mark no longer works inside a house unless you own it, co-own it or hold its Teleporter permission, including castle courtyards and owned static houses.
- You can no longer open a Gate or Recall out from inside a static house you have no access to, the same as other houses.
- When a recall spot is simply blocked, you are now told so. Before, the spell did nothing and said nothing.

### Player Impact

- Check your old runes. One that points inside another player's house will be destroyed the first time you use it.
- Runes marked on the ground outside a house, such as in front of a door, work as before.
- House owners: grant the Teleporter permission to friends who should be able to recall in.

## Logging In: Two Characters, Households and Lockouts

- You may have two characters online at once: two on one account, or one on each of your two accounts. Both must come from the same internet connection.
- Households now work. Two players who share one home connection, each with their own Discord ID, can play at the same time once staff have registered their accounts as a household. Before, an account registered in a household was refused at login.
- If you logged into the wrong account, backed out at the character list and logged into your other account, you could be refused for up to three minutes. That wait is gone.
- A Discord ID can no longer be given a third account: staff cannot create one or attach a Discord ID that already has two. Accounts that already exist are not changed.
- A login refused at the login screen now shows the right reason: the blocked-account message after too many wrong passwords, or the client's concurrency message when your Discord ID already has two characters online. Before, every refusal read "Incorrect name/password". A character refused after you pick it at the character list (a third character, a different connection, or sharing a connection without a household) is disconnected without a message.
- Five wrong passwords in a row, each within three minutes of the last, block logins to that account from that connection for ten minutes, even with the right password. Before, the block did not hold.
- A character that is refused on entering the world is no longer announced to the shard as arriving and departing.

### Player Impact

- Sharing a connection with another player? Ask staff to register your accounts as a household. Every account has to be registered, including a second account of the same player.
- Mistyped your password five times? Wait ten minutes before trying again.
- Nothing changes if you play one or two characters of your own from one connection.

## Skills: Arrows and the Verse Book

- A skill set to decrease now loses 0.1 each time you use it, instead of a whole point and whatever tenths you had. It still drops on every use, pass or fail, and locks when it reaches zero.
- The arrows in your client's own skill list now count. Setting a skill to decrease, locked or raise there does the same as in the `.skills` window, and from now on the two windows show the same arrow whenever you change one. Arrows you set before this patch are not carried over, so set them again in the window you use.
- The verse book shows which skills each verse uses, under its difficulty. Dragon Skin, for example, uses Begging and Peacemaking.

### Player Impact

- Check your arrows before you use a skill you care about: a down arrow set long ago in `.skills` still costs 0.1 per use.
- Bards: to attempt a verse you need Musicianship at or above its difficulty and every skill on its "Uses" line no more than 15 points below it. Each is rolled in turn and the verse fails at the first miss.

## Classes: Skill Gain, Strength and Rituals

- Your class bonus now works on every skill your class lists. Before, the game could look at the wrong class: a Warrior training Swordsmanship, for example, got no class bonus at all. The bonus is faster skill gain and a second roll after a failed one, which raises your chance of success.
- Warriors and Crafters now gain Strength faster and Intelligence slower. At class level 1 the Strength gain is 1.25 times as large (Intelligence 1.25 times smaller); at level 6 it is 2.5 times.
- Powerplayer: `.classinfo` now tells you how many more in-class points you need for the next level. Level 1 needs 3,750 points (75 for each of the 50 skills), and each further level needs 750 more.
- A Powerplayer still gets the class bonus only on Forensics, Throwing, Snooping and Detecting Hidden, plus its own bonus on everything at levels 3, 4 and 5.
- Only Mages can perform rituals from a ritual scroll, and only from level 2. Other classes are told "Only Mages can perform rituals."
- Mages and Mystic Archers can no longer wear any item carrying an armor-rating enchantment.

### Player Impact

- Warriors, Bladesingers and every other class should notice faster gains on their class skills.
- Warriors and Crafters: expect Strength to climb faster and Intelligence to climb more slowly.
- Powerplayers: check `.classinfo` for the exact points you still need.

## Item Identification: Your Class Skill Counts

- Identifying items now uses your class's own skill for the success roll, not just for the speed. Warriors identify with Anatomy, Bards with Taste Identification, Thieves with Remove Trap, Crafters with Arms Lore and Rangers with Camping. Mages keep using Item Identification.
- Before, only the waiting time and the chest batch looked at your class skill; the roll itself always used Item Identification, so a Warrior with 150 Anatomy still failed almost every attempt.
- Your chance is the same as a Mage's at the same skill: 85 percent on the first roll at 100 and 98 percent at 150, and your class's second roll after a miss lifts those to about 98 and 99.9 percent.
- As before, at 100 in that skill there is no wait between identifications, and with 100 Intelligence as well you can identify a whole container at once, just like a Mage.
- Identifying does not train your class skill. Mages, and anyone else who identifies with Item Identification itself, still gain as before.
- Paladins, Bladesingers, Mystic Archers, Powerplayers and characters without a class still identify with Item Identification.

### Player Impact

- Warriors, Bards, Thieves, Crafters and Rangers: identify your own loot with your class skill. At 150 it almost never fails.
- Mages: nothing changes.

## Crafting

- Bowcraft: a Crafter's chance of an exceptional item now grows with class level, like the other crafts. Before, it was the same 15 percent at every level.
- Make Bulk for arrows and bolts costs half the shafts, feathers and reagents (rounded up) during a Half-Resources power hour. Ammunition made one at a time, including Make Max, still costs one of each per piece. Make Max in the crafting windows now counts the halved cost, so it no longer stops short of your stock.
- Arrows and bolts can no longer be made for free by moving the shafts or feathers out of your pack while crafting. If you no longer have the materials you are told and the crafting stops.
- Tailoring refuses a hide that is too hard for your skill: "You aren't skilled enough to make anything with this yet." Before, you could start and fail every try.
- Silver and copper ingots (the ones made by melting coins) now show a name and difficulty in the Blacksmithy window, and they are not used for bulk orders.
- Crafter Boost products (oil, alloy, varnish and compound) are made as the correct items.
- Inscription can now make a Bulk Order Book: 100 blank scrolls, 100 mana, 5 logs and 25 cloth, at difficulty 60 (you can try from Inscription 40). Mages pay less mana and fewer scrolls, down to 40 of each at level 6; the logs and cloth are not discounted.

### Player Impact

- Crafters: higher class level means more exceptional items.
- Archers: Make Bulk ammunition in the power hour for half the cost.
- Scribes and Mages: a Bulk Order Book is now something you can make yourself.

## Spells, Songs and Buffs

- Polymorph, Shapeshift and Liche refuse to start while an armor buff is running ("Another buff you are under conflicts with that form."), before any mana is spent. Before, your body could change with nothing to change it back.
- A spell that fails because you lack reagents now gives your mana back.
- Heal, Greater Heal, Resurrect and Revive use the helpful (blue) target. If the target is in a protected area and the spell would harm them (a Liche-form player), you are told: "That would harm them, and they are in a protected area."
- Liche form lasts longer as your Mage level grows. Before, the level made no difference.
- Grand Feast (Paladin) now grows with your Paladin level.
- A Mystic Archer who casts Summon Mammals gets the Spawn of the Dead only, not extra mammals.
- Song of Defense tells you when the target already has another armor buff.
- Song of Dismissal now clears the Bard's own effects too.
- Song of Fright: a creature with no listed difficulty is no longer an automatic success.
- Enticement now ends as soon as you step in any direction, with "Enticement cancelled, you moved." Before, only a diagonal step ended it and a straight step left the song running for up to 20 seconds.
- Spirit Flock goats are tougher the better your Bard skills and level.
- A character with no class level who summons a creature gets a slightly longer, slightly stronger summon.
- The numbers shown when you cast a boost now match what was applied.

### Player Impact

- Mages: Liche lasts longer with level.
- Bards: Spirit Flock goats scale with you, and Dismissal helps you too.
- Everyone: if a buff blocks a form spell you are told so and keep your mana.

## Rangers, Pets and Mystic Archers

- A Ranger can raise a dead animal with Veterinary: it costs 5 bandages up front and the animal rises as your pet.
- A Ranger's bandages on himself are refused (Veterinary is for animals).
- Releasing a pet, or being over the pet limit, inside a safe area now confiscates it and gives you a Pet Confiscation Notice. An Animal Trainer returns the pet for a gold fine of 5 gold per point of its Strength, at least 100 gold.
- Full-damage archery range: 14 tiles for everyone, plus 1 per class level for a Ranger (20 at level 6), and 10 plus 1 per level for a Mystic Archer (11 to 16). Past that, a shot does a quarter of its damage.
- Fish scrolls, which anyone can use: a refused buff keeps the scroll and says "That effect cannot be applied to you right now, so the scroll was not used." Cure while healthy, and Teleport without enough mana or Fishing skill, also keep the scroll and tell you why.
- Dragons now count as Dragonkin, so Dragonkin slayer weapons hit them (33 dragon types fixed).
- Dragon and ostard eggs remember that they were tamed before.
- All of the newest food recipes (69 to 88) are now eaten by the hunger auto-eat; eighteen of them were missing from its list.

### Player Impact

- Rangers: raise your fallen animals, and shoot further as you level.
- Anyone releasing pets in a safe area: expect them to be confiscated, and keep the notice.
- Dragonkin slayer owners: your weapon works on dragons now.

## Thieves and Traps

- Trapped chests (from Tinkering or the Magic Trap spell) now actually spring their trap. Before, nothing triggered. Chests trapped before this patch still do not fire.
- A Thief can no longer hide with a hostile creature standing next to him.
- A treasure chest you unlock now stays for the 5 minutes the message promises, not 30 seconds.
- The Thief vendor sells Toxin Flasks and Vials of Venom, and can train Throwing.

### Player Impact

- Thieves and Tinkerers: traps are a real danger and a real tool again.
- Treasure hunters: you now have the full five minutes to loot an unlocked chest.

## Banking, Vendors, Training and Guilds

- Banker's Orders: a note you decline or that fails to cash now comes back to you. Before, it stayed with the banker. Making a note no longer takes your coins when your backpack is full, and any other item handed to a banker is returned ("I have no use for this.").
- The owner of a player vendor can buy back their own stock at no charge: the price is returned to you.
- `.escrow` no longer lists Warrior for Hire packages. Recover those through the High Priest, for his fee.
- Animal Trainers now teach exactly like town merchants: say `vendor train` (any capitals, or `vendor teach`) to open the window, and `vendor train <skill>` for a price quote.
- Merchants and trainers who know Throwing (and the last few skills) can now teach it; before, they were skipped.
- A guild can change its colour once a week and its name once a week. Before, the colour lock was 24 minutes and the name lock under 3 hours.
- A guild that has just been created cannot be renamed for a week.

### Player Impact

- Merchants: if you buy from your own vendor you no longer pay.
- Guild masters: pick your colour and name carefully, a week is the wait.
- Warrior for Hire owners: the High Priest is the only way to get a fallen warrior's belongings back.

## Houses: Redeeding Furniture

- `.redeed` only works on furniture placed inside your own house. It refuses a piece in a neighbour's house or in a pack: "That is not in your house!" or "That has to be placed in your house before it can be redeemed."
- Loose items inside a house can only be picked up by someone standing in that same house.
- Redeed still does not give the lockdown back.

### Player Impact

- House owners: your furniture is safe from a neighbour's redeed.
- Redeeding something in your own house works as before.

## Rituals: Mana Crystals

- Your mana plus up to four filled mana crystals now count towards a ritual's mana requirement, so a Mage with about 500 mana and four crystals can reach the 2,500 of the hardest rituals. Before, a crystal's mana was poured into you and anything above your maximum was lost, so a crystal could never lift you past your own maximum mana.
- The crystals are used up only once the ritual's requirement is met. If you do not have enough, the ritual is called off and you keep your crystals, though the scroll is still used up.
- If you are short you are told what the ritual needs and what you have (your mana plus your crystals).

### Player Impact

- Ritual casters: charge your crystals and bring up to four.

## Gathering: Wood, Ore and Sand

- Far more trees can be chopped. A large batch of trees and vines that your axe simply did nothing to now give wood, including most of the newer-looking trees around the world.
- A lot more rock and cave floor can be mined. Tiles that answered "You can't mine or dig anything there" now yield ore, especially inside caves.
- More beach and desert ground now gives sand.
- What each tile yields, and how fast it regrows, has not changed. These spots were always meant to be harvestable and simply were not responding.
- Ferns, grasses, cattails, flowers, rushes and weeds no longer give wood, and some dirt, grass and forest ground no longer gives ore. Ground dug for sand that held none now says "You can't mine or dig anything there." instead of letting you dig for nothing.

### Player Impact

- Lumberjacks and miners: a great deal more of the map is worth working, and the newer dungeon areas are no longer dead ground.
- Expect your usual routes to have more usable spots than before.

## Refining and Crafting

- The six newer bows - the shortbow, composite bow, repeating crossbow, yumi, elven composite longbow and skull crossbow - can be improved with a Refining Varnish. It used to refuse them with "Varnishes can only refine wooden weapons and armor."
- Leather boots, thigh boots, shoes and sandals, and the orc, bear, deer, tribal and voodoo masks, can be improved with a Refining Compound. They were refused before even though they are made from hides.
- Six tinker-made pieces of jewellery - the beaded necklace, the second bracelet and earrings, the third necklace, the second ring and the silver necklace - now count as jewellery: their tooltip shows the full details, and anything that checks the jewellery list sees them.

### Player Impact

- Archers: your newer bows can finally be refined like every other bow.
- Tailors: footwear and masks are worth refining now.
- Tinkers: half the jewellery list was being ignored by the tooltip and now shows its details.

## Poisoning

- Many more foods can be poisoned: everything the kitchen counts as food, about sixty more than before, and drinks as before. Fish steaks, several fruits and a number of cooked dishes used to answer "You can't poison that."

### Player Impact

- Poisoners: roughly sixty more foods are valid targets.
- Taste Identification follows the same list, so it works on the new foods too.

## Class Restrictions

Some items were slipping past the armour and shield rules for your class. They are now refused like the rest of their kind:

- **Chaos shields and Order shields** are shields, so Bards, Mages, Mystic Archers, Rangers, Thieves and Bladesingers can no longer carry one.
- **Female studded leather** - the female studded armour and the studded bustier - counts as studded leather, so Bladesingers and Paladins can no longer wear it.
- **The orc helm** counts with plate, so Mages, Thieves, Bards, Mystic Archers and Rangers can no longer wear one.
- **The Navar Bloody Barrier** shield was barred for Bards, Mages, Mystic Archers and Bladesingers only. Thieves and Rangers lose it too now. Thieves keep the buckler, which is deliberate.

### Player Impact

- If you are wearing one of these right now, it will be moved into your backpack the first time the shard checks your class, and you will see the usual warning about illegal items. **This is not an accusation** - the item was legitimately equippable until this patch. Nothing is confiscated and nobody is being jailed for it.
- If you built a character around one of these pieces, speak to staff.

## Staff Tools

- Staff item and player inspection tools (`.iteminfo`, `.info`) were rebuilt and fixed, and a new `.regeninfo` shows a creature's regeneration. A Seer resurrecting an Orc or Frost Elf through the staff info panel now restores the right skin color.
- The teleporter placement command now places some teleporters it used to skip.
- A staff area-clear command could delete spawn points by mistake. It no longer does, so monsters should stop vanishing from areas staff tidied up.
- The area spawner's custom-NPC regen fields are now plain points per minute; values saved earlier convert on their own.
- Staff can manage households and Discord IDs for accounts that are offline, and can correct an account's Discord ID in game.
- The IP ban list now works: a banned address, or a range such as `65.5.255.255`, is refused at login. `.ipban` also refuses a range that contains your own address and disconnects anyone already online inside a banned range.
- The class stone and `.setclass` raise the skill caps along with the skills, so level 5 and 6 skills are no longer pulled back to 130 by the hourly capper.

### Player Impact

- Fewer "a GM tried to help and it didn't work" moments.

## Summary

- All 24 rituals can be earned through quests: five new quest-givers, fourteen new bosses and Sutek in Tartarus as the finale. Valthor and Ysolde are now Mariah and Jaana.
- Ritual enchantments combine more freely; one on-hit effect per item remains the limit.
- New dungeons: the lower levels of the Ziggurat of Quetzalcohuatl, the Crimson Crucible, Drakon's Reach, Hasseth's Necropolis, the Bonecrusher Stronghold, the Necropolis of Nagash, a Secret Retreat and Alryc's fire dungeons.
- Many teleporters fixed; Ter Mur's teleporters are switched off for now.
- Monster health moved to a fixed system with no change for nearly all creatures; pixies, dryads and fairies are tougher, elemental summons scale with your class level and animated dead keep full health; curses no longer cut a monster's health.
- Chaos Lords are their own creatures with their own loot.
- Killing good-aligned NPCs no longer earns karma.
- Creature definitions standardised; summoned elementals regenerate only 5 a minute; Peacemaking and Enticement have their own difficulties.
- Hired warriors start with 150 hit points and grow with Strength.
- Old runes no longer lead into other players' houses: refused runes are destroyed, and Mark is blocked inside someone else's house.
- Households work: two players on one connection can play together once registered. The password lockout now holds, and login refusals show the right reason.
- Down-arrow skills lose 0.1 per use instead of a whole point; the client's skill arrows work; the verse book lists each verse's skills.
- Quest-givers are protected from snooping and theft; town guards no longer ride; staff tools fixed.
- Classes: the class bonus works on every skill your class lists; Warriors and Crafters gain Strength faster and Intelligence slower; Powerplayer `.classinfo` shows the points you need; only Mages can perform rituals.
- Item identification rolls on your class skill: a Warrior identifies with Anatomy and at 150 almost never fails; identifying does not train that skill, and Mages are unchanged.
- Crafting: Crafter exceptional chance scales with level; bulk ammo is half price in the power hour and ammo cannot be duplicated; tailoring refuses hides that are too hard; silver and copper ingots are listed properly; Inscription makes the Bulk Order Book.
- Spells and songs: form spells refuse while an armor buff runs and keep your mana; reagent failures refund mana; healing spells use the helpful target; Liche scales with Mage level; several song and Enticement fixes.
- Rangers and pets: raise dead animals as pets; safe-area pet confiscation with a trainer fine; archery range grows with class level; dragons count as Dragonkin.
- Thieves: trapped chests really spring; no hiding next to a hostile; unlocked treasure chests last 5 minutes; the Thief vendor sells poisons and trains Throwing.
- Banker notes come back when declined, no coins lost to a full backpack; owners buy their own vendor stock free; Animal Trainers train like merchants; guild colour and name locks are one week.
- `.redeed` only works inside your own house; loose items can only be lifted from inside the same house.
- Ritual mana crystals now pool with your mana (up to four) towards the requirement.
- Banned IP addresses and ranges are refused at login.
- Gathering: many more trees can be chopped, much more rock and cave floor can be mined, and more beach and desert gives sand; ferns, grasses and some plain ground no longer give wood or ore. Yields and regrowth are unchanged.
- Refining: the six newer bows take a varnish; leather boots, thigh boots, shoes, sandals and the five carved masks take a compound.
- Six tinker-made jewellery pieces now count as jewellery in tooltips and wherever the jewellery list is checked.
- Many more foods can be poisoned, about sixty more than before.
- Class rules: Chaos and Order shields, female studded leather and the orc helm are now refused by the classes that are barred from their kind, and Thieves and Rangers lose the Navar shield. If you are wearing one it moves to your backpack with a warning - no one is in trouble for it.

Thanks for playing Zuluhotel Omega 3.
