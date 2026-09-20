# Developer Changelog - v3.1.1

Range: Patch-3.1.0..Patch-3.1.1 (commit `b0ace6e`..`9aa5216`)
Branch: Patch-3.1.1
Date: 2026-09-19

---

## Scope Summary

- Total files changed: 558 (293 added, 242 modified, 8 deleted, 15 renamed)
- Net textual delta: 68,497 insertions, 9,527 deletions
- Largest shifts:
  - `ainotes/missing-equipment-static-overlap-20260831.json` (+34,850/-1,659) — re-run output of a dev audit tool, not game content
  - `pkg/opt/alryc/textcmd/test/alryciteminfo.src` (+3,588, new file) — full rebuild of the enchant/property item-info editor tool
  - `pkg/items/deed/config/itemdesc.cfg` (+2,565 net) — the carpentry deed backfill's item-side registration
  - `pkg/systems/combat/config/itemdesc.cfg` (+1,811 net) — new home for weapon itemdesc data after the shilhook split, plus 36 new enchanted throwing-weapon variants
  - `pkg/opt/alryc/textcmd/test/testadminpanel.src` (+1,685, new file) and `pkg/opt/admin/include/adminpanel.inc` (+841, new file) — POL2.5 admin panel port
  - `scripts/include/sounds.inc` (+1,669 net) — full mechanical port of every client sound-effect id from `soundLegacyMUL.uop`, replacing a ~120-entry curated subset
  - `pkg/opt/alryc/config/missingequipment.cfg` (+959, new file) and `ainotes/user-sort-layer1-20260904.json` (+627, new file) — dev audit tool data, not game content
  - `config/npcdesc.cfg` (+781 net) — new boss/quest-giver NPC templates for the ritual and fishing quest lines, plus spawn-pool touch-ups
  - `pkg/opt/omegacache/categories.cfg` (+265 net) — category/item ordering rework
  - `pkg/std/tinkering/tinkeringfunctions.inc` (+784, new), `pkg/std/blacksmithy/blacksmithgump.inc` (+343, new), `pkg/std/alchemy/alchemyfunctions.inc` (+507, new) — shared per-skill logic extracted for the crafting-station rollout
  - ~190 new one-shot `pkg/items/deed/built/*.cfg` files (avg. 4-15 lines each) — the carpentry/furniture deed backfill; counted once here rather than individually
- Non-merge commits in range (oldest to newest):
  - `1418633` Compile Fix
  - `d1c181d` Bulk order rewards started fishing trophies working
  - `043f44b` carpentry update and redeed fixes fishing net fix
  - `77ce910` Alrycs Item Info Update
  - `d01d089` Fishing quests updates to staff commands many bug fixes
  - `24d06d2` More fixes
  - `2e2b24a` Bunch of fixes
  - `ec26d19` More updates
  - `5faf84b` Dispel Fixes
  - `f6410d6` More Fixes
  - `5759bcd` POL fix and throwing weapon update
  - `d930368` Redeed update
  - `ced49df` Reconnect issues Fix for equipping 2 1 handed weapons at the same time
  - `9aa5216` Equip fix
