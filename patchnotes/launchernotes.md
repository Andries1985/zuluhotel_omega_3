# Latest Changes
Always check Discord announcements for all patchnotes.

## What Changed

Patch 3.0.2 introduces full Omega Cache support for housing and crafting, plus follow-up updates for Townstones, vault storage, and housing/key reliability.

Players can now place and manage cache containers in houses, use `.cache` commands for fast material control, and craft directly with cache-aware resource handling.

## Omega Cache and Housing

- Cache placement and removal are now tied to house ownership and permission checks.
- House cache slot tracking is active and persists with the house state.
- Ownership transfer and redeed flows include cache-aware safeguards.
- Cache cleanup now supports controlled handling for non-empty stores.

## Townstones and Vault Storage

- Townstone admin/member management was expanded with improved city control paths.
- Town treasury tooling now includes improved status and withdrawal support paths.
- Shared account vault support was added with dedicated command handling.

## Housing and Key Handling

- Static housing sign behavior received follow-up fixes.
- Key storage handling was moved to a dedicated storage area flow.
- Key cleanup during house demolish paths was tightened.

## Crafting Integration

- Major crafting systems now support cache-aware material consumption while preserving backpack behavior.
- Integrated skills include Blacksmithy, Tailoring, Carpentry, Alchemy, Tinkering, Inscription, Cartography, and Cooking.
- Related fletching and bladed material flows are cache-aware.

## Commands and Quality of Life

- `.cache` command support includes open, list, deposit, withdraw, and autodraw workflows.
- `.cache dump` remains staff-only for maintenance use.
- Added and refreshed additional admin/test command support, including townstone tools plus `gotomulti` and `mounttest`.

## Stability and Consistency

- Cache identity rules preserve gameplay-relevant item differences.
- Blacklist and stacking-ignore controls are enforced in cache deposit behavior.
- Command synopsis and include-layer integrations were updated for the expanded command set.

Thanks for playing Zuluhotel Omega 3.
