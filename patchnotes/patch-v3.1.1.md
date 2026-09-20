# Patch Notes - v3.1.1
**Zuluhotel Omega 3 | Beta Shard**

**Date: September 20, 2026**

---

## What Changed

Patch 3.1.1 finishes off Throwing weapons with a full set of enchanted variants, adds new fishing and ritual quest lines, brings three new crafting stations (Salvage Station, Cooper's Bench/Artificer's Toolbox, Potion Vat), reworks Poisoning into a self-sufficient skill, gives Warrior for Hire a safety net against permanent loss, and includes a large batch of fixes across combat, equipment, house security, and skill caps.

## Throwing Weapons Completed

- All 12 Throwing weapons now have Mystical, Swift, and Stygian enchanted variants (36 new items total), and they now actually drop from monster loot at the appropriate tiers.
- New Throwing-skill characters now start with a working throwing dagger.

### Player Impact

- Throwing is now a complete weapon line with the same enchanted-tier variety as other weapon skills.
- Starting a new Throwing-focused character no longer leaves you without a weapon.

## New: Fishing Content

- New tiered **lures** can be attached to a fishing pole or a deployed crustacean trap for a bonus effect. Loading a new lure replaces the old one (including its leftover charges).
- Rare and Legendary catches can now be mounted as **taxidermy trophies**.
- Carving a fish now yields more steaks the better the catch — shore catches give 1, deep-water catches give 4, Rare doubles that, and Legendary multiplies it by 10 (previously always exactly 4 regardless of catch quality).
- Two new quest-giver NPCs — **Fish Monger** and **Lobsterman** — offer new fishing and crustacean-catching quests (four quests total).
- The Bulk Order Deed reward catalog has grown substantially, with many new items to redeem points for.

### Player Impact

- More depth and reward for fishing, including a reason to chase Rare/Legendary catches beyond their sell value.
- New quest content for fishing-focused characters.
- Bigger BOD reward selection.

## New: Ritual Quest Lines

