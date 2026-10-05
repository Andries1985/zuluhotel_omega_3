# Developer Changelog - v3.1.3

Range: Patch-3.1.2..Patch-3.1.3 (commit `a4336ef`..`9164261`)
Branch: Patch-3.1.3
Date: 2026-10-05

---

## Scope Summary

- Total files changed: 252 (22 added, 197 modified, 12 deleted, 21 renamed) committed on `Patch-3.1.3`, plus 23 more in the uncommitted working tree (themes 31-36).
  - Commits `a4336ef..66b453b` (themes 1-18): 155 files, +76,416 / -57,960.
  - Commit `68a5c45` (themes 19-21, 2026-10-02): 44 files, +1,878 / -1,090.
  - Commit `9164261` (themes 22-30, 2026-10-03 and 2026-10-04): 71 files (70 modified, 1 added), +1,289 / -573. No compiled outputs are in the commit.
  - Working tree, uncommitted (themes 31-34, 2026-10-04 and 2026-10-05): 20 files (19 modified, 1 deleted), +724 / -2,038. The `itemutil.inc` audit; 1,601 of those deletions are the one dead file removed. Two new records under `ainotes/` are notes, not game content.
  - Working tree, uncommitted (theme 35, 2026-10-05): 1 file, `pkg/std/itemid/itemid.inc`, +27 / -9.
  - Working tree, uncommitted (theme 36, 2026-10-05): `scripts/include/itemutil.inc` (already counted above), `scripts/include/classes.inc`, `config/npcdesc.cfg` (comment only).
- Net textual delta: +79,463 / -59,503. Of that, 14,479 insertions are the purely additive `config/landtiles.cfg` registration (theme 5), about 6,900 changed lines are the `decoratefacets` door file renumbering after 71 removals (theme 5), and roughly +53,380 / -47,167 is the npcdesc.cfg block rewrite (theme 18, every line of every template moved). No binaries changed.
- Largest shifts:
  - `config/landtiles.cfg` (+14,479) - 1,385 new land tile definitions for the new dungeon terrain
  - `config/npcdesc.cfg` (+53,380 / -47,167 against `Patch-3.1.2`) - every one of the 1,455 template blocks rewritten into one layout (theme 18) on top of HITS/MANA/STAM on every template (commit `e0ff379`), 21 new quest-giver and boss templates, 6 Chaos Lord templates (commit `e0ff379`), dead `virtue`/`mountspawn`/`CustomHitsLevel` lines and two dead templates removed
  - `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (+3,012 / -3,864) - 71 stale door placements removed; the rest is index renumbering
  - `scripts/textcmd/gm/newiteminfo.src` (-2,674, deleted) and `scripts/textcmd/admin/iteminfo.src` (-351, deleted) - replaced by the promoted `.iteminfo`
  - `pkg/opt/rituals/config/itemdesc.cfg` (+905) - 28 new ritual altar items and 14 new chant books
  - `pkg/opt/quests/ai/sutek.src` (+528, new) - Soul Whisperer copy for the final ritual boss
  - `pkg/opt/quests/config/quests.cfg` (+342 / -22) - quests 12-25
  - `config/nlootgroup.cfg` (+253) - lootgroups 308-313 for the Chaos Lords (commit `e0ff379`)
  - `pkg/opt/quests/questlogs/RitualQuests.md` (+245, new) and `FishingQuests.md` (+113, new) - design records, not game content
  - `scripts/include/itemutil.inc` (+625 / -398, uncommitted) - the tree, ore and sand lists rebuilt from the resource configs, ten dead functions commented out, the crafting and class-restriction lists corrected (themes 31-34)
  - `pkg/utils/itemUtils/include/itemtypes.inc` (-1,601, deleted, uncommitted) - dead duplicate of six `itemutil.inc` helpers that nothing included (theme 34)
  - `scripts/include/teleporters.inc` (+213 / -55) - new dungeon network, coordinate fixes, Ter Mur links disabled, placement bug fix
  - `scripts/textcmd/gm/iteminfo.src` (+205 / -61, renamed from `alryciteminfo.src`) and `scripts/textcmd/seer/info.src` (+189 / -31) - staff tools
  - `scripts/include/virtue.inc` (-390), `scripts/ai/setup/questiesetup.inc` (-377), `scripts/ai/main/questiesetup.inc` (-365), `config/spawndef.cfg` (-541), `pkg/std/dundee/codex.cfg` (-206) - dead code removed (commit `e0ff379`)
  - `pkg/systems/accounts/include/accounts.inc` (+351 / -347, commit `68a5c45`) - login policy rework: the pending-login stamps and dead functions removed, both checks rewritten to judge from online characters only, household and DiscordID helpers added (theme 20)
  - `scripts/ai/animaltrainer.src` (+180 / -178, commit `9164261`) - the training code replaced by copies of the merchant versions (theme 28)
  - `pkg/std/inscription/inscription.src` (+137 / -1) - Bulk Order Book recipe (theme 23)
  - `scripts/include/ipban.inc` (+88, new) - IP ban matcher shared by the login scripts and `.ipban` (theme 27)
  - `pkg/std/fishing/magicfish.src` (+88 / -64) - Mystic Archer fish-scroll refusals keep the scroll (theme 25)
  - `scripts/include/classes.inc` (+46 / -24), `scripts/include/skillpoints.inc` (+30 / -2), `scripts/include/attributes.inc` (+49) - class skill-gain lookup, Warrior/Crafter stat affinity, `RaiseSkillCapsTo` (theme 22)
- Non-merge commits in range (oldest to newest):
  - `7b4c2c5` Animation test fix (alryc99, 2026-09-26) - `animationtest` template only (theme 8)
  - `d87c817` Teleporters and info newiteminfo updates (alryc99, 2026-09-27) - new dungeon teleporters, door decoration cleanup, staff tools, regen enchant naming (themes 4-7)
  - `8b0c9ff` Tele fixes (alryc99, 2026-09-27) - teleporter coordinates (theme 4)
  - `135ceb3` Final ritual quests created / Some ritual gating updates (alryc99, 2026-09-28) - quests 12-25, package rename, ritual gating (themes 1-3)
  - `941d8ba` Teleporter fixes (Sylvash, 2026-09-28) - teleporter coordinates (theme 4)
  - `e4fd8fc` Fixed doors and added new landtiles (Sylvash, 2026-09-28) - door items and land tiles (theme 5)
  - `85f13a2` Tele fixes (alryc99, 2026-09-29) - Ter Mur links, teleporter placement fix, briefing note (themes 4, 17)
  - `e0ff379` Patch Notes (alryc99, 2026-09-30) - vitals migration, Warrior for Hire vitals, Chaos Lord templates, karma sign, quest-giver snooping, dead NPC/config/virtue/mountspawn removals, staff-tool consolidation, these release notes (themes 6, 9-16)
  - `adca170` NPC hits changes (alryc99, 2026-09-30) - npcdesc script side: regen/snoop/bard fields, anchors, npcinfo, areaspawner, `.info` (theme 18)
  - `bceb357` Patch Notes (alryc99, 2026-09-30) - release notes for the above (theme 18)
  - `66b453b` NPCDESC spacing fixes (alryc99, 2026-09-30) - npcdesc.cfg rewritten into the canonical layout with space alignment, reformat report, briefing note (themes 17, 18)
  - `68a5c45` Accounts update patchnotes Housing recall fix (alryc99, 2026-10-02) - house-travel footprint test, login policy rework, skill arrows and verse book, these release notes (themes 19-21)
  - `9164261` Many fixes and bug squashing (alryc99, 2026-10-04) - class skill-gain lookup and caps, crafting fixes, spell and buff fixes, Ranger/pet/Mystic Archer work, Thief traps, IP bans, banker/escrow/vendor/trainer/guild fixes, `.redeed` lock-down, ritual crystals (themes 22-30)
- Merge commits: `ab0f893` (PR #110) and `e484311` (PR #111) re-merge `Patch-3.1.2`; the tree at `e484311` is identical to `a4336ef`. `5097add` (PR #112), `3b8b0fe`, `413aa02`, `4eb2b83` and `cff72b7` (PR #114) carry only the non-merge commits listed above.

**Before this build goes live:**
- **Quest progress datafile: decided 2026-10-05, not moved.** The quest package was renamed from `questpkg` to `quests` (theme 2), so the datafile path changed from `data/ds/questpkg/queststate.txt` to `data/ds/quests/queststate.txt`. The shard is in beta, so progress on quests 2-11 and the fishing quests 100-107 starts fresh; the patch notes say so.
- **Replace Valthor and Ysolde.** Their templates are gone (renamed `mariah` and `jaana`, theme 1) and `scripts/ai/valthor.src` is deleted. Any spawnpoint or placed NPC using `valthor` or `ysolde` must be re-placed with the new template.
- **Wipe and respawn NPCs** after the vitals migration (theme 9) and the npcdesc reformat (theme 18). There is no migration command. Existing NPCs keep their cached vitals and any old `CustomHitsLevel` / `BaseHpRegen` CProp is ignored; snoop/steal keep working on old NPCs through their old CProps. Areaspawner custom-NPC definitions convert their regen values on first load.
- **Old trapped chests stay inert.** Chests trapped before theme 26 carry the lowercase `trap_*` CProps that the trap script never read; Magic Untrap, Remove Trap and Detect Hidden still see them, but they do not fire. New traps use `Trap_Type` / `Trap_Strength` / `Trapped_By`.
- **IP bans:** the login scripts now read the datafile `bannedips`, the one `.ipban` writes (theme 27). Any ban that was saved in `:ipban:bannedips` by hand has to be copied across.
- **Characters wearing a newly restricted item will be stripped.** Theme 33 adds the Chaos and Order shields, female studded leather and the orc helm to the class restriction lists. `unequipRestrictedItems` (`classes.inc:926`) runs whenever a class level is recomputed, moves the item to the backpack and sends the standing message that threatens a jail - which reads as an accusation to a player who did nothing wrong. Consider an announcement, or hand-checking the affected characters, before the restart.
- **Resource yields open up.** Theme 31 makes the trees, ore and sand tiles added by the 2026-08-15 resource audit actually harvestable - 366 more tree objtypes, 70 more ore landtiles. The configs already allocated and regrew those units, so nothing needs regenerating, but gathering throughput across the world rises the moment this goes live.
- **Guild locks** (theme 28) now use the wall clock; stamps written by the old clock read as expired, so every guild can change its colour and name once right after the restart.
- The tree through theme 18 (with theme 19's tracing, before its fix) was compiled and run by the shard owner on 2026-10-01 for the house-travel test. Themes 19 (the fix) through 30 were written without a compile by the author; `ecompile` on the whole tree and an in-game pass over the "Expected impact" lines below are still owed. The temporary `traveldebug.inc` (`TRAVEL_DEBUG` 1) is still included by `recall.src`, `gate.src`, `mark.src` and `pkg/std/runebook/customspells.inc` until the house-recall re-test is done.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File` (A=added, M=modified, D=deleted, R=renamed as `old -> new`). The bracket lists the non-merge commits in the range that touched the file and the theme(s) below that describe it. Everything is committed; the working tree was clean at `9164261`.

