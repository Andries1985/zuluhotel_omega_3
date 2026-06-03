# Patch Notes — v1.0.3
**Zuluhotel Omega 3 | Live Shard**  
**Date: [TBD]**

---

## What Changed

Patch 1.0.3 focuses on taming and mount reliability. It restores core tamed AI behavior from the stable ZH2.5 baseline, improves reclaim flows with animal trainers, and corrects related mount conversion and NPC template mapping.

Players should notice more consistent pet and mount behavior, safer pet return handling, and fewer edge-case failures when interacting with trainers and merchants.

## Tamed AI and Mount Behavior

- Restored and aligned core `tamed` AI behavior from ZH2.5 for mount and pet interactions.
- Added stronger guard rails for pet container handling, target selection flow, and master resolution.
- Improved mount validation paths to reduce invalid state transitions.
- Reduced certain pet counting weight behavior for boss-flagged tames to better reflect active control limits.

## Animal Trainer and Merchant Fixes

- Fixed item return handling when gold/coin hand-ins are rejected by trainers and merchants.
- Added safer trainer reclaim handling with null checks to avoid failed spawn/reclaim paths.
- Added confiscation ticket reclaim support including ownership check, fine payment, and controlled pet restoration.
- Normalized restored pet HP handling to reduce inconsistent post-restore health states.
- Added cancellation handling in trainer stable targeting flow.

## Mount Data and Conversion Support

- Updated mount-related NPC template definitions in `npcdesc` to align expected mount behavior and mappings.
- Updated spawnpoint group configuration where mount template usage required refresh.
- Updated `uoconvert.cfg` mount conversion mappings to match the latest mount/NPC template set.

## Supporting Runtime and Tooling Updates

- Included branch runtime/tool updates packaged with this patch line.
- Updated supporting scripts and module files required by this branch state.

---

## Player Highlights

If you only want the short version, this patch gives you:

- More reliable tamed pet and mount behavior.
- Better trainer reclaim support, including confiscated pet recovery.
- Better item-return handling from trainers and merchants.
- Updated mount templates and conversion paths for cleaner shard behavior.

---

Thanks for playing Zuluhotel Omega 3.
