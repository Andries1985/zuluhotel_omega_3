# Patch Notes - v3.0.7
**Zuluhotel Omega 3 | Beta Shard**

**Date: [August 16, 2026]**

---

## What Changed

Patch 3.0.7 combines a guilds update with a full fishing overhaul. Guilds can now place a guildstone at their guild house (single-graphic or, for Tower-sized-or-larger houses, a multi-piece statue set), and guild leaders can grant specific ranks secure/teleporter/lockdown/house-manager access to the guild house without adding every member as a personal friend. The Membership gump got a layout fix and a bigger page size, a guild-master-only screen is now actually restricted to the guild master, and a bug that silently blocked static house owners from locking down or securing their own belongings is fixed. It also fixes custom houses not properly releasing locked-down/secured items when redeeded, relinquished, or decayed, adds per-currency balance requests and a Help command to bankers and high priests, reworks `.move` (plus two new commands, `.movebag` and `.movebankcoin`) to consolidate moved stacks instead of leaving one stack per item, and fixes a tooltip issue with Vendor Storage Bag items.

Fishing has also been reworked from the ground up: every catch now rolls a Regular/Rare/Legendary tier across 69 different fish species, and a brand-new crab/lobster trapping minigame has been added. Treasure hunting got new chest visuals that vary by map difficulty, a bonus "bardic intuition" chance for Bards, and dig sites relocated to the realms you can actually reach. On top of that: a broad pass of item/effect/NPC color corrections, a fix so stolen stackable items merge into your existing stacks instead of cluttering your backpack, a handful of previously-inert door types now working, and a guildstone left in a redeeded custom house no longer being orphaned.

## Fishing & Crustacean Trapping

- Fishing with a pole or net now rolls a catch tier — Regular, Rare, or Legendary — across 69 different fish species. Rare and Legendary catches stand out with a colored name in your catch message and in the item's tooltip.
- Net fishing has noticeably better odds at Rare/Legendary catches than pole fishing.
- Deep water, shoreline, and dungeon fishing now behave differently: deep water and dungeons give better skill gain, while fishing in safe/guarded areas gives less.
- Fixed fishing spots sometimes claiming there was nothing to catch even when fish were available. A spot that's genuinely fished out now tells you clearly and asks you to wait for it to replenish.
- New command `.togglerarecarve` — Rare and Legendary catches are now protected from being automatically carved into fish steaks by an equipped blade. Use this command if you'd rather go back to auto-carving everything.
- Carving a fish caught in shallow water now yields 1 steak instead of 4 (deep-water catches are unchanged at 4).
- New: **Crustacean Trapping**. Place a crustacean trap in the water and it becomes a floating buoy — check back periodically to find crabs and lobsters waiting inside (their own Regular/Rare/Legendary tiers), up to 5 catches per trap. An unattended or unlucky trap can wash away or come up empty, so don't leave it too long. You can have up to 5 traps active at once.
- Fixed dry-docking a boat sometimes failing to destroy its key if the key wasn't sitting in your backpack — keys stored in a bank box or secure container are now found and destroyed too.
- New fishing/trapping gear (poles, nets, hooks, traps, buoys) can no longer be re-dyed at a dye tub.

### Player Impact

- Fishing is more rewarding and varied, with visible Rare/Legendary catches worth watching for.
- Where and how you fish now meaningfully changes your catch odds and skill gain.
- No more confusing "nothing here" messages at a spot that actually has fish.
- You can protect valuable catches from being accidentally turned into steaks.
- A brand-new crab/lobster trapping activity to run alongside regular fishing.
- Boats can always be properly dry-docked, no matter where you kept the key.

## Treasure Hunting

- Treasure chests dug up from treasure maps now look different depending on the map's difficulty level, with progressively fancier chest styles at higher levels.
- Bard-class characters have a chance, when digging a level-6 map, to trigger "bardic intuition" and unearth a bonus, higher-tier chest guarded by tougher guardians.
- Treasure map dig sites have been moved off the unused Britannia facet onto the realms you can actually play in — Sosaria, Ilshenar, Tokuno, and Malas all now have dig sites.

### Player Impact

- Treasure chests are visually more interesting and telegraph the map's difficulty at a glance.
- Bards get an extra reason to dig level-6 maps.
- Every treasure map you find should now have dig sites you can actually reach.

