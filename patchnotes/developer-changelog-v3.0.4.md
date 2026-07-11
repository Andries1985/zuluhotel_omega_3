# Developer Changelog - v3.0.4

Range: Patch-3.0.3..Patch-3.0.4
Branch: Patch-3.0.4
Date: 2026-07-11

---

## Scope Summary

- Total files changed: 113
- Status breakdown: 78 modified, 25 added, 1 deleted, 9 renamed
- Net textual delta: 435637 insertions, 4825 deletions
- Largest generated/config shifts:
  - `pkg/opt/townstones/upgrades.cfg` (+377622)
  - `pkg/opt/alryc/config/nonanimatedgraphics.cfg` (+48842)
  - `pkg/opt/alryc/config/animatedgraphics.cfg` (+72 / -3134)
  - `config/npcdesc.cfg` (+1033 / -1066)
- Non-merge commits in range:
  - More Changes
  - Bunch of changes
  - moved some vanity item graphics
  - Moved around a few npcs to their categories
  - Reverted the reverted mount changes
  - Reverted mount changes
  - Merged changes in npcdesc.cfg. added categories for the wiki parser. updated bugged skills on vendors and fixed graphics
  - Gump Fix
  - Town Stone updates More teleporters
  - Snooping error fix, stealing snooping code fixes house sign fixes
  - Guilds fix and large patio house prints for failure to destroy
  - Another try to fix mount on death
  - Housing co-owner fixes housing sign access distance limit guilds housing check fix
  - Dismount fix for stale mounts on death
  - mounttest animation fix
  - Patch 3.0.4: move mounttest to player and add uncategorized spawn groups

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- M | config/cmds.cfg
- M | config/command_synopses.cfg
- M | config/npcdesc.cfg
- A | patchnotes/developer-changelog-v3.0.4.md
- M | patchnotes/developer-changelog.md
- M | patchnotes/launchernotes.md
- A | patchnotes/patch-v3.0.4.md
- A | pkg/multis/customhousing/config/icp.cfg
- A | pkg/multis/customhousing/config/itemdesc.cfg
- A | pkg/multis/customhousing/config/syshook.cfg
- A | pkg/multis/customhousing/include/house.inc
- A | pkg/multis/customhousing/include/housefriends.inc
- A | pkg/multis/customhousing/pkg.cfg
- A | pkg/multis/customhousing/scripts/customeHouseDeed.src
- A | pkg/multis/customhousing/sign.src
- A | pkg/multis/customhousing/signcontrol.src
- A | pkg/multis/customhousing/syshook/closecustomhouse.src
- M | pkg/multis/house/config/itemdesc.cfg
- M | pkg/multis/house/include/utility.inc
- M | pkg/multis/house/multiDeed/use.src
- M | pkg/multis/house/multiSign/method.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/multiCommands/commands/gm/destroymulti.src
- M | pkg/multis/staticHousing/sign/method.src
- M | pkg/opt/Events/textcmd/seer/createEventBag.src
- A | pkg/opt/alryc/animatedgraphics_regeneration_notes.txt
- M | pkg/opt/alryc/config/animatedgraphics.cfg
- A | pkg/opt/alryc/config/nonanimatedgraphics.cfg
- R100 | pkg/opt/alryc/textcmd/test/mounttest.src -> pkg/opt/alryc/textcmd/player/mounttest.src
- M | pkg/opt/alryc/textcmd/test/animatedgraphics.src
- A | pkg/opt/alryc/textcmd/test/animationtest.src
- A | pkg/opt/alryc/textcmd/test/clearmount.src
- R100 | scripts/textcmd/test/colorstest.src -> pkg/opt/alryc/textcmd/test/colorstest.src
- R100 | scripts/textcmd/test/createinbag.src -> pkg/opt/alryc/textcmd/test/createinbag.src
- R100 | scripts/textcmd/test/dupebag.src -> pkg/opt/alryc/textcmd/test/dupebag.src
- R100 | scripts/textcmd/test/editcharacter.src -> pkg/opt/alryc/textcmd/test/editcharacter.src
- R100 | scripts/textcmd/test/goteles.src -> pkg/opt/alryc/textcmd/test/goteles.src
- M | pkg/opt/alryc/textcmd/test/gotomulti.src
- A | pkg/opt/alryc/textcmd/test/nonanimatedgraphics.src
- R100 | scripts/textcmd/test/restartscript.src -> pkg/opt/alryc/textcmd/test/restartscript.src
- M | pkg/opt/dyteitems/usedyes.src
- R097 | scripts/textcmd/player/guilds.src -> pkg/opt/guilds/commands/player/guilds.src
- M | pkg/opt/guilds/include/guilds.inc
- M | pkg/opt/moongates/itemdesc.cfg
- M | pkg/opt/omegacache/categories.cfg
- M | pkg/opt/omegacache/itemdesc.cfg
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/powerscrolls/itemdesc.cfg
- M | pkg/opt/powerscrolls/randomTome.src
- M | pkg/opt/spawnpoint/config/groups.cfg
- R100 | spawnpoint_container_objtypes.txt -> pkg/opt/spawnpoint/spawnpoint_container_objtypes.md
- A | pkg/opt/townstones/playertowns.cfg
- M | pkg/opt/townstones/textcmd/admin/townbankstatus.src
- A | pkg/opt/townstones/textcmd/player/playerruntowns.src
- M | pkg/opt/townstones/tstone.inc
- M | pkg/opt/townstones/tstone.src
- M | pkg/opt/townstones/upgrades.cfg
- A | pkg/opt/townstones/upgrades.xlsx
- M | pkg/opt/vanityshop/customitemdye.src
- M | pkg/opt/vanityshop/customitemname.src
- M | pkg/opt/vanityshop/itemdesc.cfg
- M | pkg/opt/vanityshop/runebookdye.src
- M | pkg/opt/vanityshop/vanityshop.src
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/std/snooping/snooping.src
- M | pkg/std/snooping/stealing.src
- M | pkg/std/snooping/stealitems.cfg
- M | pkg/std/stealing/stealing.src
- M | poltool.exe
- M | pythonscripts/_gen_alryc_animatedgraphics_cfg.py
- A | pythonscripts/_sync_townstone_upgrades.py
- M | scripts/ai/animaltrainer.src
- M | scripts/ai/chaosmultikillpcs.src
- M | scripts/ai/humuc.src
- M | scripts/ai/instakillguard.src
- M | scripts/ai/main/assassinsleep.inc
- M | scripts/ai/main/mainloopsheep.inc
- M | scripts/ai/main/sleepmode.inc
- M | scripts/ai/merchant.src
- M | scripts/ai/newbiemultikillpcs.src
- M | scripts/ai/tamed.src
- M | scripts/ai/townguard.src
- M | scripts/ai/warrior.src
- M | scripts/control/can_insert_container.src
- M | scripts/control/can_remove_container.src
- M | scripts/ecompile.exe
- M | scripts/include/all.inc
- M | scripts/include/client.inc
- M | scripts/include/dismount.inc
- M | scripts/include/housecheck.inc
- M | scripts/include/npcboosts.inc
- M | scripts/include/spawnnet.inc
- M | scripts/include/spawnpoint.inc
- M | scripts/include/teleporters.inc
- M | scripts/misc/chrdeath.src
- A | scripts/misc/customhousecommit.src
- M | scripts/misc/death.src
- M | scripts/misc/rise.src
- M | scripts/runecl.exe
- M | scripts/textcmd/admin/admin.src
- M | scripts/textcmd/admin/akill.src
- M | scripts/textcmd/admin/destroymulti.src
- M | scripts/textcmd/admin/equip.src
- M | scripts/textcmd/player/suicide.src
- M | scripts/textcmd/seer/info.src
- M | scripts/textcmd/seer/kill.src
- D | scripts/textcmd/test/animationtest.src
- A | tmp/animatedgraphics_animdata_diff.txt
- A | tmp/diff_reports/phase_changes_annotated_review.md
- A | tmp/diff_reports/phase_changes_live_test_checklist.md
- M | uoconvert.exe
- M | uoconvert.txt
- M | uotool.exe

