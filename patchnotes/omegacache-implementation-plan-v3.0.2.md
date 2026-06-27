# Omega Cache Implementation Plan (ZH3.0.2)

Branch: 3.0.2
Source feature set: ZH2.5 `pkg/opt/omegacache` (+ related includes/commands/crafting hooks)
Target: Layer Omega Cache onto ZH3.0 systems without replacing ZH3 housing/storage/crafting architecture.

## Guardrails

1. Keep ZH3 house package model in place (`pkg/multis/house/*`).
2. Keep ZH3 crafting flow in place (`pkg/std/*`, `scripts/items/*`), then add cache-aware resource sourcing on top.
3. Do not wholesale replace ZH3 files with ZH2.5 files.
4. Port behavior, not old structure.
5. Add narrow compatibility helpers where needed (constants, wrappers, adapters).

## Scope Baseline

Port all Omega Cache capabilities that are in ZH2.5 production path:

- Cache container + deed objects and placement flow
- Per-house cache store (shared across cache containers in same house)
- Deposit/withdraw + gump browsing
- `.cache` command family
- House integration (slot limits, remove cache from house menu, demolition safety)
- Resource-manager integration so crafting can consume from backpack/cache
- Cache-aware crafting entry points and edge scripts

Out of scope for first landing:

- Styling/UI redesign
- Gameplay rebalance unrelated to cache behavior

## Phase 0 - Source Lock And Inventory

Goal: Lock exact source files and call graph before touching code.

Source files to import/translate from ZH2.5:

- `pkg/opt/omegacache/pkg.cfg`
- `pkg/opt/omegacache/itemdesc.cfg`
- `pkg/opt/omegacache/omegacache.inc`
- `pkg/opt/omegacache/omegacache.src`
- `pkg/opt/omegacache/placecache.src`
- `pkg/opt/omegacache/cacheinsert.src`
- `pkg/opt/omegacache/destroycache.src`
- `pkg/opt/omegacache/categories.cfg`
- `pkg/opt/omegacache/blacklist.cfg`
- `pkg/opt/omegacache/stacking_ignore.cfg`
- `scripts/include/omegacache_utils.inc`
- `scripts/include/resourcemanager.inc`
- `scripts/include/canstack.inc`
- `scripts/textcmd/player/cache.src`

Exit criteria:

- File set confirmed and frozen for port batch.
- Open questions list created before coding starts.

## Phase 1 - Package Scaffolding In ZH3

Goal: Introduce Omega Cache package cleanly into ZH3 tree.

Add new package tree:

- `pkg/opt/omegacache/*` (core scripts/config files above)

Integrate package registration:

- Add package registration enablement where ZH3 package loading expects it.

Integrate object definitions:

- Wire cache objtypes/deed objtypes into itemdesc/objtype references used by ZH3.

Compatibility constants:

- Add cache constants in shared include location used by ZH3 scripts.

Exit criteria:

- Package loads.
- Cache deed/container can be created by staff command.
- No compile/load errors from new package alone.

## Phase 2 - Housing Layer Integration (ZH3 House System Preserved)

Goal: Attach cache lifecycle to existing ZH3 house sign and house state model.

Primary ZH3 touchpoints:

- `pkg/multis/house/multiSign/use.src`
- `pkg/multis/house/multiSign/control.src`
- `pkg/multis/house/multiSign/method.src`
- `pkg/multis/house/multiDeed/use.src`
- `pkg/multis/house/multiDeed/changeOwner.src`
- `pkg/multis/house/config/itemdesc.cfg`

Actions:

1. Add house cache counters as separate properties (do not reuse secures):
   - `numomegacache`
   - `maxnumomegacache`
2. Initialize/cache-clamp values at house creation and ownership transfer.
3. Replace disabled menu item in house management with working Remove Omega Cache flow.
4. Add remove validation path:
   - only house-owned cache objects
   - deny removal if store non-empty (or enforce confirmed purge policy)
   - refund house cache slot on successful removal
5. Add demolition safeguards:
   - explicit warning if cached items still present
   - deterministic cleanup behavior on house destroy/redeed

