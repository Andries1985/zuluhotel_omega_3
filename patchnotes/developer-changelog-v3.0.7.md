# Developer Changelog - v3.0.7

Range: Patch-3.0.6..Patch-3.0.7 (commit `541e883`..`a642726`, plus uncommitted working-tree changes as of this writing)
Branch: Patch-3.0.7
Date: 2026-08-13

---

## Scope Summary

- Total files changed: 36
- Status breakdown: 25 modified, 11 added
- Net textual delta: 4868 insertions, 286 deletions
- Largest shifts:
  - `pkg/opt/guilds/config/guildstonegraphics.cfg` (+1568 / -0, new file) - full catalog of eligible single-piece guildstone graphics for the picker gump
  - `pkg/opt/guilds/commands/player/guilds.src` (+545 / -5 net across the range) - Membership gump rework, new Guild House Management gump, guildstone workflow, GuildMasterGump authorization fix
  - `pkg/multis/house/config/itemdesc.cfg` (+127 / -127) - guild-related itemdesc entries
  - `pkg/opt/guilds/config/guildstonesets.cfg` (+298 / -0, new file) - multi-piece statue-set guildstone catalog
  - `pkg/opt/guilds/include/guilds.inc` (+337 / -2) - guild registry, house/rank/permission datafile helpers, guildstone lifecycle, shared sign resolution
  - `pkg/opt/guilds/include/guildstonepicker.inc` (+335 / -0, new file) - graphic/set picker gump plus set-placement footprint validation
  - `pkg/opt/alryc/textcmd/test/guildstonelist.src` (+457 / -0, new file) - developer reference tool
  - `scripts/ai/banker.src` (+104 / -6) - per-currency balance speech + Help gump
  - `scripts/textcmd/player/move.src` (+75 / -43) - stack-consolidation rewrite
  - `scripts/textcmd/player/movebag.src` / `movebankcoin.src` (+161 / +150, new files) - new player commands sharing the same consolidation logic
- Non-merge commits in range:
  - Initial Guild Updates: guild stones, guild gump updates, customhouse/housing sign fixes for guilds, banker balance fixes, banker/high priest Help gumps, move/movebag/movebankcoin fixes (`edc8a64`)
  - Wired guilds into housing permissions (`a642726`)
  - Plus a substantial amount of uncommitted work carried out in this session on top of `a642726` (see Themes 3-12 below), not yet committed as of this writing.
- No merge commits in range.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- M | config/command_synopses.cfg
- M | config/mrcspawn.cfg
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/scripts/customhousedeed.src
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/customhousing/signcontrol.src
- M | pkg/multis/house/config/itemdesc.cfg
- M | pkg/multis/house/include/utility.inc
- M | pkg/multis/house/multiSign/control.src
- M | pkg/multis/house/multiSign/method.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/staticHousing/lockunlock.src
- M | pkg/multis/staticHousing/sign/destroy.src
- M | pkg/multis/staticHousing/sign/use.src
- A | pkg/opt/alryc/textcmd/test/createguildstone.src
- A | pkg/opt/alryc/textcmd/test/guildstonelist.src
- M | pkg/opt/guilds/commands/player/guilds.src
- A | pkg/opt/guilds/config/guildstonegraphics.cfg
- A | pkg/opt/guilds/config/guildstonesets.cfg
- A | pkg/opt/guilds/config/itemdesc.cfg
- M | pkg/opt/guilds/include/guildconstants.inc
- M | pkg/opt/guilds/include/guilds.inc
- A | pkg/opt/guilds/include/guildstonepicker.inc
- A | pkg/opt/guilds/items/guildstone/companions.inc
- A | pkg/opt/guilds/items/guildstone/destroy.src
- A | pkg/opt/guilds/items/guildstone/use.src
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/packethooks/megacliloc/toolTips.src
- M | pkg/utils/mdgumps/include/yesNo.inc
- M | scripts/ai/banker.src
- M | scripts/ai/bankerbalancegump.src
- M | scripts/ai/highpriest.src
- M | scripts/include/omegacache_utils.inc
- M | scripts/textcmd/player/move.src
- A | scripts/textcmd/player/movebag.src
- A | scripts/textcmd/player/movebankcoin.src

---

## Detailed Changes By Theme

### 1. Guild Stones — Placement, Graphics, and Statue Sets

