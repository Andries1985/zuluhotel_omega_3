# Developer Changelog - v3.0.2

Range: Patch-3.0.1..Patch-3.0.2  
Branch: Patch-3.0.2  
Date: 2026-07-02

---

## Scope Summary

- Total files changed: 76
- Text/config/script files changed: 76
- Net textual delta: 14824 insertions, 898 deletions
- Branch commits in range: Full Omega Cache Implementation; Patchnotes update; static house sign update; Townstones update Vault storage for shared account storage New mounttest command new gotomulti command Keys storage moved to its own spot and updated to be deleted properly when housing demolished

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - | Notes |
|---|---|---:|---:|---|
| M | config/cmds.cfg | 1 | 0 | Registered additional command mappings in this patch line. |
| M | config/command_synopses.cfg | 77 | 7 | Refreshed command synopsis entries for expanded command surface. |
| A | patchnotes/developer-changelog-v3.0.1.md | 164 | 0 | Added developer changelog documentation for patch 3.0.1. |
| A | patchnotes/developer-changelog-v3.0.2.md | 151 | 0 | Added developer changelog documentation for patch 3.0.2. |
| M | patchnotes/launchernotes.md | 18 | 35 | Updated launcher copy for latest patch messaging. |
| A | patchnotes/omegacache-implementation-plan-v3.0.2.md | 318 | 0 | Added Omega Cache implementation plan document. |
| A | patchnotes/patch-v3.0.1.md | 51 | 0 | Added player-facing patch notes file for v3.0.1. |
| A | patchnotes/patch-v3.0.2.md | 50 | 0 | Added player-facing patch notes file for v3.0.2. |
| M | pkg/items/containers/include/storageAreas.inc | 29 | 2 | Updated storage area definitions for key/vault behavior. |
| M | pkg/items/keys/include/key.inc | 77 | 2 | Updated key storage and lifecycle logic. |
| M | pkg/items/keys/textcmd/gm/fixkeyring.src | 2 | 2 | Adjusted keyring repair command alignment. |
| M | pkg/multis/house/config/itemdesc.cfg | 22 | 0 | Added house-facing cache item descriptors. |
| M | pkg/multis/house/multiDeed/changeOwner.src | 10 | 1 | Added cache-aware owner-transfer handling. |
| M | pkg/multis/house/multiDeed/use.src | 17 | 0 | Added cache-aware hooks to deed placement flow. |
| M | pkg/multis/house/multiSign/use.src | 94 | 21 | Expanded house sign flow for cache and related controls. |
| M | pkg/multis/staticHousing/commands/gm/removeStaticDeed.src | 1 | 1 | Updated static deed removal command flow. |
| M | pkg/multis/staticHousing/commands/gm/staticDeed.src | 3 | 3 | Updated static deed command flow. |
| M | pkg/multis/staticHousing/config/itemdesc.cfg | 26 | 26 | Updated static housing item descriptors. |
| M | pkg/multis/staticHousing/include/signSelectionFunctions.inc | 1 | 1 | Updated static sign selection helper behavior. |
| M | pkg/multis/staticHousing/include/staticHousing.inc | 7 | 0 | Added static housing helper logic. |
| M | pkg/multis/staticHousing/lockunlock.src | 5 | 5 | Updated static housing lock/unlock behavior. |
| M | pkg/multis/staticHousing/sign/destroy.src | 15 | 0 | Added static sign destroy-path handling updates. |
| M | pkg/multis/staticHousing/sign/method.src | 21 | 25 | Updated static sign method logic and cleanup behavior. |
| A | pkg/opt/alryc/README.txt | 6 | 0 | Added package readme for alryc tools. |
| A | pkg/opt/alryc/config/animatedgraphics.cfg | 4239 | 0 | Added animated graphics configuration dataset. |
| A | pkg/opt/alryc/pkg.cfg | 4 | 0 | Added package registration for alryc tooling package. |
| A | pkg/opt/alryc/textcmd/test/gotomulti.src | 224 | 0 | Added test command to list/select house multis and teleport. |
| A | pkg/opt/alryc/textcmd/test/mounttest.src | 213 | 0 | Added test command for mount selection/testing workflow. |
| A | pkg/opt/omegacache/blacklist.cfg | 14 | 0 | Added cache eligibility blacklist. |
| A | pkg/opt/omegacache/cacheinsert.src | 44 | 0 | Added cache insert routine. |
| A | pkg/opt/omegacache/categories.cfg | 1436 | 0 | Added cache category definitions. |
| A | pkg/opt/omegacache/destroycache.src | 64 | 0 | Added cache destroy/purge routine. |
| A | pkg/opt/omegacache/itemdesc.cfg | 35 | 0 | Added cache item descriptors. |
| A | pkg/opt/omegacache/omegacache.inc | 1810 | 0 | Added core cache helpers/constants. |
| A | pkg/opt/omegacache/omegacache.src | 34 | 0 | Added cache package entrypoint logic. |
| A | pkg/opt/omegacache/pkg.cfg | 7 | 0 | Added Omega Cache package registration. |
| A | pkg/opt/omegacache/placecache.src | 119 | 0 | Added cache placement flow for housing. |
| A | pkg/opt/omegacache/stacking_ignore.cfg | 19 | 0 | Added cache stacking-ignore definitions. |
| A | pkg/opt/townstones/electionwatch.src | 146 | 0 | Added election watch process for townstone system. |
| M | pkg/opt/townstones/itemdesc.cfg | 2 | 1 | Updated townstone item descriptors. |
| A | pkg/opt/townstones/logon.src | 63 | 0 | Added townstone logon initialization hook. |
| A | pkg/opt/townstones/start.src | 8 | 0 | Added townstone startup registration hook. |
| A | pkg/opt/townstones/textcmd/admin/cleartownmembers.src | 106 | 0 | Added admin command to clear town membership safely. |
| D | pkg/opt/townstones/textcmd/admin/createstone.src | 0 | 30 | Replaced legacy townstone create command flow. |
| A | pkg/opt/townstones/textcmd/admin/createtownstone.src | 223 | 0 | Added updated townstone creation command flow. |
| M | pkg/opt/townstones/textcmd/admin/fixstone.src | 44 | 13 | Expanded stone fix tooling and safety paths. |
| A | pkg/opt/townstones/textcmd/admin/gettowngold.src | 72 | 0 | Added admin treasury withdrawal helper command. |
| A | pkg/opt/townstones/textcmd/admin/removetownmember.src | 129 | 0 | Added admin member-removal command flow. |
| A | pkg/opt/townstones/textcmd/admin/resetpoll.src | 52 | 0 | Added admin election poll reset command flow. |
| A | pkg/opt/townstones/textcmd/admin/townbankstatus.src | 381 | 0 | Added admin command for treasury/upgrades/donation status control. |
| A | pkg/opt/townstones/townlistbootstrap.src | 118 | 0 | Added town list bootstrap initialization logic. |
| M | pkg/opt/townstones/tstone.inc | 869 | 198 | Major townstone helper and data model updates. |
| M | pkg/opt/townstones/tstone.src | 760 | 269 | Major townstone runtime logic updates. |
| A | pkg/opt/townstones/upgrades.cfg | 397 | 0 | Added town upgrades configuration definitions. |
| M | pkg/std/alchemy/alchemy.src | 70 | 12 | Added cache-aware resource consumption integration. |
| M | pkg/std/blacksmithy/make_blacksmith_items.src | 59 | 39 | Added cache-aware resource consumption integration. |
| M | pkg/std/carpentry/carpentry.src | 51 | 21 | Added cache-aware resource consumption integration. |
| M | pkg/std/cartography/cartography.src | 21 | 19 | Added cache-aware resource consumption integration. |
| M | pkg/std/cooking/cooking.src | 24 | 14 | Added cache-aware resource consumption integration. |
| M | pkg/std/inscription/inscription.src | 44 | 20 | Added cache-aware resource consumption integration. |
| M | pkg/std/mining/smelting.src | 1 | 0 | Added cache-aware include integration alignment. |
| M | pkg/std/tailoring/make_cloth_items.src | 49 | 31 | Added cache-aware resource consumption integration. |
| M | pkg/std/tinkering/tinkering.src | 50 | 30 | Added cache-aware resource consumption integration. |
| A | pythonscripts/_gen_alryc_animatedgraphics_cfg.py | 148 | 0 | Added script for animated graphics cfg generation. |
| M | scripts/ai/banker.src | 55 | 31 | Updated banker flow to support shared vault/open-vault handling. |
| M | scripts/ai/bankerbalancegump.src | 5 | 1 | Updated banker balance gump behavior for new flows. |
| M | scripts/control/keyringdestroy.src | 3 | 12 | Tightened key cleanup handling on destroy/demolish paths. |
| A | scripts/include/canstack.inc | 108 | 0 | Added shared stack-compatibility helper include. |
| A | scripts/include/omegacache_utils.inc | 127 | 0 | Added cache utility helper include. |
| A | scripts/include/resourcemanager.inc | 803 | 0 | Added shared resource manager include for crafting. |
| M | scripts/items/bladed.src | 28 | 20 | Updated bladed-item material flow for cache integration. |
| M | scripts/items/fletch.src | 10 | 5 | Updated fletching material flow for cache integration. |
| M | scripts/textcmd/gm/openbank.src | 1 | 1 | Updated openbank command routing behavior. |
| A | scripts/textcmd/gm/openvault.src | 35 | 0 | Added admin vault open command flow. |
| A | scripts/textcmd/player/cache.src | 310 | 0 | Added player cache command implementation. |
| A | scripts/textcmd/seer/vault.src | 24 | 0 | Added vault command entrypoint for seer usage. |

