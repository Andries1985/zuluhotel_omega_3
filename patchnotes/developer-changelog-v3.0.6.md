# Developer Changelog - v3.0.6

Range: Patch-3.0.5..Patch-3.0.6 (commit `38e8a76`..`6ed29ee`)
Branch: Patch-3.0.6
Date: 2026-08-11

---

## Scope Summary

- Total files changed: 97
- Status breakdown: 85 modified, 9 added, 2 deleted, 1 renamed
- Net textual delta: 9969 insertions, 7665 deletions (text files only; 6 binaries were also rebuilt: `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe`)
- Largest shifts:
  - `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg` (+3093 / -3489) - bulk regeneration, renumbered/deduplicated door decor entries
  - `pkg/multis/house/include/footagearrays.inc` (+2375 / -0, new file) - auto-generated lockdown/ban-tile arrays for newly named legacy multis
  - `pkg/multis/house/config/itemdesc.cfg` (+979 / -951) - deed renaming, two new sign graphics, demolish-refund coverage for ~114 multis
  - `pkg/multis/customhousing/sign.src` (+538 / -30) - shared friend/co-owner/ban core, decay wiring, Omega Cache removal, gump additions
  - `config/boats.cfg` (+448 / -480) - boat component schema migration (tillerman/hold removed, sails/storage/weaponslot added)
  - `pythonscripts/analyze_memory_usage.py` / `Analyze-MemoryUsage.ps1` (+286 / +199, new files) - dev tooling used to drive this patch's memory-leak fixes
  - `pkg/multis/house/multiSign/use.src` (+166 / -163) - dead-code removal, Omega Cache removal option, teleporter sub-gump rewire
  - `pkg/multis/customhousing/include/house.inc` (+137 / -95) - serial-based ownership migration, cross-account house cap
  - `config/mrcspawn.cfg` (+129 / -122) - new Master Builder / Grand Surveyor vendors replacing flat deed stock
  - `pkg/opt/omegacache/omegacache.inc` (+126 / -38) - cross-package sign resolver fixing broken co-owner/friend access
  - `pkg/multis/house/include/multihouse_settings.inc` (+125 / -0, new file) - shared cross-account house cap API
- Non-merge commits in range:
  - New update to areas and doors and regions to fix errors, attempt to fix character death being mounted (`6cc93f5`)
  - Logon and housing updates (`3d1a141`)
  - Char death mounted fix reverts as it was client side (`2c2d81a`)
  - Update core, lots of housing fixes, setph fixes, custom housing fixes, sign consolidation, memory usage analysis (`6ed29ee`)
- Merge commits in range (no unique changes): `49041b9`, `c63d3cd`

---

## Complete File Inventory (Exhaustive)

Legend: `Status | File`

- A | ainotes/vendor-buy-list-count-bug.md
- M | config/boats.cfg
- M | config/command_synopses.cfg
- M | config/mrcspawn.cfg
- M | config/npcdesc.cfg
- M | core-changes.txt
- M | pkg/items/sysbook/commands/admin/addtoshardlibrary.src
- M | pkg/items/sysbook/commands/gm/addspawnshelf.src
- M | pkg/items/sysbook/commands/gm/removespawnshelf.src
- M | pkg/items/sysbook/commands/test/spawnshelfdatawipe.src
- M | pkg/items/sysbook/include/spawnShelf.inc
- M | pkg/items/sysbook/include/sysBook.inc
- M | pkg/multis/customhousing/config/itemdesc.cfg
- A | pkg/multis/customhousing/decaywatcher.src
- M | pkg/multis/customhousing/include/house.inc
- M | pkg/multis/customhousing/include/housefriends.inc
- R | pkg/multis/customhousing/scripts/customeHouseDeed.src -> pkg/multis/customhousing/scripts/customhousedeed.src
- M | pkg/multis/customhousing/sign.src
- M | pkg/multis/customhousing/signcontrol.src
- M | pkg/multis/house/config/itemdesc.cfg
- M | pkg/multis/house/config/settings.cfg
- A | pkg/multis/house/include/footagearrays.inc
- A | pkg/multis/house/include/multihouse_settings.inc
- M | pkg/multis/house/include/utility.inc
- M | pkg/multis/house/multiDeed/changeOwner.src
- M | pkg/multis/house/multiDeed/use.src
- M | pkg/multis/house/multiSign/method.src
- M | pkg/multis/house/multiSign/use.src
- M | pkg/multis/house/walkOn.src
- M | pkg/multis/staticHousing/commands/gm/removeStaticDeed.src
- M | pkg/multis/staticHousing/commands/gm/staticDeed.src
- M | pkg/multis/staticHousing/commands/player/decorate.src
- M | pkg/multis/staticHousing/config/itemdesc.cfg
- M | pkg/multis/staticHousing/config/settings.cfg
- M | pkg/multis/staticHousing/lockunlock.src
- M | pkg/multis/staticHousing/securecontainer/staticSecureCont.src
- M | pkg/multis/staticHousing/sign/control.src
- M | pkg/multis/staticHousing/sign/destroy.src
- D | pkg/multis/staticHousing/sign/method.src
- M | pkg/multis/staticHousing/sign/use.src
- M | pkg/multis/staticHousing/transferdeed/staticTransferDeed.src
- M | pkg/opt/alryc/textcmd/test/clearmount.src
- A | pkg/opt/alryc/textcmd/test/memdump.src
- M | pkg/opt/areas/areas.cfg
- M | pkg/opt/areas/include/areapolicy.inc
- M | pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg
- M | pkg/opt/omegacache/destroycache.src
- M | pkg/opt/omegacache/omegacache.inc
- M | pkg/opt/omegacache/placecache.src
- M | pkg/opt/powerhour/textcmd/player/ph.src
- M | pkg/opt/powerhour/textcmd/player/setph.src
- M | pkg/opt/powerhour/textcmd/test/resetph.src
- M | pkg/packethooks/megacliloc/mobiledata.src
- M | pkg/systems/accounts/config/settings.cfg
- M | pkg/systems/accounts/hook/onLogin.src
- M | pkg/systems/accounts/include/accounts.inc
- M | pkg/systems/accounts/logon.src
- M | pkg/systems/accounts/reconnect.src
- M | pkg/systems/email/chardelete.src
- M | pkg/systems/email/commands/gm/inspectmail.src
- M | pkg/systems/email/email.src
- M | pkg/systems/email/logon.src
- M | pkg/systems/email/reconnect.src
- M | pkg/systems/email/webmail/webmail.src
- M | pkg/systems/playervendor/commands/player/escrow.src
- M | pkg/utils/mdgumps/commands/test/gfchart.src
- A | pkg/utils/mdgumps/commands/test/gumpbrowser.src
- A | pkg/utils/mdgumps/commands/test/gumppic.src
- A | pkg/utils/mdgumps/scripts/gumpbrowser/preview.src
- M | pol.cfg
- M | pol.exe, pol.pdb
- M | poltool.exe, poltool.pdb
- A | pythonscripts/Analyze-MemoryUsage.ps1
- A | pythonscripts/analyze_memory_usage.py
- A | pythonscripts/__pycache__/fix_house_sign_objtypes.cpython-313.pyc
- M | regions/regions.cfg
- M | scripts/ecompile.exe, scripts/ecompile.pdb
- D | scripts/include/account.inc
- M | scripts/include/dismount.inc
- M | scripts/include/housing.inc
- M | scripts/include/myutil.inc
- M | scripts/misc/chrdeath.src
- M | scripts/misc/customhousecommit.src
- M | scripts/misc/logon.src
- M | scripts/misc/runalways.src
- M | scripts/runecl.exe, scripts/runecl.pdb
- M | scripts/textcmd/coun/home.src
- M | scripts/textcmd/seer/findboat.src
- M | uoconvert.cfg
- M | uoconvert.exe, uoconvert.pdb
- M | uotool.exe, uotool.pdb

