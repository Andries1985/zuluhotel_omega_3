# Developer Changelog - v3.1.0

Range: Patch-3.0.9..Patch-3.1.0 (commit `83f1748`..`e3d88b0`)
Branch: Patch-3.1.0
Date: 2026-09-01

---

## Scope Summary

- Total files changed: 295 (9 added, 247 modified, 38 deleted, 1 renamed)
- Net textual delta: 13,055 insertions, 16,513 deletions
- Largest shifts:
  - `pkg/systems/crafting/include/craftmenu.inc` (+1516, new file) — the new shared crafting-gump framework
  - `pkg/std/carpentry/carpentry.cfg` (+1473 net), `pkg/std/tailoring/tailoring.cfg` (+890 net), `pkg/std/tinkering/tinker.cfg` (+838 net) — recipe files reformatted to the new craftmenu schema
  - `pkg/multis/staticHousing/include/old-gumps.inc` (-1429), `pkg/utils/mdgumps/include/old-gumps.inc` (-1416), `pkg/utils/gumps/include/old-gumps.inc` (-1415), `pkg/utils/mdgumps/include/old/old-gumps.inc` (-1003), `pkg/utils/gumps/include/old/old-gumps.inc` (-1003), `pkg/utils/gumps/include/gumps.inc` (-937) — the legacy `pkg/utils/gumps` package and its duplicated `old-gumps.inc` copies, deleted repo-wide
  - `pkg/std/carpentry/carpentry.src` (-790 net), `pkg/std/bowcraft/bowcraft.src` (-620 net), `pkg/std/blacksmithy/make_blacksmith_items.src` (-578 net) — crafting scripts shrank as duplicated gump code moved into `craftmenu.inc`
  - `pkg/opt/shilhook/textcmd/admin/{setglobalmultipliers,setplayermultipliers}.src` (~558 lines each) — mechanical old-gump-API-to-new-API port, no behavior change
  - `pkg/opt/townstones/tstone.inc` (+536, this session) — the consolidated townstones helper library
  - `pkg/std/runebook/runicatlas.src` (+471, new), `regions/regions.cfg` (+466 net), `pkg/std/runebook/runebook.src` (-446 net), `pkg/multis/house/multiSign/use.src` (-428 net), `pkg/opt/townstones/textcmd/admin/townbankstatus.src` (-362 net, this session), `pkg/std/runebook/runebookactions.inc` (+357, new)
  - `ainotes/missing-equipment-static-overlap-20260831.json` (+2911, new) — raw output of a new dev audit tool, not game content
- Non-merge commits in range (oldest to newest):
  - `c3026a7` Crafting Gump Update / Free magery and musicmanship to crafters
  - `1423381` Tokuno Regions
  - `6ac8b46` go locs updated
  - `bca5e1a` Removed troublesome spawns
  - `677a621` npc update and removal of water spawns on land
  - `aade070` Runic Atlas Update
  - `e3d88b0` Tons of changes (this commit; a full code-review-driven bugfix pass — 3 CRITICAL, 26 HIGH, 16 MEDIUM findings — plus a repo-wide `:gumps:`→`:mdgumps:` include migration and several standalone efficiency fixes)