---

## Detailed Changes By Theme

### 1) Omega Cache Rollout and Crafting Integration

Files involved:
- pkg/opt/omegacache/blacklist.cfg
- pkg/opt/omegacache/cacheinsert.src
- pkg/opt/omegacache/categories.cfg
- pkg/opt/omegacache/destroycache.src
- pkg/opt/omegacache/itemdesc.cfg
- pkg/opt/omegacache/omegacache.inc
- pkg/opt/omegacache/omegacache.src
- pkg/opt/omegacache/pkg.cfg
- pkg/opt/omegacache/placecache.src
- pkg/opt/omegacache/stacking_ignore.cfg
- scripts/include/canstack.inc
- scripts/include/omegacache_utils.inc
- scripts/include/resourcemanager.inc

Behavior changes:
- Introduced a new optional package (`pkg/opt/omegacache`) with config, item definitions, placement/destruction logic, and command-facing scripts.
- Added centralized cache storage and insertion/removal handling, including explicit destroy/purge flow.
- Added category metadata and filtering rules (blacklist and stacking-ignore) to preserve item identity and control eligibility.
- Added shared include-layer helpers for stack compatibility and resource routing so skill scripts can consume from cache consistently.
- Added a large shared resource manager include used by multiple crafting systems.
- Integrated cache-aware resource checks/consumption across major crafting skills and related item scripts.

