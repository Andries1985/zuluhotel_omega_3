# Developer Changelog - v3.0.3

Range: Patch-3.0.2..Patch-3.0.3  
Branch: Patch-3.0.3  
Date: 2026-07-05

---

## Scope Summary

- Total files changed: 113
- Text/config/script files changed: 113
- Net textual delta: 30582 insertions, 6875 deletions
- Branch commits in range: Many Changes; Teleporters Update; new command delete dungteles; Teleporters added; Container fixes; Wand of ID fix; Client update; Added heating stands and recall runes Spellbook too heavy error fix; Item ID Fixes brought over from 2.5 removed extra dagger from starting char areas ok button added and allowed cancelling of gump with right click mountstone fixes System Moongate Fix for Titles Removed mountstone mounts on death removed the ability to create gargoyles or elves Fixed paladins to use macefighting again; animated graphics change; Animated graphic included; Animation Test update

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - |
|---|---|---:|---:|
| M | config/command_synopses.cfg | 14 | 0 |
| M | config/equip.cfg | 13 | 0 |
| M | config/mrcspawn.cfg | 2 | 1 |
| M | config/nlootgroup.cfg | 443 | 177 |
| M | config/npcdesc.cfg | 20838 | 1490 |
| A | patchnotes/buff-alchemy-parity-audit-zh25-vs-zh30.md | 152 | 0 |
| A | patchnotes/modtoken-exhaustive-parity-report.md | 59 | 0 |
| A | patchnotes/test-plans/v3.0.3-regression-ingame-test-plan.md | 311 | 0 |
| M | pkg/items/containers/config/itemdesc.cfg | 27 | 29 |
| M | pkg/opt/GMItems/staffofnagash_usescript.src | 8 | 6 |
| M | pkg/opt/alchemyplus/newpotions.src | 110 | 39 |
| A | pkg/opt/alryc/textcmd/test/animatedgraphics.src | 126 | 0 |
| M | pkg/opt/areas/textcmd/admin/areas.src | 131 | 32 |
| M | pkg/opt/champspawns/config/spawns.cfg | 47 | 9 |
| M | pkg/opt/champspawns/include/death.inc | 2 | 6 |
| M | pkg/opt/champspawns/include/rewards.inc | 7 | 7 |
| M | pkg/opt/champspawns/include/skulls.inc | 5 | 12 |
| M | pkg/opt/champspawns/include/spawning.inc | 12 | 8 |
| M | pkg/opt/champspawns/include/titles.inc | 5 | 3 |
| M | pkg/opt/champspawns/scripts/walkOnChampReject.src | 1 | 1 |
| M | pkg/opt/champspawns/scripts/walkOnChampRejectDestard.src | 1 | 1 |
| M | pkg/opt/earth/earthblessing.src | 11 | 18 |
| M | pkg/opt/earth/shapeshift.src | 65 | 105 |
| M | pkg/opt/holybook/enlightenment.src | 12 | 26 |
| M | pkg/opt/holybook/seraphimswill.src | 8 | 82 |
| M | pkg/opt/moongates/systemmoongate.src | 2 | 2 |
| M | pkg/opt/necro/spellbind.src | 18 | 0 |
| M | pkg/opt/omegacache/omegacache.inc | 135 | 5 |
| M | pkg/opt/powerscrolls/itemdesc.cfg | 1 | 0 |
| M | pkg/opt/shilitems/wandofid.src | 53 | 13 |
| M | pkg/opt/songbook/songofdefense.src | 5 | 2 |
| M | pkg/opt/songbook/songofglory.src | 11 | 3 |
| M | pkg/opt/songbook/songofhaste.src | 25 | 2 |
| M | pkg/opt/spawnpoint/checkpoint.src | 44 | 31 |
| M | pkg/opt/spawnpoint/config/groups.cfg | 11 | 1 |
| M | pkg/opt/summoning/polymorphing.src | 5 | 5 |
| M | pkg/opt/summoning/processpersistedmod.src | 1 | 0 |
| M | pkg/opt/townstones/electionwatch.src | 183 | 85 |
| M | pkg/opt/townstones/logon.src | 37 | 18 |
| M | pkg/opt/townstones/start.src | 0 | 5 |
| M | pkg/opt/townstones/textcmd/admin/cleartownmembers.src | 142 | 38 |
| M | pkg/opt/townstones/textcmd/admin/createtownstone.src | 300 | 97 |
| M | pkg/opt/townstones/textcmd/admin/fixstone.src | 100 | 46 |
| M | pkg/opt/townstones/textcmd/admin/gettowngold.src | 118 | 21 |
| M | pkg/opt/townstones/textcmd/admin/removetownmember.src | 212 | 54 |
| M | pkg/opt/townstones/textcmd/admin/resetpoll.src | 109 | 45 |
| M | pkg/opt/townstones/textcmd/admin/townbankstatus.src | 496 | 216 |
| M | pkg/opt/townstones/townlistbootstrap.src | 48 | 77 |
| M | pkg/opt/townstones/tstone.inc | 248 | 836 |
| M | pkg/opt/townstones/tstone.src | 595 | 282 |
| M | pkg/opt/vanityshop/itemdesc.cfg | 120 | 0 |
| M | pkg/opt/vanityshop/mountstone.src | 48 | 11 |
| M | pkg/opt/vanityshop/vanityshop.src | 87 | 26 |
| M | pkg/std/alchemy/bluepotion.src | 15 | 4 |
| M | pkg/std/alchemy/whitepotion.src | 16 | 5 |
| M | pkg/std/cartography/cartography.src | 98 | 64 |
| M | pkg/std/detecthidden/detecthidden.src | 2 | 2 |
| A | pkg/std/dundee/globeofsosaria.src | 157 | 0 |
| M | pkg/std/itemid/itemid.inc | 47 | 25 |
| M | pkg/std/itemid/itemid.src | 4 | 54 |
| M | pkg/std/peacemaking/peacemaking.src | 1 | 1 |
| M | pkg/std/removetrap/removetrap.src | 12 | 1 |
| M | pkg/std/snooping/stealing.src | 5 | 5 |
| M | pkg/std/spells/SpellBook_Can_Insert.src | 12 | 0 |
| M | pkg/std/spells/archprot.src | 17 | 2 |
| M | pkg/std/spells/dispel.src | 1 | 0 |
| M | pkg/std/spells/massdispel.src | 1 | 0 |
| M | pkg/std/spells/protection with timer.src | 10 | 0 |
| M | pkg/std/spells/protection.src | 10 | 0 |
| M | pkg/std/spells/reveal.src | 15 | 280 |
| M | pkg/std/stealing/stealing.src | 11 | 17 |
| M | pkg/std/treasuremap/digtreasure.src | 27 | 24 |
| M | pkg/systems/combat/avengingonhit.src | 15 | 1 |
| M | pkg/systems/combat/banishonhit.src | 1 | 0 |
| M | pkg/systems/combat/banishscript.src | 1 | 0 |
| M | pkg/systems/combat/blackrockscript.src | 1 | 0 |
| M | pkg/systems/combat/bouncingonhit.src | 13 | 10 |
| M | pkg/systems/combat/config/enchantableitems.cfg | 72 | 47 |
| M | pkg/systems/combat/config/itemdesc.cfg | 87 | 4 |
| M | pkg/systems/combat/config/modenchantdesc.cfg | 1 | 1 |
| M | pkg/systems/combat/config/onhitscriptdesc.cfg | 13 | 13 |
| A | pkg/systems/combat/critstaminahit.src | 81 | 0 |
| M | pkg/systems/combat/deflectiononhit.src | 16 | 24 |
| M | pkg/systems/combat/dualplanaronhit.src | 71 | 30 |
| M | pkg/systems/combat/dualplanarscript.src | 23 | 18 |
| M | pkg/systems/combat/include/hitscriptinc.inc | 166 | 84 |
| M | pkg/systems/combat/lifedrainscript.src | 1 | 0 |
| M | pkg/systems/combat/slayerscript.src | 4 | 48 |
| M | pkg/systems/combat/spellonhit.src | 9 | 8 |
| M | pkg/systems/combat/spellstrikescript.src | 2 | 1 |
| M | pkg/systems/combat/trielementalonhit.src | 43 | 27 |
| M | pkg/systems/combat/trielementalscript.src | 11 | 6 |
| M | regions/regions.cfg | 2 | 2 |
| A | scripts/ai/soulwhisperer.src | 525 | 0 |
| M | scripts/include/classes.inc | 39 | 17 |
| M | scripts/include/client.inc | 10 | 1 |
| M | scripts/include/creature_spellcast.inc | 1 | 1 |
| M | scripts/include/damages.inc | 17 | 0 |
| M | scripts/include/dismount.inc | 4 | 0 |
| M | scripts/include/dotempmods.inc | 114 | 11 |
| M | scripts/include/itemutil.inc | 8 | 0 |
| M | scripts/include/spelldata.inc | 138 | 26 |
| M | scripts/include/starteqp.inc | 2673 | 2010 |
| M | scripts/include/teleporters.inc | 266 | 0 |
| M | scripts/misc/chrdeath.src | 4 | 0 |
| M | scripts/misc/logoff.src | 0 | 1 |
| M | scripts/misc/merchantidentify.src | 48 | 22 |
| M | scripts/misc/oncreate.src | 27 | 0 |
| M | scripts/misc/skillwin.src | 4 | 2 |
| A | scripts/misc/soulwhispererportal.src | 28 | 0 |
| A | scripts/textcmd/admin/deletedungteles.src | 13 | 0 |
| M | scripts/textcmd/player/skills.src | 4 | 2 |
| M | scripts/textcmd/test/animationtest.src | 1 | 1 |

