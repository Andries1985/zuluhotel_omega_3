# Patch Notes - v3.1.0
**Zuluhotel Omega 3 | Beta Shard**

**Date: September 1, 2026**

---

## What Changed

Patch 3.1.0 unifies the crafting gump across five skills, adds 30 new named regions to Tokuno, introduces the Runic Atlas (a massive-capacity runebook) with new Guild Travel gates and realm-colored runes, cleans up several problem world spawns, and includes a long list of bugfixes found during a full pass over the shard's scripts — including a login error that was hitting essentially every player.

## Crafting Gump Unification

- Blacksmithy, Bowcraft, Carpentry, Tailoring, and Tinkering now all share one clean, consistent crafting menu.
- Characters classified as Crafters no longer have their Magery or Musicianship skill points count against their specialization class — train either skill freely without losing your Crafter status.

### Player Impact

- A cleaner, more consistent crafting experience across five skills.
- Crafters can train Magery and Musicianship without penalty.

## New Tokuno Content

- Tokuno gains 30 newly named sub-regions — forests, shrines, dojos, dungeons, a PvP zone, and more — each with its own name, ambient music, and entry/exit message.
- The `.go` travel menu now correctly lists many previously miscategorized destinations (Luna, Umbra, Malas, Ilshenar, Ter Mur cities, and others), and gains all 30 new Tokuno locations as travel destinations.

### Player Impact

- Tokuno feels more alive with distinct, named areas to explore.
- Fast travel via `.go` is more complete and better organized.

## Runic Atlas & Runebook Improvements

- New item: the **Runic Atlas**, a massive-capacity runebook holding 48 marked runes and 100 recall charges (versus a standard Runebook's 16 runes and 5 charges), obtainable as a Bulk Order Deed reward. It uses a cleaner, easier selection-style interface.
- New **Guild Travel** gates: guild members can now open a travel gate that only fellow guild members can step through.
- Marked recall runes are now color-coded by which facet they lead to, both on standard Runebooks and the new Runic Atlas.
- The Runic Dye Tub can now recolor Runic Atlases as well as standard Runebooks.

### Player Impact

- A new premium runebook option for players who juggle a lot of recall locations.
- Guilds have a new way to control who can use their travel gates.
- Recall runes are easier to tell apart at a glance by destination.

## World & Spawn Fixes

- Several elemental "shrine" creatures and elemental-summon monsters have been removed from world spawns and will no longer be encountered in the wild.
- Fixed several aquatic creatures (including dolphins and sea serpents) that were incorrectly spawning on land; jellyfish, kraken, crabs, sea serpents, and walruses now move and swim correctly.

### Player Impact

- Fewer out-of-place or problematic monster encounters in the wild.

## Fixes

- **Fixed a login error that was affecting the large majority of players** attempting to log in.
- Fixed several combat and housing bugs found during a full review of the shard's scripts, including: a combat enchant that was skipping damage on most hits, custom housing's lock down/secure/release tools (which had stopped working entirely), a house-ownership transfer bug, boat drydocking not working for Row Boats, Power Hour getting permanently stuck after certain choices, and a spell-area-of-effect bug that could let a victim standing in a safe area still get hit.
- Fixed several crafting/potion issues: a missing intermediate potion-strength tier, Bowcraft's exceptional-item chance was quietly weaker than every other crafting skill (now matches), and a damage-resistance bonus that had stopped applying against several character classes.
- Fixed a Powerscroll cap-tool false positive, a quest-tracking crash, a communication-crystal linking bug, a few miscolored/mislabeled items, and a handful of smaller script errors.
- General performance cleanup: several frequently-run systems (item stacking checks, area policy saves, cooking's fire/oven detection, and the town-stone system) were optimized to do meaningfully less repeated work under the hood, with no change in how they behave.

### Player Impact

- Login should now be reliable for everyone, not just players standing inside a house.
- A wide range of small-to-medium annoyances across combat, housing, crafting, and other systems are resolved.
- Several systems should feel a bit snappier under heavy shard load.

## Summary

- Unified the crafting gump across Blacksmithy, Bowcraft, Carpentry, Tailoring, and Tinkering, and freed Crafters to train Magery/Musicianship without losing their class bonus.
- Added 30 new named regions to Tokuno and improved the `.go` travel menu's organization and coverage.
- Added the Runic Atlas (48 runes / 100 charges), Guild Travel gates, and realm-colored recall runes.
- Cleaned up several problem world spawns (elemental shrines/summons, misplaced aquatic creatures).
- Fixed a login error affecting most players, plus a long list of combat, housing, crafting, and misc bugs found during a full script review, and made several systems noticeably more efficient under the hood.

Thanks for playing Zuluhotel Omega 3.
