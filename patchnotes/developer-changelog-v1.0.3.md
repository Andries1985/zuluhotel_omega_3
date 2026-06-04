# Developer Changelog - v1.0.3

Range: 6fa3a28 (Patch-1.0.2 tip) -> e8e0338 (Patch-1.0.3 tip)  
Branch: Patch-1.0.3  
Date: 2026-06-03

---

## Scope Summary

- Total files changed: 18
- Text/config/script files changed: 13
- Binary files changed: 5
- Net textual delta: 3966 insertions, 826 deletions

---

## Complete File Inventory (Exhaustive)

| File | Status | + / - | Notes |
|---|---|---:|---|
| breaking-changes.txt | Modified | +6 / -1 | POL100.3.0 work-in-progress breaking note added. |
| config/npcdesc.cfg | Modified | +901 / -495 | Large mount/NPC template remap and normalization pass. |
| config/startloc.cfg | Modified | +5 / -59 | Replaced multi-city entries with one Shandalaar start entry. |
| core-changes.txt | Modified | +7 / -0 | Added POL100.3.0 core engine/module changes. |
| pkg/opt/spawnpoint/config/groups.cfg | Modified | +157 / -2 | Added new mount coverage group and removed two legacy spawns. |
| poltool.exe | Modified (binary) | n/a | Rebuilt/replaced binary artifact. |
| scripts/ai/animaltrainer.src | Modified | +75 / -14 | Trainer reclaim and item-return correctness updates. |
| scripts/ai/animaltrainer.src.bak | Added | +802 / -0 | Backup snapshot added in this branch. |
| scripts/ai/merchant.src | Modified | +1 / -1 | Fixed item return argument order for rejected coins. |
| scripts/ai/tamed.src | Modified | +558 / -252 | Large ZH2.5 parity pull and behavior hardening. |
| scripts/ai/tamed.src.bak | Added | +1450 / -0 | Full backup snapshot added in this branch. |
| scripts/ecompile.exe | Modified (binary) | n/a | Rebuilt/replaced binary artifact. |
| scripts/misc/oncreate.src | Modified | +1 / -1 | Updated forced new-character spawn coordinate. |
| scripts/modules/party.em | Modified | +2 / -0 | Added module declaration for ListParties(). |
| scripts/runecl.exe | Modified (binary) | n/a | Rebuilt/replaced binary artifact. |
| uoconvert.cfg | Modified | +1 / -1 | Extended mount tile list with additional mount graphics. |
| uoconvert.exe | Modified (binary) | n/a | Rebuilt/replaced binary artifact. |
| uotool.exe | Modified (binary) | n/a | Rebuilt/replaced binary artifact. |

---

## Detailed Changes By File

### 1) breaking-changes.txt

- Header advanced from POL100.2.0 work-in-progress section to POL100.3.0 work-in-progress section.
- Added 2026-05-25 Turley breaking behavior note:
  - sending classes/functionobjects to another script can no longer access globals if sender is destroyed before call,
  - caller now errors/stops in that situation.
- Retained previous POL100.2.0 section below the new header.

### 2) core-changes.txt

Added new POL100.3.0 entries:
- 2026-05-25 Turley:
  - fixed memory leak storing classes/functionobjects as globals,
  - updated script-call behavior for global access after sender destruction.
- 2026-05-23 Kevin:
  - added party::ListParties() module function.

### 3) scripts/modules/party.em

- Added module declaration line:
  - ListParties();
- This aligns exposed module declarations with new core function availability.

### 4) config/startloc.cfg

- Removed prior multi-entry starting location set.
- Kept one StartingLocation entry only.
- Updated values to:
  - City: Shandalaar
  - Description: Town Center
  - Coordinate: 1261,1841,61
  - MapID: 1
  - Cliloc: 1061250
  - Realm: britannia_alt

### 5) scripts/misc/oncreate.src

- Updated forced spawn move location for new characters:
  - from: 1433, 1861, 36
  - to: 1261, 1841, 61
- Realm remains britannia_alt.

### 6) uoconvert.cfg

- Mounts -> Tiles line extended with five additional mount graphics:
  - 0x3EDE
  - 0x3EDF
  - 0x3EE0
  - 0x3EE1
  - 0x3EE2
- Existing tile list preserved.

### 7) scripts/ai/merchant.src

- In rejected coin hand-in branch, corrected MoveItemToContainer call argument order:
  - old: MoveItemToContainer(ev.source.backpack, ev.item)
  - new: MoveItemToContainer(ev.item, ev.source.backpack)
- Effect: rejected currency returns to player backpack correctly.

### 8) scripts/ai/animaltrainer.src

Changed regions (exhaustive hunk coverage):
- Program variable setup:
  - removed unused buyimage/sellimage arrays.
- SYSEVENT_ITEM_GIVEN / pet item loop:
  - added null check around CreateNpcFromTemplate before applying ownership and restart.
  - corrected rejected coin return MoveItemToContainer argument order.
- Load_Ticket_Data():
  - added confiscation ticket branch for objtype 0xDF0C.
  - validates owner_serial against player serial.
  - reads confiscation_fine and attempts who.spendgold(fine).
  - recreates pet from template with null-check fallback.
  - restores name/color/mana and ownership/tamed script state.
  - applies nocut/noloot flags.
  - clamps PhysicalProtection if invalid (>100) and clears AttackTypeImmunities.
  - normalizes HP to GetMaxHp(newpet).
  - destroys confiscation ticket on success.