---

## Detailed Changes By Theme

### 1) Combat and Damage Pipeline Parity Pass

Files involved (high-impact set):
- pkg/systems/combat/include/hitscriptinc.inc
- pkg/systems/combat/critstaminahit.src
- pkg/systems/combat/slayerscript.src
- pkg/systems/combat/trielementalonhit.src
- pkg/systems/combat/dualplanaronhit.src
- pkg/systems/combat/deflectiononhit.src
- pkg/systems/combat/bouncingonhit.src
- pkg/systems/combat/config/enchantableitems.cfg
- pkg/systems/combat/config/itemdesc.cfg
- scripts/include/damages.inc
- scripts/include/dotempmods.inc
- scripts/include/spelldata.inc

Behavior changes:
- Restored/adjusted multiple physical and magic on-hit paths for parity with expected shard behavior.
- Added missing `:combat:critstaminahit` implementation and integrated it into combat resolution paths.
- Reworked slayer/elemental/planar interaction paths and associated configuration references.
- Updated shared combat includes for safer target filtering and consistent modifier handling.

### 2) Class, Race, and Character Lifecycle Updates

Files involved (high-impact set):
- scripts/include/classes.inc
- scripts/include/starteqp.inc
- config/equip.cfg
- scripts/misc/chrdeath.src
- scripts/misc/logoff.src
- scripts/include/dismount.inc
- scripts/textcmd/player/skills.src
- regions/regions.cfg

