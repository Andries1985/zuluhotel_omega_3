# Developer Changelog - v3.0.0

Range: e675ba4..6a1e399  
Branch: Patch-3.0.0  
Date: 2026-06-04

---

## Scope Summary

- Total files changed: 43
- Text/config/script files changed: 43
- Net textual delta: 12324 insertions, 4977 deletions
- Branch commits in range: Housing Updates, Housing Fixes

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - | Notes |
|---|---|---:|---:|---|
| M | config/mrcspawn.cfg | 1 | 0 | Added house deed entries to the deed product group. |
| A | config/mrcspawn.cfg.bak | 2645 | 0 | Backup snapshot of the updated spawn config. |
| M | config/servspecopt.cfg | 2 | 28 | Cleaned duplicated comments, restored YellRange, and tidied defaults. |
| D | pkg/multis/customHousing/config/icp.cfg | 0 | 20 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/config/itemdesc.cfg | 0 | 753 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/config/syshook.cfg | 0 | 4 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/include/house.inc | 0 | 727 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/include/housefriends.inc | 0 | 179 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/lockunlock.src | 0 | 699 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/pkg.cfg | 0 | 6 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/scripts/customHouseDeed.src | 0 | 702 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/sign.src | 0 | 633 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/signcontrol.src | 0 | 164 | Legacy custom housing package removed. |
| D | pkg/multis/customHousing/syshook/closecustomhouse.src | 0 | 64 | Legacy custom housing package removed. |
| M | pkg/multis/house/config/itemdesc.cfg | 52 | 0 | Added new deed types and refreshed house descriptions. |
| A | pkg/multis/house/config/itemdesc.cfg.bak | 529 | 0 | Backup snapshot of the migrated itemdesc file. |
| M | pkg/multis/house/config/settings.cfg | 2 | 2 | Abandon timeout updated to one year. |
| A | pkg/multis/house/include/footagearrays.inc | 526 | 0 | New footprint helper arrays for house placement checks. |
| R100 | pkg/multis/house/include/isValidLoc.inc -> pkg/multis/house/include/isValidLoc.inc.bak | 0 | 0 | Archived old location validation helper. |
| A | pkg/multis/house/include/utility.inc.bak | 64 | 0 | Backup snapshot of the migrated utility include. |
| M | pkg/multis/house/multiDeed/changeOwner.src | 2 | 7 | Refreshed decay timing and removed noisy debug output. |
| A | pkg/multis/house/multiDeed/changeOwner.src.bak | 78 | 0 | Backup snapshot of the deed owner-change flow. |
| M | pkg/multis/house/multiDeed/use.src | 248 | 377 | Reworked house placement validation and build flow. |
| A | pkg/multis/house/multiDeed/use.src.bak | 617 | 0 | Backup snapshot of the deed use flow. |
| M | pkg/multis/house/multiSign/control.src | 6 | 19 | Simplified sign listener flow and tightened unsecure/demolish handling. |
| A | pkg/multis/house/multiSign/control.src.bak | 490 | 0 | Backup snapshot of the sign control flow. |
| A | pkg/multis/house/multiSign/control.src.bak2 | 490 | 0 | Secondary backup snapshot of the sign control flow. |
| M | pkg/multis/house/multiSign/method.src | 143 | 38 | Updated sign detection, permission handling, and ban/co-owner cleanup. |
| A | pkg/multis/house/multiSign/method.src.bak | 1262 | 0 | Backup snapshot of the sign method helpers. |
| A | pkg/multis/house/multiSign/method.src.bak2 | 1252 | 0 | Secondary backup snapshot of the sign method helpers. |
| M | pkg/multis/house/multiSign/use.src | 1255 | 810 | Major house sign interaction and permission-system refactor. |
| A | pkg/multis/house/multiSign/use.src.bak | 1478 | 0 | Backup snapshot of the sign use flow. |
| A | pkg/multis/house/multiSign/use.src.bak2 | 1508 | 0 | Secondary backup snapshot of the sign use flow. |
| M | pkg/multis/house/secureCont.src | 18 | 12 | Hardened secure-container access checks. |
| A | pkg/multis/house/secureCont.src.bak | 34 | 0 | Backup snapshot of secure-container handling. |
| A | pkg/multis/house/secureCont.src.bak2 | 52 | 0 | Secondary backup snapshot of secure-container handling. |
| M | pkg/multis/house/walkOn.src | 12 | 2 | Fixed ban tile relocation behavior and null-safe sign handling. |
| A | pkg/multis/house/walkOn.src.bak | 44 | 0 | Backup snapshot of walk-on handling. |
| M | regions/regions.cfg | 4 | 0 | Added city/dungeon region type tags for placement logic. |
| M | scripts/ai/banker.src | 176 | 55 | Added balance summary, withdraw support, and banker order protection. |
| A | scripts/ai/bankerbalancegump.src | 42 | 0 | New banker balance summary gump. |
| M | scripts/ai/tamed.src | 0 | 21 | Small follow-state cleanup around house/multi boundary checks. |
| M | scripts/misc/logoff.src | 1 | 0 | Clears banker check-creation state on logoff. |

