# Patch Notes — v1.0.0
**Zuluhotel Omega 3 | Live Shard**  
**Date: [TBD]**

---

## What Changed

Patch 1.0.0 is a major foundation update. It expands combat, crafting, skills, items, NPC behavior, and shard systems while also bringing in a large set of optional gameplay packages and supporting tools.

Players should notice deeper combat interactions, more scripted items and world objects, more skill content, more utility systems, and broader support for shard events, housing, and special gameplay features.

## Areas and Region Policy Reliability

- The Areas admin gump was stabilized and now opens and saves consistently.
- Area policy settings are now stored per realm with persistent datafiles, preventing cross-realm overwrite behavior.
- Area rules now persist correctly after reboot.
- Area identity now uses explicit id=<value> tokens in areas definitions, improving save/load stability when names or ordering change.
- Area labels in the gump now display clean names (without id= prefixes).
- Internal stale-policy cleanup now removes policy records for areas no longer present in areas definitions.
- Runtime area checks now use the new datafile-backed policy path consistently.

---

## Combat & Weapons

- Combat now supports a much larger set of hit effects, weapon behaviors, and combat scripts.
- New on-hit effects and spell strike logic add more variety to fights.
- Damage handling, death behavior, and creature combat routines were expanded behind the scenes.

## Crafting & Skills

- Crafting and related skill systems were expanded with more support for player-made items and skill-based interactions.
- Core skills like tailoring, tinkering, blacksmithing, alchemy, fishing, mining, and treasure map play received support updates.
- Training, trap, stealth, stealing, tracking, and other utility skills now have broader package support.

## Spells & Magic

- The spell system now includes a larger set of core spells and spell support files.
- Magic utility such as recall, teleport, protection, invisibility, summoning, and field spells were added or refreshed.
- Spellbook, scroll, and spell config support were also expanded.

## Housing, Items, and World Objects

- More placeable items, house decorations, furniture, doors, containers, and deed-based structures are now supported.
- Currency, keys, and storage-related items were expanded.
- A wider range of world objects and special-use items can now behave differently instead of acting like simple static items.

## NPCs, AI, and Systems

- NPC behavior was expanded for guards, vendors, townsfolk, monsters, and special encounters.
- Account, email, online-count, and reputation systems were added or improved.
- Several shard systems now have better hooks for login, reconnect, and runtime behavior.

## Optional Content and Events

- A large set of optional gameplay packages was added for shard customization.
- This includes seasonal content, events, moongates, rituals, books, songs, farming, guild features, vanity systems, and other special content.
- These systems give the shard more flexibility for custom rulesets and event-driven gameplay.

## Tools and Support

- Utility packages for gumps, item inspection, data files, time handling, security reports, and cliloc support were updated.
- Command coverage was expanded for players, staff, and testing workflows.
- Repository-level configuration, build files, and tooling were refreshed as part of the initial import.

---

## Player Highlights

If you only want the short version, this patch gives you:

- More combat variety and stronger weapon-specific behavior.
- More skills, spells, and crafting support.
- More usable items, housing objects, and world interactions.
- More NPC behavior and more shard systems running in the background.
- More optional events and custom content for future gameplay expansion.

---

Thanks for playing Zuluhotel Omega 3.
