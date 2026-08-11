# Latest Changes
Always check Discord announcements for all patchnotes.

## What Changed

Patch 3.0.6 is a major housing patch: it unifies house ownership limits across all three housing systems, brings custom housing up to parity with classic and static housing (decay, co-owners, bans, Omega Cache removal), fixes a bug that blocked co-owners and friends from using Omega Cache on static and custom houses, adds two new housing deed vendors, reworks boat storage, enlarges two dungeons, and adds a personal Power Hour tracker. It also pulls in a batch of server-core engine fixes and a wave of server memory-usage improvements.

## Housing Ownership Limits

- Every account is now limited to a consistent maximum number of houses (2 by default) across custom, classic, and static/prebuilt housing alike, replacing three separate systems that could disagree with each other.

## Custom Housing Overhaul

- Custom house decay/abandonment is enforced again — it had been fully disabled behind the scenes for some time.
- Custom houses gain co-owner management, ban management, and per-friend permission toggles, matching what classic and static houses already had.
- Custom houses can now have an Omega Cache storage container removed, like other house types.
- Building or editing a custom house now costs gold: 500gp for each new house part you add. Parts you'd already placed before this patch are not charged for.

## Classic & Static House Fixes

- Fixed a bug where a decayed static house that got resold could inherit the previous owner's friends, co-owners, and ban list instead of starting clean.
- Fixed a security bug that let a character standing inside a house lock down, secure, raise, or lower items that were actually outside the house's boundary, while incorrectly blocking some legitimate in-house items.
- Fixed multipart furniture deeds not being picked back up correctly, and fixed raise/lower/rotate permission checks misreading furniture ownership, on static housing.
- Fixed demolishing many of the recently renamed legacy and castle-style houses destroying the house without refunding a deed.
- Static houses can now have an Omega Cache container removed, and relinquishing a static house is now blocked if its Omega Cache still holds items (so you can't accidentally lose stored goods).

## Omega Cache Access Fix

- Fixed a bug where co-owners and friends with storage permission could not actually deposit into or withdraw from a house's Omega Cache container on static or custom housing (only the owner could use it; classic housing was unaffected).

## New Housing Vendors

- Added two new vendor types: the Master Builder, who sells the large batch of classic/legacy house deeds, and the Grand Surveyor, who sells custom house deeds. These replace the previous generic deed vendor stock for these items.

## Boats

- Boat interiors were reworked: the old tillerman and hold furniture points were removed and replaced with new sail, storage, and weapon-slot furniture points.

## World Changes

- Enlarged the Fire Dungeon and Caverns of Despair regions.

## Power Hour

- Added a personal Power Hour on top of the existing server-wide one, with its own status and cooldown tracking. `.ph` now also tells you about your own personal Power Hour (type and time remaining), or when you're next eligible to start one.

## Quality of Life

- Pets and summoned creatures now show their owner's name when you mouse over them.
- Fixed reconnecting during an active Power Hour sometimes showing the "Power Hour has ended!" message twice.

## Server Stability and Engine Improvements

- Fixed every character briefly reading as a criminal for a fraction of a second right after the shard starts up.
- Fixed map pins being validated against the wrong bounds, which could reject legitimate pins or accept invalid ones on some maps.
- Fixed a few spellcasting edge cases: an out-of-range spell ID could wrongly cast a different spell, and a failed cast could still consume some reagents.
- Fixed several rare packet-handling bugs that could crash or destabilize the server.
- Custom houses can now have more customizable components than before, since they're no longer limited by a single network packet.

## Behind the Scenes

- Fixed several memory leaks across the accounts, email, sysbook, and area-policy systems where server memory usage would slowly grow over uptime.
- Investigated the "stuck mounted after death" appearance bug further and confirmed it's caused by the client, not the server; no server-side fix was possible, so no gameplay change is expected here.

## Dungeon Fixes

- Fixed the Fire Dungeon and Caverns of Despair 2 areas, which were previously bounded too small and cut off partway through the dungeon. Guard-zone and other area-based rules now apply correctly across the full dungeon.

## Door Decoration Cleanup

- Removed a number of duplicate door decorations that were placed on top of other doors around the world. No new doors were added or removed otherwise.

## Character Death Fix (Continued)

- Made another attempt at fixing the bug where a character's ghost could get stuck showing as mounted (with mount speed) after dying while riding a mount. Not yet confirmed fully fixed — please report it if it still happens.

## Summary

- Unified the max-houses-per-account limit across custom, classic, and static housing.
- Re-enabled custom house decay and added co-owner/ban/permission management and Omega Cache removal to custom housing; custom house construction now costs 500gp per new part.
- Fixed friend/co-owner/ban list carryover on decayed static houses, a lockdown/secure security bug, broken multipart furniture deeds, and missing deed refunds on several legacy/castle houses.
- Fixed Omega Cache deposit/withdraw access for co-owners and friends on static and custom housing.
- Added the Master Builder and Grand Surveyor vendors for housing deeds.
- Reworked boat furniture points (tillerman/hold removed, sails/storage/weapon-slots added).
- Enlarged the Fire Dungeon and Caverns of Despair.
- Added a personal Power Hour with its own status tracking via `.ph`.
- Pets/summons now show their owner in their properties; fixed a duplicate Power Hour end message on reconnect.
- Fixed a startup criminal-flag glitch, map pin validation, a couple of spellcasting edge cases, and several packet-handling crash risks; custom houses can now hold more components.
- Various server memory-leak fixes and dead-code cleanup; confirmed the "stuck mounted after death" look is a client-side issue with no server fix available.

Thanks for playing Zuluhotel Omega 3.