- Themes below are organized by subsystem, not by commit, since `e3d88b0` alone touches 252 files spanning many unrelated fixes built up over a long working session.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- M | .gitignore
- A | ainotes/missing-equipment-static-overlap-20260831.json
- M | config/command_synopses.cfg
- M | config/golocs_by_id.cfg
- M | config/itemdesc.cfg
- M | config/mrcspawn.cfg
- M | config/npcdesc.cfg
- D | out.txt
- M | pkg/commands/commands/gm/mobedit.src
- M | pkg/items/commcrystals/commCrystal.src
- M | pkg/items/commcrystals/crystalControl.src
- A | pkg/items/deed/built/carpentryFlourMill.cfg
- A | pkg/items/deed/built/carpentryOven.cfg
- M | pkg/items/deed/deed/use.src
- M | pkg/items/doors/config/itemdesc.cfg
- M | pkg/items/engravingTools/include/engravingTools.inc
- M | pkg/items/keys/include/key.inc
- M | pkg/items/kingsCollection/config/itemdesc.cfg
- M | pkg/items/lighting/config/itemdesc.cfg
- M | pkg/items/musicStand/config/icp.cfg
- M | pkg/items/origami/kit/use.src
- M | pkg/items/sysbook/books/readOnly.src
- M | pkg/items/sysbook/commands/admin/createshardlibrary.src
- M | pkg/items/sysbook/commands/admin/removefromshardlibrary.src
- M | pkg/items/sysbook/commands/admin/setbooktonotspawn.src
- M | pkg/items/sysbook/commands/admin/setbooktospawn.src
- M | pkg/items/sysbook/commands/gm/findbook.src
- M | pkg/items/sysbook/include/sysBook.inc
- M | pkg/multis/boat/multi/listener.src
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/scripts/customhousedeed.src
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/customhousing/signcontrol.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/staticHousing/commands/gm/removeStaticDeed.src
- M | pkg/multis/staticHousing/commands/gm/staticDeed.src
- M | pkg/multis/staticHousing/commands/player/decorate.src
- D | pkg/multis/staticHousing/include/old-gumps.inc
- M | pkg/multis/staticHousing/lockunlock.src
- M | pkg/multis/staticHousing/logon.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/multis/staticHousing/sign/destroy.src
- M | pkg/multis/staticHousing/sign/use.src
- M | pkg/multis/staticHousing/transferdeed/staticTransferDeed.src
- M | pkg/opt/Donator/donatorbearstone.src
- M | pkg/opt/Donator/donatorhorsestone.src
- M | pkg/opt/Donator/donatorllamastone.src
- M | pkg/opt/Donator/donatorostardstone.src
- M | pkg/opt/Donator/donatorrecall.src
- M | pkg/opt/alchemyplus/newpotions.src
- M | pkg/opt/alchemyplus/potionkeg.src
- M | pkg/opt/alryc/textcmd/player/mounttest.src
- M | pkg/opt/alryc/textcmd/test/editcharacter.src
- M | pkg/opt/alryc/textcmd/test/goteles.src
- M | pkg/opt/alryc/textcmd/test/gotomulti.src
- A | pkg/opt/alryc/textcmd/test/missingequipment.src
- A | pkg/opt/alryc/textcmd/test/testrunebook.src
- M | pkg/opt/areas/areas.cfg
- M | pkg/opt/areas/textcmd/admin/areas.src
- M | pkg/opt/areaspawner/config/areagroups.cfg
- M | pkg/opt/botanik/botanik.inc
- M | pkg/opt/botanik/harvest.src
- M | pkg/opt/champspawns/config/spawns.cfg
- M | pkg/opt/champspawns/include/death.inc
- M | pkg/opt/champspawns/include/rewards.inc
- M | pkg/opt/champspawns/include/skulls.inc
- M | pkg/opt/champspawns/include/spawning.inc
- M | pkg/opt/champspawns/textcmd/test/createchampionspawn.src
- M | pkg/opt/crafterboost/make_crafter_boosts.src
- M | pkg/opt/earth/earthportal.src
- M | pkg/opt/farming/farming.src
- M | pkg/opt/farming/gethoney.src
- M | pkg/opt/farming/newhive.src
- M | pkg/opt/guilds/commands/player/c.src
- M | pkg/opt/guilds/commands/player/guilds.src
- M | pkg/opt/guilds/commands/test/changeguildownership.src
- M | pkg/opt/guilds/include/guilds.inc
- M | pkg/opt/guilds/include/guildstonepicker.inc
- M | pkg/opt/ipban/textcmd/admin/ipban.src
- M | pkg/opt/karmafame/textcmd/player/karma.src
- M | pkg/opt/karmafame/textcmd/test/resetkf.src
- M | pkg/opt/loot/lootgump.src
- M | pkg/opt/lootlottery/commands/GM/cfglotto.src
- M | pkg/opt/moongates/moongate/ynwalkOnmoon.src
- M | pkg/opt/moongates/systemmoongate.src
- M | pkg/opt/msg/commands/player/msg.src
- M | pkg/opt/msg/commands/player/reply.src
- M | pkg/opt/msg/msgalert.src
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/omegacache/placecache.src
- M | pkg/opt/powerhour/textcmd/player/setph.src
- M | pkg/opt/powerscrolls/powerscroll.src
- M | pkg/opt/powerscrolls/statpotion.src
- M | pkg/opt/powerscrolls/textcmd/player/showcaps.src
- M | pkg/opt/powerscrolls/textcmd/test/lowerallchosencaps.src
- M | pkg/opt/powerscrolls/textcmd/test/lowerallchosenstatcaps.src
- M | pkg/opt/powerscrolls/textcmd/test/lowercaps.src
- M | pkg/opt/powerscrolls/textcmd/test/raiseallchosencaps.src
- M | pkg/opt/powerscrolls/textcmd/test/raiseallchosenstatcaps.src
- M | pkg/opt/powerscrolls/textcmd/test/raisecaps.src
- M | pkg/opt/powerscrolls/transcendscroll.src
- M | pkg/opt/questpkg/include/queststate.inc
- M | pkg/opt/rituals/altar/gump.inc
- M | pkg/opt/rituals/include/rituals.inc
- M | pkg/opt/roleplaying/textcmd/admin/fixstartgear.src
- M | pkg/opt/roleplaying/textcmd/seer/makerpergate.src
- M | pkg/opt/shilhook/textcmd/admin/setglobalmultipliers.src
- M | pkg/opt/shilhook/textcmd/admin/setplayermultipliers.src
- M | pkg/opt/spawnpoint/include/customnpc.inc
- M | pkg/opt/spawnpoint/textcmd/admin/createspawntrigger.src
- M | pkg/opt/spawnpoint/textcmd/admin/gotomobtype.src
- M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src
- M | pkg/opt/summoning/checkclasse.src
- M | pkg/opt/townstones/electionwatch.src
- M | pkg/opt/townstones/textcmd/admin/cleartownmembers.src
- M | pkg/opt/townstones/textcmd/admin/createtownstone.src
- M | pkg/opt/townstones/textcmd/admin/fixstone.src
- M | pkg/opt/townstones/textcmd/admin/gettowngold.src
- M | pkg/opt/townstones/textcmd/admin/removetownmember.src
- M | pkg/opt/townstones/textcmd/admin/resetpoll.src
- M | pkg/opt/townstones/textcmd/admin/townbankstatus.src
- M | pkg/opt/townstones/textcmd/player/playerruntowns.src
- M | pkg/opt/townstones/townlistbootstrap.src
- M | pkg/opt/townstones/tstone.inc
- M | pkg/opt/townstones/tstone.src
- M | pkg/opt/vanityshop/customitemdye.src
- M | pkg/opt/vanityshop/customitemname.src
- M | pkg/opt/vanityshop/runebookdye.src
- M | pkg/opt/vanityshop/vanityshop.src
- M | pkg/opt/versebook/Beast_Bond_Cancel.src
- M | pkg/opt/versebook/include/verseinfo.inc
- M | pkg/opt/versebook/versebook.src
- M | pkg/opt/zuluitems/Testclassbooststone.src
- M | pkg/opt/zuluitems/use_racegate.src
- M | pkg/opt/zuluitems/walkon_racegate.src
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/std/alchemy/bluepotion.src
- M | pkg/std/alchemy/whitepotion.src
- M | pkg/std/blacksmithy/blacksmithy.cfg
- M | pkg/std/blacksmithy/make_blacksmith_items.src
- M | pkg/std/bowcraft/bowcraft.cfg
- M | pkg/std/bowcraft/bowcraft.src
- M | pkg/std/bulkorders/bulkorder.inc
- M | pkg/std/bulkorders/rewards.cfg
- M | pkg/std/carpentry/carpentry.cfg
- M | pkg/std/carpentry/carpentry.src
- M | pkg/std/carpentry/carpentrydeed.src
- M | pkg/std/carpentry/itemdesc.cfg
- M | pkg/std/cartography/cartography.src
- M | pkg/std/cooking/cooking.src
- M | pkg/std/cooking/fillpitcher.src
- M | pkg/std/cooking/grinding.src
- M | pkg/std/detecthidden/detecthidden.src
- M | pkg/std/evalint/evalint.src
- M | pkg/std/help/help.src
- M | pkg/std/inscription/inscription.src
- M | pkg/std/itemid/itemid.src
- M | pkg/std/peacemaking/peacemaking.src
- M | pkg/std/poisoning/poisoning.src
- M | pkg/std/runebook/customspells.inc
- M | pkg/std/runebook/itemdesc.cfg
- M | pkg/std/runebook/runebook.src
- A | pkg/std/runebook/runebookactions.inc
- M | pkg/std/runebook/runecaninsert.src
- M | pkg/std/runebook/runeoninsert.src
- A | pkg/std/runebook/runicatlas.src
- M | pkg/std/spells/archprot.src
- M | pkg/std/spells/gate.src
- M | pkg/std/spells/magictrap.src
- M | pkg/std/spells/mark.src
- M | pkg/std/spells/recall.src
- M | pkg/std/spells/reveal.src
- M | pkg/std/tailoring/make_cloth_items.src
- M | pkg/std/tailoring/scissors.src
- M | pkg/std/tailoring/tailoring.cfg
- M | pkg/std/tinkering/itemdesc.cfg
- M | pkg/std/tinkering/tinker.cfg
- M | pkg/std/tinkering/tinkering.src
- M | pkg/std/traps/commands/seer/cleartraps.src
- M | pkg/std/traps/commands/seer/trap.src
- M | pkg/std/treasuremap/textcmd/admin/gototreasuremap.src
- M | pkg/systems/accounts/include/accounts.inc
- M | pkg/systems/combat/dualplanaronhit.src
- M | pkg/systems/combat/include/hitscriptinc.inc
- M | pkg/systems/combat/trielementalonhit.src
- M | pkg/systems/combat/trielementalscript.src
- M | pkg/systems/crafting/include/craftingfunctions.inc
- A | pkg/systems/crafting/include/craftmenu.inc
- M | pkg/systems/email/emailMessage/newEmail.src
- M | pkg/systems/email/include/email.inc
- M | pkg/systems/playervendor/playermerchant.src
- M | pkg/utils/datafile/pkg.cfg
- D | pkg/utils/gumps/changelog.txt
- D | pkg/utils/gumps/commands/admin/gumpprompt.src
- D | pkg/utils/gumps/commands/admin/htmlgump.src
- D | pkg/utils/gumps/commands/admin/requestgump.src
- D | pkg/utils/gumps/commands/admin/resizepic.src
- D | pkg/utils/gumps/commands/admin/samplegump.src
- D | pkg/utils/gumps/commands/admin/selectiongump.src
- D | pkg/utils/gumps/commands/admin/yesno.src
- D | pkg/utils/gumps/config/GumpInfo - Copy.cfg
- D | pkg/utils/gumps/config/GumpInfo.cfg
- D | pkg/utils/gumps/config/fontSize.cfg
- D | pkg/utils/gumps/config/icp.cfg
- D | pkg/utils/gumps/include/autoClose.inc
- D | pkg/utils/gumps/include/gumpprompt.inc
- D | pkg/utils/gumps/include/gumps.inc
- D | pkg/utils/gumps/include/gumps_ex.inc
- D | pkg/utils/gumps/include/htmlgump.inc
- D | pkg/utils/gumps/include/old-gumps.inc
- D | pkg/utils/gumps/include/old/old-gumps.inc
- D | pkg/utils/gumps/include/playerselectiongump.inc
- D | pkg/utils/gumps/include/requestgump.inc
- D | pkg/utils/gumps/include/selectiongump.inc
- D | pkg/utils/gumps/include/textConsts.inc
- D | pkg/utils/gumps/include/yesNoSizable.inc
- D | pkg/utils/gumps/include/yesno.inc
- D | pkg/utils/gumps/pkg.cfg
- D | pkg/utils/gumps/scripts/autoClose/autoClose.src
- D | pkg/utils/gumps/scripts/autoClose/autoCloseOnLeaveArea.src
- D | pkg/utils/gumps/scripts/autoClose/autoCloseOnMovedCoordinateDistance.src
- D | pkg/utils/gumps/scripts/autoClose/autoCloseOnMovedDistance.src
- D | pkg/utils/gumps/scripts/yesNo/yesNoGump.src
- D | pkg/utils/gumps/scripts/yesNo/yesNoMiniGump.src
- M | pkg/utils/mdgumps/commands/test/gfchart.src
- M | pkg/utils/mdgumps/include/gumpCaching.inc
- D | pkg/utils/mdgumps/include/old-gumps.inc
- D | pkg/utils/mdgumps/include/old/old-gumps.inc
- M | pkg/utils/mdgumps/include/yesNo.inc
- M | pkg/utils/timeutils/commands/test/timetest.src
- D | pythonscripts/__pycache__/fix_house_sign_objtypes.cpython-313.pyc
- M | regions/regions.cfg
- M | scripts/ai/tamed.src
- M | scripts/ai/townguard.src
- M | scripts/ai/warrior.src
- M | scripts/control/can_insert_container.src
- M | scripts/include/NameChecker.inc
- M | scripts/include/canstack.inc
- M | scripts/include/classes.inc
- M | scripts/include/creature_spellcast.inc
- M | scripts/include/damages.inc
- M | scripts/include/dotempmods.inc
- M | scripts/include/objtype.inc
- A | scripts/include/realmcolors.inc
- M | scripts/include/spelldata.inc
- M | scripts/include/starteqp.inc
- M | scripts/include/virtue.inc
- M | scripts/items/bladed.src
- M | scripts/items/moongate.src
- M | scripts/items/pvp.src
- M | scripts/items/pvp2vs2.src
- R100 | scripts/items/rune.src -> pkg/std/runebook/rune.src
- M | scripts/misc/chrdeath.src
- M | scripts/misc/logon.src
- M | scripts/misc/namechanger.src
- M | scripts/textcmd/admin/admin.src
- M | scripts/textcmd/admin/class.src
- M | scripts/textcmd/admin/colorrect.src
- M | scripts/textcmd/admin/deathgate.src
- M | scripts/textcmd/admin/dyerect.src
- M | scripts/textcmd/admin/gcmds.src
- M | scripts/textcmd/admin/ip.src
- M | scripts/textcmd/admin/maxcaps.src
- M | scripts/textcmd/admin/removechristmas.src
- M | scripts/textcmd/admin/setclass.src
- M | scripts/textcmd/coun/go.src
- M | scripts/textcmd/coun/goto.src
- M | scripts/textcmd/coun/jail.src
- D | scripts/textcmd/coun/makegate.src
- M | scripts/textcmd/coun/notes.src
- M | scripts/textcmd/coun/privs.src
- M | scripts/textcmd/coun/releaseinfo.src
- M | scripts/textcmd/coun/visit.src
- M | scripts/textcmd/gm/newiteminfo.src
- M | scripts/textcmd/gm/raiserect.src
- M | scripts/textcmd/gm/silence.src
- M | scripts/textcmd/player/arm.src
- M | scripts/textcmd/player/cast.src
- M | scripts/textcmd/player/clearmsglog.src
- M | scripts/textcmd/player/commands.src
- M | scripts/textcmd/player/dropskills.src
- M | scripts/textcmd/player/hairshop.src
- M | scripts/textcmd/player/online.src
- M | scripts/textcmd/player/password.src
- M | scripts/textcmd/player/prots.src
- M | scripts/textcmd/player/showclasse.src
- M | scripts/textcmd/seer/info.src
- M | scripts/textcmd/seer/makegate.src
- M | scripts/textcmd/seer/mark.src
- M | scripts/textcmd/seer/npclist.src
- M | scripts/textcmd/test/checksys.src
- M | scripts/textcmd/test/householdcap.src
- M | scripts/textcmd/test/householdmanager.src
- M | scripts/textcmd/test/makestaff.src
- M | scripts/textcmd/test/skillstest.src