- M | .claude/skills/escript-gotchas/SKILL.md (commits `e0ff379`, `68a5c45`, theme 20)
- M | .claude/subagent-briefing.md (commits `85f13a2`, `e0ff379`, `adca170`, `66b453b`, `68a5c45`, themes 19 and 20)
- M | ainotes/code-review-fixlog-20260921.md (commit `68a5c45`, theme 20)
- A | ainotes/npcdesc-reformat-report-2026-09-30.md (commits `adca170`, `66b453b`)
- M | config/command_synopses.cfg (commits `d87c817`, `68a5c45`, themes 19 and 20)
- M | config/equip.cfg (commits `135ceb3`, `e0ff379`)
- M | config/food.cfg (commit `9164261`, theme 25)
- M | config/itemdesc.cfg (commit `9164261`, theme 25)
- M | config/landtiles.cfg (commit `e4fd8fc`)
- M | config/mrcspawn.cfg (commit `9164261`, theme 26)
- M | config/nlootgroup.cfg (commit `e0ff379`)
- M | config/npcdesc.cfg (commits `7b4c2c5`, `135ceb3`, `e0ff379`, `adca170`, `bceb357`, `66b453b`, `9164261`, theme 25)
- D | config/spawndef.cfg (commit `e0ff379`)
- M | config/speechgroup.cfg (commit `e0ff379`)
- M | patchnotes/accounts_household_qa_runsheet.md (commit `68a5c45`, theme 20)
- A | patchnotes/developer-changelog-v3.1.3.md (commits `e0ff379`, `adca170`, `bceb357`, `66b453b`, `68a5c45`)
- M | patchnotes/developer-changelog.md (commits `e0ff379`, `adca170`, `bceb357`, `66b453b`, `68a5c45`)
- M | patchnotes/launchernotes.md (commits `e0ff379`, `adca170`, `bceb357`, `68a5c45`)
- A | patchnotes/patch-v3.1.3.md (commits `e0ff379`, `adca170`, `bceb357`, `68a5c45`)
- M | pkg/items/deed/commands/player/redeed.src (commit `9164261`, theme 29)
- M | pkg/items/doors/config/itemdesc.cfg (commit `e4fd8fc`)
- M | pkg/opt/alchemyplus/newpotions.src (commit `e0ff379`)
- M | pkg/opt/alryc/textcmd/player/classinfo.src (commit `9164261`, theme 22)
- A | pkg/opt/alryc/textcmd/test/regeninfo.src (commit `d87c817`)
- M | pkg/opt/areaspawner/config/areagroups.cfg (commit `e0ff379`)
- M | pkg/opt/areaspawner/include/areaspawner.inc (commits `e0ff379`, `adca170`, `bceb357`)
- M | pkg/opt/areaspawner/include/areaspawnergump.inc (commits `e0ff379`, `adca170`, `bceb357`)
- M | pkg/opt/champspawns/include/spawning.inc (commit `e0ff379`)
- M | pkg/opt/crafterboost/crafterboost.cfg (commit `9164261`, theme 23)
- M | pkg/opt/crafterboost/crafterboost_recipes.cfg (commit `9164261`, theme 23)
- M | pkg/opt/crafterboost/make_crafter_boosts.src (commit `9164261`, theme 23)
- M | pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg (commit `d87c817`)
- M | pkg/opt/earth/earthportal.src (commit `68a5c45`, theme 19)
- M | pkg/opt/earth/shapeshift.src (commit `9164261`, theme 24)
- M | pkg/opt/earth/summonmammals.src (commit `9164261`, theme 24)
- M | pkg/opt/guilds/commands/player/guilds.src (commit `9164261`, theme 28)
- M | pkg/opt/guilds/include/guildconstants.inc (commit `9164261`, theme 28)
- M | pkg/opt/holybook/angelicfeast.src (commit `9164261`, theme 24)
- M | pkg/opt/holybook/revive.src (commit `9164261`, theme 24)
- M | pkg/opt/ipban/textcmd/admin/ipban.src (commit `9164261`, theme 27)
- M | pkg/opt/necro/animatedead.src (commit `e0ff379`)
- M | pkg/opt/necro/liche.src (commit `9164261`, theme 24)
- A | pkg/opt/quests/ai/erethian.src (commit `135ceb3`)
- R | scripts/ai/fishmonger.src -> pkg/opt/quests/ai/fishmonger.src (commit `135ceb3`)
- R | scripts/ai/ysolde.src -> pkg/opt/quests/ai/jaana.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/julia.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/kalabar.src (commit `135ceb3`)
- R | scripts/ai/lobsterman.src -> pkg/opt/quests/ai/lobsterman.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/mariah.src (commit `135ceb3`)
- R | scripts/ai/nystul.src -> pkg/opt/quests/ai/nystul.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/rudyom.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/sutek.src (commit `135ceb3`)
- A | pkg/opt/quests/ai/thragg.src (commit `135ceb3`)
- R | pkg/opt/questpkg/config/icp.cfg -> pkg/opt/quests/config/icp.cfg (commit `135ceb3`)
- R | pkg/opt/questpkg/config/quests.cfg -> pkg/opt/quests/config/quests.cfg (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questdeath.inc -> pkg/opt/quests/include/questdeath.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questdebuggump.inc -> pkg/opt/quests/include/questdebuggump.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questfishing.inc -> pkg/opt/quests/include/questfishing.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questjournal.inc -> pkg/opt/quests/include/questjournal.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questmapgump.inc -> pkg/opt/quests/include/questmapgump.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questnpcgump.inc -> pkg/opt/quests/include/questnpcgump.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questpkg_gumpstyle.inc -> pkg/opt/quests/include/questpkg_gumpstyle.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/questspawncheck.inc -> pkg/opt/quests/include/questspawncheck.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/include/queststate.inc -> pkg/opt/quests/include/queststate.inc (commit `135ceb3`)
- R | pkg/opt/questpkg/pkg.cfg -> pkg/opt/quests/pkg.cfg (commit `135ceb3`)
- A | pkg/opt/quests/questlogs/FishingQuests.md (commit `135ceb3`)
- A | pkg/opt/quests/questlogs/RitualQuests.md (commit `135ceb3`)
- R | pkg/opt/questpkg/textcmd/gm/questcatch.src -> pkg/opt/quests/textcmd/gm/questcatch.src (commit `135ceb3`)
- R | pkg/opt/questpkg/textcmd/gm/questmap.src -> pkg/opt/quests/textcmd/gm/questmap.src (commit `135ceb3`)
- R | pkg/opt/questpkg/textcmd/gm/questtag.src -> pkg/opt/quests/textcmd/gm/questtag.src (commit `135ceb3`)
- R | pkg/opt/questpkg/textcmd/test/queststate.src -> pkg/opt/quests/textcmd/test/queststate.src (commit `135ceb3`)
- M | pkg/opt/rituals/altar/gump.inc (commit `135ceb3`)
- M | pkg/opt/rituals/altar/use.src (commit `135ceb3`)
- M | pkg/opt/rituals/config/itemdesc.cfg (commit `135ceb3`)
- M | pkg/opt/rituals/include/altarquest.inc (commit `135ceb3`)
- M | pkg/opt/rituals/include/rituals.inc (commits `135ceb3`, `9164261`, theme 30)
- M | pkg/opt/rituals/rituals/bloodSeeking.src (commit `135ceb3`)
- M | pkg/opt/rituals/rituals/hardening.src (commit `135ceb3`)
- M | pkg/opt/rituals/rituals/perilousTheurgy.src (commit `135ceb3`)
- M | pkg/opt/rituals/rituals/racialTheurgy.src (commit `135ceb3`)
- M | pkg/opt/rituals/rituals/resilience.src (commit `135ceb3`)
- M | pkg/opt/rituals/rituals/vitalInfusion.src (commit `135ceb3`)
- M | pkg/opt/rituals/scroll/use.src (commit `9164261`, theme 22)
- M | pkg/opt/songbook/songofdefense.src (commit `9164261`, theme 24)
- M | pkg/opt/songbook/songofdismissal.src (commit `9164261`, theme 24)
- M | pkg/opt/songbook/songoffright.src (commits `adca170`, `9164261`, theme 24)
- M | pkg/opt/spawnpoint/config/groups.cfg (commit `9164261`)
- M | pkg/opt/spawnpoint/include/customnpc.inc (commit `e0ff379`)
- M | pkg/opt/spawnpoint/spawnpoint.src (commit `135ceb3`)
- M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src (commit `e0ff379`)
- M | pkg/opt/summoning/polymorphing.src (commit `9164261`, theme 24)
- M | pkg/opt/summoning/processpoisonmod.src (commit `9164261`, theme 26)
- M | pkg/opt/summoning/summoning.src (commits `e0ff379`, `9164261`, theme 24)
- M | pkg/opt/versebook/include/versefunctions.inc (commit `68a5c45`, theme 21)
- M | pkg/opt/versebook/include/verseinfo.inc (commit `68a5c45`, theme 21)
- M | pkg/opt/versebook/Spirit_Flock.src (commit `9164261`, theme 24)
- A | pkg/opt/warriorforhire/include/wfhvitals.inc (commit `e0ff379`)
- M | pkg/opt/warriorforhire/warrior.src (commit `e0ff379`)
- M | pkg/opt/zuluitems/dragoneggs.src (commit `9164261`, theme 25)
- M | pkg/opt/zuluitems/ostardeggs.src (commit `9164261`, theme 25)
- M | pkg/opt/zuluitems/Testclassbooststone.src (commit `9164261`, theme 22)
- M | pkg/packethooks/megacliloc/mobiledata.src (commits `135ceb3`, `adca170`)
- M | pkg/packethooks/pickupitem/pickupitem.src (commit `9164261`, theme 29)
- M | pkg/std/blacksmithy/blacksmithy.cfg (commit `9164261`, theme 23)
- M | pkg/std/bowcraft/bowcraft.src (commit `9164261`, theme 23)
- M | pkg/std/bulkorders/bulkorders.cfg (commit `9164261`, theme 23)
- M | pkg/std/detecthidden/detecthidden.src (commit `9164261`, theme 26)
- D | pkg/std/dundee/codex.cfg (commit `e0ff379`)
- D | pkg/std/dundee/virtuewalkon.src (commit `e0ff379`)
- M | pkg/std/fishing/crustaceantrap.inc (commit `135ceb3`)
- M | pkg/std/fishing/fishing.inc (commit `135ceb3`)
- M | pkg/std/fishing/magicfish.src (commit `9164261`, theme 25)
- M | pkg/std/healing/healing.src (commit `9164261`, theme 25)
- M | pkg/std/healing/poison_heal.src (commit `9164261`, theme 26)
- M | pkg/std/hiding/hiding.src (commit `9164261`, theme 26)
- M | pkg/std/inscription/inscription.cfg (commit `9164261`, theme 23)
- M | pkg/std/inscription/inscription.src (commit `9164261`, theme 23)
- M | pkg/std/lockpicking/use/picklock.src (commit `9164261`, theme 26)
- M | pkg/std/peacemaking/peacemaking.src (commit `adca170`)
- M | pkg/std/runebook/customspells.inc (commit `68a5c45`, theme 19)
- M | pkg/std/runebook/runebookactions.inc (commit `68a5c45`, theme 19)
- M | pkg/std/snooping/snooping.src (commits `e0ff379`, `adca170`)
- M | pkg/std/snooping/stealme.cfg (commit `e0ff379`)
- M | pkg/std/spells/gate.src (commit `68a5c45`, theme 19)
- M | pkg/std/spells/gheal.src (commit `9164261`, theme 24)
- M | pkg/std/spells/heal.src (commit `9164261`, theme 24)
- M | pkg/std/spells/magictrap.src (commit `9164261`, theme 26)
- M | pkg/std/spells/magicuntrap.src (commit `9164261`, theme 26)
- M | pkg/std/spells/mark.src (commit `68a5c45`, theme 19)
- M | pkg/std/spells/polymorph.src (commit `9164261`, theme 24)
- M | pkg/std/spells/recall.src (commit `68a5c45`, theme 19)
- M | pkg/std/spells/resurrect.src (commit `9164261`, theme 24)
- M | pkg/std/spells/teleport.src (commit `68a5c45`, theme 19)
- M | pkg/std/stealing/stealing.src (commit `e0ff379`)
- M | pkg/std/tailoring/tailoringfunctions.inc (commit `9164261`, theme 23)
- M | pkg/std/taunt/taunt.src (commits `adca170`, `9164261`, theme 24)
- M | pkg/std/tinkering/tinkeringfunctions.inc (commit `9164261`, theme 26)
- M | pkg/std/veterinary/vet.src (commit `9164261`, theme 25)
- D | pkg/systems/accounts/commands/dev/eraseEmptyAccounts.src (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/config/settings.cfg (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/config/uopacket.cfg (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/hook/onLogin.src (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/include/accounts.inc (commit `68a5c45`, theme 20)
- D | pkg/systems/accounts/include/mailSystem.inc (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/logon.src (commit `68a5c45`, theme 20)
- M | pkg/systems/accounts/reconnect.src (commit `68a5c45`, theme 20)
- A | pkg/systems/attributes/config/uopacket.cfg (commit `68a5c45`, theme 21)
- M | pkg/systems/attributes/hooks/shilhook.src (commit `68a5c45`, theme 21)
- A | pkg/systems/attributes/hooks/skilllock.src (commit `68a5c45`, theme 21)
- M | pkg/systems/attributes/hooks/vitalInit.src (commit `e0ff379`)
- A | pkg/systems/attributes/include/npcvitals.inc (commits `e0ff379`, `adca170`, `bceb357`)
- M | pkg/systems/combat/config/modenchantdesc.cfg (commit `d87c817`)
- M | pkg/systems/combat/include/hitscriptinc.inc (commit `9164261`, theme 25)
- M | pkg/systems/crafting/include/craftmenu.inc (commit `9164261`, theme 23)
- M | pkg/systems/playervendor/commands/player/escrow.src (commit `9164261`, theme 28)
- M | pkg/systems/playervendor/playermerchant.src (commit `9164261`, theme 28)
- M | scripts/ai/aloof.src (commit `e0ff379`)
- M | scripts/ai/animal.src (commit `e0ff379`)
- M | scripts/ai/animaltrainer.src (commit `9164261`, themes 25, 28)
- M | scripts/ai/archerkillpcs.src (commit `e0ff379`)
- M | scripts/ai/assassinkillpcs.src (commit `e0ff379`)
- M | scripts/ai/banker.src (commit `9164261`, theme 28)
- M | scripts/ai/bardok.src (commit `e0ff379`)
- M | scripts/ai/barker.src (commit `e0ff379`)
- M | scripts/ai/barracoon.src (commit `e0ff379`)
- M | scripts/ai/chaosfirebreather.src (commit `e0ff379`)
- M | scripts/ai/chaoskillpcs.src (commit `e0ff379`)
- M | scripts/ai/chaosmultikillpcs.src (commit `e0ff379`)
- M | scripts/ai/chaosspellkillpcs.src (commit `e0ff379`)
- M | scripts/ai/chicken.src (commit `e0ff379`)
- M | scripts/ai/combat/chaosfight.inc (commit `e0ff379`)
- M | scripts/ai/combat/fight.inc (commit `e0ff379`)
- M | scripts/ai/critterhealer.src (commit `e0ff379`)
- M | scripts/ai/doppel.src (commit `e0ff379`)
- M | scripts/ai/doppelganger.src (commit `e0ff379`)
- M | scripts/ai/dragonking.src (commit `e0ff379`)
- M | scripts/ai/dumbkillpcs.src (commit `e0ff379`)
- M | scripts/ai/fastkillpcs.src (commit `e0ff379`)
- M | scripts/ai/firebreather.src (commit `e0ff379`)
- M | scripts/ai/goodcaster.src (commit `e0ff379`)
- M | scripts/ai/helppcs.src (commit `e0ff379`)
- M | scripts/ai/highpriest.src (commit `e0ff379`)
- M | scripts/ai/humuc.src (commit `e0ff379`)
- M | scripts/ai/immobile.src (commit `e0ff379`)
- M | scripts/ai/killany.src (commit `e0ff379`)
- M | scripts/ai/killpcs.src (commit `e0ff379`)
- M | scripts/ai/killpcsclock.src (commit `e0ff379`)
- M | scripts/ai/main/mainloopanimal.inc (commit `e0ff379`)
- M | scripts/ai/main/mainloopbarker.inc (commit `e0ff379`)
- M | scripts/ai/main/mainloopchicken.inc (commit `e0ff379`)
- M | scripts/ai/main/mainloopsheep.inc (commit `e0ff379`)
- M | scripts/ai/main/npcinfo.inc (commit `adca170`)
- D | scripts/ai/main/questiesetup.inc (commit `e0ff379`)
- M | scripts/ai/meek.src (commit `e0ff379`)
- M | scripts/ai/merchant.src (commit `9164261`, theme 28)
- M | scripts/ai/poisonkillpcs.src (commit `e0ff379`)
- M | scripts/ai/quagkillpcs.src (commit `e0ff379`)
- M | scripts/ai/rikktor.src (commit `e0ff379`)
- M | scripts/ai/setup/modsetup.inc (commits `e0ff379`, `adca170`)
- D | scripts/ai/setup/questiesetup.inc (commit `e0ff379`)
- M | scripts/ai/sheep.src (commit `e0ff379`)
- M | scripts/ai/slime.src (commit `e0ff379`)
- M | scripts/ai/spellkillpcs.src (commit `e0ff379`)
- M | scripts/ai/spiderlord.src (commit `e0ff379`)
- M | scripts/ai/spiders.src (commit `e0ff379`)
- M | scripts/ai/tamed.src (commits `e0ff379`, `9164261`, theme 25)
- M | scripts/ai/townguard.src (commit `e0ff379`)
- M | scripts/ai/triwolf.src (commit `e0ff379`)
- D | scripts/ai/valthor.src (commit `135ceb3`)
- M | scripts/ai/vortexai.src (commit `e0ff379`)
- M | scripts/ai/waterdragonking.src (commit `e0ff379`)
- D | scripts/CustomHpFix.src (commit `e0ff379`)
- M | scripts/include/anchors.inc (commit `adca170`)
- M | scripts/include/attributes.inc (commits `d87c817`, `e0ff379`, `9164261`, theme 22)
- M | scripts/include/chests.inc (commit `9164261`, theme 26)
- M | scripts/include/classes.inc (commit `9164261`, theme 22)
- M | scripts/include/constants/npcai.inc (commit `e0ff379`)
- M | scripts/include/dotempmods.inc (commits `d87c817`, `9164261`, theme 24)
- A | scripts/include/housetravel.inc (commit `68a5c45`, theme 19)
- A | scripts/include/ipban.inc (commit `9164261`, theme 27)
- M | scripts/include/itemutil.inc (commit `9164261`, theme 23)
- M | scripts/include/namingbyenchant.inc (commit `d87c817`)
- A | scripts/include/npcdifficulty.inc (commits `adca170`, `bceb357`)
- M | scripts/include/skillpoints.inc (commits `68a5c45`, `9164261`, theme 21; theme 22)
- M | scripts/include/speech.inc (commit `e0ff379`)
- M | scripts/include/spelldata.inc (commit `9164261`, theme 24)
- M | scripts/include/teleporters.inc (commits `d87c817`, `8b0c9ff`, `941d8ba`, `85f13a2`)
- A | scripts/include/traveldebug.inc (commit `68a5c45`, theme 19)
- D | scripts/include/virtue.inc (commit `e0ff379`)
- M | scripts/misc/death.src (commit `135ceb3`)
- M | scripts/misc/logoff.src (commit `68a5c45`, theme 20)
- M | scripts/misc/logon.src (commits `68a5c45`, `9164261`, theme 20; theme 27)
- M | scripts/misc/questbutton.src (commit `135ceb3`)
- M | scripts/misc/reconnect.src (commits `68a5c45`, `9164261`, theme 20; theme 27)
- M | scripts/start.src (commit `e0ff379`)
- M | scripts/textcmd/admin/admin.src (commit `e0ff379`)
- M | scripts/textcmd/admin/akill.src (commit `d87c817`)
- M | scripts/textcmd/admin/destroyradius.src (commit `d87c817`)
- D | scripts/textcmd/admin/iteminfo.src (commit `d87c817`)
- M | scripts/textcmd/admin/setclass.src (commit `9164261`, theme 22)
- A | scripts/textcmd/admin/setdiscord.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/admin/spellbook.src (commit `68a5c45`, theme 19)
- R | pkg/opt/alryc/textcmd/test/alryciteminfo.src -> scripts/textcmd/gm/iteminfo.src (commit `d87c817`)
- D | scripts/textcmd/gm/newiteminfo.src (commit `d87c817`)
- M | scripts/textcmd/player/skills.src (commit `68a5c45`, theme 21)
- M | scripts/textcmd/seer/info.src (commits `d87c817`, `e0ff379`, `adca170`)
- M | scripts/textcmd/test/accountpolicyinfo.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/test/extralogin.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/test/householdadd.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/test/householdcap.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/test/householdmanager.src (commit `68a5c45`, theme 20)
- M | scripts/textcmd/test/householdremove.src (commit `68a5c45`, theme 20)

---

## Detailed Changes By Theme

### 1. Ritual quests 12-25: all 24 rituals now have a quest line (commit `135ceb3`)

**Files involved:**
- `pkg/opt/quests/config/quests.cfg` (quests 12-25 added; `Giver`/`GiverName` on quests 2-8 updated)
- `pkg/opt/quests/ai/{mariah,rudyom,erethian,thragg,julia,kalabar,sutek}.src` (new), `pkg/opt/quests/ai/jaana.src` (renamed from `scripts/ai/ysolde.src`), `scripts/ai/valthor.src` (deleted), `pkg/opt/quests/ai/{nystul,fishmonger,lobsterman}.src` (moved from `scripts/ai/`)
- `pkg/opt/rituals/config/itemdesc.cfg` (28 altar items as 14 ew/ns pairs, 14 chant books)
- `pkg/opt/rituals/altar/gump.inc` (reagent names), `pkg/opt/rituals/include/altarquest.inc`, `pkg/opt/rituals/altar/use.src` (path and comment updates)
- `config/npcdesc.cfg` (committed part: 21 new templates, `valthor` and `ysolde` removed), `config/equip.cfg` (committed part: `Equipment kalabar`)
- `pkg/opt/quests/questlogs/RitualQuests.md`, `FishingQuests.md` (new design records)

**Notable functional changes:**
- Quests 2-11 existed before this range. Quests 12-25 are new, one per remaining ritual, each a single-boss kill: offer four reagents (100 each) at a two-piece altar, Summon spawns the template in the altar's `RitualAltarBossTemplate`, the boss drops the shared `QuestSkull` trophy (`0xBA3F`) tagged with the quest id, and the giver pays `RewardGold` plus a fixed chant book (`RewardItem`). All non-repeatable; abandon-and-retry stays available.
- New givers (all `invul`, `noloot 1`, `CProp QuestGiverTag s<name>`, shared quest-giver AI shape: "quest" speech opens the gump, periodic shout to nearby players with an open quest):

  | Quests | Giver | Placed | Dungeon | Bosses (real HP) |
  |---|---|---|---|---|
  | 12-15 | Cinderwarden Rudyom (`rudyom`) | Faymoor | Brimstone Catacombs of Alryc, Magma Halls, Hellfire Abyss, Inferno | Korrath 25,000; Vraxthel 35,000; Ashkelon 50,000; Nethrazul 70,000 |
  | 16-17 | Inquisitor Erethian (`erethian`) | near Bayfort | Necropolis of Nagash | Malachar 75,000; Vharesh 90,000 |
  | 18-21 | Stonewarden Thragg (`thragg`, ogre body) | Mountain Town | Rikktor's Lair | Argentyr 40,000; Aurelian 50,000; Maelithra 60,000; Zorgathrax 80,000 |
  | 22-24 | Warden Julia (`julia`) | Erilyn | Bonecrusher Stronghold | Grukthar 10,000; Mogrash 12,500; Krugnak 16,000 (melee ogres, `Boss` tier only) |
  | 25 | Kalabar (`kalabar`, gargoyle, black robe and staff) | Sunken City | Tartarus | Sutek, the Serpent's Fang 80,000 |

- Renames on existing lines: `valthor` -> `mariah` (Arch-Mage Mariah, Emissary; now female, objtype `0x191`; `QuestGiverTag` `svalthor` -> `smariah`) for quests 2-5; `ysolde` -> `jaana` (Frostkeeper Jaana, Emissary; `YSOLDE_SHOUT_RANGE` -> `JAANA_SHOUT_RANGE`) for quests 6-8. `quests.cfg` `Giver`/`GiverName` updated to match. Nystul's AI moved into the package; the three giver templates now use fully qualified `:quests:ai/<name>` script paths.
- `sutek.src` is `scripts/ai/soulwhisperer.src` with only the random name override in `KillPlayers()` removed, so the 75%/50%/25% portal summons (a random extra superboss from the shared 36-entry pool, up to three) are kept. `soulwhisperer.src` is untouched.
- `altar/gump.inc` gained display names for the six reagents the new quests use: Brimstone, Dragon's Blood, Volcanic Ash, Daemon Bone, Executioner's Cap, Bone.
- The ten pre-existing boss templates are unchanged apart from their long lore comments moving to `RitualQuests.md`.

**Expected impact:** Every ritual chant book is obtainable through a quest. Six new giver NPCs must be placed in the world (`.questmap` finds placed altars and givers). Existing Valthor/Ysolde NPCs stop working; see the deployment note in the Scope Summary.

### 2. Quest package renamed `questpkg` -> `quests` (commit `135ceb3`)

**Files involved:** every file under `pkg/opt/questpkg/` (renamed to `pkg/opt/quests/`), plus the external includers `scripts/misc/death.src`, `scripts/misc/questbutton.src`, `pkg/std/fishing/fishing.inc`, `pkg/std/fishing/crustaceantrap.inc`, `pkg/packethooks/megacliloc/mobiledata.src`, `pkg/opt/spawnpoint/spawnpoint.src`, `pkg/opt/rituals/altar/gump.inc`, `pkg/opt/rituals/altar/use.src`, `pkg/opt/rituals/include/altarquest.inc`.

**Notable functional changes:**
- `pkg.cfg` `Name questpkg` -> `Name quests`; every `include ":questpkg:..."` -> `":quests:..."`; `ReadConfigFile(":questpkg:quests")` -> `":quests:quests"`. The renamed includes differ only in paths and comments.
- `QUESTPKG_DATAFILE` in `queststate.inc`: `":questpkg:queststate"` -> `":quests:queststate"`, so the on-disk datafile moves from `data/ds/questpkg/` to `data/ds/quests/`.
- Fishing quests 100-107 are unchanged in content.

**Expected impact:** No behaviour change once the datafile is moved. Without the move, all saved quest state (quests 2-11 and 100-107) is lost on the next boot. Quests 101 and 105 (Rare tier) reward the Legendary lure; that was already the case and is recorded in `FishingQuests.md`.

### 3. Ritual enchant gating: hitscripts and physical mods may coexist (commit `135ceb3`)

**Files involved:** `pkg/opt/rituals/include/rituals.inc`, `pkg/opt/rituals/rituals/{bloodSeeking,hardening,resilience,vitalInfusion,perilousTheurgy,racialTheurgy}.src`

**Notable functional changes:**
- `RitualHasGroup3Property(item)` (damage/AR/HP/stat/ArBonus mods) deleted from `rituals.inc`. Its premise that hitscript enchants and physical mods were mutually exclusive was never true in `starteqp.inc` loot generation.
- `RitualHasHitscript(item)` check removed from Blood Seeking (`dmg_mod`), Hardening (`ar_mod`), Resilience (`maxhp_mod`) and Vital Infusion's armor branch (stat bonus).
- `RitualHasGroup3Property(item)` check removed from Perilous Theurgy and Racial Theurgy (Fury and Slayer hitscripts).
- Kept: one hitscript per item (`RitualHasHitscript` still gates the two theurgy rituals), Vital Infusion's jewel skill-vs-stat exclusivity, and the same-category strength thresholds (`dmg_mod` 30, `ar_mod` 6, HP 30).

**Expected impact:** Items that already carry Fury/Slayer/poison can take Blood Seeking, Hardening, Resilience or Vital Infusion, and items with a damage/AR/durability/stat mod can take a theurgy ritual. This is a real itemization loosening.

### 4. Teleporters: new dungeon network, coordinate fixes, Ter Mur links, placement fix (commits `d87c817`, `8b0c9ff`, `941d8ba`, `85f13a2`)

**Files involved:** `scripts/include/teleporters.inc`

**Notable functional changes:**
- Active entries 1,965 -> 2,088; commented-out entries 16 -> 42.
- New links (`d87c817`, block `//Nagash Added Sept 26 2026`): Ziggurat of Quetzalcohuatl (entrance, levels 1-4, Burial Chamber), The Crimson Crucible (levels 1-2), Drakon's Reach, Hasseth's Fortress <-> Hasseth's Necropolis, Bonecrusher Stronghold (Ogre Cave, levels 1-3, Throneroom, plus a cave shortcut to level 2), Necropolis of Nagash (from the Great Pyramid, internal room links, Boss Room, two cave exits), Secret Retreat internal hops, the Alryc chain (Aetherfall Bridge -> Brimstone Catacombs -> Magma Halls -> Hellfire Abyss -> Inferno), and Poison Dungeon level 1 -> Boss Room.
- Coordinate fixes: Sunken City Hub -> Island X 1850 -> 1849; Faymoor Island -> Solen Hive Y 2531 -> 2530; Runebound Sanctum level 2 -> 1 `{6650/6653,1436,5}` -> `{…,1435,2}`; Sosaria -> Mine X 5902 -> 5901 (two entries); Sosaria -> Cave X 5590 -> 5589; Black City of the Damned -> Tartarus was a self-loop `{951,2885,35}` -> `{951,2885,35}` and now lands at `{5529,3871,0}`. A duplicate Shandalaar Blood Sewers entry, duplicate Faymoor <-> Aetherfall Mine entries and 12 dead Mine/Underworld/Wayfarer's Abyss entries were removed.
- All 26 Ter Mur <-> britannia_alt links commented out (`85f13a2`): 14 working one-way exits from Ter Mur `{1125-1131,1215}` -> `{4194/4195,3261}`; 4 exits `{511-514,584}` -> `{6992/6993,1367}` whose destination was mistagged `termur` on 2026-09-23 (X beyond Ter Mur's 1,280 width); 8 britannia_alt entrances `{4194/4195,3260}` and `{7149-7153,756}` that were tagged `termur` as source and never created. The commented copies carry corrected realm tags.
- `CreateTeleporters()`: `GetStandingHeight()` returns a struct and was assigned straight to `fromZ`, so the fallback placement always failed; it now uses `.z`. A `TypeOf(fromZ) == "Error"` bail-out stops the up-to-100 x 100 ms retry loop on coordinates outside the realm.

**Expected impact:** About a dozen new dungeon areas are reachable. Several teleporters land on the right tile. Ter Mur has no teleporter connection to the main map at all, including the exit that worked in 3.1.2. Teleporters that needed the height fallback now get created, and boot no longer burns seconds on invalid coordinates.

### 5. Doors and land tiles (commits `e4fd8fc`, `d87c817`)

**Files involved:** `pkg/items/doors/config/itemdesc.cfg` and `config/landtiles.cfg` (`e4fd8fc`, Sylvash); `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (`d87c817`)

**Notable functional changes:**
- 10 new `Door` entries: Wallset1 doors `0x41CF`/`0x41D1` (south) and `0x41D3`/`0x41D5` (east), and `0x31A0`, `0x31A2`, `0x31A4`, `0x31A6`, `0x31A8`, `0x31AA`, each with `OpenGraphic`, `Lockable 1`, `DoorType Wood`. The "UNVERIFIED" note on the QC Wall Door b pair (`0x5128`/`0x5129`) is removed.
- `landtiles.cfg`: 2,685 -> 4,070 entries, additive only. Ranges `0x32C8-0x3679`, `0x367B-0x3742`, `0x374B-0x37CC`, `0x37DA-0x3815`, `0x3866-0x3895`, plus `0x563`. By name: 1,336 `decimated_*`, 43 snow, 4 crystal, 1 stone paver, 1 dirt. 989 are `MoveLand 1`, 360 `Blocking 1`.
- `decoratefacets` doors: 2,991 -> 2,920 placements; 71 removed, none added. All are `0x675`/`0x677`/`0x67D`/`0x67F` at Z 0, 68 inside the Quetzalcohuatl footprint (X 6200-6290, Y 1640-1860) and 3 inside Bonecrusher Stronghold (X 7046-7075, Y 786-830). The large line diff is `Decor N` renumbering.

**Expected impact:** The new door graphics open, close and lock. Terrain in the new dungeons walks and blocks correctly. Stray doors inside the rebuilt dungeon areas stop being redecorated.

### 6. Staff tools (commit `d87c817`)

**Files involved:** `scripts/textcmd/gm/iteminfo.src` (renamed from `pkg/opt/alryc/textcmd/test/alryciteminfo.src`), `scripts/textcmd/admin/iteminfo.src` (deleted), `scripts/textcmd/gm/newiteminfo.src` (deleted), `pkg/opt/alryc/textcmd/test/regeninfo.src` (new), `scripts/textcmd/admin/akill.src`, `scripts/textcmd/admin/destroyradius.src`, `scripts/textcmd/seer/info.src` (committed part), `config/command_synopses.cfg`

**Notable functional changes:**
- `.iteminfo` is now the former `.alryciteminfo`, at Game Master (CmdLevel 3; the old `.iteminfo` was Administrator 4). It replaces both the old `.iteminfo` and `.newiteminfo`. New "Tile Info" page 20 (serial, objtype, graphic, color in decimal and hex; tiles.cfg description, layer, height, weight, UoFlags and named flags). The Regeneration Mods page 11 is back as staff-only (HpRegen/ManaRegen/StamRegen, 200-1200). `ClearOtherElementalGroup`, `ClearOtherChargeImmunity` and `ClearOtherPermanentImmunity` merged into `ClearGroupOneMods` over all 18 Group 1 properties, matching `starteqp.inc`'s single enchant slot.
- `.regeninfo` (Developer): current/max, live regen rate and every named regen modifier per vital.
- `.akill`: one pass over `ListMobilesNearLocationEx` with `LISTEX_FLAG_NPC_ONLY` plus normal/hidden/concealed. Before, a player or higher-ranked mobile in range made the whole command return with nothing killed. Reports "Killed N NPC(s), closed M player vendor(s)."
- `.destroyradius`: the spawnpoint guard compared against `0xa301` (the old objtype); live spawnpoints are `0xa300`, so it destroyed them. Now skips `0xa300` and spawn triggers `0x35000`, and reports counts.
- `.info`: account `Release` read guarded for NPCs; duplicate `frozen` erase removed; the resurrect button now calls `ResurrectMobile()` (fixes Orc and Frost Elf skin color, adds the res-protection window and clears stale regen mods); graphic/color shown in decimal and hex; new Status page 6 (realm, serial, template, class, karma/fame, flags, regen rates and modifiers, AR, protections, immunities, owner); Throwing (skill 57) row added.
- `command_synopses.cfg` regenerated for the above.

**Expected impact:** Staff only, apart from the `.info` resurrect color fix for Orcs and Frost Elves. Spawnpoints missing from past `.destroyradius` use may be worth a spot check.

### 7. Regeneration enchant names (commit `d87c817`)

**Files involved:** `pkg/systems/combat/config/modenchantdesc.cfg`, `scripts/include/namingbyenchant.inc`, `scripts/include/attributes.inc`, `scripts/include/dotempmods.inc`

**Notable functional changes:**
- `MiscEn HpRegen`/`ManaRegen`/`StamRegen` had every field blank; they now have `Place 2`, colors 37/89/68 and six blessed and six cursed tier names (e.g. "of Mending" to "of the Phoenix").
- `SetNameByEnchant()` regen tier is `value/200` (was `value/100`), clamped to 1-6.
- `SetHpRegenRate()`, `SetManaRegenRate()`, `SetStaminaRegenRate()`, `CPROP_NAME_PREFIX_REGEN_RATE` (`attributes.inc`) and `BaseRegenRate()` (`dotempmods.inc`) commented out; none had callers after the regen port.

**Expected impact:** Staff-made regen items get a proper name suffix. No loot path grants these properties.

### 8. `animationtest` template (commit `7b4c2c5`)

**Files involved:** `config/npcdesc.cfg`

**Notable functional changes:** `script sheep` removed and `Color`/`TrueColor` 33784 -> 0, so the test NPC stands still and Body.def hue remaps apply.

**Expected impact:** No player-visible effect.

### 9. NPC vitals come from template HITS/MANA/STAM (commit `e0ff379`)

**Files involved:** `config/npcdesc.cfg`, `pkg/systems/attributes/hooks/vitalInit.src`, `pkg/systems/attributes/include/npcvitals.inc` (new), `scripts/ai/setup/modsetup.inc`, `scripts/CustomHpFix.src` (deleted), `scripts/start.src`, `pkg/opt/areaspawner/include/areaspawner.inc`, `pkg/opt/spawnpoint/include/customnpc.inc`, `pkg/opt/spawnpoint/textcmd/admin/newmobedit.src`, `scripts/textcmd/seer/info.src` (`e0ff379` part)

**Notable functional changes:**
- All 1,456 templates carry `HITS`, `MANA` and `STAM` (plain numbers; dice strings supported). HITS = STR + BaseStrmod, MANA = INT + BaseIntmod, STAM = DEX + BaseDexmod, except templates that had a `CProp CustomHitsLevel`, whose value was carried over exactly (e.g. `chiefparoxysmus` 2,000,000 -> `HITS 20000`), and `beckon` (HITS 200 -> 300). All 141 live template `CustomHitsLevel` CProps (and one commented-out line) are removed.
- Deliberate exceptions: `pixie` 100 -> 225, `dryad` 130 -> 225, `fairy` 100 -> 400 (formula instead of their old cap).
- `vitalInit.src`: the `CustomHitsLevel`/`CustomManaLevel`/`CustomStaminaLevel` branches, the `firstTimeCustomHp` flag and the `CustomHPFix` global are gone. `NpcVitalOrFallback()` uses the template field or, when missing, attribute x 100 with one console warning per template and field.
- `npcvitals.inc` (new): `GetNpcVitalSetting` (moved from `vitalInit.src`), `SetNpcVitalOverride`, `GetNpcVitalOverride`, `ClearNpcVitalOverride`, `GetNpcVitalTemplateValue`. Overrides live in the existing `RolledNpcVitals` dictionary CProp (x100). The cache stores every resolved value, so template edits reach only NPCs spawned afterwards.
- `modsetup.inc`: at AI start the NPC is filled to `GetMaxHp`/`GetMaxMana`/`GetMaxStamina` (was set to Str/Int/Dex). `scripts/CustomHpFix.src` and its `start.src` launcher are removed.
- Consumers: areaspawner custom-NPC overrides and `newmobedit` write `SetNpcVitalOverride`; `.info` Status shows "HITS / MANA / STAM" as template value plus live value when they differ.

**Expected impact:** Monster max HP, mana and stamina are unchanged except the three creatures above. NPC maximums no longer move with Strength/Intelligence/Dexterity buffs or curses. Spawnpoints saved from an NPC with a hand-set `CustomHitsLevel` fall back to the template HITS; re-apply the value through `newmobedit` and re-save the point to keep it.

### 10. Warrior for Hire vitals (commit `e0ff379`)

**Files involved:** `pkg/opt/warriorforhire/include/wfhvitals.inc` (new), `pkg/opt/warriorforhire/warrior.src`, `scripts/ai/highpriest.src`

**Notable functional changes:**
- `SetWfhVitalCeilings()` overrides HITS = 2 x base Str, MANA = base Int, STAM = base Dex (temporary mods subtracted). `SetWfhVitalCeilingsAndFill()` also tops the vitals up.
- `SetMeUp()` (hire) calls the fill version (was `SetHP(me, GetMaxHP(me))`); `GainStat()` (hourly) re-stamps ceilings without a heal; both High Priest restore paths re-stamp and fill.

**Expected impact:** A fresh hire has 150 HP, 50 mana, 50 stamina (base 75/50/50); before, max HP equalled Strength (75). Max HP rises 2 per Strength point gained.

### 11. Chaos Lords are real templates (commit `e0ff379`)

**Files involved:** `config/npcdesc.cfg`, `config/nlootgroup.cfg`, `scripts/ai/chaosmultikillpcs.src`

**Notable functional changes:**
- New templates `archangellord`, `nemesislord`, `plaguelord`, `scourgeterminatorlord`, `scourgeinfiltratorlord`, `scourgebattlemasterlord`: STR/INT/DEX doubled, HITS/MANA/STAM the sum of both bases (20,000 HP), `CProp merged i1`, `CProp BaseTemplate s<base>`, `MagicItemChance 99`, `MagicItemLevel 9`, lootgroups 308-313 (copies of 106, to be tuned).
- `MakeLord()` spawns `<npctemplate>lord` and kills both originals (no loot); a missing template prints to the console and cancels the merge. `SplitLord()` splits into `BaseTemplate`. The old in-place stat doubling, which double-counted BaseStrmod, is gone.
- The six base templates are the only ones using `chaosmultikillpcs`, and each has its Lord. Lords carry `merged`, so they do not merge again.

**Expected impact:** A merged Lord is its own creature with its own loot table; a split Lord returns two base creatures.

### 12. NPC karma sign follows alignment (commit `e0ff379`)

**Files involved:** `scripts/ai/setup/modsetup.inc`

**Notable functional changes:** the karma `case` read `.alignmodnpcnt`, which no template has, so every NPC took the negative default. It reads `.alignment`: `evil` and unknown values stay negative, `good` is positive. Runs only when an NPC has no `Karma` CProp yet.

**Expected impact:** The 105 `alignment good` templates now have positive karma. Killing one gives no karma gain and can cost karma, per `AffectKarmaAndFameForKill()`. Fame is unchanged. Existing NPCs keep their old karma until respawned.

### 13. Quest givers cannot be snooped or stolen from (commit `e0ff379`)

**Files involved:** `pkg/std/snooping/snooping.src`, `pkg/std/stealing/stealing.src`

**Notable functional changes:** both refuse a victim with a `QuestGiverTag` CProp ("You can't snoop quest givers.", "You can't steal from quest givers."); the stealing check is a backstop behind the snoop check.

**Expected impact:** Quest-giver packs are off limits.

### 14. Dead NPC types and configs removed (commit `e0ff379`)

**Files involved:** `config/npcdesc.cfg`, `config/equip.cfg` (`e0ff379` part), `config/speechgroup.cfg`, `pkg/std/snooping/stealme.cfg`, `scripts/include/speech.inc`, `scripts/include/constants/npcai.inc`, `scripts/ai/setup/questiesetup.inc`, `scripts/ai/main/questiesetup.inc`, `config/spawndef.cfg`

**Notable functional changes:**
- `NpcTemplate questie` (its `script questie` never existed) and `NpcTemplate quest_target` removed, with `Equipment quest_target`, `speechlink quest_target`, the `questie` snoop-loot block, `NPCAI_QUESTIE`, both `questiesetup.inc` files, and the `slave`-CProp branch plus `GiveQuestieDirections()` in `speech.inc`. Nothing spawned or included any of them. `Quest_targetWeapon` stays (used by `peasant`).
- `config/spawndef.cfg` deleted; it had no readers since the initial commit.

**Expected impact:** No gameplay change.

### 15. Virtue system removed (commit `e0ff379`)

**Files involved:** `scripts/include/virtue.inc` (deleted), `pkg/std/dundee/virtuewalkon.src` and `pkg/std/dundee/codex.cfg` (deleted), `scripts/ai/townguard.src`, `scripts/textcmd/admin/admin.src`, `scripts/textcmd/seer/info.src`, `config/npcdesc.cfg`

**Notable functional changes:**
- `virtue.inc` (Dundee virtue points, titles, Virtue Guard recruiting, honor/censure) had no callers here or in POL2.5; deleted with the unreferenced shrine walk-on and its codex text.
- `townguard.src`: `SetMeUp()` no longer rolls the 1-in-5 Virtue Guard variant (guardtype CProp, Order `0x86EF` or Chaos `0x86DF` shield, longsword, random mount); `FixStuff()` always titles "the Guard".
- `admin.src`/`info.src`: OVG/CVG and Order/Chaos name prefixes removed.
- `npcdesc.cfg`: 181 `virtue` field lines removed (never read).
- The Order/Chaos shield items and their combat checks stay; townstone upgrades still list those graphics.

**Expected impact:** All town guards spawn on foot with standard gear. Existing Virtue Guards keep their gear until they respawn.

### 16. Dead `mountspawn` CProp removed (commit `e0ff379`)

**Files involved:** `config/npcdesc.cfg`

**Notable functional changes:** 23 `CProp mountspawn` lines removed (21 live, 2 commented), plus the copies inside the six new Lord templates. Nothing read it. Mounts come from the `mount` field read by `archersetup.inc`, `killpcssetup.inc` and `spellsetup.inc`; the three templates that named a real mount (`brigandranger`, `darkranger`, `darkmage`) already had a `mount` line.

**Expected impact:** No gameplay change.

### 17. Developer notes (commits `85f13a2`, `e0ff379`, `66b453b`, `68a5c45`)

**Files involved:** `.claude/subagent-briefing.md`, `.claude/skills/escript-gotchas/SKILL.md`

**Notable functional changes:** realm map sizes and the "Felucca means britannia_alt" rule added (`85f13a2`); NPC stat-limit note (`e0ff379`); the npcdesc.cfg canonical-layout convention from theme 18 (`66b453b`); (commit `68a5c45`, 2026-10-02) the house-travel rule from theme 19 and the no-inline-comment rule for npcdesc.cfg; (commit `68a5c45`, theme 20) `foreach` over a dictionary yields its values, what a packet hook's return value means, the order and run-to-completion nature of the logon scripts, and where the login policy lives. The last four are also gotchas 27 and 28 in the escript-gotchas skill.

**Expected impact:** No player-visible effect.

### 18. npcdesc.cfg reformat: one layout for every template, CProps promoted to fields (commits `adca170`..`66b453b`, 2026-09-30)

**Files involved:** `config/npcdesc.cfg`, `ainotes/npcdesc-reformat-report-2026-09-30.md` (new), `scripts/include/npcdifficulty.inc` (new), `scripts/include/anchors.inc`, `scripts/ai/main/npcinfo.inc`, `scripts/ai/setup/modsetup.inc`, `pkg/systems/attributes/include/npcvitals.inc`, `pkg/std/peacemaking/peacemaking.src`, `pkg/opt/songbook/songoffright.src`, `pkg/std/taunt/taunt.src`, `pkg/std/snooping/snooping.src`, `pkg/packethooks/megacliloc/mobiledata.src`, `pkg/opt/areaspawner/include/areaspawner.inc`, `pkg/opt/areaspawner/include/areaspawnergump.inc`, `scripts/textcmd/seer/info.src`, `.claude/subagent-briefing.md`

**Notable functional changes:**
- All 1,455 `NpcTemplate` blocks rewritten by script into one order (identity, behaviour, bard/snoop fields, stats, skills, built-in weapon, casting, loot, CProps), one spelling per key (3,136 field keys and 169 skill lines respelt: `MaceFighting`, `EvaluatingIntelligence`, `DetectingHidden`, aliases such as `Swords`/`Resist`/`EvalInt` expanded), one tab indent then space padding (value 28 characters after the key start, CProp name at 28, CProp value at 52 - aligned at any tab width), blank lines only between groups (4,407 stray ones removed), skills and spells alphabetical inside their groups. Text between blocks is untouched; the commented reference template at the top of the file is regenerated in the same order. A parity check confirmed every kept entry survived with its value. Full record: `ainotes/npcdesc-reformat-report-2026-09-30.md`.
- Promoted from CProps to fields: `snoopme`/`stealme` (1,060 templates) and `HITSREGEN`/`MANAREGEN` (149 / 77, from `BaseHpRegen`/`BaseManaRegen`; unit is points per minute = old value x 12 / 100, so `i500` -> `60`; the five `*elementalsummons` were set to `5` by decision instead of the faithful 2.4). New `peacemake` and `entice` fields seeded from `provoke` on 1,276 templates.
- Dropped everywhere: `hostile` (1,107 - its only reader `IsHostile()` had no callers), `psub` (1 - `anchors.inc` now uses `ANCHOR_PSUB := 10`), the never-read `Karma`/`Fame` fields (166 / 138), `prop`, `graphics`, `virtue`, `buddytext`/`leadertext`/`targettext`; CProps `Equipt`/`equipt`, `pack`, `kappa`, `vortexnaga`, `PoisonProtection`. 44 commented-out lines inside blocks removed. 61 duplicate keys resolved to their first value (what the engine used); one hand-pick: `bewitchedpeasant` `equip cothes` (no such `Equipment` block) -> `bewitchedpeasant`. `terathanmatriarch`'s invalid `EvaluateIntelligence` -> `EvaluatingIntelligence`.
- `modsetup.inc`: new `ApplyTemplateRegen()` reads `GetNpcVitalSetting(who, "HITSREGEN")` (and MANA/STAM) - template field x100 = the engine's hundredths per minute, cached per NPC, tool override first - into the `"template"` regen-rate mod, and clears a stale entry when the template has no field. The `baseregen`/`basemanaregen`/`basestaminaregen` mirror CProps are gone. Regen no longer feeds the karma/fame bonus (46 templates with `BaseManaRegen i60000` were pinned at the 15,000 cap by it).
- `npcvitals.inc`: `GetNpcVitalSetting` and `SetNpcVitalOverride` accept decimals (`Cint(CDbl(x) * 100)`).
- `scripts/include/npcdifficulty.inc` (new): `GetSnoopDifficulty()` / `GetStealDifficulty()` = instance ObjProperty first (areaspawner override, and NPCs spawned before the move that still carry the CProp), then the template field. Used by `snooping.src` (both reads), the mobile tooltip in `mobiledata.src`, and the areaspawner seed.
- `peacemaking.src` and `songoffright.src` roll Peacemaking against `elem.peacemake`; `taunt.src` rolls Enticement against `elem.entice`; `provocation.src`, `grarkshep.src` and the tooltip keep `provoke`. A missing field still means 100.
- `npcinfo.inc`: `IsHostile()` and `IsGood()` deleted.
- Areaspawner custom NPCs: regen overrides are points per minute stored with `SetNpcVitalOverride(critter, "HITSREGEN"/"MANAREGEN", n)` (same slot as HITS, so `modsetup.inc` sees them first); the override struct carries `regenunits := "ppm"` and a definition saved without that marker is converted x12/100 on load; the editor labels read "HP Regen /min" / "Mana Regen /min"; the snoop/steal seed uses the helper.
- `.info` Status page: "Regen HITS/MANA/STAM" row (template values, live value when a tool override differs) replaces "Base regen HP/Mana".
- After the code review of this theme: `RegenPointsFromSetting()` (areaspawner editor seed; -1 = no field -> 0) and `NormalizeCustomNpcOverrides()` (converts an old-unit saved definition once, called from both the spawn path `TryPlaceAreaSpawnerCustomNpc` and the editor load, so ticks and full respawns convert too) were added; `GetNpcVitalSetting`/`SetNpcVitalOverride` round instead of truncating (`+ 0.5`); `GetNpcVitalOverride` returns a Double for fractional values so `.info` compares correctly; `SetNpcVitalOverride` skips `RecalcVitals` for the regen keys; the editor parses the regen fields with `CDbl`; `npcdifficulty.inc` uses `GetConfigInt`; the retired `CProp BaseHpRegen/BaseManaRegen/snoopme/stealme/MoveSpeed` lines inside the fully commented-out templates (`ringarmorsalesman`, `ebard`, `virtualcn`) were rewritten to the new fields so reviving one works; the reference block's HITSREGEN line names `SetNpcVitalOverride` as the override, not a CProp.

**Expected impact:** No change to how any creature spawns, fights, regenerates or drops loot, except: the five elemental summons regenerate 5 HP/min (was 2.4); the terathan matriarch has Evaluating Intelligence 135 (was 0); the bewitched peasant spawns equipped (was naked); the town healer's leash wall moves from 15 to 20 tiles past its spot; NPC karma no longer includes the regen bonus (the 46 high-mana-regen templates drop below the 15,000 cap). Peacemaking and Enticement difficulties equal Provocation's until a `peacemake`/`entice` line is edited. The regen fields need the NPC respawn already listed above; snoop/steal work on old NPCs as well.

### 19. Houses: travel checks use the footprint, not the rune's height (commit `68a5c45`, 2026-10-02)

**Files involved:** `scripts/include/housetravel.inc` (new), `scripts/include/traveldebug.inc` (new, temporary), `pkg/std/spells/recall.src`, `pkg/std/spells/gate.src`, `pkg/std/spells/mark.src`, `pkg/std/spells/teleport.src`, `pkg/std/runebook/customspells.inc`, `pkg/std/runebook/runebookactions.inc`, `pkg/opt/earth/earthportal.src`, `scripts/textcmd/admin/spellbook.src`, `config/command_synopses.cfg`, `.claude/subagent-briefing.md`

**Notable functional changes:**
- Cause, confirmed in game on 2026-10-01 with console tracing: every travel script tested the destination with `GetStandingHeight(tox, toy, toz, torealm).multi`. The engine (`lowest_walkheight`) answers with the lowest standable surface at or above the given z, while `MoveObjectToLocation` (`walkheight`) puts the mobile on the nearest surface within +7 above or any distance below. A rune marked on a Small Tower's second floor (z 26), used after another player's Small Stone and Plaster house (floor z 7) was placed on the spot, got "Can't stand there" from the test (no house, so no owner/friend test) and then dropped onto the floor inside. A top-floor rune (z 46) passed the test the same way but the move failed, with no message. The same test saw no house on a tile with no static house piece (castle courtyards: 7 designs, 367 enclosed tiles) or in a static house, whose own check was commented out in all four scripts.
- `housetravel.inc` (new): `FindHouseAtSpot(x, y, z, realm, scan_signs := 1)` returns the house covering a spot: (1) an owned house sign of any housing package whose `footage` box covers x,y, with z ignored; (2) the old engine test at that z, so steps, porches and boats behave as before; (3) a house multi with a piece on the tile when it has no sign or its sign has no footage. `CanTravelIntoHouse(who, house, priv)` is `IsCowner` or `IsFriend` (the Teleporter permission). `RemoveRunebookEntryAt(book, x, y, z, realm)` removes matching `RuneDefs` entries and clears a matching default location. `DestroyRefusedRune()` destroys a refused loose rune or removes the book entry; nothing is destroyed when the "house" is a boat.
- `recall.src`, `gate.src`, `earthportal.src` and `customspells.inc` (`CustomRecall`, `CustomGate`): the origin and destination tests use the helper. A refused destination destroys the rune or removes the book entry (owner's decision). `CustomRecall`/`CustomGate` return `TRAVEL_REFUSED_BY_HOUSE` (-1), and the three callers in `runebookactions.inc` call the new `RemoveRefusedEntry()`. A failed move sends "Something blocks that location." The dead commented static-house blocks are removed.
- `mark.src`: uses the helper instead of `caster.multi`.
- `teleport.src`: after the existing multi test, a destination inside a house footprint is refused for a caster without access ("You cannot teleport there."); NPC casts skip the sign scan.
- `earthportal.src`: every early exit clears `#Casting` (five did not); the "You can't gate from to this house." message is fixed.
- `spellbook.src`: `.spellbook` also creates a Bag of Infinite Reagents (`0xba30`) and a "travel test kit" bag with 100 blank runes, 100 recall scrolls and 100 gate scrolls; synopsis regenerated.
- `traveldebug.inc` (temporary): `[traveldbg]` console lines from Mark, Recall, Gate and the runebook paths while `TRAVEL_DEBUG` is 1. To be deleted, with its tagged call lines, once the fix is confirmed in game.

**Expected impact:** A rune pointing inside a house its holder may not enter is refused and destroyed at any height, in courtyards and in static houses; owners, co-owners and Teleporter-permission holders travel as before. Recall and Gate cast from inside a static house or a courtyard need the same access as from inside any house. An unowned static house is not protected. Recall and Gate onto steps and porches behave as before. The console is noisier until the tracing is removed.

### 20. Accounts: login policy reworked (DiscordID, IP and household rules) (commit `68a5c45`, 2026-10-02)

**Files involved:** `pkg/systems/accounts/include/accounts.inc`, `pkg/systems/accounts/hook/onLogin.src`, `pkg/systems/accounts/config/uopacket.cfg`, `pkg/systems/accounts/config/settings.cfg`, `pkg/systems/accounts/logon.src`, `pkg/systems/accounts/reconnect.src`, `scripts/misc/logon.src`, `scripts/misc/reconnect.src`, `scripts/misc/logoff.src`, `scripts/textcmd/test/householdadd.src`, `scripts/textcmd/test/householdremove.src`, `scripts/textcmd/test/householdcap.src`, `scripts/textcmd/test/householdmanager.src`, `scripts/textcmd/test/extralogin.src`, `scripts/textcmd/test/accountpolicyinfo.src`, `scripts/textcmd/admin/setdiscord.src` (new), `pkg/systems/accounts/include/mailSystem.inc` (deleted), `pkg/systems/accounts/commands/dev/eraseEmptyAccounts.src` (deleted), `config/command_synopses.cfg`, `patchnotes/accounts_household_qa_runsheet.md`, `.claude/subagent-briefing.md`, `.claude/skills/escript-gotchas/SKILL.md`, `ainotes/code-review-fixlog-20260921.md`

**Notable functional changes:**
- The rules, as the owner set them: a player is one DiscordID with at most 2 accounts; a DiscordID has at most 2 characters online (two on one account, or one on each of two); all of them come from one IP; two DiscordIDs may not share an IP unless they are in one household; staff are exempt and never counted. Ten decisions were picked on a findings page; households stay per account (decision 3). Every edit site carries a `// 2026-10-01:` comment.
- Bug, households: `ACCT_CheckForMaxClientsOnline_NewEngine` looped `foreach ip_discord in ( ip_discord_set )` over a dictionary used as a set. The engine's dictionary iterator hands back the values (`bscript/bdict.cpp`), all `1`, so `ACCT_HouseholdContainsDiscord( household, 1 )` failed for every account carrying a `HouseholdId`, alone or not ("mixed-household IP use denied"). The loop now walks `ip_discord_set.keys()`.
- Bug, one-IP rule: the login hook stamped the account with `#PendingPolicyLoginIP` (the address of the 0x80 connection) for 180 seconds, and the world-entry check merged other accounts' stamps into the DiscordID's IP set. On the host machine the login screen is reached on 127.0.0.1 and the game on the LAN address (`servers.cfg` `IP --lan--`), so two accounts of one DiscordID logging in together were refused ("discord active from multiple IPs"). The stamp is gone: `ACCT_CountPendingPolicyByDiscord`, `ACCT_CountPendingPolicyByIP`, `ACCT_ClearPendingLoginReservation`, `ACCT_AcquirePolicyLoginGate` and `ACCT_ReleasePolicyLoginGate` are deleted, and both checks count only characters that are online. The engine runs the logon scripts one login at a time, to completion (`call_chr_scripts`), so that list is exact and the gate was never needed. This also ends the up-to-3-minute refusal after an abandoned login and removes a pass over every account at each stage.
- `ACCT_CheckAccountLoginPolicy( account )` (login screen; the `ip` parameter is gone) refuses only when the DiscordID already has `MaxDiscordsOnline` characters online and none of them is on this account. With a character of this account online the login may be a reconnect, so it is left to the world-entry check.
- `ACCT_CheckForMaxClientsOnline_NewEngine( who )` keeps its rule order (staff, missing DiscordID, one IP, count, IP sharing, household cap, household membership) and works from the online snapshot only. A household with no record, or no cap on its record, uses the default cap; the old "household record missing" refusal could never fire, because a missing datafile element comes back as 0 and not as an error.
- `ACCT_AdmitCharacter( who )` (new) is the gate, called first thing in `scripts/misc/logon.src` and `scripts/misc/reconnect.src`. The engine runs `scripts/misc/logon.ecl` before any package's `logon.ecl`, so the check used to run after the arrival broadcast and after `playermanager.src` was started. The package's own `logon.src` and `reconnect.src` keep only `ACCT_SetLastLogin` (and the staff speed-walk packet). `logon.src` sets `#ArrivalAnnounced` when it broadcasts the arrival; `logoff.src` announces the departure, and adds the session to `onlinetimer`, only when that flag is set. A refused character still waits out the normal logout delay (`logofftest.src`): removing it at once would give players an instant logout.
- Login hook (`hook/onLogin.src`): a hook that returns 0 hands the packet to the core's own handler (`packethooks.cpp`), so the old `DeniedMSG(); return 0;` refused nothing by itself. New `RefuseLogin( connection, reason )` sends the 0x82 packet, calls `DisconnectClient` and returns 1. It is used for the 2-character refusal (reason 6, the client's concurrency-limit message), a missing DiscordID (reason 8) and the password lockout (reason 3, account blocked). The lockout branch (`ACCT_LOGIN_HACK`) had returned 0 since the first commit, so a locked-out address still got in with the right password; ModernDistro returns 1 there, and the 2026-09-21 rework of `AcctHackChecks` missed it. The `blockLogin` switch still returns 0 and was left alone.
- `DeniedMSG`: the reason byte of the 2-byte 0x82 packet is offset 1. The function wrote offset 2, which the engine rejects on a fixed-length packet, so every script refusal reached the client as reason 0 ("Incorrect name/password"). It writes offset 1 now, which also gives the `blockLogin` and auto-account refusals their intended messages.
- `GameLoginHook` (new; `uopacket.cfg` `Packet 0x91`, length 0x41): the game-login packet checks the password too and was not hooked, so guesses sent straight to it were never counted. The hook runs `AcctHackChecks` (account name at offset 5, password at 35); a locked-out address is disconnected and everything else returns 0 to the core.
- Staff detection: `ACCT_AccountHasStaffAccess` read `account.GetProp( "DefaultCmdLevel" )`, a custom property that never exists. It now reads the built-in `account.defaultcmdlevel` and accepts only a number above 0. The per-character command-level test is unchanged.
- Households stay per account (`HouseholdId` account property). `ACCT_SyncHouseholdMembers( household_id )` (new) rebuilds the record's `Members` list from the accounts that carry the id. `ACCT_AssignAccountToHousehold` and `ACCT_RemoveAccountFromHousehold` use it instead of appending and erasing by hand, so removing one of a player's two accounts no longer drops the DiscordID from the list, and a household an account is moved out of is rebuilt too. A new household's cap is `DefaultHouseholdCap` (2, was 1).
- Accounts per DiscordID: `ACCT_ListAccountsWithDiscord()` and `ACCT_DiscordAccountLimitReason()` (new). `CreateNewAccount` returns an error naming the existing accounts when the DiscordID already holds `MaxAccountsPerDiscord` (2; 0 = no limit), before anything is created, and `.mkaccount` shows it. `CreateNewAccount` also wrote the property `"Login "` with a trailing space; it writes `"Login"`.
- `ACCT_SetAccountDiscordID()` and `.setdiscord <account> <discordID>` (new, Administrator) set or change the DiscordID of an existing account under the same limit; the account keeps its household and the member list is rebuilt. Until now the only way was to edit `data/accounts.txt` with the server stopped.
- `ACCT_PickAccountForCommand()` (new): `.householdadd [household] [account]`, `.householdremove [account]`, `.householdcap [cap] [account]`, `.extralogin [account]`, `.householdmanager [account]` and `.accountpolicyinfo [account]` take an account name and then work on an offline account; with no name they target a character as before. `.accountpolicyinfo` shows the effective cap by the same rule as the login check and lists the other accounts on the DiscordID.
- `ACCT_GetPolicySetting( key, fallback, lowest )` (new) reads the policy numbers. `settings.cfg`: `MaxAccountsPerDiscord 2` and `DefaultHouseholdCap 2` added; the legacy `MaxIpsOnline`, `MaxCharsOnline` and `MaxCharsPerIP` keys, read by nothing, removed.
- Dead code removed: `ACCT_GetHouseholdForDiscord`, `ACCT_IsAccountCurrentlyOnline`, `ACCT_IsCharacterSerialCurrentlyOnline`, `ACCT_FindValidPlayer`, `include/mailSystem.inc` (included nowhere) and `commands/dev/eraseEmptyAccounts.src` (disabled, in a directory `cmds.cfg` never registered). `acctWatcher` is still never started.
- After the review of this theme (2026-10-02): a character refused at login waits out its logout delay in the world, and the engine sends a login to it inside that delay to `reconnect.ecl`, not `logon.ecl` (`char_select` takes the reattach branch while `chr->client` is still set). With the check now ahead of everything in `logon.src`, such a character had never run its login setup, so a successful retry came in with no `playermanager.src` (hunger, area ban, class assignment, news) and no announcements. `scripts/misc/reconnect.src` now starts `misc/logon` and returns when `#ArrivalAnnounced` is not set. `.householdmanager` refuses a new household id that contains a space, because `.householdadd` now reads its text as `<household> <account>`.
- Records: the QA run sheet rewritten for the new behaviour; Shard Test Board t187-t202; gotchas 27 and 28 in the escript-gotchas skill and the matching briefing lines; a correction under 3.A2 in the review fix log.

**Expected impact:** Two accounts of one DiscordID can log in at the same moment on the host machine. Accounts in a household get in, and the DiscordIDs of one household share an address up to the household's cap. A third character of a DiscordID is refused at the login screen when it comes from the other account, with the client's concurrency-limit message, and otherwise on entering the world. A refused character is not announced as arriving or departing. Five wrong passwords within 3 minutes block that account from that address for 10 minutes, on both login packets. `.mkaccount` and `.setdiscord` refuse a third account for a DiscordID. An account with `DefaultCmdLevel` above 0 counts as staff even with no staff-level character. `DebugAccountsPolicy 1` prints an `[ACCTDBG]` line at every decision. Not compiled and not tested in game.

### 21. Skill arrows lose 0.1 per use, the client's own arrows count, the verse book names its skills (commit `68a5c45`, 2026-10-02)

**Files involved:** `scripts/include/skillpoints.inc`, `pkg/systems/attributes/hooks/shilhook.src`, `pkg/systems/attributes/hooks/skilllock.src` (new), `pkg/systems/attributes/config/uopacket.cfg` (new), `scripts/textcmd/player/skills.src`, `pkg/opt/versebook/include/versefunctions.inc`, `pkg/opt/versebook/include/verseinfo.inc`

**Notable functional changes:**
- Report: a failed Dragon Skin took a point of Begging. Dragon Skin is a Begging + Peacemaking verse by design (`verses.cfg` verse 6, `UseSkill 6` and `9`, same in POL2.5), rolled at difficulty 90 after the 75+ attempt gate; the player's Begging arrow was already down (only the `.skills` gump writes that state), and `ShilCheckSkill` takes from a down-arrow skill on every check. Kept: the verse's skills and the every-check trigger. Changed, per the owner's picks:
- `AwardSkillPoints` (`skillpoints.inc`, the "d" branch): the drop is 0.1 instead of a whole point. It used `GetBaseSkill`/`SetBaseSkill`, which work in whole points, so 75.9 became 74.0; it now uses `GetBaseSkillBaseValue`/`SetBaseSkillBaseValue` (tenths) and takes 1 tenth. The lock-at-zero rule is unchanged. Comments in `shilhook.src` updated.
- New packet hook on 0x3A (`config/uopacket.cfg`, `hooks/skilllock.src`, `SkillLockHook`): the arrows in the client's own skill list used to reach only the engine's per-skill lock (`CoreHandledLocks=1`), which no script reads; the paperdoll's Skills button (`scripts/misc/skillwin.src`) only sends that native list, so for most players the arrows did nothing. The hook writes the same `SkillsState` entry the `.skills` gump writes (0 up -> "r", 1 down -> "d", 2 locked -> "l"; the client's skill numbering matches `SKILLID_*`), ignores a connection with no character or an id above `SKILLID__HIGHEST`, and returns 0 so the engine still records its own lock.
- `scripts/textcmd/player/skills.src` (the `.skills` gump): its three arrow branches now also call `SetAttributeLock` with the matching `ATTRIBUTE_LOCK_*`, so the client's list shows the same arrow. The "decrease" branch passed `res` (skill id + 1) to `TakeCurrentSkillLevelInNote`, which the other two branches pass the skill id to; it passes `res-1` now, so the combat-skill note records the right skill. `scripts/misc/skillwin.src` (whose gump code is commented out) is unchanged.
- Verse book: `ReturnVerseInfo` returns the verse's skill names (new `VerseSkillNameLines`: `skillsdef.cfg` `Name` per `UseSkill` id, `TasteIdentification` shown as "Taste ID", duplicates dropped, a second line when the first would pass 24 characters). `GetVerseInfo` prints a "Uses:" row at y 184 (second line at 198) on every verse page; the Stamina and Difficulty rows moved from y 160/180 to 152/168 and every verse icon to the top right (x 408 or 386/408, y 30) so the row does not run under them or into the page-turn strip at y 210.
- Typo: "fails the arachic verse" -> "archaic" (`versefunctions.inc`).

**Expected impact:** a skill set to decrease loses 0.1 per use instead of 1.0-1.9. Arrows set in the client's own skill list now take effect, and the two skill windows agree from the next click on. The verse book shows each verse's skills. Existing arrow states are untouched: a player whose arrow was set down in `.skills` keeps losing 0.1 per use until they change it. Not compiled and not tested in game.

---

### 22. Classes: skill-gain lookup, stat affinity, caps and the Powerplayer (commit `9164261`, 2026-10-03 and 2026-10-04)

**Files involved:** `scripts/include/classes.inc`, `scripts/include/skillpoints.inc`, `scripts/include/attributes.inc`, `pkg/opt/zuluitems/Testclassbooststone.src`, `scripts/textcmd/admin/setclass.src`, `pkg/opt/alryc/textcmd/player/classinfo.src`, `pkg/opt/rituals/scroll/use.src`

**Notable functional changes:**
- `GetClasseIdForSkill( who, skillid )` now walks the classes the character HAS a level in and returns the highest-level one whose skill list holds the skill (0 if none). It used to return the first class in `GetClasseIds()` order that listed the skill, so a Warrior's Swordsmanship resolved to Bladesinger (level 0) and the Warrior got no class skill-gain bonus, failed-roll second chance or success bonus. Detecting Hidden and Snooping are free co-skills of the Thief (not in its list), so they keep a special case that resolves them to the Thief. A Powerplayer is allowed only on Forensics, Throwing, Snooping and Detecting Hidden (its own x1.1/x1.2/x1.3 at levels 3-5 in `skillpoints.inc` covers the rest). `IsSpecialisedIn` ends with an explicit `return 0`.
- `skillpoints.inc`: new `ClassStatGainMultiplier( who, attributeid )`. `AwardPoints` multiplies the Strength dice amount by `ClasseBonus` and divides the Intelligence dice amount by it for Warriors and Crafters (higher level wins when a character holds both); rounded with `Cint( x + 0.5 )`. `HaveStatAffinity` / `HaveStatDifficulty` in `classes.inc` are still unused.
- `classes.inc`: `THIEF_BACKSTAB_BONUS_DAMAGE` and `THIEF_AMBUSH_BONUS_DAMAGE` deleted (used nowhere). `IsFromThatClasse` and `ClasseLevelFromCounts` (in `classinfo.src`) no longer hard-code 3675; the all-skills class (the only list over 20 skills) uses `AVERAGE_SKILL * number` (75 x 50 = 3750 for level 1, then 15 x number more per level). `HaveInvalidSkillEnchantmentForClasse`: the General-skills exemption line is commented out (its `and`/`or` precedence made the condition always true for a non-zero skill, so behaviour is unchanged). `IsProhibitedByClasse` also treats `ArBonus` as armor rating on enchanted armor.
- `classinfo.src`: the all-skills class shows "Next level (N): X more in-class points needed." with the point total from the same formula and no percentage or slack line.
- `attributes.inc`: new `RaiseSkillCapsTo( who, skillids, value )` writes the per-skill power-scroll matrix so a skill set to 135 (level 5) or 150 (level 6) is not pulled back to 130 by the hourly capper. `Testclassbooststone.src` and `.setclass` use it (including the free co-skills: Ranger Forensics, Crafter Magery and Musicianship, Thief Detecting Hidden and Snooping). The old `testmaxcaps()` is deleted: it saved the number 20 as the whole matrix, which collapsed every cap to the base cap. The stone treats OK with nothing picked as a cancel instead of resetting every skill.
- `pkg/opt/rituals/scroll/use.src`: only Mages (level 2 or higher) may start a ritual from a scroll; other classes get "Only Mages can perform rituals." Staff are exempt. It used to accept any class at level 2.

**Expected impact:** Classed characters get the class skill-gain bonus, second roll and success bonus on every skill their class lists, not just when the first matching class happened to be theirs. Warriors and Crafters gain Strength faster and Intelligence slower. `.classinfo` is correct for the Powerplayer. Staff-set level 5 and 6 skills hold across the hourly capper. Non-Mage classes can no longer perform rituals.

### 23. Crafting: class bonus, ammo, Make Max, hide gate, new ingots, Bulk Order Book (commit `9164261`, 2026-10-03 and 2026-10-04)

**Files involved:** `pkg/std/bowcraft/bowcraft.src`, `pkg/systems/crafting/include/craftmenu.inc`, `pkg/std/tailoring/tailoringfunctions.inc`, `pkg/std/blacksmithy/blacksmithy.cfg`, `pkg/std/bulkorders/bulkorders.cfg`, `pkg/std/inscription/inscription.src`, `pkg/std/inscription/inscription.cfg`, `pkg/opt/crafterboost/crafterboost.cfg`, `pkg/opt/crafterboost/crafterboost_recipes.cfg`, `pkg/opt/crafterboost/make_crafter_boosts.src`, `scripts/include/itemutil.inc`

**Notable functional changes:**
- Bowcraft: the Crafter exceptional-chance bonus uses `ClasseBonus( character, CLASSEID_CRAFTER )` (by class level) instead of the flat `CLASSE_BONUS` constant. Single and bulk ammo apply the Half-Resources power hour (`PHC` / `#PPHC`, rounded up with `Ceil( x / 2.0 )`). Single ammo follows the consume-first pattern: `HasCraftingResources`, then `ConsumeResource` for each material with the result checked, then `CreateItemInBackpack`; it used to create the ammo and consume afterwards without checking, so moving the shafts or feathers away mid-loop gave free ammo.
- `craftmenu.inc`: Make Max counts with the halved cost during a Half-Resources hour (not for Smelting) and guards a zero cost.
- `tailoringfunctions.inc`: the hide gate is back: Tailoring below the hide's difficulty gives "You aren't skilled enough to make anything with this yet." and stops (3.1.0 had dropped it).
- `blacksmithy.cfg`: rows for the coin-melted Silver (0x1BF5) and Copper (0x1BE3) ingots (Difficulty 1, Quality 1.0, same as Gold); `bulkorders.cfg` excludes both as bulk-order materials.
- Crafter Boost: `crafterboost.cfg`, `crafterboost_recipes.cfg`, `make_crafter_boosts.src` and `IsCBoost` in `itemutil.inc` use the objtypes 0x30381-0x30384. The items in `itemdesc.cfg` had already moved off 0x8B01-0x8B04 (those collide with real client graphics), so the recipes were still creating the old ids.
- Inscription: new Bulk Order Book recipe (objtype 0x2259): 100 blank scrolls, 100 mana, 5 plain logs (0x1BDD) and 25 cloth (any of 0x175D-0x1768), Inscription 60; a Mage's `ClasseBonus` divides the mana and the scroll cost only. New helpers `BookRequest`, `CountBookCloth`, `ConsumeBookCloth` in `inscription.src`.

**Expected impact:** Crafters' exceptional chance scales with level. Ammo costs half during a Half-Resources hour and can no longer be duplicated. Make Max stops at the real amount. Tailoring refuses a hide that is too hard. Silver and copper ingots show a name and difficulty in the Blacksmithy window. Crafter Boost products are the real items. Inscription can make the Bulk Order Book.

### 24. Spells, songs and buffs (commit `9164261`, 2026-10-03 and 2026-10-04)

**Files involved:** `scripts/include/dotempmods.inc`, `scripts/include/spelldata.inc`, `pkg/opt/earth/shapeshift.src`, `pkg/opt/earth/summonmammals.src`, `pkg/opt/necro/liche.src`, `pkg/opt/summoning/polymorphing.src`, `pkg/opt/summoning/summoning.src`, `pkg/std/spells/polymorph.src`, `pkg/std/spells/heal.src`, `pkg/std/spells/gheal.src`, `pkg/std/spells/resurrect.src`, `pkg/opt/holybook/revive.src`, `pkg/opt/holybook/angelicfeast.src`, `pkg/opt/songbook/songofdefense.src`, `pkg/opt/songbook/songofdismissal.src`, `pkg/opt/songbook/songoffright.src`, `pkg/std/taunt/taunt.src`, `pkg/opt/versebook/Spirit_Flock.src`

**Notable functional changes:**
- `dotempmods.inc`: new `CanModNoConflict( who, stat )` (true when no running mod conflicts, via `TempModConflicts`). Shapeshift, Liche, Polymorph and the polymorph effect call it BEFORE the body changes or mana is spent and answer "Another buff you are under conflicts with that form." A refused "poly" mod used to leave the new body with nothing to revert it. The Powerplayer exception in `shapeshift.src` is removed.
- `spelldata.inc`: when `ConsumeReagents` fails the mana already taken comes back (capped at max mana), as for a skill fizzle. `SendBoostMessage` no longer multiplies the shown points by 0.8 (DoTempMod stopped scaling in 3.0.3).
- Heal, Greater Heal, Resurrect and Revive use a helpful target cursor instead of a neutral one; `IsSpellPvPBlockedByArea` still protects Liche-form targets and the caster now hears "That would harm them, and they are in a protected area."
- `liche.src`: the Mage duration multiplier result was thrown away (`CInt( duration * ... )` with no assignment); it is assigned now. `summoning.src`: the no-class defaults are duration 80 (was 60) and power 10 (was 1). `angelicfeast.src`: Grand Feast scales with `IsPaladin` (was `IsMage`).
- `summonmammals.src`: a Mystic Archer gets the Spawn of the Dead only (it returns instead of falling into the mammal loop).
- Songs: Song of Defense says when a target already has an armor-affecting buff; Song of Dismissal includes the Bard (`SmartAoE` drops the caster); Song of Fright defaults to difficulty 100 (cap 150) when the template has no `peacemake` field, which `ShilCheckSkill` treated as an automatic success.
- `taunt.src` (Enticement): the cancel test is `||` (it was `&&`, so only a diagonal step cancelled it).
- `Spirit_Flock.src`: each goat's max HP comes from the Bard's skills and level through `SetNpcVitalOverride( goat, "HITS", goat_hp )` (the template is a flat 30).

**Expected impact:** Form spells and buffs refuse cleanly instead of leaving a stuck body. Healing spells show the protected-area reason. Liche lengthens with Mage level, Spirit Flock goats scale with the Bard, and Enticement cancels when you step in a straight line.

### 25. Rangers, pets and Mystic Archers (commit `9164261`, 2026-10-04)

**Files involved:** `pkg/std/veterinary/vet.src`, `pkg/std/healing/healing.src`, `scripts/ai/tamed.src`, `scripts/ai/animaltrainer.src`, `config/itemdesc.cfg`, `pkg/opt/zuluitems/dragoneggs.src`, `pkg/opt/zuluitems/ostardeggs.src`, `pkg/systems/combat/include/hitscriptinc.inc`, `pkg/std/fishing/magicfish.src`, `config/npcdesc.cfg`, `config/food.cfg`

**Notable functional changes:**
- `vet.src`: the dead half-heal branch is removed. Resurrecting an animal uses 5 bandages up front and the raised animal becomes the Ranger's pet (`master`, `PreviouslyTamed 1`, `SetMaster`, script `tamed`, `RestartScript`). The `healing.src` comment is corrected (a Ranger bandaging himself is refused by `vet.src`).
- `PreviouslyTamed` is the spelling every reader uses: `animaltrainer.src` (3 sites), `dragoneggs.src` and `ostardeggs.src` wrote `prevtamed`.
- Pet confiscation: `tamed.src` has a new `PET_CONFISCATION_FINE_PER_STR := 5` and `ConfiscatePet()`; releasing a pet (or being over the pet limit) inside a safe area now confiscates it. New item `0xDF0C` PetConfiscationNotice (graphic 0x14F0) in `config/itemdesc.cfg`; an Animal Trainer takes it for a gold fine (`Load_Ticket_Data`).
- `hitscriptinc.inc` `DistanceCheck`: full-damage range is 14 for everyone, 14 + level for a Ranger (`RANGER_LEVEL_ARCHERY_RANGE_BONUS`, previously unused) and 10 + level for a Mystic Archer.
- `magicfish.src`: a refused buff, a Cure while healthy and a failed Teleport pre-check keep the scroll and clear the lock; new `CanTakeFishMod`; message "That effect cannot be applied to you right now, so the scroll was not used."
- `npcdesc.cfg`: 33 dragon templates carry `CProp Type sDragonkin` (the slayer type; some had `sDragon` or the typo `sDragopnkin`); the thief vendor template has `Throwing 100`.
- `food.cfg`: 18 foods (recipes 69-88) added to the `cooked` group so the hunger auto-eat can choose them.

**Expected impact:** Rangers can raise animals as pets; confiscated pets can be bought back; Dragonkin slayers work on dragons; archery range depends on class and level; Mystic Archer fish scrolls are no longer wasted on a refusal; newer foods are eaten automatically.

### 26. Thieves and traps (commit `9164261`, 2026-10-04)

**Files involved:** `scripts/include/chests.inc`, `pkg/std/spells/magictrap.src`, `pkg/std/spells/magicuntrap.src`, `pkg/std/tinkering/tinkeringfunctions.inc`, `pkg/std/detecthidden/detecthidden.src`, `pkg/std/lockpicking/use/picklock.src`, `pkg/std/hiding/hiding.src`, `config/mrcspawn.cfg`, `pkg/opt/summoning/processpoisonmod.src`, `pkg/std/healing/poison_heal.src`

**Notable functional changes:**
- Trapped chests use `Trap_Type`, `Trap_Strength` and `Trapped_By` everywhere (`chests.inc`, Magic Trap, Tinkering `SetTrap`). `traps.src` reads these names but the writers used lowercase names, so a trapped chest never fired. Magic Untrap and Detect Hidden read the new names with the old lowercase names as a fallback; Magic Untrap erases both. Chests trapped before this change still do not fire.
- `picklock.src`: the unlocked treasure chest stays 300 seconds (it was deleted after 30 although the message says 5 minutes).
- `hiding.src`: the hostile-distance range never drops below 1 tile (a Thief at Hiding above 130 rounded it to 0).
- `mrcspawn.cfg` `ThiefTools`: Toxin Flask and Vial of Venom (10 each). `processpoisonmod.src` and `poison_heal.src`: comments corrected to the real values (integer division); no behaviour change.

**Expected impact:** Trapped chests fire; Thieves cannot hide next to a hostile; the Thief vendor sells Toxin Flasks and Vials of Venom and can train Throwing.

### 27. IP bans work at login (commit `9164261`, 2026-10-04)

**Files involved:** `scripts/include/ipban.inc` (new), `scripts/misc/logon.src`, `scripts/misc/reconnect.src`, `pkg/opt/ipban/textcmd/admin/ipban.src`

**Notable functional changes:**
- New `ipban.inc`: `IPBan_Matches( ban, address )` (an octet of 255 matches itself and everything after it, so `65.5.255.255` bans `65.5.*.*`; any other octet must match exactly), `IPBan_IsBanned( address )` and `IPBan_AdmitCharacter( who )` (tells online staff, disconnects, returns 0).
- `logon.src` and `reconnect.src` call `IPBan_AdmitCharacter` right after `ACCT_AdmitCharacter` and before anything is announced; the old local `CheckIPBan` functions and the `":ipban:bannedips"` constant are deleted. They read a different datafile from the one `.ipban` writes (`"bannedips"`) and could only match a 255 range.
- `ipban.src`: refuses to ban a range that contains the caster's own IP; online players inside a banned range are disconnected; `data.keys()` is called with parentheses.

**Expected impact:** A banned address or range is refused at login, with no "has arrived!" broadcast. Online staff see who tried.

### 28. Banker, escrow, player vendors, Animal Trainer, guild locks (commit `9164261`, 2026-10-04)

**Files involved:** `scripts/ai/banker.src`, `pkg/systems/playervendor/commands/player/escrow.src`, `pkg/systems/playervendor/playermerchant.src`, `scripts/ai/animaltrainer.src`, `scripts/ai/merchant.src`, `pkg/opt/guilds/commands/player/guilds.src`, `pkg/opt/guilds/include/guildconstants.inc`

**Notable functional changes:**
- `banker.src`: the four `MoveItemToContainer( event.source.backpack, event.item )` calls had their arguments reversed, so a declined or failed Banker's Order stayed with the banker; they read `( item, container )` now. `BuyBankersNote`, `ConvertCopper` and `ConvertSilver` create the note first and take the coins second (a full backpack no longer costs coins; the note is destroyed if taking the coins fails). The idle loop tests `ev.type == SYSEVENT_ITEM_GIVEN`; any item that is not a note is returned with "I have no use for this."
- `escrow.src` (`.escrow`): skips every entry whose vendor name is "Warrior for Hire" (`ESCROW_WFH_VENDOR_NAME`) and refuses to claim one; those packages are recovered through the High Priest for his fee.
- `playermerchant.src`: when the buyer is the owner (`ev.source.serial == master`) the new `RefundOwnerPurchase` returns the price (backpack, else the ground, in 60,000 chunks) and `TakeSale` is skipped. The engine charges before `SYSEVENT_MERCHANT_SOLD` arrives (`ScriptedMerchantHandlers=0`).
- `animaltrainer.src`: `MerchantTrain`, `GoldForSkillGain`, `TrainGump`, `TrainMax` and `TrainEntry` are copies of `merchant.src`'s 3.1.2 versions; speech is tested on the lowercased text and accepts `vendor` or `merchant` + `train` or `teach`; the Accept button reads `data.keys()`. `MAX_SKILLS` is 57 in both `animaltrainer.src` and `merchant.src` (Throwing is skill 57; 48 left it and the last skills untrainable).
- Guild locks: `GUILD_COLOUR_TIME` is 7 (days; it was 24 hours) and `GUILD_NAME_TIME` stays 7 days, both compared on the wall clock (`POLCORE().systime`, `use polsys`) instead of minutes against `ReadGameClock()` (seconds since the last server start). Stamps from the old clock read as expired. A guild created now cannot be renamed for a week.

**Expected impact:** Declined notes come back and no coin is lost making one; owners buy their own stock free; Warrior for Hire gear costs the High Priest's fee; Animal Trainers train like merchants and every merchant can offer Throwing; the guild colour and name locks last one week.

### 29. Houses: `.redeed` stays inside your own house (commit `9164261`, 2026-10-04)

**Files involved:** `pkg/items/deed/commands/player/redeed.src`, `pkg/packethooks/pickupitem/pickupitem.src`

**Notable functional changes:**
- `.redeed` finds the house with `FindHouseAtSpot` (the z-blind footprint test of `include/housetravel`), requires the caster to be its owner, and requires the target and every part in `OtherItems` to be inside that same house and not in a container. Messages: "That is not in your house!", "That has to be placed in your house before it can be redeemed.", "Part of that item is no longer in your house. Ask staff for help." Staff (cmdlevel 3+) are exempt.
- The pickup packet hook refuses a pickup unless the item's multi is the multi the player stands in (standing in any multi used to be enough).
- Not changed: redeed still does not return the lockdown; `.flip` has no ownership check.

**Expected impact:** You can only redeed furniture that is placed inside your own house, and loose items inside a house can only be lifted from inside that house.

### 30. Rituals: crystal mana is pooled (commit `9164261`, 2026-10-04)

**Files involved:** `pkg/opt/rituals/include/rituals.inc`

**Notable functional changes:**
- `ProcessRitual` collects up to `RITUAL_MAX_CRYSTALS` (4) filled mana crystals into `crystal_pool` and tests `GetMana( mobile ) + crystal_pool` against the ritual's mana floor (`gate_info[4]`; staff cmdlevel 4+ exempt). It used to pour each crystal into the caster with `SetMana`, which clamps to the maximum, so a crystal never helped. The crystals' `ManaLevel` is erased only after the floor check passes; a refused ritual keeps its crystals. The chant still drains a quarter of the caster's own current mana per line. New messages for the pooled amount, a repeated crystal and the shortfall.

**Expected impact:** A Mage with 500 mana and four charged crystals can meet the 2,500 floor of the hardest rituals; a refused ritual no longer burns the crystals.

### 31. Harvesting: the tree, ore and sand lists now come from the resource configs (working tree, 2026-10-05)

**Files involved:**
- `scripts/include/itemutil.inc` (`IsTree`, `IsMinable`, `IsSand`)

**Notable functional changes:**
- `IsTree` was four hardcoded objtype ranges matching 208 objtypes. `regions/wood.cfg` lists 574, having grown from 170 in the 2026-08-15 resource audit that verified it against `tiledata.mul`; the predicate never followed. It is the sole gate in `pkg/std/lumberjacking/lumberjack.src` (five sites) and `scripts/items/bladed.src` (five sites), so 366 objtypes had wood units allocated and regrowing that no axe or blade could reach. It is now the config's own list as 115 ranges, each carrying that entry's name from the config as a trailing comment. The ranges it used to accept but the config does not list (`0xc9f`-`0xca5`, `0xca7`, `0xcac`-`0xcc7`, `0xd37`-`0xd38`) are gone with it.
- `IsMinable`'s landtile branch was eighteen ranges matching 129 landtiles against `regions/ore.cfg`'s 199, so 70 ore tiles answered "You can't mine or dig anything there." It also accepted 13 tiles the config does not list, where mining ran against no resource region. Now the config's 199 as 24 named ranges. The first branch, on `othertype` for the cave-floor statics `0x053b`-`0x0553` except `0x0550`, is unchanged and still tested first.
- `IsSand` was seven ranges that matched `regions/sand.cfg` only partly - 44 configured tiles rejected, 54 accepted tiles absent from the config. Now the config's 187 as 21 named ranges.
- Overlap between the three, which matters because `mining.src:82-92` tests `IsSwamp`, then `IsMinable`, then `IsSand`, and the earlier branch silently takes the tile: `IsMinable` + `IsSand` now share nothing (the old `0x122`-`0x125` clash is gone) and `IsSwamp` + `IsSand` share nothing. `IsSwamp` + `IsMinable` share `0x250`, which is in `IsSwamp`'s hardcoded list and inside `ore.cfg`'s cave block; `IsSwamp` wins, so it digs clay and never ore, the same as before this change. `IsSwamp` is the one tile predicate with no resource config compared here (clay has `regions/clay.cfg`), and it was left alone.

**Expected impact:** The later-expansion trees (`0x309c`-`0x30de`, `0x39a3`-`0x3aef`, `0xa65c`-`0xa716`), the vines and the fallen logs become choppable; the cave and rock tiles added by the audit become mineable; the beach and desert tiles added by it become diggable for sand. Resource yields per tile are unchanged - the configs already governed those. A handful of tiles that were choppable or diggable without a matching resource region no longer respond at all, which is what the configs always said.

### 32. Crafting: refining materials, tinker jewellery, poisonable food (working tree, 2026-10-05)

**Files involved:**
- `scripts/include/itemutil.inc` (`IsWoodEquipment`, `IsLeatherEquipment`, `IsJewel`, `IsConsommable`, new `GetEdibleObjtypes`, `use cfgfile;` added)

**Notable functional changes:**
- `IsWoodEquipment`, the gate for a Refining Varnish in `pkg/opt/crafterboost/refiningitem.src`, listed five graphics twice (`0x0f4f`, `0x13fd`, `0x1403`, `0x6050`, `0xb201`) and omitted all six archery weapons added on 2026-08-23, so the varnish answered "Varnishes can only refine wooden weapons and armor." for every one of them. Deduplicated, and `0xa915` Shortbow, `0x26c2` CompositeBow, `0x26c3` RepeatingCrossbow, `0x27a5` Yumi, `0x2d1e` ElvenCompositeLongbow and `0xb4dc` SkullCrossbow added. Cross-checked against `bowcraft.cfg` and `carpentry.cfg`: every craftable wooden weapon and shield now matches.
- `IsLeatherEquipment`, the gate for a Refining Compound, matched every tailoring armour piece whose `MaterialType` is `Hide` but not the Shoes category or the Leather Misc masks. Added `0x170B` LeatherBoots, `0x170D` Sandals, `0x170F` Shoes, `0x1711` ThighBoots and the ten mask graphics `0x141B`/`0x141C` and `0x1545`-`0x154C`. `GetLeatherArmorGraphics` in the same file already counted the masks as leather armour, so the two lists had been contradicting each other.
- `IsJewel` listed `0x1085`-`0x108A` and the wristwatch `0x5015`, six of the twelve jewellery pieces `pkg/std/tinkering/tinker.cfg` makes. The other six were invisible to the four jewel branches in `pkg/opt/rituals/include/rituals.inc`, to `vitalInfusion.src`'s `skilladv` read and to the equipment block in `pkg/packethooks/megacliloc/itemdata.src:118`. Added `0x1F05` beaded necklace, `0x1F06` bracelet2, `0x1F07` earrings2, `0x1F08` necklace3, `0x1F09` ring2, `0x1F0A` silver necklace.
- `IsConsommable`, the gate for `PoisonFood` in `pkg/std/poisoning/poisoning.inc:104`, was nineteen hardcoded objtype ranges recognising 79 of the 138 distinct edible objtypes in `config/food.cfg`. The 59 misses included the fish steaks `0x09cc`-`0x09cf`, the raw fish `0x0dd6`-`0x0dd9`, the fruit `0x1a92`-`0x1a96` and the custom foods `0x30128`-`0x3012b`. It now reads `food.cfg` through a new `GetEdibleObjtypes()` and keeps the old potion ranges (`0xdc01`-`0xdc03`, `0xdc0b`-`0xdc16`) and custom dishes (`0xc900`-`0xc947`), which are not in food.cfg but are meant to be poisonable. Taste ID was never affected: `tasteid.src` falls back to the alchemy config.
- `GetEdibleObjtypes()` uses `GetConfigIntArray`, not `GetConfigStringArray` plus `CInt` - the engine converts config ints with `std::stoi( value, nullptr, 0 )` (`pol/module/cfgmod.cpp:532`), so food.cfg's `0x09d0` form comes back as a number. It caches into a file-scope `var _edible_objtypes := 0;` filled on first call rather than a file-scope initialiser, which would re-read the config at the start of every one of the 77 scripts that include `itemutil.inc`.

**Expected impact:** The six newer bows accept a Refining Varnish; leather footwear and the five carved masks accept a Refining Compound; the second tinker jewellery set works in rituals and shows its full tooltip; every food in `food.cfg` can be poisoned. No resource cost, skill check or success chance changed.

### 33. Class restrictions: Chaos and Order shields, female studded leather, the orc helm (working tree, 2026-10-05)

**Files involved:**
- `scripts/include/itemutil.inc` (`GetShieldGraphics`, `GetShieldGraphicsThief`, `GetStuddedLeatherArmorGraphics`, `GetPlatemailArmorGraphics`)

**Notable functional changes:**
- The restriction lists were compared against every `Armor` block in the merged itemdesc set (381 blocks across 68 `itemdesc.cfg` files). `EnumerateRestrictedItemTypesFromClasse` in `classes.inc` tests both `item.graphic` and `item.objtype`, so an entry covers reskins that share a graphic.
- `GetShieldGraphics` held the eleven stock shields. `0x1bc3` Chaosshield and `0x1bc4` Ordershield are equippable `Armor` blocks in the combat itemdesc and were in no list, so Bard, Mage, Mystic Archer, Ranger, Thief and Bladesinger could all carry one. Both graphics added, which also covers `0x86df` Chaosshieldguard, `0x86ef` Ordershieldguard and `0x8260` TourneyShield. `0x7ce2 ebardshield` shares graphic `0x1bc3` and becomes restricted with them; it has no creation path anywhere in the repo - no loot table, no vendor, no script - so nothing obtainable is affected.
- `GetStuddedLeatherArmorGraphics` held the male set only, so Bladesinger and Paladin could wear `0x1C02` FemaleStudded and `0x1C0C` StuddedBustier. Both added; that also covers `0x8249` SylvianFemaleStudded and `0x8275` TourneyFemaleStudded, which share graphic `0x1C02`.
- `GetPlatemailArmorGraphics` gained `0x1f0b`, the orc helm - a metal helm by `IsMetalEquipment` that belonged to no family list, so classes barred from plate could wear one. Covers `0x32d` KoboldOrchelm, same graphic.
- `GetShieldGraphicsThief` gained `0x982a` NavarBloodyBarrier, which every other shield-restricted class was already barred from. The buckler `0x1b73` stays off the thief list deliberately and now carries a comment saying so.
- Not changed: `GetGraphicsCrafter` still returns an empty set. It is a placeholder appended five times in `classes.inc` (Bard, Bladesinger, Mystic Archer, Warrior, Powerplayer) and contributes nothing; left as-is by decision.

**Expected impact:** Six classes can no longer equip a Chaos or Order shield, Bladesingers and Paladins can no longer wear studded leather in female form, five classes can no longer wear an orc helm, and Thieves lose the Navar shield. Characters already wearing one of these will have it moved to the backpack by `unequipRestrictedItems` (`classes.inc:926`), which runs whenever class level is recomputed, with the standing message that threatens a jail - see the pre-live note above.

### 34. itemutil.inc: dead code, the engine container search, DupeItem, the ingot/ore objtype clash (working tree, 2026-10-05)

**Files involved:**
- `scripts/include/itemutil.inc`, `scripts/include/objtype.inc`
- `pkg/opt/alchemyplus/alchemyplus.src`, `pkg/opt/alchemyplus/alchemyplus toad.src`, `scripts/textcmd/player/cast.src`, `pkg/std/poisoning/poisoning.inc`, `pkg/std/poisoning/poisoning.src`, `pkg/std/alchemy/alchemyfunctions.inc`, `pkg/opt/shilitems/infinitegems.src`, `pkg/opt/shilitems/infinitenormals.src`, `pkg/opt/shilitems/infinitepagans.src`
- `pkg/std/lumberjacking/lumberjack.src`, `pkg/std/mining/mining.src`, `pkg/multis/house/multiDeed/use.src`, `pkg/opt/guilds/include/guilds.inc`, `pkg/multis/customhousing/sign.src`, `pkg/multis/customhousing/signcontrol.src`
- Deleted: `pkg/utils/itemUtils/include/itemtypes.inc`
- `.claude/skills/escript-gotchas/SKILL.md`, `.claude/subagent-briefing.md`

**Notable functional changes:**
- Ten functions had no callers anywhere in the repo and are commented out in place, each with a line saying why: `GetPossiblePropertyNames`, `ConsumeObjType` (superseded by `resourcemanager.inc`), `CreateItemAt` (it started `:summoning:itemappear`, which does not exist), `CreateMagicCircleAround` and `PlayMagicCircleEffect`, `CreateWaterfall` and `PlayWaterfallEffect`, `FindRootItemInContainer` (superseded by `FindObjtypeInContainer` with `FINDOBJTYPE_ROOT_ONLY`), `ResetAllHitscriptPropsExcep`, and `FindItemInContainer` after the swap below. `pkg/opt/summoning/magiccircleappear.src` and `waterfallappear.src` consequently have no callers left; both are untouched.
- `FindItemInContainer` enumerated a container's whole tree into a script-side array and compared objtypes in interpreted code. Its 23 live call sites across nine files now call the engine's `FindObjtypeInContainer( container, objtype )` directly, taking the repo from 27 to 50 engine call sites. Verified equivalent against the engine first: both recurse into sub-containers (`containr.cpp:324` and `:477`) and both skip locked ones by default, and `BError::isTrue()` is false (`berror.cpp:95`) so a not-found error is falsy exactly like the old unset return. Every one of the 23 sites guards with `if( !x )`, `if( x )` or `x && y`, none compares to a literal or dereferences unguarded, and all pass a real container. The one behavioural difference: the helper walked depth-first, while the engine checks the whole top level (`find_toplevel_objtype`) before recursing, so where a container holds a match at both depths callers now get the top-level one.
- `DupeItem` copied fourteen members plus every cprop but not `usescript` - the second most script-assigned item member in the repo, 36 sites - nor `desc`, `sellprice` or `amount`. All four are now copied when set. Callers: the `.iteminfo` clone (two sites), `inscription.src` (two), `tailoringfunctions.inc` and `explosionlauncherscript.src`.
- `IsEquipped` renamed to `EquipIfNeeded`, with `lumberjack.src:30` and `mining.src:48` updated. It never only tested: if the item is not already in hand 1 or 2 it calls `EquipItem` and returns that, which is what both call sites rely on. No behaviour change.
- `IsInContainer` (root-only), `IsOnPlayer` (recursive) and the third, recursive local copy of `IsInContainer` in `multiDeed/use.src:387` now each carry a header comment naming their depth and pointing at the counterparts. Nothing in the names distinguished them across 33 call sites. No code change.
- `UOBJ_SHING_INGOT` (`0xc530`), `UOBJ_LEVIATHAN_INGOT` (`0xc531`) and `UOBJ_SANCTUARY_INGOT` (`0xc532`) sat inside `UOBJ_ORE3_START`..`UOBJ_ORE3_END` (`0xc530`-`0xc546`) as well, so `IsIngot()` and `IsOre()` both returned 1 for all three. All three were reserved with no itemdesc entry and referenced nowhere outside `objtype.inc`; they are removed and `UOBJ_INGOTS2_END` moves to `0xc52f` (`UOBJ_VULCAN_INGOT`), leaving the three slots to the ore range.
- `pkg/utils/itemUtils/include/itemtypes.inc` deleted (1,601 lines, 53 functions). It redefined `IsIngot`, `IsLog`, `IsOre`, `IsReagent`, `IsSign` and `IsInContainer` from `itemutil.inc`, and the only two `include` lines for it were already commented out (`customhousing/sign.src:10`, `signcontrol.src:7`), so it compiled into nothing; `guilds.inc:12` carried a note explaining it was deliberately avoided for exactly that reason. Those three references are updated so none points at a missing file.
- `IsHide` dropped `0x702f` and `0x7030`, both already inside `UOBJ_HIDES_START`..`UOBJ_HIDES_END` on the next branch. `IsSign` dropped the `0xbd0` and `0xbd2` cases, both already inside its own `0xba3`-`0xc0e` range. `DestroyTheItem` replaced `SetObjProperty( item, "Cursed", 3 )` with `SetCurseLevel( item, SETTING_CURSE_LEVEL_REVEALED_CAN_UNEQUIP )`. Same values, no behaviour change. Checked separately: all ten house-sign objtypes in the repo - regular, custom and static housing - are matched by `IsSign`, so nothing is missing from it.
- Developer notes: new gotcha 4a in the EScript gotchas skill and one line in the subagent briefing - a `ConfigFile` is iterable (`foreach elem in cfg` yields its named elements, `pol/module/cfgmod.cpp` `ConfigFileIterator`), `GetConfigIntArray` parses the `0x` form while `GetConfigStringArray` does not, and an old include may be missing `use cfgfile;`.

**Expected impact:** No player-visible effect except the container-search ordering noted above, which decides which of two matching items is consumed when a backpack holds one loose and one in a bag.

### 35. Item ID rolls on the class stand-in skill (working tree, 2026-10-05)

**Files involved:** `pkg/std/itemid/itemid.inc`

**Notable functional changes:**
- New `ItemIDClassSkillId( who )` returns the stand-in skill id per class (Bard `SKILLID_TASTEID`, Warrior `SKILLID_ANATOMY`, Thief `SKILLID_REMOVETRAP`, Crafter `SKILLID_ARMSLORE`, Ranger `SKILLID_CAMPING`, otherwise `SKILLID_ITEMID`; same if-chain order as before). `ItemIDClassSkill( who )` is now `GetEffectiveSkill( who, ItemIDClassSkillId( who ) )`; its two callers (the `do_itemid` wait skip at 100+ and the `SelectItemToID` batch gate at 100+ skill and 100+ INT) are unchanged.
- `ItemID()` rolls `CheckSkill( who, skillid, -2, thepoints )` on that id instead of the hard-coded `SKILLID_ITEMID`. `thepoints` is `get_default_points( SKILLID_ITEMID )` only when the id is Item ID; a stand-in class rolls with 0 points, so `SkillAsPercentSkillCheck` awards nothing on success and `AwardSkillPoints` is reached only with 0 points (a double fail below 25 skill, which still rolls that skill's StrAdv/IntAdv stat chance but moves no skill). The chance formula is unchanged (skill - 15, clamped 2..98) and the `IsSpecialisedIn` second chance now applies to the stand-in, since each stand-in is a class skill of its class. An arrow-down stand-in drops 0.1 per attempt through `ShilCheckSkill`, as any check on a dropping skill does.
- Not changed: `scripts/misc/merchantidentify.src` (own `ItemID()` copy, no roll), `pkg/opt/shilitems/wandofid.src` (no roll), `pkg/packethooks/megacliloc/itemdata.src` (uses `CanBeIDed` only). The roll used Item ID in every commit since `f8506ea` and in POL2.5 (`scripts/include/itemid_core.inc:65`), so this is a design change, not a regression fix.

**Expected impact:** A Warrior with 150 Anatomy identifies with the same 98% chance as a Mage with 150 Item ID (it was 2% with Item ID at 0); at 100 Anatomy, 85%. Identifying gives Warriors, Bards, Thieves, Crafters and Rangers no skill gain; Mages and everyone else rolling on Item ID itself gain as before. Not compiled by the author.

### 36. Fact-check follow-ups: drinks poisonable again, Thief and Ranger shield rules, Rudyom (working tree, 2026-10-05)

**Files involved:** `scripts/include/itemutil.inc`, `scripts/include/classes.inc`, `config/npcdesc.cfg`

**Notable functional changes:**
- `IsConsommable` (itemutil.inc): the four drink ranges the 3.1.2 list carried are back after the food.cfg rewrite of theme 32 dropped them, since nothing eats a drink so food.cfg never lists one: bottles 0x099b/0x099f/0x09c7/0x09c8, mugs 0x09ee-0x09f0, glasses and pitchers 0x1f7d-0x1f9e (milk and water included), plus onion rings 0xc950 and the alchemyplus potion 0x7059. That is the 40 objtypes the fact-check found lost; Poisoning and Taste ID (`tasteid.src:38`) both read this function.
- `GetShieldGraphicsThief` (itemutil.inc): 0x1bc3 Chaosshield and 0x1bc4 Ordershield added, so Thieves are barred like every other shield-restricted class (they had been added to `GetShieldGraphics` only).
- `HaveRestrictedItem...` Ranger branch (classes.inc): tests `item.objtype in EnumerateRestrictedItemTypesFromClasse( CLASSEID_RANGER )` as every other class does. The Navar Bloody Barrier is objtype 0x982a with graphic 0x2B01, so the graphic-only test never barred Rangers from it.
- `config/npcdesc.cfg`: the `rudyom` header comment now says Faymoor, matching `RitualQuests.md`, the quest table in theme 1 and the patch notes; the comment had said the Aetherfall Bridge hub. The template has no start position, the NPC is placed by hand.

**Expected impact:** Drinks, milk, onion rings and the alchemyplus potion can be poisoned and tasted again. Thieves cannot carry Chaos or Order shields; Rangers cannot carry the Navar Bloody Barrier (and anything else listed by objtype in the plate and shield lists). Not compiled by the author.

---

## Validation Notes

- Diff range: `git log --graph Patch-3.1.2..HEAD`, `git diff --name-status -M Patch-3.1.2..HEAD` (the inventory above; HEAD is `9164261`), `git diff --numstat -M` and `git diff --shortstat` for the totals and largest shifts, `git show --stat` on each non-merge commit, and `git diff a4336ef e484311` (empty) to confirm the re-merge commits carry nothing.
- File-status counts were derived programmatically from `git diff --name-status -M a4336ef..9164261`: 22 A / 197 M / 12 D / 21 R = 252.
- Template counts come from `^\s*NpcTemplate\s` headers: 1,433 at `Patch-3.1.2`, 1,452 at `85f13a2`, 1,455 in the working tree after the reformat. HITS/MANA/STAM against old effective values were compared programmatically for every template; the three differences are the ones listed in theme 9. The reformat (theme 18) was parity-checked entry by entry against the pre-reformat file before writing (scratchpad `npcdesc_verify.py`: 0 mismatches).
- Teleporter counts come from active and commented `{ x, y, z, ... }` rows in `teleporters.inc` at both ends of the range.
- Working tree was clean at `9164261` for themes 1-30. Themes 31-34 are the uncommitted working tree on top of it, read from `git diff -- scripts/ pkg/ .claude/` plus the staged deletion (20 files, +724 / -2,038); `patchnotes/` and `ainotes/` changes are excluded from that count. Themes 22-30 were read from `git diff 68a5c45..9164261` (71 files) and checked against the closed-beta test pages' open-question lists; none of the code was compiled or run by the author.
- Theme 20: the engine behaviour was read from the upstream clone (`bscript/bdict.cpp` for dictionary iteration, `pol/network/packethooks.cpp` for hook return values, `pol/pol.cpp` `call_chr_scripts`/`char_select` and `pol/network/clientthread.cpp` for logon/logoff order, `pol/login.cpp`, `pol/packetscrobj.cpp`) and from the shard's own `core-changes.txt` (`SetInt8` on a fixed-length packet, `account.defaultcmdlevel`). Both bugs match the refusals in `log/pol.log` of 2026-10-01 (same-second logins refused for "multiple IPs"; household accounts refused alone). The scripts were not compiled and not run by Claude; the checks are Shard Test Board t187-t202 and `patchnotes/accounts_household_qa_runsheet.md`.
- Themes 31-34: the lists were not eyeballed. Every hardcoded objtype, graphic and landtile list in `itemutil.inc` was compared programmatically against the shard's own data - 6,019 itemdesc blocks parsed from all 68 `itemdesc.cfg` files, `blacksmithy.cfg` / `tailoring.cfg` / `bowcraft.cfg` / `carpentry.cfg`, `tinker.cfg`, `config/food.cfg`, and the `Global` blocks of `regions/wood.cfg`, `ore.cfg` and `sand.cfg` (scratchpad scripts `armor_check.py`, `refine_check.py`, `resource_check.py`, `tile_check.py`, `emit_escript.py`). Caller counts for every one of the 49 functions come from a word-boundary grep over `.src`, `.inc` and cfg. Engine behaviour was read in the upstream clone: `pol/containr.cpp` (`enumerate_contents`, `find_objtype`), `pol/core.h` (the `ENUMERATE_*` and `FINDOBJTYPE_*` flags), `pol/module/uomod.cpp` (`mf_FindObjtypeInContainer`), `pol/module/cfgmod.cpp` (`ConfigFileIterator`, `mf_GetConfigIntArray`), `pol/cfgrepos.cpp` and `bscript/escrutil.cpp` (`bobject_from_string`), and `bscript/berror.cpp` (`BError::isTrue`). Nothing was compiled or run; `scripts/include/itemutil.inc` is clean in the VS Code language extension, which is what caught a missing `use cfgfile;`. The exact per-fix record is `ainotes/itemutil-audit-fixlog-20261004.md` with a companion `.diff`.
- Theme 21: the verse skills come from `pkg/opt/versebook/verses.cfg` (identical in the POL2.5 clone); the drop rule and the two lock systems were read in `scripts/include/skillpoints.inc`, `pkg/systems/attributes/hooks/shilhook.src`, `scripts/misc/skillwin.src` (dead gump code, live `SendSkillWindow`), `scripts/textcmd/player/skills.src` and the engine's `pol/irequest.cpp` (`handle_skill_lock`, `CoreHandledLocks`) and `pol/network/packethooks.cpp` (a 6-byte hook on 0x3A is accepted). Not compiled and not run by Claude; the checks are Shard Test Board t203-t207.
- Theme 35: `pkg/std/itemid/itemid.inc` is one more uncommitted file on top of the 20 counted for themes 31-34 (`git diff --numstat`: +27 / -9). The roll path was read in `pkg/systems/attributes/hooks/shilhook.src` (`ShilCheckSkill` -> `SkillAsPercentSkillCheck`) and `scripts/include/skillpoints.inc` (`AwardSkillPoints` -> `AwardPoints`); the history in `git log -- pkg/std/itemid` (every commit since `f8506ea` rolled on `SKILLID_ITEMID`) and the POL2.5 clone (`scripts/include/itemid_core.inc`). Not compiled and not run by Claude; the in-game check is a Warrior with Anatomy 150 and Item ID 0 identifying a magic item (expect near-certain success, no Anatomy or Item ID gain).
- Fact-check of `patch-v3.1.3.md` against the code, 2026-10-05: nine read-only passes over every section (scratch reports `verify_1..9`, not in the repo); about 190 bullets, roughly 50 reworded, 3 found false (teleporters are never recreated at boot, Taste ID follows the poisonable-food list, the removed tree/ore tiles did produce before). Code questions left for the shard owner, not changed: the Aetherfall Mine -> Faymoor return teleporters were removed with no replacement (one-way mine); the Secret Retreat has no teleporter leading in; drinks (bottles, mugs, glasses, pitchers, milk) and onion rings are no longer poisonable after the `IsConsommable` rewrite; `GetShieldGraphicsThief` lacks the Chaos/Order shields 0x1bc3/0x1bc4; the Ranger shield check tests graphics only, so the Navar Bloody Barrier (graphic 0x2B01) never barred Rangers; `orcbrute` and `rhinocerosbeetle` carried two `FireProtection` CProps (i50 then i20) and the reformat kept the first where the engine used the last; `pkg/opt/rituals` calls `item.IsJewel()` as an object method with no Item SystemMethod registered (pre-existing); Rudyom's template comment places him at the Aetherfall Bridge hub, the notes say Faymoor.
- Decisions of 2026-10-05 on the fact-check questions above: teleporters (one-way Aetherfall Mine, Secret Retreat entrance) deferred; drinks restored, Thief Chaos/Order and Ranger Navar fixed, Rudyom comment corrected (theme 36); orcbrute/rhinocerosbeetle FireProtection 50 accepted as is; `item.IsJewel()` left as a reminder; quest progress not carried over (beta).
