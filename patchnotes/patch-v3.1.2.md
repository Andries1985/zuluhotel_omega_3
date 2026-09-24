# Patch Notes - v3.1.2
**Zuluhotel Omega 3 | Beta Shard**

**Date: September 24, 2026**

---

## What Changed

Patch 3.1.2 is a fix patch. Every script on the shard was read line by line and about 170 bugs were fixed: crafting that charged twice or not at all, spellbooks that locked you out until relog, summons and gates that survived restarts, house and boat security holes, Omega Cache and vendor money leaks, spawned monsters that wandered off, and a long tail of smaller things. Hunger, which never actually drained anything before, now works. There are no new systems in this patch; the point is that the existing ones behave.

---

## Crafting

- Every craft now takes your materials first and makes the item second, and checks both steps. Moving materials out of your pack while the "how many" prompt was open no longer makes the batch for free, and a full backpack no longer burns ingots on Make Max while claiming success.
- Crafting Power Hour half-cost now rounds up instead of down. Recipes that cost 1 material were free during a Crafting Power Hour (45 cooking recipes, 18 tinkering recipes, single arrows and bolts); they now cost 1. Odd costs pay one more unit than before (25 becomes 13, not 12).
- Alchemy at a placed station actually makes the potion now. Before, the reagents, bottle and skill roll were spent and nothing appeared.
- Carpentry no longer fails with a false "Your backpack is full" on roughly one craft in fifteen for recipes that can be exceptional; a failed exceptional roll makes the normal item instead.
- Crafting a runebook costs a blank rune again (it had stopped charging one).
- Bulk crafting is capped at 60,000 per batch; larger batches used to charge you and make nothing.
- **Autoloom:** looms placed under 3.1.1 keep working and convert automatically. East-facing looms show East art after their first use (both facings showed the South view before). A loom no longer sticks open or on the wrong art after a disconnect, and the weaving animation plays during each craft. Bandages made at the loom respect the count you ask for (they used to convert every cloth you owned, Omega Cache included).
- Item Identification has a 10 second delay between uses again below 100 skill.
- Alchemy Plus: True Mage bonus-tier Tamla Heal potions count as two base Tamlas in any recipe that asks for a base Tamla, so a bonus Tamla can go straight into a Rebirth Potion.

### Player Impact

- No free crafts, no double charges, no lost materials on a full pack.
- Station alchemy and runebook crafting work as intended; Power Hour half-cost costs at least one unit.
- Autolooms look right and stay in sync.

## Hunger Now Works

- Hunger drain never actually ran before: the drain script was being handed the wrong thing and stopped immediately. It runs now, and it survives relogs. Hunger climbs one step per hour online. From 10, stamina drains; from 12, mana too; from 14, health too. Eating brings it down, and at 14 your character automatically eats cooked food from the backpack.
- A logged-in character's hunger is clamped at 15 on login as intended (that clamp was a no-op before).

### Player Impact

- Carry food. Going hungry has real consequences for the first time, in the gradual stages 3.1.1 described.

## Spells, Spellbooks and Songs

