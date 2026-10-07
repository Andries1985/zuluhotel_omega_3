# itemutil.inc audit — fix log

2026-10-04, branch `Patch-3.1.3`. Kept by Claude.

Separate file from `code-review-fixlog-20260921.md`: that review closed on 2026-09-23 and has
been committed since, so appending here keeps its companion `.diff` regenerable.

**Status of everything below: edited in the working tree, not committed, not compiled and not
tested in game by Claude.** The exact line-level changes are in `itemutil-audit-fixlog-20261004.diff`.

Scope: every function in `scripts/include/itemutil.inc` read in full, each one grepped for callers
across `.src`/`.inc`/cfg/python, and every hardcoded objtype, graphic and landtile list compared
against the shard's own data — 6,019 itemdesc blocks across 68 `itemdesc.cfg` files, the four craft
configs, `config/food.cfg`, and the `regions/*.cfg` resource tile lists.

Decisions came from the picker page (db doc `plan/picks_itemutil_20261004`).

## How to read an entry

File and line, what the code did (**Was**), what it does now (**Now**). **Players** is there only
when someone in game will notice. Every edit site carries a dated `// 2026-10-04:` comment.

---

## 1. Dead code removed (done before the picks)

Nine functions had no callers anywhere in the repo. Each is commented out in place, with a line
saying why. Nothing else references them, so nothing changed behaviourally.

| Function | Why it was dead |
| --- | --- |
| `GetPossiblePropertyNames` | unused since the initial commit |
| `ConsumeObjType` | superseded by `resourcemanager.inc` (`ConsumeResource` / `ConsumeFromBackpack`) |
| `CreateItemAt` | started `:summoning:itemappear`, which does not exist in the repo |
| `CreateMagicCircleAround` | with `PlayMagicCircleEffect`, the only user of `:summoning:magiccircleappear` |
| `PlayMagicCircleEffect` | hardcoded 1500ms variant of the above |
| `CreateWaterfall` | with `PlayWaterfallEffect`, the only user of `:summoning:waterfallappear` |
| `PlayWaterfallEffect` | hardcoded 5000ms / sfx 0x218 variant of the above |
| `FindRootItemInContainer` | superseded by `FindObjtypeInContainer( .., FINDOBJTYPE_ROOT_ONLY )` |
| `ResetAllHitscriptPropsExcep` | nothing bulk-clears those hitscript cprops any more |

Side effect worth knowing: `pkg/opt/summoning/magiccircleappear.src` and `waterfallappear.src` now
have no callers at all. They are left in place, untouched.

---

## 2. The six new bows can be refined (page item 1, 18)

- File: `scripts/include/itemutil.inc:1012` (`IsWoodEquipment`)
- Was: `woodstuff` listed five graphics twice (`0x0f4f`, `0x13fd`, `0x1403`, `0x6050`, `0xb201`)
  and omitted the six archery weapons added 2026-08-23. `refiningitem.src` gates on this list, so a
  Refining Varnish answered "Varnishes can only refine wooden weapons and armor." for all six.
- Now: deduplicated, and `0xa915` Shortbow, `0x26c2` CompositeBow, `0x26c3` RepeatingCrossbow,
  `0x27a5` Yumi, `0x2d1e` ElvenCompositeLongbow and `0xb4dc` SkullCrossbow added.
- **Players:** the six newer bows now accept a Refining Varnish like every other bow.

## 3. Leather footwear and the carved masks can be refined (page item 2)

- File: `scripts/include/itemutil.inc:1031` (`IsLeatherEquipment`)
- Was: of the tailoring items whose `MaterialType` is `Hide`, every armour piece was listed but the
  Shoes category and the Leather Misc masks were not — so a Refining Compound refused them. The
  masks were already counted as leather armour by `GetLeatherArmorGraphics`, so the two lists in
  this same file disagreed with each other.
- Now: added `0x170B` LeatherBoots, `0x170D` Sandals, `0x170F` Shoes, `0x1711` ThighBoots, and the
  ten mask graphics `0x141B`/`0x141C`, `0x1545`–`0x154C`.
- **Players:** leather boots, sandals, shoes, thigh boots and the five carved masks now accept a
  Refining Compound.

## 4. The other six tinker jewellery pieces count as jewellery (page item 3)

