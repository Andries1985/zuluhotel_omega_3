# Fishing Quests — Quest 100-107

This is the user's own separate quest-line (not built by Claude) —
documented here so it isn't lost, same as the ritual quest doc. Lives in
the same `quests.cfg` file as the ritual quests (id range `100-199`
reserved for fishing per the file's own header doc: "100-103 fish line,
104-107 crustacean line").

## Givers

- **Fish Monger** (`NpcTemplate fishmonger`, `config/npcdesc.cfg`) —
  `script :quests:ai/fishmonger`, objtype `0x190`, `Equip fisherman`,
  `CProp QuestGiverTag sfishmonger`. Offers Quests 100-103.
- **Lobsterman** (`NpcTemplate lobsterman`) — `script :quests:ai/lobsterman`,
  objtype `0x191`, `Equip fisherman`, `CProp QuestGiverTag slobsterman`.
  Offers Quests 104-107.

Both: invulnerable, good-aligned, human, no loot. Neither has a placed
x/y/z — user places them manually, same as every ritual giver.

Unlike the ritual quests, all 8 are **gated by `SkillId 18`/`MinSkill`**
(Fishing skill) before a giver will even offer them, and all 8 are
**`Repeatable 1`** with a `RepeatCooldown` (1 day for the tier-1 quest, 3
days for tier-2, 7 days for tier-3/weight quests) — none use `TurnInItem`
at all, since there's no boss/trophy step, just a live catch count.

## Fish line (Fish Monger)

| Quest | Name | Requirement | Reward | Cooldown |
|---|---|---|---|---|
| 100 | A Line in the Water | Catch 15 fish (any tier), MinSkill 50 | 450g + `RareFishLure` (`0x30385`) | 1 day |
| 101 | Something Worth the Hook | Catch 5 Rare fish, MinSkill 100 | 1200g + `LegendaryFishLure` (`0x30386`) | 3 days |
| 102 | The One That Doesn't Get Away | Catch 1 Legendary fish, MinSkill 130 | 2250g + `LegendaryFishLure` (`0x30386`) | 7 days |
| 103 | For the Record Books | Land a fish ≥180 stones, MinSkill 130 | 2250g + `BigFishLure` (`0x30387`) | 7 days |

**Data note**: Quest 101 (a Rare-tier requirement) rewards `0x30386`
(`LegendaryFishLure`), not `0x30385` (`RareFishLure`) — confirmed by
direct read of `quests.cfg`, not a transcription error. Worth checking
whether that's intentional (a Legendary lure as a bonus for a Rare-tier
quest) or a copy-paste slip from Quest 102 that should be `0x30385`.

## Crustacean line (Lobsterman)

| Quest | Name | Requirement | Reward | Cooldown |
|---|---|---|---|---|
| 104 | Traps Need Tending | Catch 20 crustaceans (any tier), MinSkill 50 | 450g + `RareCrustaceanLure` (`0x30388`) | 1 day |
| 105 | Something With Bite | Catch 5 Rare crustaceans, MinSkill 100 | 1200g + `LegendaryCrustaceanLure` (`0x30389`) | 3 days |
| 106 | The Big Claw | Catch 1 Legendary crustacean, MinSkill 130 | 2250g + `LegendaryCrustaceanLure` (`0x30389`) | 7 days |
| 107 | Heaviest in the Bay | Land a crustacean ≥180 stones, MinSkill 130 | 2250g + `BigCrustaceanLure` (`0x3038A`) | 7 days |

**Same data note applies**: Quest 105 (Rare-tier) rewards `0x30389`
(`LegendaryCrustaceanLure`) rather than `0x30388`
(`RareCrustaceanLure`) — same pattern as Quest 101, same "worth
double-checking" flag.

## How it works mechanically

Unlike the kill-based ritual quests (advanced by `questdeath.inc`'s
tagged-NPC-death hook, which needs a pre-tagged spawn instance), catch
quests have no NPC instance to tag — every cast/trap-pull is a fresh
random roll from `catchtable.cfg`/`crustaceantable.cfg`. So instead:

- `ResolveFishCatch()` (`pkg/std/fishing/fishing.inc:297`) and
  `CollectTrap()` (`pkg/std/fishing/crustaceantrap.inc:215`) each call
  `QP_ProcessFishCatch(character, "CatchFish"|"CatchCrustacean", tier, weight)`
  directly at the moment a catch resolves.
- `QP_ProcessFishCatch()` (`pkg/opt/quests/include/questfishing.inc:16`)
  loops the player's active quests, and for any objective whose
  `ObjKind<n>` matches, checks `ObjTier<n>` (or "Any") and `ObjMinWeight<n>`
  (if set) before advancing it.
- **Trophy weight**: `RollTrophyWeight()` (`fishing.inc:303`) is
  `Min(Random(225)+1, Random(225)+1)` — historical OSI fish-weight range
  is 1-225 stones, and taking the min of two rolls skews the result low so
  a near-record weight stays rare. Only rolled on Rare/Legendary catches
  (never Regular), matching the `ObjMinWeight` doc comment in `quests.cfg`.
- **Big lures** (`BigFishLure`/`BigCrustaceanLure`) specifically help the
  weight-record quests (103/107): a Big lure rolls `RollTrophyWeight()`
  twice and keeps the higher result. Per the code's own comment, this was
  tuned to "closer to 8%" odds of clearing the 180-stone bar (an earlier
  `Max()`-based version "tested too easy at 37%").

## The 6 quest-reward lures

`pkg/std/fishing/itemdesc.cfg:4422-4523` — explicitly commented
"quest rewards only - not craftable/vendor-bought", objtypes
`0x30385`-`0x3038A`. All `Script lure/use`, `CProp charges i50`
(50 uses), `CProp Undyable i1`. Applying one (`pkg/std/fishing/lure/use.src`)
attaches its `LureKind`/`LureTier` to a fishing pole (fish lures) or a
deployed crustacean-trap buoy (`0x3037f` — crustacean lures must target
the deployed buoy, not the trap item itself, since the trap doesn't
persist as an item through deployment). One lure loaded at a time,
overwriting whatever was there before.

## Testing without fishing: `.questcatch`

GM-only tool, `pkg/opt/quests/textcmd/gm/questcatch.src`:

```
.questcatch <CatchFish|CatchCrustacean> [tier] [weight] [times]
  tier defaults to Regular, weight defaults to 0, times defaults to 1.
```

Calls `QP_ProcessFishCatch()` directly against yourself, `times` times,
bypassing the real fishing/trap system entirely — the fastest way to
validate any of Quest 100-107's progression without actually fishing.

## Related but not quest-specific

**Taxidermy** (`pkg/std/fishing/taxidermy.src`) is general fishing-system
content, not gated by any quest, but built on the same `TrophyWeight`
property the weight-record quests check — it requires the targeted catch
to have a `TrophyWeight` ObjProperty before mounting it, otherwise
"That's not a trophy-worthy catch."