- **No more "You are already casting something!" until relog.** A cancelled `.cast`, an Earth Portal you could not open, a spell that failed to start, a verse that ended on its own, and any ritual all left you locked out of every spellbook and scroll. All of those clear the flag now.
- Summoned creatures (all summons, Blade Spirit, Energy Vortex, Song of Beckon's fairy) no longer survive a server restart as permanent pets.
- Gate Travel pairs, Earth Portal's far gate and Angelic Gate's Gate of Life no longer survive a restart as permanent gates.
- Area spells no longer hit the caster (Rising Fire, Apocalypse, Wraith's Breath, Abyssal Flame, Plague, Chain Lightning). Bard area effects (Sonic Disturbance, Bardic Boulders, Corpse Distention, Spirit Flock) no longer reach people standing in a safe area.
- Bard boosts (Life Balance, Lesser Healing, Shadows, Dragon Skin, the songs of Glory, Haste, Defense, Life, Remedy, Light and Cloaking) only land on real party members now. Before, anyone with no party at all counted as everyone's party-mate.
- Dragon Skin's protection points are always taken back when it ends, including after a fumble or a restart. Characters carrying leaked points lose them at their next login.
- Antidote and Sanctuary poison immunity no longer becomes permanent if a restart lands inside its duration.
- Song of Sirens no longer paralyzes for free when you are out of stamina. Song of Cloaking only reveals the people it cloaked. Wrath of God's reflected hit no longer counts the target as your attacker.
- Song of Salvation and Revive no longer work on bosses; Salvation now gives proper kill credit on ordinary undead.
- Healing spells and Resurrection no longer damage Liche-form players in no-PK or safe areas.
- Mind Blast hits players 40% softer and now gives kill credit. Poison damage on players is scaled to 60% (it had no scaling at all).
- Dispel works on very high-HP summons; Mass Dispel keeps going past an immune target; Dispel Field works on every map.
- Reactive Armor and Invisibility timers can no longer be confused by a second cast; a skill hide ends a lingering Invisibility timer. Cure and Invisibility no longer reflect.
- Mark no longer consumes your blank rune when it fails.
- **Classic magic wands work for everyone now:** a charge is the whole cost, with no mana, reagents, skill roll or circle requirement.
- Necromancers: the self-burn on a fumbled Kill, Liche or Sorcerer's Bane deals damage again.
- Spellbooks, the Codex, the song book and the verse book now refuse spell selections you have not learned even if the request is forged.

### Player Impact

- Casting lockouts are gone.
- Restarts no longer leave permanent summons or gates behind.
- Bard support songs need a party; solo casters stop hurting themselves.
- Wands are a real option for any character.

## Combat & Weapons

- Melee weapons with no miss sound (Pickaxe, the ghost and bat weapons and the items below) could never miss. They can now.
- Fourteen weapons defined outside the combat package (the eight astral weapons, shepherd's crook, young oak staff and a few others) had no swing animation, never missed and were stuck at 1-1 damage. They deal their real damage now; the astral Black Staff goes from 1-1 to 5-21.
- Town NPCs (merchants, bankers, shrines and the other 64 town templates) could not hurt anyone and spammed staff with an error on every swing. They hit for their template damage now.
- Paralyzing, poisoning and spell-strike weapon effects respect no-PK and safe areas.
- A poisoned weapon's last poison charge no longer eats the swing. A cursed poisoned weapon only backfires its poison, not the whole hit.
- A skill you have set to "down" keeps working while it drains (it used to fail every check).
- Astral weapon mana and stamina drain works. Weapon tooltips show truer damage numbers and a corrected DPS (roughly half the old figure for a 100-Dex character; the tooltip only, not the damage).

### Player Impact

- Some weapons that were free hits or dead weight are real weapons now.
- Town NPCs fight back.

## Housing, Boats and Containers

- Only the captain (owner, crew or staff) can give sailing commands or hand maps to the tillerman. Any passenger could before.
- Secure containers are guarded on every access path, not only when opened from the gump. A chest window left open after your access was revoked, or a macro using a known serial, could empty a secure before.
- Custom house doors open with the house key from the first time the owner clicks the sign. "Change the locks" needs a blank key and re-keys every door.
- Demolishing a custom house pays you half of what the house cost (foundation plus customization). It paid nothing before.
- Classic house demolish with a full backpack puts the deed on the ground at your feet instead of losing it. Classic house decay no longer destroys the deed while leaving the house standing.
- Only the owner can transfer or demolish a custom house; transferring clears the old co-owner, friend and ban lists.
- Two people can no longer buy the same static house at once (both were charged before). A static house's decay timer is only refreshed by the owner and co-owners.
- Eject only works on people actually inside your house, and never on staff, vendors, the owner or co-owners.
- Banning refuses staff on all sign types. Traps can only be set on containers within reach that you are allowed to touch; Remove Trap needs line of sight.
- Playing an instrument no longer unlocks it from a house lockdown.

### Player Impact

- Your house, boat and secured chests are only usable by the people you allow.
- Demolishing pays and never loses a deed.

## Gold, Vendors, Omega Cache and Bulk Orders

- Player vendors can no longer be given a negative buy price (which minted gold on purchase).
- Omega Cache deposit: a stack that was in use by another script (a bandage stack mid-heal, ingredients mid-cook, ore at the forge) was credited to the store while staying in your pack. Such stacks are now refused with "That item is in use."
- Omega Cache withdraw no longer pays out units that a craft in progress has reserved.
- Bulk Order Deeds: a Large deed can only consume Smalls in your own backpack; payouts above 60,000 gold are paid in full (overflow to your bank box, then your feet); a stack of items fills as many slots as the order still needs instead of one; you are only paid once the deed is really gone.
- Vanity shop x5/x10 bundles are all-or-nothing when your pack fills mid-purchase (they used to leave half the pieces in your pack and take nothing).
- Random Ancient Tomes are no longer lost to a full backpack.
- Omega Dye, Soul Pen and Runic Dye Tub stop at zero charges instead of carrying on; the Soul Pen refuses stackable items.
- Staff-marked items sell to NPC vendors for nothing.

### Player Impact

- The economy leaks are closed; bulk orders pay what they promise.

## Guilds, Towns and Looting

- Forming a guild no longer spends the 120,000 gold on a house it then refuses (for example a house owned by another character on your account, or one another guild already holds).
- When a guild master leaves or is deleted, the next member becomes master; the departing master could be "promoted" to replace themselves before, and everyone was told they were the new master.
- Recruiting no longer opens the guild list on the recruit; the member list no longer double-lists the master; applicants show correctly.
- Town stones only accept actions from people with the right role: citizens of that town vote, run, call elections and leave; the mayor starts polls; you cannot join a second town. Town populations can no longer go negative.
- Donator mounts work in player-run towns such as Zento, not only the old Britannia cities.
- Looting a corpse of your own guild, an enemy guild or an ally in town no longer flags you criminal or jails you. This exemption existed but never worked.
- Leaving a town no longer mangles your name.

### Player Impact

- Guild and town actions do what the buttons say, and cost only what they should.

## Spawns, Quests and the World

- Monsters from spawn points keep to their spawn point's wander range. That leash was never applied, so spawned monsters drifted freely.
- Spawned monsters no longer occasionally appear with 1 hit point.
- Champion spawn gold now covers the full circle around the champion (it landed on one quadrant); the total per champion is unchanged.
- Quest rewards are created before the quest is marked complete; a full or overweight pack no longer loses them (they go to your feet instead).
- Ritual darkness lifts on its own after two minutes per step and always at the end of the ritual; bystanders were left in the dark until logoff.
- Hatched dragons and ostards count as yours for your own area spells and songs.
- Ter Mur's four teleporters work (they pointed at a realm that does not exist).
- Skill gain stops inside the jail again.
- Guards called on pets judge each pet by its own owner (one criminal owner used to tar every pet scanned after it).
- Santa hands out one present per day, including across restarts.
- Townsfolk talk again and the minstrel's tune reaches everyone nearby. Loke and Thor stop dropping a spare boss weapon. The high priest's forgiveness fine is priced like his other services (it was one coin for classless characters). The casino charges the right bet after a double-down.
- NPC training at merchants and the Water shrine's shell check behave as intended.

### Player Impact

- Spawns behave like spawns; quest and champion rewards arrive in full.

## Skills, Caps and Classes

- The Throwing skill cap from power scrolls is enforced (it read past the end of the cap list and had no cap at all).
- The background cap check no longer crashes.
- `.ph` and `.setph` work for characters who never used a Power Hour.
- Loot stacks double on either hunting Power Hour, not only when both are on.
- Skill title 43 reads Wrestler.
- Class bonuses no longer sweep your paperdoll on every swing and resist roll: a measurable saving in combat. Prohibited items are still stripped when a class is assigned and refused when equipped.

### Player Impact

- Caps are enforced correctly; combat is a little lighter on the server.

## Commands and Accounts

- `.recalltotem` works (it failed silently and still started the 24-hour cooldown).
- Trash cans empty after a restart; `.trashlb` opens instantly.
- `.online` shows each player's real account age. `.disarm` and `.undressme` check the shield hand's curse correctly. `.clearmsglog` no longer drops your first message from the archive or archives an empty log. `.updatetp` refreshes a tooltip. `.msg` shows your first stored message.
- The email commands (`.email`, `.inspectmail`) are disabled.
- Login lockout works: five wrong passwords within three minutes lock the account and address for the configured time (it could be bypassed before). A good login clears the counter.
- Lockpicking has a delay between attempts: 5 seconds after a success, 10 after a failure. The pickpocket dummy trains Stealing to 25.
- Safe-zone and no-PK protection are set correctly the moment you log in or reconnect.

### Player Impact

- Player commands behave; accounts are safer.

## Staff Tools

- A number of staff commands that silently did nothing or the wrong thing were fixed, among them the global no-loot timer (60 minutes lasted 60 hours), thawing NPCs (they re-froze at the next restart), `.makekey`, `.unconcealhim`, `.untile`, `.releaseinfo` and `.maxcaps`, which used to wipe a player's power scroll caps instead of raising them. The obsolete `.makemoongates` was removed; the moongate network is built by the server itself.

### Player Impact

- Fewer "a GM tried to help and it didn't work" moments.

## Server

- The bundled server build is a newer nightly: world load at startup is 20-30% faster and custom-property memory is lower. Weapon and armor can now carry a Damage Increase value and equipment templates can pick colors from ranges; neither is used by shard content yet.

### Player Impact

- Shorter restarts.

## Summary

- Crafting takes materials first and creates second everywhere; Power Hour half-cost rounds up; station alchemy, runebook crafting and the Autoloom work correctly.
- Hunger drains for the first time, in stages, and survives relogs.
- Casting-flag lockouts are gone; summons and temporary gates no longer survive restarts; area spells miss the caster; bard boosts need a party; wands are free to use for any character.
- Fourteen weapons deal real damage, town NPCs fight back, weapon effects respect safe areas.
- Boats, secure containers, custom house keys, demolish payouts, static house sales and eject are locked down.
- Vendor, Omega Cache, bulk order, bundle and charged-item money leaks are closed.
- Guild formation, succession and recruiting, town stone roles, donator mounts in player towns and guild-war looting are fixed.
- Spawned monsters keep their leash and full health; champion gold covers the whole circle; quest rewards are never lost; Ter Mur teleporters work.
- The Throwing cap is enforced, the cap check no longer crashes, class bonuses stop sweeping the paperdoll.
- Login lockout, lockpicking delays, `.recalltotem`, trash cans and several staff tools fixed; a faster server startup.

Thanks for playing Zuluhotel Omega 3.
