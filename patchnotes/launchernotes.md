# Latest Changes
Always check Discord announcements for all the patch notes.

---

## What Changed

Patch 3.0.6 is a major housing patch: it unifies house ownership limits across all three housing systems, brings custom housing up to parity with classic and static housing (decay, co-owners, bans, Omega Cache removal), fixes a bug that blocked co-owners and friends from using Omega Cache on static and custom houses, adds two new housing deed vendors, reworks boat storage, enlarges two dungeons, and adds a personal Power Hour tracker. It also reworks the house sign display, rescales lockdown/secure/Omega Cache/teleporter limits on the large batch of Fiddler-imported houses to match their actual size, fixes a batch of those houses' ban tiles and missing signs, fixes where you land when banned from a house, fixes a stack-overflow bug when dropping items, and pulls in a batch of server-core engine fixes and a wave of server memory-usage improvements.

## Housing Ownership Limits

- Every account is now limited to a consistent maximum number of houses (2 by default) across custom, classic, and static/prebuilt housing alike, replacing three separate systems that could disagree with each other.

### Player Impact

- You can no longer stack up more houses on one account than the shard allows by exploiting differences between housing types; the limit is now the same no matter which kind of house you buy.

## Custom Housing Overhaul

- Custom house decay/abandonment is enforced again — it had been fully disabled behind the scenes for some time.
- Custom houses gain co-owner management, ban management, and per-friend permission toggles, matching what classic and static houses already had.
- Custom houses can now have an Omega Cache storage container removed, like other house types.
- Building or editing a custom house now costs gold: 500gp for each new house part you add. Parts you'd already placed before this patch are not charged for.

### Player Impact

- Abandoned/inactive custom houses will decay and fall again, freeing up the land.
- You can now assign co-owners, ban unwanted players, and set fine-grained friend permissions on your custom house.
- You can remove an empty Omega Cache container from a custom house if you no longer want it.
- Adding new parts to a custom house now costs 500 gold per part; your existing build is unaffected.

## Classic & Static House Fixes

