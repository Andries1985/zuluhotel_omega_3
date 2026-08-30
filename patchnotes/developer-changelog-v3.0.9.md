# Developer Changelog - v3.0.9

Range: Patch-3.0.8..Patch-3.0.9 (commit `b367a82`..`db15da7`)
Branch: Patch-3.0.9
Date: 2026-08-29

---

## Scope Summary

- Total files changed: 380 (260 added, 113 modified, 7 deleted)
- Net textual delta: 40,998 insertions, 4,607 deletions
- Largest shifts:
  - `pkg/std/carpentry/carpentry.cfg` (+3037 net) - dozens of new furniture/decor recipes from the item-package port, plus the 12-statue removal (relocated to Tinkering, see Theme 3)
  - `pkg/items/lighting/config/itemdesc.cfg` (+2261, was already present, absorbed the rest of `pkg/opt/lighting`) / `pkg/opt/lighting/itemdesc.cfg` (-962, package deleted) - completed a lighting-package merge that a prior session had left half-finished
  - `pkg/std/bulkorders/bulkorder_templates.cfg` (+1847, new file, 166 templates) and `bulkorders.cfg` (+631, new file) - the entire Large/Small BOD template catalog and per-craft config (Theme 1)
  - `pkg/items/tokunoFurniture/config/itemdesc.cfg` (+1552), `pkg/items/kingsCollection/config/itemdesc.cfg` (+1412), `pkg/items/banners/config/itemdesc.cfg` (+1058), `pkg/items/graveStones/config/itemdesc.cfg` (+801), `pkg/items/tables/config/itemdesc.cfg` (+779), `pkg/items/evilHomeDecor/config/itemdesc.cfg` (+500) - a handful of the ~30 new item packages from the POLMD port (Theme 2)
  - `pkg/std/tinkering/tinker.cfg` (+1293), `pkg/std/tailoring/tailoring.cfg` (+1170), `pkg/std/cooking/itemdesc.cfg` (+685) - new craftable recipes across three crafts, feeding both the item port and the BOD pools
  - `pkg/std/bowcraft/bowcraft.src` (+992, new file) - the extracted, from-scratch Bowcraft crafting tool (Theme 5)
  - `pkg/opt/rituals/config/rituals.cfg` (650 changed, mostly ID renumbering) - every ritual's spell-id realigned to match new `spells.cfg` registrations (Theme 4)
  - `pkg/std/bulkorders/bulkorderbook.src` (+627), `bulkorder.inc` (+610) - the BOD book UI and core constants/reward math
  - `pkg/opt/questpkg/include/queststate.inc` (+482, new file) - the new generic quest framework's per-character state (Theme 6)
- Non-merge commits in range (oldest to newest):
  - `68e3a51` Checkpoint: precious metals, bowcraft package extraction, Omega Cache crafting-tool targeting
  - `f404618` Bowcraft package done / Cooking fix / soulforge fix / trash can insert sound / objtype updates / materialbag command / guild colors updates
  - `caaaecd` Begging update
  - `db15da7` Large patch.. patchnotes coming (this commit; includes the full BOD system, the item-package port, the statue-relocation mechanic, and a string of boot-blocking bugfixes surfaced by repeated server-start testing)
