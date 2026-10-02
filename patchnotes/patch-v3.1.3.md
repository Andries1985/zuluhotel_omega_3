# Patch Notes - v3.1.3
**Zuluhotel Omega 3 | Beta Shard**

**Date: October 2, 2026**

---

## What Changed

Patch 3.1.3 completes the ritual quests: every one of the 24 rituals can now be learned through a quest, with six new quest-givers and fourteen new bosses. A wave of new dungeons opens up to reach them, many teleporters now land where they should, and ritual enchantments combine more freely. Monster health was moved to a new system that keeps almost every creature exactly as tough as before, and hired warriors get sturdier. Logging in was repaired as well: households work, and the password lockout now holds. Skills set to decrease now drop gently, and your client's own skill arrows finally count.

---

## Ritual Quests: All 24 Rituals

- Every ritual now has a quest that rewards its chant book. Before this patch only 10 of the 24 did.
- Six quest-givers offer the 14 new quests. Say "quest" to one of them to see what they have.
  - **Cinderwarden Rudyom** at Faymoor: four bosses down Alryc's fire dungeons, from the Brimstone Catacombs to the Inferno.
  - **Inquisitor Erethian** near Bayfort: two bosses in the Necropolis of Nagash.
  - **Stonewarden Thragg** in Mountain Town: four great dragons in Rikktor's Lair.
  - **Warden Julia** in Erilyn: three ogre chieftains in the Bonecrusher Stronghold who stole chant books without knowing what they are.
  - **Kalabar** in the Sunken City: the last ritual, Cursing, guarded by Sutek, the Serpent's Fang, deep in Tartarus.
- Each quest works like the existing ones. Offer the listed reagents at the ritual altar, defeat the boss you summon, and bring its trophy back for gold and the chant book.
- Some new altars ask for Brimstone, Dragon's Blood, Volcanic Ash, Daemon Bone, Executioner's Cap or Bone.
- The fire dungeon, Necropolis and dragon bosses are endgame fights with 25,000 to 90,000 hit points. The Bonecrusher ogres are lighter, from 10,000 to 16,000.
- Sutek has 80,000 hit points. As he takes damage he opens portals, and up to three random elite bosses can join the fight.
- Two familiar quest-givers have new names. Valthor is now **Arch-Mage Mariah** in Dracham, and Ysolde is now **Frostkeeper Jaana** in Winterwyn. Their quests and your progress on them are unchanged.
- Quest-givers can no longer be snooped or stolen from.

### Player Impact

- Every ritual can be earned through play now.
- Six new quest-givers to find, and fourteen new bosses to beat.

## Ritual Enchantments Combine More Freely

- Blood Seeking, Hardening, Resilience and Vital Infusion can now be performed on an item that already has Fury, a Slayer property or a poison effect.
- Perilous Theurgy and Racial Theurgy can now be performed on an item that already has a damage, armor, durability or stat bonus.
- An item still holds only one on-hit effect such as Fury, Slayer or poison. A ritual still refuses an item that already has an equal or stronger bonus of the same kind.

### Player Impact

- More combinations are possible on a single weapon or piece of armor. Items that rituals used to refuse can be enchanted now.

## New Dungeons and Teleporters

- New areas are reachable by teleporter:
  - The Ziggurat of Quetzalcohuatl, four levels and a Burial Chamber.
  - The Crimson Crucible, two levels.
  - Drakon's Reach.
  - Hasseth's Fortress and Hasseth's Necropolis.
  - The Bonecrusher Stronghold, three levels and a Throneroom, entered from the Ogre Cave.
  - The Necropolis of Nagash, reached from the Great Pyramid, with its own Boss Room.
  - A Secret Retreat.
  - A chain of fire dungeons: Aetherfall Bridge to the Brimstone Catacombs of Alryc, then Alryc's Magma Halls, Hellfire Abyss and Inferno.
