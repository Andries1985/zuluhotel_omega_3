# Patch Notes - v3.0.1
**Zuluhotel Omega 3 | Live Shard**  
**Date: [TBD]**

---

## What Changed

Patch 3.0.1 is a focused stability update for resource-gathering and crafting skill flows. It resolves null-resource edge cases that could interrupt normal actions and cause inconsistent skill gain behavior.

Players should notice fewer interrupted actions while gathering or crafting and more reliable skill progression in affected systems.

## Resource and Skill-Gain Stability

- Added null-resource safety handling in camping so invalid resource states no longer break the action flow.
- Added null-resource safety handling in smelting so missing resource references are handled safely.
- Updated cartography flow checks so resource usage and skill-gain paths remain valid in more edge cases.
- Reduced fail-state paths that previously caused action interruption when resource references were unexpectedly missing.

## Player Quality-of-Life

- Gathering and crafting actions in affected systems now fail less often on edge-case states.
- Skill-gain handling is now more consistent when resource objects are unavailable or invalid.

---

## Player Highlights

If you only want the short version, this patch gives you:

- Better reliability for camping, smelting, and cartography actions.
- Fewer null-resource failures during gameplay.
- More consistent skill-gain behavior in affected crafting/resource flows.

---

Thanks for playing Zuluhotel Omega 3.
