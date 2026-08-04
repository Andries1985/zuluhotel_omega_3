# Developer Changelog - v3.0.5

Range: Patch-3.0.4..Patch-3.0.5 (commit `2afb166`..`6cc93f5`)
Branch: Patch-3.0.5
Date: 2026-08-02 through 2026-08-04

---

## Scope Summary

- Total files changed: 26 (excluding the patch-note docs themselves)
- Status breakdown: 26 modified, 0 added, 0 deleted, 0 renamed
- Net textual delta: 6640 insertions, 3663 deletions (text files only; 6 binaries were also rebuilt: `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe`)
- Largest shifts:
  - `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (+3093 / -3489) - renumbered after removing 33 duplicate/erroneous door decor entries
  - `pkg/multis/house/config/itemdesc.cfg` (+3041 / -113) - ~90 new Fiddler-imported house deed entries + Gothic Fortress
  - `core-changes.txt` (+148 / -0) - polserver engine changelog sync, 05-31-2026 through 07-30-2026
  - `config/mrcspawn.cfg` (+123 / -1) - new house deeds added to deed vendor stock
  - `scripts/misc/chrdeath.src` (+63 / -11) - fourth attempt at the mount-stuck-on-death fix
  - `scripts/include/teleporters.inc` (+54 / -18) - new dungeon teleporters + disabled bad routes
  - `pol.cfg` (+20 / -9), `pol.cfg.example` (+11 / -5) - server settings changes following engine sync, plus reverting the debug-watch settings back off
- Non-merge commits in range:
  - New multis from Fiddler NPC fix (`80d5327`)
  - New Core / patch notes update / teleporters fix / naming case fix (`38e8a76`)
  - New update to areas and doors and regions to fix errors / attempt to fix character death being mounted (`6cc93f5`)

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- M | config/mrcspawn.cfg
- M | config/npcdesc.cfg
- M | core-changes.txt
- M | pkg/multis/house/config/itemdesc.cfg
- M | pkg/multis/house/include/footagearrays.inc
- M | pkg/multis/house/multiDeed/use.src
- M | pkg/multis/staticHousing/logoff.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/opt/areas/areas.cfg
- M | pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg
- M | pkg/utils/gumps/commands/admin/selectiongump.src
- M | pol.cfg
- M | pol.cfg.example
- M | pol.exe
- M | poltool.exe
- M | regions/regions.cfg
- M | scripts/ecompile.exe
- M | scripts/include/teleporters.inc
- M | scripts/misc/chrdeath.src
- M | scripts/modules/npc.em
- M | scripts/modules/os.em
- M | scripts/modules/uo.em
- M | scripts/modules/vitals.em
- M | scripts/runecl.exe
- M | uoconvert.exe
- M | uotool.exe

---

## Detailed Changes By Theme

### 1) New Player Housing - Fiddler-Imported House Deeds + Gothic Fortress

Files involved:
- `config/mrcspawn.cfg`
- `pkg/multis/house/config/itemdesc.cfg`
- `pkg/multis/house/include/footagearrays.inc`
- `pkg/multis/house/multiDeed/use.src`

Notable functional changes:
- Added roughly 90 new `Item` entries to `pkg/multis/house/config/itemdesc.cfg` for house deeds imported via the Fiddler tool (e.g. `multi147ehousedeed` through `multi14a1housedeed`, and a further `multi1xx` series), each with `numlockdowns`, `numsecure`, and `numomegacache` cprops and a `HouseObjType` pointing at a matching multi.
- Added a dedicated `gothicfortressdeed` entry (`HouseObjType gothicfortress`) with its own `numlockdowns i577`, `numsecure i29`, `numomegacache i3`.
- Added `gothicfortressarray(house, x, y, z)` to `footagearrays.inc`: builds a 15/15/67-high lockdown box and spawns 4 `bantile` items along the north-ish edge (`y+16`), recording their serials on the house via `SetObjProperty(house, "bantiles", banarray)`.
- `GetHouseFootprintBounds()` in `multiDeed/use.src` gained a case for `0x1607D` (Gothic Fortress bounds) and its `default:` branch no longer unconditionally `return 0`s: it now reads the house's `itemdesc` entry, resolves `MultiID` via `CInt(elem.MultiID)`, calls `GetMultiDimensions(multiid)`, and derives `bounds` from `dims.xmin/ymin/xmax/ymax` when available.
- `FindBoxArray()` added a `0x1607D` case calling `gothicfortressarray(...)`, and now falls back to a new `BuildGenericFootageArray(housetype, house)` function whenever no hand-written array function matched. `BuildGenericFootageArray` looks up the house's `itemdesc` entry, resolves `MultiID`, calls `GetMultiDimensions`, and builds a single lockdown box from `dims.xmin/ymin/xmax/ymax` offset by the house position (with `z-8`/`z+80` vertical bounds), also initializing `SetObjProperty(house, "bantiles", {})`.
- `config/mrcspawn.cfg`: added `Item ... 10` vendor-stock lines for `gothicfortressdeed` and all ~90 new `multiXXXhousedeed` entries under `ProductGroup Deeds`.

Expected impact:
- All newly imported house types automatically receive a working lockdown/secure/ban-tile footprint derived from their multi dimensions, without needing a hand-written footprint function, closing a gap where new house types previously got no lockdown region (`return 0`) unless explicitly coded.

### 2) NPC Data Fix

Files involved:
- `config/npcdesc.cfg`

Notable functional changes:
- `NpcTemplate hornedrat`: `Color` and `TrueColor` changed from `1947` to `0` (default hue), removing an incorrect custom color.

### 3) Teleporter Network Updates

Files involved:
- `scripts/include/teleporters.inc`

Notable functional changes:
- Commented out (prefixed with a `//BAD TELEPORTER - REMOVE` marker comment, left in place rather than deleted) several duplicate/incorrect one-way teleporter entries:
  - Winterwyn Mining: `{5183,1161,0,...}` and `{5184,1161,0,...}` (both `-> Winterwyn Mining`), and `{1068,629,21,...}`/`{1069,629,21,...}` (`Winterwyn -> Winterwyn Mine`).
  - Cult of the Serpent Isle <-> Nexus Outskirts: all 6 entries (`2210/2211/2212,1097,...` and `5793/5794/5795,457,...`).
  - Cult of the Serpent Isle -> Mine: `2210/2211/2212,1097,... -> 6311/6312/6313,3003,...`.
  - Nexus Outpost -> Underdark: `5793/5794/5795,457,... -> 5490/5491/5492,64,...`.
  - Mine -> Sosaria: `5901,2076,0,...` and `5901,2077,0,...`.