Exit criteria:

- House sign displays/handles cache slots correctly.
- Place/remove cache works with permission checks.
- Redeed/demolish paths are safe and predictable.

## Phase 3 - Cache UX And Commands

Goal: Provide usable player/admin interaction beyond double-click.

Touchpoints:

- New: `scripts/textcmd/player/cache.src`
- Existing command synopsis generation path in ZH3

Actions:

1. Port `.cache` command family:
   - open
   - deposit
   - deposit target
   - list
   - withdraw
   - autodraw toggle
   - dump (staff/debug)
2. Keep gump logic centralized in cache include.
3. Ensure command synopsis generation is rerun after adding command file.

Exit criteria:

- `.cache` command works end-to-end.
- Help/synopsis entry present.

## Phase 4 - Shared Resource Abstraction Layer

Goal: Introduce cache-aware resource requests without breaking existing ZH3 crafting architecture.

New shared includes:

- `scripts/include/resourcemanager.inc`
- `scripts/include/canstack.inc`
- `scripts/include/omegacache_utils.inc`

Design requirements:

1. Preserve existing target-and-craft user flow where possible.
2. Add adapter entry points:
   - `MakeBackpackRequest(...)`
   - `SelectMaterialFromCache(...)`
   - `GetAvailableResource(...)`
   - `ConsumeResource(...)`
3. Add lease handling to prevent race/overdraw from cache-backed loops.
4. Respect ZH3 stacking/cprop policy and cache identity rules.

Exit criteria:

- Shared functions compile and are callable from one pilot craft script.
- No regression in non-cache crafting path.

Progress (2026-06-25):

- Pilot integration completed in `pkg/std/blacksmithy/make_blacksmith_items.src`.
- Blacksmithing now builds `ResourceRequest` structs via `MakeBackpackRequest(...)` and uses `GetAvailableResource(...)`, `ConsumeResource(...)`, and lease helpers for material checks/consumption.
- Existing blacksmith target flow is preserved (backpack-first), with cache-aware fallback handled by the shared abstraction layer.

## Phase 5 - Crafting Integration Batches

Goal: Layer cache consumption into existing ZH3 crafting files incrementally.

Batch A (core production skills):

- `pkg/std/blacksmithy/make_blacksmith_items.src`
- `pkg/std/tailoring/make_cloth_items.src`
- `pkg/std/carpentry/carpentry.src`
- `pkg/std/alchemy/alchemy.src`
- `pkg/std/tinkering/tinkering.src`

Progress (2026-06-25):

- Batch A integrated with shared resource abstraction.
- All five core crafting scripts now route material checks/consumption through `GetAvailableResource(...)` and `ConsumeResource(...)` with lease helpers.
- Existing craft menus and target flows were preserved, with cache-aware autodraw layered on top of backpack-first behavior.

Batch B (resource-heavy secondary):

- `pkg/std/inscription/inscription.src`
- `pkg/std/cartography/cartography.src`
- `pkg/std/cooking/cooking.src`

Progress (2026-06-25):

- Batch B integrated for primary material paths.
- `pkg/std/inscription/inscription.src` now uses `GetAvailableResource(...)` and `ConsumeResource(...)` for runebook components and blank-scroll crafting loops, with lease helpers in autoloop flow.
- `pkg/std/cartography/cartography.src` now routes blank-map availability/consumption through request-based resource functions in map creation paths.
- `pkg/std/cooking/cooking.src` now checks and consumes standard recipe ingredients through resource requests (backpack-first, cache-aware fallback), while preserving special ingredient handlers.

Batch C (edge scripts):

- `scripts/items/fletch.src`
- `scripts/items/bladed.src`
- Any additional ZH3 scripts found using direct `SubtractAmount` against stackables targeted by player.

Progress (2026-06-25):

- Batch C integrated for edge crafting/combination paths in target scripts.
- `scripts/items/fletch.src` now uses request-based availability and consumption for shafts/feathers.
- `scripts/items/bladed.src` now uses request-based consumption for log carving material use, special reagent consumption, and arrow/reagent combine paths.
- Remaining direct `SubtractAmount(...)` usages in `scripts/items/*.src` were reviewed; non-target gameplay scripts were left unchanged for this phase.