---

## Detailed Changes By Theme

### 1) Housing Package Migration and Placement Validation

The biggest change in this branch is the migration away from the old custom housing package and into the main house package.

Files involved:
- pkg/multis/customHousing/config/icp.cfg
- pkg/multis/customHousing/config/itemdesc.cfg
- pkg/multis/customHousing/config/syshook.cfg
- pkg/multis/customHousing/include/house.inc
- pkg/multis/customHousing/include/housefriends.inc
- pkg/multis/customHousing/lockunlock.src
- pkg/multis/customHousing/pkg.cfg
- pkg/multis/customHousing/scripts/customHouseDeed.src
- pkg/multis/customHousing/sign.src
- pkg/multis/customHousing/signcontrol.src
- pkg/multis/customHousing/syshook/closecustomhouse.src
- pkg/multis/house/include/footagearrays.inc
- pkg/multis/house/include/isValidLoc.inc.bak
- pkg/multis/house/include/utility.inc.bak
- pkg/multis/house/multiDeed/use.src
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/config/settings.cfg
- pkg/multis/house/config/itemdesc.cfg.bak
- pkg/multis/house/multiDeed/changeOwner.src
- pkg/multis/house/multiSign/control.src
- pkg/multis/house/multiSign/method.src
- pkg/multis/house/multiSign/use.src
- pkg/multis/house/secureCont.src
- pkg/multis/house/walkOn.src

Behavior changes:
- Added new footprint helper arrays in footagearrays.inc for multiple house shapes and workshop/tower variants.
- Replaced the old isValidLoc-style placement gating with region-type footprint checks in multiDeed/use.src.
- House placement now checks city and dungeon regions against the footprint bounds of the target house type.
- Added a more robust house footprint function to resolve the occupied area per house type.
- The build flow now sets the built deed name after item creation and no longer retries placement in multiple offset positions.
- House ownership and decay timing now use the shared multihouse settings path rather than hard-coded timing.
- Sign control and permission helpers were refactored to work from objtype checks and stored permission lists instead of the older footage-based sign discovery.
- Co-owner and friend cleanup now clears all permission buckets when a mobile is removed from those lists.
- Ban list handling now enforces the intended 20-item ceiling correctly.
- Secure-container use now requires the secure and the player to be in the same house multi, and it now uses sign-derived permissions when available.
- House walk-on ban handling now drops the player at a standing-height-safe location instead of force-moving to the house Z value directly.
- House sign listener flow was simplified and decay refresh behavior remains aligned with the new package.
- House deed change-owner handling now uses the shared abandon timeout setting.
- The new house config set adds house descriptions and the new garden shed deed types.
- The migrate-and-backup pattern is reflected in the numerous .bak snapshots that preserve the previous package behavior.

### 2) Banking and Economy Support

Files involved:
- config/servspecopt.cfg
- scripts/ai/banker.src
- scripts/ai/bankerbalancegump.src
- scripts/misc/logoff.src

Behavior changes:
- Added a dedicated banker balance gump for showing coins and cheques by currency tier.
- Banker speech handling now supports full balance summaries for gold, silver, and copper coins and checks.
- Added withdraw speech support for classic-style commands like withdraw 1000 gold.
- Added a banker check-creation lock flag so balance/withdraw handling does not interrupt note creation.
- Introduced a dedicated banker order object type constant for summary processing.
- Added helper functions to summarize currency and enforce note withdrawal limits.
- Logoff now clears banker_making_check so interrupted sessions do not leave the character in a locked banker state.
- Servspecopt cleanup removed duplicate comments, normalized the defaults block, and restored YellRange to 25.

### 3) Regions, Spawn Tables, and Housing Deeds

Files involved:
- config/mrcspawn.cfg
- config/mrcspawn.cfg.bak
- regions/regions.cfg
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/config/settings.cfg
- pkg/multis/house/include/footagearrays.inc
- pkg/multis/house/walkOn.src

Behavior changes:
- Added small brick house, garden shed south, and garden shed east deeds to the deed product group.
- Added matching house object definitions for the new deed types.
- Updated house descriptions across the existing house list so item and house labels are cleaner and more explicit.
- Added Type tags for city/dungeon regions so the new house placement logic can reliably distinguish placement restrictions.
- Updated the house abandon timeout to one year.
- Added a spawn config backup snapshot as part of the migration.

### 4) Small AI Cleanup

Files involved:
- scripts/ai/tamed.src

Behavior changes:
- Tightened follow logic around house and multi boundary checks by returning early instead of forcibly resetting the follow target in several cases.
- This is a small behavior cleanup that reduces unnecessary state clobbering during tamed follow resolution.

---

## Validation Notes

- Diff range used: e675ba4..6a1e399
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - targeted diff reads for housing, banking, region, and support scripts
- The changelog is file-complete for the Patch-3.0.0 branch delta.
