# Developer Changelog - v3.1.3

Range: Patch-3.1.2..Patch-3.1.3 (commit `a4336ef`..`e0ff379`) plus the uncommitted npcdesc reformat (theme 18) on `Patch-3.1.3` as of 2026-09-30
Branch: Patch-3.1.3
Date: 2026-09-30

---

## Scope Summary

- Total files changed: 155 (148 in the committed range: 14 added, 103 modified, 10 deleted, 21 renamed; plus 17 tracked files with uncommitted theme-18 edits, 12 of which are also in the range, and 2 untracked new files).
  - Committed (`a4336ef..e0ff379`): 148 files (14 A, 103 M, 10 D, 21 R), +34,731 / -20,313.
  - Uncommitted working tree on top of `e0ff379` (theme 18 only): 17 tracked files (all M), +53,027 / -49,391 (almost all of it the `config/npcdesc.cfg` rewrite), plus 2 untracked new files (`ainotes/npcdesc-reformat-report-2026-09-30.md`, `scripts/include/npcdifficulty.inc`, +336).
- Net textual delta: +88,094 / -69,704. Of that, 14,479 insertions are the purely additive `config/landtiles.cfg` registration (theme 5), about 6,900 changed lines are the `decoratefacets` door file renumbering after 71 removals (theme 5), and +53,027 / -49,391 is the npcdesc.cfg block rewrite (theme 18, every line of every template moved). No binaries changed.
- Largest shifts:
  - `config/landtiles.cfg` (+14,479) - 1,385 new land tile definitions for the new dungeon terrain
  - `config/npcdesc.cfg` (+52,892 / -49,286 against `Patch-3.1.2`) - every one of the 1,455 template blocks rewritten into one layout (theme 18, uncommitted) on top of HITS/MANA/STAM on every template (commit `e0ff379`), 21 new quest-giver and boss templates, 6 Chaos Lord templates (commit `e0ff379`), dead `virtue`/`mountspawn`/`CustomHitsLevel` lines and two dead templates removed
  - `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (+3,012 / -3,864) - 71 stale door placements removed; the rest is index renumbering
  - `scripts/textcmd/gm/newiteminfo.src` (-2,674, deleted) and `scripts/textcmd/admin/iteminfo.src` (-351, deleted) - replaced by the promoted `.iteminfo`
  - `pkg/opt/rituals/config/itemdesc.cfg` (+905) - 28 new ritual altar items and 14 new chant books
  - `pkg/opt/quests/ai/sutek.src` (+528, new) - Soul Whisperer copy for the final ritual boss
  - `pkg/opt/quests/config/quests.cfg` (+342 / -22) - quests 12-25
  - `config/nlootgroup.cfg` (+253) - lootgroups 308-313 for the Chaos Lords (commit `e0ff379`)
  - `pkg/opt/quests/questlogs/RitualQuests.md` (+245, new) and `FishingQuests.md` (+113, new) - design records, not game content
  - `scripts/include/teleporters.inc` (+213 / -55) - new dungeon network, coordinate fixes, Ter Mur links disabled, placement bug fix
  - `scripts/textcmd/gm/iteminfo.src` (+205 / -61, renamed from `alryciteminfo.src`) and `scripts/textcmd/seer/info.src` (+189 / -31) - staff tools
  - `scripts/include/virtue.inc` (-390), `scripts/ai/setup/questiesetup.inc` (-377), `scripts/ai/main/questiesetup.inc` (-365), `config/spawndef.cfg` (-541), `pkg/std/dundee/codex.cfg` (-206) - dead code removed (commit `e0ff379`)
- Non-merge commits in range (oldest to newest):
  - `7b4c2c5` Animation test fix (alryc99, 2026-09-26) - `animationtest` template only (theme 8)
  - `d87c817` Teleporters and info newiteminfo updates (alryc99, 2026-09-27) - new dungeon teleporters, door decoration cleanup, staff tools, regen enchant naming (themes 4-7)
  - `8b0c9ff` Tele fixes (alryc99, 2026-09-27) - teleporter coordinates (theme 4)
  - `135ceb3` Final ritual quests created / Some ritual gating updates (alryc99, 2026-09-28) - quests 12-25, package rename, ritual gating (themes 1-3)
  - `941d8ba` Teleporter fixes (Sylvash, 2026-09-28) - teleporter coordinates (theme 4)
  - `e4fd8fc` Fixed doors and added new landtiles (Sylvash, 2026-09-28) - door items and land tiles (theme 5)
  - `85f13a2` Tele fixes (alryc99, 2026-09-29) - Ter Mur links, teleporter placement fix, briefing note (themes 4, 17)
  - `e0ff379` Patch Notes (alryc99, 2026-09-30) - vitals migration, Warrior for Hire vitals, Chaos Lord templates, karma sign, quest-giver snooping, dead NPC/config/virtue/mountspawn removals, staff-tool consolidation, these release notes (themes 6, 9-16)
- Merge commits: `ab0f893` (PR #110) and `e484311` (PR #111) re-merge `Patch-3.1.2`; the tree at `e484311` is identical to `a4336ef`. `5097add` (PR #112), `3b8b0fe`, `413aa02`, `4eb2b83` and `cff72b7` (PR #114) carry only the non-merge commits listed above.

**Before this build goes live:**
- **Move the quest progress datafile.** The quest package was renamed from `questpkg` to `quests` (theme 2), so its datafile moves from `data/ds/questpkg/queststate.txt` to `data/ds/quests/queststate.txt`. Without moving it, every character's progress on quests 2-11 (live since 3.0.9/3.1.1) and the fishing quests 100-107 silently resets.
- **Replace Valthor and Ysolde.** Their templates are gone (renamed `mariah` and `jaana`, theme 1) and `scripts/ai/valthor.src` is deleted. Any spawnpoint or placed NPC using `valthor` or `ysolde` must be re-placed with the new template.
- **Wipe and respawn NPCs** after the vitals migration (theme 9) and the npcdesc reformat (theme 18). There is no migration command. Existing NPCs keep their cached vitals and any old `CustomHitsLevel` / `BaseHpRegen` CProp is ignored; snoop/steal keep working on old NPCs through their old CProps. Areaspawner custom-NPC definitions convert their regen values on first load.
- Nothing in `e0ff379` or in the uncommitted theme 18 has been compiled or run. `ecompile` on the whole tree and an in-game pass over the "Expected impact" lines below are still owed.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File` (A=added, M=modified, D=deleted, R=renamed as `old -> new`). "uncommitted" marks working-tree-only changes; "committed + uncommitted edits" marks files changed in both.

