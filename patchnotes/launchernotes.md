# Latest Changes
Always check Discord announcements for all the patch notes.

---

## What Changed

Patch 3.1.4 is about powerhours and classes. The server-wide powerhour can no longer get stuck on after a restart, its bonus-hour odds finally follow the rule they were meant to, a weekly powerhour that gets crowded out is no longer lost, and staff can now run and adjust powerhours in game. Personal powerhours end when they should, even after you relog, and survive a restart. On the class side, Powerplayers lose the last of their per-skill bonuses, characters with two classes are judged by their stronger one, and combat, spells and item tooltips stop recounting your skills and re-checking your gear every time they look at your class. In houses, every character on the owner's account is now treated as the owner. Warriors for Hire get a long list of fixes, from the High Priest to potions, and area checks get much cheaper for the server. Mayors can now step down, the vanity shop sells scrolls that copy a runebook or Runic Atlas, fish steaks and fish stacks are sorted out, and elemental arrows come earlier for bowyers. And Arcomage, the tower-building card game, opens in taverns across Britannia. Late in the patch, Sanctuary, Antidote and Dragon Skin were reworked into proper timed buffs with icons, spellcasting monsters got smarter about who they hit, and town folk became their own creature type.

---

## Server-Wide Powerhour

- A server-wide powerhour no longer stays on after a server restart. Until now, a restart in the middle of one left its bonus switched on with nothing to switch it off, sometimes for up to a week. Its end time is now kept, so after a restart it carries on and ends on time. A restart during the 2-minute warning still gives you the powerhour you were promised.
- If the server was down for more than 30 minutes past a powerhour's end, it is simply over when the server comes back. You won't get an "ended" message for an hour you never saw.
- The bonus "second powerhour" now follows its rule: a 50% chance after a half-resources powerhour, 10% after a hunting or double-skillgain one. Until now the game rolled against a random type instead of the one that had run, so every week got the same blended odds. Over many weeks the number of bonus hours stays about the same; they just land where they were meant to.
- Only the weekly powerhour can earn a bonus hour. Powerhours started by staff, bonus hours themselves, and a weekly powerhour that staff ended early or re-timed never do. That keeps it to at most one bonus hour a week.
- A weekly powerhour is no longer lost when another powerhour is running at its scheduled time, or the server is down. It starts as soon as the server is free, as long as that is within 6 hours of its usual time.
- `.ph` now shows how long the server-wide powerhour has left, and says when one is about to start during the 2-minute warning. Countdowns skip the empty parts: "3 hours and 12 minutes" rather than "0 days, 3 hours, 12 minutes and 0 seconds".
- The weekly powerhour stays on Sunday at 19:00 GMT. Staff can now move it. If they do, `.ph` shows the new time.

### Player Impact

- No more week-long "stuck" powerhours after a restart.
- Half-resources weeks are the ones most likely to bring a second powerhour.
- Expect to see powerhours around events more often, now that staff can start them without a restart.

## Personal Powerhour

- A personal powerhour now ends on time after you log out and back in. Before, relogging during one left it running until the next server restart.
- A personal powerhour now survives more than one server restart within the same hour. Before, the second restart ended it early and the week still counted as used.
- If staff reset your powerhour and you start a new one, the old hour's timer no longer cuts the new one short.
- After using your personal powerhour on a Sunday, `.ph` now shows the real time until you can start the next one. It could be up to a day short.
- Your weekly lock now lifts at the next Sunday 00:00 GMT (a full week later if you used it on a Sunday) and stays lifted. Before, if you didn't start a new one by then, the lock came back the following week on the weekday you last used it, until the hour you used it, while `.ph` said you could start one in less than a minute.
- `.setph` and `.ph` now use the same names for the three types everywhere: Hunting, Half-resources and Double skillgain.

### Player Impact

- If you relogged during a personal powerhour and it kept going, that will stop happening.
- The weekly rule for `.setph` has not changed.

## Classes

