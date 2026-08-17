# Developer Changelog - v3.0.7

Range: Patch-3.0.6..Patch-3.0.7 (commit `541e883`..`60949cb`)
Branch: Patch-3.0.7
Date: 2026-08-16

---

## Scope Summary

- Total files changed: 131
- Status breakdown: 105 modified, 25 added, 1 renamed, 0 deleted
- Net textual delta: 19747 insertions, 3641 deletions
- Largest shifts:
  - `pkg/std/fishing/itemdesc.cfg` (+4331 / -~450) - fish catalog moved onto a dedicated objtype block and rebuilt for the tiered Regular/Rare/Legendary catch system, plus new crustacean/trap/buoy item entries
  - `ainotes/Hue names list.txt` (+3000, new file) - reference doc backing the hue-correction sweep
  - `pkg/opt/guilds/config/guildstonegraphics.cfg` (+1598 net across the full range) - guildstone catalog, initially added then extended
  - `ainotes/Hues Audit.md` (+1544, new file) - per-hue-swap audit trail (methodology, every confirmed live location, revert reference)
  - `regions/wood.cfg` (+844 net) - realm-scoped resource region blocks (see Theme 19); `regions/ore.cfg` (+468), `regions/sand.cfg` (+448), `regions/clay.cfg` (+619), `regions/fish.cfg` (+316) follow the same pattern
  - `pkg/items/containers/config/itemdesc.cfg` (+526 net) - 18 new treasure-chest container variants
  - `config/npcdesc.cfg` (538 changed lines) - hue corrections + mount/ethereal death-sound fixes
  - `pkg/items/doors/config/itemdesc.cfg` (+410, new file) - 24 of 182 audited missing door registrations
  - `pkg/std/fishing/fishing.inc` (+328 net) / `pkg/std/fishing/fishing.src` (+315 net) - core fishing rewrite
  - `pkg/std/fishing/crustaceantrap.inc` (+194, new) / `crustaceantable.cfg` (+177, new) / `crustaceansweeper.src` (+158, new) - new crustacean trapping subsystem
  - `pkg/opt/guilds/commands/player/guilds.src` (+545 net across the range) - Membership gump rework, new Guild House Management gump, guildstone workflow, GuildMasterGump authorization fix
  - `scripts/textcmd/player/move.src` (+75 / -43) - stack-consolidation rewrite
- Non-merge commits in range (oldest to newest):
  - `edc8a64` Initial Guild Updates: guild stones, guild gump updates, customhouse/housing sign fixes for guilds, banker balance fixes, banker/high priest Help gumps, move/movebag/movebankcoin fixes
  - `a642726` Wired guilds into housing permissions
  - `165ac24` Latest core changes
  - `3f7a10c` Patchnotes: Guild housing updates
  - `68000b7` New create itemdesc command
  - `9c6c080` Tons of Hue fixes / Guildstone removed on house destroy
  - `00c5358` Hue fix
  - `4e80db9` Treasure Map updates
  - `9d6d0bc` Powerscroll animation fix
  - `dc2885f` Update chests for dig treasure
  - `efae7d2` Snooping/stealing stacking fix / New containers added / Dig treasure updates
  - `3acf1af` Doors fixes / Fishing update / resources fix for realms
  - `e09d049` Fishing changes / Crabs/lobsters / Boat key fix
  - `3ffee6c` Fishing Updates
  - `60949cb` Crustacean Fishing