- M | .claude/subagent-briefing.md (commit `e0ff379` + uncommitted, theme 18)
- A | ainotes/npcdesc-reformat-report-2026-09-30.md (uncommitted, theme 18)
- M | config/command_synopses.cfg
- M | config/equip.cfg (commit `e0ff379`)
- M | config/landtiles.cfg
- M | config/nlootgroup.cfg (commit `e0ff379`)
- M | config/npcdesc.cfg (commit `e0ff379` + uncommitted, theme 18)
- D | config/spawndef.cfg (commit `e0ff379`)
- M | config/speechgroup.cfg (commit `e0ff379`)
- M | pkg/items/doors/config/itemdesc.cfg
- A | pkg/opt/alryc/textcmd/test/regeninfo.src
- M | pkg/opt/areaspawner/include/areaspawner.inc (commit `e0ff379` + uncommitted, theme 18)
- M | pkg/opt/areaspawner/include/areaspawnergump.inc (commit `e0ff379` + uncommitted, theme 18)
- M | pkg/opt/songbook/songoffright.src (uncommitted, theme 18)
- M | pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg
- A | pkg/opt/quests/ai/erethian.src
- R | scripts/ai/fishmonger.src -> pkg/opt/quests/ai/fishmonger.src
- R | scripts/ai/ysolde.src -> pkg/opt/quests/ai/jaana.src
- A | pkg/opt/quests/ai/julia.src
- A | pkg/opt/quests/ai/kalabar.src
- R | scripts/ai/lobsterman.src -> pkg/opt/quests/ai/lobsterman.src
- A | pkg/opt/quests/ai/mariah.src
- R | scripts/ai/nystul.src -> pkg/opt/quests/ai/nystul.src
- A | pkg/opt/quests/ai/rudyom.src
- A | pkg/opt/quests/ai/sutek.src
- A | pkg/opt/quests/ai/thragg.src
- R | pkg/opt/questpkg/config/icp.cfg -> pkg/opt/quests/config/icp.cfg
- R | pkg/opt/questpkg/config/quests.cfg -> pkg/opt/quests/config/quests.cfg
- R | pkg/opt/questpkg/include/questdeath.inc -> pkg/opt/quests/include/questdeath.inc
- R | pkg/opt/questpkg/include/questdebuggump.inc -> pkg/opt/quests/include/questdebuggump.inc
- R | pkg/opt/questpkg/include/questfishing.inc -> pkg/opt/quests/include/questfishing.inc
- R | pkg/opt/questpkg/include/questjournal.inc -> pkg/opt/quests/include/questjournal.inc
- R | pkg/opt/questpkg/include/questmapgump.inc -> pkg/opt/quests/include/questmapgump.inc
- R | pkg/opt/questpkg/include/questnpcgump.inc -> pkg/opt/quests/include/questnpcgump.inc
- R | pkg/opt/questpkg/include/questpkg_gumpstyle.inc -> pkg/opt/quests/include/questpkg_gumpstyle.inc
- R | pkg/opt/questpkg/include/questspawncheck.inc -> pkg/opt/quests/include/questspawncheck.inc
- R | pkg/opt/questpkg/include/queststate.inc -> pkg/opt/quests/include/queststate.inc
- R | pkg/opt/questpkg/pkg.cfg -> pkg/opt/quests/pkg.cfg
- A | pkg/opt/quests/questlogs/FishingQuests.md
- A | pkg/opt/quests/questlogs/RitualQuests.md
- R | pkg/opt/questpkg/textcmd/gm/questcatch.src -> pkg/opt/quests/textcmd/gm/questcatch.src
- R | pkg/opt/questpkg/textcmd/gm/questmap.src -> pkg/opt/quests/textcmd/gm/questmap.src
- R | pkg/opt/questpkg/textcmd/gm/questtag.src -> pkg/opt/quests/textcmd/gm/questtag.src
- R | pkg/opt/questpkg/textcmd/test/queststate.src -> pkg/opt/quests/textcmd/test/queststate.src
- M | pkg/opt/rituals/altar/gump.inc
- M | pkg/opt/rituals/altar/use.src
- M | pkg/opt/rituals/config/itemdesc.cfg
- M | pkg/opt/rituals/include/altarquest.inc
- M | pkg/opt/rituals/include/rituals.inc
- M | pkg/opt/rituals/rituals/bloodSeeking.src
- M | pkg/opt/rituals/rituals/hardening.src
- M | pkg/opt/rituals/rituals/perilousTheurgy.src
- M | pkg/opt/rituals/rituals/racialTheurgy.src
- M | pkg/opt/rituals/rituals/resilience.src
- M | pkg/opt/rituals/rituals/vitalInfusion.src
- M | pkg/opt/spawnpoint/include/customnpc.inc (commit `e0ff379`)
- M | pkg/opt/spawnpoint/spawnpoint.src
- M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src (commit `e0ff379`)
- A | pkg/opt/warriorforhire/include/wfhvitals.inc (commit `e0ff379`)
- M | pkg/opt/warriorforhire/warrior.src (commit `e0ff379`)
- M | pkg/packethooks/megacliloc/mobiledata.src (uncommitted, theme 18)
- M | pkg/std/peacemaking/peacemaking.src (uncommitted, theme 18)
- D | pkg/std/dundee/codex.cfg (commit `e0ff379`)
- D | pkg/std/dundee/virtuewalkon.src (commit `e0ff379`)
- M | pkg/std/fishing/crustaceantrap.inc
- M | pkg/std/fishing/fishing.inc
- M | pkg/std/snooping/snooping.src (commit `e0ff379` + uncommitted, theme 18)
- M | pkg/std/taunt/taunt.src (uncommitted, theme 18)
- M | pkg/std/snooping/stealme.cfg (commit `e0ff379`)
- M | pkg/std/stealing/stealing.src (commit `e0ff379`)
- M | pkg/systems/attributes/hooks/vitalInit.src (commit `e0ff379`)
- A | pkg/systems/attributes/include/npcvitals.inc (commit `e0ff379` + uncommitted, theme 18)
- M | pkg/systems/combat/config/modenchantdesc.cfg
- M | scripts/ai/chaosmultikillpcs.src (commit `e0ff379`)
- M | scripts/ai/highpriest.src (commit `e0ff379`)
- M | scripts/ai/main/npcinfo.inc (uncommitted, theme 18)
- D | scripts/ai/main/questiesetup.inc (commit `e0ff379`)
- M | scripts/ai/setup/modsetup.inc (commit `e0ff379` + uncommitted, theme 18)
- D | scripts/ai/setup/questiesetup.inc (commit `e0ff379`)
- M | scripts/ai/townguard.src (commit `e0ff379`)
- D | scripts/ai/valthor.src
- D | scripts/CustomHpFix.src (commit `e0ff379`)
- M | scripts/include/anchors.inc (uncommitted, theme 18)
- M | scripts/include/attributes.inc (commit `e0ff379`)
- M | scripts/include/constants/npcai.inc (commit `e0ff379`)
- M | scripts/include/dotempmods.inc
- M | scripts/include/namingbyenchant.inc
- A | scripts/include/npcdifficulty.inc (uncommitted, theme 18)
- M | scripts/include/speech.inc (commit `e0ff379`)
- M | scripts/include/teleporters.inc
- D | scripts/include/virtue.inc (commit `e0ff379`)
- M | scripts/misc/death.src
- M | scripts/misc/questbutton.src
- M | scripts/start.src (commit `e0ff379`)
- M | scripts/textcmd/admin/admin.src (commit `e0ff379`)
- M | scripts/textcmd/admin/akill.src
- M | scripts/textcmd/admin/destroyradius.src
- D | scripts/textcmd/admin/iteminfo.src
- R | pkg/opt/alryc/textcmd/test/alryciteminfo.src -> scripts/textcmd/gm/iteminfo.src
- D | scripts/textcmd/gm/newiteminfo.src
- M | scripts/textcmd/seer/info.src (commit `e0ff379` + uncommitted, theme 18)

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
- Coordinate fixes: Sunken City Hub -> Island Y 1850 -> 1849; Faymoor Island -> Solen Hive Y 2531 -> 2530; Runebound Sanctum level 2 -> 1 `{6650/6653,1436,5}` -> `{…,1435,2}`; Sosaria -> Mine X 5902 -> 5901 (two entries); Sosaria -> Cave X 5590 -> 5589; Black City of the Damned -> Tartarus was a self-loop `{951,2885,35}` -> `{951,2885,35}` and now lands at `{5529,3871,0}`. A duplicate Shandalaar Blood Sewers entry, duplicate Faymoor <-> Aetherfall Mine entries and 12 dead Mine/Underworld/Wayfarer's Abyss entries were removed.
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