- Themes below are organized by subsystem, not by commit, since most of this range (especially `db15da7`) was built incrementally across a long working session and is best read as continuous pieces of work.
- **Flagged, not addressed:** `out.txt` (repo root) is a stray scratch/debug output file that appears to have been committed by accident — not game content, safe to delete whenever convenient.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- D | config/bowcraft.cfg
- M | config/command_synopses.cfg
- M | config/equip.cfg
- M | config/food.cfg
- M | config/itemdesc.cfg
- M | config/mrcspawn.cfg
- M | config/nlootgroup.cfg
- M | config/npcdesc.cfg
- A | out.txt
- A | pkg/items/LifeStones/config/icp.cfg
- A | pkg/items/LifeStones/config/itemdesc.cfg
- A | pkg/items/LifeStones/healthstone.src
- A | pkg/items/LifeStones/manastone.src
- A | pkg/items/LifeStones/pkg.cfg
- A | pkg/items/LifeStones/staminastone.src
- A | pkg/items/advancedTrainingDummy/config/icp.cfg
- A | pkg/items/advancedTrainingDummy/config/itemdesc.cfg
- A | pkg/items/advancedTrainingDummy/pkg.cfg
- A | pkg/items/banners/config/icp.cfg
- A | pkg/items/banners/config/itemdesc.cfg
- A | pkg/items/banners/pkg.cfg
- A | pkg/items/benches/config/icp.cfg
- A | pkg/items/benches/config/itemdesc.cfg
- A | pkg/items/benches/pkg.cfg
- A | pkg/items/celestial/bronzeglobe/bronzeGlobeUse.src
- A | pkg/items/celestial/config/itemdesc.cfg
- A | pkg/items/celestial/orrery/orreryUse.src
- A | pkg/items/celestial/pkg.cfg
- A | pkg/items/celestial/telescope/telescopeUse.src
- A | pkg/items/chairs/config/icp.cfg
- A | pkg/items/chairs/config/itemdesc.cfg
- A | pkg/items/chairs/pkg.cfg
- A | pkg/items/clocks/config/icp.cfg
- A | pkg/items/clocks/config/itemdesc.cfg
- A | pkg/items/clocks/pkg.cfg
- A | pkg/items/clocks/use.src
- A | pkg/items/coffins/config/icp.cfg
- A | pkg/items/coffins/config/itemdesc.cfg
- A | pkg/items/coffins/pkg.cfg
- A | pkg/items/commcrystals/commCrystal.src
- A | pkg/items/commcrystals/config/icp.cfg
- A | pkg/items/commcrystals/config/itemdesc.cfg
- A | pkg/items/commcrystals/crystalControl.src
- A | pkg/items/commcrystals/pkg.cfg
- M | pkg/items/containers/config/itemdesc.cfg
- A | pkg/items/debris/config/icp.cfg
- A | pkg/items/debris/config/itemdesc.cfg
- A | pkg/items/debris/debris/control.src
- A | pkg/items/debris/debris/method.src
- A | pkg/items/debris/debris/onCreate.src
- A | pkg/items/debris/pkg.cfg
- A | pkg/items/decorativeArmor/config/icp.cfg
- A | pkg/items/decorativeArmor/config/itemdesc.cfg
- A | pkg/items/decorativeArmor/pkg.cfg
- A | pkg/items/decorativeShields/config/icp.cfg
- A | pkg/items/decorativeShields/config/itemdesc.cfg
- A | pkg/items/decorativeShields/pkg.cfg
- A | pkg/items/decorativeWeapons/config/icp.cfg
- A | pkg/items/decorativeWeapons/config/itemdesc.cfg
- A | pkg/items/decorativeWeapons/pkg.cfg
- M | pkg/items/deed/built/advancedTrainingDummy.cfg
- A | pkg/items/deed/built/bambooTable.cfg
- A | pkg/items/deed/built/birdLamp.cfg
- A | pkg/items/deed/built/dragonLamp.cfg
- A | pkg/items/deed/built/dragonLantern.cfg
- M | pkg/items/deed/built/globeOfSosaria.cfg
- A | pkg/items/deed/built/kingsStarLargeTable.cfg
- A | pkg/items/deed/built/koiLamp.cfg
- M | pkg/items/deed/built/largeForge.cfg
- A | pkg/items/deed/built/largeForgeBellows1.cfg
- A | pkg/items/deed/built/largeForgeBellows2.cfg
- A | pkg/items/deed/built/largeForgeNoBellows.cfg
- A | pkg/items/deed/built/logTable.cfg
- A | pkg/items/deed/built/marbleTable2.cfg
- A | pkg/items/deed/built/minocStyleTable.cfg
- M | pkg/items/deed/built/plushLoveSeat.cfg
- A | pkg/items/deed/built/purpleClothLargeTable.cfg
- A | pkg/items/deed/built/purpleTartanLargeTable.cfg
- A | pkg/items/deed/built/redClothLargeTable.cfg
- A | pkg/items/deed/built/sandstoneTable.cfg
- M | pkg/items/deed/built/shadowFirePit.cfg
- A | pkg/items/deed/built/stainedGlassLamp.cfg
- A | pkg/items/deed/built/tallDoubleLamp.cfg
- A | pkg/items/deed/built/tallLamp.cfg
- A | pkg/items/deed/built/tartanCoveredTable.cfg
- A | pkg/items/deed/built/yewStyleTablePieces.cfg
- M | pkg/items/deed/config/itemdesc.cfg
- A | pkg/items/displayCases/config/icp.cfg
- A | pkg/items/displayCases/config/itemdesc.cfg
- A | pkg/items/displayCases/pkg.cfg
- A | pkg/items/engravingTools/config/icp.cfg
- A | pkg/items/engravingTools/config/itemdesc.cfg
- A | pkg/items/engravingTools/include/engravingTools.inc
- A | pkg/items/engravingTools/items/leatherEngraver/use.src
- A | pkg/items/engravingTools/items/metalEngraver/use.src
- A | pkg/items/engravingTools/items/methods.src
- A | pkg/items/engravingTools/items/woodEngraver/use.src
- A | pkg/items/engravingTools/pkg.cfg
- A | pkg/items/evilHomeDecor/config/icp.cfg
- A | pkg/items/evilHomeDecor/config/itemdesc.cfg
- A | pkg/items/evilHomeDecor/items/bedOfNails/walkOn.src
- A | pkg/items/evilHomeDecor/items/boneCouch/walkOn.src
- A | pkg/items/evilHomeDecor/items/boneThrone/walkOn.src
- A | pkg/items/evilHomeDecor/items/pixies/use.src
- A | pkg/items/evilHomeDecor/pkg.cfg
- A | pkg/items/forge/config/icp.cfg
- A | pkg/items/forge/config/itemdesc.cfg
- A | pkg/items/forge/methods.src
- A | pkg/items/forge/pkg.cfg
- A | pkg/items/forge/use.src
- A | pkg/items/fountains/config/icp.cfg
- A | pkg/items/fountains/config/itemdesc.cfg
- A | pkg/items/fountains/pkg.cfg
- A | pkg/items/gardenShed/config/icp.cfg
- A | pkg/items/gardenShed/config/itemdesc.cfg
- A | pkg/items/gardenShed/pkg.cfg
- A | pkg/items/gargishFurniture/config/icp.cfg
- A | pkg/items/gargishFurniture/config/itemdesc.cfg
- A | pkg/items/gargishFurniture/pkg.cfg
- A | pkg/items/gothicThemePack/config/icp.cfg
- A | pkg/items/gothicThemePack/config/itemdesc.cfg
- A | pkg/items/gothicThemePack/pkg.cfg
- A | pkg/items/graveStones/config/icp.cfg
- A | pkg/items/graveStones/config/itemdesc.cfg
- A | pkg/items/graveStones/graveStone/method.src
- A | pkg/items/graveStones/graveStone/use.src
- A | pkg/items/graveStones/pkg.cfg
- A | pkg/items/interiordecoratortool/config/icp.cfg
- A | pkg/items/interiordecoratortool/config/itemdesc.cfg
- A | pkg/items/interiordecoratortool/interiordecorator.src
- A | pkg/items/interiordecoratortool/pkg.cfg
- A | pkg/items/kingsCollection/config/icp.cfg
- A | pkg/items/kingsCollection/config/itemdesc.cfg
- A | pkg/items/kingsCollection/pkg.cfg
- A | pkg/items/lighting/bonfire/method.src
- A | pkg/items/lighting/brazier/method.src
- A | pkg/items/lighting/config/icp.cfg
- A | pkg/items/lighting/config/itemdesc.cfg
- A | pkg/items/lighting/firepit/method.src
- A | pkg/items/lighting/fireplace/method.src
- A | pkg/items/lighting/fireplace/use.src
- A | pkg/items/lighting/light/method.src
- A | pkg/items/lighting/light/use.src
- A | pkg/items/lighting/oven/method.src
- A | pkg/items/lighting/pkg.cfg
- A | pkg/items/lighting/stove/method.src
- A | pkg/items/miscItems/config/icp.cfg
- A | pkg/items/miscItems/config/itemdesc.cfg
- A | pkg/items/miscItems/fireworks.src
- A | pkg/items/miscItems/pkg.cfg
- A | pkg/items/musicStand/config/icp.cfg
- A | pkg/items/musicStand/config/itemdesc.cfg
- A | pkg/items/musicStand/pkg.cfg
- A | pkg/items/noncrafteditems.txt
- A | pkg/items/origami/config/icp.cfg
- A | pkg/items/origami/config/itemdesc.cfg
- A | pkg/items/origami/config/origami.cfg
- A | pkg/items/origami/folded/method.src
- A | pkg/items/origami/kit/method.src
- A | pkg/items/origami/kit/use.src
- A | pkg/items/origami/pkg.cfg
- A | pkg/items/pillows/config/icp.cfg
- A | pkg/items/pillows/config/itemdesc.cfg
- A | pkg/items/pillows/pkg.cfg
- A | pkg/items/rusticThemePack/config/icp.cfg
- A | pkg/items/rusticThemePack/config/itemdesc.cfg
- A | pkg/items/rusticThemePack/pkg.cfg
- A | pkg/items/shadowThemePack/config/icp.cfg
- A | pkg/items/shadowThemePack/config/itemdesc.cfg
- A | pkg/items/shadowThemePack/pkg.cfg
- A | pkg/items/spittoon/config/icp.cfg
- A | pkg/items/spittoon/config/itemdesc.cfg
- A | pkg/items/spittoon/pkg.cfg
- A | pkg/items/spittoon/spittoon/use.src
- A | pkg/items/statues/config/icp.cfg
- A | pkg/items/statues/config/itemdesc.cfg
- A | pkg/items/statues/pkg.cfg
- A | pkg/items/tables/config/icp.cfg
- A | pkg/items/tables/config/itemdesc.cfg
- A | pkg/items/tables/pkg.cfg
- A | pkg/items/tapestry/config/icp.cfg
- A | pkg/items/tapestry/config/itemdesc.cfg
- A | pkg/items/tapestry/pkg.cfg
- A | pkg/items/tokunoFurniture/config/icp.cfg
- A | pkg/items/tokunoFurniture/config/itemdesc.cfg
- A | pkg/items/tokunoFurniture/pkg.cfg
- A | pkg/items/tools/config/icp.cfg
- A | pkg/items/tools/config/itemdesc.cfg
- A | pkg/items/tools/pkg.cfg
- A | pkg/items/waterTrough/config/icp.cfg
- A | pkg/items/waterTrough/config/itemdesc.cfg
- A | pkg/items/waterTrough/pkg.cfg
- M | pkg/mobiles/job/job.src
- M | pkg/multis/customhousing/decaywatcher.src
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/house/config/itemdesc.cfg
- M | pkg/multis/house/multiDeed/use.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/multis/staticHousing/sign/destroy.src
- A | pkg/opt/alchemyplus/heatingstand.src
- M | pkg/opt/alchemyplus/itemdesc.cfg
- A | pkg/opt/alryc/textcmd/test/combattest.src
- M | pkg/opt/alryc/textcmd/test/createitemdesc.src
- A | pkg/opt/alryc/textcmd/test/forceexceptional.src
- A | pkg/opt/alryc/textcmd/test/materialbag.src
- A | pkg/opt/alryc/textcmd/test/osibsgump.src
- A | pkg/opt/alryc/textcmd/test/osibulkgump.src
- A | pkg/opt/alryc/textcmd/test/osibulkrewardsgump.src
- A | pkg/opt/alryc/textcmd/test/osiquestgump.src
- A | pkg/opt/alryc/textcmd/test/osiquestnpcgump.src
- M | pkg/opt/areaspawner/config/areachests.cfg
- M | pkg/opt/areaspawner/config/eventareachests.cfg
- M | pkg/opt/dyteitems/dyeitems.cfg
- M | pkg/opt/dyteitems/itemdesc.cfg
- M | pkg/opt/guilds/commands/player/guilds.src
- M | pkg/opt/guilds/include/guildconstants.inc
- D | pkg/opt/lighting/change.src
- D | pkg/opt/lighting/duration.src
- D | pkg/opt/lighting/itemdesc.cfg
- D | pkg/opt/lighting/lighting.html
- D | pkg/opt/lighting/pkg.cfg
- M | pkg/opt/omegacache/categories.cfg
- A | pkg/opt/questpkg/config/icp.cfg
- A | pkg/opt/questpkg/config/quests.cfg
- A | pkg/opt/questpkg/include/questdeath.inc
- A | pkg/opt/questpkg/include/questdebuggump.inc
- A | pkg/opt/questpkg/include/questjournal.inc
- A | pkg/opt/questpkg/include/questnpcgump.inc
- A | pkg/opt/questpkg/include/questpkg_gumpstyle.inc
- A | pkg/opt/questpkg/include/queststate.inc
- A | pkg/opt/questpkg/pkg.cfg
- A | pkg/opt/questpkg/textcmd/gm/questtag.src
- A | pkg/opt/questpkg/textcmd/test/queststate.src
- A | pkg/opt/rituals/altar/create.src
- A | pkg/opt/rituals/altar/destroy.src
- A | pkg/opt/rituals/altar/gump.inc
- A | pkg/opt/rituals/altar/use.src
- M | pkg/opt/rituals/captor/control.src
- M | pkg/opt/rituals/captor/method.src
- A | pkg/opt/rituals/chantbook/use.src
- M | pkg/opt/rituals/config/itemdesc.cfg
- M | pkg/opt/rituals/config/rituals.cfg
- A | pkg/opt/rituals/config/spells.cfg
- M | pkg/opt/rituals/crystal/method.src
- M | pkg/opt/rituals/equipment/robe.src
- M | pkg/opt/rituals/equipment/staff.src
- A | pkg/opt/rituals/include/altarquest.inc
- A | pkg/opt/rituals/include/chantbook.inc
- M | pkg/opt/rituals/include/rituals.inc
- M | pkg/opt/rituals/pkg.cfg
- M | pkg/opt/rituals/rituals/attunement.src
- M | pkg/opt/rituals/rituals/bloodSeeking.src
- M | pkg/opt/rituals/rituals/consecration.src
- A | pkg/opt/rituals/rituals/demonstration.src
- M | pkg/opt/rituals/rituals/disenchantment.src
- A | pkg/opt/rituals/rituals/elementalWard.src
- M | pkg/opt/rituals/rituals/enhancement.src
- M | pkg/opt/rituals/rituals/freeMovement.src
- M | pkg/opt/rituals/rituals/hardening.src
- M | pkg/opt/rituals/rituals/manaDimissal.src
- M | pkg/opt/rituals/rituals/manaFlux.src
- A | pkg/opt/rituals/rituals/perilousTheurgy.src
- M | pkg/opt/rituals/rituals/physicalWard.src
- A | pkg/opt/rituals/rituals/planardWard.src
- M | pkg/opt/rituals/rituals/quickHealing.src
- A | pkg/opt/rituals/rituals/racialTheurgy.src
- M | pkg/opt/rituals/rituals/resilience.src
- M | pkg/opt/rituals/rituals/restoration.src
- M | pkg/opt/rituals/rituals/spellBouncing.src
- M | pkg/opt/rituals/rituals/spellWarding.src
- M | pkg/opt/rituals/rituals/venomBane.src
- M | pkg/opt/rituals/rituals/venomMastery.src
- A | pkg/opt/rituals/rituals/vitalInfusion.src
- M | pkg/opt/rituals/scroll/use.src
- A | pkg/opt/rituals/textcmd/test/chantbook.src
- A | pkg/opt/rituals/textcmd/test/chantbookcalib.src
- A | pkg/opt/rituals/textcmd/test/ritualbag.src
- A | pkg/opt/rituals/textcmd/test/ritualbooks.src
- M | pkg/opt/songbook/itemdesc.cfg
- M | pkg/opt/spawnpoint/checkpoint.src
- A | pkg/opt/spawnpoint/config/itemgroups.cfg
- M | pkg/opt/spawnpoint/spawnpoint.src
- M | pkg/packethooks/SingleClick/singleClick.src
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/packethooks/megacliloc/mobiledata.src
- M | pkg/std/alchemy/itemdesc.cfg
- M | pkg/std/begging/begging.src
- M | pkg/std/blacksmithy/blacksmithy.cfg
- M | pkg/std/blacksmithy/itemdesc.cfg
- M | pkg/std/blacksmithy/make_blacksmith_items.src
- M | pkg/std/blacksmithy/meltdown.src
- A | pkg/std/bowcraft/bowcraft.cfg
- A | pkg/std/bowcraft/bowcraft.src
- A | pkg/std/bowcraft/itemdesc.cfg
- A | pkg/std/bowcraft/pkg.cfg
- A | pkg/std/bulkorders/bulkorder.inc
- A | pkg/std/bulkorders/bulkorder_generation.inc
- A | pkg/std/bulkorders/bulkorder_gumpstyle.inc
- A | pkg/std/bulkorders/bulkorder_ledger.inc
- A | pkg/std/bulkorders/bulkorder_matching.inc
- A | pkg/std/bulkorders/bulkorder_templates.cfg
- A | pkg/std/bulkorders/bulkorderbook.src
- A | pkg/std/bulkorders/bulkorderbook_caninsert.src
- A | pkg/std/bulkorders/bulkorderdeed.src
- A | pkg/std/bulkorders/bulkorderrewards.src
- A | pkg/std/bulkorders/bulkorders.cfg
- A | pkg/std/bulkorders/commands/gm/autofillbod.src
- A | pkg/std/bulkorders/commands/gm/forcegivebod.src
- A | pkg/std/bulkorders/commands/gm/givebulkorderbook.src
- A | pkg/std/bulkorders/commands/gm/resetbodcooldown.src
- A | pkg/std/bulkorders/itemdesc.cfg
- A | pkg/std/bulkorders/pkg.cfg
- A | pkg/std/bulkorders/rewards.cfg
- M | pkg/std/carpentry/carpentry.cfg
- M | pkg/std/carpentry/carpentry.src
- M | pkg/std/carpentry/itemdesc.cfg
- M | pkg/std/cooking/cooking.cfg
- M | pkg/std/cooking/cooking.src
- M | pkg/std/cooking/food.inc
- M | pkg/std/cooking/itemdesc.cfg
- M | pkg/std/decorations/itemdesc.cfg
- M | pkg/std/fishing/itemdesc.cfg
- M | pkg/std/healing/itemdesc.cfg
- M | pkg/std/lockpicking/config/lockpicking.cfg
- M | pkg/std/mining/itemdesc.cfg
- M | pkg/std/musicianship/itemdesc.cfg
- M | pkg/std/removetrap/removetrap.src
- M | pkg/std/snooping/stealitems.cfg
- M | pkg/std/spells/itemdesc.cfg
- M | pkg/std/tailoring/itemdesc.cfg
- M | pkg/std/tailoring/make_cloth_items.src
- M | pkg/std/tailoring/tailoring.cfg
- M | pkg/std/tinkering/itemdesc.cfg
- M | pkg/std/tinkering/tinker.cfg
- M | pkg/std/tinkering/tinkering.src
- M | pkg/std/tracking/tracking.src
- M | pkg/std/training/dummy.src
- A | pkg/std/traps/commands/seer/cleartraps.src
- A | pkg/std/traps/commands/seer/trap.src
- A | pkg/std/traps/config/traps.cfg
- A | pkg/std/traps/include/traps.inc
- M | pkg/std/traps/itemdesc.cfg
- A | pkg/std/traps/trapScripts/dartTrap.src
- A | pkg/std/traps/trapScripts/explosionTrap.src
- A | pkg/std/traps/trapScripts/gasTrap.src
- A | pkg/std/traps/trapScripts/magicTrap.src
- A | pkg/std/traps/trapScripts/setTrap.src
- A | pkg/systems/combat/airFuryOnHit.src
- A | pkg/systems/combat/airFuryScript.src
- M | pkg/systems/combat/config/enchantableitems.cfg
- M | pkg/systems/combat/config/hitscriptdesc.cfg
- M | pkg/systems/combat/config/itemdesc.cfg
- M | pkg/systems/combat/config/onhitscriptdesc.cfg
- A | pkg/systems/combat/earthFuryOnHit.src
- A | pkg/systems/combat/earthFuryScript.src
- A | pkg/systems/combat/fireFuryOnHit.src
- A | pkg/systems/combat/fireFuryScript.src
- A | pkg/systems/combat/holyFuryOnHit.src
- A | pkg/systems/combat/holyFuryScript.src
- A | pkg/systems/combat/necroFuryOnHit.src
- A | pkg/systems/combat/necroFuryScript.src
- A | pkg/systems/combat/waterFuryOnHit.src
- A | pkg/systems/combat/waterFuryScript.src
- M | pkg/systems/crafting/include/craftingfunctions.inc
- M | pkg/systems/playervendor/itemdesc.cfg
- M | pkg/systems/playervendor/vendordeed.src
- M | pkg/utils/mdgumps/commands/test/gumpbrowser.src
- M | pkg/utils/mdgumps/scripts/gumpbrowser/preview.src
- M | scripts/ai/merchant.src
- A | scripts/ai/valthor.src
- M | scripts/control/skilladvancerequip.src
- M | scripts/control/trashInsert.src
- M | scripts/include/autoloop.inc
- M | scripts/include/itemutil.inc
- M | scripts/include/objtype.inc
- M | scripts/items/bladed.src
- D | scripts/items/fletch.src
- M | scripts/misc/death.src
- M | scripts/misc/questbutton.src