- The Poison Dungeon has a teleporter to its boss room.
- The teleporter from the Black City of the Damned to Tartarus used to send you back to the tile you were standing on. It now takes you to Tartarus.
- Teleporters at the Sunken City, Faymoor Island to the Solen Hive, Runebound Sanctum, and two Sosaria entrances to a mine and a cave now land you on the right tile.
- Duplicate and dead teleporters around the mines, the Underworld and Wayfarer's Abyss were removed. Every place they led is still reachable.
- **Ter Mur is closed off for now.** All teleporters between Ter Mur and the main lands are switched off, including the exit that worked before. Most of the others never worked.
- Some teleporters that failed to appear after a server restart now appear reliably.
- The scorched and ruined ground in the new dungeons, plus some new snow terrain, now blocks and lets you walk exactly as it looks.
- Ten new elven and wall-set door styles open, close and lock like normal doors.
- Stray doors left inside the rebuilt Quetzalcohuatl and Bonecrusher areas are gone.

### Player Impact

- A dozen new areas to explore.
- Fewer teleporters that drop you in the wrong spot or loop you back.
- No teleporter route in or out of Ter Mur this patch.

## Monsters and NPCs

- Monster health, mana and stamina now come from a fixed value per creature. For nearly every creature the numbers are exactly the same as before.
- Three creatures got tougher: pixies go from 100 to 225 hit points, dryads from 130 to 225, and fairies from 100 to 400.
- A monster's maximum health, mana and stamina no longer rise or fall with Strength, Intelligence or Dexterity changes. Weakening or cursing a monster still lowers its stats, but no longer shrinks its health bar.
- Newly spawned monsters start at full health, mana and stamina.
- When two elite Chaos creatures merge, the Lord that appears is now its own creature. It has 20,000 hit points and its own loot table. A Lord that splits turns back into two of the original creature.
- Killing a good-aligned NPC no longer earns you karma. Before, every NPC counted as evil, so killing the good ones rewarded karma like killing a monster. Now it earns none and can cost you karma. Fame works as before.
- Town guards all look the same now. The mounted "Virtue Guard" variant with a shield, sword and horse no longer spawns.
- Every creature's definition was reorganised into one standard layout. Nothing about how creatures spawn, fight or drop loot changed, apart from the points below.
- Summoned elementals (the Earthbook summons) regenerate only 5 hit points a minute on their own, well under a normal creature's 12 (they had an oddly tiny 2.4 before). Keep them alive with heals or resummon them.
- Peacemaking and Enticement now roll against their own per-creature difficulty instead of borrowing Provocation's. Today the numbers are identical, so nothing changes until a creature gets tuned.
- The Terathan Matriarch now has the Evaluating Intelligence skill she was always meant to have, and the Bewitched Peasant spawns with its gear (it used to appear naked).
- A creature's own regeneration no longer counts toward the karma you earn for killing it; a few fast-regenerating monsters are worth a little less karma.

### Player Impact

- Monsters hit as hard and last as long as before, apart from the three fairy-folk above.
- Curses and weakening effects no longer cut a monster's health.
- Think twice before killing good-aligned townsfolk.
- Heal or resummon elemental summons; they still barely regenerate on their own.
- A few fast-regenerating bosses give slightly less karma.

## Warrior for Hire

- A newly hired warrior starts with 150 hit points, double the 75 it had before. It also starts with 50 mana and 50 stamina.
- As your warrior's Strength grows, its maximum health grows by 2 per point. Mana and stamina grow with Intelligence and Dexterity.
- A warrior restored by the High Priest comes back with health, mana and stamina matching its stats, fully topped up.

### Player Impact

- Hired warriors are twice as sturdy from day one and keep growing with their Strength.

## Houses: Recall, Gate and Mark