---

## Detailed Changes By Theme

### 1) Housing and Multi Control

Files involved:
- `pkg/multis/customhousing/**`
- `pkg/multis/house/include/utility.inc`
- `pkg/multis/house/multiDeed/use.src`
- `pkg/multis/house/multiSign/method.src`
- `pkg/multis/house/multiSign/use.src`
- `pkg/multis/multiCommands/commands/gm/destroymulti.src`
- `pkg/multis/staticHousing/sign/method.src`
- `scripts/misc/customhousecommit.src`
- `scripts/textcmd/admin/destroymulti.src`

Behavior changes:
- Reintroduced and expanded custom housing package files in-tree.
- Adjusted house sign ownership, co-owner/friend handling, and sign interaction flow.
- Updated deed/sign helper methods and destroy-multi command paths.
- Added dedicated custom house commit script hook.

### 2) Townstones and Player-Run Town Surfaces

Files involved:
- `pkg/opt/townstones/playertowns.cfg`
- `pkg/opt/townstones/textcmd/admin/townbankstatus.src`
- `pkg/opt/townstones/textcmd/player/playerruntowns.src`
- `pkg/opt/townstones/tstone.inc`
- `pkg/opt/townstones/tstone.src`
- `pkg/opt/townstones/upgrades.cfg`
- `pkg/opt/townstones/upgrades.xlsx`
- `pythonscripts/_sync_townstone_upgrades.py`

