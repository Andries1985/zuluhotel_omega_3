# Developer Changelog - v3.0.1

Range: Patch-3.0.0..Patch-3.0.1  
Branch: Patch-3.0.1  
Date: 2026-06-25

---

## Scope Summary

- Total files changed: 45
- Text/config/script files changed: 44
- Binary/archive files changed: 1
- Net textual delta: 9637 insertions, 16 deletions
- Branch commits in range: Custom Housing Attempt; null resources with skill gain fixes

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - | Notes |
|---|---|---:|---:|---|
| M | pkg/items/doors/include/settings.inc | 0 | 0 | Minor settings-level alignment for door-related static housing behavior (metadata-only net change). |
| M | pkg/items/houseExtras/include/settings.inc | 1 | 1 | Settings adjustment for static housing flow integration. |
| M | pkg/multis/house/multiSign/method.src | 1 | 1 | Small method hook alignment with static housing sign flow. |
| M | pkg/multis/multiCommands/include/multicommands.inc | 1 | 1 | Added/adjusted command include wiring for static housing commands. |
| A | pkg/multis/staticHousing/bantile/walkOn.src | 22 | 0 | New static-housing ban tile walk-on handler. |
| A | pkg/multis/staticHousing/commands/gm/removeStaticDeed.src | 205 | 0 | New staff command flow for static deed removal. |
| A | pkg/multis/staticHousing/commands/gm/staticDeed.src | 412 | 0 | New staff command flow for static deed management/creation. |
| A | pkg/multis/staticHousing/commands/player/decorate.src | 445 | 0 | New player decorate command flow for static houses. |
| A | pkg/multis/staticHousing/config/icp.cfg | 12 | 0 | Package ICP config for static housing. |
| A | pkg/multis/staticHousing/config/itemdesc.cfg | 80 | 0 | Item descriptors for static housing signs/deeds/components. |
| A | pkg/multis/staticHousing/config/settings.cfg | 133 | 0 | Static housing configuration defaults and toggles. |
| A | pkg/multis/staticHousing/documentation/How to use this staticHousing package.txt | 57 | 0 | Static housing usage documentation. |
| A | pkg/multis/staticHousing/documentation/Using the static housing system.txt | 115 | 0 | Additional static housing operational documentation. |
| A | pkg/multis/staticHousing/include/coownerlayout.inc | 271 | 0 | Co-owner gump/layout include. |
| A | pkg/multis/staticHousing/include/defaultlayout.inc | 45 | 0 | Default static housing layout include. |
| A | pkg/multis/staticHousing/include/friendlayout.inc | 223 | 0 | Friend layout include. |
| A | pkg/multis/staticHousing/include/gumpMessage.inc | 146 | 0 | Gump messaging include for static housing UI. |
| A | pkg/multis/staticHousing/include/old-gumps.inc | 1429 | 0 | Legacy/compat gump definitions included for static housing flows. |
| A | pkg/multis/staticHousing/include/ownerlayout.inc | 319 | 0 | Owner layout include. |
| A | pkg/multis/staticHousing/include/settings.inc | 86 | 0 | Static housing include settings/constants. |
| A | pkg/multis/staticHousing/include/signSelectionFunctions.inc | 78 | 0 | Sign selection helper functions for static housing flows. |
| A | pkg/multis/staticHousing/include/staticHousing.inc | 101 | 0 | Core static housing include helpers. |
| A | pkg/multis/staticHousing/include/staticlayout.inc | 314 | 0 | Static housing layout definitions. |
| A | pkg/multis/staticHousing/include/staticlayout_2.inc | 315 | 0 | Additional static housing layout definitions. |
| A | pkg/multis/staticHousing/lockunlock.src | 771 | 0 | New lock/unlock behavior for static house signs/containers. |
| A | pkg/multis/staticHousing/logoff.src | 81 | 0 | Static housing logoff hook. |
| A | pkg/multis/staticHousing/logon.src | 15 | 0 | Static housing logon hook. |
| A | pkg/multis/staticHousing/pkg.cfg | 6 | 0 | Static housing package registration. |
| A | pkg/multis/staticHousing/reconnect.src | 21 | 0 | Static housing reconnect hook. |
| A | pkg/multis/staticHousing/securecontainer/staticSecureCont.src | 88 | 0 | Static housing secure-container behavior. |
| A | pkg/multis/staticHousing/sign/control.src | 755 | 0 | Static housing sign control logic. |
| A | pkg/multis/staticHousing/sign/destroy.src | 88 | 0 | Static housing sign destroy/demolish flow. |
| A | pkg/multis/staticHousing/sign/method.src | 1423 | 0 | Static housing sign method helpers. |
| A | pkg/multis/staticHousing/sign/use.src | 1397 | 0 | Static housing sign interaction flow. |
| A | pkg/multis/staticHousing/staticHousing.zip | - | - | Added static housing packaged archive asset (binary). |
| A | pkg/multis/staticHousing/transferdeed/staticTransferDeed.src | 134 | 0 | Static housing transfer deed support. |
| M | pkg/opt/earth/earthportal.src | 1 | 1 | Minor portal flow alignment in same branch range. |
| M | pkg/std/camping/camping.src | 3 | 0 | Added backpack-or-ground guard for kindling use. |
| M | pkg/std/cartography/cartography.src | 34 | 6 | Added blank-map validation helper and centralized map material-cost helper; added special-map prechecks. |
| M | pkg/std/mining/smelting.src | 3 | 0 | Added backpack requirement guard for ore smelting path. |
| M | pkg/std/runebook/customspells.inc | 1 | 1 | Minor spell/include alignment update in branch range. |
| M | pkg/std/spells/gate.src | 1 | 1 | Minor spell flow alignment update in branch range. |
| M | pkg/std/spells/mark.src | 1 | 1 | Minor spell flow alignment update in branch range. |
| M | pkg/std/spells/recall.src | 1 | 1 | Minor spell flow alignment update in branch range. |
| M | pkg/utils/clilocs/include/clilocs.inc | 1 | 1 | Minor cliloc/include alignment update in branch range. |

