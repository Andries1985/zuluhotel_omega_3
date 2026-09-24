# Developer Changelog - v3.1.2

Range: Patch-3.1.1..Patch-3.1.2 (commit `9aa5216`..`cdd3ef2`)
Branch: Patch-3.1.2
Date: 2026-09-24

---

## Scope Summary

- Total files changed: 299 (9 added, 281 modified, 8 deleted, 1 renamed)
- Net textual delta: 21,832 insertions, 3,363 deletions. Of that, 16,887 insertions are the two review record files under `ainotes/` (the fix log and its regenerated diff) and the 3.1.1 release notes landing in this range; the game content itself is 287 files, +4,945 / -2,898. Six rebuilt binaries (`pol.exe`, `poltool.exe`, `uoconvert.exe`, `uotool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`) are counted as files but not as lines.
- Largest shifts:
  - `ainotes/code-review-fixlog-20260921.diff` (+13,646, new) and `ainotes/code-review-fixlog-20260921.md` (+1,355, new) — the line-level record of the whole-codebase review (18 sections, 172 numbered entries); not game content
  - `patchnotes/developer-changelog-v3.1.1.md` (+856, new), `patchnotes/patch-v3.1.1.md` (+159, new), `patchnotes/developer-changelog.md` (+751/-428), `patchnotes/launchernotes.md` (+120/-37) — the 3.1.1 release notes, committed after the 3.1.1 range closed
  - `pkg/std/tailoring/tailoringfunctions.inc` (+652, new) and `pkg/std/tailoring/make_cloth_items.src` (-544) — the tailoring logic moved into a shared include for the Autoloom station; `autoloom_use.src` (+56) and `autoloom_watchdog.src` (+42) are new
  - `pkg/multis/house/multiSign/use.src` (+203/-146), `pkg/multis/customhousing/sign.src` (+138/-88), `pkg/multis/customhousing/include/house.inc` (+90/-2), `scripts/include/housing.inc` (+85) — housing security and demolish rework
  - `pkg/opt/omegacache/omegacache.inc` (+124/-37) — deposit/withdraw made destroy-first and lease-aware
  - `pkg/std/bulkorders/bulkorder_matching.inc` (+114/-14) — bulk order payout and stack handling
  - `pkg/std/bowcraft/bowcraft.src` (+93/-20), `pkg/std/carpentry/carpentry.src` (+79/-37), `pkg/std/blacksmithy/blacksmithgump.inc` (+52/-18) — consume-first crafting
  - `pkg/packethooks/megacliloc/itemdata.src` (+86/-45) — weapon tooltip damage and DPS aligned with the live combat formula
  - `pkg/systems/accounts/hook/onLogin.src` (+81/-52) — login lockout rewritten
  - `pkg/systems/combat/include/hitscriptinc.inc` (+77/-28), `pkg/opt/powerscrolls/createpowerscroll.src` (+75/-1), `pkg/opt/questpkg/include/queststate.inc` (+73/-29), `pkg/opt/alchemyplus/alchemyplus.src` (+69/-16), `pkg/opt/guilds/commands/player/guilds.src` (+68/-65), `scripts/include/spelldata.inc` (+66/-2), `scripts/include/spellgrants.inc` (+62, new)
- Non-merge commits in range (oldest to newest):
  - `12a9981` Patch notes.. capper crash (the 3.1.1 release notes plus `pkg/opt/capper/capper.src`)
  - `e6a8b79` Add latest nightly (engine binaries, `core-changes.txt`, `pol.cfg.example`, `scripts/ecompile.cfg.example`)
  - `e632446` PH Fixes (`pkg/opt/powerhour/textcmd/player/{ph,setph}.src`)
  - `cdd3ef2` Fable 5.1 fixes (everything else: the review fixes, the Autoloom rework, the alchemyplus Tamla work, `pol.cfg`, the review records, the docs)