- An old rune can no longer take you into someone else's house. Before, a rune marked inside a house (for example on an upper floor of a tower) kept working after that house was gone and another player built on the same spot, landing the rune's holder inside the new house.
- Recall, Gate, the runebook and Mark now treat the whole footprint of a house as that house: floors, upper storeys, castle courtyards and static houses alike.
- Who may travel in is unchanged: the owner, co-owners, and friends or guild ranks holding the house's Teleporter permission.
- If you use a rune that leads into a house you may not enter, the spell is refused and **the rune is destroyed**. In a runebook, that entry is removed from the book.
- Mark no longer works inside someone else's house, including courtyards and static houses.
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
- Each player (one Discord ID) may hold at most two accounts.
- A refused login now shows the right reason, such as too many characters online or a temporarily blocked account. Before, it could read "Incorrect name/password" whatever the cause.
- Five wrong passwords within three minutes block logins to that account from that connection for ten minutes, even with the right password. Before, the block did not hold.
- A character that is refused on entering the world is no longer announced to the shard as arriving and departing.

### Player Impact

- Sharing a connection with another player? Ask staff to register your accounts as a household. Every account has to be registered, including a second account of the same player.
- Mistyped your password five times? Wait ten minutes before trying again.
- Nothing changes if you play one or two characters of your own from one connection.

## Skills: Arrows and the Verse Book

- A skill set to decrease now loses 0.1 each time you use it, instead of a whole point and whatever tenths you had. It still drops on every use, pass or fail, and locks when it reaches zero.
- The arrows in your client's own skill list now count. Setting a skill to decrease, locked or raise there does the same as in the `.skills` window, and the two windows show the same arrows.
- The verse book shows which skills each verse uses, under its difficulty. Dragon Skin, for example, uses Begging and Peacemaking.

### Player Impact

- Check your arrows before you use a skill you care about: a down arrow set long ago in `.skills` still costs 0.1 per use.
- Bards: a verse rolls every skill on its "Uses" line, and you need each of them within 15 points of the verse's difficulty to attempt it.

## Staff Tools

- Staff item, area and player inspection tools were rebuilt and fixed. A Seer resurrecting an Orc or Frost Elf through the staff info panel now restores the right skin color.
- A staff area-clear command could delete spawn points by mistake. It no longer does, so monsters should stop vanishing from areas staff tidied up.
- The area spawner's custom-NPC regen fields are now plain points per minute; values saved earlier convert on their own.
- Staff can manage households and Discord IDs for accounts that are offline, and can correct an account's Discord ID in game.

### Player Impact

- Fewer "a GM tried to help and it didn't work" moments.

## Summary

- All 24 rituals can be earned through quests: six new quest-givers, fourteen new bosses and Sutek in Tartarus as the finale. Valthor and Ysolde are now Mariah and Jaana.
- Ritual enchantments combine more freely; one on-hit effect per item remains the limit.
- New dungeons: the Ziggurat of Quetzalcohuatl, the Crimson Crucible, Drakon's Reach, Hasseth's Fortress and Necropolis, the Bonecrusher Stronghold, the Necropolis of Nagash, a Secret Retreat and Alryc's fire dungeons.
- Many teleporters fixed; Ter Mur's teleporters are switched off for now.
- Monster health moved to a fixed system with no change for nearly all creatures; pixies, dryads and fairies are tougher; curses no longer cut a monster's health.
- Chaos Lords are their own creatures with their own loot.
- Killing good-aligned NPCs no longer earns karma.
- Creature definitions standardised; summoned elementals regenerate only 5 a minute; Peacemaking and Enticement have their own difficulties.
- Hired warriors start with 150 hit points and grow with Strength.
- Old runes no longer lead into other players' houses: refused runes are destroyed, and Mark is blocked inside someone else's house.
- Households work: two players on one connection can play together once registered. The password lockout now holds, and login refusals show the right reason.
- Down-arrow skills lose 0.1 per use instead of a whole point; the client's skill arrows work; the verse book lists each verse's skills.
- Quest-givers can't be snooped or robbed; town guards are uniform; staff tools fixed.

Thanks for playing Zuluhotel Omega 3.
