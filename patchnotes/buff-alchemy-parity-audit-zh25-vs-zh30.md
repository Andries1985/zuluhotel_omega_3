# Buff + Alchemy Parity Audit (ZH2.5 -> ZH3.0)

Scope: spells and potion systems that affect temp stat/AR mods and related checks (`CanMod`, `DoTempMod`, `GetModAmount`, `GetModDuration`) with focus on `all`, `ebless`, `poly`, `str`, `dex`, `int`, `ar`.

## High-impact differences

### 1) Core temp-mod engine drift
File: `scripts/include/dotempmods.inc`

- `AddToStatMods` behavior changed:
  - ZH2.5: blocks conflicting families via `TempModConflicts` (str-family, dex-family, int-family, bless/poly family, AR family, and AR<->poly cross-conflict).
  - ZH3.0: only blocks exact same key (`allmods[i][1] == mmod[1]`).
- ZH3.0 removed conflict helper functions used by ZH2.5:
  - `TempModTouchesStrength`, `TempModTouchesDexterity`, `TempModTouchesIntelligence`, `TempModIsBlessPoly`, `TempModIsPoly`, `TempModTouchesArmor`, `TempModConflicts`.
- `DoTempMod` scales amount down in ZH3.0:
  - ZH2.5: `parms[3] := CInt(amt)`
  - ZH3.0: `parms[3] := CInt(amt*0.8)`
- `GetModAmount` formula differs:
  - ZH2.5: `5 + CInt(magery/7)` (+ magic efficiency only for mage class)
  - ZH3.0: `RandomInt(5) + CInt(magery/7)` (then always modified by magic efficiency)
- `GetModDuration` formula differs:
  - ZH2.5: `(magery * 20) + 5` (magic-eff line commented out)
  - ZH3.0: `magery * 5`, then magic efficiency, then `*2`.

Impact: this single file changes effective power and stacking behavior for nearly every buff/debuff spell and buff potion.

### 2) Holybook class scaling + class property key drift
Files:
- `pkg/opt/holybook/enlightenment.src`
- `pkg/opt/holybook/seraphimswill.src`

- ZH2.5 uses class flags like `CLASSEID_MAGE`, `CLASSEID_PALADIN`, `CLASSEID_POWERPLAYER`.
- ZH3.0 uses string props (`"IsMage"`, `"IsPaladin"`, `"IsPowerplayer"`) and different multipliers.
- `enlightenment` multipliers:
  - ZH2.5: Mage `x1`, Paladin `x3`, Powerplayer `x3`, default `x1`, AR from `mod_amount * 0.5 + 1`.
  - ZH3.0: Paladin `x5`, Mage `x1`, Powerplayer `x3`, default `x3`, AR from `mod_amount / 1.25`.
- `seraphimswill` behavior drift:
  - ZH2.5 applies `DoTempMod(..., "all", ...)`.
  - ZH3.0 applies `DoTempMod(..., "poly", ...)` in active code and has additional commented-out alternate logic.

Impact: direct class balance changes and changed buff channel (`all` vs `poly`) for Seraphim's Will.

### 3) Earth spell scaling drift
Files:
- `pkg/opt/earth/earthblessing.src`
- `pkg/opt/earth/shapeshift.src`

- `earthblessing` multipliers:
  - ZH2.5: Mage `x1.5`, Mystic Archer `x3`, Powerplayer `x3`, default `x1`.
  - ZH3.0: Mage `x4.25`, Mystic Archer `x4.25`, Powerplayer `x2`, default `x3`.
- `shapeshift` multipliers:
  - ZH2.5: Mage `x1.25`, Mystic Archer `x3`, Powerplayer `x3`, default `x1`.
  - ZH3.0: Mage `x2.75`, Mystic Archer `x3`, Powerplayer `x2`, default `x3`.

Impact: very large statmod and effective AR shifts during self-buff transformations.

### 4) AR spell conflict-guard changes
Files:
- `pkg/std/spells/protection.src`
- `pkg/std/spells/protection with timer.src`
- `pkg/std/spells/archprot.src`

- ZH2.5 explicitly checks active `#mods` and blocks if any of: `ar`, `car`, `poly`, `cpoly` are active.
- ZH3.0 removed those explicit checks and relies only on `CanMod(cast_on, "ar")`.
- In `archprot`, ZH2.5 had per-target "blocked" messaging for armor-affecting buffs; ZH3.0 dropped it.

Impact: stacking/polymorph interaction differs unless restored by the core conflict model in `dotempmods.inc`.