- File: `scripts/include/itemutil.inc:1074` (`IsJewel`)
- Was: `tinker.cfg` makes twelve jewellery pieces; only `0x1085`–`0x108A` and the wristwatch
  `0x5015` were listed. The second set was invisible to the ritual jewel checks in
  `pkg/opt/rituals/include/rituals.inc` (four branches), to `vitalInfusion.src`'s `skilladv` read,
  and to the equipment block in `megacliloc/itemdata.src:118`.
- Now: `0x1F05` beaded necklace, `0x1F06` bracelet2, `0x1F07` earrings2, `0x1F08` necklace3,
  `0x1F09` ring2 and `0x1F0A` silver necklace added.
- **Players:** those six can be used in jewel rituals and show the full tooltip.

## 5. Poisonable food comes from food.cfg (page item 4)

- File: `scripts/include/itemutil.inc:457` (`IsConsommable`), new `GetEdibleObjtypes` at `:487`,
  `use cfgfile;` added at `:3`
- Was: nineteen hardcoded objtype ranges. Measured against `config/food.cfg`, they recognised 79 of
  its 138 distinct edible objtypes. The 59 misses included the fish steaks (`0x09cc`–`0x09cf`), the
  raw fish (`0x0dd6`–`0x0dd9`), the fruit at `0x1a92`–`0x1a96` and the custom foods at
  `0x30128`–`0x3012b`. `poisoning.inc:104` gates `PoisonFood` on this, so poisoning any of them
  said "You can't poison that."
- Now: reads `config/food.cfg` — all four of its blocks (veggie, meat, all, cooked) — and keeps the
  old potion ranges (`0xdc01`–`0xdc03`, `0xdc0b`–`0xdc16`) and the custom dishes
  (`0xc900`–`0xc947`), which are not in food.cfg but are meant to be poisonable.
- Note: `GetConfigIntArray`, not `GetConfigStringArray` — the engine parses config ints with
  `std::stoi( .., 0 )` (`cfgmod.cpp:532`), so food.cfg's `0x09d0` form comes back as a number.
- Cached in a file-scope `_edible_objtypes` built on first use, not a file-scope initialiser, which
  would re-read the config at the start of every one of the 77 scripts that include this file.
- **Players:** every food in food.cfg can now be poisoned.

## 6. Chaos and Order shields are class-restricted (page item 8)

- File: `scripts/include/itemutil.inc:231` (`GetShieldGraphics`)
- Was: the eleven stock shields. `0x1bc3` Chaosshield and `0x1bc4` Ordershield are real equippable
  `Armor` blocks in the combat itemdesc and were in no list, so Bard, Mage, Mystic Archer, Ranger,
  Thief and Bladesinger could all carry one.
- Now: both graphics added. That also covers `0x86df` Chaosshieldguard, `0x86ef` Ordershieldguard
  and `0x8260` TourneyShield, which reuse them.
- Checked for the page's caveat: `0x7ce2 ebardshield` also uses graphic `0x1bc3` and becomes
  restricted too. It has no creation path anywhere in the repo — no loot table, no vendor, no
  script that makes one — so nothing obtainable is affected. Your pick, with that noted.
- **Players:** the six shield-restricted classes can no longer equip a Chaos or Order shield.

## 7. Female studded leather is class-restricted (page item 9)

- File: `scripts/include/itemutil.inc:205` (`GetStuddedLeatherArmorGraphics`)
- Was: the male set only, so Bladesinger and Paladin — both barred from studded leather — could wear
  `0x1C02` FemaleStudded and `0x1C0C` StuddedBustier.
- Now: both added. Covers `0x8249` SylvianFemaleStudded and `0x8275` TourneyFemaleStudded, which
  share graphic `0x1C02`.
- **Players:** Bladesingers and Paladins can no longer wear studded leather in female form.

## 8. Thieves no longer get a free pass on the Navar shield (page item 10)

- File: `scripts/include/itemutil.inc:244` (`GetShieldGraphicsThief`)
- Was: `GetShieldGraphics` minus the buckler `0x1b73` and minus `0x982a` NavarBloodyBarrier, which
  every other shield-restricted class was barred from.
- Now: `0x982a` added. The buckler stays off the list deliberately, with a comment saying so.

## 9. The orc helm counts as plate (page item 12)

- File: `scripts/include/itemutil.inc:170` (`GetPlatemailArmorGraphics`)
- Was: `0x1f0b` Orchelm was counted as metal by `IsMetalEquipment` but sat in no family list, so
  classes barred from plate could still wear a metal helm.
