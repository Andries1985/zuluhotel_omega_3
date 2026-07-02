# Patch Notes - v3.0.2
**Zuluhotel Omega 3 | Beta Shard**  
**Date: [July 2, 2026]**

---

## What Changed

Patch 3.0.2 delivers the full Omega Cache rollout and adds a broader systems pass focused on Townstones, shared vault storage, and housing/sign/key reliability.

Players should notice faster crafting material handling, improved town administration tools, and cleaner account/housing storage behavior.

## Omega Cache and Housing

- Omega Cache containers can now be placed and managed in houses with ownership/permission checks.
- House cache slot tracking is active and tied to the house state.
- Ownership transfer and redeed flows now include cache-aware safeguards.
- Cache removal supports controlled cleanup behavior for non-empty cache stores.

## Townstones and City Management

- Townstone command/admin support has been expanded with clearer membership and treasury management tooling.
- New and updated townstone flows include member cleanup/removal, poll reset support, and city treasury status/withdraw paths.
- Townstone startup/bootstrap support was expanded to keep city data and runtime state in sync.

## Crafting Integration

- Core crafting now supports cache-aware material consumption while preserving normal backpack behavior.
- Integrated systems include Blacksmithy, Tailoring, Carpentry, Alchemy, Tinkering, Inscription, Cartography, and Cooking.
- Related item flows, including bladed/fletching support paths, are cache-aware.

## Vault and Storage Updates

- Added shared account vault command flow support with dedicated open-vault handling.
- Key storage handling was moved to dedicated storage areas and key cleanup on housing-demolish paths was tightened.

## Commands and Quality-of-Life

- Added `.cache` command support for cache interaction workflows.
- Added cache control options for listing, deposit/withdraw, open/access, and autodraw behavior.
- Staff-only maintenance path remains available through `.cache dump`.
- Added/updated admin tooling including townstone management commands and test utilities such as `gotomulti` and `mounttest`.

## Stability and Consistency

- Cache identity and stacking rules now preserve gameplay-relevant item differences.
- Blacklist and stacking-ignore controls are enforced in deposit behavior.
- Static housing sign behavior and related destroy/selection handling received follow-up fixes.
- Command synopsis and support include plumbing were updated for the expanded command set.

---

## Player Highlights

If you only want the short version, this patch gives you:

- Full Omega Cache support in housing and crafting.
- Expanded townstone admin/member/treasury support.
- Shared account vault support and improved key storage cleanup behavior.
- Faster material access with less backpack micromanagement.
- Better consistency and safeguards around cache ownership, transfer, and cleanup.

---

Thanks for playing Zuluhotel Omega 3.
