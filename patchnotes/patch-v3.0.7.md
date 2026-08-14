# Patch Notes - v3.0.7
**Zuluhotel Omega 3 | Beta Shard**

**Date: [August 13, 2026]**

---

## What Changed

Patch 3.0.7 is a guilds patch: guilds can now place a guildstone at their guild house (single-graphic or, for Tower-sized-or-larger houses, a multi-piece statue set), and guild leaders can grant specific ranks secure/teleporter/lockdown/house-manager access to the guild house without adding every member as a personal friend. The Membership gump got a layout fix and a bigger page size, a guild-master-only screen is now actually restricted to the guild master, and a bug that silently blocked static house owners from locking down or securing their own belongings is fixed. It also fixes custom houses not properly releasing locked-down/secured items when redeeded, relinquished, or decayed, adds per-currency balance requests and a Help command to bankers and high priests, reworks `.move` (plus two new commands, `.movebag` and `.movebankcoin`) to consolidate moved stacks instead of leaving one stack per item, and fixes a tooltip issue with Vendor Storage Bag items.

## Guild Stones

- Guilds can now place a guildstone at their guild house, choosing from a catalog of single-graphic tombstones/statues or, if the house is Tower-sized or larger, a multi-piece statue set.
- The guildstone (and all its pieces, for statue sets) is automatically removed if the guild disbands or changes/loses its guild house.
- Fixed statue sets being placeable in a way that could block a door; every piece is now checked, not just the first one.

### Player Impact

- You can give your guild a visual landmark at its house.
- You don't need to manually clean up a guildstone when your guild's house situation changes — it's handled for you.
- You can no longer accidentally (or intentionally) wedge a statue-set guildstone against a door.

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

## Miscellaneous

- Fixed item tooltips inside a Vendor Storage Bag showing the plural item description instead of the singular one.

### Player Impact

- Items in a Vendor Storage Bag now show a correctly worded name/count in their tooltip.

## Behind the Scenes

- Consolidated duplicated house-sign lookup code shared by guilds, housing, and custom housing.
- Migrated custom housing's internal sign/house link to match the same property naming classic and static housing already use, with automatic backward-compatible migration for existing houses.
- Started cleaning up how a house's Omega Cache storage record is released when the house is permanently torn down.

### Player Impact

- No direct gameplay change expected from this section — it's internal cleanup and consistency work.

## Summary

- Added guildstones (single-graphic or multi-piece statue sets for large houses), auto-removed on disband/house change, with door-blocking placement fixed.
- Reworked the Membership gump (wider, 15/page, no overlap) and added a new Guild House Management screen for granting ranks secure/teleporter/lockdown/house-manager access to the guild house.
- Closed a security gap that let the guild-master-only screen be reached without actually being the guild master.
- Fixed static house owners being blocked from locking down/securing their own items, custom houses not releasing locked-down/secured/display items on teardown, and a transferred custom house not being selectable as a guild house.
- Removed the 24-hour guild colour cooldown for staff.
- Added per-currency balance speech commands and a Help command to bankers and high priests, plus a combined-total column on the balance gump.
- Reworked `.move` to consolidate stacks, and added `.movebag`/`.movebankcoin` for the same behavior elsewhere.
- Fixed Vendor Storage Bag tooltips showing plural item descriptions.
- Various internal housing/guild code consolidation with no direct gameplay effect.

Thanks for playing Zuluhotel Omega 3.