- Added a replacement pair `{5902,2076,0,4613,475,5,...}` / `{5902,2077,0,4613,476,5,...}` (Mine -> Sosaria) under a new `//Nagash Added August 2026` block.
- Added new two-way teleporter groups under the same block:
  - Lost City <-> Ancient Sewers: `719/720/721,1424,22 <-> 6391/6392/6393,357,22`.
  - Ancient Sewers <-> Vault of the First Dynasty Lvl1: `6418/6419,185,0 <-> 6798/6799,459,0`.
  - Vault of the First Dynasty Lvl1 <-> Lvl2: `6878,515/516,0 <-> 6621,679/680,0`.

Expected impact:
- Players gain new travel routes between Lost City, the Ancient Sewers, and the Vault of the First Dynasty (both levels). The disabled entries stop routing players through duplicate/incorrect one-way links in the affected areas; they remain in the file as commented-out markers for future cleanup rather than being deleted outright.

### 4) Engine Core Sync (polserver)

Files involved:
- `core-changes.txt`
- `pol.cfg`
- `pol.cfg.example`
- `scripts/modules/npc.em`
- `scripts/modules/os.em`
- `scripts/modules/uo.em`
- `scripts/modules/vitals.em`
- `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe`

Notable functional changes:
- `core-changes.txt` gained 18 upstream entries dated 05-31-2026 through 07-30-2026 (see [[reference_polserver_engine_source]] for upstream detail). Highlights:
  - **Attackable items**: new itemdesc property `Attackable 1/0` (default 0) lets a non-mobile item be engaged in combat like a mobile (attack packet or doubleclick in warmode), take damage against `HP`/`MaxHP`, always be hit on a swing, and (with a ControlScript) receive `SYSEVENT_DAMAGED`/`SYSEVENT_ENGAGED`/`SYSEVENT_DISENGAGED`/`SYSEVENT_OPPONENT_MOVED`. Without a ControlScript the item is destroyed at 0 HP. Only top-level world items are attackable; picking up/containerizing/equipping/realm-moving disengages attackers. Attackable items do not stack. Requires client 7.x.
  - **Module signature updates** reflecting the above: `npc.em SetOpponent(character)` -> `SetOpponent(attackable)`; `uo.em ListHostiles(character, ...)` -> `ListHostiles(attackable, ...)`; `vitals.em ApplyDamage(mobile, ...)` -> `ApplyDamage(attackable, ...)` and `ApplyRawDamage(character, ...)` -> `ApplyRawDamage(attackable, ...)`. These are parameter-name-only changes (no default/behavior change for existing mobile-only callers).
  - **Hostname resolution**: switched from `gethostbyname` to `getaddrinfo`; address selection for SERVERS.CFG `--ip--`/`--lan--` may now differ on multi-adapter hosts (Hyper-V/WSL/VPN), and unresolvable SERVERS.CFG hostnames are now reported at startup on Windows too.
  - **AOS tooltip title fix**: title separators (`ServSpecOpt` `TitlePrefixSeparator`/`TitleSuffixSeparator`/`TitleRaceSeparator`/`TitleGuildSeparator`) were being inserted incorrectly for unset title parts; the tooltip now builds titles with the same code path as the paperdoll. Default-separator output is unchanged.
  - **Login server performance**: the single-thread login listener now waits on and accepts all already-queued client connections per pass instead of one at a time; measured (loopback, 75 clients) login packet latency dropped from 247ms to 0.8ms and admitting 75 connections dropped from 28.7s to 0.8s.
  - **`LoginServerSelectTimeout` removed** (now a no-op if left in `pol.cfg`) and replaced by **`StalledPeerTimeout`** (seconds, default 60): bounds how long a peer may accept no data at all before the core closes an aux-service/`os::OpenConnection()`/webserver-page connection; progress resets the countdown; minimum 1s; re-read on config reload.
  - Aux service and `os::OpenConnection()` sockets are now non-blocking with the same 60s stalled-peer cutoff (previously a stuck peer could block on `send()` for the OS TCP timeout, minutes).
  - Webserver connections are now non-blocking (a page script write that doesn't fit sleeps/resumes instead of blocking the whole shard's script lock); plus a config-reload data race fix, transient `accept()` errors no longer kill the webserver thread, error pages no longer reflect unescaped request data (XSS fix), Nagle disabled for HTTP, `Content-Length`/`Connection` headers + larger transfer buffer, GET-only (405 otherwise), 64-header-line/10s request limit, and MIME list additions (css/json/svg/txt/webp/woff2/wasm).
  - `DebugLocalOnly`/`WebServerLocalOnly` (both default 1) now actually bind the listener to `127.0.0.1` instead of merely filtering by peer address, and now accept the whole `127.0.0.0/8` range rather than only `127.0.0.1` exactly.
  - `os::OpenConnection()` gained a `connect_timeout_ms` parameter (default 10000; 0 restores old blocking-until-OS-default behavior) and now resolves hostnames (previously non-numeric hosts silently connected to `255.255.255.255`).
  - Line-buffered connections (aux services, webserver, remote debugger) cap unbounded line buffers at 16MB and only reset receive timeouts on actual data.
  - Two ShortCircuit codegen bugs fixed: a nested `(a && b) || (c && d)` assigned via `var x := ...` left an uninitialized stack value; and a separate ShortCircuit optimization produced wrong instructions in certain cases. Both require script recompilation.
  - `uoconvert`: fixed a UOP size-estimator bug that dropped a block from the final chunk (broke conversion of Ilshenar/map2 and Tokuno/map4), fixed a `maptile.dat` floor-vs-ceil block-index bug that corrupted the easternmost/southernmost tile-block on realms whose dimensions aren't multiples of 64 (bumping `RealmDescriptor::VERSION` 1->2, requiring realm reconversion), and parallelized `map`/`maptile` conversion across CPU cores (new `threads=` argument, default all cores) with byte-identical output.
  - A `customhouse` include/inc: `addhousepart`/`erasehousepart` now also accept a single array-of-struct `{graphic,xoffset,yoffset,z}` argument for adding/erasing multiple parts at once.
  - Misc data-race/socket fixes: aux connection close vs. deferred transmit race; macOS `POLLHUP`-before-drain data loss on socket close.