### 17. Developer notes (commits `85f13a2`, `e0ff379`, plus uncommitted theme 18)

**Files involved:** `.claude/subagent-briefing.md`

**Notable functional changes:** realm map sizes and the "Felucca means britannia_alt" rule added (`85f13a2`); NPC stat-limit note (`e0ff379`); (commit `e0ff379`) the npcdesc.cfg canonical-layout convention from theme 18.

**Expected impact:** No player-visible effect.

### 18. npcdesc.cfg reformat: one layout for every template, CProps promoted to fields (uncommitted, 2026-09-30)

**Files involved:** `config/npcdesc.cfg`, `ainotes/npcdesc-reformat-report-2026-09-30.md` (new), `scripts/include/npcdifficulty.inc` (new), `scripts/include/anchors.inc`, `scripts/ai/main/npcinfo.inc`, `scripts/ai/setup/modsetup.inc`, `pkg/systems/attributes/include/npcvitals.inc`, `pkg/std/peacemaking/peacemaking.src`, `pkg/opt/songbook/songoffright.src`, `pkg/std/taunt/taunt.src`, `pkg/std/snooping/snooping.src`, `pkg/packethooks/megacliloc/mobiledata.src`, `pkg/opt/areaspawner/include/areaspawner.inc`, `pkg/opt/areaspawner/include/areaspawnergump.inc`, `scripts/textcmd/seer/info.src`, `.claude/subagent-briefing.md`