Behavior changes:
- Applied class/race parity updates and related equipment/start-state flow changes.
- Updated startup equipment pipelines with large data/config revisions.
- Tightened death/logoff/dismount lifecycle handling to prevent incorrect retained mount states.
- Added class/title/world interaction follow-up changes and restrictions alignment.

### 3) Teleporter and World Interaction Tooling

Files involved (high-impact set):
- scripts/include/teleporters.inc
- scripts/textcmd/admin/deletedungteles.src
- pkg/opt/areas/textcmd/admin/areas.src

Behavior changes:
- Expanded teleporter definitions and helper coverage.
- Added admin command `.deletedungteles` for dungeon teleporter cleanup.
- Updated area tooling to align with revised world/teleporter behavior.

### 4) Item Identification, Containers, and Utility Content

Files involved (high-impact set):
- pkg/std/itemid/itemid.inc
- pkg/std/itemid/itemid.src
- pkg/opt/shilitems/wandofid.src
- scripts/misc/merchantidentify.src
- pkg/items/containers/config/itemdesc.cfg
- config/mrcspawn.cfg
- pkg/std/spells/SpellBook_Can_Insert.src
- pkg/std/dundee/globeofsosaria.src

Behavior changes:
- Applied identification system parity fixes across item-id scripts and wand behavior.
- Updated merchant identify handling and related item utility flows.
- Added/fixed supporting world items/content (including recall rune/heating stand support paths).
- Revised container/item descriptors where required for consistency.

