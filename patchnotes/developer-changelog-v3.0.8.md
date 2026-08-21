# Developer Changelog - v3.0.8

Range: Patch-3.0.7..Patch-3.0.8 (commit `f80feec`..`78018b2`), plus uncommitted work-tree changes to `pkg/multis/boat/*` and `config/mrcspawn.cfg` (Theme 9) made after `78018b2`.
Branch: Patch-3.0.8
Date: 2026-08-20

---

## Scope Summary

- Total files changed: 35 committed (see Theme 1 note on `areaspawnerdeleterunner.src`), plus 9 more from the uncommitted Theme 9 boat work (1 new, 8 modified)
- Status breakdown: 20 added, 15 modified, 0 deleted (net; committed range only)
- Net textual delta: 9574 insertions, 5222 deletions (committed range only)
- Largest shifts:
  - `pkg/opt/areaspawner/include/areaspawnergump.inc` (+1912, new file) - every Area Spawner gump: type/realm pickers, spawner list, entry editors (including the 4-category Custom NPC editor), group/chest reference browsers, error list, and the new facet census
  - `pkg/opt/areaspawner/include/areaspawner.inc` (+1219 net across the range, now ~1720 lines) - the whole Area Spawner engine: datafile schema, placement logic for all 5 spawner kinds, tick/kick chain, error tracking
  - `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (+3564/-4344 net, i.e. a near-total rewrite) - decorate-facet door registrations fixed for Britannia Alt (Sosaria)
  - `scripts/include/teleporters.inc` (+108 net, 113 changed lines) - ~50 new Sosaria cave/dungeon teleporter pairs, a Water Dungeon level restructure, and removal of stale/duplicate/"BAD TELEPORTER" entries
  - `config/npcdesc.cfg` (656 changed lines) - 14 new NPC templates (7 mounts, a barber, 6 tiered Wyrm reward NPCs) plus surrounding template adjustments
  - `pkg/opt/areaspawner/areaspawnerentrytick.src` (+145, new file) - the one-shot per-entry tick/placement/reschedule script
  - `pkg/opt/areaspawner/config/areagroups.cfg` (+80, new file) / `config/areachests.cfg` (+142, new) / `config/eventareachests.cfg` (+141, new) - curated NPC-group and chest catalogs
  - `pkg/opt/areaspawner/areaspawnergroupbrowser.src` (+87, new) / `areaspawnerchestbrowser.src` (+75, new) / `eventareaspawnerchestbrowser.src` (+75, new) - standalone reference gumps for the catalogs above
- Non-merge commits in range (oldest to newest):
  - `036303e` Test wyrms created
  - `4fa5002` Preliminary Area Spawner / New teleporters / Doors fixed for decorate facets / new setpropradius command
  - `7a3fb13` Speedwalk fix
  - `78018b2` Area Spawner Update
- Themes below are organized by subsystem rather than by commit, since the Area Spawner feature (Themes 1-2) was built incrementally across the `4fa5002`/`78018b2` commits and is best read as one continuous piece of work.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- A | pkg/opt/alryc/include/speedwalk.inc
- A | pkg/opt/alryc/textcmd/test/setpropradius.src
- A | pkg/opt/areaspawner/areaspawnerchestbrowser.src
- A | pkg/opt/areaspawner/areaspawnerdeath.src
- A | pkg/opt/areaspawner/areaspawnerentrytick.src
- A | pkg/opt/areaspawner/areaspawnerfullrespawn.src
- A | pkg/opt/areaspawner/areaspawnergroupbrowser.src
- A | pkg/opt/areaspawner/areaspawnermanager.src
- A | pkg/opt/areaspawner/config/areachests.cfg
- A | pkg/opt/areaspawner/config/areagroups.cfg
- A | pkg/opt/areaspawner/config/eventareachests.cfg
- A | pkg/opt/areaspawner/eventareaspawnerchestbrowser.src
- A | pkg/opt/areaspawner/include/areaspawner.inc
- A | pkg/opt/areaspawner/include/areaspawnergump.inc
- A | pkg/opt/areaspawner/pkg.cfg
- A | pkg/opt/areaspawner/start.src
- A | pkg/opt/areaspawner/textcmd/admin/areaspawner.src
- A | pkg/opt/areaspawner/textcmd/admin/aserrorall.src
- A | pkg/opt/areaspawner/textcmd/admin/aslist.src
- A | pkg/opt/areaspawner/textcmd/admin/eventareaspawner.src
- A | scripts/include/chestunlock.inc
- M | config/command_synopses.cfg
- M | config/nlootgroup.cfg
- M | config/npcdesc.cfg
- M | pkg/items/containers/container/canDestroy.src
- M | pkg/items/donationbox/include/donationbox.inc
- M | pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg
- M | pkg/std/lockpicking/use/picklock.src
- M | pkg/systems/accounts/logon.src
- M | scripts/include/constants/propids.inc
- M | scripts/include/managers.inc
- M | scripts/include/teleporters.inc
- M | scripts/playermanager.src
- M | scripts/textcmd/admin/unlock.src
- M | scripts/textcmd/seer/speedwalk.src

Not shown above: `pkg/opt/areaspawner/areaspawnerdeleterunner.src` was added in `4fa5002` as a first-pass one-shot delete/reschedule script, then replaced and removed in `78018b2` once `areaspawnerentrytick.src` took over that job with the full tick/placement/reschedule chain — it never shipped in a released state, so it doesn't appear in the file inventory (added and removed within this same range nets to no diff).

**Theme 9 files (uncommitted, made after `78018b2` - see Range note above):**

- A | pkg/multis/boat/include/boatHelpers.inc
- M | config/mrcspawn.cfg
- M | pkg/multis/boat/boat/use.src
- M | pkg/multis/boat/commands/gm/restartboat.src
- M | pkg/multis/boat/config/itemdesc.cfg
- M | pkg/multis/boat/multi/listener.src
- M | pkg/multis/boat/rope/use.src
- M | pkg/multis/boat/tiller/methods.src
- M | pkg/multis/boat/tiller/use.src

---

## Detailed Changes By Theme

### 1. Area Spawner — New Feature (Core Engine)

Files involved:
- `pkg/opt/areaspawner/include/areaspawner.inc`, `include/areaspawnergump.inc`
- `pkg/opt/areaspawner/areaspawnerentrytick.src`, `areaspawnerdeath.src`, `areaspawnerfullrespawn.src`, `areaspawnermanager.src`, `start.src`, `pkg.cfg`
- `pkg/opt/areaspawner/textcmd/admin/areaspawner.src`, `eventareaspawner.src`, `aslist.src` (new), `aserrorall.src` (new)
- `pkg/opt/areaspawner/config/areagroups.cfg`, `config/areachests.cfg`, `config/eventareachests.cfg`
- `pkg/opt/areaspawner/areaspawnergroupbrowser.src`, `areaspawnerchestbrowser.src`, `eventareaspawnerchestbrowser.src`
- `scripts/include/managers.inc`, `scripts/include/constants/propids.inc`, `scripts/playermanager.src`

A brand-new spawner system, built and iterated on entirely within this patch's range. Unlike `pkg/opt/spawnpoint`, an Area Spawner has no physical world item — it's a realm-scoped, datafile-backed record covering a box region, managed entirely through admin commands and gumps.

**Five spawner kinds**, each independently manageable and each available in both a regular and an "Event" flavor (identical behavior, separate datafile prefix so event spawners can be bulk-toggled without scrolling past the regular list):
- **Area** — regular NPC template spawns, placed via `.cfg`-declared templates.
- **Custom NPC** — a full in-gump character builder (see Theme 2) saved and placed per-entry, independent of any `npcdesc.cfg` template.
- **Chest** — spawns from a curated container catalog (`areachests.cfg`/`eventareachests.cfg`, 142/141 lines respectively; deliberately hand-picked graphics rather than "any itemdesc entry", since `spawnpoint`'s Container kind has no real whitelist either). Loot, lock difficulty, and trap are rolled automatically from `config/nlootgroup.cfg` lootgroups 301-307 at spawn time, matching `spawnpoint`'s existing Container behavior.
- **Item** — plain item template spawns.
- **Individual** — a single hand-picked template/count, for one-off placements that don't need a full group.

**Event-driven, not polled**: each entry tracks its own `next_spawn_time`/`refilling` state; `areaspawnerentrytick.src` is a one-shot script kicked on death, on entry save, on spawner enable, and by a coarse 4-hour safety-net sweep (`AreaSpawnerKickAllEnabled`, mirroring `spawnpoint`'s `allspawner.src` pattern) rather than any continuous poll loop. `WarnManagerOfNpcDeath` (`managers.inc`) now calls a new `WarnAreaSpawnerOfNpcDeath()` ahead of the existing spawnpoint logic, reading a new `PROPID_MOBILE_AREASPAWNER` ("AreaSpawner") ObjProperty off the corpse to route the death back to the right entry.

**NPC group catalog** (`areagroups.cfg`, curated, not auto-generated): organized by `npcdesc.cfg` SlayType, one block of 100 IDs per slaytype, tiered Peons/Warband/Horde/Elders/Paragons/Titans by total-STR and Boss/LesserBoss/SuperBoss CProps — a group picks a random template from its `spawn` list the same way a chest group picks a random `item`.

**Chest lifecycle wiring** — previously nothing told an Area Spawner (or, in the unlock-command case, `spawnpoint` either) that one of its placed chests was gone:
- `pkg/items/containers/container/canDestroy.src` (the generic per-container `DestroyScript` hook already used by `spawnpoint`'s `WarnSpawnPoint`) now also calls a new `NotifyAreaSpawnerChestGone()`, a no-op for anything neither package spawned.
- `pkg/std/lockpicking/use/picklock.src`'s `PickTreasureChest` additionally calls the new `NotifyChestUnlocked()` (`scripts/include/chestunlock.inc`, new) right after a successful pick, as a speed optimization that kicks the refill immediately instead of waiting for the chest's scheduled destroy 5 minutes later. `spawnpoint` deliberately does *not* get an equivalent immediate call here, since its `spawndeath.src` has no duplicate-fire guard (unlike Area Spawner's `RemoveAreaSpawnerSerial`) and is already correctly notified once, at actual destroy time.
- `scripts/textcmd/admin/unlock.src` (the GM override unlock command) previously just cleared `.locked` and stopped, leaving a spawner-placed chest sitting there forever with nothing ever noticing the slot had opened. It now checks for either `PROPID_MOBILE_SPAWNPOINT_SERIAL` or the new `PROPID_MOBILE_AREASPAWNER` and schedules the same 30-second `::misc/deleter` cleanup `PickTreasureChest` already used, so destruction (and the notification chain above) still happens.

**Consolidated error reporting**: originally shipped as 6 separate per-kind `.aserror*` commands; consolidated down to one **`.aserrorall`**, which lists every auto-disabled spawner or paused entry across every kind and event/non-event flavor in a single gump (matching the exact scope the login/staff notice already scans, closing a scope-mismatch bug where the notice could report a problem `.aserrorarea` alone couldn't see). New **`.aslist`** gives a per-facet census — region name, status, population, and each entry's next-fire timer and contents, across every spawner of every kind in the chosen realm — including a `[chain active]` tag on any entry whose kick chain hasn't finished yet, so a stuck-in-progress entry is visible at a glance.

**Production-hardening fixes found during this patch's own review and live testing**:
- `TryPlaceAreaSpawnerChest`'s placement loop didn't check for an existing container already standing on the target tile, so two chests could stack on the same spot — fixed with a `ListItemsNearLocation`/`IsA(POLCLASS_CONTAINER)` occupancy check (NPCs are unaffected; only containers needed this).
- A structural bug where an entry's `refilling` flag (guards against a duplicate concurrent kick chain) had no recovery path if the tick chain ever died mid-run — nothing (not re-enabling the spawner, not editing+saving an entry, not even the 4-hour safety net) could ever clear it, permanently stalling that entry at 0 population with no error shown. Fixed by having `AreaSpawnerResetEntryCooldowns()` (already called on enable/save) also clear `refilling`, wiring that same reset into the safety-net sweep (which previously reset nothing at all), and adding the `refilling := 0` reset to `UpdateAreaSpawnerEntry` directly.
- `RemoveAreaSpawnerSerial` (fired on every NPC death) was unconditionally saving the datafile twice per call in one branch — restructured to save exactly once.
- `AreaSpawnerPrefixLabel()` only recognized the bare `"eventareaspawner"` string, silently mislabeling every other kind/event combination in gumps — rewritten to properly compose the kind label with an `AreaSpawnerIsEventPrefix()` check.
- Dead `else { ShowNotImplemented }` branches removed from both `.areaspawner` and `.eventareaspawner` (all 5 kinds have been fully implemented for a while; the fallback was unreachable), and the now-unused `ShowNotImplemented` gump function deleted.

Expected impact: a full realm-scoped, no-world-item alternative to `spawnpoint` for area-based spawning across all 5 content kinds, with its own admin tooling, error reporting, and self-healing recovery from a stuck kick chain.

### 2. Area Spawner — Custom NPC Editor

Files involved: `pkg/opt/areaspawner/include/areaspawner.inc`, `include/areaspawnergump.inc`

The Custom NPC editor lets an admin build a one-off character (independent of any `npcdesc.cfg` template) directly in a gump, saved per-entry. Restructured mid-patch from a single flat field list into four category sub-pages, each a modal `SendDialogGump` returning the updated overrides struct:
- **Appearance** — name, objtype/body, true color/hue, gender.
- **Stats** — hits/stamina/mana, movement speed, boss tier (Lesser Boss / Paragon (Boss) / Super Boss, in that order), regen rates.
- **Prots** — all 7 damage-type protections plus permanent poison/magic immunity and free action.
- **Other** — slaytype (own picker gump, mirroring the 16-entry list from `pkg/systems/combat/include/hitscriptinc.inc` since no runtime enumeration API exists for it), snoop/steal/loot flags, permanent magic resistance.

A read-only **LG/MIL/MIC line** (lootgroup / MagicItemLevel / MagicItemChance) now shows directly under the template picker via a new `AreaSpawnerNpcLootInfo()`, so an admin can see what a chosen base template would drop without leaving the editor.

**Bug fix**: Hits/Stamina/Mana overrides were being applied via `SetVital()` alone, which is both 100x under-scaled against how `CustomHitsLevel`/`CustomManaLevel`/`CustomStaminaLevel` are actually stored (hundredths, read live by `pkg/opt/shilhook/regen.src`'s max-vital functions) and never touched the max-vital ceiling, only the current value. Now sets the `Custom*Level` ObjProperty (value x100) alongside the vital itself, so a configured Hits/Stamina/Mana override actually raises the NPC's real cap instead of just topping off a vital that would immediately re-clamp down.

A defensive field-by-field migration copy was added when loading a previously-saved entry's overrides, so an older saved entry (from before a field existed) doesn't hard-error on a missing struct member when the editor opens.

Expected impact: building a custom NPC is now organized into logical categories instead of one long field list, includes a previously-missing Boss tier and correctly-scaled vital overrides, and shows loot-table info for the chosen base template up front.

### 3. New Sosaria Teleporters & Cleanup

Files involved: `scripts/include/teleporters.inc`

~50 new teleporter pairs added across Sosaria (`britannia_alt`) connecting the overworld to a set of caves/dungeons, plus a restructured Water Dungeon that now has distinct Level 1 through Level 6 connections (previously some levels shared ambiguous/duplicate links), new links for Wizard Tower of Renah <-> Wayfarer's Abyss <-> Renah's Sanctum, and Cult of the Serpent's Isle -> Mine. Also removed: 4 stale duplicate Water Dungeon teleporters that were superseded by corrected coordinates added in the same pass, and 4 commented-out "BAD TELEPORTER - REMOVE" entries that were left in as dead reference clutter.

Expected impact: several previously unreachable or ambiguously-linked Sosaria dungeon areas are now reachable via teleporter; the Water Dungeon's level progression is unambiguous.

### 4. Decorate Facets — Britannia Alt Door Registrations

Files involved: `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg`

A near-total rewrite of the Sosaria (`britannia_alt`) decorate-facet door config (+3564/-4344 lines).

Expected impact: door placements on the Sosaria decorate facet should now open/function correctly where they previously didn't; not independently re-verified tile-by-tile as part of this documentation pass given the scale of the rewrite.

### 5. New `.setpropradius` Developer Command

Files involved: `pkg/opt/alryc/textcmd/test/setpropradius.src` (new), `config/command_synopses.cfg`

`.setpropradius <propname> <value> <radius>` sets a named property member on every item within the given radius of the caller, type-coercing the value to match the property's existing type (Integer/String, defaulting to Integer). Reports how many items were changed vs. skipped (skipped = the property doesn't exist on that item via `get_member`/`errortext`). CmdLevel 5 (Developer).

Expected impact: bulk property edits across a physical area (e.g. tagging every item in a test region) no longer require hand-editing each item individually.

### 6. Speedwalk Fix

Files involved: `pkg/opt/alryc/include/speedwalk.inc` (new), `pkg/systems/accounts/logon.src`, `scripts/textcmd/seer/speedwalk.src`, `config/command_synopses.cfg`

`.speedwalk` and the on-login speed-walk restore both called `mobile.SpeedWalk(toggle)`, a method that lives on the (disabled) `objClassMethods` package — with that package off, the call silently no-op'd. New `SendSpeedWalk(mobile, toggle)` sends the same underlying client packet (`0xBF` subcommand `0x26`) directly, with no dependency on `objClassMethods` being enabled, and both call sites were switched to it.

Expected impact: `.speedwalk` and the automatic speed-walk restore on login actually work again.

### 7. New Wyrm Reward Tiers & Loot Group Cleanup

Files involved: `config/npcdesc.cfg`, `config/nlootgroup.cfg`

6 new tiered Wyrm NPC templates (`Wyrm9Weapon`/`Wyrm10Weapon`/`Wyrm11Weapon`, `Wyrm9Armor`/`Wyrm10Armor`/`Wyrm11Armor`, extending the existing lower-tier Wyrm ladder), plus 7 new rideable mount templates (`mountarmoredswampdragon`, `mountmondainshorse`, `mountridgeback`, `mountsavageridgeback`, `mountskeletalhorse`, `mountswampdragon`, `mountunicorn`) and a new `barber` NPC template. `nlootgroup.cfg`'s lootgroups 196/197 (armor/weapon drop tables, largely commented-out) were folded into groups 198/199 with `GMArmor`/`Pentagrams` entries re-enabled at full weight, which the new Wyrm tiers draw from.

Expected impact: a higher-tier armor/weapon Wyrm reward ladder (levels 9-11) with meaningfully better drops (GM-quality gear, pentagrams) than the existing lower tiers; several new mounts and a barber NPC are available to spawn.

### 8. Debug Logging Disabled for Production

Files involved: `pkg/items/donationbox/include/donationbox.inc`, `pkg/opt/areaspawner/include/areaspawner.inc`

`DONATION_DEBUG` (donation box sweeper) was already flipped off (`1`->`0`) as part of the wyrms commit. `AREASPAWNER_DEBUG` (the single flag gating every `Print("[areaspawner] ...")` call across the whole package, used heavily this patch to live-debug the stuck-refilling issue in Theme 1) is flipped off the same way at the end of this range, ahead of going live. Both follow the same pattern: a single `AreaSpawnerDebug(msg)`/`DonationDebug(msg)` wrapper function gates every console print behind one const, so no call sites needed touching — just the flag.

Expected impact: no more `[areaspawner]`/donation-box console spam in normal operation; flipping either flag back to `1` re-enables full diagnostic logging for that subsystem if needed again.

### 9. Boat Package Overhaul — Galleon Support, Speed, Ropes, Cargo Holds

Files involved: `pkg/multis/boat/include/boatHelpers.inc` (new), `pkg/multis/boat/boat/use.src`, `pkg/multis/boat/multi/listener.src`, `pkg/multis/boat/rope/use.src`, `pkg/multis/boat/tiller/use.src`, `pkg/multis/boat/tiller/methods.src`, `pkg/multis/boat/commands/gm/restartboat.src`, `pkg/multis/boat/config/itemdesc.cfg`, `config/mrcspawn.cfg`

The Orc/Gargoyle/Tokuno/Britannia galleon deeds existed in `config/itemdesc.cfg` and `config/boats.cfg` (stock polserver engine data) but were never fully wired up against this package's control/cargo logic, which had only ever assumed the classic small/medium/long/dragon boat layout (`Tillerman` + `Hold` + gangplanks). `config/boats.cfg` only gives Row Boat a `Tiller` and every galleon/Britannia hull a `Wheel` — none of the newer families get a `Tillerman` or `Hold` at all, and none of those newer extobj roles (`Tiller`, `Wheel`, `Storage`, ...) are exposed as real dot-properties the way `.tillerman`/`.hold`/`.portplank`/`.starboardplank` are (confirmed in testing: `boat.wheel` returns a live-looking placeholder object with objtype 0 regardless of whether a Wheel component actually exists, even for a verifiably clickable one).

**New `boatHelpers.inc`** centralizes generic resolution so the rest of the package can treat every ship family the same way:
- `IsValidBoatItem()` — treats objtype 0 as invalid in addition to falsy/errortext, needed because a missing extobj role doesn't reliably come back as a clean null.
- `GetBoatController()` — resolves the Tillerman (classic), Tiller (Row Boat), or Wheel (galleons/Britannia) generically by scanning `boat.components` when the dot-property fails, caching the resolved serial on the boat (`ControllerSerial`) since this is called every tick from the main loop.
- `GetBoatHolds()` — resolves the native Hold (classic), every native Storage crate (Britannia, several per ship), or creates+registers (`RegisterItemWithBoat`) a single escript-created container for Orc/Gargoyle/Tokuno, tracked via a `CustomHoldSerial` property so it survives restarts. Row Boat explicitly returns no hold (by design — it's the one ship family meant to have no cargo hold).
- `GetPreferredHoldOffset()` / `FindHoldSpot()` — place the custom hold near the controller rather than at `boat.x/boat.y` (the multi's anchor tile, not necessarily open deck — confirmed in testing this landed the hold half-inside the mast/half-underground). Orc/Gargoyle/Tokuno each got a specific hand-picked `(dx,dy)` offset from the wheel, found by spawning a visible placement-blocker item in-game and reading its coordinates back from a world save, after the generic nearby-wheel search proved unreliable for those hull shapes; other/future ship types fall back to that generic search.
- `GetBoatLockId()` — reads a boat's LockID off one of its ropes (always present, always locked at placement) rather than storing it anywhere centrally, so a custom hold that needs to be recreated later (see below) can still be locked to the same key as the rest of the boat, on boats that predate this patch too.
- `DestroyCustomBoatHold()` — now also destroys the owner's key (backpack or bank) for the hold's LockID before destroying the item, rather than relying on the caller to have already done it.

**Cargo hold visibility bug**: `config/itemdesc.cfg`'s `Container 0x1F01A` (Storage) entry has a default `Graphic` of `1` — an essentially invisible placeholder. Native engine-created Storage components (Britannia's crates) were rendering with that default the whole time; `GetBoatHolds()` now explicitly overrides `.graphic` (`0x5C2D`) on every native Storage component it resolves, matching what the escript-created custom hold already does. The custom hold's graphic went through two iterations: `0x5C2A` (Britannia's own native crate graphic) rendered floating well above the deck for a standalone item, because that art relies on the multi's own plane/component height data (which only applies to real `boats.cfg` components) rather than the item's logical Z — the same reason a boat component's own `.z` reads back as the multi's waterline Z regardless of how high it visually sits on deck (confirmed directly: the Wheel's `.z` matched `boat.z` exactly). `0x5C2D` was chosen in-game instead and confirmed to self-elevate correctly from `boat.z` on its own.

**`itemdesc.cfg`**: the Wheel (`0x1F015`)'s `Script`/`MethodScript`/`CanInsertScript` were commented out — re-enabled, since the Wheel was completely non-functional without them. Rope (`0x1F014`)'s `DoubleClickRange` raised from `5` to `9`, since it was silently capping click range below what the per-ship-type boarding distances in `rope/use.src` (2 to 9 tiles depending on hull) already expected.

**Disembark search rewrite** (`rope/use.src`): the search for a valid off-ship spot to place a disembarking player was rewritten from scratch through several iterations during in-game testing (visualized at one point by spawning marker items at every candidate tile and color-coding pass/fail, before being converted back to a silent single-move production version). Final design: one `GetStandingCoordinates()` call per attempt (previously guessed candidate heights one at a time via `GetStandingHeight`, which proved unreliable near a boat, resolving to the sea floor under the deck in one case); an outward-direction bias (`GetOutwardBias`) computed from the ship's real `.facing` property plus its bounding-box center (`GetShipBounds`, built from `boat.components`) rather than the rope's raw offset from the multi's anchor tile, which gave inconsistent, sometimes wrong-signed results for ropes at different points along the same hull (three ropes on the same north edge of the same ship produced three different biases under the old method); and a rejection of any candidate belonging to *any* multi — this ship's own deck, or a different multi entirely (a house, another docked boat) — not just this ship, since `GetStandingCoordinates` only reports whether a tile is standable, not whose space it belongs to.

**Fixed 5-tier speed system** (`tiller/methods.src`, `multi/listener.src`): replaced the old freeform ms-delay `SetSpeed()`/`GetSpeed()` (250-450ms range, a barely-perceptible 1.8x spread, plus a lazy-init bug where the very first read after boat creation returned a stale pre-fetch value instead of the just-initialized default) with 5 fixed tiers stored as a `SpeedLevel` index (1-5, default 3) mapped through `GetSpeedTable()`: 760/380/180/130/80ms per tile-move, i.e. 1/2.6/5.6/7.7/12.5 tiles-per-second. Calibrated against this shard's own real movement delays (`config/servspecopt.cfg`'s `SpeedHack_MountRunDelay`/`MountWalkDelay`/`FootRunDelay`/`FootWalkDelay`) rather than arbitrary numbers: level 1 is 2x slower than walking on foot, level 2 matches walking, level 3 matches running on foot (also mounted-walk pace, tied in this shard's config), level 5 matches mounted running (level 4 is an interpolated step with no real-movement equivalent). Speed Up/Down now move one tier at a time instead of adding/subtracting a raw ms amount.

**Other `multi/listener.src` fixes**:
- `ProcessEvent(<uninitialized object>)` console error: `DryDock()` destroys the boat multi, but `ProcessEvent` was continuing to execute afterward using the now-destroyed `boat` reference — fixed with an early `return 1;` immediately after the `DryDock()` call.
- Dry-docking now closes the Wheel/navigator gump (a separate process with no way to know the boat is about to be destroyed) via a new `NavigatorPID` property set on the mobile when `tiller/use.src` starts the navigator script, and `GFCloseGump()`'d from `DryDock`.
- `ClosePlanks` guards both planks with `IsValidBoatItem()` before calling `.Extended()` — galleons/Britannia have no Gangplank components at all.
- Random sea-monster spawning near boats (`DoEncounter`) disabled via an early `return 0`, spawn code left in place below it (unreachable) rather than deleted, for an easy one-line revert.

**Debug logging removed for production**: ~65 `Print("[BoatDebug] ...")` console statements added across this patch's iterative in-game debugging (every file listed above) were stripped once each underlying fix was confirmed working; the `use basicio;` imports that existed only to support them were removed alongside.

**Code review finding, not acted on**: `tiller/methods.src` has a complete, unused crew/ownership permission system (`CanCommand`/`IsCrewMate`/`AddCrewMate`/`RemoveCrewMate`/`GetCrewMates`/`SetCrewMates`) that `ProcessEvent` never calls — it only checks `IsOnBoat`, so currently any mobile standing on a boat (not just the owner) can issue movement/anchor/speed/drydock commands. Left as-is pending a decision on whether to wire it in or remove the dead code.

**`config/mrcspawn.cfg`**: the Shipwright's `ProductGroup ShipItems` now stocks all 5 galleon/Row Boat deeds (`rowboatdeed`, `orcboatdeed`, `gargoyleboatdeed`, `tokunoboatdeed`, `britannianboatdeed`) alongside the existing classic boat models — they already had `Name`/`VendorSellsFor`/`VendorBuysFor` set in `itemdesc.cfg`, just weren't in any merchant's stock list.

Expected impact: Orc, Gargoyle, Tokuno, and Britannia galleons (plus Row Boat) are now fully functional — steering, anchor, speed, dry-docking, boarding/disembarking via rope, and cargo storage all work per ship family, matching the classic boats' feature set — and are purchasable from Shipwright NPCs.

---

## Validation Notes

- Diff range for this update: `git diff f80feec..HEAD` (full range) and per-commit `git show <hash>` for individual theme detail.
- Working tree state at time of writing: the `AREASPAWNER_DEBUG` flag flip (Theme 8) is uncommitted: it's confirmed by direct in-game observation across the `.aslist`/console-log debugging session that everything it covers is working (stuck-refilling recovery, chest occupancy, all 5 spawner kinds placing/ticking/rescheduling correctly).
- Theme 9 (boat package) is also entirely uncommitted, developed and verified through an extended live in-game debugging session across all 5 ship families (Row Boat, Orc, Gargoyle, Tokuno, Britannia) - each fix in the summary above was confirmed against real console log output and/or in-game observation before moving to the next, including several multi-iteration items (the disembark search, custom hold placement/graphic, speed range) that were deliberately re-tested after each change.
- Per repo convention, compile validation was not run as part of this documentation pass — the user compiles EScript changes themselves.