- The 7 merge commits (PR #97-103, `dcf2bfb`..`4a742e3`) are no-op integration merges: a file-level diff of the 14 non-merge commits combined reconciles exactly against the full `b0ace6e..9aa5216` diff once renames are accounted for (confirmed by comparing `git show --name-only` across all 14 commits against `git diff --name-status`) — they carry no content beyond what the commits above already contain.
- Themes below are organized by subsystem, not by commit, since several commits (`d01d089`, `2e2b24a`, `ec26d19`, `f6410d6`) each touch 70-280 files spanning many unrelated fixes and new systems built up over a long working session.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File` (A=added, M=modified, D=deleted, R=renamed as `old -> new`)

- A | .claude/skills/escript-gotchas/SKILL.md
- A | .claude/subagent-briefing.md
- A | ainotes/crafting-station-audit-20260909.md
- M | ainotes/missing-equipment-static-overlap-20260831.json
- A | ainotes/user-sort-layer1-20260904.json
- M | breaking-changes.txt
- M | config/cmds.cfg
- M | config/command_synopses.cfg
- M | config/equip.cfg
- M | config/itemdesc.cfg
- M | config/mrcspawn.cfg
- M | config/nlootgroup.cfg
- M | config/npcdesc.cfg
- M | config/starteqp.cfg
- M | core-changes.txt
- M | pkg/commands/commands/gm/mobedit.src
- M | pkg/items/anvil/config/itemdesc.cfg
- M | pkg/items/banners/config/itemdesc.cfg
- M | pkg/items/carpets/config/itemdesc.cfg
- M | pkg/items/celestial/config/itemdesc.cfg
- M | pkg/items/clocks/config/itemdesc.cfg
- M | pkg/items/commcrystals/config/itemdesc.cfg
- M | pkg/items/containers/config/itemdesc.cfg
- M | pkg/items/containers/container/canInsert.src
- M | pkg/items/containers/container/canRemove.src
- A | pkg/items/coopersbench/itemdesc.cfg
- A | pkg/items/coopersbench/pkg.cfg
- M | pkg/items/crystalThemePack/config/itemdesc.cfg
- M | pkg/items/decorativeArmor/config/itemdesc.cfg
- M | pkg/items/decorativeShields/config/itemdesc.cfg
- M | pkg/items/decorativeWeapons/config/itemdesc.cfg
- A | pkg/items/deed/built/amalgamator.cfg
- A | pkg/items/deed/built/autoloom.cfg
- A | pkg/items/deed/built/bambooScreenCorner.cfg
- A | pkg/items/deed/built/bambooScreenEast.cfg
- A | pkg/items/deed/built/bambooScreenOpen1.cfg
- A | pkg/items/deed/built/bambooScreenOpen2.cfg
- A | pkg/items/deed/built/bambooScreenSouth.cfg
- A | pkg/items/deed/built/basket.cfg
- A | pkg/items/deed/built/blacksmithMachine.cfg
- A | pkg/items/deed/built/boneTable.cfg
- A | pkg/items/deed/built/butcherBlock.cfg
- A | pkg/items/deed/built/carpentryBench.cfg
- A | pkg/items/deed/built/carpentryBench3.cfg
- A | pkg/items/deed/built/carpentryBox.cfg
- A | pkg/items/deed/built/carpentryBrownChestOfDrawers.cfg
- A | pkg/items/deed/built/carpentryBrownClosedArmoire.cfg
- A | pkg/items/deed/built/carpentryBrownOpenArmoire.cfg
- A | pkg/items/deed/built/carpentryChair.cfg
- A | pkg/items/deed/built/carpentryChair2.cfg
- A | pkg/items/deed/built/carpentryChair3.cfg
- A | pkg/items/deed/built/carpentryChair4.cfg
- A | pkg/items/deed/built/carpentryChair5.cfg
- A | pkg/items/deed/built/carpentryDartBoard.cfg
- A | pkg/items/deed/built/carpentryDressForm.cfg
- A | pkg/items/deed/built/carpentryEmptyBookShelf.cfg
- A | pkg/items/deed/built/carpentryFlourMill2.cfg
- A | pkg/items/deed/built/carpentryFullBookShelf1.cfg
- A | pkg/items/deed/built/carpentryFullBookShelf2.cfg
- A | pkg/items/deed/built/carpentryFullBookShelf3.cfg
- A | pkg/items/deed/built/carpentryGiftBox.cfg
- A | pkg/items/deed/built/carpentryLargeCrate.cfg
- A | pkg/items/deed/built/carpentryLoom2.cfg
- A | pkg/items/deed/built/carpentryLoomBench.cfg
- A | pkg/items/deed/built/carpentryMediumCrate.cfg
- A | pkg/items/deed/built/carpentryMusicStand.cfg
- M | pkg/items/deed/built/carpentryOven.cfg
- A | pkg/items/deed/built/carpentryOven2.cfg
- A | pkg/items/deed/built/carpentryPainting.cfg
- A | pkg/items/deed/built/carpentryPainting2.cfg
- A | pkg/items/deed/built/carpentryPickpocketsDip.cfg
- A | pkg/items/deed/built/carpentryPirateChest.cfg
- A | pkg/items/deed/built/carpentryPouch.cfg
- A | pkg/items/deed/built/carpentryRedChestOfDrawers.cfg
- A | pkg/items/deed/built/carpentryRedClosedArmoire.cfg
- A | pkg/items/deed/built/carpentryRedOpenArmoire.cfg
- A | pkg/items/deed/built/carpentrySmallCrate.cfg
- A | pkg/items/deed/built/carpentrySmallSquareBasket.cfg
- A | pkg/items/deed/built/carpentrySquareBasket.cfg
- A | pkg/items/deed/built/carpentryStatue2.cfg
- A | pkg/items/deed/built/carpentryStatue3.cfg
- A | pkg/items/deed/built/carpentryStoneBench.cfg
- A | pkg/items/deed/built/carpentryStoneChair.cfg
- A | pkg/items/deed/built/carpentryStool.cfg
- A | pkg/items/deed/built/carpentryStool2.cfg
- A | pkg/items/deed/built/carpentryStrongBox.cfg
- A | pkg/items/deed/built/carpentryThrone.cfg
- A | pkg/items/deed/built/carpentryVat.cfg
- A | pkg/items/deed/built/carpentryVatGrapes.cfg
- A | pkg/items/deed/built/carpentryVatWater.cfg
- A | pkg/items/deed/built/carpentryWoodenChest.cfg
- A | pkg/items/deed/built/cartographyTable.cfg
- A | pkg/items/deed/built/cauldronStation.cfg
- A | pkg/items/deed/built/cherryArmoire.cfg
- A | pkg/items/deed/built/closedBarrel.cfg
- A | pkg/items/deed/built/colorfulTapestryEast.cfg
- A | pkg/items/deed/built/colorfulTapestrySouth1.cfg
- A | pkg/items/deed/built/colorfulTapestrySouth2.cfg
- A | pkg/items/deed/built/compassionTapestryEast.cfg
- A | pkg/items/deed/built/compassionTapestrySouth.cfg
- A | pkg/items/deed/built/cookingMachineStation.cfg
- A | pkg/items/deed/built/coopersbench.cfg
- A | pkg/items/deed/built/creepyPortraitEast1.cfg
- A | pkg/items/deed/built/creepyPortraitEast2.cfg
- A | pkg/items/deed/built/creepyPortraitEast3.cfg
- A | pkg/items/deed/built/creepyPortraitEast4.cfg
- A | pkg/items/deed/built/creepyPortraitSouth1.cfg
- A | pkg/items/deed/built/creepyPortraitSouth2.cfg
- A | pkg/items/deed/built/creepyPortraitSouth3.cfg
- A | pkg/items/deed/built/creepyPortraitSouth4.cfg
- A | pkg/items/deed/built/darkFlowerTapestryEast1.cfg
- A | pkg/items/deed/built/darkFlowerTapestryEast2.cfg
- A | pkg/items/deed/built/darkFlowerTapestrySouth1.cfg
- A | pkg/items/deed/built/darkFlowerTapestrySouth2.cfg
- A | pkg/items/deed/built/decorativeAcorn.cfg
- A | pkg/items/deed/built/disturbingPortraitEast1.cfg
- A | pkg/items/deed/built/disturbingPortraitEast2.cfg
- A | pkg/items/deed/built/disturbingPortraitEast3.cfg
- A | pkg/items/deed/built/disturbingPortraitEast4.cfg
- A | pkg/items/deed/built/disturbingPortraitSouth1.cfg
- A | pkg/items/deed/built/disturbingPortraitSouth2.cfg
- A | pkg/items/deed/built/disturbingPortraitSouth3.cfg
- A | pkg/items/deed/built/disturbingPortraitSouth4.cfg
- A | pkg/items/deed/built/dyeCabinet.cfg
- A | pkg/items/deed/built/elegantArmoire.cfg
- A | pkg/items/deed/built/elegantLowTable.cfg
- A | pkg/items/deed/built/elvenSpinningWheelA.cfg
- A | pkg/items/deed/built/elvenWallMapEast.cfg
- A | pkg/items/deed/built/elvenWallMapSouth.cfg
- A | pkg/items/deed/built/enchanter.cfg
- A | pkg/items/deed/built/fineTapestryEast1.cfg
- A | pkg/items/deed/built/fineTapestryEast2.cfg
- A | pkg/items/deed/built/fineTapestrySouth1.cfg
- A | pkg/items/deed/built/fineTapestrySouth2.cfg
- A | pkg/items/deed/built/finishedWoodenChest.cfg
- A | pkg/items/deed/built/flowerTapestryEast1.cfg
- A | pkg/items/deed/built/flowerTapestryEast2.cfg
- A | pkg/items/deed/built/flowerTapestrySouth1.cfg
- A | pkg/items/deed/built/flowerTapestrySouth2.cfg
- A | pkg/items/deed/built/footStool.cfg
- A | pkg/items/deed/built/gargishChest.cfg
- A | pkg/items/deed/built/gildedWoodenChest.cfg
- A | pkg/items/deed/built/goblinTopiary.cfg
- A | pkg/items/deed/built/gothicChest.cfg
- A | pkg/items/deed/built/grandTapestrySouth1.cfg
- A | pkg/items/deed/built/grandTapestrySouth2.cfg
- A | pkg/items/deed/built/greenhouse.cfg
- A | pkg/items/deed/built/grillStation.cfg
- A | pkg/items/deed/built/hangingCookingRack.cfg
- A | pkg/items/deed/built/hangingToolsEast.cfg
- A | pkg/items/deed/built/hangingToolsSouth.cfg
- A | pkg/items/deed/built/hauntedMirrorCracked.cfg
- A | pkg/items/deed/built/hauntedMirrorShattered.cfg
- A | pkg/items/deed/built/honestyTapestryEast.cfg
- A | pkg/items/deed/built/honestyTapestrySouth.cfg
- A | pkg/items/deed/built/honorTapestryEast.cfg
- A | pkg/items/deed/built/honorTapestrySouth.cfg
- A | pkg/items/deed/built/humilityTapestryEast.cfg
- A | pkg/items/deed/built/humilityTapestrySouth.cfg
- A | pkg/items/deed/built/intricateTapestryEast1.cfg
- A | pkg/items/deed/built/intricateTapestryEast2.cfg
- A | pkg/items/deed/built/intricateTapestrySouth1.cfg
- A | pkg/items/deed/built/intricateTapestrySouth2.cfg
- A | pkg/items/deed/built/justiceTapestryEast.cfg
- A | pkg/items/deed/built/justiceTapestrySouth.cfg
- A | pkg/items/deed/built/llamaTopiary.cfg
- A | pkg/items/deed/built/machineFletching.cfg
- A | pkg/items/deed/built/machineSewing.cfg
- A | pkg/items/deed/built/machineWoodworking.cfg
- A | pkg/items/deed/built/magicBookStand.cfg
- A | pkg/items/deed/built/mapleArmoire.cfg
- A | pkg/items/deed/built/marbleBenchPartEast1.cfg
- A | pkg/items/deed/built/marbleBenchPartEast2.cfg
- A | pkg/items/deed/built/marbleBenchPartEast3.cfg
- A | pkg/items/deed/built/marbleBenchPartEast4.cfg
- A | pkg/items/deed/built/marbleBenchPartSouth1.cfg
- A | pkg/items/deed/built/marbleBenchPartSouth2.cfg
- A | pkg/items/deed/built/masonryBench.cfg
- A | pkg/items/deed/built/mediumTokunoSculpture.cfg
- A | pkg/items/deed/built/minocStyleBenchPartEast1.cfg
- A | pkg/items/deed/built/minocStyleBenchPartEast2.cfg
- A | pkg/items/deed/built/minocStyleBenchPartEast3.cfg
- A | pkg/items/deed/built/necroticBrazier.cfg
- A | pkg/items/deed/built/ornateElvenTapestryEast.cfg
- A | pkg/items/deed/built/ornateElvenTapestrySouth.cfg
- A | pkg/items/deed/built/ornateWoodenChest.cfg
- A | pkg/items/deed/built/picnicBasket.cfg
- A | pkg/items/deed/built/plainLowTable.cfg
- A | pkg/items/deed/built/plainTapestryEast1.cfg
- A | pkg/items/deed/built/plainTapestryEast2.cfg
- A | pkg/items/deed/built/plainTapestrySouth1.cfg
- A | pkg/items/deed/built/plainTapestrySouth2.cfg
- A | pkg/items/deed/built/plainWoodenChest.cfg
- A | pkg/items/deed/built/plow.cfg
- A | pkg/items/deed/built/potionvat.cfg
- A | pkg/items/deed/built/redArmoire.cfg
- A | pkg/items/deed/built/repairTable.cfg
- A | pkg/items/deed/built/roundBasket.cfg
- A | pkg/items/deed/built/roundBasketWithHandles.cfg
- A | pkg/items/deed/built/sacrificeTapestryEast.cfg
- A | pkg/items/deed/built/sacrificeTapestrySouth.cfg
- A | pkg/items/deed/built/sacrificialAltarEast1.cfg
- A | pkg/items/deed/built/sacrificialAltarEast2.cfg
- A | pkg/items/deed/built/sacrificialAltarSouth1.cfg
- A | pkg/items/deed/built/sacrificialAltarSouth2.cfg
- A | pkg/items/deed/built/salvageStation.cfg
- A | pkg/items/deed/built/sandstoneBenchPartEast1.cfg
- A | pkg/items/deed/built/sandstoneBenchPartEast2.cfg
- A | pkg/items/deed/built/sandstoneBenchPartEast3.cfg
- A | pkg/items/deed/built/sandstoneBenchPartSouth1.cfg
- A | pkg/items/deed/built/sandstoneBenchPartSouth2.cfg
- A | pkg/items/deed/built/sandstoneBenchPartSouth3.cfg
- A | pkg/items/deed/built/shojiScreenCorner.cfg
- A | pkg/items/deed/built/shojiScreenEast.cfg
- A | pkg/items/deed/built/shojiScreenOpen1.cfg
- A | pkg/items/deed/built/shojiScreenOpen2.cfg
- A | pkg/items/deed/built/shojiScreenSouth.cfg
- A | pkg/items/deed/built/shortCabinet.cfg
- A | pkg/items/deed/built/shortTokunoSculpture.cfg
- A | pkg/items/deed/built/smallBushel.cfg
- A | pkg/items/deed/built/smallRoundBasket.cfg
- A | pkg/items/deed/built/sophisticatedElvenTapestryEast.cfg
- A | pkg/items/deed/built/sophisticatedElvenTapestrySouth.cfg
- A | pkg/items/deed/built/soulforge.cfg
- A | pkg/items/deed/built/spiritualityTapestryEast.cfg
- A | pkg/items/deed/built/spiritualityTapestrySouth.cfg
- A | pkg/items/deed/built/stoneBench4.cfg
- A | pkg/items/deed/built/stoneBench5.cfg
- A | pkg/items/deed/built/stoneBench6.cfg
- A | pkg/items/deed/built/tallBasket.cfg
- A | pkg/items/deed/built/tallCabinet.cfg
- A | pkg/items/deed/built/tallRoundBasket.cfg
- A | pkg/items/deed/built/tallTokunoSculpture.cfg
- A | pkg/items/deed/built/tapestryOfSosariaEast.cfg
- A | pkg/items/deed/built/tapestryOfSosariaSouth.cfg
- A | pkg/items/deed/built/terMurStyleTable.cfg
- A | pkg/items/deed/built/tinkerTableA.cfg
- A | pkg/items/deed/built/tinkerTableB.cfg
- A | pkg/items/deed/built/tinkerTableC.cfg
- A | pkg/items/deed/built/toolRack.cfg
- A | pkg/items/deed/built/unsettlingPortraitEast1.cfg
- A | pkg/items/deed/built/unsettlingPortraitEast2.cfg
- A | pkg/items/deed/built/unsettlingPortraitSouth1.cfg
- A | pkg/items/deed/built/unsettlingPortraitSouth2.cfg
- A | pkg/items/deed/built/valorTapestryEast.cfg
- A | pkg/items/deed/built/valorTapestrySouth.cfg
- A | pkg/items/deed/built/waterBarrel.cfg
- A | pkg/items/deed/built/winnowingBasket.cfg
- A | pkg/items/deed/built/woodenFootlocker.cfg
- R | pkg/items/deed/commands/player/redeednew.src -> pkg/items/deed/commands/player/redeed.src
- M | pkg/items/deed/config/itemdesc.cfg
- D | pkg/items/deed/config/itemdesc2.cfg
- M | pkg/items/deed/deed/use.src
- M | pkg/items/elvenFurniture/config/itemdesc.cfg
- M | pkg/items/evilHomeDecor/config/itemdesc.cfg
- A | pkg/items/forge/animatebellows.src
- M | pkg/items/forge/config/itemdesc.cfg
- M | pkg/items/forge/use.src
- M | pkg/items/graveStones/config/itemdesc.cfg
- M | pkg/items/lighting/config/itemdesc.cfg
- A | pkg/items/lighting/glassblowing/use.src
- M | pkg/items/pillows/config/itemdesc.cfg
- A | pkg/items/potionvat/itemdesc.cfg
- A | pkg/items/potionvat/pkg.cfg
- A | pkg/items/potionvat/use.src
- M | pkg/items/spittoon/config/itemdesc.cfg
- M | pkg/items/tokunoFurniture/config/itemdesc.cfg
- A | pkg/items/toolbox/config/barrelparts.cfg
- A | pkg/items/toolbox/itemdesc.cfg
- A | pkg/items/toolbox/pkg.cfg
- A | pkg/items/toolbox/use.src
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/house/multiSign/method.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/house/secureCont.src
- M | pkg/multis/house/walkOn.src
- M | pkg/multis/staticHousing/bantile/walkOn.src
- M | pkg/multis/staticHousing/lockunlock.src
- M | pkg/multis/staticHousing/logoff.src
- M | pkg/multis/staticHousing/reconnect.src
- M | pkg/multis/staticHousing/securecontainer/staticSecureCont.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/multis/staticHousing/sign/use.src
- M | pkg/opt/GMItems/fanofknives.src
- M | pkg/opt/GMItems/itemdesc.cfg
- A | pkg/opt/admin/include/adminpanel.inc
- A | pkg/opt/admin/pkg.cfg
- A | pkg/opt/alchemyplus/kegvat.inc
- M | pkg/opt/alchemyplus/potionkeg.src
- A | pkg/opt/alryc/config/missingequipment.cfg
- A | pkg/opt/alryc/textcmd/player/classinfo.src
- A | pkg/opt/alryc/textcmd/test/alryciteminfo.src
- M | pkg/opt/alryc/textcmd/test/combattest.src
- M | pkg/opt/alryc/textcmd/test/editcharacter.src
- A | pkg/opt/alryc/textcmd/test/fishingkit.src
- M | pkg/opt/alryc/textcmd/test/materialbag.src
- M | pkg/opt/alryc/textcmd/test/missingequipment.src
- A | pkg/opt/alryc/textcmd/test/testadminpanel.src
- A | pkg/opt/alryc/textcmd/test/wipecache.src
- M | pkg/opt/areaspawner/include/areaspawner.inc
- M | pkg/opt/areaspawner/include/areaspawnergump.inc
- A | pkg/opt/crafterboost/crafterboost_recipes.cfg
- M | pkg/opt/crafterboost/itemdesc.cfg
- M | pkg/opt/crafterboost/mendingoil.src
- A | pkg/opt/crafterboost/refinement_gump.src
- A | pkg/opt/dyteitems/dyecabinet.src
- M | pkg/opt/dyteitems/itemdesc.cfg
- M | pkg/opt/earth/bookofearth.src
- M | pkg/opt/holybook/holybook.src
- A | pkg/opt/imbuingstub/imbuing_stub.src
- A | pkg/opt/imbuingstub/itemdesc.cfg
- A | pkg/opt/imbuingstub/pkg.cfg
- M | pkg/opt/loot/antiloot.inc
- M | pkg/opt/necro/animatedead.src
- M | pkg/opt/necro/codexdamnorum.src
- M | pkg/opt/omegacache/blacklist.cfg
- M | pkg/opt/omegacache/cacheinsert.src
- M | pkg/opt/omegacache/categories.cfg
- M | pkg/opt/omegacache/destroycache.src
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/omegacache/omegacache.src
- M | pkg/opt/omegacache/placecache.src
- M | pkg/opt/powerscrolls/createpowerscroll.src
- M | pkg/opt/powerscrolls/powerscroll.src
- M | pkg/opt/powerscrolls/textcmd/player/showcaps.src
- M | pkg/opt/powerscrolls/textcmd/test/lowerallchosencaps.src
- M | pkg/opt/powerscrolls/textcmd/test/lowercaps.src
- M | pkg/opt/powerscrolls/textcmd/test/raiseallchosencaps.src
- M | pkg/opt/powerscrolls/textcmd/test/raisecaps.src
- M | pkg/opt/powerscrolls/transcendscroll.src
- M | pkg/opt/questpkg/config/quests.cfg
- A | pkg/opt/questpkg/include/questfishing.inc
- A | pkg/opt/questpkg/include/questmapgump.inc
- M | pkg/opt/questpkg/include/questnpcgump.inc
- A | pkg/opt/questpkg/include/questspawncheck.inc
- M | pkg/opt/questpkg/include/queststate.inc
- A | pkg/opt/questpkg/textcmd/gm/questcatch.src
- A | pkg/opt/questpkg/textcmd/gm/questmap.src
- M | pkg/opt/rituals/altar/gump.inc
- M | pkg/opt/rituals/config/itemdesc.cfg
- M | pkg/opt/rituals/config/spells.cfg
- M | pkg/opt/rituals/include/rituals.inc
- M | pkg/opt/rituals/rituals/attunement.src
- M | pkg/opt/rituals/rituals/consecration.src
- M | pkg/opt/rituals/rituals/createFocus.src
- M | pkg/opt/rituals/rituals/physicalWard.src
- M | pkg/opt/rituals/rituals/quickHealing.src
- M | pkg/opt/roleplaying/rperstone.src
- D | pkg/opt/shilhook/parry.src
- D | pkg/opt/shilhook/pkg.cfg
- D | pkg/opt/shilhook/regen.src
- D | pkg/opt/shilhook/syshook.cfg
- D | pkg/opt/shilhook/vitals.cfg
- M | pkg/opt/songbook/songbook.src
- M | pkg/opt/spawnpoint/checkpoint.src
- M | pkg/opt/spawnpoint/config/itemdesc.cfg
- M | pkg/opt/spawnpoint/despawner.src
- M | pkg/opt/spawnpoint/destroypoint.src
- M | pkg/opt/spawnpoint/include/customnpc.inc
- M | pkg/opt/spawnpoint/spawnpoint.src
- M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src
- M | pkg/opt/summoning/checkclasse.src
- M | pkg/opt/summoning/npcsummoning.src
- M | pkg/opt/summoning/processpersistedmod.src
- M | pkg/opt/summoning/processpoisonmod.src
- M | pkg/opt/summoning/processtempmod.src
- M | pkg/opt/summoning/summoning.src
- M | pkg/opt/townstones/electionwatch.src
- M | pkg/opt/townstones/itemdesc.cfg
- M | pkg/opt/townstones/textcmd/admin/createtownstone.src
- M | pkg/opt/townstones/textcmd/admin/removetownmember.src
- M | pkg/opt/townstones/textcmd/admin/townbankstatus.src
- M | pkg/opt/townstones/townlistbootstrap.src
- M | pkg/opt/townstones/tstone.inc
- M | pkg/opt/versebook/ai_spirit_flock.src
- M | pkg/opt/versebook/include/versefunctions.inc
- A | pkg/opt/warriorforhire/escrowsweep.src
- A | pkg/opt/warriorforhire/include/wfhcommon.inc
- A | pkg/opt/warriorforhire/include/wfhescrow.inc
- A | pkg/opt/warriorforhire/itemdesc.cfg
- A | pkg/opt/warriorforhire/pkg.cfg
- A | pkg/opt/warriorforhire/resetwfhdeaths.src
- A | pkg/opt/warriorforhire/start.src
- A | pkg/opt/warriorforhire/textcmd/test/setwfhdamage.src
- R | scripts/ai/warrior.src -> pkg/opt/warriorforhire/warrior.src
- R | scripts/items/warriorforhire.src -> pkg/opt/warriorforhire/warriorforhire.src
- M | pkg/opt/zuluitems/Testclassbooststone.src
- M | pkg/opt/zuluitems/classbooststone.src
- M | pkg/opt/zuluitems/itemdesc.cfg
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/packethooks/packethook/packethook.src
- M | pkg/std/alchemy/alchemy.src
- A | pkg/std/alchemy/alchemy_station_use.src
- A | pkg/std/alchemy/alchemyfunctions.inc
- M | pkg/std/alchemy/itemdesc.cfg
- A | pkg/std/blacksmithy/blacksmithgump.inc
- M | pkg/std/blacksmithy/blacksmithy.cfg
- M | pkg/std/blacksmithy/make_blacksmith_items.src
- A | pkg/std/blacksmithy/repair_table_use.src
- M | pkg/std/bowcraft/bowcraft.src
- M | pkg/std/bulkorders/bulkorderrewards.src
- M | pkg/std/bulkorders/rewards.cfg
- M | pkg/std/camping/itemdesc.cfg
- M | pkg/std/carpentry/carpentry.cfg
- M | pkg/std/carpentry/carpentry.src
- D | pkg/std/carpentry/carpentrydeed.src
- D | pkg/std/carpentry/commands/player/redeed.src
- M | pkg/std/carpentry/itemdesc.cfg
- M | pkg/std/cooking/cookbook.src
- M | pkg/std/cooking/cooking.cfg
- M | pkg/std/cooking/cooking.src
- M | pkg/std/cooking/fillpitcher.src
- M | pkg/std/cooking/food.inc
- M | pkg/std/cooking/hunger.src
- A | pkg/std/cooking/hungerdamage.src
- M | pkg/std/cooking/itemdesc.cfg
- M | pkg/std/dundee/globeofsosaria.src
- M | pkg/std/fishing/crustaceansweeper.src
- M | pkg/std/fishing/crustaceantrap.inc
- M | pkg/std/fishing/crustaceantrap.src
- M | pkg/std/fishing/fishing.inc
- M | pkg/std/fishing/fishing.src
- M | pkg/std/fishing/fishingnet.src
- M | pkg/std/fishing/itemdesc.cfg
- A | pkg/std/fishing/lure/use.src
- M | pkg/std/fishing/magicfish.src
- A | pkg/std/fishing/taxidermy.src
- A | pkg/std/fishing/trophyart.cfg
- M | pkg/std/healing/itemdesc.cfg
- M | pkg/std/healing/npchealing.src
- A | pkg/std/inscription/itemdesc.cfg
- M | pkg/std/itemid/itemid.src
- A | pkg/std/poisoning/itemdesc.cfg
- A | pkg/std/poisoning/poisoning.cfg
- A | pkg/std/poisoning/poisoning.inc
- M | pkg/std/poisoning/poisoning.src
- A | pkg/std/poisoning/toxinflask.src
- A | pkg/std/poisoning/vialofvenom.src
- A | pkg/std/salvage/itemdesc.cfg
- A | pkg/std/salvage/pkg.cfg
- A | pkg/std/salvage/salvage.src
- M | pkg/std/snooping/stealitems.cfg
- M | pkg/std/spells/blade_spirit.src
- M | pkg/std/spells/dispel.src
- M | pkg/std/spells/massdispel.src
- M | pkg/std/spells/vortex.src
- M | pkg/std/tailoring/itemdesc.cfg
- M | pkg/std/tailoring/make_cloth_items.src
- M | pkg/std/tailoring/tailoring.cfg
- M | pkg/std/tinkering/itemdesc.cfg
- M | pkg/std/tinkering/tinker.cfg
- M | pkg/std/tinkering/tinkering.src
- A | pkg/std/tinkering/tinkeringfunctions.inc
- M | pkg/systems/accounts/include/accounts.inc
- R | pkg/opt/shilhook/attributes.cfg -> pkg/systems/attributes/config/attributes.cfg
- R | pkg/opt/shilhook/checkskill.cfg -> pkg/systems/attributes/config/checkskill.cfg
- R | pkg/opt/shilhook/skillsdef.cfg -> pkg/systems/attributes/config/skillsdef.cfg
- A | pkg/systems/attributes/config/syshook.cfg
- R | pkg/opt/shilhook/uoclient.cfg -> pkg/systems/attributes/config/uoclient.cfg
- R | pkg/opt/shilhook/uoskills.cfg -> pkg/systems/attributes/config/uoskills.cfg
- A | pkg/systems/attributes/config/vitals.cfg
- R | pkg/opt/shilhook/shilhook.src -> pkg/systems/attributes/hooks/shilhook.src
- A | pkg/systems/attributes/hooks/vitalInit.src
- A | pkg/systems/attributes/include/regen.inc
- A | pkg/systems/attributes/pkg.cfg
- R | pkg/opt/shilhook/textcmd/admin/setglobalmultipliers.src -> pkg/systems/attributes/textcmd/admin/setglobalmultipliers.src
- R | pkg/opt/shilhook/textcmd/admin/setplayermultipliers.src -> pkg/systems/attributes/textcmd/admin/setplayermultipliers.src
- M | pkg/systems/combat/banishonhit.src
- M | pkg/systems/combat/banishscript.src
- M | pkg/systems/combat/blackrockscript.src
- M | pkg/systems/combat/config/itemdesc.cfg
- A | pkg/systems/combat/config/syshook.cfg
- R | pkg/opt/shilhook/omegaattack.inc -> pkg/systems/combat/hooks/omegaattack.inc
- R | pkg/opt/shilhook/omegaattack.src -> pkg/systems/combat/hooks/omegaattack.src
- R | pkg/opt/shilhook/shilcombat.src -> pkg/systems/combat/hooks/shilcombat.src
- M | pkg/systems/combat/include/hitscriptinc.inc
- R | pkg/opt/shilhook/shilcombat.inc -> pkg/systems/combat/shilcombat.inc
- M | pkg/systems/crafting/include/craftingfunctions.inc
- M | pkg/systems/crafting/include/craftmenu.inc
- A | pkg/systems/crafting/include/multicraft.inc
- M | pkg/utils/mdgumps/changelog.txt
- M | pkg/utils/mdgumps/include/gumps.inc
- M | pkg/utils/mdgumps/pkg.cfg
- M | pol.cfg.example
- M | pol.exe
- M | poltool.exe
- M | regions/regions.cfg
- A | scratch_itemdesc_list.txt
- M | scripts/ai/animaltrainer.src
- M | scripts/ai/combat/explosioncombatevent.inc
- A | scripts/ai/fishmonger.src
- M | scripts/ai/highpriest.src
- M | scripts/ai/legendaryhunter.src
- A | scripts/ai/lobsterman.src
- M | scripts/ai/loke.src
- M | scripts/ai/merchant.src
- A | scripts/ai/nystul.src
- M | scripts/ai/setup/modsetup.inc
- M | scripts/ai/thor.src
- A | scripts/ai/ysolde.src
- M | scripts/control/corpsedecay.src
- M | scripts/control/skilladvancerequip.src
- M | scripts/control/skilladvancerunequip.src
- M | scripts/ecompile.exe
- M | scripts/include/all.inc
- M | scripts/include/attributes.inc
- M | scripts/include/classes.inc
- M | scripts/include/client.inc
- M | scripts/include/constants/cfgfiles.inc
- M | scripts/include/constants/gumpids.inc
- M | scripts/include/constants/layers.inc
- M | scripts/include/constants/skillids.inc
- M | scripts/include/damages.inc
- M | scripts/include/dotempmods.inc
- M | scripts/include/drinkpotion.inc
- M | scripts/include/housing.inc
- M | scripts/include/itemutil.inc
- M | scripts/include/namingbyenchant.inc
- M | scripts/include/objtype.inc
- M | scripts/include/omegacache_utils.inc
- M | scripts/include/res.inc
- M | scripts/include/skillpoints.inc
- M | scripts/include/sounds.inc
- M | scripts/include/spawnpoint.inc
- M | scripts/include/starteqp.inc
- M | scripts/items/bladed.src
- M | scripts/items/racegate.src
- M | scripts/misc/chrdeath.src
- M | scripts/misc/death.src
- M | scripts/misc/guildbutton.src
- M | scripts/misc/namechanger.src
- M | scripts/modules/cliloc.em
- M | scripts/modules/os.em
- M | scripts/modules/unicode.em
- M | scripts/modules/uo.em
- M | scripts/playermanager.src
- M | scripts/runecl.exe
- M | scripts/start.src
- M | scripts/textcmd/admin/admin.src
- M | scripts/textcmd/admin/class.src
- A | scripts/textcmd/admin/destroyomegacache.src
- A | scripts/textcmd/admin/getbyserial.src
- M | scripts/textcmd/admin/setclass.src
- M | scripts/textcmd/admin/setname.src
- M | scripts/textcmd/coun/notes.src
- M | scripts/textcmd/gm/changename.src
- M | scripts/textcmd/gm/setprop.src
- M | scripts/textcmd/player/dropskills.src
- M | scripts/textcmd/player/hungry.src
- M | scripts/textcmd/player/showclasse.src
- M | scripts/textcmd/seer/info.src
- A | scripts/textcmd/test/bodpoints.src
- M | scripts/textcmd/test/householdmanager.src
- M | scripts/textcmd/test/skillstest.src
- M | scripts/util/repair.inc
- M | uoconvert.exe
- M | uotool.exe

---

## Detailed Changes By Theme

### 1. Bundled polserver engine (core binary) update

**Files involved:** `pol.exe`, `poltool.exe`, `uoconvert.exe`, `uotool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe` (all binary); `core-changes.txt`, `breaking-changes.txt` (this repo's local mirror of upstream's changelogs); `pol.cfg.example`; `scripts/modules/{cliloc.em,os.em,unicode.em,uo.em}`.

**Notable functional changes:**
- The shard's compiled polserver core was bumped to a newer upstream build, pulling in everything upstream shipped between roughly 08-14-2026 and 09-13-2026 (mirrored verbatim into `core-changes.txt`/`breaking-changes.txt`). Highlights that matter here:
  - eScript operator semantics tightened: an operator with no rule for its operand pair (e.g. mixing an `Error` with an `Integer`) now produces an explicit error instead of silently returning the left operand. **This is the root cause several fixes in this release exist to work around** — see Theme 7 below.
  - Booleans now count as 1/0 in arithmetic; Integer/Real comparison tolerance now applies symmetrically regardless of which side the Real is on; `<`/`<=`/`>`/`>=` on Booleans now compare values instead of object addresses.
  - `item.x`/`item.y`/`item.z` on an equipped item now track its wearer live instead of freezing at equip time; a corpse now visibly wears what its owner had on at death; several weather/season/light desync bugs on login, reconnect, resurrection, and realm-crossing boats were fixed client-side.
  - World-save performance improved substantially (new `WorldSaveThreads`/`LogWorldSaveDetails` pol.cfg options); `LoginServerDisconnectUnknownPkts` now defaults to 1 (drops unrecognized pre-login packets instead of logging and continuing).
  - `PolCore().internal(2/5/6)` removed in favor of `log_memory_usage()`/`log_script_memory()`/`log_script_variables()`.
- `pol.cfg.example` updated with the new/changed options described above.

**Expected impact:** Mostly invisible infrastructure, but a few effects are directly visible: corpses now display the deceased's clothing, weather/season/lighting resyncs correctly after login/reconnect/death/resurrection/boat travel, and world saves cause much shorter server freezes. The stricter operator-error behavior is also what exposed the `canInsert`/`canRemove` crash fixed in Theme 7.

### 2. Attributes/Vitals/Combat system split (shilhook retirement)

**Files involved:** entire `pkg/opt/shilhook` package retired — `attributes.cfg`, `checkskill.cfg`, `skillsdef.cfg`, `uoclient.cfg`, `uoskills.cfg`, `shilhook.src`, `textcmd/admin/{setglobalmultipliers,setplayermultipliers}.src` renamed into new `pkg/systems/attributes`; `omegaattack.inc/.src`, `shilcombat.src`, `shilcombat.inc` renamed into new `pkg/systems/combat`; deleted outright: `parry.src`, `pkg.cfg`, `regen.src`, `syshook.cfg`, `vitals.cfg`; new `pkg/systems/attributes/{config/syshook.cfg,config/vitals.cfg,hooks/vitalInit.src,include/regen.inc,pkg.cfg}` and `pkg/systems/combat/config/syshook.cfg`; `scripts/include/{attributes.inc,constants/cfgfiles.inc,constants/skillids.inc}`; `config/cmds.cfg`.

**Notable functional changes:**
- This finalizes the "Vitals/POLMD port" work: `pkg/opt/shilhook` (POLMD's deprecated pre-0.98 flat-CProp vitals shim) is gone, replaced by `pkg/systems/attributes` (skills/attributes config + hooks) and `pkg/systems/combat` (combat hooks), the current ModernDistro pattern.
- New `vitalInit.src`/`GetNpcVitalSetting()` resolves an NPC template's `HITS`/`MANA`/`STAM` fields from `npcdesc.cfg` — including dice-string values (e.g. `3d20+50`), rolled once and cached per-instance — before falling back to STR/INT/DEX×100 as before. `CustomHitsLevel`/`CustomManaLevel`/`CustomStaminaLevel` per-instance overrides are still checked first, so every existing override resolves identically.
- `SKILLID_THROWING := 57` is now a real, first-class skill id (was previously only reachable through ad hoc workarounds); 8 AOS-era skill ids (Necromancy, Focus, Chivalry, Bushido, Ninjitsu, Spellweaving, Mysticism, Imbuing = ids 49-56) are formally registered as dummy/unimplemented (present only for client protocol compliance — `MaxSkillID=57`), and excluded from every "real skill" enumeration (`GetAttributeIds()`, power scroll rolls, class-skill resets, etc.).
- New `NormalizeSkillAndSlot()` helper in `attributes.inc` reconciles the historical mismatch between a real skillid and its slot in the 50-entry power-scroll cap matrix (`pScrollMatrix`) — Alchemy is skillid 0 but matrix slot 49, Throwing is skillid 57 but matrix slot 50. Every caller that previously conflated the two (power scrolls, transcendence scrolls, `.dropskills`, the GM cap-management tools) now goes through this one conversion point. See Theme 3.
- `config/cmds.cfg`'s Admin command directory repointed from `pkg/opt/shilhook/textcmd/admin` to `pkg/systems/attributes/textcmd/admin`.

**Expected impact:** No direct player-visible change on its own, but it unblocks NPC template HITS/MANA/STAM randomization (previously gated on this port per project notes) and is the foundation the Throwing-skill fixes in Theme 3 build on.

### 3. Class/skill-cap correctness fixes (Throwing & Alchemy slot confusion)

**Files involved:** `scripts/include/attributes.inc`, `scripts/include/classes.inc`, `pkg/opt/powerscrolls/{createpowerscroll.src,powerscroll.src,transcendscroll.src,textcmd/player/showcaps.src,textcmd/test/{lowerallchosencaps,lowercaps,raiseallchosencaps,raisecaps}.src}`, `scripts/textcmd/player/dropskills.src`, `scripts/textcmd/admin/setclass.src`, new `pkg/opt/alryc/textcmd/player/classinfo.src`, `pkg/opt/capper/capper.src` (post-hoc addition, see note below).

**Notable functional changes:**
- `createpowerscroll.src` previously rolled `Random(49)`, which could select one of the AOS-era dummy skill ids (49-56) that don't exist on this shard, silently wasting a power scroll drop. Now rolls from the full skill space and re-rolls if it lands on 49-56.
- `powerscroll.src` used skillid directly as a matrix index, an off-by-shape bug affecting Alchemy (skillid 0) and — once Throwing (skillid 57) was added — Throwing scrolls, which could index out of the matrix entirely. Rewritten to use `NormalizeSkillAndSlot()`; also pads any legacy 49-entry matrix (saved before Throwing/slot 50 existed) up to the new size on first use instead of only on fresh allocation.
- `dropskills.src`'s "reset all skills" tool never reset Throwing (slot 50 wasn't in its skip/iterate logic) — fixed via the same normalization helper.
- New player command **`.classinfo`**: read-only report of a character's class-level math (in-class vs. out-of-class skill percentage, points needed for the next class level or points to drop to reach it instead).
- **Found after this changelog was first drafted:** `pkg/opt/capper/capper.src` (the periodic stat/skill-cap enforcer) was the one caller of the old skillid-as-matrix-index pattern this theme missed. It looped the raw `0..SKILLID__HIGHEST` range and indexed `pScrollMatrix[skill]` directly, which now throws immediately on `skill == 0` (`Operator + not supported for Integer and Error`, since matrix slot 0 doesn't exist — Alchemy's cap lives at slot 49) under the engine's stricter operator rules from Theme 1. Rewritten to iterate `GetSkillIds()` (skipping the 8 dummy AOS ids automatically) and resolve each skill's matrix slot via `NormalizeSkillAndSlot()`, same as the other callers above. This also silently fixes Throwing's cap, which was likewise indexing the non-existent slot 57 instead of 50.

**Expected impact:** Power scrolls can no longer roll a "dead" AOS skill and waste the drop; Throwing- and Alchemy-cap power scrolls/transcendence scrolls/GM cap tools now track correctly instead of risking a matrix-index bug; `.dropskills` now actually resets Throwing; players get a new self-service `.classinfo` command. The periodic stat/skill capper no longer crashes on its first character each cycle, and now correctly enforces the Throwing cap it was silently skipping before.

### 4. Throwing weapons: enchanted variants completed

**Files involved:** `pkg/systems/combat/config/itemdesc.cfg`, `config/nlootgroup.cfg`, `config/starteqp.cfg`.

**Notable functional changes:**
- Added 36 new enchanted throwing-weapon variants (Mystical/Swift/Stygian tiers of Throwing Dagger, Skull Glaive, Skull Glaive 2, Throwing Cleaver, Throwing Short Spear, Throwing Battle Axe, Throwing Axe, Throwing Hatchet, Throwing Executioner's Axe, Throwing Large Battle Axe, Throwing Sai, Throwing Ornate Axe) at objtypes `0x36000`+, matching the existing enchant-tier pattern used elsewhere.
- All throwing-weapon `Name` fields (base and enchanted) were lowercased to match this codebase's naming/enchant-lookup convention (`throwingdagger`, `skullglaive`, etc.) — matters for `SetNameByEnchant`/loot generation, which key off these names.
- All 12 base throwing weapons and all 36 enchanted variants wired into `config/nlootgroup.cfg`'s `NormalWeapons`, `MagicWeapons`, `Junk`, and `StygianShrine` groups, so they now actually drop from monster loot at the appropriate tiers.
- `config/starteqp.cfg`'s Throwing starting-equipment kit was pointed at the correct item name (`throwingdagger`, was `Throwing Dagger` — didn't resolve after the lowercasing); a fallback "Generic" starting kit (30 Bandages) was added for the dummy AOS skill ids so character creation can't break if one is ever selected.

**Expected impact:** Throwing weapons (base and 3 enchanted tiers) now drop from appropriate monster loot pools and are lootable/sellable/spawnable by name; new Throwing-skill characters get a working starting dagger. Crafting recipes for throwing weapons are still not implemented (unchanged from before this release).

### 5. Combat dispel fixes (overflow-damage kill path)

**Files involved:** `pkg/std/spells/{dispel.src,massdispel.src}`, `pkg/systems/combat/{banishonhit.src,banishscript.src,blackrockscript.src}`.

**Notable functional changes:** All 5 of these scripts used `ApplyTheDamage(target, source, GetMaxHP(target) + 3, DMGID_MAGIC)` as a "guaranteed lethal hit" pattern to kill a dispelled/banished creature. This silently no-ops for any creature with more than 65,535 max HP (a known engine damage-value cap). All 5 now set `KilledBy`/`KilledBySerial` on the target directly and call `.kill()` instead, so credit still flows correctly to `RegisterNPC`/`ProcessQuestKill`.

**Expected impact:** Dispel, Mass Dispel, and the Banish-on-hit/Banish/Blackrock combat effects now reliably kill their target regardless of its max HP, instead of potentially leaving an extremely high-HP creature undamaged and un-killed by these effects.

### 6. Equipment / weapon-slot fixes

**Files involved:** `config/equip.cfg`, `scripts/control/skilladvancerequip.src`, `pkg/std/itemid/itemid.src`, `pkg/multis/staticHousing/reconnect.src`.

**Notable functional changes:**
- **Double-equip guard:** `skilladvancerequip.src`'s `equip()` now checks whether the item's target layer (`it.tile_layer`, since `.layer` reads 0 before the item is actually worn) already holds a different item, and if so, sends "That spot is already in use." and returns the item to the backpack instead of letting it silently occupy an already-filled slot. Root-caused to at least one live case ("Xarafax's Axe" double-equip, two 1-handed weapons equipped simultaneously) reachable via abnormal character history or non-standard client wear sequencing.
- `config/equip.cfg`: Vampire1/Vampire2 NPC equipment sets were missing the dye hue (`0x66d`) on their shirt (`0x1517`), equipping it undyed while the matching cloak/robe were dyed — cosmetic fix.
- `pkg/std/itemid/itemid.src`: `GetObjProperty(who, "#LastID")` is now wrapped in `CInt()` — under the stricter engine operator rules (Theme 1), comparing an unconverted property value against a number could throw instead of silently coercing.
- `pkg/multis/staticHousing/reconnect.src`: removed a stray `run_script_to_completion(":motd:textcmd/player/motd", who)` call that unconditionally re-showed the MOTD gump to every player on every reconnect (not just first login).

**Expected impact:** Players can no longer end up with two weapons/items occupying the same equipment layer simultaneously; the two Vampire NPC costume sets display their shirt with the correct dye; reconnecting no longer re-pops the MOTD gump every time.

### 7. Container/housing crash fix: system moves with `mobile == 0`

**Files involved:** `pkg/items/containers/container/canInsert.src`, `pkg/items/containers/container/canRemove.src`, `scripts/include/housing.inc`.

**Notable functional changes:** The engine update in Theme 1 turned a previously-silent "Operator - not supported for Error and Integer" case into a hard script error. `GetHouseSign()` (called from both container hooks on essentially every item move) does arithmetic on `who.x`, and some system-initiated container moves invoke these hooks with `mobile` as a bare `0` rather than a real character object — a case the earlier `SCRIPTOPT_CAN_ACCESS_OFFLINE_MOBILES` fix (still present, now documented as a no-op for this specific bug) does not cover, since that flag only affects module functions that convert a character parameter, not raw member access like `mobile.x`. Both hooks and `GetHouseSign()` now explicitly guard `if(!mobile) return ...;` before touching any member.

**Expected impact:** Fixes a class of hard script errors/crashes on certain system-driven item moves (e.g. automated container operations) that the engine update would otherwise have newly surfaced as fatal instead of silently swallowed.

### 8. House ban enforcement hardening

**Files involved:** `pkg/multis/house/multiSign/method.src`, `pkg/multis/house/walkOn.src`, `pkg/multis/staticHousing/bantile/walkOn.src`.

**Notable functional changes:**
- `HasHousePermission()` now checks `IsBanned(sign, mobile)` first and short-circuits to 0 if true, closing a gap where a banned player who reached the interior by any route other than physically walking over a ban tile (a house teleporter, being carried in, a missing/misplaced ban tile) retained full friend/co-owner-level access to secure containers, lockdown/release, and the house-manager gump.
- The safe "drop banned player just outside the house" logic (added 2026-08-12) — previously duplicated byte-for-byte in both `house/walkOn.src` and `staticHousing/bantile/walkOn.src` — was consolidated into a single exported `sign.GetBanTileDropSpot()`/`sign.BootBannedFromHouse()` pair on the shared house-sign method script, so any future ban-enforcement call site can reuse it instead of a third copy.

**Expected impact:** A banned player can no longer retain house-interior privileges (secure access, lockdown/release, house management) if they get inside by a route other than the tracked ban-tile walk-on trigger.

### 9. Fishing: lures, trophies/taxidermy, and Bulk Order Deed rewards

**Files involved:** new `pkg/std/fishing/{lure/use.src,taxidermy.src,trophyart.cfg}`; `pkg/std/fishing/{crustaceantrap.inc,fishing.inc,fishing.src,itemdesc.cfg,magicfish.src,crustaceansweeper.src,fishingnet.src}`; `pkg/std/bulkorders/{bulkorderrewards.src,rewards.cfg}`; `pkg/packethooks/megacliloc/itemdata.src`; new `scripts/textcmd/test/bodpoints.src`; `scripts/items/bladed.src`.

**Notable functional changes:**
- New **fishing/crustacean lures**: `lure_use()` attaches a tiered lure (`LureKind`/`LureTier`/charges) either to a fishing pole or a deployed crustacean-trap buoy (not the un-deployed trap itself, since the trap item is consumed/replaced on each deployment). One lure loaded at a time; applying a new one overwrites the old, remaining charges included. MegaCliloc tooltip updated to show the attached lure.
- New **fish/crustacean trophies**: `taxidermy.src` plus `trophyart.cfg` (164 lines) add mountable trophy art for Rare/Legendary catches, with an associated `TrophyWeight` roll.
- `bulkorderrewards.src` gained new reward handling and `rewards.cfg` grew by 588 lines of new reward-catalog entries (including, per earlier project history, a Runic Atlas line and this session's fishing-trophy-related rewards).
- `bladed.src`'s `CarveFish()`: steaks-per-fish is no longer a flat 4 — it now reads the catch's own `DeepWaterCatch`/`FishTier` CProps (set by `fishing.inc`'s catch logic): shore catches give 1 steak, deep-water catches give 4, Rare catches double whichever of those applies, Legendary catches multiply by 10. A pre-existing fish with neither CProp (caught before this session) defaults to the lowest case (shore/Regular).
- New GM tool `.bodpoints` (staff command to inspect/adjust BOD point balances, per its file name and package).

**Expected impact:** Players can buy/apply lures to fishing poles and crustacean traps for a bonus effect; Rare/Legendary catches can now be mounted as trophies; carving a fish yields more steaks the better/deeper the catch was (previously always exactly 4 regardless of catch quality); the Bulk Order Deed reward catalog has substantially more entries to redeem points against.

### 10. Fishing & ritual quest content expansion

**Files involved:** new `pkg/opt/questpkg/include/{questfishing.inc,questmapgump.inc,questspawncheck.inc}`; new `pkg/opt/questpkg/textcmd/gm/{questcatch.src,questmap.src}`; `pkg/opt/questpkg/config/quests.cfg`, `pkg/opt/questpkg/include/{questnpcgump.inc,queststate.inc}`; new NPCs `scripts/ai/{fishmonger,lobsterman,nystul,ysolde}.src`; `config/npcdesc.cfg`; `pkg/opt/rituals/config/itemdesc.cfg` (new altars); `pkg/opt/spawnpoint/spawnpoint.src`.

**Notable functional changes:**
- New catch-based quest objective type: `QP_ProcessFishCatch()` advances any active quest objective flagged `ObjKind<n> = CatchFish|CatchCrustacean` (optionally gated by `ObjTier<n>` and `ObjMinWeight<n>`), called from the fishing/trap-collection code path — parallel to the existing kill-quest hook but without a pre-taggable NPC instance, since every catch is a fresh random roll.
- Two new quest-giver NPCs: **Fish Monger** (Quests 100-103, the fish line) and **Lobsterman** (Quests 104-107, the crustacean line), each with idle reminder shouts to nearby players with an undone quest.
- Two new ritual quest-giver NPCs: **Frostkeeper Ysolde** (Quests 6-8: Mana Dissimal, Mana Flux, Venom Bane, in Winterwyn's Everfrost Catacombs) and **Nystul the Enchanter** (Quests 9-11: Advanced Theurgy, Protective Aura, Free Movement) — each quest follows the existing ritual-altar pattern (offer reagents at a dedicated altar to summon a boss, kill it, turn in proof) and rewards a new ritual. New boss NPC templates and altar objtypes added to support them.
- New GM staff tools: `.questcatch` (simulate N fish/crustacean catches against yourself to test a catch-quest objective without fishing) and `.questmap` (list every quest with its live world location, discovered by scanning altar/giver CProps rather than a hand-maintained registry, with a Go-to-location button).
- Spawnpoint gained two new dedicated types, **Quest Item** and **Quest NPC** — placing quest content (an altar or quest-giver) under a generic Item/NPC/Container/Custom NPC type is now blocked with a redirect message, so `.questmap`'s live scan can always find it.

**Expected impact:** Two new fishing-line quests (4 total) and two new ritual quest lines (6 total quests) with 4 new quest-giver NPCs are now live and completable; catching a specific tier/weight of fish or crustacean can satisfy a quest objective directly.

### 11. Carpentry, redeed, and the furniture-deed backfill

**Files involved:** ~190 new one-line-per-item `pkg/items/deed/built/*.cfg` files; `pkg/items/deed/config/itemdesc.cfg`, `pkg/items/deed/config/itemdesc2.cfg` (deleted, merged in); `pkg/items/deed/deed/use.src`; `pkg/std/carpentry/{carpentry.cfg,carpentry.src,itemdesc.cfg}`; deleted `pkg/std/carpentry/carpentrydeed.src`, `pkg/std/carpentry/commands/player/redeed.src`; renamed `pkg/items/deed/commands/player/redeednew.src` -> `redeed.src`; `pkg/std/tailoring/itemdesc.cfg`; `pkg/std/fishing/fishingnet.src`.

**Notable functional changes:**
- Continues the carpentry-deed backfill: each new `built/*.cfg` file is a minimal `BuildCFG` block wiring one placed-furniture objtype onto the shared `:deed:deed/use` deed system, replacing the legacy `carpentrydeed.src`/`ObjList`/`ObjXMod`/`ObjYMod` placement path, which is now fully deleted.
- The renamed `redeednew.src` -> `redeed.src` (dispatch is filename-based) picked up the "Trash Can Redeed" behavior folded in from the retired carpentry `.redeed` command, plus the realm-aware `GetStandingHeight(...,mobile.realm)` fix.
- `carpentry.cfg`: ~15 recipes were recategorized in the crafting gump (e.g. several Looms/Spinning Wheel/training items moved from "Cloth: House Furniture"/"Cloth: Tools and Training Items" into more specific "Cloth: Tools" or "Cloth: Paintings" categories) — organizational only, same recipes/costs.
- `pkg/std/tailoring/itemdesc.cfg`: added itemdesc entries for the Loom's East-facing main tile (`0x1060`, previously missing entirely) and both East/South companion "side" tiles (`0x105F`/`0x1062`/`0x1064`) so all 4 Loom tiles are independently clickable, not just the primary tile of each facing.

**Expected impact:** ~190 additional placed furniture/station items can now be picked back up as a deed via `.redeed` instead of being stuck in place or lost; the Trash Can redeed shortcut still works from the unified command; carpentry recipes are organized into more sensible gump categories; East-facing and companion Loom tiles are now clickable instead of dead art.

### 12. Crafting station expansion: Multi-Item Crafting, Salvage Station, Potion Vat

**Files involved:** new `pkg/systems/crafting/include/multicraft.inc`; new `pkg/items/toolbox/*` (Artificer's Toolbox), `pkg/items/coopersbench/*` (Cooper's Bench), `pkg/items/potionvat/*` (Potion Vat); new `pkg/opt/alchemyplus/kegvat.inc`; new `pkg/std/salvage/*` (Salvage Station); new `pkg/std/blacksmithy/{blacksmithgump.inc,repair_table_use.src}`; new `pkg/std/alchemy/{alchemy_station_use.src,alchemyfunctions.inc}`; new `pkg/std/tinkering/tinkeringfunctions.inc`; `pkg/std/blacksmithy/make_blacksmith_items.src`, `pkg/std/tinkering/tinkering.src`, `pkg/std/alchemy/alchemy.src` (all shrank as logic moved into the new shared `.inc` files); `pkg/opt/alchemyplus/potionkeg.src`; new `pkg/opt/crafterboost/{crafterboost_recipes.cfg,refinement_gump.src}`; new `pkg/opt/imbuingstub/*`; new `pkg/opt/dyteitems/dyecabinet.src`; new `pkg/items/lighting/glassblowing/use.src`; `pkg/items/forge/use.src` + new `pkg/items/forge/animatebellows.src`.

**Notable functional changes:**
- New shared **Multi-Item Crafting** gump engine (`multicraft.inc`), for recipes needing several distinct fixed-quantity parts at once (unlike `craftmenu.inc`'s single-material-family recipes) — e.g. Barrel Staves + Lid + Hoops -> Closed Barrel. First consumer: the new **Artificer's Toolbox** and **Cooper's Bench** items, both driven from `pkg/items/toolbox/config/barrelparts.cfg`.
- New **Salvage Station**: a universal bulk "melt/unstitch back to raw material" apparatus covering all 5 modern crafting skills (Blacksmithy, Tailoring, Bowcraft, Carpentry, Tinkering) in one gump, generalizing the previously single-item, single-skill Tongs/Scissors tools. Supports "Salvage All," "Salvage All of Type," and a per-character "Protect Exceptional" toggle (defaults on) so a bulk pass can't accidentally destroy a hard-earned exceptional item. Owned/bound items are never salvageable.
- New **Potion Vat** item: a larger-capacity sibling to the Potion Keg, sharing the same Fill/Empty/Transfer gump (`kegvat.inc`, extracted from `potionkeg.src`).
- `blacksmithgump.inc`, `alchemyfunctions.inc`, and `tinkeringfunctions.inc` extract each skill's gump/crafting logic out of its main `.src` into a shared include, mirroring the pattern `craftmenu.inc` already established for the other 5 skills — mostly internal reorganization, but `bowcraft.src`/`tinkering.src`'s previously-drifted exceptional-quality multiplier math is now shared and consistent (per this and the prior release's fixes).
- `forge/use.src`'s bellows-pumping animation moved to a new standalone `animatebellows.src`, fixing an argument-unpacking bug (`Start_Script` passes one array argument, not two separate ones) that meant the visual almost certainly never played or errored; it now runs concurrently with the crafting gump instead of blocking around it.
- New `crafterboost_recipes.cfg`/`refinement_gump.src` (Crafter Boost package) and a scaffolded `imbuingstub` package (not yet a full feature).

**Expected impact:** Two new craftable/usable stations (Artificer's Toolbox/Cooper's Bench for barrel assembly, Salvage Station for bulk material recovery) and a new Potion Vat container item; the Large Forge's bellows animation now actually plays during Blacksmithy crafting.

### 13. Poisoning rework: self-sufficient crafting skill

**Files involved:** new `pkg/std/poisoning/{poisoning.cfg,poisoning.inc,itemdesc.cfg,toxinflask.src,vialofvenom.src}`; `pkg/std/poisoning/poisoning.src` (shrank as logic moved to `poisoning.inc`).

**Notable functional changes:** Two new items make Poisoning self-sufficient as a skill instead of always depending on Alchemy-made poison potions: **Vial of Venom** applies poison from a potion to a weapon/food/bandage directly, and **Toxin Flask** brews poison potions itself from Nightshade via the Poisoning skill (`poisoning.cfg` mirrors Alchemy's existing poison-potion skill/reagent tiers exactly, just gated on `SKILLID_POISONING`). `do_poisoning()` still falls back to the original "potion + item" flow if neither tool is in the backpack; owning one auto-selects that action, owning both prompts which to use.

**Expected impact:** Poisoning is now a more standalone skill — players can brew their own poison potions and apply them without needing Alchemy at all.

### 14. Cooking & hunger rebalance

**Files involved:** `pkg/std/cooking/{hunger.src,food.inc,cooking.src,cooking.cfg,fillpitcher.src,cookbook.src,itemdesc.cfg}`; new `pkg/std/cooking/hungerdamage.src`; `scripts/textcmd/player/hungry.src`.

**Notable functional changes:**
- Hunger scale stretched from a 0-9 range (single "famished" tier at 9) to 0-15, with staged resource drain instead of one abrupt penalty: stamina starts draining at hunger 10, mana also drains from 12, health also drains from 14. The drain now runs as an independent background script (`hungerdamage.src`, spawned via `Start_Script` once hunger first reaches 10) instead of blocking inline inside the hourly hunger-increment loop, so hunger keeps climbing while the drain runs concurrently instead of freezing in place.
- `.hungry` command's status messages were rewritten to cover all 16 levels (0-15) with distinct flavor text for the new intermediate tiers.

**Expected impact:** Going hungry is a more gradual, multi-stage experience (stamina, then mana, then health) instead of one abrupt HP/stamina hit at the old ceiling; `.hungry` reports the new stages accurately.

### 15. Warrior for Hire: package move, escrow safety net, and revival

**Files involved:** `scripts/ai/warrior.src` -> `pkg/opt/warriorforhire/warrior.src` (renamed, substantially edited); `scripts/items/warriorforhire.src` -> `pkg/opt/warriorforhire/warriorforhire.src` (renamed); new `pkg/opt/warriorforhire/{itemdesc.cfg,pkg.cfg,start.src,escrowsweep.src,resetwfhdeaths.src,include/wfhcommon.inc,include/wfhescrow.inc,textcmd/test/setwfhdamage.src}`; `scripts/misc/death.src`; `scripts/ai/highpriest.src`; `config/cmds.cfg`.

**Notable functional changes:**
- The Warrior for Hire mercenary-NPC feature moved out of the generic `scripts/ai`/`scripts/items` locations into its own `pkg/opt/warriorforhire` package.
- New **escrow safety net**: `WFH_SaveToEscrow()` places a hired warrior's items into a 90-day escrow (reusing the player-vendor escrow storage pattern) instead of losing them outright; `escrowsweep.src` runs once a day via `start.src` to expire escrow past that window.
- Death/"Heart" mechanic reworked in `death.src`: the resurrection-attempt limit (`resnum`) was raised from 4 to 11; the heart now also reaches the owner's bank box (instead of failing outright) if the owner is offline when the warrior dies; a `WFHBackup` snapshot (stats, name, gender, skills, resnum) is now saved to the owner every time, enabling permanent recovery even after the heart itself disintegrates.
- New: the **High Priest** NPC now offers "resurrect warrior" — for 10,000 gold, fully recreates a permanently-lost Warrior for Hire from its saved `WFHBackup` (stats, skills, name, and gender restored), provided the player doesn't already have a living one.
- New GM tools: `.setwfhdamage` (gump to set global damage multipliers dealt/taken vs. normal NPCs and vs. bosses) and a "reset death count" item-based tool (`resetwfhdeaths.src`).
- `config/cmds.cfg` gained the package's Test-tier command directory.

**Expected impact:** Losing a hired warrior's gear or the warrior itself is no longer necessarily permanent — items go to a recoverable 90-day escrow, and a fully "dead" (heart-disintegrated) warrior can be bought back from the High Priest for 10,000 gold instead of being unrecoverable. Warrior for Hire can survive more encounters before the heart mechanic gives up (4 -> 11 attempts).

### 16. Ritual system: 2 new quest lines + altar/rituals.inc fixes

**Files involved:** `pkg/opt/rituals/config/itemdesc.cfg` (new altars), `pkg/opt/rituals/config/spells.cfg`, `pkg/opt/rituals/include/rituals.inc`, `pkg/opt/rituals/altar/gump.inc`, `pkg/opt/rituals/rituals/{attunement,consecration,createFocus,physicalWard,quickHealing}.src`.

**Notable functional changes:** New ritual altars and boss templates supporting the Ysolde/Nystul quest lines described in Theme 10 (`ritualaltar135`-`ritualaltar14x` series, each carrying `RitualAltarQuestId`/`RitualAltarBossTemplate`/`RitualAltarTrophyObjtype`/reagent CProps). Existing ritual scripts (`attunement`, `consecration`, `createFocus`, `physicalWard`, `quickHealing`) received incidental fixes/adjustments alongside the new content; `rituals.inc` and `spells.cfg` updated to register the new rituals granted by the Ysolde/Nystul quest lines.

**Expected impact:** See Theme 10 — this is the altar/ritual-mechanics half of the new ritual quest content.

### 17. Omega Cache: category and item ordering rework

**Files involved:** `pkg/opt/omegacache/{omegacache.inc,categories.cfg,cacheinsert.src,destroycache.src,omegacache.src,placecache.src,blacklist.cfg}`; `scripts/include/omegacache_utils.inc`; new `scripts/textcmd/admin/destroyomegacache.src`; new `pkg/opt/alryc/textcmd/test/wipecache.src`.

**Notable functional changes:**
- Fixed a real display-order bug: `ListConfigElemProps()` returns category names via an internal case-insensitive multimap, which is always alphabetical — never file/declaration order. `categories.cfg`'s existing "ordering convention" comment (categories intended to appear in a specific curated order) was therefore silently ignored at runtime. `LoadCategoryLookup()` now explicitly sorts by each category's declared numeric weight (documented convention: space by 100), and a parallel per-item weight was added for ordering within a category's item list (default 1 = alphabetical, matching current behavior until deliberately overridden).
- Category-menu page navigation (Prev/label/Next) now uses fixed-width slots so the buttons land in the same screen position on every page, instead of the group re-centering (and Next moving) depending on whether that particular page has a Prev button.
- New GM tool `.wipecache` (target an Omega Cache container and wipe all stored entries/leases for a clean testing slate — explicit-target, not nearest-cache auto-detect, to avoid wiping the wrong cache); new `.destroyomegacache` (admin-tier).

**Expected impact:** The Omega Cache category menu now actually displays in the order its config says it should, and item ordering within a category is configurable; page navigation buttons no longer shift position between pages.

### 18. Repair system: high-skill "last hit point" save chance

**Files involved:** `scripts/util/repair.inc`.

**Notable functional changes:** Previously any item at `hp < 2` broke unconditionally on a repair attempt, regardless of the repairer's skill. Now a Grandmaster-tier crafter (`IsCrafter()` level 5 or 6) gets a 50/50 chance to save the item instead of an automatic break; anything below that level, or a failed roll, still breaks it as before.

**Expected impact:** High-level crafters (GM/Elder tier) now have a chance to save an item that's down to its last hit point instead of guaranteed losing it on the repair attempt.

### 19. Admin/staff tooling

**Files involved:** new `pkg/opt/admin/{include/adminpanel.inc,pkg.cfg}`; new `pkg/opt/alryc/textcmd/test/{testadminpanel.src,alryciteminfo.src}` (rewrite), `pkg/opt/alryc/config/missingequipment.cfg`; `scripts/textcmd/test/householdmanager.src`; `pkg/systems/accounts/include/accounts.inc`; new `scripts/textcmd/admin/getbyserial.src`; `pkg/utils/mdgumps/include/gumps.inc`.

**Notable functional changes:**
- Ported POL2.5's admin panel (`adminpanel.inc`) — per-account/character history tracking (name/death/poison/notes history, capped at 100 entries each) backing the `.testadminpanel` staff tool.
- Rebuilt `alryciteminfo.src` from scratch (3,588 lines) — an enchant/property editor for a targeted item, constrained to real loot-legal values, with visible before/after logging via the shared `SetNameByEnchant`.
- Fixed a real runtime bug in `.householdmanager`: it called `.Keys()` on the value returned by the household datafile-open wrapper, which doesn't actually expose that method on this engine build (confirmed broken — "Method id '56' [keys] not found" — independently hit and fixed the same way in the new admin panel's own household browser). Replaced with `CollectHouseholdIds()`, which derives the household list from every account's own Discord info instead of enumerating the datafile directly.
- `accounts.inc`: `ACCT_TrimDiscordID()`/`ACCT_GetAccountDiscordInfo()` didn't guard against `GetProp()` returning an unresolved `error{}` struct (e.g. for an account that predates the `DiscordID` property, like the original bootstrap admin account) — that struct is truthy and would `CStr()` into literal `"error{errortext=...}"` text shown in the account browser. Now explicitly checked.
- New staff command `.getbyserial <hexserial>`: finds any item by its hex serial and moves it into the caller's backpack.
- `pkg/utils/mdgumps/include/gumps.inc` gained several new low-level gump primitives (`GFMasterGump`, `GFToggleUpperWordCase`, `GFToggleCroppedText`, `GFECHandleInput`, `GFPicInPic`, `GFTilePicAsGumpPic`, a `partial` parameter on `GFGumpPic`) and fixed a copy-paste bug in `GFDisposable()`'s toggle-off branch (checked `gump.base.CloseLoc` instead of `gump.base.DisposeLoc`).

**Expected impact:** Staff-facing only — no direct player impact. Enables faster/more reliable item property editing, account/character history browsing, and household/Discord-link auditing for the team; `.householdmanager` and the admin panel's household browser no longer error out.

### 20. Miscellaneous smaller fixes

- `pkg/opt/GMItems/itemdesc.cfg`: the Mass Shuriken GM item (`0x9B11`) was retired (commented out) as superseded by the throwing-weapon rework in Theme 4.
- `pkg/opt/GMItems/fanofknives.src`: replaced a bare `0x01` flag literal with its named constant `LISTEX_FLAG_NORMAL` — no functional change.
- `pkg/std/snooping/stealitems.cfg`, `pkg/opt/loot/antiloot.inc`: minor list/config touch-ups (steal-eligible item list and anti-loot exceptions).
- `pkg/opt/townstones/*` (electionwatch.src, itemdesc.cfg, 3 textcmd/admin scripts, townlistbootstrap.src, tstone.inc): small fixes bundled into the same working sessions as the larger themes above, plus one `1418633 Compile Fix` commit correcting a syntax slip introduced by an earlier townstones edit.
- `pkg/commands/commands/gm/mobedit.src`: extended alongside the vitals/attributes port to expose the new HITS/MANA/STAM template fields.
- `scripts/ai/{animaltrainer,legendaryhunter,loke,merchant,thor}.src`, `scripts/ai/combat/explosioncombatevent.inc`, `scripts/ai/setup/modsetup.inc`: small compatibility touch-ups for the attributes/vitals include-path rename (Theme 2).
- `scripts/control/corpsedecay.src`, `scripts/misc/{chrdeath.src,guildbutton.src,namechanger.src}`, `scripts/playermanager.src`, `scripts/items/racegate.src`, `scripts/start.src`: incidental fixes bundled into the same commits as the larger themes (mostly `include`-path updates for the shilhook->attributes/combat rename, plus small defensive guards).
- `scripts/textcmd/admin/{admin.src,class.src,setclass.src,setname.src}`, `scripts/textcmd/coun/notes.src`, `scripts/textcmd/gm/{changename.src,setprop.src}`, `scripts/textcmd/seer/info.src`, `scripts/textcmd/test/skillstest.src`: small additions tied into the admin-panel history logging added in Theme 19 (these commands now record into the new history tables) and the class-system fixes in Theme 3.
- `scripts/include/{all.inc,client.inc,damages.inc,dotempmods.inc,drinkpotion.inc,itemutil.inc,namingbyenchant.inc,objtype.inc,res.inc,skillpoints.inc,spawnpoint.inc,starteqp.inc}`: small, mostly single-digit-line fixes and include-path updates bundled alongside the larger themes above rather than standalone changes.
- `scripts/include/sounds.inc`: full mechanical port of every sound-effect id from the client's `soundLegacyMUL.uop` sound list (`SoundList.csv`), replacing a hand-curated ~120-entry enum with the complete set (named directly from source `.wav` filenames). Old enum kept commented out for reference. Dev/scripting convenience only — this change alone does not alter what sounds currently play in-game.
- `scripts/modules/{cliloc.em,os.em,unicode.em,uo.em}`: builtin declarations updated to match the new engine binary (Theme 1); no script-visible API change beyond what's already covered there.
- `config/command_synopses.cfg`: regenerated to reflect every new/changed textcmd script's `// Synopsis:` header across this release (mechanical, via the repo's synopsis generator).
- `regions/regions.cfg`: minor touch-ups (20 lines) alongside the ritual/quest content additions.
- `ainotes/crafting-station-audit-20260909.md`, `ainotes/missing-equipment-static-overlap-20260831.json`, `ainotes/user-sort-layer1-20260904.json`, `pkg/opt/alryc/config/missingequipment.cfg`, `scratch_itemdesc_list.txt`: dev audit tool output/config, not game content.
- `.claude/skills/escript-gotchas/SKILL.md`, `.claude/subagent-briefing.md`: repo tooling documentation, not game content.

**Expected impact:** No direct player-visible effect for this section as a whole, beyond what's already covered in the themes each item is tied to.

---

## Validation Notes

- Diff range: `git log --oneline --no-merges b0ace6e..9aa5216`, `git diff --stat b0ace6e..9aa5216`, `git diff --name-status b0ace6e..9aa5216`, plus `git show <hash> --stat`/`git show <hash> -- <path>` for each of the 14 non-merge commits to inspect real functional diffs (prioritizing new files and large rewrites; large mechanical config additions such as the ~190 carpentry deed `.cfg` files and the sounds.inc enum port were characterized as a pattern rather than transcribed line-by-line).
- Merge commits (`dcf2bfb`, `ab7f06e`, `de288c1`, `b5d41d5`, `7ceb4ef`, `b7c58ae`, `4a742e3`) were checked against the union of files touched by the 14 non-merge commits; the two lists reconcile exactly once renames are accounted for, confirming the merges carry no additional content of their own.
- File-status counts (293 A / 242 M / 8 D / 15 R = 558) were derived programmatically from `git diff --name-status` and cross-checked against the printed inventory.
- Working tree was clean at the time of this writing (`git status` reported "nothing to commit, working tree clean" on `Patch-3.1.1` at `9aa5216`).
- Addendum: `pkg/opt/capper/capper.src` was fixed in-session immediately after this changelog was drafted (a live server error report), not yet committed as of this writing. Folded into Theme 3 above rather than commit-range-diffed since it isn't part of the `b0ace6e..9aa5216` range; will land in whatever commit actually ships this file.
