# Patch Notes - v3.0.1
**Zuluhotel Omega 3 | Beta Shard**  
**Date: [June 2, 2026]**

---

## What Changed

Patch 3.0.1 introduces the new static housing system and includes follow-up stability fixes for resource-gathering and crafting flows.

Players should notice improved static house interaction options, updated housing control behavior, and fewer interrupted actions in affected crafting/resource systems.

## Static Housing System

- Added a dedicated static housing package with new static sign/control flows.
- Added static housing support commands for staff deed management and player decoration.
- Added static-house secure container, lock/unlock, transfer deed, and ban tile handling.
- Added static housing configuration and layout support for owner, co-owner, and friend interaction paths.

## Housing Integration

- Updated existing housing command/sign wiring to align with static housing support paths.
- Included supporting settings alignments for related house/door integration points.

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

- New static housing system support with updated sign/control and decoration workflows.
- Better static house interaction behavior for ownership/permissions and secure features.
- Better reliability for camping, smelting, and cartography actions.
- Fewer null-resource failures during gameplay.
- More consistent skill-gain behavior in affected crafting/resource flows.

---

Thanks for playing Zuluhotel Omega 3.