- Two new ritual quest-givers: **Frostkeeper Ysolde** (in Winterwyn's Everfrost Catacombs) and **Nystul the Enchanter**, each offering a 3-quest line (six quests total) that ends in summoning and defeating a boss for a new ritual reward.

### Player Impact

- Six new completable quests and two new rituals to earn through them.

## New Crafting Stations

- **Artificer's Toolbox** and **Cooper's Bench**: assemble multiple distinct parts (like barrel staves, a lid, and hoops) into a finished item in one gump — the first use of this is building a Closed Barrel.
- **Salvage Station**: a universal station that bulk-melts or unstitches items back into raw material across Blacksmithy, Tailoring, Bowcraft, Carpentry, and Tinkering. Supports "Salvage All" and "Salvage All of Type," and defaults to protecting exceptional items from being accidentally destroyed in a bulk pass.
- **Potion Vat**: a larger-capacity version of the Potion Keg, using the same fill/empty/transfer interface.
- The Large Forge's bellows animation now actually plays while crafting at it (it was silently broken before).

### Player Impact

- A faster, safer way to recover materials from unwanted crafted items in bulk.
- New assembly-style crafting for barrel-based items.
- A bigger potion storage option.

## Poisoning Rework

- Two new items make Poisoning self-sufficient: the **Vial of Venom** applies poison directly to a weapon, food, or bandage, and the **Toxin Flask** brews poison potions from Nightshade using the Poisoning skill itself.

### Player Impact

- Poisoning characters no longer need to lean on Alchemy just to get poison.

## Carpentry & Furniture

- Roughly 190 more placed furniture and crafting-station items can now be picked back up with `.redeed` (previously they were stuck in place once placed).
- Several Loom tiles that were dead, unclickable art are now fully usable (both facings and their side pieces).
- Carpentry's crafting-gump categories were reorganized for clarity (same recipes and costs, just better organized).

### Player Impact

- Far fewer "I can't pick this back up" furniture placements.
- Looms work correctly no matter which tile you click.

## Warrior for Hire

- Losing a hired warrior's gear no longer means it's gone for good — items now go into a recoverable 90-day escrow instead of vanishing.
- Resurrection attempts before a warrior is lost permanently increased from 4 to 11.
- New: if your Warrior for Hire is permanently lost, the **High Priest** can fully resurrect it (stats, skills, name, and gender restored) for 10,000 gold.

### Player Impact

- Warrior for Hire is much more forgiving — both gear and the warrior itself are now recoverable instead of being permanent losses.

## Hunger System Rebalance

- Hunger now drains gradually across a wider scale instead of hitting all at once: stamina starts draining first, then mana, then health as hunger gets progressively worse.
- The `.hungry` status message now reports each of these stages accurately.

### Player Impact

- Going hungry is a gradual, multi-stage experience instead of one sudden hit once you hit the old ceiling.

## Skill Caps, Power Scrolls & Classes

- Power scrolls can no longer roll a non-functional "dead" skill and waste the drop.
- Throwing and Alchemy power scroll/transcendence scroll caps now track correctly (they could previously fail to apply properly).
- `.dropskills` now actually resets Throwing along with every other skill.
- The periodic background stat/skill-cap check no longer errors out, and now correctly enforces the Throwing skill cap it was previously skipping.
- New player command **`.classinfo`**: see exactly how close you are to your class's next skill-level tier, or how many points you'd need to drop to change tiers.

### Player Impact

- Power scroll drops are never wasted on a skill that doesn't exist.
- Throwing and Alchemy skill caps behave correctly and get properly enforced.
- A new self-service way to check your class progress.

## Combat & Equipment Fixes

- Dispel, Mass Dispel, and Banish effects (on-hit, Banish, and Blackrock) now reliably finish off extremely high-HP targets instead of potentially leaving them undamaged.
- Fixed a bug that could let two items occupy the same equipment slot at once (e.g. two one-handed weapons equipped simultaneously).
- The two Vampire NPC costumes now show their shirt with the correct dye color.
- Reconnecting no longer re-shows the MOTD window every single time.

### Player Impact

- Dispel-type effects work reliably against any target, no matter how much HP it has.
- No more double-equipped weapon slots.
- Reconnecting is less annoying.

## House Security

- Closed a gap where a banned player who got inside a house by any means other than physically walking over its ban tile (for example, a house teleporter) could retain secure-container, lockdown, and house-management access.

### Player Impact

- Banned players can no longer keep house access through a side entrance.

## Other Fixes

- Corpses now correctly display the clothing the character was wearing at death.
- Weather, season, and lighting now resync correctly after logging in, reconnecting, dying, or crossing realms by boat (previously could desync).
- Server saves now cause noticeably shorter freezes.
- The Omega Cache's category menu now actually displays categories in the intended order instead of always alphabetically, and page-navigation buttons no longer shift position between pages.
- Grandmaster/Elder-tier crafters now have a 50/50 chance to save an item at its last hit point during repair instead of it always breaking.

### Player Impact

- More visual polish and fewer desyncs around corpses, weather, and lighting.
- Shorter pauses during server saves.
- The Omega Cache menu behaves the way its category order was always meant to.
- High-level crafters get a fighting chance to save a nearly-broken item on repair.

## Summary

- Completed the Throwing weapon line with 36 enchanted variants and fixed loot drops/starting gear.
- Added new fishing content: lures, taxidermy trophies, quality-based fish carving, two new fishing quest-givers, and an expanded Bulk Order Deed reward list.
- Added two new ritual quest lines (six quests, two new rituals).
- Added three new crafting stations: Artificer's Toolbox/Cooper's Bench, Salvage Station, and Potion Vat.
- Reworked Poisoning into a self-sufficient skill with two new items.
- Fixed ~190 furniture items that couldn't be redeeded, and repaired dead Loom tiles.
- Gave Warrior for Hire a recoverable item escrow and a way to fully resurrect a permanently lost warrior.
- Rebalanced hunger to drain gradually across stamina, mana, and health instead of all at once.
- Fixed power scroll/transcendence scroll cap tracking for Throwing and Alchemy, and added the `.classinfo` command.
- Fixed Dispel/Banish effects failing against extremely high-HP targets, a double-equip bug, and a house-ban access gap.
- Polished corpse clothing display, weather/lighting resync, server-save speed, Omega Cache ordering, and gave high-level crafters a repair save-chance.

Thanks for playing Zuluhotel Omega 3.