- Powerplayers no longer get the class bonus on Forensics, Throwing, Snooping and Detecting Hidden. That bonus was faster gain and a second roll after a failed check, and those four skills were the last ones that had it. A Powerplayer's skill-gain bonus is now its own one on every skill, and it starts a level sooner and keeps growing: 1.1 times at level 2, 1.2 at level 3, 1.3 at level 4, 1.4 at level 5 and 1.5 at level 6. Before, it was 1.1 at level 3, 1.2 at level 4, 1.3 at level 5 and nothing at levels 1, 2 or 6.
- If you qualify for two classes, the game now treats you as the one you are strongest in. That applies to which items you may wear and every other rule that looks at your class. Until now it took whichever came first in the class list, so a level 3 Ranger who was also a level 1 Mystic Archer was judged as a level 1 Mystic Archer. This is rare: it needs two closely related classes and a narrow build.
- When your two classes disagree on a stat, the higher-level class decides whether it grows faster or slower.
- Your class is now looked up from your last class check, not recounted from all your skills every time. This affects hits, spells, healing, songs, the guild window and item tooltips. When you reach a new class level, it counts from your next class check: when you log in, use `.showclasse`, or within 10 minutes.
- Hits, spells and item tooltips no longer re-check all your worn gear for items your class may not wear. That check now runs only when your class is checked: at login, on the class commands, and every 10 minutes. Expect combat and hovering over items to feel snappier.
- Characters without a class are now also checked at login for items marked for role-players. Anyone who is not a role-player has such an item moved to their backpack.

### Player Impact

- Powerplayers: expect slower gains on Forensics, Throwing, Snooping and Detecting Hidden than before, and faster gains everywhere else from level 2.
- Single-class characters see no change in rules, only in speed.
- Just reached a new class level? Log out and back in, or use `.showclasse`, to use it straight away.

## Housing

- Every character on the same account as a house's owner now has full owner rights in that house: locking down and securing items, house teleporters, ejecting, banning, managing friends and co-owners, redeeding, trapping, and relinquishing the house. Until now only the owning character had most of these, and your other characters could not even be added to the friend list.
- Your other characters can no longer be banned or ejected from your house. If one of them was already on your ban list, the ban no longer applies.
- Trying to add one of your own characters as a friend or co-owner now tells you it already has full owner rights, instead of saying "Cancelled."
- You can hand a house to another character on your own account even when your account is at its house limit, since your house count does not change.
- Co-owners and friends of a house can now set traps on containers in it, with the trap spell or a trap kit. Until now only the owner could in most houses.
- The house sign still shows the owning character's name.

### Player Impact

- Log in on any character on your account and your house works for it the same as for the owner.

## Warriors for Hire

- A warrior can now be brought back ten times in all, by its heart or by the High Priest, and the eleventh death is final. Until now the High Priest could rebuild a warrior from memory for 10,000 gold as often as you paid, and a heart and the priest could both bring back the same death.
- Only the heart from your warrior's latest death works. An older heart you kept crumbles when you use it, and bringing your warrior back destroys any other hearts you still hold.
- Hearts from before this patch no longer bring a warrior back; they crumble. If your warrior died before the patch, ask the High Priest to resurrect it from memory instead.
- Only the warrior's own master can hand its heart to the High Priest. A heart given to him by anyone else crumbles, so don't pass your warrior's heart to a friend.
- A warrior revived from its heart, or from the High Priest's memory, now loses one point of each stat per death so far, never below 10. Before, the first heart revival actually added a point, and a memory revival kept every stat.
- A warrior that died while blessed no longer comes back with the blessing added to its base stats for good.
- The High Priest no longer stops working when he refuses a heart (for example because you already have a henchman); he hands the heart back. His questions now close after 30 seconds, so one unanswered window no longer freezes him for everyone.
- When you release a warrior or give it up at the High Priest, its gear now goes to your escrow, where the High Priest returns it for 20,000 gold, as after any other death. It used to vanish.
- If your dead warrior's gear cannot be filed into escrow, it now goes to your bank box instead of being destroyed.
- Saying "stop" now makes your warrior stand still and stop guarding, in a fight or out of one, so it no longer jumps back in. It stays put until you tell it to follow ("follow me"), and it guards you again once you say "guard" or give it something to kill.
- If the guards kill your warrior, its gear now goes to your escrow like after any other death. It used to vanish with the body.
- Your warrior's status window now counts Dragon Skin and Sanctuary or Antidote in its protection and poison immunity.
- Warriors now recognise every bow and crossbow, not just about 20 of them, and use them as ranged weapons.
- Warriors drink one heal potion at a time. They used to use up the whole stack for one drink. They now use Light Heal, Heal, Greater Heal, the true-mage potions and Tamla, weakest first.
- Warriors no longer try to bandage a dead master, and no longer start a new self-heal every second while one is already running.
- NPC healers no longer use bandages they are no longer carrying.
- Your warrior keeps fighting, following and healing while its status window is open. The window no longer lists its backpack, hair and beard, and now shows its hits.
- Your hits on your own warrior now do half damage, the same as its hits on you.
- A warrior's hits, mana and stamina now work like yours: they equal its Strength, Intelligence and Dexterity. A Bless or the Staff of Nagash raises both the stat and the vital, and both drop back when it ends. Warriors used to have twice their Strength in hits, so expect about half the hits you are used to.