---

## Detailed Changes By Theme

### 1. Crafting gump framework unification + Crafter skill exemption

**Files involved:** new `pkg/systems/crafting/include/craftmenu.inc` (1516 lines, extracted from Bowcraft's existing gump); `pkg/std/bowcraft/{bowcraft.cfg,bowcraft.src}`, `pkg/std/blacksmithy/{blacksmithy.cfg,make_blacksmith_items.src}`, `pkg/std/carpentry/{carpentry.cfg,carpentry.src}`, `pkg/std/tailoring/{tailoring.cfg,make_cloth_items.src,scissors.src}`, `pkg/std/tinkering/{tinker.cfg,itemdesc.cfg,tinkering.src}`; `scripts/include/classes.inc`, `pkg/opt/summoning/checkclasse.src`, `scripts/textcmd/player/showclasse.src`, `pkg/opt/zuluitems/Testclassbooststone.src`; `config/mrcspawn.cfg`; new dev tool `pkg/opt/alryc/textcmd/test/missingequipment.src`.

**Notable functional changes:**
- Bowcraft, Tailoring, Blacksmithy, Carpentry, and Tinkering are now all built on one shared `craftmenu.inc` category/detail/material-picker gump, replacing five separate near-duplicate gump implementations — each skill's `.src` shrank substantially as the duplicated gump code was deleted. Each skill kept only its own craft-execution logic and skill-specific extras (Bowcraft: Repair Item option; others: Difficulty-based quality divider).
- The corresponding recipe `.cfg` files for all 5 skills were reformatted/expanded to the schema `craftmenu.inc` expects.
- `classes.inc`'s `IsFromThatClasse` had its third parameter renamed `alchemyallowed`→`musicianshipallowed` and its skip-condition rewritten as a single boolean OR. **Crafter-class characters now have both Magery and Musicianship excluded from their spec-dilution calculation** (previously only Magery was exempted). `HaveInvalidSkillEnchantmentForClasse` was updated to match, and `AssignClasse`/`checkclasse.src`/`showclasse.src`/`Testclassbooststone.src` were all updated for the new 3-flag signature.
- `mrcspawn.cfg` had ~10 legacy cloth-variant vendor stock items removed from the `Bolts` product group as part of the Tailoring recipe rework.
- New Developer-tier GM tool `missingequipment.src` drops sort chests per equipment layer for objtype auditing (used to generate `ainotes/missing-equipment-static-overlap-20260831.json`); not player-facing.

**Expected impact:** see patch notes — unified crafting UI across 5 skills, and Crafters no longer penalized for training Magery/Musicianship.

### 2. Tokuno world content: 30 new regions + travel-menu fixes

**Files involved:** `regions/regions.cfg`, `pkg/opt/areas/areas.cfg`, `config/golocs_by_id.cfg`.

**Notable functional changes:**
- Added 30 new named Area/Region sub-zones covering the Tokuno facet (Crane Marsh, Yomotsu Mines, Lightning Watch, Field of Echos, Homare Shrine, Bushido Dojo, Homare's Eye, Kitsune Woods East/West, Sho Toh, Revenant-Jima, PVP Island, Zen Maze, The Waste, Beetlescape, Winterspur, Fan Dancer Dojo, Isamu Gazebo, Valley of the Sleeping Dragon, Yamandon Point, Tsuki Forest, Shrine of Isamu, Lightest Dark, Hiryu Forest, Mount Sho, Citadel, Isamu Refuge, Lotus Lakes, Hotaka Plains, Mount Hakonu, Storm Point, Lake Kappa), each with its own coordinate range, unique MIDI track, and `EnterText`/`LeaveText` messages. Restored a previously-disabled `Region TokunoDungeons` block.
- `golocs_by_id.cfg` gained the same 30 Tokuno POIs as `.go` destinations, fixed two dungeon bounding boxes (`firedungeon` Y-max 2259→2559, `cavernsofdespair2` Y-min 1076→1048), and recategorized ~16 pre-existing entries that had defaulted to `Type None` into their correct World/City/Dungeon groupings (Sosaria, Ilshenar, Luna, Umbra, Malas, Zento, Tokuno, Royal City, Gargoyle Palace/Queen, Holy City, Ter Mur, etc.) — these had likely been hidden or misplaced in the categorized `.go` gump before this fix.

**Expected impact:** see patch notes.

### 3. World spawn cleanup

**Files involved:** `pkg/opt/areaspawner/config/areagroups.cfg`, `config/npcdesc.cfg`.

**Notable functional changes:**
- Removed 14 problematic entries from elemental-themed spawn groups 1500-1502: all 8 "shrine" spawns (waterlord/earth/air/fire/shadow/poison/holy/stygian), the 5 "elementalsummons" spawns, and 5 more evil-shrine spawns. Templates remain defined in `npcdesc.cfg` — only their inclusion in world spawn pools was removed.
- Added `MoveMode LS` (land+swim) to 5 aquatic NPC templates that previously had no explicit MoveMode (`jellyfish`, `kraken2`, `crab`, `seaserpent2`, `walrus2`), fixing incorrect land-only pathing.
- Removed `dolphin` (a water-only creature incorrectly present in land-animal spawn group 600) plus `packllama`/`packhorse` from group 600, and `seaserpent` from Lizardman group 1700; relocated `cavedrake`/`shadowdragon` from group 1700 into the more appropriate group 1701 ("Dragonkin_Warband") rather than deleting them.

**Expected impact:** see patch notes.

### 4. Runic Atlas, Guild Travel gates, and realm-colored runes

**Files involved:** new `pkg/std/runebook/{runicatlas.src,runebookactions.inc}`; `pkg/std/runebook/{runebook.src,runecaninsert.src,runeoninsert.src,customspells.inc,itemdesc.cfg}`; `scripts/items/rune.src` renamed to `pkg/std/runebook/rune.src`; new `scripts/include/realmcolors.inc`; `pkg/std/spells/{gate.src,magictrap.src,mark.src,recall.src}`, `scripts/textcmd/seer/mark.src`, `pkg/opt/earth/earthportal.src`, `pkg/opt/moongates/moongate/ynwalkOnmoon.src`, `pkg/opt/vanityshop/runebookdye.src`, `pkg/packethooks/megacliloc/itemdata.src`, `scripts/include/objtype.inc`; new dev tool `pkg/opt/alryc/textcmd/test/testrunebook.src`; `pkg/std/bulkorders/rewards.cfg`; incidental carpentry-deed migration: `pkg/std/carpentry/{itemdesc.cfg,carpentrydeed.src}`, new `pkg/items/deed/built/{carpentryFlourMill,carpentryOven}.cfg`, `pkg/items/deed/deed/use.src`.

**Notable functional changes:**
- New item **Runic Atlas** (`0x9C16`/`0x9C17`, EW/NS graphics), ported from POLMD's `runicAtlas` package and wired into this repo's own runebook scripts. 48-rune capacity and 100 max recall charges, vs. a standard Runebook's 16 runes/5 charges. Vendor 15,000gp sell / 4,500gp buy (3x standard Runebook). New selection-style OSI gump (`runicatlas.src`, gump art 39923): click a rune name, then act on it (Recall, Gate, Rename, Set Default, Drop, Move Up/Down, and a new Guild Travel option).
- Shared rune-action logic (rename, recall with/without charges, gate, drop, swap) extracted into new `runebookactions.inc`, used by both the refactored standard `runebook.src` and the new `runicatlas.src`.
- `runecaninsert.src` generalized the hardcoded 16-rune cap to a per-item `MaxRunes` property (default 16, Atlas 48) and the charge cap to check `MaxCharges` — required for the Atlas's advertised capacity to actually function.
- New **Guild Travel** gate type: `customspells.inc`'s `CustomGate()` gained a `guild_only` parameter; the resulting gate is tagged `GuildOnly=<guildid>` and colored with the caster's guild color, and `ynwalkOnmoon.src` now blocks non-guildmates from stepping through with a system message.
- New **realm-colored runes**: `scripts/include/realmcolors.inc` maps each realm (Britannia, Britannia_alt, Ilshenar, Malas, Tokuno, Ter Mur, default) to a hue. Marked recall runes (player Mark spell and the Seer `.mark` command) are now colored per-realm on creation, and the Runic Atlas's rune list uses the same mapping.
- Recall/Gate/Earth Portal spells, Magic Trap, and the Runic Dye Tub now recognize the new Runic Atlas objtypes alongside standard runebooks/runes. `itemdata.src`'s megacliloc tooltip hook now shows "Max Charges:" on Runic Atlas items too.
- Loose recall-rune item definitions and `rune.src` moved out of the base `config/itemdesc.cfg`/`scripts/items/` into the `pkg/std/runebook` package (organizational cleanup, no functional change).
- New Bulk Order Deed reward: "Runic Atlas", 900 BOD points (≈25 Omega Tokens), grants `0x9C16`.
- Incidental bundled cleanup: several legacy carpentry furniture deeds (Bench x2, Flour Mill, Oven, Altar, Loom, Spinning Wheel) migrated off the old `carpentrydeed`/`ObjList`/`ObjXMod`/`ObjYMod` placement system onto the modern `:deed:deed/use` + `BuildCFG` deed system.
- New dev-only QA tool `testrunebook.src` fills a targeted runebook/atlas with random valid marked runes for gump testing.

**Expected impact:** see patch notes.

### 5. Static housing: dispatched-method realm-argument fix

**Files involved:** `pkg/multis/staticHousing/lockunlock.src`, `pkg/multis/staticHousing/commands/player/decorate.src`.

**Notable functional changes:** `IsLocationInsideHouse()` is called as a dispatched method off the house sign object, and dispatched method calls don't honor a function's declared default parameter values (the same class of bug already fixed once in `guilds.inc`'s `IsInsideGuildHouse`). Both call sites were passing only 3 of the method's arguments (x, y, z) and silently relying on a default for the realm argument that never actually applied at runtime, which could cause `IsLocationInsideHouse` to evaluate against the wrong realm. Fixed by passing all 4 positional args explicitly (`item.x, item.y, item.z, sign.realm`) at both call sites.

**Expected impact:** see patch notes.

### 6. Code review pass — CRITICAL fixes (3)

A full code-review sweep of everything on this branch surfaced 3 critical, ship-blocking bugs, all fixed:

- **`pkg/multis/staticHousing/logon.src`** — `GetHouseSign(who)` returns plain `0` for the ~100% of logins where the player isn't standing inside a static house's footage, and the very next line called `.IsBanned(who)` on it with no guard, erroring on essentially every login. Fixed by adding an `hsign &&` guard.
- **`pkg/systems/combat/dualplanaronhit.src`** — `ApplyTheDamage()` was gated behind a proc-chance roll, so most hits silently skipped damage entirely, unlike every sibling on-hit script. Restructured so damage always applies unconditionally; only the paralyze/astral-storm special effect (and its `-15` damage-floor clamp) stays gated on the roll and the Windsbreath re-entrancy guard.
- **`pkg/multis/customhousing/include/house.inc`** — `LockdownItem`/`SecureItem`/`UnlockItem`/`UnsecureItem` were non-functional after a referenced script was deleted mid-refactor, leaving customhousing with no lockdown/secure/release path at all. Rebuilt tied into the same shared `:house:` House Management flow `pkg/multis/house` already uses: `HasHousePermission()` permission checks, `IsInside()` validation, the shared `:house:secureCont` access-gate script, lazily-seeded `RemainingLockdowns`/`RemainingSecures` capacity counters, and a new House Management sign-gump button. `RaiseItem`/`LowerItem`/`DisplayContainer` were deliberately not rebuilt — `pkg/multis/house`'s own current reference gump doesn't expose these either.

### 7. Code review pass — HIGH-severity fixes (23 fixed, 1 confirmed not-a-bug, 2 retracted false positives, 1 left for maintainer)

- **Recurring "dropped `realm` argument" bug** — a diff-based sweep against the merge-base found 30 call sites across 12 files, all fixed: `pkg/opt/alchemyplus/newpotions.src` (3), `pkg/opt/champspawns/include/{rewards,skulls,spawning}.inc` (16 combined), `pkg/std/detecthidden/detecthidden.src` (2), `pkg/std/peacemaking/peacemaking.src` (1), `pkg/std/spells/{archprot,reveal}.src` (1 each), `pkg/systems/combat/{trielementalonhit,trielementalscript}.src` (1 each), `pkg/opt/omegacache/placecache.src` (2 — a new file that never used `where.realm`, not a regression), `scripts/include/dotempmods.inc` (2 — root cause was a dropped `include "include/constants/locations"`, restored).
- `pkg/multis/staticHousing/sign/control.src` — removed a stray module-scope duplicate statement; fixed realm/flags argument order at 3 `MoveObjectToLocation` call sites (also `sign/destroy.src`, `sign/use.src`); removed a duplicated "credit one secure slot back" block in `unsecure()`.
- `pkg/opt/rituals/include/rituals.inc` — new shared `RitualIsAlreadyMaxed()` cap-check, called before any mana/circle cost is spent, covering all 8 tiered rituals.
- `scripts/ai/tamed.src` — `Guard` command no longer force-overrides `following`; also fixed `Guard()` unconditionally re-arming the same priority bug every tick it ran.
- `scripts/control/can_insert_container.src` — the `#SecureRemove` grace-period bypass now also requires `IsFriend(who, sign, 0)`, restoring a dropped requirement.
- `pkg/std/tailoring/itemdesc.cfg` — **retracted, false positive.** Banner/Pillow objtypes exist in the newer POLMD-ported item packages; nothing to fix.
- `scripts/include/starteqp.inc` — magiclevel-11 threshold `power<30000`→`power<60000`, restoring the documented 20/40/40% tier split (was 20/10/70%).
- `pkg/multis/house/multiSign/use.src` (`ChangeHouseOwner`) — now resolves the real previous owner from the sign's own `ownerserial` instead of trusting the caller argument.
- `pkg/multis/boat/multi/listener.src` (`DryDock`) — added a `GetBoatLockId(boat)` fallback, fixing Row Boats.
- `pkg/opt/powerhour/textcmd/player/setph.src` — added a `default` case branch fixing a permanent lockout; added a durable `pph_active_type` property so reboot-resume actually works.
- `pkg/std/alchemy/{bluepotion,whitepotion}.src` — added a missing intermediate rank tier.
- `pkg/opt/townstones` — all realm-search helpers now loop every realm the shard defines instead of hardcoding `britannia`.
- `scripts/include/classes.inc` / `pkg/opt/summoning/checkclasse.src` — **confirmed intentional, not a bug.** Crafter's Magery+Musicianship exemption and Thief's lack of a coskill exemption are the intended design; Ranger's Forensics exemption verified intact everywhere it's checked.
- `scripts/include/spelldata.inc` (`SmartAoE`) — restored the per-victim `IsInSafeArea()` check dropped in an earlier commit (`89f9092`).
- `pkg/systems/combat/include/hitscriptinc.inc` (`CalcPhysicalDamage`) — Mage-attacker damage reduction moved out of the defender-class `elseif` chain into its own independent check, restoring the penalty vs. Paladin/Warrior/Bladesinger defenders.
- `pkg/std/cartography/cartography.src` — restored a dropped `ReserveItem(blank)` check.
- `pkg/std/itemid/itemid.src` — `#LastID` cooldown now set unconditionally, matching the pre-patch baseline.
- `pkg/std/traps/commands/seer/cleartraps.src` — replaced an index-desyncing removal loop with a single `RemoveTrap(object, "all")` call.
- `pkg/items/lighting/config/itemdesc.cfg` — fixed 2 items' `ChangeTo` pointing at the wrong graphic.
- `pkg/items/origami/kit/use.src` — added a post-indexing cancel check.
- `pkg/items/commcrystals/{commCrystal,crystalControl}.src` — fixed a "BLANK" seed-value append bug plus a defensive skip/null-check for already-affected crystals.
- `scripts/include/damages.inc` — the PvP-slowdown check now also applies when the attacker is an NPC pet controlled by a non-NPC master.
- `scripts/include/teleporters.inc:1217` — "Black City of the Damned → Tartarus" has identical source/destination coordinates (a no-op teleporter). **Left for the maintainer to fix directly.**
- `pkg/opt/questpkg/include/queststate.inc` — two readers now `continue` past a falsy quest-state entry before indexing into it.
- `pkg/opt/powerscrolls/itemdesc.cfg` — **retracted, false positive**, verified against engine source (`pol-core/plib/pkg.cpp`): `::maindestroy` is documented core-file syntax that correctly resolves for every caller.

### 8. Code review pass — MEDIUM-severity fixes (14 fixed, 1 left as-is, 1 duplicate removed without replacement)

- `scripts/include/spelldata.inc` (`SmartAoE`) — fixed a duplicate-append edge case where two independent exclusion checks could both fire for the same victim.
- `pkg/items/sysbook/include/sysBook.inc` / `pkg/systems/accounts/include/accounts.inc` — removed a premature `UnloadDataFile()` from two getters; unload now happens at each caller's true last-use point.
- `pkg/items/doors/config/itemdesc.cfg` — fixed 3 broken `OpenGraphic` values and closed a broken graphic-swap loop.
- `pkg/items/musicStand/config/icp.cfg`, `pkg/items/kingsCollection/config/itemdesc.cfg` — corrected a mislabeled name and description, and 2 items missing their `Sound`.
- `scripts/textcmd/test/householdmanager.src` — footer buttons now render on always-visible page 0 instead of breaking once the household list exceeds one page; also fixed a `datafile` handle left open after the gump closed.
- `scripts/textcmd/test/householdcap.src` — `SendTextEntryGump`'s positional args were wrong, so its 4-character input cap was silently never applied; fixed, plus added missing cancel/error validation.
- `pkg/opt/champspawns/config/spawns.cfg` — a duplicate `level3 darkknight` line was **removed but not replaced**; this Spawn 4 group is now one entry short of a real template.
- `pkg/opt/champspawns/include/death.inc` — restored a dropped `Sleep(2); DestroyItem(corpse);`.
- `pkg/opt/vanityshop/vanityshop.src` — bundle purchases now unpack contents directly into the backpack instead of moving the temp bag itself in.
- `pkg/systems/combat/include/hitscriptinc.inc` (`GetSlayMultiplier`) — restored "Human" as index 16 of the `WhoKnows` random table.
- `scripts/ai/soulwhisperer.src` — **left as-is.** Confirmed dead-weight branch (no third loadout function exists to differentiate it from the fallback), not fixed per maintainer direction.
- `pkg/items/keys/include/key.inc` — `CP_CREATE`→`CP_NOCREATE`, matching the function's own default and its sibling's convention.
- `config/npcdesc.cfg` — removed an older duplicate of 6 `NpcTemplate` blocks, kept the newer copy.
- `out.txt` (repo root), `pythonscripts/__pycache__/*.pyc` — removed stray committed files; added to `.gitignore`.

### 9. Performance and duplication cleanup

- `scripts/include/canstack.inc` (`CanStack`) — was rebuilding its ignore-list dictionary from `stacking.cfg` on every single call (every stack-merge/drag-drop shard-wide). Now lazy-loads once into a module-level dictionary via a new `CanStack_GetIgnoreList()` helper.
- `pkg/opt/areas/textcmd/admin/areas.src` (`SavePolicyArraysForRealm`) — was doing up to 3 datafile open/close cycles *per area* on Commit. Now builds all entries into an in-memory dictionary and writes once via the existing bulk `SaveRealmPolicies()` helper.
- `pkg/std/cooking/cooking.src` — the fire-or-oven check ran up to 10 separate location scans per cooking attempt. Now does one shared scan (`GetNearbyCookObjtypes`) and checks the collected objtypes against each fire/oven range in memory.
- `pkg/std/bowcraft/bowcraft.src` / `pkg/std/tinkering/tinkering.src` — Bowcraft's exceptional-quality-multiplier math was a duplicated inline copy of Tinkering's that had silently drifted to a 50%-base multiplier instead of the correct 100%-base (confirmed a bug, not intentional archery balance). Both now share a new `CalcExceptionalQualityMultiplier()` in `pkg/systems/crafting/include/craftingfunctions.inc`.
- `pkg/opt/townstones` — the same ~20-function datafile-sync helper family (`OpenTownstoneDataFile`, `TouchTownstoneRegionEntry`, `SyncTownstoneState`, `GetTownBankGold`/`SetTownBankGold`, gump utility helpers, player-town config readers, etc.) was duplicated near-verbatim across 9-10 files. Consolidated into `tstone.inc`; the two functions that had genuinely diverged across copies (`TouchTownstoneRegionEntry`, `ParseEnabled`) were reconciled to their most complete/permissive variant, resolving a real, previously-tracked behavioral inconsistency in `ParseEnabled` as a side effect. `PLAYERTOWNS_CFG` was also moved into `tstone.inc` alongside `LoadPlayerTownsConfig`, since every includer now compiles that function and needs the constant it references in scope. Every affected `.src` file's `use`/`include` list was updated accordingly (several needed a newly-added `use cfgfile;`/`use basic;` they hadn't required before).
- `pkg/systems/accounts/include/accounts.inc` — the login-policy engine does multiple full account-table scans per login (`ACCT_CountOnlineByDiscordAndIP`, `ACCT_CountPendingPolicyByDiscord`/`ByIP`, `ACCT_HouseholdContainsDiscord` once per shared-IP household member), serialized behind a global spin-lock, and pays the whole cost twice (auth screen + world entry). **Reviewed and left as-is** — confirmed correct, just not fast; not worth the risk of touching the concurrency-guard logic right now.
- `scripts/include/starteqp.inc` (`MakeLoot`) / `pkg/opt/alchemyplus/newpotions.src` (`FindItemType`) — both still call `UnloadConfigFile()` immediately before `ReadConfigFile()` on hot paths (every corpse's loot roll, every potion drink), forcing a disk re-parse instead of using the config cache. **Not addressed this patch.**

### 10. Gump framework migration: `pkg/utils/gumps` deleted, `pkg/utils/mdgumps` is now the sole framework

**Files involved:** the entire `pkg/utils/gumps/` package deleted (39 files, ~9,245 lines — `pkg.cfg`, all `commands/admin/*.src`, all `config/*.cfg`, all `include/*.inc` including duplicated `old-gumps.inc` copies, both `scripts/autoClose/*.src` and `scripts/yesNo/*.src`); orphaned duplicate `old-gumps.inc` copies also removed from `pkg/utils/mdgumps/` and `pkg/multis/staticHousing/`; `pkg/utils/mdgumps/{commands/test/gfchart.src,include/gumpCaching.inc,include/yesNo.inc}` updated; and roughly 150 other scripts across nearly every package (`scripts/ai`, `scripts/items`, `scripts/misc`, `scripts/textcmd/*`, most of `pkg/opt/*`, `pkg/std/*`, `pkg/multis/*`, `pkg/systems/*`) had their `include ":gumps:..."` lines repointed to `:mdgumps:`.

**Notable functional changes:**
- Confirmed via repo-wide grep that no script still references `:gumps:` after this change — `pkg/utils/mdgumps` is the sole surviving gump framework (`pkg.cfg` confirms `Name mdgumps`, `Version 2.0`).
- A meaningful subset of `scripts/textcmd/*` and several `pkg/opt/*` files (e.g. `pkg/opt/shilhook/textcmd/admin/{setglobalmultipliers,setplayermultipliers}.src`) also had their gump-construction code ported from the old low-level API (`GFInitGump()`, bare `GFResizePic(x,y,...)`, `GFButtonID()`, reading `data[btn.keyid]`) to the new API (`GFCreateGump(x,y,w,h)`, every draw call taking the gump handle as its first argument, `GFAddButton(gump, ..., GF_CLOSE_BTN|GF_PAGE_BTN, ...)`, reading `data[btn]`) — same layout, graphics, and buttons; purely a backend rewrite with zero player-visible difference.
- `pkg/opt/rituals/altar/gump.inc`'s local `RitualAltar_ReagentName()` name-lookup helper comment updated to reflect why it avoids pulling in `:omegacache:` (which now drags in `:mdgumps:gumps`/`gumps_ex`, already in scope there — a second include would be a duplicate-function compile error).
- Two files (`pkg/opt/lootlottery/commands/GM/cfglotto.src`, `pkg/utils/timeutils/commands/test/timetest.src`) picked up a `// Synopsis:` header comment as a drive-by for the command-synopsis doc generator; `pkg/utils/datafile/pkg.cfg` had a dead commented-out `//Requires gumps 1.2` line removed.

**Command-tier side effect:** `scripts/textcmd/coun/makegate.src` — a near-duplicate of `scripts/textcmd/seer/makegate.src` gated behind a temporary `.makegate` grant — was deleted as part of this cleanup, and `scripts/textcmd/admin/gcmds.src` was updated in lockstep to remove `.makegate` from `counsCommands`/`grantCommands()`. Counselors permanently lose access to `.makegate` (previously obtainable only via a temporary admin grant); it now exists only at the Seer tier. Staff-only, no player impact.

**Expected impact:** none for players — internal UI-framework consolidation.

---

## Validation Notes

- Diff range: `git diff 83f1748..e3d88b0` / `git log --oneline 83f1748..e3d88b0`.
- File inventory and per-file diffs verified directly against `git diff`/`git show` output for every theme above; the CRITICAL/HIGH/MEDIUM/performance items were verified against live code during the code-review-and-fix session itself (structural balance re-checked after every edit: matching `if`/`endif`, `function`/`endfunction`, `for`/`endfor`, `foreach`/`endforeach` counts).
- Two originally-flagged HIGH findings were retracted as false positives after deeper verification (tailoring itemdesc objtypes existed in a different reviewed chunk; `::maindestroy` syntax verified correct against actual engine source) rather than left in the report incorrectly.
- Working tree was clean at the time of this writing (everything in this range is committed as of `e3d88b0`).