Files involved:
- `pkg/opt/guilds/items/guildstone/use.src`, `pkg/opt/guilds/items/guildstone/destroy.src`, `pkg/opt/guilds/items/guildstone/companions.inc`
- `pkg/opt/guilds/include/guildstonepicker.inc`
- `pkg/opt/guilds/config/guildstonegraphics.cfg`, `pkg/opt/guilds/config/guildstonesets.cfg`, `pkg/opt/guilds/config/itemdesc.cfg`
- `pkg/opt/guilds/include/guilds.inc`
- `pkg/opt/alryc/textcmd/test/createguildstone.src`, `pkg/opt/alryc/textcmd/test/guildstonelist.src`

Notable functional changes:
- New guildstone item type: a placeable item that marks a guild's house, selectable from either a curated single-graphic catalog (`guildstonegraphics.cfg`) or a multi-piece "statue set" catalog (`guildstonesets.cfg`, gated behind `IsHouseTowerSizeOrBigger()` — statue sets require a Tower-or-larger footprint, computed via actual `GetMultiDimensions()` against the house rather than a name/type whitelist).
- `guildstonepicker.inc`'s `ShowGuildstoneGraphicPicker()`/`FindGuildstoneConfigIndex()` (merged from earlier separate singles/sets pickers during this session's cleanup) drive graphic selection; `IsGuildstoneSetPlacementValid()` validates every companion piece's landing spot (inside the house + minimum distance from doors, `GUILDSTONE_DOOR_PLACEMENT_DISTANCE`/`GUILDSTONE_DOOR_SET_DISTANCE`) rather than just the anchor piece, closing off a way players could otherwise use a statue set to block doors.
- `companions.inc`'s spawned statue pieces are all named "Guildstone" (via `SetName`) to match the primary piece instead of their raw item description.
- `guilds.inc`'s `DestroyGuildStone(guild)` centralizes guildstone + companion teardown (used by both `DisbandGuild()` and `ChangeGuildHouse()` — a guild's stone can only ever sit inside its current house, so changing or losing the house destroys the stone).
- `use.src`'s `program guildstone(who)` dropped an unused `item` parameter (confirmed safe via the existing `pkg/opt/GMItems/fanofknives.src` precedent for the same pattern) and gained an ownership check on click, and no longer allows non-owners to interact with the guildstone.
- `createguildstone.src`/`guildstonelist.src` are developer-only test/reference tools (`pkg/opt/alryc/textcmd/test/`, CmdLevel 5) for spawning a test guildstone or a full reference grid of every eligible statue/gravestone graphic.

Expected impact: guilds can place a visual guildstone marker at their house, in either a single-graphic or (Tower+ houses only) multi-piece statue-set style; the stone is destroyed automatically if the guild disbands or changes/loses its house; statue sets can no longer be placed to block a door.

### 2. Guild Membership Gump

Files involved:
- `pkg/opt/guilds/commands/player/guilds.src`
- `pkg/opt/guilds/include/guildconstants.inc`