### Player Impact

- Keep your warrior's newest heart, and bring it to the High Priest yourself; old hearts are worthless now.
- Releasing a warrior, or losing it to the guards, no longer costs you its gear.
- "Stop" means stand still now; say "follow me" when you want it back at your side.
- Warriors with bows and potions are much better at staying alive.
- Warriors have about half the hits they used to; Bless them to raise it.

## Eon-Prism

- The Eon-Prism comes to this shard. Using it lets you start another personal powerhour this week with `.setph`. It can't be used during a personal or server-wide powerhour, or when you can already start one, and it must be in your backpack. It can be found in the strange box, like other artifacts.

### Player Impact

- One more way to get a second personal powerhour in a week.

## Townstones

- A mayor can now resign and leave the town. The mayor's townstone window has a new "Resign and leave town" button. After you confirm, you stop being mayor and give up your citizenship, and the server is told you resigned. Before, a mayor had no way to leave.
- An election or poll that is already running carries on. With no mayor, citizens can call a new election straight away.

### Player Impact

- Mayors who want out no longer need staff to remove them.

## Vanity Shop: Transcription Scrolls

- New Runebook Transcription Scroll (5 tokens) and Runic Atlas Transcription Scroll (10 tokens). Use one, pick the book to copy, then pick an empty book of the same kind: every rune is copied into it and the scroll is used up.
- Runebook to runebook and Runic Atlas to Runic Atlas only. The book you copy from must have runes, the book you copy into must be empty, and both must be in your backpack. The scroll does not make a new book.
- Only the runes are copied. The new book keeps its own charges, name and colour, and has no default rune set.
- Close both books first: a book with its window open can't be copied. While the scroll is working, the two books can't be moved or opened.
- Copied runes follow the same travel rules as the originals, so a copy can't take you anywhere the original couldn't.

### Player Impact

- Share a rune library with friends, or keep a backup, without marking every rune again.

## Fishing

- How many fish steaks you get now depends on where the fish lives: 1 for a shore fish, 4 for a deep-water fish and 6 for a dungeon fish. Dungeon fish used to give 4 at most. This is the same whether you carve the catch as you fish or later with a knife.
- Rare fish give twice the steaks and Legendary fish ten times, now also when you carve them as you fish. Before, that bonus only came from a knife.
- Ordinary fish stack in your backpack again. Each new catch used to land as a separate pile.
- Rare and Legendary fish and crustaceans never stack, so every one keeps its own trophy weight.

### Player Impact

- Dungeon fishing is the best source of steaks.
- No more backpacks full of single fish.

## Bowcraft

- Fire Arrows, Ice Arrows and Thunder Bolts now need 40 less skill, whether you craft them from scratch or improve a plain arrow or bolt: 74 for Fire Arrows (was 114), 79 for Ice Arrows (was 119) and 84 for Thunder Bolts (was 124).

### Player Impact

- Bowyers can make elemental ammunition much earlier.

## Arcomage

