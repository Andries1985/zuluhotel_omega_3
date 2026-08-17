# Live Test Checklist - Chaosmulti and Rise Loot Transfer

Goal:
Validate the full behavior after the phase patch set.

## Pre-check

1. Restart scripts/server so updated AI and misc scripts are active.
2. Ensure staff character has tools to spawn/test chaosmultikillpcs and newbiemultikillpcs.
3. Enable normal logging path you use for validation.

## Test A - Rise transfer chain (chaosmulti)

1. Spawn a chaosmultikillpcs mob in a controlled test area.
2. Ensure nearby killable humanoid corpse exists with loot in corpse container.
3. Trigger reanimation branch and observe:
- Reanimated mob appears.
- Reanimated mob receives transferred items.
4. Kill reanimated mob in a condition where no-loot cleanup would apply.
5. Confirm:
- Items marked for transfer are not destroyed by no-loot cleanup.
- Temporary transfer properties are cleared after death handling.

Expected:
- No unintended item deletion from the reanimated transfer set.

## Test B - Rise transfer chain (newbiemulti)

Repeat Test A using newbiemultikillpcs.

Expected:
- Same behavior as chaosmulti path.

## Test C - Merge/split stability (chaosmulti)

1. Trigger merge event (MakeLord path).
2. Confirm no odd extra action burst occurs immediately after merge.
3. Trigger split event at low HP.
4. Confirm split twins spawn and relocate correctly without realm mismatch.

Expected:
- Merge branch exits cleanly.
- Split behavior works with current relocation constants.

## Test D - Target filtering sanity

1. Place mixed entities in LOS:
- self clone/ally-like NPC names
- valid hostile targets
2. Trigger crazy arch/melee/casting branches.
3. Confirm AI does not select itself and avoids obvious invalid targets.

Expected:
- Hostile target selection remains coherent.

## Test E - Weapon mode switching

1. Force repeated swaps between melee, staff, and bow modes.
2. Confirm off-hand and main-hand cleanup does not destroy wrong item.

Expected:
- No broken equip state after repeated mode transitions.

## Test F - Balders Dead handling

1. Present opponent with and without Balders Dead condition.
2. Confirm anti-GM weapon branch responds correctly.

Expected:
- Correct branch behavior with no inverted logic symptoms.

## Pass Criteria

1. No script errors in runtime logs from touched files.
2. Reanimated transfer path preserves intended items.
3. No realm-cross anomalies during create/move/list calls.
4. Chaosmulti/newbiemulti behavior is consistent for repaired paths.

## Touched Files

- scripts/ai/chaosmultikillpcs.src
- scripts/ai/newbiemultikillpcs.src
- scripts/misc/death.src
- scripts/misc/rise.src