- Themes 1-15 below (guild stones, membership gump, house permissions, colour cooldown, sign consolidation, custom housing migration, GuildMasterGump auth fix, static housing lockdown fix, custom housing teardown fix, ChangeOwner fix, Omega Cache cleanup, dead-code note, banker/high priest speech, move/movebag/movebankcoin, tooltip fix) cover `541e883..a642726` and were written up when that slice shipped; Themes 16+ cover everything added afterward, up to `60949cb`.

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- A | ainotes/Hue names list.txt
- A | ainotes/Hues Audit.md
- A | ainotes/missing-doors-audit-20260815.md
- M | config/command_synopses.cfg
- M | config/equip.cfg
- M | config/itemdesc.cfg
- M | config/mrcspawn.cfg
- M | config/npcdesc.cfg
- M | core-changes.txt
- A | patchnotes/developer-changelog-v3.0.7.md
- M | patchnotes/developer-changelog.md
- M | patchnotes/launchernotes.md
- A | patchnotes/patch-v3.0.7.md
- M | pkg/items/containers/config/itemdesc.cfg
- M | pkg/items/currency/config/itemdesc.cfg
- M | pkg/items/deed/config/itemdesc.cfg
- M | pkg/items/doors/config/itemdesc.cfg
- M | pkg/multis/boat/multi/listener.src
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
- M | pkg/opt/GMItems/bowofshadows_usescript.src
- M | pkg/opt/GMItems/itemdesc.cfg
- M | pkg/opt/alchemyplus/alchemyplus.cfg
- M | pkg/opt/alchemyplus/itemdesc.cfg
- M | pkg/opt/alryc/textcmd/test/animatedgraphics.src
- A | pkg/opt/alryc/textcmd/test/createguildstone.src
- A | pkg/opt/alryc/textcmd/test/createitemdesc.src
- A | pkg/opt/alryc/textcmd/test/guildstonelist.src
- M | pkg/opt/astralfights/itemdesc.cfg
- M | pkg/opt/botanik/itemdesc.cfg
- M | pkg/opt/champspawns/include/rewards.inc
- M | pkg/opt/champspawns/scripts/oncreate.src
- M | pkg/opt/christmas/Christmasgifts.src
- M | pkg/opt/crafterboost/itemdesc.cfg
- M | pkg/opt/earth/itemdesc.cfg
- M | pkg/opt/farming/itemdesc.cfg
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
- M | pkg/opt/loot/itemdesc.cfg
- M | pkg/opt/lootlottery/itemdesc.cfg
- M | pkg/opt/omegacache/categories.cfg
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/powerscrolls/powerscroll.src
- R | pkg/opt/powerscrolls/textcmd/admin/raisecaps.src -> pkg/opt/powerscrolls/textcmd/test/raisecaps.src
- M | pkg/opt/rituals/config/itemdesc.cfg
- M | pkg/opt/shilitems/itemdesc.cfg
- M | pkg/opt/shilitems/trashcanofwonders.src
- M | pkg/opt/shrink/itemdesc.cfg
- M | pkg/opt/songbook/itemdesc.cfg
- M | pkg/opt/townstones/itemdesc.cfg
- M | pkg/opt/vanityshop/customitemdye.src
- M | pkg/opt/versebook/itemdesc.cfg
- M | pkg/opt/zuluitems/itemdesc.cfg
- M | pkg/opt/zuluitems/use_racegate.src
- M | pkg/packethooks/megacliloc/itemdata.src
- M | pkg/packethooks/megacliloc/toolTips.src
- M | pkg/std/cooking/itemdesc.cfg
- A | pkg/std/fishing/catchtable.cfg
- A | pkg/std/fishing/crustaceansweeper.src
- A | pkg/std/fishing/crustaceantable.cfg
- A | pkg/std/fishing/crustaceantrap.inc
- A | pkg/std/fishing/crustaceantrap.src
- A | pkg/std/fishing/crustaceantrapbuoy.src
- M | pkg/std/fishing/fishing.inc
- M | pkg/std/fishing/fishing.src
- M | pkg/std/fishing/fishingnet.src
- M | pkg/std/fishing/itemdesc.cfg
- M | pkg/std/fishing/sosarea.cfg
- M | pkg/std/fishing/sosbottle.src
- M | pkg/std/lumberjacking/itemdesc.cfg
- M | pkg/std/mining/itemdesc.cfg
- M | pkg/std/snooping/itemdesc.cfg
- M | pkg/std/snooping/snooping.src
- M | pkg/std/snooping/stealing.src
- M | pkg/std/stealing/stealing.src
- M | pkg/std/tailoring/itemdesc.cfg
- M | pkg/std/treasuremap/digtreasure.src
- A | pkg/std/treasuremap/textcmd/admin/gototreasuremap.src
- M | pkg/std/treasuremap/treasure.cfg
- M | pkg/systems/combat/config/itemdesc.cfg
- M | pkg/systems/combat/config/modenchantdesc.cfg
- M | pkg/utils/itemUtils/config/sets.cfg
- M | pkg/utils/mdgumps/include/yesNo.inc
- M | pol.exe (binary rebuild)
- M | poltool.exe (binary rebuild)
- M | regions/clay.cfg
- M | regions/fish.cfg
- M | regions/ore.cfg
- M | regions/regions.cfg
- M | regions/sand.cfg
- M | regions/wood.cfg
- M | scripts/ai/banker.src
- M | scripts/ai/bankerbalancegump.src
- M | scripts/ai/highpriest.src
- M | scripts/ai/merchant.src
- M | scripts/control/skilladvancerequip.src
- M | scripts/ecompile.exe (binary rebuild)
- M | scripts/include/npccastspells.inc
- M | scripts/include/omegacache_utils.inc
- M | scripts/items/bladed.src
- M | scripts/items/pvp.src
- M | scripts/items/pvp2vs2.src
- M | scripts/misc/death.src
- M | scripts/misc/dressme.src
- M | scripts/modules/polsys.em
- M | scripts/runecl.exe (binary rebuild)
- M | scripts/start.src
- M | scripts/textcmd/player/move.src
- A | scripts/textcmd/player/movebag.src
- A | scripts/textcmd/player/movebankcoin.src
- A | scripts/textcmd/player/togglerarecarve.src
- M | uoconvert.exe (binary rebuild)
- M | uotool.exe (binary rebuild)

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