- `pol.cfg`: `LogLevel`, `WatchRPM`, `WatchSysLoad`, `LogScriptCycles` all changed from `0` to `1` (this shard now logs/watches these by default); added `DefaultPriority=10` documentation block; replaced the `LoginServerSelectTimeout` block with a documented `StalledPeerTimeout=60`; commented out `DebugLocalOnly=1` (now implied/enforced by the engine default) and `MiniDumpType=variable`; added a clarifying comment line to `DebugLocalOnly`'s doc block noting the new bind-to-127.0.0.1 behavior. (`LogLevel`, `WatchRPM`, `WatchSysLoad`, and `LogSysLoad` were then reverted back from `1` to `0` in `6cc93f5` after the initial sync landed; `LogScriptCycles` was left untouched.)
- `pol.cfg.example`: same `StalledPeerTimeout` doc replacement and `DebugLocalOnly`/`WebServerLocalOnly` clarifying comments as `pol.cfg` (values left at their example defaults).
- Rebuilt `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe` against the synced engine revision.

Expected impact:
- Faster, more resilient logins under concurrent load; the shard can no longer be frozen by a slow/stalled aux, webserver, or debug connection; `DebugPort`/webserver are no longer reachable from outside the host by default; AOS tooltips render titles consistently with the paperdoll; two rare script-codegen bugs are fixed (recompilation needed for scripts affected by the ShortCircuit bugs); `uoconvert` map conversions are faster and correct for previously-broken realm dimensions; a new `Attackable` itemdesc property and matching module signature changes are available for future content but are not yet used by any script in this repo.