- Fixed a bug where a decayed static house that got resold could inherit the previous owner's friends, co-owners, and ban list instead of starting clean.
- Fixed a security bug that let a character standing inside a house lock down, secure, raise, or lower items that were actually outside the house's boundary, while incorrectly blocking some legitimate in-house items.
- Fixed multipart furniture deeds not being picked back up correctly, and fixed raise/lower/rotate permission checks misreading furniture ownership, on static housing.
- Fixed demolishing many of the recently renamed legacy and castle-style houses destroying the house without refunding a deed.
- Static houses can now have an Omega Cache container removed, and relinquishing a static house is now blocked if its Omega Cache still holds items (so you can't accidentally lose stored goods).

### Player Impact

- Reselling a decayed static house now gives the new owner a clean friend/co-owner/ban list.
- You can no longer lock down or secure items that aren't actually inside your house, and legitimate in-house items are no longer wrongly blocked.
- Multipart furniture deeds and item permission checks work correctly again on static housing.
- Demolishing an affected legacy/castle house now correctly returns its deed.
- You can remove an unused Omega Cache container from a static house, and can't accidentally relinquish a house while cache items would be lost.

## House Sign Improvements

- House signs (classic, static, and custom) show a cleaner single-line house name and always show the current owner's name, even if you're not the owner and even after the owner has changed their character's name.
- House signs now also show how many Omega Cache containers and teleporters your house has, alongside the existing lockdown/secure counts.
- Custom houses can now view their ban list and remove a ban directly from the sign, and gained the same friend/co-owner permission management screen that classic and static houses already had.

### Player Impact

- House sign info is easier to read and always up to date.
- You can see your Omega Cache and teleporter usage at a glance on the sign.
- Custom house owners can manage bans and friend permissions the same way classic/static house owners can.

## Omega Cache Access Fix

- Fixed a bug where co-owners and friends with storage permission could not actually deposit into or withdraw from a house's Omega Cache container on static or custom housing (only the owner could use it; classic housing was unaffected).

### Player Impact

- Co-owners and friends with the right permission can now use Omega Cache storage on static and custom houses, not just the owner.

## New Housing Vendors

- Added two new vendor types: the Master Builder, who sells the large batch of classic/legacy house deeds, and the Grand Surveyor, who sells custom house deeds. These replace the previous generic deed vendor stock for these items.

### Player Impact

- Look for the Master Builder or Grand Surveyor when shopping for a house deed instead of the general deed vendors.

## Fiddler-Imported House Fixes

- Lockdown, secure, Omega Cache, and teleporter limits on the large batch of Fiddler-imported house deeds now scale with the house's actual size, instead of every one of these houses sharing the same flat limits regardless of how big or small they were. Teleporters are a brand-new stat on these deeds. This only applies to houses built from a deed after this patch — houses you've already built keep their old numbers unless you redeed and rebuild.
- Fixed missing or misplaced ban tiles, and added missing signs, on a batch of 30 previously-broken Fiddler-imported houses.
- One house type, the Large Plaster and Tile House with Outbuildings, was confirmed broken and pulled from sale. If you already own the deed it will still work, but it's no longer sold.
- Fixed a bug where getting banned from a house could drop you on or near the house sign, which on some of these houses could land you back inside the house (or on another ban tile) and immediately re-trigger the ban. You're now dropped just outside the house in a safe, walkable spot instead.

### Player Impact

- Newly built Fiddler-imported houses have lockdown/secure/Omega Cache/teleporter limits that make sense for their size, and a new teleporter limit.
- Ban tiles and signs on the affected batch of houses now work correctly.
- The Large Plaster and Tile House with Outbuildings deed is no longer available for purchase.
- Getting banned from a house now drops you safely outside it instead of risking a stuck-in-a-loop situation.

## Boats

- Boat interiors were reworked: the old tillerman and hold furniture points were removed and replaced with new sail, storage, and weapon-slot furniture points.

### Player Impact

- Boats look and are furnished differently than before; if something on your boat seems off after this patch, let staff know.

## World Changes

- Enlarged the Fire Dungeon and Caverns of Despair regions.

### Player Impact

- Both dungeons now cover more ground for anything tied to their region (guards, spawns, PvP rules, etc.).

## Power Hour

- Added a personal Power Hour on top of the existing server-wide one, with its own status and cooldown tracking.

### Player Impact

- `.ph` now also tells you about your own personal Power Hour (type and time remaining), or when you're next eligible to start one, separate from the server-wide event.

## Quality of Life

- Pets and summoned creatures now show their owner's name when you mouse over them.
- Fixed reconnecting during an active Power Hour sometimes showing the "Power Hour has ended!" message twice.
- Fixed dropping a stack of items (most commonly gold) onto another stack that would push it over the 60,000 stack limit: it now fills the target stack to the max and leaves the rest in your hand/backpack, instead of doing nothing.

### Player Impact

- You can see who owns a pet or summoned creature at a glance.
- No more duplicate Power Hour end messages after reconnecting.
- Overflowing stack drops (like large gold piles) now partially fill instead of silently failing.

## Server Stability and Engine Improvements

- Fixed every character briefly reading as a criminal for a fraction of a second right after the shard starts up.
- Fixed map pins being validated against the wrong bounds, which could reject legitimate pins or accept invalid ones on some maps.
- Fixed a few spellcasting edge cases: an out-of-range spell ID could wrongly cast a different spell, and a failed cast could still consume some reagents.
- Fixed several rare packet-handling bugs that could crash or destabilize the server.
- Custom houses can now have more customizable components than before, since they're no longer limited by a single network packet.

### Player Impact

- Fewer chances of being wrongly flagged as a criminal right after a restart.
- More reliable in-game maps.
- More consistent spellcasting behavior on failed or edge-case casts.
- Better overall server stability.
- Larger, more detailed custom houses are now possible.

## Behind the Scenes

- Fixed several memory leaks across the accounts, email, sysbook, and area-policy systems where server memory usage would slowly grow over uptime.
- Investigated the "stuck mounted after death" appearance bug further and confirmed it's caused by the client, not the server; no server-side fix was possible, so no gameplay change is expected here.
- Removed a substantial amount of unused/dead code from the accounts and housing systems.
- Cleaned up duplicate door decoration entries left over from earlier housing data imports.

### Player Impact

- No direct gameplay change expected from this section — it's server maintenance and cleanup.

## Summary

- Unified the max-houses-per-account limit across custom, classic, and static housing.
- Re-enabled custom house decay and added co-owner/ban/permission management and Omega Cache removal to custom housing; custom house construction now costs 500gp per new part.
- Fixed friend/co-owner/ban list carryover on decayed static houses, a lockdown/secure security bug, broken multipart furniture deeds, and missing deed refunds on several legacy/castle houses.
- Fixed Omega Cache deposit/withdraw access for co-owners and friends on static and custom housing.
- Reworked the house sign display (single-line name, always-current owner name, Omega Cache/teleporter counts); custom houses gained ban-list viewing/removal and the shared friend-permission screen.
- Rescaled lockdown/secure/Omega Cache/teleporter limits on Fiddler-imported house deeds to match house size, fixed ban tiles/signs on 30 of those houses, pulled one broken house type from sale, and fixed where you land when banned from a house.
- Added the Master Builder and Grand Surveyor vendors for housing deeds.
- Reworked boat furniture points (tillerman/hold removed, sails/storage/weapon-slots added).
- Enlarged the Fire Dungeon and Caverns of Despair.
- Added a personal Power Hour with its own status tracking via `.ph`.
- Pets/summons now show their owner in their properties; fixed a duplicate Power Hour end message on reconnect; fixed overflowing stack drops (e.g. large gold piles) doing nothing instead of partially filling.
- Fixed a startup criminal-flag glitch, map pin validation, a couple of spellcasting edge cases, and several packet-handling crash risks; custom houses can now hold more components.
- Various server memory-leak fixes and dead-code cleanup; confirmed the "stuck mounted after death" look is a client-side issue with no server fix available.

Thanks for playing Zuluhotel Omega 3.