### 2) Housing, Static Housing, and Key Lifecycle Follow-ups

Files involved:
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/multiDeed/changeOwner.src
- pkg/multis/house/multiDeed/use.src
- pkg/multis/house/multiSign/use.src
- pkg/multis/staticHousing/commands/gm/removeStaticDeed.src
- pkg/multis/staticHousing/commands/gm/staticDeed.src
- pkg/multis/staticHousing/config/itemdesc.cfg
- pkg/multis/staticHousing/include/signSelectionFunctions.inc
- pkg/multis/staticHousing/include/staticHousing.inc
- pkg/multis/staticHousing/lockunlock.src
- pkg/multis/staticHousing/sign/destroy.src
- pkg/multis/staticHousing/sign/method.src
- pkg/items/containers/include/storageAreas.inc
- pkg/items/keys/include/key.inc
- pkg/items/keys/textcmd/gm/fixkeyring.src
- scripts/control/keyringdestroy.src
- pkg/opt/omegacache/placecache.src
- pkg/opt/omegacache/destroycache.src

Behavior changes:
- House package gained cache-related descriptors and integration points.
- House deed create/ownership-transfer flows now include cache-aware handling to keep cache state aligned with house ownership lifecycle.
- House sign interaction flow added cache-management paths and guards.
- Placement and destruction flows for cache containers are wired to housing access/permission context.
- Static housing sign and deed support paths received follow-up behavior fixes and descriptor updates.
- Key storage was moved to dedicated storage-area handling and demolish/destroy cleanup paths were tightened.