### 5) Townstones and Governance Maintenance Follow-up

Files involved:
- pkg/opt/townstones/electionwatch.src
- pkg/opt/townstones/logon.src
- pkg/opt/townstones/start.src
- pkg/opt/townstones/townlistbootstrap.src
- pkg/opt/townstones/tstone.inc
- pkg/opt/townstones/tstone.src
- pkg/opt/townstones/textcmd/admin/cleartownmembers.src
- pkg/opt/townstones/textcmd/admin/createtownstone.src
- pkg/opt/townstones/textcmd/admin/fixstone.src
- pkg/opt/townstones/textcmd/admin/gettowngold.src
- pkg/opt/townstones/textcmd/admin/removetownmember.src
- pkg/opt/townstones/textcmd/admin/resetpoll.src
- pkg/opt/townstones/textcmd/admin/townbankstatus.src

Behavior changes:
- Continued townstone admin/member/treasury maintenance and flow hardening.
- Updated election/start/bootstrap state flow to improve reliability.
- Refined admin command handling and data/state consistency.

### 6) Spell/Skill and Supporting Systems Parity Sweep

Files involved (sample):
- pkg/opt/earth/shapeshift.src
- pkg/opt/holybook/enlightenment.src
- pkg/opt/holybook/seraphimswill.src
- pkg/opt/songbook/songofhaste.src
- pkg/std/spells/reveal.src
- pkg/std/cartography/cartography.src
- pkg/std/stealing/stealing.src
- pkg/std/treasuremap/digtreasure.src
- pkg/opt/champspawns/include/spawning.inc
- pkg/opt/champspawns/include/rewards.inc

Behavior changes:
- Adjusted multiple spell and skill scripts for parity, validation, and targeting consistency.
- Applied no-PK/party/pet safety handling improvements across affected pathways.
- Updated champ/spawn/auxiliary systems to align with revised behavior.

### 7) Documentation, Testing, and Command Surface Updates

Files involved:
- config/command_synopses.cfg
- patchnotes/buff-alchemy-parity-audit-zh25-vs-zh30.md
- patchnotes/modtoken-exhaustive-parity-report.md
- patchnotes/test-plans/v3.0.3-regression-ingame-test-plan.md
- pkg/opt/alryc/textcmd/test/animatedgraphics.src
- scripts/textcmd/test/animationtest.src
- scripts/ai/soulwhisperer.src
- scripts/misc/soulwhispererportal.src

Behavior changes:
- Expanded command synopsis and test/support command coverage for current systems.
- Added new regression planning/audit documents for this patch cycle.
- Added/updated world utility and test scripts (animated graphics and soul whisperer support).

---

## Validation Notes

- Diff range used: Patch-3.0.2..Patch-3.0.3
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - git diff --shortstat
  - git log --oneline --no-merges
- The changelog is file-complete for the Patch-3.0.3 branch delta.