### 16. Fishing — Tiered Catch System Rewrite

Files involved: `pkg/std/fishing/fishing.inc`, `pkg/std/fishing/fishing.src`, `pkg/std/fishing/fishingnet.src`, `pkg/std/fishing/sosbottle.src`, `pkg/std/fishing/sosarea.cfg`, `pkg/std/fishing/catchtable.cfg` (new), `pkg/std/fishing/itemdesc.cfg`, `pkg/packethooks/megacliloc/itemdata.src`, `scripts/textcmd/player/togglerarecarve.src` (new), `config/command_synopses.cfg`, `regions/regions.cfg`

Notable functional changes:
- Every pole/net catch now rolls a **Regular / Rare / Legendary** tier (pole: 1/500 Rare, 1/5000 Legendary; net: 5x better at 1/100 / 1/1000). If no entry in the new `catchtable.cfg` matches the roll against the caster's current Location (Deep Water / Shores / Dungeons), Realm, and skill (`MinSkill` gates Rare/Legendary), the catch falls back a tier rather than being wasted. `catchtable.cfg` catalogs 69 species (42 Regular / 13 Rare / 14 Legendary).
- All fish objtypes relocated onto a dedicated `0x30300`+ block, replacing the old `0x09cc`-`0x0dd9` range that overlapped unrelated items; SOS bottle/message/tile objtypes similarly renumbered off legacy `0xa360`-range IDs onto `0x9600`-`0x9602` for the same reason. `pkg/opt/omegacache/categories.cfg`'s magic-fish icon entries were updated to match.
- New `IsDeepWater()` replaces the old fixed/no-op check: flood-fills a configurable radius (default 16 tiles) of water tiles and rejects if `IsNearDockPlanks()` finds dock planking nearby; recomputed on every cast so a boat-anchored caster's water classification can change without the caster moving.
- New `IsFishingInDungeon()` reads `GetRegionNameAtLocation()` and checks the new `Type World` marker added to the Sosaria/Ilshenar/Malas/Tokuno/Ter Mur top-level realm regions in `regions.cfg` (see Theme 19) — a region lacking `Type World` is treated as a dungeon.
- Skill-gain multiplier: 0.5x in safe/guarded areas, 2x in dungeons, 1.5x in deep water, 1x on shores.
- The old `GetHarvestDifficulty`-based "nothing here" gate (which produced false aborts) was removed in favor of trusting `HarvestResource()`'s actual return, matching the existing mining/lumberjacking pattern; a genuinely depleted pool now returns a clear "fished these waters dry" message instead of a confusing silent failure.
- Rare/Legendary catches are hued (yellow 1731 / orange 1531) in the sysmessage and now also in the item's tooltip via a new `FishTier` MegaCliloc line in `itemdata.src`.
- New player command **`.togglerarecarve`**: a blade equipped while fishing previously auto-carved *any* catch, including rares, into fish steaks; Rare/Legendary catches are now protected from auto-carve unless the player opts in via this toggle. Shallow-water catches also now yield 1 steak when carved vs. 4 for deep-water catches.
- Dead/unreachable SOS-bottle spawn code (`FetchBottle`, `CreateChest`, `FindSpot`, `CleanSos`) confirmed unreachable (SOS bottles are never spawned in this build) and commented out with an explanatory note rather than left silently dead.
- `fishingnet.src`: removed an obsolete `CheckCity`-based "deep sea only" gate, replaced with the real `IsDeepWater()` check; net catches now route through the same tiered `ResolveFishCatch()` as the pole.

