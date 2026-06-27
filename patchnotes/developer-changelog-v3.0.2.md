# Developer Changelog - v3.0.2

Range: 724b92f..c7cb592  
Branch: Patch-3.0.2  
Date: 2026-06-25

---

## Scope Summary

- Total files changed: 32
- Text/config/script files changed: 32
- Net textual delta: 5823 insertions, 249 deletions
- Branch commits in range: Full Omega Cache Implementation

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - | Notes |
|---|---|---:|---:|---|
| M | config/command_synopses.cfg | 7 | 0 | Added synopsis entries for cache command textcmds. |
| M | patchnotes/launchernotes.md | 19 | 35 | Updated launcher-facing patch copy to Omega Cache messaging. |
| A | patchnotes/omegacache-implementation-plan-v3.0.2.md | 318 | 0 | Added implementation plan and technical rollout notes. |
| M | pkg/multis/house/config/itemdesc.cfg | 22 | 0 | Added house-facing item definitions for Omega Cache placement/support. |
| M | pkg/multis/house/multiDeed/changeOwner.src | 10 | 1 | Added owner-transfer handling for attached house cache state. |
| M | pkg/multis/house/multiDeed/use.src | 17 | 0 | Added deed/build flow hooks for cache-related house initialization. |
| M | pkg/multis/house/multiSign/use.src | 93 | 2 | Added sign-flow options/guards for cache management and permissions. |
| A | pkg/opt/omegacache/blacklist.cfg | 14 | 0 | Added object blacklist for cache-eligibility filtering. |
| A | pkg/opt/omegacache/cacheinsert.src | 44 | 0 | Added cache insertion routine. |
| A | pkg/opt/omegacache/categories.cfg | 1436 | 0 | Added large category mapping table for cache item organization. |
| A | pkg/opt/omegacache/destroycache.src | 64 | 0 | Added cache destruction/purge handling. |
| A | pkg/opt/omegacache/itemdesc.cfg | 35 | 0 | Added item descriptors for cache objects. |
| A | pkg/opt/omegacache/omegacache.inc | 1810 | 0 | Added core cache include with shared logic/constants/helpers. |
| A | pkg/opt/omegacache/omegacache.src | 34 | 0 | Added package entrypoint and cache initialization flow. |
| A | pkg/opt/omegacache/pkg.cfg | 7 | 0 | Added package registration/config for Omega Cache. |
| A | pkg/opt/omegacache/placecache.src | 119 | 0 | Added cache placement flow for in-world housing usage. |
| A | pkg/opt/omegacache/stacking_ignore.cfg | 19 | 0 | Added stacking-ignore control list for cache identity rules. |
| M | pkg/std/alchemy/alchemy.src | 70 | 12 | Integrated cache-aware material consumption into alchemy crafting flow. |
| M | pkg/std/blacksmithy/make_blacksmith_items.src | 59 | 39 | Integrated cache-aware resource handling into blacksmith crafting flow. |
| M | pkg/std/carpentry/carpentry.src | 51 | 21 | Integrated cache-aware resource handling into carpentry crafting flow. |
| M | pkg/std/cartography/cartography.src | 21 | 19 | Integrated cache-aware resource draw behavior into cartography flow. |
| M | pkg/std/cooking/cooking.src | 24 | 14 | Integrated cache-aware resource handling into cooking flow. |
| M | pkg/std/inscription/inscription.src | 44 | 20 | Integrated cache-aware resource handling into inscription flow. |
| M | pkg/std/mining/smelting.src | 1 | 0 | Added cache-path include/hook alignment for smelting-related flows. |
| M | pkg/std/tailoring/make_cloth_items.src | 49 | 31 | Integrated cache-aware resource handling into tailoring flow. |
| M | pkg/std/tinkering/tinkering.src | 50 | 30 | Integrated cache-aware resource handling into tinkering flow. |
| A | scripts/include/canstack.inc | 108 | 0 | Added reusable stack-compatibility helpers used by cache logic. |
| A | scripts/include/omegacache_utils.inc | 127 | 0 | Added utility helpers for cache command and storage operations. |
| A | scripts/include/resourcemanager.inc | 803 | 0 | Added shared resource manager used by multi-skill cache-aware crafting. |
| M | scripts/items/bladed.src | 28 | 20 | Updated bladed-material conversion flow for cache-aware resource checks. |
| M | scripts/items/fletch.src | 10 | 5 | Updated fletching flow for cache-aware resource checks. |
| A | scripts/textcmd/player/cache.src | 310 | 0 | Added player `.cache` text command implementation. |

---

## Detailed Changes By Theme

### 1) New Omega Cache Package and Data Model

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
- Introduced a new optional package (`pkg/opt/omegacache`) with its own config, item definitions, and command-facing scripts.
- Added centralized cache storage and insertion/removal handling, including explicit destroy/purge flow.
- Added category metadata and filtering rules (blacklist and stacking-ignore) to preserve item identity and control eligibility.
- Added shared include-layer helpers for stack compatibility and resource routing so skill scripts can consume from cache consistently.
- Added a large shared resource manager include used by multiple crafting systems.

### 2) Housing Integration and Ownership/Permission Hooks

Files involved:
- pkg/multis/house/config/itemdesc.cfg
- pkg/multis/house/multiDeed/changeOwner.src
- pkg/multis/house/multiDeed/use.src
- pkg/multis/house/multiSign/use.src
- pkg/opt/omegacache/placecache.src
- pkg/opt/omegacache/destroycache.src

Behavior changes:
- House package gained cache-related descriptors and integration points.
- House deed create/ownership-transfer flows now include cache-aware handling to keep cache state aligned with house ownership lifecycle.
- House sign interaction flow added cache-management paths and guards.
- Placement and destruction flows for cache containers are wired to housing access/permission context.

### 3) Crafting and Resource Consumption Integration

Files involved:
- pkg/std/alchemy/alchemy.src
- pkg/std/blacksmithy/make_blacksmith_items.src
- pkg/std/carpentry/carpentry.src
- pkg/std/cartography/cartography.src
- pkg/std/cooking/cooking.src
- pkg/std/inscription/inscription.src
- pkg/std/mining/smelting.src
- pkg/std/tailoring/make_cloth_items.src
- pkg/std/tinkering/tinkering.src
- scripts/items/bladed.src
- scripts/items/fletch.src
- scripts/include/resourcemanager.inc

Behavior changes:
- Core crafting skills now route resource checks/consumption through cache-aware logic while preserving backpack flow compatibility.
- Shared resource-draw behavior is now centralized via resource manager helpers for consistent per-skill behavior.
- Related edge item flows (bladed/fletching) were updated to respect the same cache-aware resource path.
- The integration reduces per-skill duplication and aligns resource handling semantics across systems.

### 4) Command Surface and Documentation Sync

Files involved:
- scripts/textcmd/player/cache.src
- config/command_synopses.cfg

Behavior changes:
- Added new player text command `.cache` with operational subcommands for cache interactions.
- Updated command synopsis config so help/synopsis output includes the new cache command set.

### 5) Patch Documentation Updates in Branch

Files involved:
- patchnotes/launchernotes.md
- patchnotes/omegacache-implementation-plan-v3.0.2.md

Behavior changes:
- Launcher notes were updated to reflect the Omega Cache rollout messaging.
- Added a dedicated implementation plan document for technical rollout context.

---

## Validation Notes

- Diff range used: 724b92f..c7cb592
- Coverage checks used:
  - git diff --name-status
  - git diff --numstat
  - git diff --shortstat
  - targeted commit stat review (c7cb592)
- The changelog is file-complete for the Patch-3.0.2 branch delta.