- Arcomage, the tower-building card game, comes to Britannia. 24 card players wait in taverns across the land, from Sherry the Mouse and Chuckles the Jester to Mondain and Captain Hook. Hover over one to see their title in gold, from Arcomage Novice to Arcomage Grandmaster, and say "arcomage" next to them to play.
- You and your opponent each have a tower and a wall. Damage hits the wall first and the rest comes off the tower. Your Mine makes Ore, your Grove makes Reagents and your Barracks make Recruits, and every card costs one of the three.
- On your turn you draw a card, then play one you can afford or throw one away. Some cards let you play again. You win by building your tower to the goal, gathering the goal in any one resource, or bringing the enemy tower down to nothing. There are 102 cards.
- Every card player has their own rules: starting tower, wall, resources and goals. The better-known names play the longer games.
- Before each game you pick a difficulty, from 30 to 150 like a skill. At 30 your opponent plays almost at random; at 150 it makes its best move every time.
- Your first win at each difficulty each day pays 100 gold per point of difficulty: 3,000 gold at 30, up to 15,000 at 150. It doesn't matter who you beat. This is per account and resets at server midnight; later wins that day still count for your record.
- The board shows the last few moves, with exactly what each card did to each side.
- Walking more than 8 tiles from the table, logging out or dying ends the game as a loss. You can also resign.
- `.arcomage` shows your wins against every card player at every difficulty, and the shard's top 20.
- Play against other players with an Arcomage Deck, 5 tokens in the vanity shop. Use it on a player within 3 tiles, choose whose rules to play by (any card player you have beaten), and choose Timed or Untimed. They get an invitation and have 60 seconds to accept. A coin flip decides who goes first.
- Timed games give each move 120 seconds. Run out and a random card from your hand is thrown away; run out three times in a row and you lose. You can move around, but stay within 8 tiles of your spot.
- Untimed games have no clock, but taking a single step from your spot loses the game.
- In either mode, logging out, dying or resigning loses the game. You can't play against a character on your own account.
- `.arcomage watch` lets you watch someone else's game: target one of the players while you are within 12 tiles. Up to 6 people can watch a game, and they see the board but not the players' hands.

### Player Impact

- A new way to earn gold every day: up to 45,000 an account if you win at all five difficulties.
- Records and the leaderboard are per character. Games against other players are just for fun: no gold and no records.

## Sanctuary, Antidote and Dragon Skin

- All three now last from 2 minutes at class level 1 up to 10 minutes at level 6 (2:00, 3:36, 5:12, 6:48, 8:24, 10:00), and give a level equal to your class level, from 1 to 6. Characters without a class count as level 1. Magery, armor and Intelligence gear no longer change the length or the level. Before, a Mage could get about 16 minutes while a Paladin in plate got about 3, Sanctuary gave Powerplayers level 1, and Antidote could show levels far above 6 that did nothing extra.
- Antidote's poison immunity now goes to the person you cast it on. It used to go to you, the caster, whoever you cured.
- The spells no longer add to your gear or overwrite it: you get whichever is higher, your gear's level or the spell's. Putting on or taking off poison immunity jewelry during Sanctuary no longer switches the immunity off, and equipping anything during Dragon Skin no longer drops the skin or lowers your gear's protection afterwards.
- If your gear or a stronger cast already gives you as much, the spell adds nothing. Casting the same level again refreshes it.
- Dragon Skin is now one play instead of a song you keep playing. It protects you and your party, guild and pets in range, costs 20 stamina per person, and lasts its full time even if they walk away. Its protection is your bard level, and it no longer stacks on top of protection gear: a level 6 bard in level 6 gear gets 6, not 12.
- Dispel, Mass Dispel and Song of Dismissal now remove Sanctuary, Antidote, Dragon Skin and Reactive Armor. Logging out also ends them.
- At your first login after this patch, your physical protection and poison immunity are reset to what your gear gives. This clears leftover Dragon Skin bonuses that never wore off.

### Player Impact

- Mages lose their extra-long Sanctuary and Antidote; every class now gets the same time for its level.
- Cast Antidote on a friend and the friend is protected, not you.
- Dragon Skin is weaker for bards in heavy protection gear, but it no longer needs you to keep playing.

## Buff Icons

- New buff icons, with a countdown where the effect has one: poison immunity from Sanctuary and Antidote, Dragon Skin, Reactive Armor, night sight (the spell, Owl Sight, Light of Day, Song of Light and the night sight potion), being poisoned, and Magic Reflection.
- Being blinded removes the night sight icon. Magic Reflection's icon stays until the reflection is used up and comes back when you log in.

### Player Impact

- You can see at a glance what is protecting you and when it runs out.

## Monsters and Spellcasting