Expected impact: fishing/net catches now surface visibly distinct Rare/Legendary tiers (name, color, tooltip); depletion messaging is accurate instead of misleading; a new opt-in toggle protects valuable catches from accidental carving; shallow-water catches yield less than deep-water ones.

### 17. Crustacean Trapping (New Feature)

Files involved: `pkg/std/fishing/crustaceantrap.inc` (new), `crustaceantrap.src` (new), `crustaceantrapbuoy.src` (new), `crustaceansweeper.src` (new), `crustaceantable.cfg` (new), `scripts/start.src`

Notable functional changes:
- New placeable item, the crustacean trap (objtype `0x3037E`): placed into water within 10 tiles, LOS-checked, not from inside a house, not within 2 tiles of another trap. Placing it spawns a buoy (`0x3037F`) — the interactive world object the owner later retrieves.
- New background sweeper (`crustaceansweeper.src`, modeled on the existing donation-box sweeper pattern) ticks every 5 seconds: a buoy is destroyed if its owner logs off/disconnects or wanders past the 10-tile leash, or after 15 minutes unretrieved ("washed away"). Every 60 seconds a trap "bobs" — an escalating 5%-per-bob loss chance, and on a surviving bob a 35% chance to add one crustacean catch (max 5 per trap, matching OSI behavior).
- Crustacean catches use the same Regular/Rare/Legendary roll structure as fish, via a parallel `crustaceantable.cfg` (12 Regular, 3 Rare, 5 Legendary crab/lobster species) — reachable only through the trap, not the pole or net.
- Max 5 simultaneously active traps per player. Retrieving (double-click the buoy, owner-only, must be within leash range) returns the physical trap item plus all accumulated catches with rare/legendary sysmessages, then tears down the buoy.
- `scripts/start.src` wires the sweeper via `start_script(":fishing:crustaceansweeper")` on every server boot, and force-scans all 5 realms for stray orphaned buoys (registry state isn't trusted to have survived an unclean shutdown); also force-clears the sweeper's persistent single-instance guard property so a real restart doesn't permanently block it from ever starting again.

Expected impact: a new crab/lobster trapping minigame independent of pole/net fishing, with its own Rare/Legendary tier structure and a passive "check back later" placement/retrieval loop.

### 18. Boat Key Destruction Fix

Files involved: `pkg/multis/boat/multi/listener.src`

Notable functional changes: `DestroyBoatKey()` previously only searched the boat owner's backpack root items and keyrings for a matching key when dry-docking, silently failing (logged an error, left the key undestroyed) if the key was anywhere else — bank box, secure container, etc. Replaced with `KP_DestroyOwnedKeysForLockIDs()` from the shared `:keys:key` include, which searches more broadly across the owner's held containers for every key matching the boat hold's `LockID`.

Expected impact: dry-docking a boat now reliably destroys its keys regardless of where the owner is storing them.

### 19. Resource Realm-Bleed Fix

Files involved: `regions/fish.cfg`, `regions/ore.cfg`, `regions/wood.cfg`, `regions/sand.cfg`, `regions/clay.cfg`, `regions/regions.cfg`

Notable functional changes: each resource type previously had a single `Region "The Whole World"` block with no `Realm` field, so the engine's depletion pool (tied to the Region object, not `(x,y,realm)`) was shared across every realm using the same raw coordinates — e.g. Tokuno's `0-1447,0-1447` box aliased directly onto Britannia's heavily-fished NW corner. Fixed by adding one additional realm-scoped Region block per resource type, each with an explicit `Realm` field and correct facet bounds: Sosaria (`Realm britannia_alt`, `0 0 7167 4095`), Ilshenar (`Realm ilshenar`, `0 0 2303 1599`), Malas (`Realm malas`, `0 0 2559 2047`), Tokuno (`Realm tokuno`, `0 0 1447 1447`), Ter Mur (`Realm termur`, `0 0 1279 4095`); the original "Whole World" block also gained an explicit `Realm britannia`. `fish.cfg` additionally dropped landtiles `76-111`/`0x01AA`-`0x01AB` (verified via `tiledata.mul` as not actually water-flagged) and added `0x00AA`-`0x00AB` (the missing other half of a 4-tile water texture set). `regions.cfg` also tags `Type World` onto the Sosaria/Ilshenar/Malas/Tokuno/Ter Mur top-level realm regions (consumed by the new `IsFishingInDungeon()`, Theme 16) and fixes a duplicate/conflicting `MIDI` line on Ilshenar and Malas.

Expected impact: fishing/mining/lumberjacking/sand-mining/clay resource pools in Tokuno, Malas, Ilshenar, and Ter Mur no longer silently share depletion state with Felucca/Trammel at the same raw coordinates — each realm now regrows and depletes independently.

### 20. Doors — Partial Fix for Missing Door Registrations

Files involved: `pkg/items/doors/config/itemdesc.cfg` (new), `ainotes/missing-doors-audit-20260815.md` (new)

Notable functional changes: the audit doc catalogs 182 door-flagged tiledata tiles with zero `itemdesc.cfg` registration (no `Door {}` block ⇒ engine never classifies the objtype as `POLCLASS_DOOR` ⇒ inert in-game, per prior investigation). This commit registers 24 of the 182: Bar Door (`0x190E`-`0x190F`), Moon Door Alt Set (`0x319C`-`0x319F`), Crystal wall (`0x35E7`-`0x35E8`), Shadow door (`0x3640`-`0x3643`, `0x3645`-`0x3646`), QC Wall b (`0x5128`-`0x5129`), and Wallset3 South/East doors (`0x409B`-`0x40A2`, flagged `UNVERIFIED` in-file — pattern-matched from the confirmed Wallset1 behavior, not independently checked in UOFiddler).

**Gap:** the headline bug that motivated the audit — Gargish Grey Door / Wallset1 family (`0x41CF`-`0x41D6`) — is still not fixed, nor are GargoyleDoor, Wallset2 Sun Door, Gargish Set Door (A1a-D2b), RuinDoor, QueenDoor/QueenDoorH, Gargish Red Door, Door South01, castle-era metal doors (32 tiles), or castle-era wooden gates (13 tiles) — roughly 158 of the original 182 remain unregistered.

Expected impact: a handful of newer-era decorative doors (bar doors, alt moon doors, crystal-wall doors, shadow doors, two stone wall-door variants) now open/close correctly. The majority of the previously-identified broken catalog — including the specific multis flagged in the prior audit (Gothic Rose Castle, Castle of Oceania, Sandalwood Keep, Keep Incarcerated, Sally Trees Refurbished Keep, Clovers Keep, Terrace Gardens) — remains affected. This is a partial pass, not the full fix.

### 21. Treasure Map / Dig Treasure

Files involved: `pkg/std/treasuremap/digtreasure.src`, `pkg/std/treasuremap/treasure.cfg`, `pkg/std/treasuremap/textcmd/admin/gototreasuremap.src` (new), `pkg/items/containers/config/itemdesc.cfg`, `config/command_synopses.cfg`, `config/mrcspawn.cfg`

Notable functional changes:
- Treasure chests previously always spawned the single graphic `0x0E40` regardless of map level. `GetTreasureChestObjtype(lvl)` now picks randomly from a level-appropriate pool of 18 new dedicated Container objtypes (`0xB4A6`-`0xB4B9`): Small (levels 1-3, 8 variants: Basic/Adorned/Menacing x South/East), Medium (levels 4-5, 6 variants), Large (level 6+, 6 variants). Chest identification for cleanup switched from `objtype == UOBJECT_TREASURE_CHEST` to a `treasurechest` CProp check since the objtype is now variable.
- New "bardic intuition" mechanic: on a level-6 dig, a Bard-class digger has a `bard_level`%-scaled chance to upgrade the dig to an effective level 7 — spawns 3 guardians from the level-6 guardian table and prints "Your bardic intuition senses something extraordinary here!" Non-bard/non-triggered rolls keep the existing level 1-3 / level 4+ escalation behavior; guardian-spawn branching was refactored (variable hoisting, cleaner branches) without changing that behavior.
- `dig_treasure`'s unused `shovel` parameter dropped.
- `treasure.cfg`: dig sites relocated off the unused `"britannia"` realm onto the shard's actual playable realms — 15 sites moved to `britannia_alt` (Sosaria), plus new sites added on `ilshenar` (6), `tokuno` (6), `malas` (3).
- New GM/admin tool **`.gototreasuremap`**: paginated gump listing every configured dig location (realm, X/Y/Z) with one-click teleport, for spot-checking dig sites.
- `mrcspawn.cfg`: removed dead `fish1`-`fish8` entries from the merchant "Fish" restock group (objtypes no longer exist post-fishing-rewrite) and added `Diggingtool` to the "MapItems" restock group.

Expected impact: treasure chests now look distinct (and progressively fancier) by map difficulty instead of one generic graphic; Bard-class characters get a rare bonus-tier chance on level-6 maps; treasure maps spawn their dig sites in the realms players can actually reach (Sosaria/Ilshenar/Tokuno/Malas) instead of the dead Britannia facet.

### 22. Snooping/Stealing — Stack Consolidation

Files involved: `pkg/std/stealing/stealing.src` (live script, bound via `attributes.cfg`), `pkg/std/snooping/stealing.src` (confirmed dead/unused, patched for consistency and commented as such), `pkg/std/snooping/snooping.src`

Notable functional changes: same root-cause class as the prior Gold Stack Overflow fix — stolen items were always placed into the thief's backpack via a plain `MoveItemToContainer`, which never merges into a matching existing stack, so every successful steal of a stackable item created a brand-new stack with no protection against the 60,000 cap. New `StealIntoContainer()`/`FindOpenStack()` helpers (added to both `stealing.src` files) find a compatible existing stack via `CanStack()`, top it up to `STACK_CAP := 60000`, and only spill the remainder into a new stack if it doesn't fully fit. Also removed a leftover, no-longer-referenced `"dodgy"` CProp that `snoop()` was setting/copying.

Expected impact: stealing a stackable item (reagents, ingots, arrows, etc.) now merges into an existing matching stack in the thief's pack instead of always creating a separate one, with correct 60,000-cap overflow handling.

### 23. Hue Corrections (Broad Sweep)

Files involved: ~35 `itemdesc.cfg`/`npcdesc.cfg`/`equip.cfg`/`sets.cfg`/`modenchantdesc.cfg` files across `config/`, `pkg/opt/*`, `pkg/std/*`, `pkg/systems/combat/*`; hardcoded-color references in `scripts/control/skilladvancerequip.src`, `scripts/items/bladed.src`, `scripts/misc/death.src`, `scripts/items/pvp.src`, `scripts/items/pvp2vs2.src`, `scripts/include/npccastspells.inc`, `pkg/opt/champspawns/{include/rewards.inc,scripts/oncreate.src}`; new reference docs `ainotes/Hue names list.txt` and `ainotes/Hues Audit.md`

Notable functional changes: a systematic, audited hue-value migration, not scattered spot-fixes. `Hues Audit.md` documents one section per swap with search methodology (full-repo regex scan for the old numeric hue, every hit classified as real color-use vs. false positive like a coordinate/objtype/array-index collision), every confirmed live location, and a revert reference. Confirmed swaps in this range: `1765`→`2669` ("New Zulu" signature color), `1155`-`1166`→`2730`-`2745` (sequential remap covering fire/lava/elemental colors), `1170`→`2243`, plus scattered hex-form individual hues (`0x0492`→`0x8c3`, `0x0494`→`0xabe`, `0x0485`→`0xaac`, `0x048b`→`0x0ab6`, `0x0486`→`0xaad`, and others). These land across NPC body colors, armor/weapon dyes, GM-item enchant colors, PvP arena fences/stones, champion-altar pieces, elemental spell-cast effect colors, quest stones, and book items — every hardcoded use of the old (apparently reused/conflicting) hue IDs was moved onto the new dedicated range. `config/npcdesc.cfg` separately picked up correct, distinct `deathsnd` values for several mount templates (`mountbeetle`, `mountkirin`, `mountraptalon`, `mountwolf`, `mounthairiyo`) and ethereal creatures (horse, llama, ostard, kirin) and `runebeetle` that previously shared a generic placeholder sound or had none. `pkg/opt/vanityshop/customitemdye.src` now also checks a new `Undyable` CProp and refuses to dye a tagged item — this is what protects the new fishing gear (poles, nets, hooks, buoys, traps, all tagged `Undyable` elsewhere in this patch) from being recolored via the dye tub. `config/itemdesc.cfg` also moved `PvPStone` off objtype `0xa392` onto `0xa394` (freeing the old slot as part of the fishing/crustacean/SOS objtype renumbering) and fixed several commented-out `DecayOnMultis`→`DecaysOnMultis` typos in disabled reference blocks (dead code, no live effect).

Expected impact: a broad, mostly invisible-unless-you-knew-it-was-wrong visual correction — many items/effects/NPCs render in a different (intended) color than before; several mounts get correct death sounds instead of a generic placeholder; new fishing gear can't be re-dyed.

### 24. Guildstone Cleanup on Custom House Redeed

Files involved: `pkg/multis/customhousing/sign.src`

Notable functional changes: redeeding a custom house via its sign never swept for a guildstone inside it, so a guildstone (and any statue-set companion pieces) left in the house orphaned in place once `DestroyMulti` ran, with the owning guild's stone reference left dangling. Fixed by collecting any `GUILDSTONE_OBJTYPE` item found in the same box-sweep the sign already runs for Omega Cache containers, calling the existing `DestroyGuildstoneCompanions()` cleanup, clearing the `GUILD_STONE` property on the owning guild if still resolvable, then destroying the stone.

**Scope caveat:** this fix only touches `pkg/multis/customhousing/sign.src`; the classic house sign path (`pkg/multis/house/multiSign/method.src`) and static housing (`pkg/multis/staticHousing/sign/*`) have no equivalent guildstone-cleanup code — a guildstone left in a classic (non-custom) house that gets redeeded would still orphan.

Expected impact: a guildstone in a custom house is now correctly removed when that house is redeeded; the same is not yet true for classic/static houses.

### 25. Powerscroll Read Animation Fix

Files involved: `pkg/opt/powerscrolls/powerscroll.src`, `pkg/opt/powerscrolls/textcmd/admin/raisecaps.src` (renamed to `textcmd/test/raisecaps.src`), `config/command_synopses.cfg`

Notable functional changes: reading a power scroll played `PlayObjectCenteredEffect(who, 0x33EA, 0x1, 10)`, which is not the intended "wings" visual — changed to `0x6F61`, keeping the accompanying sparkle effect (`0x373A`) and sound unchanged. In the same commit, `raisecaps` was moved from `admin`/`CmdLevel 4` to `test`/`CmdLevel 5` in both its file location and `command_synopses.cfg`, restricting an already-admin-only command further to developer-tier access.

Expected impact: power scroll use shows the correct visual effect; `raisecaps` is further access-restricted (no player-facing effect either way).

### 26. New `.createitemdesc` Developer Tool

Files involved: `pkg/opt/alryc/textcmd/test/createitemdesc.src` (new)

Notable functional changes: `.createitemdesc <package> [perrow]` loads a package's `itemdesc.cfg` and spawns/locks down one of every item objtype it defines at the caller's location, arranged in rows (default 20/row, max 50), each auto-named `"<hex objtype> : <config Name>"` for visual review of an entire item catalog at once — used during this patch to eyeball the new fishing/crustacean/treasure-chest catalogs.

Expected impact: none (developer tool only).

### 27. Core Engine Changes

Files involved: `core-changes.txt`, `pol.exe`/`poltool.exe`/`uoconvert.exe`/`uotool.exe`/`scripts/ecompile.exe`/`scripts/runecl.exe` (binary rebuilds), `scripts/modules/polsys.em`

Notable functional changes (per the `08-10-2026 Nando:` engine-changelog entry, prepended to `core-changes.txt`):
- Added `polsys::CheckItemIntegrity()` (exposed via the new `scripts/modules/polsys.em` line) — validates every item's container/corpse-layer/storage/cursor/world-zone bookkeeping agrees both ways, logs disagreements without changing anything, returns `{checks, violations}`; the `.integ` text command now runs it and reports both counts.
- Added `corpse.equipped_items` — the items a corpse visually shows worn, in layer order, matching the client.
- Changed: `item.layer` now reads 0 until an item is actually worn across all four equip paths (previously `uo::EquipItem`/world-load set it early while the client's own equip paths never did); scripts needing the eventual layer should read `item.tile_layer`. An unworn item no longer reports/saves a stale layer value. A corpse now shows what its owner was wearing at death and keeps showing it until an item is looted; re-adding a looted item afterward is ordinary loot, not re-dressing.
- Fixed: a race-changer crash on a hair/beard id the core accepts but shard tiledata lacks; a worn item whose EquipScript rejected it during world load used to halt server startup entirely, now goes to backpack instead; `uo::EquipItem` crash when an item's own EquipScript made it unequippable mid-run, now returns an error instead; `uo::MoveItemToContainer` could destroy an item still listed in its origin container (delayed crash), now refused instead; a house component moved out of the house stayed on the house's component list, so destroying the house destroyed the item wherever it ended up; an item could get double-registered as a boat traveller (moved twice per step, listed twice after restart); dropping an item while another was already on the cursor sent no reply, leaving the dropped item stuck to the cursor client-side; `uo::DestroyItem` on a cursor-held item left the character unable to pick anything else up; an equipped item rejected by `uo::MoveItemToSecureTradeWin` went to backpack instead of back onto its original layer; corpses showed no equipment visually after a restart despite the data being intact; corpses held onto every item ever shown as equipment indefinitely, blocking those items from being freed; container slots (`UseContainerSlots`) were effectively non-functional (slot assignment not saved, free-slot search only checked the first item, full containers still reported room) — now slots persist via a new `SlotIndex` save line (old worlds unaffected, only written for items that have one), free-slot detection is correct, and full containers are properly refused; splitting a stack gave the new slot to the split-off portion instead of the part left behind; an item moved off a boat stayed listed as travelling with it until the boat's next move, briefly visible to nearby players in places it shouldn't be; destroying worn equipment left the status window showing stale weight; `CreateItemAtLocation`/`CreateItemCopyAtLocation` could return an item not actually at the requested location if the item's own create-script relocated it mid-creation — the requested location now always wins.

Expected impact: mostly invisible server-stability and correctness fixes (several server-crash/shutdown bugs fixed); container slot systems, corpse equipment display, and boat item-tracking should behave visibly more correctly.

### 28. Miscellaneous

Files involved: `scripts/ai/merchant.src`, `pkg/opt/alryc/textcmd/test/animatedgraphics.src`, `scripts/include/omegacache_utils.inc`

Notable functional changes:
- `merchant.src`: removed two `sleepms(100)` per-item pauses in `BuyBag`/`BuyAll` (bulk vendor sell-all) — reduces delay/lag when selling a full backpack to a vendor.
- `animatedgraphics.src` (dev tool): fixed a spawn-order/direction bug — was spawning configured animation groups from the selected group down to 1 (reverse order), each subsequent group offset in the negative Y direction; now spawns group 1 up to the selected group, offset in the positive Y direction. Synopsis/sysmessage text updated to match.
- `scripts/include/omegacache_utils.inc` adds `DeleteOmegaCacheStore()`, currently functionally identical to the existing `CloseOmegaCacheStore()` (just unloads the datafile) since the engine still has no exposed file-deletion call — kept as a distinct, forward-looking name so teardown call sites read as "gone for good" and a real delete can be dropped in later.

Expected impact: faster vendor bulk-sell; no other player-visible effect (dev tool + internal naming only).

---

## Validation Notes

- Diff range for this update: `git diff a642726..HEAD` and `git log --oneline a642726..HEAD` for the newly-covered slice (Themes 16-28); `git diff 541e883..HEAD` for the full-patch scope numbers in the Scope Summary above. Individual theme detail derived via `git show <hash>` and `git diff <range> -- <path>` per file/commit.
- Working tree state: **clean** as of this update — all work through `60949cb` (Crustacean Fishing) is committed.
- The engine-level changes in Theme 27 come from `core-changes.txt`'s own changelog entry, not independently re-verified against the C++ source in this pass — treat that section as the core team's own record of their work.
- Compile validation for Themes 1-15 (`541e883..a642726`) was performed at the time (see prior note, retained implicitly): `scripts/ecompile.exe -b -r -u` run twice, one real error found and fixed (`DeleteDataFile`), 0 errors on the second pass. Compile validation for Themes 16-28 (`a642726..HEAD`) was **not independently re-run as part of this documentation pass** — per repo convention the user compiles EScript changes themselves; this changelog reflects source-level analysis of the committed diff, not a fresh compile check.
