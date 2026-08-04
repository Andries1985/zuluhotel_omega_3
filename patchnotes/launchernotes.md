# Latest Changes
Always check Discord announcements for all patchnotes.

## What Changed

Patch 3.0.5 adds a large wave of new player housing (including a new Gothic Fortress), fixes several broken teleporter routes and adds new dungeon connections, corrects a bugged NPC color, pulls in a batch of server-core engine improvements focused on login speed and connection stability, fixes two dungeon areas that were cut short, and includes another attempt at fixing ghosts getting stuck in a mounted pose after dying.

## New Housing

- Added roughly 90 new house deeds imported from the Fiddler house tool, each with working lockdown, secure, and Omega Cache storage allowances.
- Added a new purchasable house: the Gothic Fortress, complete with its own banner decorations and properly sized lockdown/secure area.
- Any newly imported house type now automatically gets a correctly sized lockdown/secure area based on its footprint, even without custom setup, so new houses added in the future should work correctly out of the box.

## NPC Fix

- Corrected the Horned Rat's color, which was displaying an incorrect custom hue.

## Teleporter Network

- Added new two-way teleporters connecting Lost City, the Ancient Sewers, and the Vault of the First Dynasty (both levels).
- Disabled a handful of duplicate/incorrect one-way teleporter links around Winterwyn Mining, the Cult of the Serpent Isle, Nexus Outskirts, the Underdark, and the Sosaria mine connections; a corrected replacement link was added where one of these was needed.

## Server Stability and Performance

- Logging in is now much faster, especially right after a restart when many players reconnect at once.
- The server can no longer freeze up shard-wide because of a slow or stalled web/API connection.
- Character name tooltips (title/prefix/suffix/race/guild) now display consistently with what shows on the paperdoll.

## Behind the Scenes

- The server engine gained the ability for certain non-creature world objects to be engaged and damaged directly in combat. No current content uses this yet, so no direct gameplay change is expected.

## Dungeon Fixes

- Fixed the Fire Dungeon and Caverns of Despair 2 areas, which were previously bounded too small and cut off partway through the dungeon. Guard-zone and other area-based rules now apply correctly across the full dungeon.

## Door Decoration Cleanup

- Removed a number of duplicate door decorations that were placed on top of other doors around the world. No new doors were added or removed otherwise.

## Character Death Fix (Continued)

- Made another attempt at fixing the bug where a character's ghost could get stuck showing as mounted (with mount speed) after dying while riding a mount. Not yet confirmed fully fixed — please report it if it still happens.

## Summary

- Added ~90 new house deeds plus a new Gothic Fortress house, with automatic lockdown/secure sizing for future house imports.
- Fixed the Horned Rat's color.
- Added new teleporters linking Lost City, the Ancient Sewers, and the Vault of the First Dynasty; disabled several broken teleporter routes elsewhere.
- Significantly faster logins and improved server stability against stalled connections; fixed inconsistent name tooltip formatting.
- Added (unused-for-now) engine support for combat-targetable non-creature objects.
- Fixed the Fire Dungeon and Caverns of Despair 2 areas being cut off too early.
- Cleaned up duplicate door decorations placed on top of other doors.
- Another attempt at fixing ghosts getting stuck in a mounted pose after death (not yet confirmed fully resolved).

Thanks for playing Zuluhotel Omega 3.
