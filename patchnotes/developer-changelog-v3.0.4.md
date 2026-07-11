# Developer Changelog - v3.0.4

Range: Patch-3.0.3..Patch-3.0.4
Branch: Patch-3.0.4
Date: 2026-07-11

---

## Scope Summary

- Total files changed: 75
- Text/config/script files changed: 69
- Net textual delta: 383559 insertions, 277 deletions
- Branch commits in range: Bunch of changes; Gump Fix; Town Stone updates More teleporters; Snooping error fix, stealing snooping code fixes house sign fixes; Guilds fix and large patio house prints for failure to destroy; Another try to fix mount on death; Housing co-owner fixes housing sign access distance limit guilds housing check fix; Dismount fix for stale mounts on death; mounttest animation fix; Patch 3.0.4: move mounttest to player and add uncategorized spawn groups

---

## Complete File Inventory (Exhaustive)

### Command surface and test utilities
- config/cmds.cfg
- config/command_synopses.cfg
- scripts/textcmd/admin/admin.src
- scripts/textcmd/admin/akill.src
- scripts/textcmd/admin/destroymulti.src
- scripts/textcmd/admin/equip.src
- scripts/textcmd/player/guilds.src
- scripts/textcmd/player/suicide.src
- scripts/textcmd/seer/info.src
- scripts/textcmd/seer/kill.src
- scripts/textcmd/test/animationtest.src
- scripts/textcmd/test/clearmount.src
- pkg/opt/alryc/textcmd/player/mounttest.src (moved from test/)
- pkg/opt/alryc/textcmd/test/gotomulti.src

### Housing migration and multi behavior
- pkg/multis/customhousing/config/icp.cfg
- pkg/multis/customhousing/config/itemdesc.cfg
- pkg/multis/customhousing/config/syshook.cfg
- pkg/multis/customhousing/include/house.inc
- pkg/multis/customhousing/include/housefriends.inc
- pkg/multis/customhousing/pkg.cfg
- pkg/multis/customhousing/scripts/customeHouseDeed.src
- pkg/multis/customhousing/sign.src
- pkg/multis/customhousing/signcontrol.src
- pkg/multis/customhousing/syshook/closecustomhouse.src
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/include/utility.inc
- pkg/multis/house/multiSign/use.src
- pkg/multis/multiCommands/commands/gm/destroymulti.src
- scripts/misc/customhousecommit.src

### Townstones, spawnpoint data, and governance
- pkg/opt/spawnpoint/config/groups.cfg
- pkg/opt/spawnpoint/spawnpoint_container_objtypes.md (renamed from .txt)
- pkg/opt/townstones/playertowns.cfg
- pkg/opt/townstones/textcmd/admin/townbankstatus.src
- pkg/opt/townstones/textcmd/player/playerruntowns.src
- pkg/opt/townstones/tstone.inc
- pkg/opt/townstones/tstone.src
- pkg/opt/townstones/upgrades.cfg
- pkg/opt/townstones/upgrades.xlsx
- pythonscripts/_sync_townstone_upgrades.py

### Economy, cache, and snooping/stealing
- pkg/opt/omegacache/omegacache.inc
- pkg/opt/vanityshop/vanityshop.src
- pkg/std/snooping/snooping.src
- pkg/std/snooping/stealing.src
- pkg/std/snooping/stealitems.cfg
- pkg/std/stealing/stealing.src

### Guilds, mounts, death, AI, and world support
- pkg/opt/guilds/include/guilds.inc
- scripts/include/all.inc
- scripts/include/dismount.inc
- scripts/include/npcboosts.inc
- scripts/include/spawnnet.inc
- scripts/include/spawnpoint.inc
- scripts/include/teleporters.inc
- scripts/ai/animaltrainer.src
- scripts/ai/chaosmultikillpcs.src
- scripts/ai/humuc.src
- scripts/ai/instakillguard.src
- scripts/ai/main/assassinsleep.inc
- scripts/ai/main/mainloopsheep.inc
- scripts/ai/main/sleepmode.inc
- scripts/ai/merchant.src
- scripts/ai/newbiemultikillpcs.src
- scripts/ai/tamed.src
- scripts/ai/townguard.src
- scripts/ai/warrior.src
- scripts/misc/chrdeath.src
- scripts/misc/death.src
- scripts/misc/rise.src

### Generated and support artifacts
- poltool.exe
- scripts/ecompile.exe
- scripts/runecl.exe
- uoconvert.exe
- uoconvert.txt
- uotool.exe
- tmp/diff_reports/phase_changes_annotated_review.md
- tmp/diff_reports/phase_changes_live_test_checklist.md

---

## Detailed Changes By Theme

### 1) Housing package migration and sign handling

The housing work in this range continues the migration into the main house package and tightens the sign and permission flow.

Files involved:
- pkg/multis/customhousing/config/icp.cfg
- pkg/multis/customhousing/config/itemdesc.cfg
- pkg/multis/customhousing/config/syshook.cfg
- pkg/multis/customhousing/include/house.inc
- pkg/multis/customhousing/include/housefriends.inc
- pkg/multis/customhousing/pkg.cfg
- pkg/multis/customhousing/scripts/customeHouseDeed.src
- pkg/multis/customhousing/sign.src
- pkg/multis/customhousing/signcontrol.src
- pkg/multis/customhousing/syshook/closecustomhouse.src
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/include/utility.inc
- pkg/multis/house/multiSign/use.src
- pkg/multis/multiCommands/commands/gm/destroymulti.src
- scripts/misc/customhousecommit.src