Behavior changes:
- Added player-run towns config and player-facing town status command.
- Expanded admin treasury status tooling with toggles and deeper region-state handling.
- Regenerated townstone upgrades at very large scale from source data.

### 3) Animated Graphics Tooling and Command Relocation

Files involved:
- `config/npcdesc.cfg`
- `pkg/opt/alryc/config/animatedgraphics.cfg`
- `pkg/opt/alryc/config/nonanimatedgraphics.cfg`
- `pkg/opt/alryc/animatedgraphics_regeneration_notes.txt`
- `pkg/opt/alryc/textcmd/test/animatedgraphics.src`
- `pkg/opt/alryc/textcmd/test/nonanimatedgraphics.src`
- `pkg/opt/alryc/textcmd/test/animationtest.src`
- `pkg/opt/alryc/textcmd/test/clearmount.src`
- `pythonscripts/_gen_alryc_animatedgraphics_cfg.py`
- `tmp/animatedgraphics_animdata_diff.txt`

Behavior changes:
- Added regenerated non-animated graphics config and refreshed animated graphics mappings.
- Moved/centralized multiple test commands under `pkg/opt/alryc/textcmd/test`.
- Added regeneration notes and diff artifacts for animated graphics processing.

### 4) Mount, Death, and Character-State Stability

Files involved:
- `scripts/include/dismount.inc`
- `scripts/misc/chrdeath.src`
- `scripts/misc/death.src`
- `scripts/misc/rise.src`
- `pkg/opt/alryc/textcmd/player/mounttest.src`
- `scripts/control/can_insert_container.src`
- `scripts/control/can_remove_container.src`

Behavior changes:
- Continued mount-on-death and stale mount cleanup corrections.
- Added/updated mount utility testing commands and moved mounttest into player command scope.
- Adjusted container insertion/removal control checks used by character state transitions.

### 5) Guilds, Snoop/Steal, Vanity, and Cache Follow-up

Files involved:
- `pkg/opt/guilds/commands/player/guilds.src`
- `pkg/opt/guilds/include/guilds.inc`
- `pkg/std/snooping/snooping.src`
- `pkg/std/snooping/stealing.src`
- `pkg/std/snooping/stealitems.cfg`
- `pkg/std/stealing/stealing.src`
- `pkg/opt/vanityshop/*.src`
- `pkg/opt/vanityshop/itemdesc.cfg`
- `pkg/opt/omegacache/*.cfg`
- `pkg/opt/omegacache/omegacache.inc`

Behavior changes:
- Moved guilds player command into guild package command path and adjusted guild include behavior.
- Applied snooping/stealing validation and item handling fixes.
- Updated vanity item dye/name/runebook handling and associated descriptors.
- Applied cache category/itemdesc/inc consistency updates.

### 6) World Support, Teleporters, and Misc Command Surface

Files involved:
- `pkg/opt/spawnpoint/config/groups.cfg`
- `pkg/opt/spawnpoint/spawnpoint_container_objtypes.md`
- `scripts/include/teleporters.inc`
- `scripts/include/spawnnet.inc`
- `scripts/include/spawnpoint.inc`
- `scripts/include/housecheck.inc`
- `scripts/include/client.inc`
- `scripts/ai/**/*.src`
- `scripts/textcmd/admin/*.src`
- `scripts/textcmd/seer/*.src`
- `scripts/textcmd/player/suicide.src`

Behavior changes:
- Added uncategorized spawn groups and refreshed spawnpoint object-type documentation.
- Expanded teleporter include data and related support includes.
- Performed broad AI cleanup pass across multiple creature/guard scripts.
- Continued admin/seer command maintenance tied to world operations.

### 7) Tooling, Binaries, and Conversion Assets

Files involved:
- `poltool.exe`
- `scripts/ecompile.exe`
- `scripts/runecl.exe`
- `uoconvert.exe`
- `uoconvert.txt`
- `uotool.exe`

Behavior changes:
- Updated shipped toolchain binaries and conversion text to align with the branch content.

---

## Validation Notes

- Diff range used: `Patch-3.0.3..Patch-3.0.4`
- Coverage checks used:
  - `git diff --name-status`
  - `git diff --numstat`
  - `git log --no-merges --oneline`
- This changelog is exhaustive for the branch delta at the time of generation.