---

## Detailed Changes By Theme

### 1) Static Housing Package Introduction (Major)

Files involved:
- pkg/multis/staticHousing/pkg.cfg
- pkg/multis/staticHousing/config/icp.cfg
- pkg/multis/staticHousing/config/itemdesc.cfg
- pkg/multis/staticHousing/config/settings.cfg
- pkg/multis/staticHousing/include/staticHousing.inc
- pkg/multis/staticHousing/include/settings.inc
- pkg/multis/staticHousing/include/defaultlayout.inc
- pkg/multis/staticHousing/include/staticlayout.inc
- pkg/multis/staticHousing/include/staticlayout_2.inc
- pkg/multis/staticHousing/include/ownerlayout.inc
- pkg/multis/staticHousing/include/coownerlayout.inc
- pkg/multis/staticHousing/include/friendlayout.inc
- pkg/multis/staticHousing/include/signSelectionFunctions.inc
- pkg/multis/staticHousing/include/gumpMessage.inc
- pkg/multis/staticHousing/include/old-gumps.inc
- pkg/multis/staticHousing/sign/control.src
- pkg/multis/staticHousing/sign/method.src
- pkg/multis/staticHousing/sign/use.src
- pkg/multis/staticHousing/sign/destroy.src
- pkg/multis/staticHousing/securecontainer/staticSecureCont.src
- pkg/multis/staticHousing/transferdeed/staticTransferDeed.src
- pkg/multis/staticHousing/lockunlock.src
- pkg/multis/staticHousing/bantile/walkOn.src
- pkg/multis/staticHousing/logon.src
- pkg/multis/staticHousing/logoff.src
- pkg/multis/staticHousing/reconnect.src
- pkg/multis/staticHousing/commands/gm/staticDeed.src
- pkg/multis/staticHousing/commands/gm/removeStaticDeed.src
- pkg/multis/staticHousing/commands/player/decorate.src
- pkg/multis/staticHousing/documentation/How to use this staticHousing package.txt
- pkg/multis/staticHousing/documentation/Using the static housing system.txt
- pkg/multis/staticHousing/staticHousing.zip

Behavior changes:
- Introduced a full static housing package under pkg/multis/staticHousing with its own config, command set, sign logic, secure handling, deed transfer support, lifecycle hooks, and UI layout includes.
- Added GM and player command surfaces for static deed management and player decoration flows.
- Added sign control/use/method stacks and related lock/unlock, ban tile, and secure-container handlers for static housing behavior.
- Added comprehensive layout/gump include sets to support owner/co-owner/friend/default interactions.
- Added package documentation and a packaged archive asset (staticHousing.zip) inside the branch delta.

### 2) House and Command Wiring for Static Housing

Files involved:
- pkg/multis/multiCommands/include/multicommands.inc
- pkg/multis/house/multiSign/method.src
- pkg/items/houseExtras/include/settings.inc
- pkg/items/doors/include/settings.inc

Behavior changes:
- Existing multi-command and house sign layers were adjusted to align with static housing command/sign pathways.
- Settings-level toggles and include wiring were updated to keep static housing behavior reachable from existing package flows.

### 3) Resource Validation / Skill-Gain Stability Fixes

Files involved:
- pkg/std/camping/camping.src
- pkg/std/cartography/cartography.src
- pkg/std/mining/smelting.src

Behavior changes:
- Camping now rejects kindling use if the item is in an invalid container context and not in backpack/ground flow.
- Smelting now requires ore to be in the character backpack before smelting proceeds.
- Cartography now centralizes special map material-cost calculation and validates blank-map ownership/location before continuing.
- Added explicit prechecks so null/invalid resource paths fail early instead of entering inconsistent skill/action flow.

### 4) Small Supporting Branch-Range Alignments

Files involved:
- pkg/opt/earth/earthportal.src
- pkg/std/runebook/customspells.inc
- pkg/std/spells/gate.src
- pkg/std/spells/mark.src
- pkg/std/spells/recall.src
- pkg/utils/clilocs/include/clilocs.inc

Behavior changes:
- Minor one-line alignment updates were included in the same branch range and are captured for completeness.

---

## Validation Notes

- Diff range used: Patch-3.0.0..Patch-3.0.1
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - git diff --shortstat
  - git log --oneline --no-merges
- The changelog is file-complete for the Patch-3.0.1 branch delta and now includes static housing changes.