---

## Detailed Changes By Theme

### 1) Housing - Unified Cross-Account House Cap

Files involved:
- `pkg/multis/house/include/multihouse_settings.inc` (new)
- `pkg/multis/house/config/settings.cfg`
- `pkg/multis/customhousing/include/house.inc`
- `pkg/multis/staticHousing/commands/gm/staticDeed.src`
- `pkg/multis/staticHousing/config/settings.cfg`
- `pkg/multis/house/multiDeed/use.src`, `changeOwner.src`
- `pkg/multis/house/multiSign/use.src`
- `pkg/multis/staticHousing/sign/use.src`, `control.src`, `destroy.src`
- `pkg/multis/staticHousing/transferdeed/staticTransferDeed.src`
- `pkg/multis/customhousing/scripts/customhousedeed.src`, `sign.src`, `signcontrol.src`

Notable functional changes:
- New `:house:multihouse_settings` include shared by all three housing packages, replacing three separate, inconsistent per-package "how many houses can one account own" systems (customhousing's `HOUSE_VALUE_LIMIT`, staticHousing's value-weighted `HouseValuesCount`/`HouseValuesDeterminedBy` scoring, house's dormant equivalent) with one count-based cap.
- New functions: `get_max_houses_per_account()` (reads `MaxHousesPerAccount` in `house/config/settings.cfg`, default `2`), `RegisterAccountHouseSign(sign, acctname)`, `UnregisterAccountHouseSign(sign, acctname)`, `GetAccountHouseCount(acctname)` (self-healing — drops stale/destroyed sign references on read), `IsAccountUnderHouseCap(acctname)`. Tracking is stored on the existing global property `#housing_of_<acctname>`.
- All three packages gate new-house purchase/transfer on `IsAccountUnderHouseCap` and register/unregister the account on build, transfer, relinquish, decay-demolish, and forced GM destroy.
- staticHousing's entire per-house `housevalue` scoring block in `SetupHouse()` (`staticDeed.src`) was deleted, along with the `RefreshTime`/`HouseValuesCount`/`HouseValuesDeterminedBy`/`HouseValueAppliedTo`/`HouseValueDivisor`/`MaxHouseValue` settings in `settings.cfg`.
- `MigrateSignToSharedHouseCore(sign)` (new, `staticHousing/sign/control.src`, run at every `StaticSignListener` startup) renames the legacy `owneraccount` cprop to `owneracct` and retroactively registers pre-existing owned houses into the new cap registry.

Expected impact:
- One consistent, single-source-of-truth house cap (default 2 per account) across custom, classic, and static housing, replacing three divergent systems where the cap could differ per package or silently not apply (staticHousing's scoring was effectively off by default; customhousing's was hardcoded).

### 2) Housing - staticHousing Sign Core Consolidated Onto Shared `:house:` Method Script

Files involved:
- `pkg/multis/staticHousing/sign/method.src` (deleted, 1421 lines)
- `pkg/multis/staticHousing/config/itemdesc.cfg`
- `pkg/multis/house/multiSign/method.src`
- `scripts/include/housing.inc`
- `pkg/multis/staticHousing/sign/control.src`, `destroy.src`, `use.src`

Notable functional changes:
- staticHousing's `MethodScript` now points at `:house:multiSign/method` instead of its own 1421-line `sign/method.src`, which is deleted entirely. All of staticHousing's sign methods (`IsHouseSign`, `IsACoOwner`, `IsAFriend`, `AddFriend`/`RemoveFriend`, `AddBan`/`RemoveBan`, `AddCoOwner`/`RemoveCoOwner`, `GetLockdowns`/`GetMaxLockdowns`, `UpdateLockdowns`/`UpdateSecures`, `IsAffiliated`, permission helpers, `GetHouseSize`, etc.) now resolve through the shared `house/multiSign/method.src`, which absorbed a new `GetItemsInHouse(sign)` (previously staticHousing-only logic).
- `IsLocationInsideHouse(sign, loc_or_x, y, z, realm)` in the shared method script was extended to accept both a single location-object argument and explicit `x, y, z` arguments, so staticHousing's existing call sites work unmodified.
- Bug fix: `RefreshHouse(sign)` previously read `settings.RefreshTime` via `SH_GetSettingsCfgElem()`, but `:house:settings` has no such key, so the call silently reset decay to "now" instead of extending it. It was dead code in house's own package (which refreshes decay inline elsewhere) but is now live for staticHousing/customhousing, which call `sign.RefreshHouse()` directly. Fixed to use `get_abandon_time()`.
- `scripts/include/housing.inc`'s `GetHouseItems(footage, map)` (duplicate of the new shared `GetItemsInHouse`) was removed; its 3 callers in `staticHousing/sign/control.src` switched to `sign.GetItemsInHouse()`.
- Bug fix: `demolish()` (`control.src`) and `StaticHouseDestruction` (`destroy.src`) previously left `friendlist`/`coowners`/`banlist` intact on decay/forced teardown (unlike the relinquish/change-owner paths), so a decayed house re-sold to a new owner retained the previous owner's friends/co-owners/bans. Both paths now clear these lists.
- Decay/forced-destroy teardown now force-clears any Omega Cache container in the house regardless of contents (erasing `houseserial` first to bypass the non-empty safety block), matching house's own forced-demolish behavior.
- Speech commands ("I wish to lock this down / release this / secure this / unsecure this / remove thyself / I ban thee") are disabled (commented out, not deleted) in `control.src`'s `handle_speech`; `OpenHouseManagementGump` (`sign/use.src`, button 15) is now the sole interface for these actions.

Expected impact:
- One shared sign-method implementation instead of two independently-maintained copies; several latent bugs around house decay's friend/co-owner/ban carryover and secure-decay Omega Cache handling are fixed as a side effect of consolidation.

### 3) Housing - staticHousing Bug Fixes (Independent of Consolidation)

Files involved:
- `pkg/multis/staticHousing/lockunlock.src`
- `pkg/multis/staticHousing/commands/player/decorate.src`
- `pkg/multis/staticHousing/commands/gm/staticDeed.src`
- `pkg/multis/staticHousing/commands/gm/removeStaticDeed.src`
- `pkg/multis/staticHousing/securecontainer/staticSecureCont.src`

Notable functional changes:
- **Security-relevant bug fix**: `lockunlock.src` called `sign.IsLocationInsideHouse(character, item)` (2 args). Because the new dual-signature `IsLocationInsideHouse` dispatches on whether `y`/`z` were passed, a 2-arg call took the "single location object" branch bound to `character`, silently discarding `item` — meaning the check validated the *actor's* position, not the *item's*. This allowed a character standing inside a house to lock down/secure/raise/lower items physically outside its footage, and could incorrectly block legitimate in-house items. Fixed by explicitly passing `item.x, item.y, item.z` in both call sites (`HouseLockUnlockItem`, plus a new `IsLocationInsideHouseMsg` wrapper used by secure/raise/lower).
- Bug fix: `sign.IsLocationInsideHouseMsg(...)` was called in three places but was never defined anywhere (not in the old `method.src`, not in the new shared one) — every call was erroring at runtime. Replaced with a local `IsLocationInsideHouseMsg(sign, character, item)` wrapper in `lockunlock.src`.
- Bug fix: `IsADeedItem` (multipart-furniture pickup path) read lowercase `deedobjtype`/`deedcolor`/`otheritems` cprops, but the carpentry-deed system always writes capitalized `DeedObjType`/`DeedColor`/`OtherItems` — the lookup never matched, so the "preferred" multipart-deed pickup path never fired. Fixed casing. Same casing bug fixed in `decorate.src` (`OPTION_ROTATE`, `IsLockedDownMovableItem`, `MoveItemInDirection`, `ManipulateItemHeight`), which read `otheritems` instead of `OtherItems`.
- Bug fix: `SignSerial` cprop casing corrected to `signserial` in `staticDeed.src` and `removeStaticDeed.src`, matching what's actually read elsewhere.
- `staticSecureCont.src`: simplified the fallback `owneraccount`→`owneracct` lookup down to just `owneracct`, now that the migration in Theme 1 covers the rename.

Expected impact:
- Fixes a security bug allowing lockdown/secure actions on items outside a house's footage; fixes previously-broken multipart-furniture deed pickup and raise/lower/rotate permission checks in staticHousing.

### 4) Housing - Custom Housing Overhaul (Decay, Ownership Model, Shared Friend Core)

Files involved:
- `pkg/multis/customhousing/decaywatcher.src` (new)
- `pkg/multis/customhousing/include/house.inc`
- `pkg/multis/customhousing/include/housefriends.inc`
- `pkg/multis/customhousing/sign.src`
- `pkg/multis/customhousing/signcontrol.src`
- `pkg/multis/customhousing/scripts/customeHouseDeed.src` -> `customhousedeed.src` (renamed)
- `pkg/multis/customhousing/config/itemdesc.cfg`
- `scripts/misc/runalways.src`

Notable functional changes:
- **New feature**: `decaywatcher.src` is a single global background thread, started once at boot from `scripts/misc/runalways.src` (`Start_Script(":customhousing:decaywatcher")`), sweeping a new `#customhouse_all_signs` registry every 120 seconds and calling `DemolishAbandonedCustomHouse(sign)` on anything past its `decay` timestamp. Custom house decay/abandonment was previously **fully disabled** (in-code comment: "Removed as does not work with ZH scripts"); this patch re-enables enforcement. `signcontrol.src` sets an initial `decay` cprop on load if missing and idempotently registers every sign into the sweep on each `SignListener` run, so pre-existing houses are pulled into decay enforcement rather than staying permanently exempt.
- `scripts/misc/runalways.src` was previously an inert placeholder (infinite loop with `Sleep(600)` commented out, spinning with no yield). It now starts `decaywatcher` once and un-comments its own `Sleep(600)`, fixing a background CPU-spin bug in the process.
- **Ownership model migration**: custom housing moves from account-name-only ownership (`account` cprop, dictionary-based `housefriends`) to the same serial-based owner + shared friend/co-owner/ban/permission core used by `house` and `staticHousing` (`sign.IsOwner`, `AddFriend`/`RemoveFriend`, `AddCoOwner`/`RemoveCoOwner`, `AddBan`/`RemoveBan`, per-friend permission toggles). Migration is lazy: `MigrateCustomHouseSignOwnership(sign, character)` runs on every sign use and promotes the interacting character to serial-based owner only if their account matches the legacy `account` string; un-migrated houses keep working via the legacy account check as a fallback.
- **New feature**: custom housing gains co-owner management (`ManageCoOwners`), ban management (`ManageBans`), and per-friend permission toggles (`ManageFriendPermissions`/`ToggleCustomHousePermission`/`ShowCustomHousePermissionOverview`) via new gump buttons (29/30 plus page additions) — none of this existed before (customhousing previously supported only a flat friends list).
- **New feature**: Omega Cache container removal (`CustomHouseFunctionRemoveOmegaCache`, gump button 12/31), mirroring the equivalent added to `house` and `staticHousing` (Theme 5).
- Bug fix / safety: redeeding a custom house now blocks if any Omega Cache container inside still holds contents (previously unchecked for customhousing).
- `housefriends.inc`'s entire old friend implementation (`IsAFriend`, `IsMerchantHouseFriend`, `AdjustHouseFriendsFromSign`, dictionary-keyed by account-or-serial) was deleted, replaced by `sign.src`'s `AdjustFriendListFromSign` built on the shared core. The "merchant-only friend" concept was dropped (comment notes it had no live consumer).
- Sign objtype changed from `0xfbd2` to `0xFFF4` (customhousing signs are plain items, not multi.mul-sourced, so this does not hit the `uoconvert`-regeneration conflict described in Theme 6).
- `SyncFootageFromHomeinfo(sign)` (new, `signcontrol.src`) derives the shared core's box-array `footage` cprop from customhousing's original single-box `homeinfo`, run on every sign load.
- File rename: `scripts/customeHouseDeed.src` -> `scripts/customhousedeed.src` (fixes a typo in the original filename).

Expected impact:
- Custom house decay/abandonment enforcement returns after being fully disabled; custom housing gains co-owner/ban/permission management and Omega Cache removal parity with the other two housing packages; ownership records migrate transparently from account-name to serial-based tracking.

### 5) Housing - Omega Cache "Remove Container" Option (house / staticHousing)

Files involved:
- `pkg/multis/house/multiSign/use.src`
- `pkg/multis/staticHousing/sign/use.src`
- (customhousing side covered in Theme 4)

Notable functional changes:
- New player-facing gump option ("Remove Omega Cache Container") in the house management gump: house's existing button 9 was reorganized into the new layout; staticHousing gains a new button 228 along with a new `FindStaticOmegaCacheSerial(sign)` helper, since static signs don't track a house-multi serial the way `house` does. All variants block removal while the cache still holds items and refund one cache slot on success.
- Relinquishing a staticHousing house is now also blocked if its Omega Cache still has contents (previously unchecked, unlike house's relinquish path).

Expected impact:
- Players can remove an unwanted Omega Cache storage container from any of the three housing types once it's empty, and can no longer relinquish/redeed a house while cache contents would be lost silently.

### 6) Housing - Omega Cache Cross-Package Access-Control Fix

Files involved:
- `pkg/opt/omegacache/omegacache.inc`
- `pkg/opt/omegacache/destroycache.src`
- `pkg/opt/omegacache/placecache.src`

Notable functional changes:
- Root cause (documented in-line): sign resolution previously relied on a `signserial` cprop set on the house multi, which only the `house` package populates. `staticHousing` and `customhousing` multis never set it (`customhousing` uses a differently-named `house_serial` cprop; `staticHousing` tracks coverage purely by geometry). Lookups for those two packages silently fell back to a legacy `"Friends"` array format that none of the three housing packages populate anymore, so **co-owner/friend deposit/withdraw access to Omega Cache was broken for staticHousing and customhousing houses** (owner-only access via `ownerserial` still worked).
- New `FindOmegaCacheHouseSign()` resolver checks each package's own linkage (`houseserial`, `house_serial` cprops) and falls back to a footage-geometry match (`IsObjectInsideHouse`) for staticHousing, searching a 50-tile radius (bumped from an implicit 24, needed to reach across customhousing's largest 31x31 foundation from a corner anchor).
- New `OmegaCacheHasPrivilege()` / `OmegaCachePrivilegeGranted()` helpers unify the owner/co-owner/friend-with-privilege check against the resolved sign, used in `FindAccessibleContainer`, `ValidateWithdrawDestination`, and `ValidateDepositTarget`.
- `destroycache.src` and `placecache.src` now use the new resolver for both access checks and sign slot-count bookkeeping (`numomegacache`/`maxnumomegacache`), which previously desynced silently for the two affected packages.

Expected impact:
- Co-owners and friends with secure-container permission on staticHousing/customhousing houses can now actually deposit/withdraw from Omega Cache; cache slot counts shown on signs for those house types now update correctly.

### 7) Housing - House Sign Objtype Registration Fix + Deed Renaming/Footage Data

Files involved:
- `pkg/multis/house/config/itemdesc.cfg`
- `pkg/multis/house/include/utility.inc`
- `pkg/multis/house/include/footagearrays.inc` (new)
- `pkg/multis/house/multiDeed/use.src`
- `pkg/multis/house/multiSign/use.src`
- `pkg/multis/house/walkOn.src`
- `scripts/include/housing.inc`

Notable functional changes:
- **Reverts** the earlier custom-objtype sign migration (0xFFF0-0xFFF3, documented separately in [[project_ew_house_sign_bug]]) for the `house` package specifically: signs are now registered directly under raw graphic objtypes `0xBCF`/`0xBD0`/`0xBD1`/`0xBD2`. Rationale (in-code): `uoconvert multis` always regenerates `config/multis.cfg` with raw graphic values baked in from client data, so any scheme relying on remapped objtypes silently breaks on every reconversion. `0xBCF` and `0xBD1` are newly registered as house signs (previously only `0xBD0`/`0xBD2` were), each with distinct names (`woodensign1EW`, `brasssign1EW`, `brasssign2EW`, `brasssign2NS`). All sign-objtype-set checks across `utility.inc`, `multiDeed/use.src`, `walkOn.src`, and `scripts/include/housing.inc` were updated to include the two new graphics.
- Bulk of the `itemdesc.cfg` diff (~1900 lines touched) and the new `footagearrays.inc` content (2375 lines) is auto-generated data continuing the already-documented 3.0.5 multi-import work: renaming ~38 castle-range deeds from generic `multiXXXXhousedeed` placeholders to descriptive names (e.g. "Gothic Rose Castle", "Trinsic Keep", "Camelot"), plus auto-generated heuristic ban-tile arrays for those multis and 76 additional legacy Fiddler-imported multis. Per source comments this data is heuristic and not yet spot-checked in-game (consistent with [[project_legacy86_multis_triage]]).
- Bug fix: `demolish()` (`multiSign/use.src`) previously had no `case` branch for most of these multi IDs, so demolishing one of them destroyed the house with no deed refund. All 38 castle-range and 76 legacy multi IDs now have `CreateItemInBackpack` entries returning the correct specific deed.
- Public/Private house declaration (button 19, `DeclarePrivPub`) disabled — button hidden and case handler commented out (kept, not deleted).
- Dead code removed from `multiSign/use.src`: a second, never-called management gump+dispatcher pair (`OpenHouseManagementGump`/`HandleHouseManagementAction`, confirmed via repo-wide grep as having no live caller), along with `SecureContainerFromMenu` and `PlaceTrashBarrelFromMenu`, which only that dead pair called. The teleporter management sub-gump (`OpenTeleporterManagementGump`) was kept and wired into the real `HouseManagement()` gump as new button 10, since it previously had no live entry point.

Expected impact:
- House sign graphics are now generation-safe against future `uoconvert multis` runs; demolishing any of the ~114 newly-named legacy/castle multis now correctly refunds its specific deed instead of destroying the house for nothing; two dead gump code paths removed.

### 8) Housing - Custom House Construction Billing (New Feature)

Files involved:
- `scripts/misc/customhousecommit.src`

Notable functional changes:
- `CustomHouseCommitScript` now charges gold for custom house construction: `CUSTOMHOUSE_PRICE_PER_TILE` (500 gold) per net-new house part added since the last paid commit, tracked via a new `customhouse_paidparts` cprop on the house. Pre-existing houses are grandfathered — their first commit under the new system treats the current part count as already paid, with no retroactive charge. GMs (`cmdlevel > 0`) are exempt. If the player can't afford the charge, the commit is rejected (`house.acceptcommit(who, 0)`) with a gold-amount message; otherwise it's accepted and gold is deducted.
- **Previously, custom house construction/editing was entirely free.** This is a new cost added to the system, not a bug fix.

Expected impact:
- Custom house building/editing now costs 500 gold per newly added part; existing houses aren't retroactively charged for parts already placed.

### 9) Mount-on-Death Investigation (Net Simplification, Confirmed Client-Side)

Files involved:
- `scripts/misc/chrdeath.src`

Notable functional changes:
- Commit `6cc93f5` added an aggressive server-side attempt to fix the "character appears mounted after death" bug: checking the ghost's root contents/mount layer in addition to corpse layer 25, a 30-layer stray-mount sweep, a backpack sweep, extra property erases (`DMountSerial`, `bmSpeed`, `SpeedWalk`), an `IncRevision`+`MoveObjectToLocation` force-redraw, and a raw `SendPacket` movement-state reset.
- Commit `2c2d81a` reverted essentially all of it after confirming (see [[project_mount_stuck_on_death_bug]]) that the bug is client-side, not server-side.
- Net effect between `38e8a76` and `HEAD` is **not** an exact revert to the pre-investigation baseline — the mount-detection logic ends up simplified *further* than baseline. Baseline checked corpse layer 25 → ghost layer 25 → corpse root contents (3 checks) and, on dismount failure, force-destroyed the mount item with a diagnostic `Print`; it also kept the `SendPacket`/`IncRevision`/`MoveObjectToLocation` resync block. HEAD only checks corpse root contents for objtype `0x1F021` (one check, via inline `foreach`) and calls `Dismount(ghost, mount)` with no failure fallback, no diagnostics, and no resync packet block.

Expected impact:
- No player-visible change expected — the underlying bug is confirmed client-side. Flagging for awareness that the net server-side mount-clearing logic is narrower than the pre-3.0.5 baseline in case an edge case the original 3-way check covered resurfaces; `pkg/opt/alryc/textcmd/test/clearmount.src` remains available as a manual GM tool per [[project_mount_stuck_on_death_bug]].

### 10) FindObjtypeInContainer / EnumerateItemsInContainer Root-Only Cleanup

Files involved:
- `scripts/include/myutil.inc`
- `scripts/include/dismount.inc`
- `pkg/opt/alryc/textcmd/test/clearmount.src`
- `pkg/systems/playervendor/commands/player/escrow.src`

Notable functional changes:
- Continuation of [[project_findobjtype_rootonly_cleanup]]: repo-side manual `container ==` filtering loops replaced with the engine's native `ENUMERATE_ROOT_ONLY` (`EnumerateItemsInContainer`) / `FINDOBJTYPE_ROOT_ONLY` (`FindObjtypeInContainer`) flags.
- `myutil.inc`: `ListRootItemsInContainer(container)` now returns `EnumerateItemsInContainer(container, ENUMERATE_ROOT_ONLY)` directly instead of manually enumerating all nested items and filtering by `item.container == container`. `ListRootItemsInContainerOfObjtype(container, objtype)` now iterates the root-only enumeration and filters only by objtype.
- `dismount.inc`: `dismount(me, mount:=0)`'s corpse-type branch now does `mount := FindObjtypeInContainer(me, 0x1f021, FINDOBJTYPE_ROOT_ONLY)` instead of a manual last-match loop.
- `clearmount.src`: `RemoveRootMountItems(mob)` switched to a single `FindObjtypeInContainer(mob, 0x1F021, FINDOBJTYPE_ROOT_ONLY)` call.
- `escrow.src`: `DestroyContainerAndContents(byref container)` now uses `EnumerateItemsInContainer(container, ENUMERATE_ROOT_ONLY)` directly instead of a manual container-equality filter.

Expected impact:
- Internal refactor/perf cleanup only; behavior should be functionally equivalent, now using native engine filtering instead of manual O(n) checks over potentially-nested enumeration.

### 11) Accounts / Logon Changes

Files involved:
- `pkg/systems/accounts/include/accounts.inc`
- `pkg/systems/accounts/logon.src`, `reconnect.src`
- `pkg/systems/accounts/hook/onLogin.src`
- `pkg/systems/accounts/config/settings.cfg`
- `scripts/misc/logon.src`
- `scripts/include/account.inc` (deleted)
- `scripts/textcmd/seer/findboat.src`
- `scripts/misc/runalways.src` (powerhour-spin fix noted in Theme 4)
- `scripts/textcmd/coun/home.src`

Notable functional changes:
- `accounts.inc`: dead-code removal (~220 lines) — `ACCT_GetPendingLoginReservations`, `ACCT_SavePendingLoginReservations` (unused reservation-dictionary helpers), `VerifyStaffOnline(who)` (its actual enforcement logic was already commented out), `CheckForMaxClientsOnline_Legacy(who)` (the old IP/Discord/character-limit algorithm), and `ACCT_CheckForMaxClientsOnline_NewEngineScaffold(who)` (an A/B comparison shim that ran both legacy and new-engine checks and logged mismatches). The live `CheckForMaxClientsOnline(who)` and `ACCT_CheckForMaxClientsOnline_NewEngine(who)` remain as the sole implementation. `ACCT_ClearPendingLoginReservation` had its now-dead reservation-dictionary lookup/erase block removed (kept only the account-property erases).
- `accounts.inc`: `ACCT_GetHouseholdStore(create_if_missing:=1)` now calls `UnloadDataFile("AccountsHouseholds")` before returning, fixing a memory-usage issue where this datafile stayed resident after every household lookup.
- `logon.src` / `reconnect.src` (accounts package): drop their now-removed `VerifyStaffOnline(who)` call.
- `hook/onLogin.src`: `discordID, pin` were declared uninitialized; now initialized to `"", 0` so auto-created accounts (no Discord packet data available at this stage) get real empty-string/zero values passed into `CreateNewAccount()` instead of undefined vars.
- `accounts/config/settings.cfg`: `DebugAccountsPolicy` flipped `1 -> 0`, turning off `[ACCTDBG]` console trace spam left on from earlier debugging.
- `scripts/misc/logon.src` (the separate, older logon hook): removed its own `CheckForMaxClientsOnline(who)` call, which duplicated the check already performed by `pkg/systems/accounts/logon.src`'s own call to the same function (both scripts hook `Logon`) — eliminates a redundant duplicate max-clients check per login. Also adds powerhour-resume robustness: on reconnect with a stale `#PPHH`/`#PPHC`/`#PPHS` property set, it now kills the original `activateph()` process via `GetObjProperty(who,"#PPHPid")` + `GetProcess(...).kill()` before starting `resumeph.src`, and clears `#SettingPH`/`#PPHPid`. This fixes a real bug where a plain disconnect (not a reboot) left the original powerhour timer alive alongside the resumed one, causing players to see the "has ended!" message twice.
- `scripts/include/account.inc` deleted entirely (32 lines) — a standalone `GetAccountProperty`/`SetAccountProperty`/`EraseAccountProperty` trio backed by its own `:statistics:statistics` datafile, unrelated to `pkg/systems/accounts`. Its only caller, `scripts/textcmd/seer/findboat.src`, had the `include "include/account";` removed with no replacement (unused/dead there). This is a straight removal of an orphaned legacy include, not a move.
- `scripts/textcmd/coun/home.src`: `.home` GM/counselor teleport target changed from `(9, 2049, 0, britannia)` to `(7141, 50, 0, britannia)`.

Expected impact:
- Reduced memory footprint from account/household datafile handling; a duplicate per-login max-clients check removed; a real bug fixed where reconnecting players could see a duplicate "Power Hour has ended!" message; dead/orphaned code removed with no behavior change.

### 12) Email System Memory-Usage Fix

Files involved:
- `pkg/systems/email/chardelete.src`, `logon.src`, `reconnect.src`, `email.src`, `webmail/webmail.src`, `commands/gm/inspectmail.src`

Notable functional changes:
- Uniform fix applied across the email package: every script now calls `UnloadDataFile("Emails")` (and `"BlockLists"`, `"AddressBooks"` where relevant) on every exit path, including early returns, instead of leaving the datafile resident after each run. `chardelete.src`'s `OnDelete(mobile)` additionally unloads `"AddressBooks"` and `"BlockLists"` after deleting the corresponding elements.

Expected impact:
- Reduced server memory growth from the email system over uptime; no player-visible behavior change.

### 13) New Debug Tooling

Files involved:
- `pkg/opt/alryc/textcmd/test/memdump.src` (new)
- `config/command_synopses.cfg`
- `pkg/utils/mdgumps/commands/test/gfchart.src`, `gumpbrowser.src` (new), `gumppic.src` (new), `pkg/utils/mdgumps/scripts/gumpbrowser/preview.src` (new)
- `pythonscripts/Analyze-MemoryUsage.ps1` (new), `pythonscripts/analyze_memory_usage.py` (new)

Notable functional changes:
- `memdump.src`: new test-tier GM command `program memdump(who)` calling `polcore().internal(2)` and `internal(5)` (POL core diagnostic dump codes for script memory/profiling) with a confirmation sysmessage.
- `gfchart.src` reworked from a fixed single-page 1-120 hue chart into a paginated browser (Back/Next/Goto-page controls, configurable background gump) covering all 3000 hues across 30 pages.
- `gumpbrowser.src` / `gumppic.src` (new test commands) and `scripts/gumpbrowser/preview.src` add a new gump/gump-picture browsing debug tool.
- `Analyze-MemoryUsage.ps1` (199 lines) / `analyze_memory_usage.py` (286 lines): new standalone scripts parsing `memoryusagescripts.log` for per-script memory statistics (top-N consumers, CSV export) — the tooling used to identify the datafile-leak fixes in Themes 11, 12, 16, and 17.
- `command_synopses.cfg` auto-regenerated to include `memdump` and other new/changed commands.

Expected impact:
- GM/dev-only tooling; no player-visible effect.

### 14) Open Investigation: Vendor Buy-List Stack Count Display Bug (Unresolved)

Files involved:
- `ainotes/vendor-buy-list-count-bug.md` (new)

Notable functional changes:
- Documents an open, **unresolved** investigation: NPC vendor buy windows show stacked items (e.g. house deeds with `Amount=10`) with the stack count baked into the display name plus a blind `+s` pluralization (e.g. "10 Deed To ... Easts at 1000gp"), duplicating the separate "Avail." column. Traced into upstream `polserver/polserver` engine source (`item.cpp`'s `merchant_description()`, `uomod2.cpp`'s `send_vendorwindow_contents`, `ufunc.cpp`'s `format_description()`); `merchant_description()` is expected to hardcode `amount=1` for vendor windows, but the real stack size still leaks into the display text. Ruled out as causes: cliloc text, `itemdesc.cfg` `Desc` field, and escript (`mrcspawn.inc`/`merchant.src`/`playermerchant.src` don't touch the display text). Ends with an open question to upstream about the leak; this is an engine-level bug, not fixable from this repo's scripts, and remains unresolved as of this patch.

Expected impact:
- No fix in this patch; documented for future follow-up (likely requires an upstream polserver engine fix).

### 15) Engine Core Sync (polserver)

Files involved:
- `core-changes.txt`
- `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe` (rebuilt)

Notable functional changes:
- `core-changes.txt` gained 30 lines documenting upstream engine fixes dated 07-31-2026 through 08-08-2026 (Turley/Kevin). Highlights:
  - Custom housing customizable components are no longer limited to a single network packet, raising the practical per-house component cap (enables [[project_customhouse_plane_4095_byte_limit]], noted resolved 2026-08-10).
  - Fixed: every mobile briefly read as criminal for the first ~0.01s after shard boot (a cleared criminal timer compared against a polclock starting at zero).
  - Fixed: client map pins were bounds-checked against the map's world area instead of its gump size (rejected valid pins on a map smaller than its gump, accepted invalid pins on a map larger than it); divide-by-zero on a map with a zero-size axis is now avoided by not sending such maps to the client.
  - Fixed: `Spellbook.addspell()/hasspell()/removespell()` accepted a spellid of another school or past the 64-spell book limit, wrapping it onto a spell of the caller's own school.
  - Fixed: `uo::GetSpellDifficulty`/`StartSpellEffect`/`ConsumeReagents`/`SpeakPowerWords` and `vitals::ConsumeMana` read out of bounds for a spellid one past the highest defined spell.
  - Fixed: `uo::ConsumeReagents` consumed reagents it had already found when a later required reagent turned out to be missing (partial-consumption bug on a failed cast).
  - Fixed: several ways to crash the shard in CustomHouses; the wrong revision was sent after erase/clear; syshook `CloseCustomHouse` now also fires on client commit when no `misc/customhousecommit.src` exists.
  - Fixed: `PacketHook` with `SubCommandOffset` on a too-short packet now falls through to the parent/default handler instead of misbehaving; several out-of-bounds reads in packet scriptobjects fixed; an outgoing `PacketHook` setting an encoded length larger than the buffer no longer shuts the server down (now sent at buffer size and logged).
  - Fixed: `XMLNode.AppendXMLNode`/`AppendXMLText`/`SetXMLAttribute`/`RemoveXMLAttribute` crashed on an invalid node (now returns "Node is not an element"); `XMLFile.RemoveXMLNode`/`XMLNode.RemoveXMLNode` never actually removed anything when a node object was passed; `BinaryFile` could not be used again after a failed read — `Seek()`/`Size()` now work again, allowing recovery.
  - Fixed: `AuxConnection::transmit()` didn't guarantee packet ordering; datastore loading fixed once the version overflows; `uo::MoveItemToSecureTradeWin` could crash if the item was rejected from the trade (e.g. via a `CanTrade` hook); `uo::PrintTextAbovePrivate` ignored `journal_print` for UTF-8 text.
- Rebuilt `pol.exe`, `poltool.exe`, `scripts/ecompile.exe`, `scripts/runecl.exe`, `uoconvert.exe`, `uotool.exe` against the synced engine revision.

Expected impact:
- Custom housing supports larger customizable-component counts; a startup-only criminal-flag false positive is fixed; map-pin bounds and divide-by-zero issues fixed; several spellcasting edge-case bugs fixed; packet-handling crash/OOB-read fixes; XML/BinaryFile datafile API reliability fixes.

### 16) Boats - Component Schema Migration

Files involved:
- `config/boats.cfg`

Notable functional changes:
- Not new boats — every boat definition was reformatted and every entry's `TillerMan` and `Hold` component lines were removed (16 each, across all boat orientations), replaced with new `Sails`, `Storage`, and `Weaponslot` component rows that didn't exist before.

Expected impact:
- Boat interiors gain new storage and weapon-slot furniture points; the old tillerman/hold component model is removed shard-wide. This reads as a boat-system rework rather than a content tweak — flagged for confirmation that the migration is complete and intentional.

### 17) NPC/Vendor Spawns - New Housing Deed Vendors

Files involved:
- `config/mrcspawn.cfg`
- `config/npcdesc.cfg`

Notable functional changes:
- Two new merchant types and matching NPC templates added, replacing a flat list of legacy multi-house deed items in the generic "Deeds" `ProductGroup`:
  - **MasterBuilder** (`npcdesc: masterbuilder`, human vendor, `architect` equip set) sells/buys the new `FiddlerMultiDeeds` `ProductGroup` — ~90 named classic-shape multi deeds (castles, keeps, cabins, etc.), matching [[project_legacy86_multis_triage]].
  - **GrandSurveyor** (`npcdesc: grandsurveyor`, same template shape) sells/buys the new `CustomHouseDeeds` `ProductGroup` (`customtwostoryhousedeed`, `customthreestoryhousedeed`).
  - The old flat "Deeds" `ProductGroup` lost ~100 `multiXXXhousedeed` line items (moved into `FiddlerMultiDeeds`) and one entry, `gothicfortressdeed`, that appears dropped outright rather than relocated to either new group.

Expected impact:
- Two new vendor NPC types spawn (Master Builder, Grand Surveyor) selling housing deeds previously sold generically; existing generic deed vendors lose multi-house deed stock. `gothicfortressdeed` availability should be spot-checked — it does not appear in either new `ProductGroup`.

### 18) Areas / Regions

Files involved:
- `pkg/opt/areas/areas.cfg`
- `regions/regions.cfg`
- `pkg/opt/areas/include/areapolicy.inc`

Notable functional changes:
- Two dungeon region bounding boxes enlarged, edited in lockstep across both config files: Fire Dungeon's Y-range extended from `2196-2259` to `2196-2559` (300 tiles taller); Caverns of Despair's Y-range extended from `1076-1463` to `1048-1463` (28 tiles).
- `areapolicy.inc`: every code path that opens the per-realm area-policy datafile (`LoadRealmPolicies`, `SaveRealmPolicies`, `GetPolicyMask`, `SetPolicyMask`, `EnsurePolicyDataInitialized`, `PruneStaleRealmPolicyEntries`) now explicitly calls `UnloadDataFile()` before returning on every path including errors — a datafile-handle leak fix (same theme as Themes 12, 17).

Expected impact:
- Fire Dungeon and Caverns of Despair now cover more map area for region-based effects (no-PK/guards/spawns/whatever policy is bound to those region IDs); reduced server memory growth from area-policy datafile handling.

### 19) Power Hour - Personal System Added

Files involved:
- `pkg/opt/powerhour/textcmd/player/ph.src`, `setph.src`
- `pkg/opt/powerhour/textcmd/test/resetph.src`

Notable functional changes:
- `setph.src`: gump rewritten from raw `SendDialogGump` primitives to the `mdgumps` GFCreateGump/GFResizePic/GFRadioButton framework (resizable panel, wrapped text, cleaner layout). `activateph()` now also stores `#PPHPid` (the sleeping script's process ID) so it can be force-killed rather than waited out.
- `ph.src`: `.ph` now reports the caller's own active personal Power Hour (Hunting/Half-resources/Double-skillgain, with remaining minutes) and, if none is active, the ETA until they're next eligible (mirroring `setph.src`'s weekly eligibility window) — previously it only reported server-wide Power Hour status.
- `resetph.src` (test/admin command): now also kills the sleeping `activateph()` process via the stored `#PPHPid` and clears `#SettingPH`, instead of just erasing property flags and leaving the sleeping script instance to expire on its own up to an hour later.

Expected impact:
- Players get a personal Power Hour with its own `.ph`/`.setph` status and cooldown reporting, distinct from the server-wide one; `.resetph` now terminates a player's personal PH immediately and cleanly instead of leaving a lingering background process.

### 20) Sysbook / Library Datafile-Leak Fixes

Files involved:
- `pkg/items/sysbook/commands/admin/addtoshardlibrary.src`, `commands/gm/addspawnshelf.src`, `commands/gm/removespawnshelf.src`, `commands/test/spawnshelfdatawipe.src`, `include/spawnShelf.inc`, `include/sysBook.inc`

Notable functional changes:
- Same datafile-leak-fix pattern as Themes 11/12/18: all gained `UnloadDataFile()` calls on the `shard_library` and `spawnedbookshelves` datafiles after use, including on early-return/cancel paths.

Expected impact:
- Reduced server memory growth from the sysbook/library system; no player-visible behavior change.

### 21) Other Small/Behind-the-Scenes Items

Files involved:
- `pol.cfg`
- `uoconvert.cfg`
- `pkg/packethooks/megacliloc/mobiledata.src`
- `pkg/opt/decoratefacets/decorations/britannia_alt/doors.cfg`

Notable functional changes:
- `pol.cfg`: `LogLevel`, `WatchRPM`, `WatchSysLoad`, `LogSysLoad` all flipped from `1` back to `0` — verbose console/log output left on for the memory-usage investigation is disabled again for normal operation.
- `uoconvert.cfg`: the `Mounts`/`Tiles` mount-graphic ID list was reformatted (tab indent, uppercase hex) and reordered; one stray tile range (`0x3EDE`-`0x3EE2`) present before is gone, and `0x3E9D`/`0x3E9C` swapped order. Net set looks like a near 1:1 reorder/cleanup rather than a substantive content change — worth a diff-by-set check if mount conversion behavior regresses.
- `pkg/packethooks/megacliloc/mobiledata.src`: mobiles with `.master` set (tamed/summoned creatures) now get an "Owner: `<name>`" line added to their tooltip/OPL via cliloc `1070722` (the generic freeform-text passthrough cliloc).
- `doors.cfg` (6582-line diff): bulk regeneration — `Decor` entries renumbered sequentially (gaps closed) and a number of duplicate/near-duplicate door entries at the same coordinates removed, consistent with the ongoing 3.0.5-era multi-regeneration work; not a hand-authored content change.

Expected impact:
- Reduced log verbosity for normal operation; mousing over an owned pet/summon now shows its owner's name in the properties popup; door-decoration data deduplicated with no expected functional change.

---

## Validation Notes

- Diff range used: `Patch-3.0.5..Patch-3.0.6` (`38e8a76`..`6ed29ee`).
- Coverage checks used:
  - `git diff --name-status 38e8a76..HEAD`
  - `git diff --numstat 38e8a76..HEAD`
  - `git log --no-merges --oneline 38e8a76..HEAD`
  - Full-diff review of each subsystem area (housing, accounts/core-bugfix, misc/engine) via parallel deep-dive passes.
- Working tree was clean (no staged/uncommitted changes) at time of writing — this changelog is exhaustive for the full committed branch delta.