Notable functional changes:
- `ShowMembershipGump()` widened to a 720px panel (from the previous 640px default) to stop the per-rank column headers from overlapping, raised the page size from a smaller default to 15 members per page, and added an explicit warning ("Unsaved changes are lost when paging") since paging rebuilds the gump from the datafile and drops any unsaved rank picks on the current page.
- OK/Cancel buttons moved to bottom-center of the panel; the redundant `GFTextLine` "Okay"/"Cancel" labels next to them were removed once it was noticed the button art (graphics 2128/2129, 2119/2120) already renders that text itself — the labels were overlapping the art's own text.
- Rank-column button-id mapping unified through `GUILD_ASSIGNABLE_RANKS`/`GUILD_RANK_COLUMN_X` (shared arrays driving the header labels, the per-row radio buttons, and `ApplyMembershipRankChanges()`'s button-id-to-rank lookup) so the three can't drift out of sync with each other — previously each was hand-written independently.

Expected impact: the Membership gump displays correctly at 15 rows per page without header/button-text overlap, and shows more members per page than before.

### 3. Guild House Permissions (new feature)

Files involved:
- `pkg/opt/guilds/include/guildconstants.inc`, `pkg/opt/guilds/include/guilds.inc`
- `pkg/opt/guilds/commands/player/guilds.src`
- `pkg/multis/house/multiSign/method.src`, `pkg/multis/house/multiSign/use.src`

Notable functional changes:
- New per-guild-rank house permission model: four independent boolean categories per rank (`secure`, `teleporter`, `lockdown`, `house_manager`), stored under a new `HousePerms` property in the existing guild registry datafile (`GetGuildHousePerms()`/`SetGuildHousePerms()` in `guilds.inc`, mirroring the existing `MemberRanks` read/write pattern).
- New "Guild Management" button on `GuildMasterGump` opens `ShowGuildHouseManagementGump()` — one row per rank (Leader/Officer/Veteran/Member/Recruit) x 8 checkbox columns (View/Add/Remove secures -> `secure`; Recall/Gate To/Gate From -> `teleporter`; Lockdown; Manager), all driven off shared `GUILD_HOUSE_PERM_COLUMN_X`/`_LABEL`/`_KEY` arrays so the headers, checkboxes, and `ApplyGuildHousePermsChanges()`'s unpacking can't drift apart. The Guild Leader's row renders as static checkmark art (`GFGumpPic` with the checked-checkbox graphic, no button behind it) instead of real checkboxes, since the leader is always granted every permission in code and has nothing to configure.
- `:house:multiSign/method.src` gained `GuildRankHasHousePermission(sign, mobile, perm_key)`, which resolves the mobile's guild rank (leader-or-`MemberRanks`-lookup, default "Member") and checks it against the house's `HousePerms`, provided the house's `GuildHouse` property matches the mobile's own guild. It's read directly from the shared datafile (bare `use guilds;`/`use datafile;`, no `:guilds:` include) to avoid a `:house:` -> `:guilds:` package dependency, and short-circuits to grant=1 immediately for the Guild Leader rank without touching the datafile at all.
- `HasHousePermission()` restructured from `IsFullHouseManager OR (IsAFriend AND has-friend-perm)` (which returned 0 immediately for any non-friend) to also fall through to `GuildRankHasHousePermission()`, so a guild member with a rank-granted permission is recognized even when they aren't personally on the house's friend list.
- `Mulit_House_sign`'s `can_manage` (gates the House Management tool, friends/ban/eject management, and sign rename) widened to also grant access via `sign.HasHousePermission(who, "house_manager")`, evaluated last in the OR chain so the datafile is only touched once every cheaper check has already failed.
- `HandleLockdownRelease` (lock/release execution) previously had no permission check of its own at all beyond the outer `can_manage` gate; it now also checks `HasHousePermission(who, "lockdown")` directly, matching every other individual action (secure, teleporter) which already had its own check.

Expected impact: guild leaders can grant secure/teleporter/lockdown/house-manager access to specific ranks for the guild's own house, without needing to add every member to the friends list individually; friend-list and guild-rank grants combine (either is sufficient).

### 4. Guild Colour Cooldown Staff Bypass

Files involved: `pkg/opt/guilds/commands/player/guilds.src`

Notable functional changes: the 24-hour guild-colour change cooldown (`GUILD_COLOUR_TIME`) is bypassed for staff (`cmdlevel >= 3`).

Expected impact: staff can change a guild's colour without waiting out the normal 24-hour cooldown; regular players are unaffected.

### 5. Shared House-Sign Resolution Consolidation

Files involved:
- `pkg/multis/house/include/utility.inc`
- `pkg/opt/guilds/include/guilds.inc`

Notable functional changes:
- `:house:utility.inc`'s `GetHouseSignFromObject()` (self-is-sign / `signserial` cprop / nearby-scan-on-`houseserial` resolution) extended to recognize custom housing's `0xFFF4` sign objtype via a shared `HOUSE_SIGN_OBJTYPES` array, making it the single canonical sign resolver for `:house:`, `:staticHousing:`, and `:customhousing:` alike.
- `guilds.inc`'s own duplicate `ResolveHouseSign()`/`HOUSE_SIGN_OBJTYPES` were deleted; `IsStaticHouse()` now calls the shared `GetHouseSignFromObject()` instead.

Expected impact: no direct gameplay change — internal deduplication. Guild code now resolves custom-housing signs correctly wherever it previously only handled `:house:`/`:staticHousing:` signs (e.g. `IsInsideGuildHouse()`).

### 6. Custom Housing `house_serial` -> `houseserial`/`signserial` Migration

Files involved:
- `pkg/multis/customhousing/scripts/customhousedeed.src`
- `pkg/multis/customhousing/include/house.inc`
- `pkg/multis/customhousing/sign.src`, `pkg/multis/customhousing/signcontrol.src`
- `pkg/opt/omegacache/omegacache.inc`

Notable functional changes:
- New custom houses now write the sign/bracket/pole link using `:house:`'s own convention (`houseserial`/`signserial`, no underscore) instead of customhousing's legacy `house_serial`.
- New `ResolveCustomHouseSerial(sign)` (`include/house.inc`) reads `houseserial` first, falling back to the legacy `house_serial` with a self-healing backfill (writes `houseserial` once resolved, so old signs migrate transparently on next use). All internal `house_serial` reads across `house.inc`, `sign.src`, and `signcontrol.src` were repointed through this bridge.
- `omegacache.inc`'s `FindOmegaCacheHouseSign()` had its own independent, now-redundant `house_serial` fallback check for the same underlying gap — removed, since the sign-resolution fix above covers it upstream.

Expected impact: no direct gameplay change for existing houses (transparently migrated on next access); eliminates a 3-way duplicated/divergent sign-resolution implementation across `:guilds:`, `:house:utility`, and `omegacache.inc`.

### 7. Guild Master Gump — Server-Side Authorization Fix

Files involved: `pkg/opt/guilds/commands/player/guilds.src`

Notable functional changes: `GuildMasterGump(who)` previously had no server-side check that `who` was actually the guild master — the calling gump (`GuildGump`) only conditionally *rendered* the button that reaches it (`if(who.guild.getprop("GuildMaster") == who.serial)`), which is a client-side choice, not enforcement; any gump-response packet naming that button id would reach full guild-master authority (disband, guildstone removal, change guild house/master, and the new house-permissions gump) regardless of actual rank. `GuildMasterGump()` now checks `who.guild` and `GuildMaster` itself before doing anything, redirecting to the regular `GuildGump()` with a message otherwise.

Expected impact: closes an access-control gap that predates this patch across the whole guild package (not introduced by this patch's other guild work, but caught while reviewing it) — guild-master-only actions can no longer be reached by a non-master sending a crafted gump response.

### 8. Static Housing — Lockdown/Secure Permission Gate Fix

Files involved: `pkg/multis/staticHousing/lockunlock.src`

Notable functional changes: `lockunlockitem`'s permission gate previously checked only `house_sign.IsAFriend(character)` (falling through to a `character.cmdlevel` override) before allowing lockdown/release/secure/display/raise/lower actions — the house *owner* is never added to their own house's friend list (it's actively erased on purchase), so a plain owner failed this check and was silently blocked (no message) from every one of these actions in their own house, even though the calling code (`decorate.src`, `sign/use.src`'s `HandleHouseManagementAction`) had already granted them access via `HasHousePermission()`. The gate now calls `HasHousePermission(character, required_perm)` (`"secure"` for secure/unsecure actions, `"lockdown"` for everything else), matching what the callers already check.

Expected impact: static house owners (and co-owners not separately friended) can now actually use the decorator tool and House Management actions in their own house.

### 9. Custom Housing — Teardown Item-Release Fix

Files involved: `pkg/multis/customhousing/include/house.inc`

Notable functional changes: `ReleaseAll(owner, sign)` — run on every full house teardown (manual redeed, relinquish, decay auto-demolish) — previously routed each locked-down/secured/displayed item through `run_script_to_completion(":housing:lockunlock", parms)`. No package named `:housing:` exists anywhere in this repo (registered package names are `house`/`customhousing`/`statichousing`; the one `lockunlock.src` that exists lives under `:statichousing:`), so this call always failed to resolve and no item was ever actually released before the house was destroyed immediately afterward. Rewritten to release items inline (mirroring `:house:`'s own `demolish()` in `multiSign/use.src` — clear the marking CProp, restore `movable`/`usescript` directly, no external script), per the user's explicit direction to match `:house:`'s teardown pattern rather than redirecting the broken reference to `:statichousing:`.

Expected impact: items that were locked down, secured, or on display in a custom house are now correctly released (made movable again, secure containers' original script restored) when the house is redeeded, relinquished, or decays — previously they were left in a broken immovable/orphaned state.

### 10. Custom Housing — `ChangeOwner` Stale Multi Ownerserial Fix

Files involved: `pkg/multis/customhousing/include/house.inc`

Notable functional changes: `ChangeOwner(who, sign)` previously only updated the house multi's own `ownerserial` property in the branch where `homeinfo` was *absent* — in the normal case (`homeinfo` present), only the sign's `ownerserial`/`owneracct` were updated, leaving the multi's own copy pointing at the previous owner. `guilds.inc`'s `IsGuildHouseOwner()`/`GetOwnedGuildCandidateHouses()` read `ownerserial`/`owneracct` directly off the multi (not resolved via the sign), so a custom house transferred through `ChangeOwner` couldn't be selected as a guild house by its new owner. Now always mirrors both properties onto the multi regardless of `homeinfo`.

Expected impact: a customhousing house that changed hands via the in-game ownership-transfer option can now be selected as a guild house by its current owner.

### 11. Omega Cache DataFile Cleanup on House Teardown

Files involved:
- `scripts/include/omegacache_utils.inc`
- `pkg/multis/house/multiSign/use.src`, `pkg/multis/house/multiSign/control.src`
- `pkg/multis/customhousing/sign.src`, `pkg/multis/customhousing/include/house.inc`
- `pkg/multis/staticHousing/sign/destroy.src`, `pkg/multis/staticHousing/sign/use.src`

Notable functional changes:
- New `DeleteOmegaCacheStore(house_serial)` helper. Originally intended to fully delete the per-house Omega Cache DataFile via `DeleteDataFile()` when a house is torn down for good, so the file/registry entry didn't sit around forever for houses that no longer exist — however, `DeleteDataFile` turned out not to be available in this engine build at all (it's commented out in `datafile.em`, unlike `CreateDataFile`/`OpenDataFile`/`UnloadDataFile` which are real), confirmed via a failed compile. The function now just unloads the DataFile (same effect as the existing `CloseOmegaCacheStore`), kept under its own name so the teardown call sites still read as "this house is gone for good" and so a real delete can be dropped in later if the engine ever adds one.
- Wired into every whole-house teardown path that wasn't already unloading the cache: `:house:`'s manual redeed (already had a `CloseOmegaCacheStore` call, upgraded) and decay auto-demolish (`multiSign/control.src`, previously didn't touch Omega Cache at all — added with an `IsOmegaCacheEmpty()` guard so an abandoned house's *unclaimed* cache contents aren't silently destroyed on top of the house); customhousing's manual redeed and decay auto-demolish (`DemolishAbandonedCustomHouse`); staticHousing's decay destruction and `RelinquishHouse`.
- Deliberately *not* wired into the three single-container "remove one Omega Cache" functions (`:house:`'s `HouseFunctionRemoveOmegaCache`, staticHousing's `StaticHouseFunctionRemoveOmegaCache`, customhousing's `CustomHouseFunctionRemoveOmegaCache`) — a house's cache containers (up to 3, see `GetMaxOmegaCacheForHouse`) all share one DataFile keyed by house serial, so unloading there could affect a sibling container still in use.

Expected impact: no gameplay-visible change today (deletion isn't possible), but the in-memory handle for a torn-down house's cache is now released at the same point across all three housing packages consistently, where before only `:house:`'s manual redeed path did so at all.

### 12. Custom Housing — `0x17060` Legacy Branch Documented as Likely Dead

Files involved: `pkg/multis/customhousing/sign.src`

Notable functional changes: documentation-only. Added a comment at `use_house_sign`'s `sign.objtype == 0x17060` dispatch (which routes to `DisownHouse`, a legacy "static" resale path predating the current deed-placed flow) noting that customhousing's only deed script (`customhousedeed.src`'s `usehousedeed`) never creates a `0x17060` sign for either of its two deed types — both always create `0xFFF4` signs. No code removed, since dead-code status wasn't confirmed with certainty (a `0x17060` sign could exist if one was hand-spawned by staff at some point).

Expected impact: none (no code changed, comment only).

### 13. Banker & High Priest — Per-Currency Balance and Help Gumps

Files involved: `scripts/ai/banker.src`, `scripts/ai/bankerbalancegump.src`, `scripts/ai/highpriest.src`, `config/command_synopses.cfg`

Notable functional changes:
- Bankers now understand `"balance gold"`/`"balance silver"`/`"balance copper"` (and the `"vault balance <currency>"` equivalents), answering out loud with the combined coin+cheque total for just that currency via new `SpeakCurrencyBalance()`, without opening the balance gump.
- `bankerbalancegump.src` reworked to also show a combined coin+cheque "Total" column per currency (`gold_total`/`silver_total`/`copper_total`, computed in `ShowStorageBalance()`), widened to fit the new column.
- Both bankers and high priests now respond to `"help"` with a `ShowHelp()` tip-window listing every speech command they understand and what it does.

Expected impact: players can ask for a single currency's balance without opening a gump, see combined totals in the balance gump, and get an in-character list of banker/high-priest speech commands on request.

### 14. `move`/`movebag`/`movebankcoin` — Stack Consolidation Rewrite

Files involved: `scripts/textcmd/player/move.src`, `scripts/textcmd/player/movebag.src` (new), `scripts/textcmd/player/movebankcoin.src` (new), `config/command_synopses.cfg`, `pkg/utils/mdgumps/include/yesNo.inc`

Notable functional changes:
- `.move`'s "move all" flow previously scanned `who.backpack` unconditionally (regardless of which container the targeted item actually came from) and moved each matching item as its own separate stack. Rewritten to scan the item's actual source container and consolidate matches through a single "carrier" stack per destination via new `MoveConsolidated()`/`FindOpenStack()`, topping each stack off to a 60,000 cap before starting a new one, instead of leaving many small stacks behind.
- The custom hand-rolled confirmation gump was replaced with the shared `YesNo()` prompt.
- Two new player commands sharing the same consolidation logic: `.movebag` (empty one backpack-contained container into another) and `.movebankcoin` (move gold/silver/copper between containers in your bank box or vault).
- `:mdgumps:yesNo.inc`'s `YesNo()` changed from `Start_Script` to `Run_Script` when launching the underlying prompt script, so the caller reliably gets the player's actual response back rather than a result from an async launch.

Expected impact: `.move all`, `.movebag`, and the new `.movebankcoin` now leave fewer, more fully-stacked piles behind instead of one stack per moved item.

### 15. MegaCliloc Tooltip — Vendor Storage Bag Item Naming

Files involved: `pkg/packethooks/megacliloc/toolTips.src`

Notable functional changes: item tooltips inside a Vendor Storage Bag (objtype `0x1966A`) now use the item's singular description (`FormatItemDescription(xObject.name, 1, xObject.name_suffix)`) instead of the plural description used everywhere else.

Expected impact: items shown inside a Vendor Storage Bag display a grammatically correct singular name/count instead of always reading as a plural.

---

## Validation Notes

- Diff range: `git diff 541e883` (working tree, uncommitted changes included) for the full Patch-3.0.7 scope; `git log --oneline 541e883..HEAD` and `git show <hash>` for per-commit detail on `edc8a64`/`a642726`; `git diff --numstat` (working tree only) to isolate this session's own uncommitted contribution (Themes 3, 5-12 above) from the two already-committed commits (Themes 1-2, 4, 13-15).
- Working tree state: **not clean** — 11 modified files remain uncommitted as of this writing (`pkg/multis/customhousing/include/house.inc`, `pkg/multis/customhousing/sign.src`, `pkg/multis/house/multiSign/control.src`, `pkg/multis/house/multiSign/method.src`, `pkg/multis/house/multiSign/use.src`, `pkg/multis/staticHousing/lockunlock.src`, `pkg/multis/staticHousing/sign/destroy.src`, `pkg/multis/staticHousing/sign/use.src`, `pkg/opt/guilds/commands/player/guilds.src`, `pkg/opt/guilds/include/guildconstants.inc`, `scripts/include/omegacache_utils.inc`). This changelog covers them as part of the patch's scope on the assumption they'll be committed before release; the file inventory and scope numbers above already include them.
- Compile validation: `scripts/ecompile.exe -b -r -u` (compile all updated scripts) run twice. First run surfaced one real error — `Unknown identifier 'DeleteDataFile'` in `scripts/include/omegacache_utils.inc`, affecting every script that includes it (6 scripts failed) — fixed per Theme 11 above. Second run: 0 errors across all 26 recompiled scripts. Remaining warnings (unused local variables in `pkg/utils/mdgumps/include/gumpPrompt.inc`, `pkg/opt/omegacache/cacheinsert.src`, `pkg/opt/omegacache/omegacache.src`, `scripts/include/resourcemanager.inc`, `scripts/include/classes.inc`, `pkg/std/cartography/cartography.src`, `pkg/multis/house/multiSign/use.src:127`) are pre-existing and unrelated to this patch's changes.