Behavior changes:
- Continued moving custom housing behavior into the live house package.
- Refined house sign ownership, friend, co-owner, ban, and secure-container checks.
- Improved sign owner-name resolution and decay display handling.
- Kept commit flow from leaving concealed players in a bad state.
- Tightened house destruction and multi targeting so destroy-multi can work from a target item or house serial.

### 2) Townstones, player towns, and spawnpoint data

Files involved:
- pkg/opt/spawnpoint/config/groups.cfg
- pkg/opt/spawnpoint/spawnpoint_container_objtypes.md
- pkg/opt/townstones/playertowns.cfg
- pkg/opt/townstones/textcmd/admin/townbankstatus.src
- pkg/opt/townstones/textcmd/player/playerruntowns.src
- pkg/opt/townstones/tstone.inc
- pkg/opt/townstones/tstone.src
- pkg/opt/townstones/upgrades.cfg
- pkg/opt/townstones/upgrades.xlsx
- pythonscripts/_sync_townstone_upgrades.py

Behavior changes:
- Added uncategorized spawn groups and refreshed the spawnpoint container documentation.
- Expanded town treasury/status tooling to show balances, upgrades, donations, and player-town availability.
- Added a player-run town overview gump with mayor, population, election, and feature-state columns.
- Synchronized townstone upgrades from the spreadsheet-backed source into the generated cfg.
- Updated player town config and townstone core data handling to match the new status screens.

### 3) Mount cleanup, guild houses, and movement reliability

Files involved:
- pkg/opt/alryc/textcmd/player/mounttest.src
- pkg/opt/alryc/textcmd/test/gotomulti.src
- pkg/opt/guilds/include/guilds.inc
- scripts/include/dismount.inc
- scripts/misc/chrdeath.src
- scripts/misc/death.src
- scripts/misc/rise.src

Behavior changes:
- Moved the mount test command from the test tree into the player command surface.
- Improved mount recovery when dismounting near death tiles by falling back to the standard relocation point.
- Kept donor mount and pet state metadata intact when dismounting.
- Tightened death and rise handling so stale mount state is less likely to survive a death cycle.
- Improved guild-house candidate selection and ownership checks, including account-based house ownership paths.

### 4) Snooping, stealing, vanity shop, and cache behavior

Files involved:
- pkg/opt/omegacache/omegacache.inc
- pkg/opt/vanityshop/vanityshop.src
- pkg/std/snooping/snooping.src
- pkg/std/snooping/stealing.src
- pkg/std/snooping/stealitems.cfg
- pkg/std/stealing/stealing.src

Behavior changes:
- Refined the Omega Cache keying rules and item-property handling.
- Kept item identity and shop inventory behavior in sync with the updated cache logic.
- Tightened snooping restrictions for bosses, vendors, and other protected targets.
- Improved stolen-item tracking and cleanup so snooping/stealing flows are less likely to leave stale item state behind.
- Updated the stealing path so stolen items get their flags cleared or preserved consistently depending on objtype.

### 5) Teleporters, AI, and world support scripts

Files involved:
- scripts/include/all.inc
- scripts/include/npcboosts.inc
- scripts/include/spawnnet.inc
- scripts/include/spawnpoint.inc
- scripts/include/teleporters.inc
- scripts/ai/animaltrainer.src
- scripts/ai/chaosmultikillpcs.src
- scripts/ai/humuc.src
- scripts/ai/instakillguard.src
- scripts/ai/main/assassinsleep.inc
- scripts/ai/main/mainloopsheep.inc
- scripts/ai/main/sleepmode.inc
- scripts/ai/merchant.src
- scripts/ai/newbiemultikillpcs.src
- scripts/ai/tamed.src
- scripts/ai/townguard.src
- scripts/ai/warrior.src

Behavior changes:
- Expanded teleporter and spawn support data.
- Reworked a number of AI safety and follow-state checks so NPCs recover more cleanly.
- Updated merchant, guard, animal, sheep, and combat AI helpers for consistency.
- Kept the shared include surface aligned with the new world and spawnpoint behavior.

### 6) Command and tooling refresh

Files involved:
- config/cmds.cfg
- config/command_synopses.cfg
- scripts/textcmd/admin/admin.src
- scripts/textcmd/admin/akill.src
- scripts/textcmd/admin/destroymulti.src
- scripts/textcmd/admin/equip.src
- scripts/textcmd/player/guilds.src
- scripts/textcmd/player/suicide.src
- scripts/textcmd/seer/info.src
- scripts/textcmd/seer/kill.src
- scripts/textcmd/test/animationtest.src
- scripts/textcmd/test/clearmount.src
- poltool.exe
- scripts/ecompile.exe
- scripts/runecl.exe
- uoconvert.exe
- uoconvert.txt
- uotool.exe
- tmp/diff_reports/phase_changes_annotated_review.md
- tmp/diff_reports/phase_changes_live_test_checklist.md

Behavior changes:
- Updated command registrations and synopsis text for the current utility surface.
- Refreshed admin, seer, and player command support tied to the new housing, mount, and townstone flows.
- Rebuilt the shipped tool binaries and refreshed the supporting diff-report artifacts.

## Validation Notes

- Diff range used: Patch-3.0.3..Patch-3.0.4
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - targeted reads of housing, townstone, mount, guild, and snooping scripts
- The changelog is file-complete for the Patch-3.0.4 branch delta.