- Now: added. Covers `0x32d` KoboldOrchelm, same graphic.
- **Players:** Mage, Thief, Bard, Mystic Archer and Ranger can no longer wear an orc helm.

## 10. Three objtypes are no longer both an ingot and an ore (page item 13)

- File: `scripts/include/objtype.inc:435`
- Was: `UOBJ_SHING_INGOT` (`0xc530`), `UOBJ_LEVIATHAN_INGOT` (`0xc531`) and `UOBJ_SANCTUARY_INGOT`
  (`0xc532`) sat inside `UOBJ_ORE3_START`..`UOBJ_ORE3_END` (`0xc530`–`0xc546`) as well, so
  `IsIngot()` and `IsOre()` both returned 1 for all three. No live items — all three were reserved
  with no itemdesc entry — but it would have broken the day those ingots shipped.
- Now: the three constants removed per your note, and `UOBJ_INGOTS2_END` moved to `0xc52f`
  (`UOBJ_VULCAN_INGOT`). The three slots belong to the ore range alone. Nothing outside
  `objtype.inc` referenced the removed names.

## 11. DupeItem keeps the use script, description and price (page item 14)

- File: `scripts/include/itemutil.inc:89`
- Was: copied fourteen members plus every cprop. `usescript` — the second most script-assigned item
  member in the repo, 36 sites — was not among them, nor `desc`, `sellprice` or `amount`.
- Now: all four copied when set. Callers: the `.iteminfo` clone (two sites), inscription (two),
  `tailoringfunctions.inc`, and `explosionlauncherscript.src`.
- **Players:** a GM-cloned item keeps its script-assigned use script and description.

## 12. IsEquipped renamed to EquipIfNeeded (page item 15)

- Files: `scripts/include/itemutil.inc:516`, `pkg/std/lumberjacking/lumberjack.src:30`,
  `pkg/std/mining/mining.src:48`
- Was: a function named `IsEquipped` that, if the item was not already in hand 1 or 2, called
  `EquipItem` and returned its result. Both call sites rely on that — the axe and the shovel are
  equipped by the "check" — so the behaviour is load-bearing and the name was the problem.
- Now: renamed, both callers updated, with a comment at each site. No behaviour change. Grep
  confirms no `IsEquipped(` call remains.

## 13. The two container searches say which depth they use (page item 16)

- Files: `scripts/include/itemutil.inc:570` (`IsInContainer`), `:970` (`IsOnPlayer`),
  `pkg/multis/house/multiDeed/use.src:387`
- Was: two functions doing the same serial search at different depths — `IsInContainer` with
  `ENUMERATE_ROOT_ONLY`, `IsOnPlayer` recursive — with nothing in either name saying which, across
  33 call sites. `multiDeed/use.src` defines a third, local `IsInContainer` that recurses, the
  opposite of the shared function by that name.
- Now: header comments on all three naming the depth and pointing at the counterpart. No code
  change. `multiDeed/use.src` does not include `itemutil`, so there is no collision.

## 14. Redundant entries removed (page item 19)

- Files: `scripts/include/itemutil.inc:664` (`IsHide`), `:1052` (`IsSign`)
- `IsHide` named `0x702f` and `0x7030` explicitly, both already inside
  `UOBJ_HIDES_START`..`UOBJ_HIDES_END` (`0x7020`–`0x7030`) on the next branch. Dropped.
- `IsSign` cased `0xbd0` and `0xbd2`, both already inside its own `0xba3`–`0xc0e` range. Dropped.
- No behaviour change. Checked separately: every house sign in the repo — regular, custom and static
  housing, ten objtypes — is matched by `IsSign`, so nothing is missing from it.

## 15. DestroyTheItem uses the named curse level (page item 20)

- File: `scripts/include/itemutil.inc:121`
- Was: `SetObjProperty( item, "Cursed", 3 )`.
- Now: `SetCurseLevel( item, SETTING_CURSE_LEVEL_REVEALED_CAN_UNEQUIP )`, reading the level through
  `GetCurseLevel`. Both were already defined in this file and the constants already included.
  Same value, no behaviour change.

## 16. The dead duplicate of these helpers is gone (page item 17)

- Deleted: `pkg/utils/itemUtils/include/itemtypes.inc` (1,601 lines, 53 functions)
- It redefined `IsIngot`, `IsLog`, `IsOre`, `IsReagent`, `IsSign` and `IsInContainer` — six names
  `itemutil.inc` already provides — and the only two `include` lines that would have pulled it in
  were already commented out (`customhousing/sign.src:10`, `signcontrol.src:7`), so it compiled into
  nothing. `guilds.inc:12` carried a note explaining that it was deliberately not included for
  exactly that reason.