## Guild Stones

- Guilds can now place a guildstone at their guild house, choosing from a catalog of single-graphic tombstones/statues or, if the house is Tower-sized or larger, a multi-piece statue set.
- The guildstone (and all its pieces, for statue sets) is automatically removed if the guild disbands or changes/loses its guild house.
- Fixed statue sets being placeable in a way that could block a door; every piece is now checked, not just the first one.
- Fixed a guildstone left inside a custom house not being removed when that house was redeeded — it now disappears along with the house. (This fix currently only covers custom-built houses; classic and static houses aren't covered yet.)

### Player Impact

- You can give your guild a visual landmark at its house.
- You don't need to manually clean up a guildstone when your guild's house situation changes — it's handled for you.
- You can no longer accidentally (or intentionally) wedge a statue-set guildstone against a door.
- Redeeding a custom house that had a guildstone in it no longer leaves the stone behind.

## Guild Membership & House Permissions

- The Membership gump is wider, shows 15 members per page instead of fewer, and no longer has overlapping column headers or button text. It now warns you that unsaved rank changes are lost if you flip pages.
- New: guild leaders can open a "Guild Management" screen and grant each rank (Officer, Veteran, Member, Recruit) any combination of secure container access, recall/gate teleporter access, lockdown access, and full house-manager access (friends list, bans, ejects, and the House Management tool) — all scoped to the guild's own house.
- These rank-based grants work alongside your existing friends-list permissions on the house; either one is enough.
- The guild leader always has full access to everything and doesn't need to be configured.
- Fixed a security gap where the guild-master-only screen (disband, change guild master/house, guildstone removal, and the new house permissions screen) wasn't actually restricted to the guild master server-side.

### Player Impact

- The Membership gump is easier to read and shows more members at once.
- You can give guild ranks real access to your guild house without friending every single member individually.
- Only the actual guild master can reach guild-master-only actions now.

## Housing Fixes

- Fixed static house owners being silently blocked from locking down, securing, displaying, raising, or lowering items in their own house.
- Fixed custom houses not actually releasing locked-down, secured, or on-display items when the house is redeeded, relinquished, or decays — affected items were left stuck immovable instead of being freed.
- Fixed a custom house that changed owners through the in-game ownership transfer not being selectable as a guild house by its new owner.
- The 24-hour guild colour change cooldown no longer applies to staff.

### Player Impact

- You can once again lock down and secure items in your own static house.
- Locked-down/secured/display items in a custom house are properly released instead of getting stuck when the house is redeeded, relinquished, or decays.
- A custom house you received via ownership transfer can now be set as your guild's house.

## Banker & High Priest Speech Improvements

- Bankers understand `"balance gold"`, `"balance silver"`, and `"balance copper"` (and the vault equivalents) — they'll tell you that currency's total out loud without opening the balance gump.
- The balance gump itself now shows a combined coin+cheque total for each currency, not just the separate coin and cheque counts.
- Both bankers and high priests respond to `"help"` with a list of every speech command they understand.

### Player Impact

- You can quickly check one currency's balance without opening a gump.
- The balance gump is easier to read at a glance.
- Bankers and high priests will remind you what you can say to them.

## Move Command Improvements

- `.move all` now consolidates moved items into full stacks (up to 60,000) at the destination instead of leaving each item as its own separate stack.
- New command `.movebag` — empty one bag in your backpack into another, consolidating stacks the same way.
- New command `.movebankcoin` — move gold, silver, or copper between containers in your bank box or vault, consolidating stacks the same way.

### Player Impact

- Moving a large number of items leaves you with far fewer, fuller stacks to deal with afterward.
- Two new convenience commands for bag-to-bag and bank/vault currency organization.

## World Fixes

- Several previously-inert door types now open and close correctly: bar doors, an alternate moon door set, crystal-wall doors, shadow doors, and two stone wall-door variants.
- This is a partial fix — most of the previously identified broken door types, including several player-built castles and keeps, are still affected and will be addressed in a future patch.

### Player Impact

- A handful of specific door types across the shard now function instead of being purely decorative.

## Snooping & Stealing

- Fixed stealing a stackable item (reagents, ingots, arrows, etc.) always creating a brand-new stack in your backpack instead of merging into a matching stack you already had.

### Player Impact

- Stolen stackable items now merge into your existing stacks instead of cluttering your backpack with duplicates.

## Visual & Sound Corrections

- A large number of items, effects, and NPCs across the shard have had incorrect hue (color) values corrected to their intended color — this touches armor/weapon dyes, PvP arena decorations, champion spawn altars, elemental spell effects, and more.
- Several mounts and ethereal creatures now play a correct, distinct death sound instead of a shared generic placeholder sound.

### Player Impact

- Many items and effects will now look like a different (correct) color than before — this is intentional, not a bug.
- Mounts sound right when they die instead of all sharing the same generic sound.

## Miscellaneous

- Fixed item tooltips inside a Vendor Storage Bag showing the plural item description instead of the singular one.
- Selling a full backpack to a vendor (sell-all) is noticeably faster now.

### Player Impact

- Items in a Vendor Storage Bag now show a correctly worded name/count in their tooltip.
- Bulk-selling to a vendor no longer pauses between each item.

## Behind the Scenes

- Consolidated duplicated house-sign lookup code shared by guilds, housing, and custom housing.
- Migrated custom housing's internal sign/house link to match the same property naming classic and static housing already use, with automatic backward-compatible migration for existing houses.
- Started cleaning up how a house's Omega Cache storage record is released when the house is permanently torn down.
- A batch of underlying engine (core) fixes landed alongside this patch: several rare server-crash and shutdown bugs are fixed, corpses now reliably show a dead character's equipment (including right after a server restart), and container "slot" behavior on items that use it now actually persists and works correctly.
- Fixed a long-standing visual bug where a character killed while riding a mount could stay stuck in the mounted sitting pose (and keep mount movement speed) as a ghost, through resurrection, until mounting and dismounting an animal again.

### Player Impact

- Most of this section has no direct gameplay change — it's internal cleanup and consistency work — but the engine fixes should mean fewer rare crashes, corpses that consistently show what a character was wearing when they died, and no more getting stuck looking (and moving) like you're still mounted after dying on one.

## Summary

- Reworked fishing with a Regular/Rare/Legendary catch-tier system across 69 fish species, fixed a misleading "nothing here" message, and added `.togglerarecarve` to protect valuable catches from auto-carving.
- Added Crustacean Trapping — a new crab/lobster trapping minigame using placeable traps and buoys.
- Fixed boats not always destroying their key on dry-dock if the key wasn't in the owner's backpack.
- Treasure chests now vary in appearance by map level, Bards get a bonus "bardic intuition" chance on level-6 maps, and dig sites were relocated to the realms players can actually reach.
- Added guildstones (single-graphic or multi-piece statue sets for large houses), auto-removed on disband/house change, with door-blocking placement fixed, and a redeeded custom house no longer orphans its guildstone.
- Reworked the Membership gump (wider, 15/page, no overlap) and added a new Guild House Management screen for granting ranks secure/teleporter/lockdown/house-manager access to the guild house.
- Closed a security gap that let the guild-master-only screen be reached without actually being the guild master.
- Fixed static house owners being blocked from locking down/securing their own items, custom houses not releasing locked-down/secured/display items on teardown, and a transferred custom house not being selectable as a guild house.
- Removed the 24-hour guild colour cooldown for staff.
- Fixed stolen stackable items always creating a new stack instead of merging into an existing one.
- Fixed a handful of previously-inert door types (bar doors, alt moon doors, crystal-wall doors, shadow doors, two wall-door variants); most of the broader known door issue remains for a future patch.
- Corrected a broad set of item/effect/NPC colors and several mount/ethereal death sounds.
- Added per-currency balance speech commands and a Help command to bankers and high priests, plus a combined-total column on the balance gump.
- Reworked `.move` to consolidate stacks, and added `.movebag`/`.movebankcoin` for the same behavior elsewhere.
- Fixed Vendor Storage Bag tooltips showing plural item descriptions, and sped up vendor bulk-selling.
- Fixed a long-standing bug where dying while mounted could leave a character visually stuck in the mounted pose/speed as a ghost until mounting and dismounting again.
- Various internal housing/guild code consolidation and underlying engine stability fixes with no direct gameplay effect (beyond fewer rare crashes and more reliable corpse equipment display).

Thanks for playing Zuluhotel Omega 3.