- Spellcasting monsters no longer waste casts. Until now about one cast in every few did nothing, and half of them for monsters that know only one spell. Expect casters to cast more often.
- Monsters that know healing or other helpful spells now cast them on themselves, not on you.
- A monster's area spells no longer hurt other monsters, its own summoned or animated creatures, town folk, guards, vendors or good creatures such as pixies, wisps and unicorns. Its area buffs now only help other monsters; they used to reach the players around it. The same goes for fire and other fields a monster creates.
- A pet's area spells now spare you, your party and your other pets, and only hit players you could hit yourself.

### Player Impact

- Boss and champion fights with spellcasters will feel harder: monsters stop softening each other up for you.
- Area spells cast near town are safer for the townsfolk.

## Bard Songs and Verses

- Fumbling Beastal Bond no longer leaves you frozen and invisible at the creature until you log out. Ending the bond any way, including logging out or a server restart, now puts you back where you started.
- Song of Earth can now be interrupted by taking damage, like the other songs. Its check never worked before.
- Peacemaking now judges each creature by its own difficulty. Before, every creature in range used the difficulty of whichever one was checked last.

### Player Impact

- Beastal Bond is safe to use again.
- Peacemaking a mixed group is fairer: easy creatures calm down easily even next to a hard one.

## Townsfolk and Tracking

- Town folk, guards, merchants, healers, nobles, player vendors and Warriors for Hire are now their own creature type, Townsfolk, instead of Human. Tracking lists them under a new Townsfolk heading.
- Human slayer weapons and Human protection no longer count against them. They still work against human monsters and players.
- Bandages still use Healing on them.
- Town NPCs already in the world change over when they next respawn.

### Player Impact

- Tracking for humans now finds the human monsters, not every shopkeeper in town.
- A Human slayer won't help you against a guard or a Warrior for Hire.

## Guards and Areas

- Guards called on a criminal now record the right player's account. They looked it up by character name, so two characters with the same name could get the wrong account blamed.
- Checking which area rules apply where you stand (safe zones, no-PK areas, guarded towns, forbidden areas) is now much cheaper for the server. Every monster, spell and skill gain does this check, so the shard should run lighter under load.
- Recalling or gating onto a courtyard tile of a house without a house sign footprint is now checked against that house.

### Player Impact

- No direct gameplay change expected from the area work, apart from a smoother server.

## Doors

- Two elven door entries shared their names with two other doors. They now have names of their own.

### Player Impact

- No direct gameplay change expected.

## Staff Tools

- Administrators have a new `.phadmin` window. It starts a server-wide powerhour of a chosen type (always after the usual 2-minute warning), extends, shortens or ends the one that is running, and changes the day and time of the weekly powerhour. A new end time is announced to everyone.
- `.resetph` now also clears a personal powerhour's saved resume state, so a reset hour can't come back at the player's next login.
- `.speedwalk` (Seer) now only accepts 0 to 4. Anything else used to switch it off, and a negative number was saved and sent again at every login.
- `.setwfhdamage` refuses anything that is not a number from 0.1 to 10. A typo used to set a multiplier to 0, and a negative one made warriors heal bosses.
- `.phadmin`'s window is laid out properly: title, panels and buttons line up with the frame.
- Staff can say "<name> status" to any warrior to see its status window. Testers have `.maxwfh`, which maxes a warrior's stats and fighting skills.
- `.areas` Save only rewrites the areas you changed.
- Developers have `.classbonusinfo`. It targets a character and lists their classes, primary class, stat multipliers and every skill that carries a class bonus.
- Administrators have `.arcomagenpcs`. It places, lists, removes or respawns all the Arcomage card players at once, touching no other spawns, and `.arcomagenpcs here <n>` prints a ready-made spawn entry for where you stand. Testers have `.arcomagelineup`, which spawns every card player in a row to look them over.