---

## Detailed Changes By Theme

### 1. Bulk Order Deeds (BOD) system — built from scratch, all 8 crafts, all 5 phases

**Files involved:** entire new `pkg/std/bulkorders/` package (24 files: `bulkorder.inc`, `bulkorder_generation.inc`, `bulkorder_matching.inc`, `bulkorder_gumpstyle.inc`, `bulkorder_ledger.inc`, `bulkorder_templates.cfg`, `bulkorders.cfg`, `bulkorderbook.src` + `bulkorderbook_caninsert.src`, `bulkorderdeed.src`, `bulkorderrewards.src`, `rewards.cfg`, `itemdesc.cfg`, `pkg.cfg`, 4 GM commands); `pkg/mobiles/job/job.src` (trimmed the 6 overlapping crafts out of the old job-issuance tradelist); `scripts/ai/merchant.src` (new "bulk order"/"bulk order rewards" speech triggers); `pkg/packethooks/megacliloc/itemdata.src` (deed/book tooltips); `pkg/systems/crafting/include/craftingfunctions.inc` (`IncRevision` fix for non-deed items, both carpentry and tinkering).

**Notable functional changes:**
- Two-tier system (Small unlocks at 50 skill, Large at 100), no Medium tier, no OSI-style points decay/banking — a plain running points balance instead.
- Each craft's Small-BOD item pool is read live from that craft's own recipe `.cfg` at generation time (never duplicated into `bulkorders.cfg`), filtered by an `ExcludeItem` list per craft. Per-craft filter shape: Blacksmithy/Carpentry/Tailoring/Tinkering key on a `Type` field; Bowcraft on `Category` in {Bows, Crossbows}; Inscription on a `Circle` field; Cooking dedupes numbered recipes by `product_objtype`; Alchemy reads `Potion <name>`-keyed blocks, extended with 15 hardcoded objtypes for `pkg/opt/alchemyplus`'s Grand Mage Refresh Elixir/Rebirth Potion/etc. family (those don't expose their real objtype via any cfg field — only via a `case(itemtype)` table in `alchemyplus.src`).
- Large BOD is a curated catalog, not a random N-item draw: 166 hand-built templates across 8 crafts in `bulkorder_templates.cfg`, matched by exact item + material + quality (quantity deliberately not matched). Material rolled once per Large via a quadratic Difficulty curve; exceptional rolled once at a flat 1-in-6 for the whole order.
- Reward system has 4 independent currencies: flat 10 points/Small (Large = 3x the sum of consumed Smalls' points, redeemed 36pts/token into the existing `vanitytoken` Omega Token currency); Gold scaling 350-3,500 by material Quality for the 6 material-tiered crafts (flat 1,000/Small for Cooking/Alchemy); Fame/Karma via the real `AwardFame`/`AwardKarma` helpers (which divide by 50 and clamp to 1-25 — reward budget rescaled to fit that real engine constraint rather than bypassing it), randomly split 20-80% between the two per turn-in.
- The Book (`Container`, graphic `0x2259`) is a real 500-slot container with a custom paginated browsing gump, not a DataFile abstraction.
- Carpentry's deed-only recipes are matched via their `MakeDeed` field rather than being excluded from the pool, since `ApplyMaterialPropertiesDeed` creates the deed item directly rather than the recipe's own key objtype.

**Expected impact:** an entirely new mid/late-game crafting-and-turn-in loop for all 8 crafting skills; see patch notes for the player-facing framing.

### 2. POLMD item-package port — ~30 new decor/furniture/tool packages

**Files involved:** all new top-level directories under `pkg/items/` (LifeStones, advancedTrainingDummy, banners, benches, celestial, chairs, clocks, coffins, commcrystals, debris, decorativeArmor/Shields/Weapons, displayCases, engravingTools, evilHomeDecor, forge, fountains, gardenShed, gargishFurniture, gothicThemePack, graveStones, interiordecoratortool, kingsCollection, lighting, miscItems, musicStand, origami, pillows, rusticThemePack, shadowThemePack, spittoon, statues, tables, tapestry, tokunoFurniture, tools, waterTrough), plus ~25 new `pkg/items/deed/built/*.cfg` deed-build files for the furniture/lamp/table items among them, plus modifications to several existing crafts' recipe `.cfg`s (`carpentry.cfg`, `tailoring.cfg`, `tinker.cfg`, `cooking.cfg`, `blacksmithy.cfg`) to make the new items craftable.
**Notable functional changes:** each package follows the same shape (`pkg.cfg`, `config/icp.cfg` registration, `config/itemdesc.cfg`, and a `use.src`/`method.src` where the item needs interactive behavior — engraving tools, lighting fixtures, gravestones, spittoons, communication crystals, celestial instruments, origami, evil-decor walk-on effects). All new craftable items were subsequently wired into the BOD template catalog (Theme 1).
**Expected impact:** a large volume of new placeable decor, furniture, and tool items become craftable and obtainable, spanning most of the game's decorative-item space.

### 3. Statue crafting relocated Carpentry -> Tinkering, new combo-material mechanic

**Files involved:** `pkg/std/carpentry/carpentry.cfg` (12 statue recipes removed), `pkg/std/tinkering/tinker.cfg` (same 12 added as Type-42 "Statues" recipes, clay-primary), `pkg/std/tinkering/tinkering.src` (`TryToMakeItem` extended).
**Notable functional changes:** the 12 statues (Fire Daemon, Minotaur, Poseidon, Squirrel, Arcanist, Warrior, plain Statue x3, Bronze Archer/Fairy/Man) moved from wood-primary Carpentry recipes to clay-primary Tinkering recipes. New engine behavior: when a Tinkering recipe's `CoMaterial` field is set, `TryToMakeItem` now prompts for a second `Target()`, requires it to be a log or an ingot, dynamically resolves the skill gate to Carpentry (wood) or Blacksmithy (ore) based on `IsLog`/`IsIngot`, scales the secondary skill requirement by that specific material's own `Difficulty`, and names/colors the finished item off the secondary material via `ApplyMaterialProperties(..., SKILLID_TINKERING)` (which already auto-detects log vs. ingot). Previously Tinkering's `CoSkillid` field was a pure skill-level gate with no actual second-material consumption; this is now a real dual-material recipe shape.
**Expected impact:** statues are crafted via Tinkering (clay) with the player's choice of a second material (any log or any ingot), and the finished statue is named/hued after whichever material was used (e.g. a Bloodwood-made statue vs. a Valorite-made one).

### 4. Rituals package — revived a non-functional system, added 6 new rituals, altar/quest content

**Files involved:** `pkg/opt/rituals/include/rituals.inc`, `config/spells.cfg` (new), `config/rituals.cfg` (renumbered), `include/chantbook.inc` + `chantbook/use.src` (new), `altar/*.src` + `include/altarquest.inc` (new), 6 new ritual scripts (`demonstration.src`, `elementalWard.src`, `planardWard.src`, `vitalInfusion.src`, `perilousTheurgy.src`, `racialTheurgy.src`), plus edits to every existing ritual script for the new risk/failure-feedback system.
**Notable functional changes:** ritual scrolls previously did nothing when cast because no `Spell` config entries existed for the ritual spell-id range at all; added the missing registrations and renumbered every `Ritual <id>` block in `rituals.cfg` to match. Added a two-gate risk system (a hard Magery floor, then a success-percentage roll that can fizzle/backfire/catastrophically fail) with per-ritual risk bands driven by an item's current enchant "tier" for 8 reinforcement-style rituals. Added `RITUAL_FailFeedback()` so every failure branch (wrong tile/facing/timing/words, interrupted, low mana) now messages the caster instead of silently no-op'ing. New Ritual Chant Book system (readable in-game books reproducing a ritual's chant lines) and new Ritual Altar content (a placed two-piece altar that, during an active quest, accepts specific reagent offerings to summon a boss NPC).
**Expected impact:** rituals are now a functioning system rather than dead flavor content; see patch notes.

### 5. New generic quest framework (`pkg/opt/questpkg`)

**Files involved:** entire new package (`config/quests.cfg`, `include/queststate.inc`, `questjournal.inc`, `questnpcgump.inc`, `questdeath.inc`, `questdebuggump.inc`, `questpkg_gumpstyle.inc`, GM/test commands), plus `scripts/ai/valthor.src` (new quest-giver NPC) and `scripts/misc/questbutton.src` (modified).
**Notable functional changes:** config-driven quests (name/description/quest-giver/journal text/kill-count objectives/turn-in item/rewards) backed by a per-character DataFile rather than CProps; a quest-giver gump, a player quest-journal gump with objective tracking, and a kill-tracking hook that tags NPCs so kills count toward the right quest/player. One quest defined so far ("The Rite of Consecration"), tied into the new rituals altar content (Theme 4).
**Expected impact:** first proper quest-journal/quest-giver system in the game; currently powers one quest chain end to end.

### 6. Bowcraft extracted to its own package + Omega Cache crafting-tool targeting

**Files involved:** new `pkg/std/bowcraft/` package (`bowcraft.src`, `bowcraft.cfg`, `itemdesc.cfg`, `pkg.cfg`); removed `scripts/items/fletch.src` and ~439 lines from `scripts/items/bladed.src`; removed `config/bowcraft.cfg`; `pkg/opt/omegacache/categories.cfg` and `pkg/std/blacksmithy/make_blacksmith_items.src` (cache-targeting support).
**Notable functional changes:** Bowcraft (bows/crossbows/arrows/bolts), previously crafted through a chunk of the generic bladed-weapon script plus a separate fletch script, is now its own package with a dedicated Bowcraft Tool (`0x1022`) and a from-scratch gump UI (category buttons: Shafts/Bows/Crossbows/Ammo/Misc) replacing the old text-menu flow, plus a one-roll bulk-craft option. Separately, crafting tools (blacksmith hammer, saw, tinker's tools, sewing kit, and Bowcraft's own material auto-detect) can now be targeted directly at an Omega Cache to pull stacked raw material from it instead of requiring materials in the backpack first.
**Expected impact:** see patch notes.

### 7. Cooking fix (static fireplaces/ovens) + Soulforge-gate fix (Tinkering Crystal Items)

**Files involved:** `pkg/std/cooking/cooking.src`, `pkg/std/tinkering/tinkering.src`.
**Notable functional changes:** cooking's "near a fire/oven" check only scanned real spawned items via `ListItemsNearLocation`; added a static-tile scan (`ListStaticsAtLocation`) so a house's built-in fireplace/oven (never spawned as a real item) is recognized. Removed an erroneous `NearASoulforge()` gate that was blocking Tinkering's "Crystal Items" menu category — a check that made sense for Carpentry's Soulforge-gated items but not this Tinkering category.
**Expected impact:** cooking now works next to built-in house fireplaces/ovens; Tinkering's Crystal Items no longer requires standing near a Soulforge.

### 8. Trash can sound, elemental-ammo objtype/hue fixes

**Files involved:** `scripts/control/trashInsert.src`, `scripts/include/objtype.inc`, `config/itemdesc.cfg`.
**Notable functional changes:** trash cans previously played no sound on item insert, unlike regular containers; copied the same material-based sound logic (coins clink by material, gems have their own sound, everything else gets a generic thump) from `pkg/items/containers/container/onInsert.src`. Fixed `UOBJ_FIRE_ARROW` (was `0x16051`, an invalid 5-digit overflow of the real `0x6051`), added missing `UOBJ_ICE_ARROW`/`UOBJ_THUNDER_BOLT` constants, corrected Thunderbolt's graphic (was `0x1bfe`, should be `0x1BFB`) and adjusted Fire/Ice Arrow and Thunderbolt hues.
**Expected impact:** trash cans make a sound on insert now; fire/ice arrows and thunderbolts render with the correct color, and thunderbolts show the correct item graphic.

### 9. `.materialbag` GM test command

**Files involved:** `pkg/opt/alryc/textcmd/test/materialbag.src` (new).
**Notable functional changes:** drops 6 backpacks (Logs/Ore/Ingots/Hides/Tailoring/Materials) each stacked with 5,000 units of every crafting-relevant raw material, plus one of every crafting tool, at the caster's feet.
**Expected impact:** none for players — GM/staff crafting-test convenience only.

### 10. Guild color picker — full hue range, pagination, live preview

**Files involved:** `pkg/opt/guilds/commands/player/guilds.src`, `pkg/opt/guilds/include/guildconstants.inc`.
**Notable functional changes:** expanded the selectable guild-color palette from ~150 hand-picked hues to nearly the full dyeable range 1000-2999 (minus hand-excluded skin-tone/near-duplicate bands) — paginated 150 colors per page, plus a one-at-a-time cycle-preview control with a live 3-mannequin (mage/warrior/ranger) preview. Also fixed the "which guild already owns this color" lookup to build one reverse-lookup dictionary per gump-open instead of rescanning every guild per swatch per render.
**Expected impact:** guilds have a vastly larger color selection with a working live preview.

### 11. Begging payout update

**Files involved:** `pkg/std/begging/begging.src`.
**Notable functional changes:** random bonus on a successful beg widened from `Random(20)` to `Random(51)`; the flat gold-coin payout was replaced with a 3-way currency roll (30% copper at 4x, 30% silver at 2x, 40% gold at 1x, roughly value-normalized).
**Expected impact:** begging pays out in copper/silver/gold stacks with a slightly higher, more variable amount.

### 12. Precious metals: Copper renamed to Orichalcum, coin melting

**Files involved:** `pkg/std/mining/itemdesc.cfg`, `pkg/std/blacksmithy/meltdown.src`, `pkg/opt/omegacache/categories.cfg`.
**Notable functional changes:** the ore/ingot previously named Copper (`0x600C`/`0x6014`) renamed to Orichalcum, freeing the "Copper" name for a new coin-melting feature: targeting a stack of Gold/Silver/Copper coins with Tongs at a forge converts them to real Gold/Silver/Copper Ingots at 500/1,000/2,000 coins per ingot respectively (`MeltCoinsToIngot()`).
**Expected impact:** see patch notes.

### 13. New "Fury" single-element combat enchants (ritual-only)

**Files involved:** `pkg/systems/combat/{fire,water,air,earth,holy,necro}FuryScript.src` + matching `*FuryOnHit.src` (12 new files), `config/hitscriptdesc.cfg`, `config/onhitscriptdesc.cfg`, `config/enchantableitems.cfg`, `config/itemdesc.cfg`.
**Notable functional changes:** 6 new single-element weapon/armor hitscript pairs, each a single-element trim of the existing combined `trielementalscript`/`trielementalonhit`, registered as new enchantment suffixes ("of Fire Fury" / cursed "of Fire Vengeance", etc.). Explicitly ritual-granted only (via the new Perilous Theurgy ritual, Theme 4) — never rolled as normal loot.
**Expected impact:** no change to monster loot; a new single-element proc enchant obtainable only via ritual.

### 14. Lighting package merge completed

**Files involved:** `pkg/items/lighting/*` (absorbed the remaining content), `pkg/opt/lighting/*` (deleted: `change.src`, `duration.src`, `itemdesc.cfg`, `lighting.html`, `pkg.cfg`), `pkg/items/forge/config/itemdesc.cfg` (received 2 stray forge-middle stub items that belonged there), 53 `:lightingfixtures:` script-path references across 4 files reverted to `:lighting:`.
**Notable functional changes:** a prior session had started migrating `pkg/opt/lighting` into `pkg/items/lighting` (both declared package `Name lighting`, a real collision) but left it half-finished; completed the merge (moved all remaining real content — firecol, campfire, firepit2, fireplace corners, ~30 window-facing wall stubs — into `pkg/items/lighting`, confirmed the old `change.src` toggle script had zero live callers) and deleted the old package outright rather than renaming either side.
**Expected impact:** none — pure internal deduplication of two packages that had accidentally claimed the same name.

### 15. Objtype-collision and compile-error fixes (server would not boot)

**Files involved:** `pkg/items/advancedTrainingDummy/config/itemdesc.cfg`, `pkg/items/deed/built/advancedTrainingDummy.cfg`, `scripts/include/objtype.inc`, `pkg/std/carpentry/carpentry.cfg`, `config/multis.cfg`, `pkg/items/miscItems/config/itemdesc.cfg`, `pkg/std/cooking/itemdesc.cfg`, `pkg/std/cooking/cooking.cfg`, `config/food.cfg`, `pkg/items/tools/config/itemdesc.cfg`, `pkg/items/clocks/use.src`, `pkg/std/traps/trapScripts/{dartTrap,explosionTrap,gasTrap,magicTrap}.src`.
**Notable functional changes:** a full-repo boot/compile pass (triggered by repeated server-start attempts) surfaced and fixed:
  - Advanced Training Dummy's 16 objtypes (`0x9821`/`0x981C`/`0x9822`/`0x9826`-`0x9833`) collided with `pkg/systems/combat/config/itemdesc.cfg`'s pre-existing armor catalog — relocated the whole family to a verified-free `0x9834`-`0x9845` block, including the deed's `Build` component list, `objtype.inc`'s 4 `UOBJ_DUMMY*_ADV_*` constants, the Carpentry recipe key, `config/multis.cfg`'s two auto-registered static-housing `Multi` blocks, and the `dummy` script reference (was a bare `Script dummy` resolving to a nonexistent file in the item's own package — corrected to `:training:dummy`, the actual shared script).
  - `pkg/items/miscItems`'s "Globe" (`0x1048`) collided with the base `config/itemdesc.cfg`'s "globe1" container — relocated to `0x1DCA2` with a `Graphic 0x1048` override to preserve its appearance; later renamed from "Globe" to "MiscGlobe" after it turned out to coincidentally share a display name with an unrelated pre-existing item at `0x1047`.
  - `pkg/items/tools`'s "shears" weapon had two broken script references (`:combat:destroy`, `:combat:mainHitScript` — neither file exists) — corrected to the real, universally-used `::maindestroy` and `:combat:mainhit`.
  - Cooking had 4 genuine duplicate items (Pitcher of Water/Wine/Cider, Corn Muffin) each defined twice at different objtypes with slightly different stats; removed the older definition of each and repointed the surviving objtype's recipe, `food.cfg` hunger value, and BOD template references accordingly.
  - `pkg/items/clocks/use.src` had its `program` literally named `use`, colliding with the reserved `use` keyword — renamed to `ClockUse`.
  - 4 new trap scripts (`dartTrap`/`explosionTrap`/`gasTrap`/`magicTrap`) were missing `use os;` (for `SleepMS`) and `include "include/dotempmods"` (for `ApplyRawDamage`/`SetPoison`).
**Expected impact:** the server would not boot at all prior to these fixes ("ObjType defined more than once" configuration errors); a full repo compile now passes with 0 errors and a full itemdesc scan shows 0 remaining duplicate objtypes anywhere in the codebase.

---

## Validation Notes

- Diff range: `git diff b367a82..HEAD` / `git log --oneline b367a82..HEAD`.
- Full-repo compile verified clean via `scripts/ecompile.exe -b -r ..` run from `scripts/`: 1373 scripts, 0 errors (run repeatedly during the boot-error fixup pass in Theme 15, each time confirming 0 errors before moving to the next issue).
- Full itemdesc objtype-collision scan run across all 130 `itemdesc.cfg` files in the repo (custom script, not part of the normal build) confirmed 0 remaining duplicate objtypes (Item/Armor/Weapon/Boat/Container/Door/House/Map/Spellbook namespace; `Graphic`-elemtype entries are a separate namespace and excluded from this check) after the Theme 15 fixes.
- Working tree was clean at the time of this writing (everything in this range is committed as of `db15da7`).
