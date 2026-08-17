# Phase Changes Annotated Review

Scope:
- scripts/ai/chaosmultikillpcs.src
- scripts/ai/newbiemultikillpcs.src
- scripts/misc/death.src
- scripts/misc/rise.src

## High Impact Behavior Restorations

1. Rise loot transfer chain restored end-to-end
- Reanimated critters now set RiseLootTransfer and transferred items now set KeepOnNoLootDeath.
- Added in:
  - scripts/ai/chaosmultikillpcs.src
  - scripts/ai/newbiemultikillpcs.src
  - scripts/misc/rise.src
- Death cleanup now preserves marked transferred items for no-loot corpses and removes temporary flags afterward.
- Added in:
  - scripts/misc/death.src

Gameplay impact:
- Reanimated mobs no longer silently lose transferred corpse items during no-loot cleanup.
- Temporary transfer flags are cleaned after use to avoid persistence side effects.

2. Chaosmulti merge flow now exits after merge action
- Added return after MakeLord branch in scripts/ai/chaosmultikillpcs.src.

Gameplay impact:
- Prevents executing additional combat logic in same tick after merge event, reducing unstable post-merge behavior.

3. Target filtering regression fixed in chaosmulti/newbiemulti multi-target loops
- Restored filtering from broad critter check to critter != me and name mismatch checks in three loop groups per file.

Gameplay impact:
- Reduces self/friendly contamination in target selection.
- Restores expected hostile-target prioritization under crowded LOS conditions.

4. GM-item detection logic repaired
- Fixed wrong property read from ow1 into ow2 in both chaosmulti and newbiemulti.
- Preserved proper object references for IsGMItem checks in melee-archer scan path.

Gameplay impact:
- Correctly identifies Balders Dead item state and prevents false positives/negatives in anti-GM-weapon branch.

5. Weapon cleanup bug fixed
- Corrected DestroyItem target for off-hand cleanup from weaponone to weapontwo in both ReadyStaff and ReadySword paths.

Gameplay impact:
- Prevents destroying the wrong equipped weapon while swapping combat modes.

6. Chaosmulti split spawn now uses default relocation constants
- Replaced hardcoded britannia_alt spawn coords for split twins with default relocation constants.

Gameplay impact:
- Keeps split creation aligned with relocation policy and avoids hardwired map coupling.

## Realm Safety Status

Realm-safe call signatures were retained in all touched files:
- ListItemsNearLocation(..., realm)
- ListMobilesNearLocation(..., realm)
- CreateNpcFromTemplate(..., realm)
- CreateItemAtLocation(..., realm)
- MoveObjectToLocation(..., realm)

No realm-hardcoded rollback introduced by this patch set.

## Residual Risk Notes

1. AI behavior variance under massive LOS crowds
- While filtering was restored, chaos AI remains dense and branch-heavy. Large events should still be smoke-tested.

2. No-loot transfer edge cases
- Transfer behavior depends on corpse item ownership and timing between rise/death handlers. Validate with repeated reanimate-kill cycles.

3. Script parity drift over time
- chaosmultikillpcs and newbiemultikillpcs are now aligned for the repaired areas, but future edits should be mirrored intentionally.

## Quick File References

- scripts/ai/chaosmultikillpcs.src
- scripts/ai/newbiemultikillpcs.src
- scripts/misc/death.src
- scripts/misc/rise.src