- Also updated so nothing points at a file that no longer exists: those two commented include lines,
  and the `guilds.inc` note.
- A copy is kept outside the repo in this session's scratchpad.

---

## 17. The three resource predicates now come from their configs (page items 5, 6, 7)

All three were hardcoded ranges that the 2026-08-15 resource-config audit left behind. Each is now
the config's own list, grouped into ranges, with each entry's name carried over from the config as a
trailing comment — so a line can be deleted here to take something out of harvesting without
touching the resource config, which is what you asked for.

### 17.1 IsTree ← `regions/wood.cfg`

- File: `scripts/include/itemutil.inc:745`
- Was: four hardcoded ranges covering 208 of the config's 574 objtypes. The 2026-08-15 audit grew
  `wood.cfg` from 170 to 574, verified against tiledata, and the predicate never followed — so two
  thirds of the configured trees had wood units allocated and regrowing that no axe could harvest.
  It also accepted `0xc9f`–`0xca5`, `0xca7`, `0xcac`–`0xcc7` and `0xd37`–`0xd38`, which the config
  does not list.
- Now: all 574 objtypes as 115 named ranges. The previously-accepted-but-unconfigured objtypes are
  gone with it.
- Sole gate in `pkg/std/lumberjacking/lumberjack.src` (five sites) and `scripts/items/bladed.src`
  (five sites), so this widens what a blade can carve as well as what an axe can chop.
- **Players:** the later-expansion trees (`0x309c`–`0x30de`, `0x39a3`–`0x3aef`, `0xa65c`–`0xa716`),
  the vines, the fallen logs and the rest of the 2026-08-15 additions become harvestable.

### 17.2 IsMinable ← `regions/ore.cfg`

- File: `scripts/include/itemutil.inc:611`
- Was: eighteen hardcoded ranges covering 129 of the config's 199 landtiles. On the 70 it missed,
  mining said "You can't mine or dig anything there." It also accepted 13 tiles the config does not
  list, where mining ran against no resource region.
- Now: all 199 landtiles as 24 named ranges (rock and cave). The `othertype` branch for the
  cave-floor statics `0x053b`–`0x0553` (except `0x0550`) is unchanged and still tested first.
- **Players:** the cave and rock tiles added by the audit (`0x24a`–`0x26d`, `0x2bc`–`0x2cb`,
  `0x63b`–`0x63e`, `0x7c2`–`0x7c4`, `0x7d1`–`0x7d4`, `0x9ec`–`0xa03`) become mineable.

### 17.3 IsSand ← `regions/sand.cfg`

- File: `scripts/include/itemutil.inc:694`
- Was: seven hardcoded ranges that matched the config only partly — 44 configured sand tiles
  rejected, 54 accepted tiles not in the config at all.
- Now: all 187 landtiles as 21 named ranges.
- **Players:** the beach and desert tiles added by the audit become diggable for sand.

### 17.4 Overlap check after the change

The three mining predicates are tested in order `IsSwamp`, `IsMinable`, `IsSand` in
`mining.src:82-92`, so an overlap silently gives the earlier branch the tile.

| pair | overlap |
| --- | --- |
| IsMinable ∩ IsSand | none (the old `0x122`–`0x125` clash is gone) |
| IsSwamp ∩ IsSand | none |
| IsSwamp ∩ IsMinable | `0x250` |

`0x250` is in `IsSwamp`'s hardcoded list and inside `ore.cfg`'s cave block, and `IsSwamp` wins, so
it digs clay and never ore — the same as before this change. `IsSwamp` is the one tile predicate
with no resource config of its own (clay has `regions/clay.cfg`, which was not compared here), so
it was left alone. Flagged, not fixed.

---

## Not changed, by your pick

- `GetGraphicsCrafter` — left as the empty placeholder it was (page item 11).
- `FindItemInContainer` — you asked to hear more first (page item 21). Untouched.

## Edit-site index