### 3) Townstones Expansion and City Treasury Tooling

Files involved:
- pkg/opt/townstones/electionwatch.src
- pkg/opt/townstones/itemdesc.cfg
- pkg/opt/townstones/logon.src
- pkg/opt/townstones/start.src
- pkg/opt/townstones/textcmd/admin/cleartownmembers.src
- pkg/opt/townstones/textcmd/admin/createtownstone.src
- pkg/opt/townstones/textcmd/admin/createstone.src (deleted)
- pkg/opt/townstones/textcmd/admin/fixstone.src
- pkg/opt/townstones/textcmd/admin/gettowngold.src
- pkg/opt/townstones/textcmd/admin/removetownmember.src
- pkg/opt/townstones/textcmd/admin/resetpoll.src
- pkg/opt/townstones/textcmd/admin/townbankstatus.src
- pkg/opt/townstones/townlistbootstrap.src
- pkg/opt/townstones/tstone.inc
- pkg/opt/townstones/tstone.src
- pkg/opt/townstones/upgrades.cfg

Behavior changes:
- Townstone runtime and helper layers were heavily expanded for member, treasury, and poll management behavior.
- Legacy `createstone` flow was replaced by `createtownstone` command flow.
- Added admin commands for clearing/removing members, resetting polls, viewing/toggling treasury/upgrades status, and withdrawing town funds.
- Added election/watch and bootstrap/start hooks to improve townstone lifecycle consistency.
- Added upgrade configuration definitions for city treasury upgrade behavior.

### 4) Vault and Command Surface Expansion

Files involved:
- scripts/ai/banker.src
- scripts/ai/bankerbalancegump.src
- scripts/textcmd/gm/openbank.src
- scripts/textcmd/gm/openvault.src
- scripts/textcmd/seer/vault.src
- scripts/textcmd/player/cache.src
- config/command_synopses.cfg
- config/cmds.cfg
- pkg/opt/alryc/textcmd/test/gotomulti.src
- pkg/opt/alryc/textcmd/test/mounttest.src

Behavior changes:
- Added shared account vault command support with dedicated open-vault command paths.
- Added new player text command `.cache` with operational subcommands for cache interactions.
- Added test/admin command flows for `gotomulti` and `mounttest`.
- Updated command registration and synopsis output for the expanded command set.

### 5) Patch Documentation and Tooling Files

Files involved:
- patchnotes/developer-changelog-v3.0.1.md
- patchnotes/developer-changelog-v3.0.2.md
- patchnotes/launchernotes.md
- patchnotes/omegacache-implementation-plan-v3.0.2.md
- patchnotes/patch-v3.0.1.md
- patchnotes/patch-v3.0.2.md
- pkg/opt/alryc/README.txt
- pkg/opt/alryc/config/animatedgraphics.cfg
- pkg/opt/alryc/pkg.cfg
- pythonscripts/_gen_alryc_animatedgraphics_cfg.py

Behavior changes:
- Patch documentation set was expanded and updated for 3.0.1/3.0.2 documentation coverage.
- Added dedicated Omega Cache implementation plan and updated launcher copy.
- Added alryc tooling/package docs and animated graphics generator/config support files.

---

## Validation Notes

- Diff range used: Patch-3.0.1..Patch-3.0.2
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - git diff --shortstat
  - git log --oneline --no-merges
- The changelog is file-complete for the Patch-3.0.2 branch delta.