- Existing stable ticket branch (0x186E / graphic 5360):
  - added null-check around CreateNPCFromTemplate.
  - replaced unreliable ticket HP restore path with SetHp(newpet, GetMaxHp(newpet)).
- stable(who):
  - added target cancel handling; sends canceled message and returns when no target selected.

### 9) scripts/ai/tamed.src

This file is a large behavioral parity pull and hardening pass (+558/-252) with very broad region touch.

Changed regions (exhaustive hunk coverage by function/area):
- Global state and startup:
  - include cleanup and initialization refactor.
  - new vars: guardinghp, autonomous_defense, guardattackers, selfattackers, lastspeechkey, nextspeechallowed, awaitingtargetselection, ignorespeechuntil.
  - replaced direct backpack creation block with EnsureMyPack().
- TamedAI() entry/setup:
  - master-null early wild fallback.
  - caster/poison detection changed to data-driven checks (spell arrays / Poisoning skill).
- Fight(opponent):
  - state preservation expanded (oldguarding/oldstaying).
  - guard command now tracks target and guard HP baseline.
  - engaged/damaged handling retuned.
  - poison spell targeting corrected to current opponent.
  - low-HP flee branch disabled/commented in this path.
  - loop exhaustion now restores prior command state instead of forced flee.
  - post-combat follow restore simplified.
- CloseDistance(opponent):
  - engagement distance thresholds retuned (10 and 2 bounds).
  - corrected conditional syntax/flow in move-away and move-toward checks.
- Transfer():
  - added EBSummons restriction.
  - poisoner now included in advanced transfer/taming gate path.
- Release():
  - summon/animated/charmed release death path now uses me.kill() in place of raw-damage kill pattern.
  - relocation constants normalized to NPC relocation defaults.
- Fetch(), drop(), TakeItem(), MainAILoop(), Guard(), ProcessSpeech(), DoPoison(), flee(), GoWild():
  - substantial command and speech handling hardening.
  - additional anti-spam timing and target-selection state controls.
  - flow cleanup and guard/command behavior normalization.
- New helper functions added:
  - EnsureMyPack()
  - AcquireTargetWithSpeechLock()
  - ReturnItemToSourcePackOrGround()
- SpecialFrenzyRelease() block is commented out in this revision.
- OpenMyPack() logic significantly revised:
  - robust master reacquisition (including offline search fallback),
  - ensure-pack behavior,
  - mount restrictions/diagnostics and expanded mapping handling updates.

### 10) config/npcdesc.cfg

Large-scale mount/NPC template remap and definition refresh (+901/-495).

Exhaustive high-level content changes in this patch range:
- Mount template block around the horse/ostard/kirin/unicorn/ridgeback/ethereal families was heavily rewritten.
- Multiple template identities/scripts/objtypes/equip mappings were realigned against hardcoded mount graphics behavior.
- Added or refreshed templates visible in this diff extraction include:
  - darksteed, etherealhorse, nightmare, silversteed,
  - warhorsepurple, warhorselightblue, warhorsebloodred, warhorselightgreen,
  - unicorn, kirin, seahorsemediumblue,
  - beetle, etherealllama, etherealostard,
  - nightmare2, nightmare3, nightmare4,
  - ridgeback, savageridgeback,
  - etherealkirin, etherealunicorn, etherealridgeback, etherealarmoredswampdragon,
  - horse2, horse, desertostard, polarbear, frenziedostard, forestostard, llama, horse3, horse4,
  - boura, giantbeetle2, hellhound, tarantula, frostmite, serpentinedragon,
  - clydsdale, horseelementalearth, horseelementalfire, horseelementalwater, horseelementalair.
- Existing templates in this region were also normalized (stats/skills/equip/script/object fields) to match revised mount behavior expectations.

### 11) pkg/opt/spawnpoint/config/groups.cfg

- Removed one spawn entry from group 9:
  - removed: banehorn
- Removed one spawn entry from group 67:
  - removed: blama
- Added new group 150 titled Hardcoded Mount Graphics (All Matching NPC Templates).
- Group 150 contains a comprehensive spawn list covering hardcoded mount graphics and mapped templates (horses, ostards, ridgebacks, swamp dragons, specialty mounts, elemental horses, canine mounts, etc.).

### 12) scripts/ai/animaltrainer.src.bak

- New backup file added.
- Contains branch snapshot of trainer AI logic as imported/carried with this patch.

### 13) scripts/ai/tamed.src.bak

- New backup file added.
- Contains full backup snapshot of expanded tamed AI logic.

### 14) Binary Artifacts

Files:
- poltool.exe
- scripts/ecompile.exe
- scripts/runecl.exe
- uoconvert.exe
- uotool.exe

Notes:
- All five are binary replacements in this range.
- Source-level diff is not available via text diff.
- Sizes in stat output remained unchanged, indicating rebuild/refresh or equivalent-binary replacement in branch history.

---

## Validation Method Used For This Changelog

- Diff range used: git diff 6fa3a28..e8e0338
- Coverage checks used:
  - name-status inventory,
  - numstat counts,
  - stat summary,
  - targeted full diffs for smaller files,
  - targeted large-file extraction for npcdesc/tamed/spawnpoint/animaltrainer.

This document intentionally enumerates all files touched in the branch range and records their concrete changes at file level.
