# Exhaustive Mod-Token Parity Report (ZH2.5 vs ZH3.0)

Strict scope: only files and lines directly affecting `DoTempMod` / `CanMod` tokens (`all`,`ebless`,`poly`,`str`,`dex`,`int`,`ar`) plus core mod-amount/duration functions.

## Summary

- Relevant files scanned: **35**
- MATCH: **35**
- DIFF: **0**
- ONLY_ZH2.5: **0**
- ONLY_ZH3.0: **0**

## By Category

- fish: total=1, diff=0, only25=0, only3=0, match=1
- other: total=5, diff=0, only25=0, only3=0, match=5
- potions: total=3, diff=0, only25=0, only3=0, match=3
- spells: total=26, diff=0, only25=0, only3=0, match=26

## Non-Matching Files

All strict-scope files match.
## Matching Files

- pkg/opt/GMItems/staffofnagash_usescript.src [other]
- pkg/opt/alchemyplus/newpotions.src [potions]
- pkg/opt/earth/earthblessing.src [spells]
- pkg/opt/earth/shapeshift.src [spells]
- pkg/opt/earth/shiftingearth.src [spells]
- pkg/opt/holybook/enlightenment.src [spells]
- pkg/opt/holybook/seraphimswill.src [spells]
- pkg/opt/necro/decayingray.src [spells]
- pkg/opt/necro/liche.src [spells]
- pkg/opt/necro/wraithform.src [spells]
- pkg/opt/songbook/songofdefense.src [spells]
- pkg/opt/songbook/songofglory.src [spells]
- pkg/opt/songbook/songofhaste.src [spells]
- pkg/opt/summoning/polymorphing.src [spells]
- pkg/std/alchemy/bluepotion.src [potions]
- pkg/std/alchemy/whitepotion.src [potions]
- pkg/std/fishing/magicfish.src [fish]
- pkg/std/spells/agility.src [spells]
- pkg/std/spells/archprot.src [spells]
- pkg/std/spells/bless timer.src [spells]
- pkg/std/spells/bless.src [spells]
- pkg/std/spells/clumsy.src [spells]
- pkg/std/spells/cunning.src [spells]
- pkg/std/spells/curse.src [spells]
- pkg/std/spells/feeblemind.src [spells]
- pkg/std/spells/masscurse.src [spells]
- pkg/std/spells/polymorph.src [spells]
- pkg/std/spells/protection with timer.src [spells]
- pkg/std/spells/protection.src [spells]
- pkg/std/spells/strength.src [spells]
- pkg/std/spells/weaken.src [spells]
- scripts/include/dotempmods.inc [other]
- scripts/include/npccast.inc [other]
- scripts/include/npccastspells.inc [other]
- scripts/include/statmod.inc [other]
