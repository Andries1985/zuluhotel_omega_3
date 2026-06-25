# Latest Changes
Always check Discord announcements for all patchnotes.

## What Changed

Omega Cache has now been fully integrated into Omega 3 housing and crafting flows.

Players can place and use Omega Cache containers in houses, deposit and withdraw stackable materials, and use `.cache` commands for faster storage access.

## Omega Cache and Housing

- Omega Cache placement and removal now work through house ownership and permissions.
- House cache slot tracking is active and tied to each house.
- Cache removal now supports explicit purge-confirmation behavior for non-empty cache stores.
- Ownership transfer and redeed flows now include cache-aware safety handling.

## Crafting Integration

- Core crafting now supports cache-aware material consumption while keeping normal backpack flow intact.
- Integrated skills include Blacksmithy, Tailoring, Carpentry, Alchemy, Tinkering, Inscription, Cartography, and Cooking.
- Edge item flows such as fletching and bladed arrow combinations are also cache-aware.

## Commands and Quality of Life

- `.cache` command set is available, including open, deposit, list, withdraw, and autodraw controls.
- `.cache dump` remains staff-only.
- Command help/synopsis entries were regenerated for the new cache commands.

## Stability and Integrity

- Cache identity rules now preserve gameplay-significant item differences.
- Blacklist and stacking compatibility rules are enforced consistently in cache deposit behavior.
- Updated scripts compile clean for all Omega Cache touched files.

Thanks for playing Zuluhotel Omega 3.
