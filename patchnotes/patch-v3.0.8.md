# Patch Notes - v3.0.8
**Zuluhotel Omega 3 | Beta Shard**

**Date: [August 20, 2026]**

---

## What Changed

Patch 3.0.8 is primarily a staff-tooling patch: a brand-new Area Spawner system for managing region-based spawns, alongside a batch of world fixes — new teleporter connections in Sosaria, door repairs on the Sosaria decorate facet, a fix for `.speedwalk` not working, six new higher-tier Wyrm reward encounters, several new mounts, and a full overhaul of the galleon ships (Orc, Gargoyle, Tokuno, Britannia) that makes them fully functional and purchasable for the first time.

## World Fixes

- Fixed `.speedwalk` (and the automatic speed-walk restore on login) silently doing nothing (for staff only).
- Added roughly 50 new teleporter connections in Sosaria, opening up several previously unreachable or awkwardly-linked cave and dungeon areas, and cleaned up the Water Dungeon's level-to-level connections so they're no longer ambiguous.
- Fixed a large number of non-functional doors on the Sosaria decorate facet.

### Player Impact

- More of Sosaria's dungeons and caves are reachable by teleporter than before.
- Doors on the Sosaria decorate facet that were previously purely decorative should now open and close correctly.

## New Content

- Six new higher-tier Wyrm encounters (Levels 9-11, Weapon and Armor variants) with meaningfully better rewards — including a chance at GM-quality gear and pentagrams — than the existing lower-level Wyrms. (for testing purposes)
- New rideable mounts added to the world: armored swamp dragon, Mondain's horse, ridgeback, savage ridgeback, skeletal horse, swamp dragon, and unicorn.
- A new barber NPC template.

### Player Impact

- A new, tougher tier of Wyrm to farm for better rewards.
- Several new mount types now exist in the world.

## Galleon Ships Overhaul

- Orc, Gargoyle, Tokuno, and Britannia galleons are now fully functional: steering wheel, anchor, speed control, dry-docking, and boarding/disembarking via rope all work correctly, matching the classic boats.
- All four galleon deeds, plus the Row Boat deed, are now purchasable (and sellable) from Shipwright NPCs.
- Cargo holds now work correctly on every ship type: Britannia's storage crates, previously invisible due to a graphic bug, are now visible; Orc, Gargoyle, and Tokuno galleons each have a working cargo hold; Row Boats correctly have none.
- Boat speed now has 5 real, distinct settings instead of a barely-noticeable range — from about 2x slower than walking at the low end, up to full mounted-running speed at the top.
- Ropes now reliably place you on solid dry ground when getting off a galleon, instead of occasionally failing or dropping you somewhere odd.
- Sea monsters no longer randomly spawn and attack boats.

### Player Impact

- Every galleon type can now be bought, sailed, and used for cargo storage — previously several were non-functional or missing entirely from vendors.
- Boat speed control now feels meaningfully different between settings.
- Getting on and off a galleon via rope is more reliable.

## Behind the Scenes

- Added a new Area Spawner system (staff/admin tool) for creating region-based spawns of NPCs, custom-built characters, chests, and items, with its own management commands and error reporting.

### Player Impact

- No direct player-facing change — this is a staff content-management tool. Over time it should mean more varied, better-maintained spawns across the world.

## Summary

- Fixed `.speedwalk` not working.
- Added ~50 new Sosaria teleporter connections and cleaned up the Water Dungeon's level links.
- Fixed a large batch of non-functional doors on the Sosaria decorate facet.
- Added 6 new higher-tier Wyrm reward encounters (Levels 9-11).
- Added 7 new rideable mounts and a barber NPC.
- Overhauled galleon ships (Orc, Gargoyle, Tokuno, Britannia): now fully functional, with working cargo holds, 5-tier speed control, more reliable ropes, and no more random sea-monster attacks — all now purchasable from Shipwrights.
- New internal Area Spawner tooling for staff-managed region spawns (no direct player-facing change).

Thanks for playing Zuluhotel Omega 3.