### 5) Alchemy buff behavior drift
Files:
- `pkg/std/alchemy/bluepotion.src`
- `pkg/std/alchemy/whitepotion.src`
- `pkg/opt/alchemyplus/newpotions.src`

Base alchemy (`bluepotion`, `whitepotion`):
- ZH2.5 uses rank-based deterministic scaling (`rank * 10 + 5`, `(rank + 1) * 480`) and TrueMage rank boosts.
- ZH3.0 uses `GetPotionStrength` + dice (`RandomDiceStr(Cstr(power*2)+"d5")`) and `power * DURATION_MULTIPLIER`.

AlchemyPlus (`newpotions`):
- ZH2.5 has stricter guards (for example bless/poly paths requiring `CanMod(all) && CanMod(ebless) && CanMod(poly)`), plus stronger fixed/tier scaling in several effects.
- ZH3.0 simplifies to weaker random formulas in key places (`int`, `ar`, `all`, `poly`) and relaxes some checks.
- Examples:
  - protection-like effect: ZH2.5 `strength * 2` + armor conflict guard; ZH3.0 `RandomDiceStr(strength+"d2")`.
  - bless-like effect: ZH2.5 tier logic (`tier * 15`) for some itemtypes; ZH3.0 `RandomDiceStr(strength+"d3")`.
  - poly-like effect: ZH2.5 up to fixed tier values and strict triplet gating; ZH3.0 `(strength*5)+critter` with only `CanMod("poly")`.

Impact: potion buff strength and anti-stacking safety are materially different.

## Files checked and status

DIFF:
- `scripts/include/dotempmods.inc`
- `pkg/std/spells/protection.src`
- `pkg/std/spells/protection with timer.src`
- `pkg/std/spells/archprot.src`
- `pkg/std/spells/masscurse.src` (realm param only)
- `pkg/opt/holybook/enlightenment.src`
- `pkg/opt/holybook/seraphimswill.src`
- `pkg/opt/earth/earthblessing.src`
- `pkg/opt/earth/shapeshift.src`
- `pkg/opt/earth/owlsight.src` (realm param only)
- `pkg/std/alchemy/bluepotion.src`
- `pkg/std/alchemy/whitepotion.src`
- `pkg/opt/alchemyplus/newpotions.src`

SAME:
- `pkg/std/spells/bless.src`
- `pkg/std/spells/bless timer.src`
- `pkg/std/spells/strength.src`
- `pkg/std/spells/agility.src`
- `pkg/std/spells/cunning.src`
- `pkg/std/spells/polymorph.src`
- `pkg/std/spells/curse.src`
- `pkg/std/spells/weaken.src`
- `pkg/std/spells/clumsy.src`
- `pkg/std/spells/feeblemind.src`
- `pkg/opt/earth/shiftingearth.src`

## What it takes to implement parity (ZH2.5 behavior in ZH3.0)

### Minimal parity pass (safe, focused)
1. Restore core conflict and formula behavior in `scripts/include/dotempmods.inc`:
   - Reintroduce `TempModConflicts` family checks and helper funcs.
   - Remove `*0.8` scaling from `DoTempMod` amount.
   - Restore ZH2.5 `GetModAmount` and `GetModDuration` formulas.
2. Restore class key usage and multipliers in:
   - `pkg/opt/holybook/enlightenment.src`
   - `pkg/opt/holybook/seraphimswill.src`
   - `pkg/opt/earth/earthblessing.src`
   - `pkg/opt/earth/shapeshift.src`
3. Restore AR conflict checks in:
   - `pkg/std/spells/protection.src`
   - `pkg/std/spells/protection with timer.src`
   - `pkg/std/spells/archprot.src`
4. Restore base alchemy scaling in:
   - `pkg/std/alchemy/bluepotion.src`
   - `pkg/std/alchemy/whitepotion.src`
5. Restore AlchemyPlus formulas and gates in:
   - `pkg/opt/alchemyplus/newpotions.src`

### Optional parity pass
- Revert only non-balance-neutral bits in `masscurse`/`owlsight` if desired (these differences are realm-safety parameters, not buff math).

## Estimated implementation effort

- Code changes only: medium (roughly 10-14 files touched).
- Verification: medium-high.
  - Recompile changed scripts.
  - Run in-game checks for stack conflicts (`all/ebless/poly/ar`) and class-specific scaling (Mage/Paladin/Powerplayer/Mystic Archer).
  - Test potion tiers and TrueMage interactions.

Approximate active time: 2-4 focused hours for code + first-pass validation; additional balancing/QA iteration likely needed depending on intended final meta.