**Notable functional changes:**
- All 1,455 `NpcTemplate` blocks rewritten by script into one order (identity, behaviour, bard/snoop fields, stats, skills, built-in weapon, casting, loot, CProps), one spelling per key (3,136 field keys and 169 skill lines respelt: `MaceFighting`, `EvaluatingIntelligence`, `DetectingHidden`, aliases such as `Swords`/`Resist`/`EvalInt` expanded), tab indent with values at column 40, blank lines only between groups (4,407 stray ones removed), skills and spells alphabetical inside their groups. Text between blocks is untouched; the commented reference template at the top of the file is regenerated in the same order. A parity check confirmed every kept entry survived with its value. Full record: `ainotes/npcdesc-reformat-report-2026-09-30.md`.
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

---

## Validation Notes

- Diff range: `git log --graph Patch-3.1.2..HEAD`, `git diff --name-status -M Patch-3.1.2..HEAD` (committed), `git diff --name-status -M Patch-3.1.2` plus `git ls-files --others --exclude-standard` (committed + working tree; the inventory above), `git diff --numstat -M` on both for the totals and largest shifts, `git show --stat` on each non-merge commit, and `git diff a4336ef e484311` (empty) to confirm the PR #110/#111 merges carry nothing.
- File-status counts were derived programmatically: 12 A / 50 M / 10 D / 21 R = 93.
- Template counts come from `^\s*NpcTemplate\s` headers: 1,433 at `Patch-3.1.2`, 1,452 at `85f13a2`, 1,455 in the working tree after the reformat. HITS/MANA/STAM against old effective values were compared programmatically for every template; the three differences are the ones listed in theme 9. The reformat (theme 18) was parity-checked entry by entry against the pre-reformat file before writing (scratchpad `npcdesc_verify.py`: 0 mismatches).
- Teleporter counts come from active and commented `{ x, y, z, ... }` rows in `teleporters.inc` at both ends of the range.
- Working tree was not clean: 17 tracked files modified and 2 untracked new files, all theme 18 and described above. Themes 9-16 were committed as `e0ff379` ('Patch Notes') after the first draft of these notes; the inventory labels were recomputed from `git show --name-only e0ff379` and `git status --porcelain` on 2026-09-30. Nothing in the range was compiled or run by the author of these notes.