### 5) Include Path Case-Sensitivity Fixes

Files involved:
- `pkg/multis/staticHousing/logoff.src`
- `pkg/multis/staticHousing/sign/control.src`
- `pkg/utils/gumps/commands/admin/selectiongump.src`

Notable functional changes:
- `logoff.src`: `include "include/eventID"` -> `include "include/eventid"`.
- `sign/control.src`: `include "include/eventID"` -> `include "include/eventid"`; `include ":gumps:yesNo"` -> `include ":gumps:yesno"`; `include "include/sysEvent"` -> `include "include/sysevent"`.
- `selectiongump.src`: `include ":gumps:selectionGump"` -> `include ":gumps:selectiongump"`.

Expected impact:
- Matches the on-disk (lowercase) filenames, avoiding include-resolution failures on case-sensitive filesystems/toolchains per [[reference_escript_language_basics]].

### 6) Mount-Stuck-On-Death Fix (Fourth Attempt)

Files involved:
- `scripts/misc/chrdeath.src`

Notable functional changes:
- This is the fourth attempt at fixing the bug tracked in [[project_mount_stuck_on_death_bug]] (ghost left with a stuck mounted pose/speed after PvP death); not yet confirmed fixed live.
- Mount-location lookup now checks a fourth location: `foreach item in ListRootItemsInContainerOfObjtype(ghost,0x1F021)` over the ghost's root contents, in addition to the existing corpse-layer-25, ghost-layer, and corpse-root-contents checks. Matches the same four locations `clearmount.src` (the manual GM recovery command for this bug) already checks.
- Added an explicit `Print()` when no mount item is found in any of the four locations, for diagnosing recurrences from console output.
- Added a "belt-and-suspenders" sweep after the normal dismount: loops all equipment layers 1-30 (skipping `LAYER_MOUNT`) destroying any stray item with `objtype == 0x1F021`, and separately scans the ghost's backpack root contents (`EnumerateItemsInContainer(ghost.backpack, ENUMERATE_ROOT_ONLY)`) destroying any mount-objtype item found there. Each destroy attempt falls back to `ReleaseItem()` + `SleepMS(50)` + retry if `DestroyItem()` fails outright.
- Added explicit clearing of `DMountSerial`, `bmSpeed`, and `SpeedWalk` obj-properties (previously only `DonatorMounted` was erased), matching everything `clearmount.src` resets by hand.
- Replaced the old `SendPacket(ghost, "BF0006002600")` stale-movement-packet reset with a hide/unhide cycle: `ghost.hidden := 1; SleepMS(50); ghost.hidden := 0;` before the existing `IncRevision()` + `MoveObjectToLocation(..., MOVEOBJECT_FORCELOCATION)` client-resync call. Rationale documented inline: a forced/instant mount-to-ghost transition (dying while mounted, rather than a normal double-click dismount) is a known UO client bug class where the client keeps rendering the pre-transition mounted pose; briefly removing and re-inserting the mobile forces a full client redraw, using the same technique already applied elsewhere in this codebase (`pvp.src`, `pvp2vs2.src`, `chaosmultikillpcs.src`).