- Using `.setclass` on an NPC could stop the world from saving until the next restart. It now only accepts players, and no staff log is ever written for an NPC. A new staff member's first logged command is no longer lost.
- `.newmobedit` (Administrator) gained pages for NPCs: AI and loot (script, title, lootgroup, magic item level and chance, whether worn or backpack items drop), flags (boss tiers, champion, tame, snoop, steal, provoke and peace difficulty, where 0 means it can't be done), a custom HP ceiling, HP, mana and stamina regeneration, and a spell list picker. Closing it with a right-click now cancels; it used to set eight warrior skills to 0. The Bard level buttons set bard skills (they set crafter skills), level presets reach 150 exactly, and magic immunity and reflection edits now take effect.
- `.iteminfo` (Game Master): setting a spell hitscript now works and names the item; the use-script picker uses the entry you click and shows its description; special hitscripts say there are none; the console is quiet again.
- `.npclist` (Seer) now groups every NPC template by creature type, with its script, lootgroup and magic item chance and level, 20 to a page.
- `.info` (Seer) shows an active Dragon Skin, Sanctuary or Antidote next to physical protection and poison immunity.

### Player Impact

- Powerhours can now be run, re-timed and rescheduled without a server restart.
- No more lost world saves from a staff command on an NPC.

## Summary

- Server-wide powerhours survive restarts and end on time. A weekly powerhour crowded out by another one, or by downtime, starts later instead of being lost.
- The bonus second powerhour is 50% likely after a half-resources week and 10% after the others, and only the weekly powerhour can earn one.
- `.ph` shows time left on the server-wide powerhour, warns when one is about to start, and has an accurate Sunday countdown.
- Personal powerhours end on time after a relog, survive more than one restart, and are no longer cut short by an old timer after a reset; the weekly lock lifts at Sunday 00:00 GMT and stays lifted.
- Powerplayers no longer get the per-skill class bonus on any skill; their bonus on every skill now runs from 1.1 times at level 2 to 1.5 times at level 6.
- Characters with two classes are judged by their stronger one, for gear and for stat growth.
- Class effects use your last class check, refreshed at login, on `.showclasse` and every 10 minutes; combat, spells and tooltips no longer re-check your gear each time.
- Every character on a house owner's account has full owner rights in that house and cannot be banned or ejected from it; houses can move between your own characters at the house limit; co-owners and friends can set traps.
- Two elven doors got names of their own.
- Warriors for Hire: ten revivals in all, only the newest heart works and only from its master's hand, pre-patch hearts are void, the same stat loss by heart or by memory, no permanent blessings, a High Priest that keeps working, gear kept on release or a guard kill, "stop" that means stand still, every bow, one potion at a time, half damage both ways when sparring, and hits, mana and stamina that follow their stats like a player's.
- The Eon-Prism lets you start another personal powerhour this week, used from your backpack.
- Mayors can resign and leave their town from the townstone.
- The vanity shop sells Transcription Scrolls that copy a runebook (5 tokens) or Runic Atlas (10 tokens) into an empty book of the same kind.
- Fish steaks follow where the fish lives (shore 1, deep water 4, dungeon 6), with the Rare and Legendary bonus also when you carve as you fish; ordinary fish stack again and rare ones never do.
- Fire Arrows, Ice Arrows and Thunder Bolts need 40 less Bowcraft (74, 79 and 84).
- Arcomage: 24 card players in taverns, difficulty 30 to 150, daily gold for your first win at each difficulty, and `.arcomage` for your record and the leaderboard. Challenge other players with the Arcomage Deck (vanity shop), Timed or Untimed, and watch their games with `.arcomage watch`.
- Sanctuary, Antidote and Dragon Skin: level and 2 to 10 minutes by class level, Antidote protects its target, the higher of gear and spell counts, Dragon Skin is one play, and Dispel removes them; leftover bonuses are cleared at login.
- New buff icons for poison immunity, Dragon Skin, Reactive Armor, night sight, poison and Magic Reflection.
- Spellcasting monsters lose no casts, buff themselves, and their area spells spare other monsters, their minions and town folk; their area buffs no longer reach players.
- Beastal Bond fumbles and logouts return you to your spot; Song of Earth can be interrupted; peacemaking judges each creature on its own.
- Town NPCs are Townsfolk: their own Tracking heading, and Human slayers no longer count against them.
- Guards record the right account; area checks are much cheaper for the server.
- Staff: `.phadmin` runs and reschedules server-wide powerhours; `.resetph` clears saved resume state; `.arcomagenpcs` places and removes the card players; developers have `.classbonusinfo`; `.setclass` on an NPC no longer breaks world saves; `.newmobedit` has NPC pages, `.iteminfo` is fixed, `.npclist` is rebuilt and `.info` shows spell protection.

Thanks for playing Zuluhotel Omega 3.