- Merge commits: `cd19f9d` (PR #107, brings `12a9981`), `a453686` (PR #108) and `48ee9c8` (PR #109) (both bring `e6a8b79`) carry nothing beyond those commits. `4f8e46f`, `629d485`, `4705aea` (PR #104-106) appear in the log because they are on the main line, but their content was already in the `9aa5216` tree; a tree diff `9aa5216..cdd3ef2` reconciles against the four non-merge commits above.
- Almost everything in `cdd3ef2` comes from the whole-codebase review run between 2026-09-20 and 2026-09-23. The review read every file under `pkg/systems`, `pkg/std`, `pkg/multis`, `pkg/packethooks`, `pkg/opt` and `scripts/` and applied 172 numbered fixes chosen item by item; `ainotes/code-review-fixlog-20260921.md` is the line-level companion to the themes below (each theme names the fix-log sections it covers), and every edit site in the code carries a dated `// 2026-09-2x:` comment. The themes are by subsystem, not by commit.

**Two things a reviewer of this release should know first:**
- `pol.cfg` is in this range with eight debug options switched from 0 to 1: `WatchRPM`, `WatchSysLoad`, `LogSysLoad`, `ReportRunToCompletionScripts`, `ReportCriticalScripts`, `ShowRealmInfo`, `ProfileCProps`, `EnforceMountObjtype`. These were the local diagnostics kept on during the review and were meant to stay out of the commit; they add console/log volume and, for `ProfileCProps`, a per-property-access cost. Turn them back off before this build goes live unless they are wanted.
- Nothing in `cdd3ef2` was compiled or tested in game by the reviewer; `ecompile` on the whole tree and a smoke pass over the "Expected impact" lines below are still owed.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File` (A=added, M=modified, D=deleted, R=renamed as `old -> new`)

- M | .claude/skills/escript-gotchas/SKILL.md
- M | .claude/subagent-briefing.md
- A | ainotes/code-review-fixlog-20260921.diff
- A | ainotes/code-review-fixlog-20260921.md
- M | config/command_synopses.cfg
- M | config/itemdesc.cfg
- M | core-changes.txt
- A | patchnotes/developer-changelog-v3.1.1.md
- M | patchnotes/developer-changelog.md
- M | patchnotes/launchernotes.md
- A | patchnotes/patch-v3.1.1.md
- M | pkg/items/containers/container/canInsert.src
- M | pkg/items/containers/container/canRemove.src
- M | pkg/items/containers/container/use.src
- M | pkg/items/deed/built/autoloom.cfg
- M | pkg/multis/boat/multi/listener.src
- M | pkg/multis/boat/tiller/canInsert.src
- M | pkg/multis/boat/tiller/methods.src
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/scripts/customhousedeed.src
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/customhousing/syshook/closecustomhouse.src
- M | pkg/multis/house/multiSign/control.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/staticHousing/config/settings.cfg
- M | pkg/multis/staticHousing/logon.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/multis/staticHousing/sign/destroy.src
- M | pkg/multis/staticHousing/sign/use.src
- M | pkg/opt/alchemyplus/alchemyplus.src
- R | pkg/opt/alryc/textcmd/player/mounttest.src -> pkg/opt/alryc/textcmd/test/mounttest.src
- M | pkg/opt/areas/callguards.src
- M | pkg/opt/areas/include/areapolicy.inc
- M | pkg/opt/areas/textcmd/admin/areas.src
- M | pkg/opt/ArtifactSystem/artifactbox.src
- M | pkg/opt/ArtifactSystem/itemdesc.cfg
- M | pkg/opt/capper/capper.src
- M | pkg/opt/champspawns/include/rewards.inc
- M | pkg/opt/champspawns/scripts/control.src
- M | pkg/opt/christmas/Christmasgifts.src
- M | pkg/opt/crafterboost/make_crafter_boosts.src
- M | pkg/opt/crafterboost/refinement_gump.src
- M | pkg/opt/decoratefacets/commands/test/udestroymany.src
- M | pkg/opt/Donator/donatorbearstone.src
- M | pkg/opt/Donator/donatorhorsestone.src
- M | pkg/opt/Donator/donatorllamastone.src
- M | pkg/opt/Donator/donatorostardstone.src
- A | pkg/opt/Donator/include/playertown.inc
- M | pkg/opt/earth/antidote.src
- M | pkg/opt/earth/bookofearth.src
- M | pkg/opt/earth/druidscroll.src
- M | pkg/opt/earth/earthportal.src
- D | pkg/opt/earth/shapechange.cfg
- M | pkg/opt/Events/textcmd/seer/createEventBag.src
- M | pkg/opt/guilds/commands/player/guilds.src
- M | pkg/opt/guilds/commands/test/changeguildownership.src
- M | pkg/opt/guilds/include/guilds.inc
- M | pkg/opt/guilds/ondelete.src
- M | pkg/opt/holybook/angelicfeast.src
- M | pkg/opt/holybook/angelicgate.src
- M | pkg/opt/holybook/holybook.src
- M | pkg/opt/holybook/holyscroll.src
- M | pkg/opt/holybook/revive.src
- M | pkg/opt/holybook/sanctuary.src
- M | pkg/opt/holybook/wrathofgod.src
- M | pkg/opt/loot/antiloot.inc
- M | pkg/opt/lootlottery/commands/GM/cfglotto.src
- M | pkg/opt/MagicWands/magicwands.src
- M | pkg/opt/moongates/itemdesc.cfg
- M | pkg/opt/msg/commands/player/msg.src
- M | pkg/opt/necro/codexdamnorum.src
- M | pkg/opt/necro/necroscroll.src
- D | pkg/opt/necro/sunderingsword.src
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/powerhour/textcmd/player/ph.src
- M | pkg/opt/powerhour/textcmd/player/setph.src
- M | pkg/opt/powerhour/textcmd/test/resetph.src
- M | pkg/opt/powerscrolls/createpowerscroll.src
- M | pkg/opt/powerscrolls/randomTome.src
- M | pkg/opt/questpkg/include/questdeath.inc
- M | pkg/opt/questpkg/include/questfishing.inc
- M | pkg/opt/questpkg/include/questjournal.inc
- M | pkg/opt/questpkg/include/questnpcgump.inc
- M | pkg/opt/questpkg/include/queststate.inc
- M | pkg/opt/rituals/include/rituals.inc
- M | pkg/opt/rituals/rituals/demonstration.src
- M | pkg/opt/roleplaying/macrotimer.src
- M | pkg/opt/roleplaying/rperstone.src
- M | pkg/opt/roleplaying/textcmd/admin/fixstartgear.src
- M | pkg/opt/roleplaying/textcmd/coun/macrotest.src
- M | pkg/opt/songbook/songbook.src
- M | pkg/opt/songbook/songofbeckon.src
- M | pkg/opt/songbook/songofcloaking.src
- M | pkg/opt/songbook/songofsalvation.src
- M | pkg/opt/songbook/songofsirens.src
- M | pkg/opt/songbook/songscroll.src
- M | pkg/opt/spawnpoint/checkpoint.src
- M | pkg/opt/spawnpoint/include/customnpc.inc
- M | pkg/opt/spawnpoint/spawnpointmanager.src
- M | pkg/opt/spawnpoint/spawntriggerwalkon.src
- M | pkg/opt/spawnpoint/textcmd/admin/despawn.src
- M | pkg/opt/spawnpoint/textcmd/admin/forcespawn.src
- M | pkg/opt/spawnpoint/textcmd/admin/forcespawnarea.src
- M | pkg/opt/spawnpoint/textcmd/admin/gotospawnpoint.src
- M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src
- M | pkg/opt/spawnpoint/textcmd/admin/primespawn.src
- M | pkg/opt/Staff/RecordXYZ.src
- M | pkg/opt/summoning/npcsummoning.src
- M | pkg/opt/summoning/processpoisonmod.src
- M | pkg/opt/summoning/summoning.src
- M | pkg/opt/townstones/electionwatch.src
- M | pkg/opt/townstones/tstone.inc
- M | pkg/opt/townstones/tstone.src
- M | pkg/opt/vanityshop/customitemdye.src
- M | pkg/opt/vanityshop/customitemname.src
- D | pkg/opt/vanityshop/include/mountFunctions.src
- M | pkg/opt/vanityshop/runebookdye.src
- M | pkg/opt/vanityshop/vanityshop.src
- M | pkg/opt/versebook/Bardic_Boulders.src
- M | pkg/opt/versebook/Beastal_Bond.src
- M | pkg/opt/versebook/Corpse_Distention.src
- M | pkg/opt/versebook/Dragon_Skin.src
- M | pkg/opt/versebook/include/versefunctions.inc
- M | pkg/opt/versebook/Lesser_Healing.src
- M | pkg/opt/versebook/Life_Balance.src
- M | pkg/opt/versebook/Not_Implemented.src
- M | pkg/opt/versebook/Shadows.src
- M | pkg/opt/versebook/Sonic_Disturbance.src
- M | pkg/opt/versebook/Spirit_Flock.src
- M | pkg/opt/versebook/versebook.src
- M | pkg/opt/warriorforhire/warrior.src
- M | pkg/opt/zuluitems/cannon.src
- M | pkg/opt/zuluitems/catapult.src
- M | pkg/opt/zuluitems/dragoneggs.src
- D | pkg/opt/zuluitems/dyecheck.src
- M | pkg/opt/zuluitems/ostardeggs.src
- M | pkg/packethooks/megacliloc/commands/player/updatetp.src
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/packethooks/packethook/packethook.src
- M | pkg/packethooks/packethook/uopacket.cfg
- M | pkg/packethooks/speech/receivespeechhook.src
- M | pkg/packethooks/versionHook/versionhook.src
- M | pkg/std/alchemy/alchemyfunctions.inc
- M | pkg/std/blacksmithy/blacksmithgump.inc
- M | pkg/std/bowcraft/bowcraft.src
- M | pkg/std/bulkorders/bulkorder_matching.inc
- M | pkg/std/bulkorders/bulkorderdeed.src
- M | pkg/std/bulkorders/bulkorderrewards.src
- M | pkg/std/camping/camping.src
- M | pkg/std/carpentry/carpentry.cfg
- M | pkg/std/carpentry/carpentry.src
- M | pkg/std/cartography/cartography.src
- M | pkg/std/cooking/cooking.src
- M | pkg/std/cooking/hunger.src
- M | pkg/std/cooking/hungerdamage.src
- M | pkg/std/dundee/globeofsosaria.src
- M | pkg/std/healing/healing.src
- M | pkg/std/help/help.src
- M | pkg/std/herding/herd.src
- M | pkg/std/herding/herdedai.src
- M | pkg/std/hiding/hiding.src
- M | pkg/std/inscription/inscription.src
- M | pkg/std/itemid/itemid.inc
- M | pkg/std/itemid/itemid.src
- M | pkg/std/lockpicking/use/picklock.src
- M | pkg/std/musicianship/musicianship.src
- M | pkg/std/removetrap/removetrap.src
- M | pkg/std/runebook/customspells.inc
- M | pkg/std/runebook/runebook.src
- M | pkg/std/runebook/runebookactions.inc
- M | pkg/std/runebook/runicatlas.src
- M | pkg/std/snooping/snooping.src
- M | pkg/std/spells/blade_spirit.src
- D | pkg/std/spells/bless timer.src
- M | pkg/std/spells/cure.src
- M | pkg/std/spells/dispel.src
- M | pkg/std/spells/dispel_field.src
- M | pkg/std/spells/gheal.src
- M | pkg/std/spells/heal.src
- M | pkg/std/spells/invisibility.src
- M | pkg/std/spells/mark.src
- M | pkg/std/spells/massdispel.src
- M | pkg/std/spells/mindblast.src
- M | pkg/std/spells/polymorph.src
- D | pkg/std/spells/protection with timer.src
- M | pkg/std/spells/reactivearmor.src
- M | pkg/std/spells/resurrect.src
- M | pkg/std/spells/teleport.src
- M | pkg/std/spells/unlock.src
- M | pkg/std/spells/vortex.src
- A | pkg/std/tailoring/autoloom_use.src
- A | pkg/std/tailoring/autoloom_watchdog.src
- M | pkg/std/tailoring/itemdesc.cfg
- M | pkg/std/tailoring/make_cloth_items.src
- A | pkg/std/tailoring/tailoringfunctions.inc
- M | pkg/std/tasteid/tasteid.src
- M | pkg/std/taunt/enticeai.src
- M | pkg/std/tinkering/tinkeringfunctions.inc
- M | pkg/std/tracking/tracking.src
- M | pkg/std/training/dummy_pickpocket.src
- M | pkg/std/traps/trapScripts/setTrap.src
- M | pkg/std/treasuremap/digtreasure.src
- M | pkg/std/veterinary/vet.src
- M | pkg/systems/accounts/acctWatcher/acctWatcher.src
- M | pkg/systems/accounts/config/settings.cfg
- M | pkg/systems/accounts/hook/onLogin.src
- M | pkg/systems/attributes/hooks/shilhook.src
- M | pkg/systems/combat/hooks/omegaattack.inc
- M | pkg/systems/combat/include/hitscriptinc.inc
- M | pkg/systems/combat/paralyzehit.src
- D | pkg/systems/combat/shilcombat.inc
- M | pkg/systems/combat/spellstrikescript.src
- M | pkg/systems/combat/thiefpoisonhit.src
- M | pkg/systems/crafting/include/multicraft.inc
- M | pkg/systems/email/pkg.cfg
- M | pkg/systems/playervendor/playermerchant.src
- M | pol.cfg
- M | pol.cfg.example
- M | pol.exe
- M | poltool.exe
- M | scripts/ai/chaosmultikillpcs.src
- M | scripts/ai/combat/doppelcombatevent.inc
- M | scripts/ai/gambler.src
- M | scripts/ai/highpriest.src
- M | scripts/ai/humuc.src
- M | scripts/ai/immobile.src
- M | scripts/ai/loke.src
- M | scripts/ai/main/chaoskillpcsloop.inc
- M | scripts/ai/main/vortexloopkill.inc
- M | scripts/ai/merchant.src
- M | scripts/ai/setup/modsetup.inc
- M | scripts/ai/soulwhisperer.src
- M | scripts/ai/sum.src
- M | scripts/ai/thor.src
- M | scripts/ai/townguard.src
- M | scripts/ai/water.src
- M | scripts/control/trashControl.src
- M | scripts/ecompile.cfg.example
- M | scripts/ecompile.exe
- M | scripts/include/attributes.inc
- M | scripts/include/bard.inc
- M | scripts/include/classes.inc
- M | scripts/include/client.inc
- M | scripts/include/damages.inc
- M | scripts/include/housing.inc
- M | scripts/include/jailcheck.inc
- M | scripts/include/mrcspawn.inc
- M | scripts/include/npccast.inc
- M | scripts/include/privs.inc
- M | scripts/include/resourcemanager.inc
- M | scripts/include/skillpoints.inc
- M | scripts/include/skilltitles.inc
- M | scripts/include/speech.inc
- M | scripts/include/spelldata.inc
- A | scripts/include/spellgrants.inc
- M | scripts/include/starteqp.inc
- M | scripts/include/teleporters.inc
- M | scripts/include/townsfolk.inc
- M | scripts/misc/death.src
- M | scripts/misc/logon.src
- M | scripts/misc/reconnect.src
- M | scripts/playermanager.src
- M | scripts/runecl.exe
- M | scripts/textcmd/admin/admin.src
- M | scripts/textcmd/admin/class.src
- M | scripts/textcmd/admin/deathgate.src
- M | scripts/textcmd/admin/getglobal.src
- M | scripts/textcmd/admin/globalnoloot.src
- D | scripts/textcmd/admin/makemoongates.src
- M | scripts/textcmd/admin/maxcaps.src
- M | scripts/textcmd/admin/resetpw.src
- M | scripts/textcmd/admin/setallskills.src
- M | scripts/textcmd/admin/untile.src
- M | scripts/textcmd/coun/gorealm.src
- M | scripts/textcmd/coun/releaseinfo.src
- M | scripts/textcmd/coun/thaw.src
- M | scripts/textcmd/coun/unparalyze.src
- M | scripts/textcmd/gm/makekey.src
- M | scripts/textcmd/gm/newiteminfo.src
- M | scripts/textcmd/gm/px.src
- M | scripts/textcmd/gm/py.src
- M | scripts/textcmd/gm/pz.src
- M | scripts/textcmd/gm/unconcealhim.src
- M | scripts/textcmd/player/cast.src
- M | scripts/textcmd/player/clearmsglog.src
- M | scripts/textcmd/player/disarm.src
- M | scripts/textcmd/player/online.src
- M | scripts/textcmd/player/password.src
- M | scripts/textcmd/player/recalltotem.src
- M | scripts/textcmd/player/removejewels.src
- M | scripts/textcmd/player/showclasse.src
- M | scripts/textcmd/player/trashlb.src
- M | scripts/textcmd/player/undressme.src
- M | scripts/textcmd/seer/info.src
- M | scripts/textcmd/seer/npclist.src
- M | scripts/textcmd/seer/thawme.src
- M | scripts/textcmd/test/restartall.src
- M | uoconvert.exe
- M | uotool.exe

---

## Detailed Changes By Theme

### 1. Bundled polserver engine (core binary) nightly, 2026-09-20 build

**Files involved:** `pol.exe`, `poltool.exe`, `uoconvert.exe`, `uotool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe` (binary); `core-changes.txt`; `pol.cfg.example`; `scripts/ecompile.cfg.example`.

**Notable functional changes:**
- World data load is 20-30% faster at startup (varies with object count); the memory an object's custom properties take is reduced. Nothing about the data files or what is loaded from them changes.
- New `pol.cfg` option `LogWorldLoadDetails` (default 0): logs where reading each world data file spent its time (parsing, object creation, property reads, placement), total and per element.
- `ecompile -p` replaces the line-per-script output with one redrawn line plus a final summary; the summary now ends with the error and warning totals for the run (an error inside an include counts once per script that includes it). New `ecompile.cfg` option `DisplayFileOutcome` (default 1) controls the per-script "N errors" line; `-q` and `-p` set it to 0. Fixed: `-q` still printed one line per script; the summary reported only the last group of scripts when `-r` named several or `-A` was combined with script names under threaded compilation.
- New `damage_increase` / `damage_increase_mod` on characters and items (itemdesc `DamageIncrease`/`DamageIncreaseMod`, also accepted on NPC templates); an equipped item's value adds to the wearer's total, which is now sent in the status-bar field that always reported 0. The core only stores and shows it; no script in this repo uses it yet. `weapon.dmg_mod` is unchanged.
- `equip.cfg` color slots accept an inclusive range (`1701-1754`) and comma-separated lists of colors or ranges; `EquipFromTemplate()` picks per item per call and logs a color it cannot read.
- The script debugger now lists every member of a mobile (was cut off after 59). The tip window shows the tip text instead of its file name (Turley, 09-13).

**Expected impact:** Shorter server startup, slightly lower memory, better `ecompile` output. No gameplay change on its own; the equip color ranges and damage-increase properties are available for future templates.

### 2. `pol.cfg` debug options (unintended)

**Files involved:** `pol.cfg`.

**Notable functional changes:** `WatchRPM`, `WatchSysLoad`, `LogSysLoad`, `ReportRunToCompletionScripts`, `ReportCriticalScripts`, `ShowRealmInfo`, `ProfileCProps`, `EnforceMountObjtype` all `0` -> `1`.

**Expected impact:** More console and log output every cycle, a `ProfileCProps` cost on every custom-property read and write, and `EnforceMountObjtype` making the core refuse to mount an item whose objtype is not a registered mount. Flip back to 0 for the live build unless each one is wanted (see the note in the Scope Summary).

### 3. Power Hour and capper crash fixes (commits `e632446`, `12a9981`)

**Files involved:** `pkg/opt/powerhour/textcmd/player/ph.src`, `pkg/opt/powerhour/textcmd/player/setph.src`, `pkg/opt/capper/capper.src`.

**Notable functional changes:**
- `.ph` and `.setph`: `pph_use_time` and `pph_use_weekday` are read through `CInt()`. A character who never used a power hour (or ran `.resetph`) has neither property, and `use_time + week` was `error + integer`, which the current engine reports as a runtime error instead of returning the left operand; the script died. 0 now counts as eligible.
- `capper.src` (the periodic skill-cap enforcer): iterates `GetSkillIds()` instead of `0..SKILLID__HIGHEST`, and reads the power scroll matrix through `NormalizeSkillAndSlot(skill).slot`. Ids 49-56 are dummy AOS skills with no matrix slot; Alchemy (0) and Throwing (57) live in slots 49 and 50. The old loop indexed the matrix by raw id and crashed (a live server error report right after the 3.1.1 notes were written).

**Expected impact:** `.ph`/`.setph` work for fresh characters; the background cap check runs to completion and enforces the Throwing cap.

### 4. Autoloom station rework and the tailoring split (fix-log section 1)

**Files involved:** `pkg/std/tailoring/tailoringfunctions.inc` (new), `pkg/std/tailoring/autoloom_use.src` (new), `pkg/std/tailoring/autoloom_watchdog.src` (new), `pkg/std/tailoring/make_cloth_items.src` (thinned to an include plus its program), `pkg/std/tailoring/itemdesc.cfg`, `pkg/items/deed/built/autoloom.cfg`, `pkg/std/carpentry/carpentry.cfg`.

**Notable functional changes:**
- The Autoloom shipped in 3.1.1 as objtypes `0xA81D`/`0xA81E`; those are frames 0 and 1 of the South weaving animation, so both facings showed the South view. The station is now `0xA82D` (South) / `0xA82F` (East), the real stored poses, with `OldObjType 0xA81D`/`0xA81E` so looms already placed convert at world load. The deed build cfg and the carpentry recipe key (`Carpentry 0xA82D`) follow.
- One writer for the loom's graphic: `GetAutoloomGraphics(station)` returns `{anim_start, open_graphic, stored_graphic}` keyed on objtype; `AutoloomSetWeaving(station, weaving)` writes the client-animated start frame (`0xA81D`/`0xA825`) once per craft and lets the client cycle the frames; `autoloom_use.src` opens on double-click, stores on exit, stamps `#AutoloomOwner` with its pid and starts `autoloom_watchdog.src`, which reverts the graphic inside `Set_Critical` if the owning session dies (disconnect, crash) and the stamp still names it. The fire-and-forget `autoloom_animate.src` (server-stepped frames, raced the revert) is gone with its compiled outputs.
- `TryToMakeItem` and `CraftClothBulk` consume before they create and check every result: a full backpack charges once, breaks out and releases the resource lease; success and fame messages only fire when the item exists; Bandages honour the requested count at the cfg rate (2 cloth each) instead of converting every cloth in backpack plus Omega Cache at 4:1; the colour is set on the item descriptor so stacks merge (the engine only merges when `item.color == descriptor.color`). `make_cloth_items.src` lost six `use` lines the include now declares.

**Expected impact:** East looms show East art after their first use; a loom never sticks open or on the wrong facing after a disconnect; Make Now/Number/Max on Bandages makes the asked-for count; the weave loops at the client's frame rate during each craft.

### 5. Crafting: consume first, Power Hour half-cost, bulk caps (fix-log 2.0, 2.C1-2.C3, 3.S1-3.S4, 8.3, 8.5, 8.7, 8.12)

**Files involved:** `scripts/include/resourcemanager.inc` (new `HasCraftingResources`), `pkg/std/bowcraft/bowcraft.src`, `pkg/std/blacksmithy/blacksmithgump.inc`, `pkg/std/carpentry/carpentry.src`, `pkg/std/tinkering/tinkeringfunctions.inc`, `pkg/systems/crafting/include/multicraft.inc`, `pkg/opt/crafterboost/refinement_gump.src`, `pkg/opt/crafterboost/make_crafter_boosts.src`, `pkg/std/inscription/inscription.src`, `pkg/std/cartography/cartography.src`, `pkg/std/cooking/cooking.src`, `pkg/std/alchemy/alchemyfunctions.inc`, `pkg/std/itemid/itemid.src`, `pkg/std/itemid/itemid.inc`.

**Notable functional changes:**
- Every craft path now consumes with the result checked, then creates: `CraftLogBulk`, `CraftAmmoBulk`, `CraftLogRecipe` (bowcraft), `MakeSmeltedIngots` and `MakeBlacksmithItems` (the create is checked too; "Your backpack is full." and `break`), `TryToCreateItem` and `MakeYoungOakStaff` (carpentry), tinkering, `multicraft.inc` (every part re-checked, leases released on early exit), refinement gump, inscription, cartography (`ConsumeMap` returns its result). Before, the quantity was capped against materials counted before the "how many" prompt and never re-checked, so moving the materials away during the prompt produced the batch for free; Blacksmithy never checked the create, so Make Max on a full pack burned ingots every pass while announcing success. Cooking was traced and left; inscription/cartography had no real window and were aligned for consistency.
- `qty` clamped to 60,000 in the bulk paths (`CreateItemInBackpack` refuses larger amounts and the whole batch was charged for nothing).
- Power Hour half-cost: 24 sites of `CInt(Ceil(material/2))` became `/2.0`. Integer division ran first, so 1/2 = 0 and 25/2 = 12: 45 of 88 cooking recipes, 18 tinkering recipes and single arrows/bolts were free during a Crafting Power Hour. Odd costs now round up (25 -> 13).
- Carpentry: a failed exceptional `CheckSkill` after the exceptional chance came up made no item at all (false "backpack full", materials lost, roughly 1 craft in 15 on recipes that can be exceptional); a `make_excep` flag decided up front makes the normal item instead. Every plain carpentry craft also rolled a `CheckSkill` on skill id 0 (Alchemy) because `( skillid2 ) && !CheckSkill(...)` does not short-circuit in this build; split into nested `if`s.
- Alchemy at a placed station: `CreateItemInContainer(mortar.container, ...)` returned 0 for a world item, so reagents, bottle and skill roll were spent and no potion made (the mixture was stored on the station). New `AlchemyCreatePotion(user, mortar, product)` creates in the mortar's container when it has one, else the brewer's backpack, and only clears the stored mixture once the potion exists.
- Runebook crafting charges the targeted blank rune again (`SubtractAmount(item, 1)`; it fell off the component list in the resource-manager port) and checks mana before anything is consumed.
- Cooking loop condition `==` -> `<=`; bulk shaft colour on the descriptor.
- Item ID: `BASE_DELAY` 0 -> 10 s between identifications (it had been 0 since 2025-12-23; POL2.5 has 10), skipped at 100+ in the class's ID skill via new `ItemIDClassSkill(who)`.

**Expected impact:** No more free or double-charged crafts; half-cost items with an odd cost pay one more unit than before; alchemy at a station makes the potion; a runebook costs a blank rune; Item ID has a 10 s delay below 100 skill.

### 6. Combat and weapons (fix-log 2.2-2.8, 7.6, 7.10b, 16.2, 16.3)

**Files involved:** `pkg/systems/combat/hooks/omegaattack.inc`, `pkg/systems/combat/include/hitscriptinc.inc`, `pkg/systems/combat/hitscripts/paralyzehit.src`, `thiefpoisonhit.src`, `spellstrikescript.src`, `pkg/systems/attributes/hooks/shilhook.src`, `pkg/std/spells/mindblast.src`, `pkg/opt/summoning/processpoisonmod.src`, `scripts/include/damages.inc`, `scripts/include/spelldata.inc` (`BurnSelf`).

**Notable functional changes:**
- Melee weapons with no `MissSound` never missed: the miss `return` sat inside `if(attackerweaponType.MissSound)`. Now outside, like the ranged branch (Pickaxe, GhostWeapon, BatWeapon and everything below).
- `omegaattack.inc` reads `":*:itemdesc"` instead of `":combat:itemdesc"`: 14 weapons defined in other packages (8 astral weapons, shepherd's crook, young oak staff, `0x13e3`, `0x5002`, `0x30000`, `0x9000`) had no swing animation, never missed and were forced to `1d1`. The astral Black Staff goes from `1d1` to `4d5+1`.
- Town NPC swings: the 64 town templates with `AttackSpeed`/`AttackDamage 5d100` and no `AttackHitScript` get an intrinsic weapon with an empty hit script; every swing sent "Can't find a weapon hitscript" to staff and did nothing. An intrinsic weapon with no hit script now defaults to `:combat:mainhit` and takes its dice from the template's `AttackDamage`. Merchants, bankers and shrines hit for their template's 5d100 (the pick was "real damage").
- New `IsPvPBlockedByArea(attacker, defender)` in `hitscriptinc.inc`; paralyze, thief poison and spellstrike hit scripts call it before applying their effect (they applied it before the area check in `RecalcDmg`; thief poison had no check at all and passed no attacker to `SetPoison`, so a poison kill was not credited).
- A poisoned weapon's last charge (`PoisonCharges` 0 with `SkillPoisoned` set) made `DealDamage` erase both and return 0, eating the swing; it now clears the poison and carries on. Cursed poisoned weapons: `defender := attacker` leaked into reactive armor, on-hit and `ApplyTheDamage`; a separate `poison_target` means only the poison backfires.
- A skill set to "down" always failed (`return AwardSkillPoints(who, skillid, 0)` returns 0); it now drops once per use, zeroes `points`, sets `dropping` and rolls normally; `SkillAsPercentSkillCheck` skips its two unguarded half-point awards while dropping.
- Mind Blast: `ApplyRawDamage` -> `ApplyTheDamage(..., DMGID_MAGIC)` (kill credit works; players take 40% less). Poison ticks against players are scaled by `ReducePoison` when a GM set it with `.reducepoison`, otherwise 0.6 (the old condition also required `DEBUG_MODE`, so poison had no scaling at all); victim-based, so monster poison on a player is scaled too.
- `ApplyTheAstralDamage` read `who.mana` and wrote `who.mana`/`who.stamina`, none of which are character members in this engine: the test was always false, the stamina step could raise stamina, mana was never touched. Now `GetMana`/`SetMana`/`SetStamina`.
- `BurnSelf` (necro self-burn on a fizzle of Kill, Liche, Sorcerer's Bane) called `Resisted` with the circle and the target swapped, so the burn was 0; arguments fixed, the caster takes the resisted half.
- `pkg/systems/combat/shilcombat.inc` deleted (no caller, could not have compiled); its include removed from `scripts/misc/death.src`.

**Expected impact:** Weapons without a miss sound can miss; fourteen cross-package weapons deal their real damage; town NPCs fight back; weapon effects respect no-PK and safe areas; poisoned-weapon edge cases behave; skills being dropped still work; astral drain and necro fizzle burn work; Mind Blast and poison hit players more gently.

### 7. Player vendors, Omega Cache, bulk orders and shop items (fix-log 2.1, 3.A4, 5.1-5.5, 11.1, 11.2, 11.8, 11.10, 11.13, 11.14, 16.4)

**Files involved:** `pkg/systems/playervendor/playermerchant.src`, `pkg/std/bulkorders/bulkorderdeed.src`, `pkg/std/bulkorders/bulkorder_matching.inc`, `pkg/std/bulkorders/bulkorderrewards.src`, `pkg/opt/omegacache/omegacache.inc`, `pkg/opt/vanityshop/vanityshop.src`, `pkg/opt/vanityshop/customitemdye.src`, `customitemname.src`, `runebookdye.src`, `pkg/opt/powerscrolls/randomTome.src`, `pkg/opt/lootlottery/commands/GM/cfglotto.src`, `scripts/include/mrcspawn.inc`.

**Notable functional changes:**
- Player vendors could mint gold: `if(!price)` rejected only 0, so a negative buy price was stored; `BuyItem` let a negative total pass the affordability check and `mygold - theprice` grew the pool that `cheque` pays out. Now `price <= 0` is refused, the total is computed as a double and refused below 1 or above 2,000,000,000. Existing stored prices are not purged (the pick). `CashOut`/`CashCheque`: when the bank box and escrow both fail, a GM page is queued with the pack serial (the pack sits at 5288,1176).
- Bulk orders: a Large deed could consume anyone's completed Small (new `BOD_IsInOwnBackpack`, Small reserved before use); payouts above 60,000 paid nothing (new `BOD_PayGold`: stacks of 60,000 to the backpack, overflow to the bank box, then the feet; the seven-slot template goes over from about difficulty 120); the deed is destroyed before anything is paid or counted; a stack fills as many units as the order still needs (`SubtractAmount` when part stays) instead of being eaten for one credit; the reward table refuses `Cost < 1` and the gump loops in the program instead of re-opening from inside `BOD_RedeemReward`.
- Omega Cache deposit: `DepositItem` credited the store, then `DestroyItem` ran unchecked; a stack another script had reserved (bandages mid-heal, ingredients mid-cook, ore at the forge) stayed in the pack while the store was credited. Now the item is reserved first (refused with "That item is in use."), read, destroyed, and only a successful destroy writes the store (`DepositItemData`; the container path skips held stacks; drag-and-drop goes through the same function).
- Omega Cache withdraw: the available amount ignored leases (crafting from the cache leases the shortfall for 60 s per item) and every stack was created before the lease-aware `WithdrawItem`, whose smaller return was dropped, so leased units were created without a debit. Now `GetStoredAmount` excludes leases, each stack is debited first and exactly the debited amount is recreated from a property snapshot (`SnapshotCacheElement`, `RestoreCacheElement`, `RecreateItemFromProps`); a failed create puts the debit back.
- Vanity shop x5/x10 bundles: pieces already moved into a pack that filled part-way are destroyed with the bag so an aborted purchase hands over nothing (charging first would need a free slot in the very pack that was full). Omega Dye, Soul Pen and Runic Dye Tub return after the "disintegrates" message instead of continuing with 0 charges; the Soul Pen refuses stackables (a renamed pile lost the name through the cache). Random Ancient Tome: create the Power Tome first, refuse on a full pack, destroy the Random Tome afterwards.
- `.cfglotto`: Single Winner checkbox initialised from `Lotto_single` (was `Lotto_item`, a string, always ticked) and both boxes stored as 0/1 (an unticked box came back as an error value).
- NPC vendor sell list: the `LFucker` (staff-marked item) sweep was a `foreach` over `EnumerateItemsInContainer(array)` nested in the pricing loop and never ran; marked items are now priced at 0 in one loop after pricing.

**Expected impact:** No negative vendor prices; bulk order turn-ins pay in full and only from your own pack; the Omega Cache cannot be credited for a stack that stayed in the pack nor pay out leased units; bundle purchases are all-or-nothing; charged dye/rename items stop at zero charges; staff-marked items sell for nothing.

### 8. Accounts, login and hunger (fix-log 3.0, 3.A1, 3.A2, 8.8, 14.10, 15.8)

**Files involved:** `pkg/systems/email/pkg.cfg`, `pkg/systems/accounts/acctWatcher/acctWatcher.src`, `pkg/systems/accounts/hook/onLogin.src`, `pkg/systems/accounts/config/settings.cfg`, `pkg/std/cooking/hunger.src`, `pkg/std/cooking/hungerdamage.src`, `scripts/misc/logon.src`, `scripts/misc/reconnect.src`, `scripts/playermanager.src`, `scripts/include/client.inc`.

**Notable functional changes:**
- The email package is disabled (`Enabled 0`; takes `.email`, `.inspectmail` and its hooks; nothing outside it references `:email:`; the `Emails` datafile is untouched).
- Account watcher: an empty account with no `LastLogin` was stamped and then deleted in the same pass on the stale local `last_used`; it now returns after stamping.
- Login lockout (`AcctHackChecks` rewritten): the failure counter was only consulted when the password was wrong, so a locked-out address with the right password got in; `FailureInterval 3` (seconds) meant failures rarely accumulated. Now the lockout is checked before the password, a good login clears the address's failures, reaching `MaxLoginFailures` locks the account and address for `DisableLength`, `DisableGrace` works as documented; `FailureInterval` 3 -> 180 (still seconds).
- Hunger never drained: `Start_Script(":cooking:hungerdamage", {chr})` passed a one-item list which the script used as the character, so its loop never ran; it also only started at the 9 -> 10 tick and exited on relog. Now the character is passed, `hungerdamage.src` stamps `#HungerDrainPid`, `EnsureHungerDrain(chr)` starts one when none is alive (at login when hunger >= 10 and after every hourly tick at >= 10), `StopHungerDrain(chr)` kills a stale one at login. `scripts/include/client.inc` `DEBUG_MODE := 0`, so `logon.src` no longer resets hunger to 1 at login; `playermanager.src` writes the login clamp as `SetObjProperty(who, "hunger", 15)` (`who.hunger` was not a field).
- Safe-zone and no-PK flags at login and reconnect: login stripped the safe-zone protection and never re-checked position (racing the region-enter script), reconnect did nothing. Both now strip, then decide from the areas helpers: inside a safe area grant, inside a no-PK area set the flag, otherwise clear a stale no-PK flag. The `.areas` refresh grants only to players inside a scripted region.

**Expected impact:** Hunger is live for the first time in practice: it climbs 1 per hour online; from 10 stamina drains 2 per 6 s, from 12 mana too, from 14 health too; eating brings it down and at 14 the character auto-eats cooked food from the backpack. Failed logins lock the address after five tries within three minutes; email commands are gone; safe-zone flags are right from the first second after login.

### 9. Theft, stealth, traps, gathering and instruments (fix-log 4.1-4.5, 8.4a, 8.6, 8.14)

**Files involved:** `pkg/std/traps/trapScripts/setTrap.src`, `pkg/std/removetrap/removetrap.src`, `pkg/std/snooping/snooping.src`, `pkg/std/lockpicking/use/picklock.src`, `pkg/std/musicianship/musicianship.src`, `pkg/std/treasuremap/digtreasure.src`, `pkg/std/training/dummy_pickpocket.src`.

**Notable functional changes:**
- Trap items worked on any container in view: `Distance > 2`, `!Accessible` and new `CanSetTrapHere(who, cont)` (mirrors `magictrap.src`: refuses objtypes `0x7100`, `0xefa`, `0x9c16`, `0x9c17`, a player's corpse, and any container in a house unless owner/co-owner/friend). Remove Trap needs line of sight and reach.
- Snooping container keyed by `Hex(who.serial) + " Snooping"` (was by name; the old one is removed at each snoop).
- Lockpicking: `LOCKPICK_DELAY_SUCCESS := 5`, `LOCKPICK_DELAY_FAILURE := 10` stored in `#LockpickDelay` on the player and checked before the target prompt (the old script set its delay on the player and checked it on the chest); the pick-break roll passes 0 points.
- Instruments unlocked themselves when played (`instrument.movable := 1` at the top of the script; a house lockdown is `movable := 0`), so anyone inside a house could play a locked-down harp and carry it off. Lines removed; an immovable instrument still plays.
- Digging needs the map in the digger's backpack. Pickpocket dummy stops at 25 Stealing and refuses a second user while one is in progress (`#picking`).

**Expected impact:** Traps and Remove Trap only within reach and on containers you may touch; lockpicking has a 5 s / 10 s delay; locked-down instruments stay locked down; the pickpocket dummy trains to 25.

### 10. Spells, spell books, verses and songs (fix-log 6.1-6.3, 7.1-7.11, 12.1-12.17, 15.1)

**Files involved:** `pkg/opt/moongates/itemdesc.cfg`, `pkg/std/spells/{mark,unlock,heal,gheal,resurrect,dispel,massdispel,dispel_field,reactivearmor,invisibility,cure,polymorph,teleport,blade_spirit,vortex}.src`, `pkg/std/hiding/hiding.src`, `pkg/opt/summoning/{summoning,npcsummoning}.src`, `pkg/opt/versebook/*` (`include/versefunctions.inc`, `versebook.src`, ten verse scripts), `pkg/opt/songbook/*` (`songbook.src`, `songofsirens.src`, `songofbeckon.src`, `songofcloaking.src`, `songofsalvation.src`, `songscroll.src`), `pkg/opt/holybook/*` (`holybook.src`, `wrathofgod.src`, `angelicgate.src`, `angelicfeast.src`, `revive.src`, `sanctuary.src`, `holyscroll.src`), `pkg/opt/earth/*` (`earthportal.src`, `bookofearth.src`, `antidote.src`, `druidscroll.src`), `pkg/opt/necro/{codexdamnorum,necroscroll}.src`, `pkg/opt/MagicWands/magicwands.src`, `scripts/include/spelldata.inc`, `scripts/include/bard.inc`, `scripts/include/spellgrants.inc` (new), `scripts/misc/logon.src`, `scripts/textcmd/player/cast.src`, `config/itemdesc.cfg` (0x7012).

**Notable functional changes:**
- Casting-flag lockouts, the biggest quality-of-life item in this release: `#Casting` is only cleared by logoff, and every spellbook, ritual scroll and `.cast` refuses while it is set. It was left set by `.cast` (nineteen early exits in `CastSpellFunction` after the command had set it; the early set is gone), by Earth Portal's two early returns, by any spell script that failed to start (the books and scrolls now check `Start_Script`'s result and clear), by every verse that ended on its own (new `EndVerse`, called from all ten verse scripts), and by rituals (theme 14).
- Book reply validation: each book drew buttons only for the spells it held but took any reply id straight to the spell (a forged reply performed any verse or song; in the codex an unlearned slot became spell id 0 and locked the player). The verse book performs only a learned verse, the song/earth/holy books accept ids 1-16 whose Lesser/Greater bit is set, the codex only slots that hold a spell.
- Summons: `saveonexit := 0` on `SummonCreature`, `npcsummoning`, blade spirit, vortex and Song of Beckon's fairy (a save inside their lifetime left permanent 300-hp pets that the tamed AI never released); the fairy is ended with the fade effect and `.kill()`. Dispel's summoned branch uses `KilledBy`/`KilledBySerial`/`.kill()` instead of `ApplyRawDamage(GetMaxHp(caster) + 3)`; Mass Dispel `continue`s past an immune victim (was `return`) with a per-victim `dispel_power`; Dispel Field passes the realm.
- Temporary gates: the Gate spell pair `0x99b1` `SaveOnExit 1 -> 0`; Earth Portal's far gate `0x7012` `SaveOnExit 0` in `config/itemdesc.cfg`; Angelic Gate's Gate of Life and Earth Portal set `saveonexit := 0` on creation. A save plus restart inside the 30-150 s window used to leave permanent gates.
- Healing spells damaged Liche-form players anywhere (the Undead branch used `ApplyTheDamage`, which has no area check; Resurrection dealt max HP + 3): new `IsSpellPvPBlockedByArea(caster, cast_on)`; no such damage between players when either has `NOPKAREA` or is in a safe area.
- Timers: Reactive Armor uses `CanTargetSpell` and a `#ReactiveArmorPid` stamp cleared only by its own cast; Invisibility polls every second and ends when the target is revealed or the stamp changes; a skill hide erases `#InvisPid`. Cure and Invisibility no longer reflect. Polymorph's Warrior `critter` clamped at 0. Teleport's stand check always returns.
- Mark: the staff tag went onto the targeted blank rune instead of the marked one; the blank is no longer used up when the marked rune could not be created. Unlock refuses `houseserial`/`.multi` targets like lockpicking.
- Area helpers: `SmartAoE` skips the caster first (a solo caster was a victim of Rising Fire, two Apocalypse parts, Wraith's Breath, Abyssal Flame, Plague and Chain Lightning); `SmartSongAoE` skips anyone in a safe area (Sonic Disturbance, Bardic Boulders, explode corpse, Spirit Flock could hit people inside a bank from outside); `ValidSongBoost`/`SmartSongBoost` require a real party (`a.party == b.party` compared two error values as equal for party-less characters, so every stranger counted as a party-mate for Life Balance, Lesser Healing, Shadows, Dragon Skin and the boosting songs).
- Wrath of God's reflected hit is self-inflicted (`ApplyPlanarDamage(caster, caster, ...)`; the target was being recorded as aggressor and killer). Song of Sirens returns on "Out of Stamina" instead of paralyzing for free without the cast time. Song of Cloaking reveals only the people it cloaked. Salvation deals its half through `ApplyTheDamage(..., DMGID_MAGIC)` and, with Revive, exempts Boss/SuperBoss.
- Dragon Skin: the grant is recorded in `DragonSkinGrant`, taken back on every exit (the fumble path used to skip it) and at login via `ReclaimDragonSkinGrant` (leaked points stacked another set after a restart). Antidote and Sanctuary mark their `PermPoisonImmunity` grant with `SpellPoisonImmunity`; the spell's end and every login reset it to the highest worn-item level (`ReclaimSpellPoisonImmunity`). Both reclaims run from `logon.src`.
- Scrolls (druid, holy, necro): the "already casting" check comes before the scroll is subtracted, `CastingOpts` are cleared after the wait (a spell that stopped before `TryToCast` left the next book cast reagent-free), plus the `Start_Script` guard. `CastingNecro` (set and read by nothing) removed. Angelic Feast made one food item too many (`<=` -> `<`). Corpse Distention leaves critical mode before its unsuitable-corpse return. Beastal Bond no longer starts a script that does not exist.
- Classic wands: `magicwands.src` sets `NOREGS`, `NOMANA`, `NOSKILL` and a 30 s `#WandCast` stamp that `can_cast` honours for the circle limit; the charge is the whole cost and any character can use any wand (the golem, toxic cloud and wall of death wands were already free).
- Deleted: `pkg/std/spells/bless timer.src`, `protection with timer.src`, `pkg/opt/necro/sunderingsword.src` (bound to no item; killed any summon in sight), `pkg/opt/earth/shapechange.cfg` (unused twin of shapeshift.cfg).

**Expected impact:** No more "You are already casting something!" until relog; summons and temporary gates never survive a restart; area spells miss the caster and respect safe areas; bard boosts only reach real party-mates; Dragon Skin and poison-immunity grants cannot leak; wands work for everyone at the cost of a charge; Liche players are safe from heals in protected areas.

### 11. Housing, boats and secure containers (fix-log 9.1-9.14)

**Files involved:** `pkg/multis/house/multiSign/use.src`, `pkg/multis/house/multiSign/control.src`, `pkg/multis/customhousing/sign.src`, `pkg/multis/customhousing/include/house.inc`, `pkg/multis/customhousing/scripts/customhousedeed.src`, `pkg/multis/customhousing/syshook/closecustomhouse.src`, `pkg/multis/staticHousing/sign/{use,control,destroy}.src`, `pkg/multis/staticHousing/logon.src`, `pkg/multis/staticHousing/config/settings.cfg`, `pkg/multis/boat/multi/listener.src`, `pkg/multis/boat/tiller/{methods,canInsert}.src`, `scripts/include/housing.inc`, `pkg/items/containers/container/{canRemove,canInsert,use}.src`.

**Notable functional changes:**
- Eject: classic and custom refused only when the target was outside every house and the ejector more than 15 tiles from the sign; static checked the ejector. Now mobiles only, never staff, a player vendor, the owner or a co-owner, and only someone `IsObjectInsideHouse` puts in this house; the ejected player is told.
- Boats: `ProcessEvent` ran every spoken command from anyone on deck (`CanCommand` existed and was never called); commands and maps handed to the tillerman now need `CanCommand` ("Ye ain't the captain o' this ship!"); crew mates match by serial. A map refused by the tillerman goes back to the backpack.
- Custom house transfer and demolish are owner-only (cases 13 and 14 were open to every full manager); a transfer wipes co-owners, friends, bans and permissions (`ChangeOwner` kept them).
- Secure containers were only guarded when opened: the hooks tested `container.IsSecured()` (a `.level` no package writes) against the mobile's house. New `FindHouseSecureContainer`/`HouseSecureAccessAllowed` in `housing.inc`: the remove/insert/use hooks walk up to the secure container (a bag inside one counts), resolve its own house sign (custom: `secured`; classic: `houseserial` -> multi -> `signserial`; static: `houseserial`) and apply that package's rule: staff always, a ban never, static `SecuredLevel` by the owner/co-owner/friend ladder, otherwise `HasHousePermission(who, "secure")`; a secure whose house cannot be found refuses everyone but staff.
- Houses-per-account: the classic sign registers its owner's account when its control script starts (the `#housing_of_<acct>` registry is a runtime global; static and custom already re-registered).
- Custom house keys: the package wrote a lowercase `lockid` (value: the house serial) on keys, sign and doors while the key package reads `LockID`; the close hook looked for a classic sign a custom house never has and locked every door with 0; "Change the locks" re-keyed the key and no door. Now `AllocateLockID` and `LockID` everywhere, the close hook takes the id from the 0xFFF4 sign, "Change the locks" needs a blank key and re-keys every door, the demolish sweep uses `KP_DestroyOwnedKeysForLockIDs`, and `MigrateCustomHouseLockIds` runs on every sign use to convert old houses, their doors and the keys in the owner's backpack and bank.
- Custom house demolish: the price was read off a classic sign that was never found, so it returned nothing. Now (the owner's call) half of what the house cost: the foundation price on the sign plus `customhouse_paidparts` x 500, no cap, no deed, gold in stacks of 60,000 to the backpack or the feet. A classic multi placed through this package still gets its deed.
- Classic decay called `demolish(house)` where the function expects the sign: the deed was destroyed and the house stayed; now `demolish(sign)` with the account registry updated. Classic demolish with a full backpack: the deed type is resolved first, the deed created before anything is touched (backpack or ground), an unknown type refuses ("page a GM"), and a refused multi destroy takes the new deed back.
- Static house double sale: the "buy?" question blocked with the sign still for sale and both confirmations were charged; the sale is re-checked after confirmation, payment and hand-over run critical, the sign is marked sold when the gold is taken. Static decay refresh only for the owner and co-owners (was anyone who clicked the sign or spoke inside). `DebugLogging 1 -> 0`.
- Small: "Ban someone" refuses staff on classic and custom signs; static logon uses `BootBannedFromHouse` (was a start of a script that does not exist); static destroy erases `LockID` from the component, not its serial; the owner's own account is refused before a friend is added.

**Expected impact:** Only the captain sails; only the owner transfers or demolishes a custom house; secure containers are guarded at every access path; custom house doors open with the house key; demolishing a custom house pays half its cost; classic demolish never loses the deed; static houses cannot be sold twice; eject only works on people actually in the house.

### 12. Packet hooks and tooltips (fix-log 10.2-10.8)

**Files involved:** `pkg/packethooks/speech/receivespeechhook.src`, `pkg/packethooks/megacliloc/itemdata.src`, `pkg/packethooks/megacliloc/commands/player/updatetp.src`, `pkg/packethooks/versionHook/versionhook.src`, `pkg/packethooks/packethook/packethook.src`, `pkg/packethooks/packethook/uopacket.cfg`.

**Notable functional changes:**
- Staff speech containing a client keyword (packet type 0xC0) was logged as "1" and echoed back as a system message (`speech := SendSysMessage(...)`); now decoded without the send. The staff speech log re-opens its data file after creating it (the first line for a new staff account was lost).
- Weapon tooltip damage and DPS use the attacker side of today's `CalcPhysicalDamage` (quality, durability, per-class skill averages, level bonuses, Mage divisor) instead of a 2024 copy of the formula, and DPS uses the engine's swing time (15000 / ((Dex + 100) x (Speed + speed mod))). The hit-script tooltip lines match the item's own `hitscript` with the package prefix stripped (only combat-package weapons matched before); the tri-elemental label is "Chance to strike with lightning".
- `.updatetp` did nothing (a position struct was passed to `IncRevision`); it now targets an object. Two no-op `SleepMS(5)` calls removed from the version hook (a packet hook cannot suspend).
- The 14-byte legacy drop hook (pre-6.0.1.7 clients, never admitted by the version gate) is removed with its `uopacket.cfg` entry.

**Expected impact:** Weapon tooltips show truer damage and about half the previous DPS number for a 100-Dex character (display only); `.updatetp` refreshes a tooltip; staff no longer see their keyword speech echoed.

### 13. Guilds, towns, donator mounts and corpse looting (fix-log 11.4-11.7, 11.9, 11.11, 11.12, 11.15, 11.16)

**Files involved:** `pkg/opt/guilds/commands/player/guilds.src`, `pkg/opt/guilds/include/guilds.inc`, `pkg/opt/guilds/commands/test/changeguildownership.src`, `pkg/opt/guilds/ondelete.src`, `pkg/opt/townstones/tstone.src`, `pkg/opt/townstones/tstone.inc`, `pkg/opt/townstones/electionwatch.src`, `pkg/opt/Donator/include/playertown.inc` (new), `pkg/opt/Donator/donator{horse,bear,llama,ostard}stone.src`, `pkg/opt/powerhour/textcmd/test/resetph.src`, `pkg/opt/loot/antiloot.inc`; deleted `pkg/opt/vanityshop/include/mountFunctions.src`.

**Notable functional changes:**
- Guild formation took the 120,000 gold before checking that the house belonged to the character (a sibling character's house cost the gold and formed nothing) and ignored an existing `GuildHouse` tag; the check runs before any gold moves, a house another guild holds is refused and no longer listed, both re-checked after the confirmation gump.
- `ChangeGuildMaster` promoted `guild.members[1]` even when that was the master being removed (character deletion runs it for an offline master) and told every member they were the new master; now the first member who is not leaving, with the successor named. The Dev `changeguildownership` refuses an OK with nobody selected.
- Recruiting no longer opens the guild-list gump on the recruit (`JoinGuildRequest` takes `show_list`); `DisplayGuildInfo` keeps the master out of the member list; `DisplayGuildMembers` lists applicants from the live `JoinGuildRequest` list; the dead "Recruits" property and `AddRecruit` are gone; `ondelete.src` removes a deleted character from every guild's join list.
- Town stone handlers trusted the gump: any reply id let a non-citizen vote, declare candidacy, call an election or leave a town they never joined (one off the population each time), anyone start a poll, and a citizen of another town join a second one. New `IsTownCitizen`; citizen-of-this-town for voting, candidacy, elections and leaving; mayor for polls; joining refused with an existing town; population never below zero. Leaving a town strips the name with `StripTownSuffix` instead of find-and-cut arithmetic that went negative. Town cheque donation requires the cheque inside the donor's backpack.
- Election watcher: a 10,000-tile item scan in six realms every 5 s for as long as any election or poll ran; the stones are resolved from the townstone datafile's `stone_serial` and the tick is 60 s (world scan only when none resolve).
- Donator mounts: `IsInPlayerCity` is now "standing in a City region with a town stone registered in the townstone datafile" (shared `playertown.inc`); the four stones' own 250-tile search for objtype 0x7566, an item this repo never had, is removed. `resetph.src` tests `isa(POLCLASS_NPC)`. `mountFunctions.src` (included by nothing, one broken loop) deleted.
- Corpse-loot guild exemption read a `guild_id` property nothing sets; it now compares the looter's guild with the corpse owner's (offline included): same guild, a guild at war, or an ally loots without the criminal flag, report or auto-jail.

**Expected impact:** Guild formation cannot lose the gold; guild succession picks a real successor; town stone actions require the right role; donator mounts work in player-run towns such as Zento; guild-war looting in town is exempt as designed; the election watcher no longer scans the world every five seconds.

### 14. Rituals, artifacts, eggs and zulu items (fix-log 13.4-13.10)

**Files involved:** `pkg/opt/rituals/include/rituals.inc`, `pkg/opt/rituals/rituals/demonstration.src`, `pkg/opt/ArtifactSystem/itemdesc.cfg`, `pkg/opt/ArtifactSystem/artifactbox.src`, `pkg/opt/zuluitems/{dragoneggs,ostardeggs,cannon,catapult}.src`, `pkg/opt/christmas/Christmasgifts.src`; deleted `pkg/opt/zuluitems/dyecheck.src`.

**Notable functional changes:**
- The artifact box shared objtype 0x7990 with an orc boat hull component (config/boats.cfg), so that hull piece took the box's script and double-clicking it handed out an artifact; the box is now 0x303C8 (custom range) and is destroyed only after the artifact is in the backpack ("You cannot carry what is inside the box." otherwise). Boxes already in the save keep 0x7990 and show as the hull piece; replace with `.create artifactbox`.
- Ritual darkness: each drawing step set an until-logoff light override on everyone within ten tiles; now a 120 s override refreshed per step (`RITUAL_LIGHT_STEP_SECONDS`), cleared outright on success and by both teardown helpers. The ritual casting flag is cleared on every exit (`RITUAL_ClearCasting`; PerformRitual's five early checks, the demonstration's three exits, both teardown helpers); the two undo-phase "captor vanished" exits tear the circle down like their siblings instead of leaving the items on the ground.
- Hatched dragons and ostards get `SetMaster(who)` (they had only the `master` cprop, so the owner's own area spells and songs treated them as wild; frenzied eggs already did this).
- Cannon and catapult clamp the impact point to plus or minus 10 in every direction (only east and south before) and refuse without `CheckLosAt` from the shooter; the cannon's too-far exit clears `#inuse`. Powder and cannonballs are still not obtainable, so this stays latent.
- Santa's one-present-per-day stamp is the saved `GiftedAlready` (the runtime `#GiftedAlready` was forgotten at every restart). `dyecheck.src` (an unbound older copy of the dye tub script) deleted.

**Expected impact:** Bystanders are not left in the dark by rituals; a ritual never locks the caster out of spellbooks; hatched pets count as yours for area effects; orc boat hull pieces stop dispensing artifacts.

### 15. Spawn points, areas, champions, quests, roleplaying and staff logging (fix-log 14.1-14.13)

**Files involved:** `pkg/opt/alryc/textcmd/test/mounttest.src` (moved from `.../player/`), `pkg/opt/areas/callguards.src`, `pkg/opt/areas/include/areapolicy.inc`, `pkg/opt/areas/textcmd/admin/areas.src`, `pkg/opt/roleplaying/rperstone.src`, `pkg/opt/roleplaying/macrotimer.src`, `pkg/opt/roleplaying/textcmd/coun/macrotest.src`, `pkg/opt/roleplaying/textcmd/admin/fixstartgear.src`, `pkg/opt/questpkg/include/{queststate,questdeath,questfishing,questjournal,questnpcgump}.inc`, `pkg/opt/champspawns/include/rewards.inc`, `pkg/opt/champspawns/scripts/control.src`, `pkg/opt/Staff/RecordXYZ.src`, `pkg/opt/spawnpoint/checkpoint.src`, `pkg/opt/spawnpoint/spawnpointmanager.src`, `pkg/opt/spawnpoint/include/customnpc.inc`, `pkg/opt/spawnpoint/spawntriggerwalkon.src`, `pkg/opt/spawnpoint/textcmd/admin/{newmobedit,gotospawnpoint,despawn,forcespawn,primespawn,forcespawnarea}.src`, `pkg/opt/warriorforhire/warrior.src`, `pkg/opt/msg/commands/player/msg.src`, `pkg/opt/decoratefacets/commands/test/udestroymany.src`, `pkg/opt/Events/textcmd/seer/createEventBag.src`, `scripts/misc/death.src`.

**Notable functional changes:**
- `.mounttest` moved from the player command folder to the test folder (any player could kill their mount and equip any of 68 mounts for free); the stale compiled outputs in the player folder removed.
- Guard call: the "criminal master" flag is reset per tamed creature (it stuck after the first one). RPer stone: OKAY without a class returns before zeroing skills (it used to strip every skill and set the class skills of nothing). Macro check gump moved to x 60-156 / y 50-170 (off-screen below ~1,000 px wide before); a player who disconnected during the fifteen minutes is reported to staff instead of jailed. `fixstartgear` reads the class name after giving the gear.
- Quest rewards are created before the quest completes (new `QP_CreateReward`: pack, else the feet with a message; nothing completes if a reward cannot be created); the cooldown text rounds up; 28 informational prints go behind `QUESTPKG_DEBUG`/`QP_Debug` (also used by `death.src` for quest kills).
- Champion gold: the map check used the loop offsets as coordinates, so gold landed on one quadrant (121 tiles); it now covers all 441 tiles at 137-685 per pile, same total per champion. The champion "less than 2 hours / 1 hour" windows are as wide as the idle sleep so exactly one tick lands in each.
- Staff positions are logged every 30 s instead of every 2 s (a disk write per step).
- Spawn points: a retried placement for a normal point restored non-existent saved vitals and left the creature at 1 hit point (`SetHP(critter, vits.hits)` on an error); the restore runs only for `CustomPoint` points and placement tries up to ten spots. Time-expire stamps the creation time, compares the right way round and deletes through the manager's destroy event; a Group point at an unstandable spot returns "Invalid spawning location" instead of retrying forever; the hidden `.gotospawnpoint <template> 27347` mass delete is gone; `.despawn`/`.forcespawn`/`.primespawn` refuse a non-spawn-point target; trigger tiles ignore NPCs and ghosts; a saved custom NPC override writes `npc.objtype` (was its strength). `newmobedit`: the AR edit subtracts the target's own armour (was the staff member's); HP/stamina/mana edits set the Custom*Level and the current value through the scaled helpers ("500" became 5 before).
- Area policy masks are cached in the global property `AreaPolicyCache_<realm>` (rebuilt from the datafile when missing, rewritten by every save path); every safe-area/no-PK/guarded/anti-magic/recall question used to open the datafile from disk. The `.areas` page checkbox state applies on any button press.
- Warrior for Hire's "stop following staff" tests `cmdlevel` (was `.cmd`, not a member); `.msg` shows the first message ever logged (index 0 vs 1); `udestroymany` range mode tests `DecorateFacetsStatic`; `createEventBag` uses the package's `GetPrize` instead of a 105-row copy of the table.

**Expected impact:** Champion gold spreads over the whole circle; quest rewards are never lost to a full pack; spawned monsters keep full health; area checks stop hitting the disk; the macro check can be answered on every screen size; guards judge each pet by its own owner.

### 16. Player commands and top-level scripts (fix-log 15.1-15.11)

**Files involved:** `scripts/textcmd/player/{cast,recalltotem,online,showclasse,disarm,undressme,clearmsglog,trashlb,password,removejewels}.src`, `scripts/control/trashControl.src`, `scripts/playermanager.src`, `scripts/misc/death.src`.

**Notable functional changes:**
- `.cast` no longer sets `#Casting` up front (theme 10). `.recalltotem` looks the totem up by serial (the serial was used as the creature; unhide, move and speech failed silently and the 24 h cooldown was stamped anyway); the cooldown is stamped only after a successful move. Trash cans destroy items with no `#DestroyAt` stamp (anything in a can at a restart stayed forever). `.online` shows each row's account age (both loops read the viewer's). The first level-4 Bladesinger broadcast saved under "4bs" (was the Paladin key). `.disarm`/`.undressme` read the shield hand's curse from the shield. `.clearmsglog` keeps message 0 in the archive, treats an empty log as nothing to clear (no archive, no cooldown) and reopens its datafiles after creating them. `.trashlb` builds the board once and caches it for ten minutes (`TrashLeaderboardCache`; was two full passes over every account per press). `.password` says the limit is 16; `.removejewels` spelling.

**Expected impact:** Totem recall works; trash cans empty after a restart; the trash leaderboard opens instantly; `.online` and `.showclasse` show the right data.

### 17. Shared includes (fix-log 16.1-16.14)

**Files involved:** `scripts/include/{attributes,damages,spelldata,mrcspawn,speech,skilltitles,townsfolk,teleporters,jailcheck,skillpoints,npccast,privs,starteqp,classes}.inc`.

**Notable functional changes:**
- `AwardRawAttributePoints` reads the power scroll matrix through `NormalizeSkillAndSlot` (Throwing read past the end and had no cap at all); `GetSpeed` accepts numeric `MoveSpeed` values (the digit loop started at position 0 and every numeric value fell to the slowest speed); skill title 43 is Wrestler (entry 42 was written twice).
- `ClasseBonus`/`ClasseBonusBySkillId` no longer run `unequipRestrictedItems` (a paperdoll sweep with two 5 ms pauses per worn item) from their 127 call sites, including the combat hit script and the spell resist roll; the sweep still runs where the class is set (`AssignClasse`, the 10-minute class loop, `.showclasse`, the stones) and the equip hook refuses prohibited items up front.
- Townsfolk: the three sayings tables were filled from index 0 (refused) and drawn with `Random(10)`; now 1-10 and `Random(len)+1`; the flee loop tests `SYSEVENT_DAMAGED` (was the word "damage"); `PlayMidi` sends the tune to each online character within 15 tiles on the singer's realm. Quest-item direction whispers go to the player (they named the item). `CreateTeleporters` stops after a hundred tries (was endless once it reached 100); the four Ter Mur teleporter rows point at `termur` (was "Felucca", a realm that does not exist here).
- `Jailcheck` tests a 40-tile box around `DEFAULT_LOCATION_JAIL_X/Y/R` (was an old-map box), and `AwardRawSkillPoints` includes and calls it, so skill gain stops in the jail again. Loot stacks double on either hunting power hour (`&&` -> the same rule the item chance rolls use). `npccast.inc` console prints removed; `AlterPrivsBase` calls `who.disable(priv)`.

**Expected impact:** The Throwing cap applies; NPC templates with numeric move speeds move at that speed; townsfolk talk and the minstrel plays; Ter Mur teleporters work; no skill gain in jail; a measurable saving on every swing and resist roll.

### 18. NPC AI (fix-log 17.1-17.12)

**Files involved:** `scripts/ai/setup/modsetup.inc`, `scripts/ai/{chaosmultikillpcs,sum,soulwhisperer,humuc,loke,thor,immobile,gambler,water,townguard,highpriest,merchant}.src`, `scripts/ai/main/{vortexloopkill,chaoskillpcsloop}.inc`, `scripts/ai/combat/doppelcombatevent.inc`.

**Notable functional changes:**
- Spawn-point anchors are applied: `modsetup.inc` read the "Anchor" cprop as a struct while the spawner writes a six-slot array, so `SetAnchor` got a range of 0 (anchor off) and spawned NPCs were never leashed. Both shapes are read now; outside the range each step further away is refused with probability (distance - range) x psub percent, floor 5%, not in war mode; the spawner default is range 4, psub 25.
- `me.mana`, `me.dexterity`, `critter.mana`, `master.mana` (not character members in this engine) replaced by `GetMana`/`SetMana`/`GetDexterity` in chaosmultikillpcs, vortexloopkill, sum, soulwhisperer and humuc; the totem's "vamp" works on a fresh totem (the "lastvamp" arithmetic was an error). `look_around` and the soulwhisperer field sweep pass `me.realm` (searched the old map).
- Loke and Thor swap weapons by what is in each hand (melee within 7 tiles, bow beyond) instead of pre-creating spares that went to the corpse. Rooted monsters (`immobile.src`) return 1 from `CloseDistance` so the fight loop waits a second per pass instead of spinning every 150 ms. The blackjack double-down flag is shared (a local `dd` hid it, so every hand after a double-down was charged double). The water shrine's "claim" checks the nine shells without writing "shell0". The town guard's two old-map self-destruct boxes are gone. The high priest's forgiveness fine is level x 2500 floored to 1,000 and capped at 60,000 (a classless player was forgiven for one coin). The merchant training window calls `data.keys()` and rounds the typed level to whole tenths. Doppel form-change console print removed.

**Expected impact:** Monsters from spawn points keep to their wander range; Loke/Thor drop no spare weapon; rooted monsters stop hammering the server; the casino charges the right bet; the priest's fine is priced like the other services.

### 19. Staff commands (fix-log 18.1-18.20)

**Files involved:** `scripts/textcmd/admin/{globalnoloot,admin,untile,deathgate,getglobal,resetpw,setallskills,class,maxcaps}.src`, `scripts/textcmd/coun/{unparalyze,thaw,releaseinfo,gorealm}.src`, `scripts/textcmd/seer/{info,thawme,npclist}.src`, `scripts/textcmd/gm/{makekey,unconcealhim,newiteminfo,px,py,pz}.src`, `scripts/textcmd/test/restartall.src`, `config/command_synopses.cfg`; deleted `scripts/textcmd/admin/makemoongates.src`.

**Notable functional changes:**
- `.globalnoloot` multiplied the minutes by 60 twice (60 minutes became 60 hours and the broadcast said 3600); once now. `.unparalyze` set the "frozen" cprop to 1 after unfreezing and `.thaw`, `.info` unfreeze and `.thawme` never erased it, so a thawed NPC re-froze on its next script start (61 AI scripts read it); all four erase it. `.makekey` wrote lowercase "lockid" while every lock check reads "LockID" (cprop names are case-sensitive), so its keys never fit. `.info`'s cage bars used `decay` (not a member) and were permanent; `decayat` now. `.admin` resurrect recoloured the staff member instead of the player. `.unconcealhim` compared `targ.cmdlvl` (an error; `error >= number` is always true) and refused everyone. `.untile` omitted the realm (the module default is britannia, the unused old map) and never removed anything. `.releaseinfo`'s Go buttons passed the force-location flag as the realm (new `ReleaseInfoGoTo`, word 7 of the record). `.newiteminfo` read the use-script descriptions from a package that does not exist (`:shilitems:` now). `.gorealm` accepts "x y" and "x y z" (realm defaults to the caller's, z from the map). `.resetpw` returns on an unknown account. `.setallskills` loops `GetSkillIds()` (skipped ids above 48). `.maxcaps` saved the number 20 as the whole power scroll matrix, collapsing every cap on the target to 130; it now writes a fresh 50-slot matrix of 20s (every cap 150) on a targeted player, tells both and logs. `.info`'s skill-editor guard (`skill>48 and skill<0`, never true) rejects ids not in `GetSkillIds()`. `.restartall` lists each realm once with `ListMobilesInBox` over `Realms()` (was ~123,000 near-location calls on the caller's realm only; NPCs only now). `.px`/`.py`/`.pz`/`.class` synopses corrected, `.pz` uses the object's realm, `.npclist` script-name typos fixed, two console prints removed.
- `.makemoongates` deleted: it built the classic 0x6002 gates at the Felucca town spots on the old map; the moongates package builds the real network at every start.
- `config/command_synopses.cfg` regenerated (344 commands; also catches up `.mounttest`'s level and `.updatetp`).

**Expected impact:** Staff tools do what their names say; a global no-loot lasts the minutes typed; thawed NPCs stay thawed; `.maxcaps` no longer wipes a player's scroll caps.

### 20. Alchemy Plus: Tamla bonus-tier potions as reagents (not part of the review)

**Files involved:** `pkg/opt/alchemyplus/alchemyplus.src`.

**Notable functional changes:** New `tamlafamily` (`0xff86`-`0xff8b`) and `TamlaUnitWorth(objtype)`: a True Mage bonus-tier Tamla Heal potion (`0xff87`-`0xff8b`) counts as 2 base Tamlas (`0xff86`) toward any recipe that calls for `0xff86`, in both the counting and the consuming pass, matching what `DoFullHeal()` hands back when one is drunk. A bonus Tamla can be spent directly on a Rebirth Potion without drinking it first.

**Expected impact:** Bonus-tier Tamlas are worth two base Tamlas in Alchemy Plus recipes.

### 21. Files deleted, dead code and records

**Files involved:** deleted `pkg/systems/combat/shilcombat.inc`, `pkg/std/spells/bless timer.src`, `pkg/std/spells/protection with timer.src`, `pkg/opt/vanityshop/include/mountFunctions.src`, `pkg/opt/necro/sunderingsword.src`, `pkg/opt/earth/shapechange.cfg`, `pkg/opt/zuluitems/dyecheck.src`, `scripts/textcmd/admin/makemoongates.src` (each with its compiled outputs where it had any); `ainotes/code-review-fixlog-20260921.md` and `.diff` (new); `.claude/skills/escript-gotchas/SKILL.md`, `.claude/subagent-briefing.md`; `patchnotes/*` for 3.1.1.

**Notable functional changes:** None in game. The escript-gotchas skill gained rules 16-24 (consume first then create, `SaveOnExit 0` for temporaries, no short-circuit evaluation, worn item container is the wearer, unset values compare equal, mobiles have no hp/mana/stamina/dexterity members, call dictionary methods with parentheses, never omit the realm argument, cprop names are case-sensitive), mirrored in the subagent briefing. Items the review found and left by decision are listed under "Left by your picks" in each fix-log section (16 in total), and two were parked for later: the wear-item hook (a crafted 0x13 plus an NPC with your `master` cprop lets `EquipItem` take any world item) and the dungeon-drop lookup keyed by objtype.

**Expected impact:** No player-visible effect.

---

## Validation Notes

- Diff range: `git log --graph 9aa5216..HEAD`, `git diff --shortstat 9aa5216..HEAD`, `git diff --name-status -M 9aa5216..HEAD` (the inventory above is that list verbatim), `git diff --numstat -M 9aa5216..HEAD` for the largest shifts, `git show --stat` on each of the four non-merge commits, and `git diff <merge>^1 <merge> --name-only` on the merges to confirm they carry nothing of their own.
- File-status counts (9 A / 281 M / 8 D / 1 R = 299) were derived programmatically from `git diff --name-status` and cross-checked against the printed inventory.
- The themes were written from `ainotes/code-review-fixlog-20260921.md` (the review's own line-level record, kept as each round was applied) rather than by re-reading the 13,646-line diff; the fix log names every file and line for each entry.
- Working tree was clean at the time of this writing (`git status` on `Patch-3.1.2` at `cdd3ef2`).
- Nothing in `cdd3ef2` was compiled or run by the reviewer. The `pol.cfg` debug flags (theme 2) are in the commit and should be reverted before a live build.