Expected impact:
- More mount-item locations and stale properties are now caught and cleared on death, and the client-resync method changed from a raw movement packet to a hide/unhide cycle. Whether this actually eliminates the stuck-mounted-ghost bug is unconfirmed — watch server console `Print()` output (the new "no mount item found..." and "destroying stray mount item..." lines) if it recurs.

### 7) Dungeon Region/Area Boundary Fixes + Door Decoration Cleanup

Files involved:
- `pkg/opt/areas/areas.cfg`
- `regions/regions.cfg`
- `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg`

Notable functional changes:
- Fire Dungeon: `Area`/`Region` Y-range extended from `2196-2259` to `2196-2559` in both `areas.cfg` and `regions.cfg` (matching pair of edits) — the previous range cut the dungeon's named area/region off partway through its actual footprint.
- Caverns of Despair 2: `Area`/`Region` Y-range extended from `1076-1463` to `1048-1463` in both files, for the same reason.
- `doors.cfg`: removed 33 duplicate door decoration entries (verified by diffing the full old/new entry sets by `(ObjType, X, Y, Z, Color, Realm)` — no coordinates, graphics, or colors changed on any surviving entry, and nothing was added; the 33 removed entries were exact duplicates/overlaps of other door placements). The large line-count diff on this file is from every subsequent `Decor N` block being renumbered after the removals, not from unrelated content changes.

Expected impact:
- Players in the Fire Dungeon and Caverns of Despair 2 areas are no longer cut off from area/region-based mechanics (e.g. guard zones, area-triggered scripts) partway through the dungeon.
- Removing the duplicate door decorations should not change visible behavior other than resolving whatever "errors" (per the commit message) the duplicates were causing — most likely double-triggering or targeting ambiguity on doors placed at the same coordinates.

---

## Validation Notes

- Diff range used: `Patch-3.0.4..Patch-3.0.5` (`2afb166..6cc93f5`), covering all three non-merge commits in range (`80d5327`, `38e8a76`, `6cc93f5`).
- Coverage checks used:
  - `git diff --name-status 2afb166..6cc93f5`
  - `git diff --numstat 2afb166..6cc93f5`
  - `git log --no-merges --oneline 2afb166..6cc93f5`
  - Per-file `git show <commit> -- <path>` review for each theme, plus a scripted old/new entry-set diff of `doors.cfg` to separate real content changes from renumbering noise.
- This changelog is exhaustive for the branch delta at the time of generation (2026-08-04, `HEAD` = `6cc93f5`, working tree clean). If further commits land on `Patch-3.0.5` before release, re-verify the file inventory and commit list above.