Per-file migration pattern:

1. Keep original behavior path for backpack items.
2. Add cache-target entry branch (`OMEGACACHE_OBJTYPE` equivalent constant).
3. Convert local consumption from direct stack subtraction to resource request consume path.
4. Preserve skill checks, class bonuses, PHC hooks, and existing ZH3 messages.

Exit criteria:

- Batch-by-batch verification passes before moving to next batch.

## Phase 6 - Data Integrity And Compatibility

Goal: Ensure cache item identity, blacklist, and stack behavior are stable.

Actions:

1. Align cache key cprop-ignore behavior with ZH3 `config/stacking.cfg`.
2. Enforce blacklist exclusions in cache deposit path.
3. Validate special-case items that share objtype but differ by meaningful props.
4. Verify no destructive loss of gameplay-significant props on withdraw.

Exit criteria:

- Deterministic deposit/withdraw identity behavior.
- No known dupes or silent merges for protected variants.

Progress (2026-06-25):

- CProp ignore behavior is now aligned to engine stacking baseline by reading `config/stacking.cfg` `IgnoreCprops` directly in cache key generation.
- Cache-specific ignore additions remain configurable via `pkg/opt/omegacache/stacking_ignore.cfg` (currently `fromLoot`).
- Gameplay-significant identity CProps are explicitly protected from being ignored (`BaseName`, `foodvalue`) to prevent silent merges/loss on withdrawal.
- Blacklist validation was hardened to match both numeric and hex-key objtype forms during eligibility checks, ensuring exclusions are enforced consistently in deposit paths.
- Withdrawal recreation path remains create-first-then-debit and restores stored non-default properties/CProps, avoiding destructive loss when creation fails mid-withdraw.

## Phase 7 - Verification Matrix

Run after each phase and again pre-merge.

Housing and lifecycle:

- Place cache in owned house
- Deny place in non-owned house
- Remove empty cache
- Deny/remove policy for non-empty cache
- Transfer ownership and validate counters/permissions
- Redeed/demolish behavior with and without stored contents

Storage behavior:

- Single deposit
- Deposit all
- Drag-drop insert path
- Withdraw to backpack and to target container
- Multiple cache containers in same house share same store

Crafting behavior:

- Backpack-only path unchanged
- Cache-targeted path works for each integrated script
- Mixed source (backpack + cache) consumption correct
- Autodraw toggle behavior correct

Concurrency and safety:

- Two players accessing same house cache
- Lease limits respected under loop crafting

Progress (2026-06-25):

- Compile validation completed for all Omega Cache touched source scripts (housing, command, cache package, Batch A/B/C crafting and edge scripts): all compile clean.
- Full-repo compile still reports an unrelated pre-existing error in `pkg/std/mining/smelting.src` (`Unknown identifier 'IsInContainer'`), outside Omega Cache touch scope.
- Static integrity checks completed in code and compile:
   - cache key CProp-ignore alignment with `config/stacking.cfg`
   - blacklist enforcement in deposit eligibility
   - protected variant identity (`BaseName`, `foodvalue`)
   - withdraw recreate/debit ordering
- Manual in-game verification remains pending for runtime-only scenarios:
   - house lifecycle placement/transfer/redeed edge cases
   - multi-player concurrent access behavior
   - lease contention behavior during live loop crafting

## Implementation Order (Execution)

1. Phase 1 package scaffold
2. Phase 2 house integration
3. Phase 3 command/UI completion
4. Phase 4 resource abstraction
5. Phase 5 crafting batches A -> B -> C
6. Phase 6 data-integrity pass
7. Phase 7 full verification and hardening

## Resolved Decisions

1. Non-empty cache removal policy: allow forced purge with explicit confirmation.
2. Cache slot limits: keep ZH2.5 values for current house set.
3. `.cache dump` policy: staff-only.
4. Crafting release staging question: no longer applicable for this landing (Batch A/B/C completed in this implementation).