| File | Lines |
| --- | --- |
| `scripts/include/itemutil.inc` | 3, 14, 40, 57, 73, 106, 123, 153, 178, 212, 237, 250, 270, 459, 512, 606, 666, 689, 739, 900, 914, 928, 967, 1014, 1033, 1053, 1093 (every `// 2026-10-04:` line) |
| `scripts/include/objtype.inc` | 435 |
| `pkg/std/lumberjacking/lumberjack.src` | 30 |
| `pkg/std/mining/mining.src` | 48 |
| `pkg/multis/house/multiDeed/use.src` | 387 |
| `pkg/opt/guilds/include/guilds.inc` | 12 |
| `pkg/multis/customhousing/sign.src` | 10 |
| `pkg/multis/customhousing/signcontrol.src` | 7 |

## Files deleted

| File | Why |
| --- | --- |
| `pkg/utils/itemUtils/include/itemtypes.inc` | dead; nothing included it; duplicated six `itemutil.inc` helpers |

## Docs and conventions changed

| File | Change |
| --- | --- |
| `.claude/skills/escript-gotchas/SKILL.md` | new gotcha 4a: a `ConfigFile` is iterable (`foreach elem in cfg`, named blocks only), `GetConfigIntArray` parses the `0x09d0` form via `std::stoi( .., 0 )` while `GetConfigStringArray` does not, and an old include may be missing `use cfgfile;` |
| `.claude/subagent-briefing.md` | same, one line in the high-frequency subset |

---

## 18. FindItemInContainer retired for the engine call (page item 21), 2026-10-05

Your pick after the explanation: swap the call sites, not just the helper's body.

- Files: the nine listed below, plus `scripts/include/itemutil.inc:138` (the helper itself)
- Was: `FindItemInContainer` enumerated the entire container tree into a script-side array and
  compared objtypes one at a time in interpreted code. 23 live call sites used it; 27 sites
  elsewhere in the repo already called the engine instead.
- Now: all 23 call `FindObjtypeInContainer( container, objtype )` directly — 50 engine call sites
  in the repo. The helper has no callers left and is commented out in place with a note saying why.

Checked against the engine source before the swap, because two things had to match:

| | old helper (`EnumerateItemsInContainer`, no flags) | engine call (`FindObjtypeInContainer`, flags 0) |
| --- | --- | --- |
| recursion | into sub-containers (`containr.cpp:324`) | into sub-containers (`containr.cpp:477`) |
| locked sub-containers | skipped unless `ENUMERATE_IGNORE_LOCKED` (`containr.cpp:328`) | skipped unless `FINDOBJTYPE_IGNORE_LOCKED` (`containr.cpp:484`) |
| not found | unset var | `BError`, and `BError::isTrue()` is false (`berror.cpp:95`) |
| not a container | `foreach` over the error finds nothing, returns unset | `BError( "That is not a container" )` — also falsy |

So every `if( !x )` / `if( x )` / `x && y` guard behaves identically, and all 23 sites use exactly
those forms — checked one by one, none compares the result to a literal or dereferences it unguarded.
All 23 pass a real container (`character.backpack`, `who.backpack`, `user.backpack`, or the `bag`
program parameter). Every one of the nine files already has `use uo;`.

**The one real behavioural difference:** the helper walked depth-first, so a match inside a
sub-container could be returned before a match sitting loose in the backpack. The engine checks the
whole top level first (`find_toplevel_objtype`) and only then recurses. Where a container holds a
match at both depths, callers now get the top-level one. For these call sites — find an empty
bottle, a reagent, a mortar, a spellbook — that is the better answer, and it is what the 27
pre-existing engine call sites already did. It is still a change in *which* item comes back.

| File | Calls |
| --- | --- |
| `pkg/opt/alchemyplus/alchemyplus.src` | 5 live + 1 already commented out |
| `pkg/opt/alchemyplus/alchemyplus toad.src` | 5 |
| `scripts/textcmd/player/cast.src` | 5 |
| `pkg/std/poisoning/poisoning.inc` | 2 |
| `pkg/std/poisoning/poisoning.src` | 2 |
| `pkg/std/alchemy/alchemyfunctions.inc` | 1 |
| `pkg/opt/shilitems/infinitegems.src` | 1 |
| `pkg/opt/shilitems/infinitenormals.src` | 1 |
| `pkg/opt/shilitems/infinitepagans.src` | 1 |

Each file carries one dated comment above its first swapped call rather than a comment per site.

**Note on scope:** 10 of the 23 are in `pkg/opt/alchemyplus/`, the Tamla potion work that the
2026-09-21 review deliberately left alone as someone else's in-progress change. Those two files are
edited here on your pick. `alchemyplus toad.src` is a variant copy of `alchemyplus.src` with the
same five sites and a current `.ecl`, so it is live and not a stale backup.
