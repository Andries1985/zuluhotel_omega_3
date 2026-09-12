# Crafting Station Graphic ID Audit

Audit of ~55 crafting-station/tool graphic ID ranges: use-script presence, craftability, and idle/animation-frame breakdown.

## Table 1 — Use Script & Craftability

| Graphic ID(s) | Description | Has Use Script? | Craftable? |
|---|---|---|---|
| 0x1069-0x106F | Leatherworking | No | No |
| 0x107A-0x1080 | Leatherworking | No | No |
| 0x1920-0x1937 | Flour Mill | 0x1920, 0x1922, 0x192C, 0x192E: **Yes**; rest No | 0x192C, 0x192D: **Yes (indirect, via deed)**; rest No |
| 0x197A-0x19A9 | Forge | 0x197A, 0x1986, 0x1992, 0x199E (full) + 0x197E, 0x1982, 0x198A, 0x198E, 0x1996, 0x199A, 0x19A2, 0x19A6 (partial): **Yes**; rest No | 0x197A, 0x1982, 0x199E, 0x19A6: **Yes (indirect)**; rest No |
| 0x2068 / 0x207A | Cauldron | No | No |
| 0x2DB0 | Soulforge | No | No |
| 0x2DD3-0x2DD4 | Elven Alchemy Table | Both: Yes | 0x2DD3: Yes (indirect); 0x2DD4: No |
| 0x2DD5-0x2DD6 | Elven Anvil | Both: Yes | 0x2DD5: Yes (indirect); 0x2DD6: No |
| 0x2DD8 | Elven Forge | No | **Yes (indirect)** |
| 0x2DD9-0x2DDA | Elven Spinning Wheel | No | No |
| 0x2DDB-0x2DDC | Elven Stove | No | No |
| 0x2E3C-0x2E3F | Elven Spinning Wheel | No | No |
| 0x3077-0x307A | Elven Alchemy Table | No (defined, no script field) | No |
| 0x3DA2 | Crystal Altar | No | No |
| 0x407C-0x407D | Incubator | No | No |
| 0x423B-0x4243 | Soulforge Base East | No | 0x423F: Yes (indirect); rest No |
| 0x424B-0x4253 | Soulforge Roof East | No | No |
| 0x4254-0x4257 | Soulforge Anvil | All: Yes | 0x4254: Yes (indirect); rest No |
| 0x4263-0x4272 | Soulforge BaseAnvil | No | No |
| 0x4277-0x4286 | Soulforge BaseAnvilR | No | No |
| 0x44C7-0x44CA | GargoyleMiniSoulForge | No | 0x44C7: Yes (indirect); rest No |
| 0x4CE6-0x4CED | Tinker Tables | No | No |
| 0x4F80-0x4FBB | Tinker Tables | No | No |
| 0x71A4-0x71AF | Tool racks | No | No |
| 0x71BA-0x71BB | Cooking rack | No | No |
| 0x9966 | Amalgamator | No | No |
| 0x9987-0x999E | Grill | No | No |
| 0x9A38-0x9A49 | Machine Sewing | No | No |
| 0x9A82-0x9A91, 0x9AA8-0x9AA9 | Machine Blacksmith | 5 IDs present but unrelated repurposed items (boost stones, bag renamer, fireworks) | No |
| 0x9C1A-0x9C2D | Machine Woodworking | No | No |
| 0x9C2E-0x9C41 | Machine Fletching | No | No |
| 0x9D83-0x9D96 | Machine Alchemy | No | No |
| 0x9D98-0x9DA2 | Machine Cooking | No | No |
| 0xA212-0xA221 | Tinker Tables | No | No |
| 0xA277-0xA284 | Repair Tables | No | No |
| 0xA2A3 | Butcher Block | No | No |
| 0xA2A4-0xA2AB | Wood Stove | 0xA2A4, 0xA2A5, 0xA2A8, 0xA2A9: Yes; rest No | No |
| 0xA302-0xA303 | Hanging tools south | No | No |
| 0xA31F-0xA320 | Plow | 0xA320: Yes, but repurposed townstone, not a plow | No |
| 0xA329-0xA32A | Hanging tools east | No | No |
| 0xA52E-0xA537 | Glassblowing | 0xA530, 0xA531, 0xA534, 0xA535: Yes; rest No | No |
| 0xA538-0xA547 | Masonry | No | No |
| 0xA588-0xA593 | Magic Book stand | No | No |
| 0xA73F-0xA742 | Enchanter | No | No |
| 0xA81D-0xA830 | Autoloom | No | No |
| 0xA833-0xA84A | Cartography Table | No | No |
| 0xA84B-0xA84E | Potion Vat | No | No |
| 0xAC49-0xAC6A | Salvage station | No | No |
| 0xAC74-0xAC75 | Refinement Generator | No | No |
| 0xAF21-0xAF2C | Greenhouse | No | No |
| 0xB00D-0xB012 | Necrotic Brazier | No | No |
| 0xB128-0xB129 | Dye Cabinet | No | No |

**Notes:**
- Roughly 400+ of the ~600 individual graphic IDs in this list have **no itemdesc.cfg entry at all** — pure client art (tiles.cfg/offset.cfg/animatedgraphics.cfg only), not live items in any sense.
- Every "craftable" hit is a Carpentry recipe using `MakeDeed` — the player receives a deed objtype, not the graphic itself, hence "Yes (indirect)".
- No hits at all in blacksmithy.cfg, bowcraft.cfg, tinker.cfg, or tailoring.cfg for anything in this list.
- A few IDs (0x9A86-0x9A8B, 0xA320) are defined in itemdesc.cfg but as unrelated repurposed items (boost stones, town stones, fireworks), not real crafting stations.

## Table 2 — Idle / Animation-Frame Breakdown

| Range (craft) | Static furniture | Idle/off tool | In-use animation | Source |
|---|---|---|---|---|
| 0x1069-0x106F (Leatherworking) | 0x1069 (decorations pkg) | — | none | Rest not registered |
| 0x107A-0x1080 (Leatherworking) | 0x107C (decorations pkg) | — | none | Rest not registered |
| 0x1920-0x1937 (Flour Mill) | — | Mill1: back 0x1920, front 0x1922. Mill2: back 0x192C, front 0x192E | **Real multi-frame animation.** Mill1: front→0x1926, back→0x1921→0x1925→revert. Mill2: front→0x1932, back→0x192D→0x1931→revert | `pkg/std/cooking/grinding.src`, `animate_mill()` |
| 0x197A-0x19A9 (Forge) | Middle/end pieces (no trigger): 0x198A, 0x1996, 0x199A, 0x198E, 0x197E, 0x19A2, 0x1982, 0x19A6 | Bellows (clickable): 0x1986, 0x1992, 0x197A, 0x199E | No distinct frame objtypes — uses engine-native `.Animate()` per piece | `pkg/items/forge/use.src`, `Animate_Large_Forge_Items()` |
| 0x2DD5-0x2DD6 (Elven Anvil) | Both (South/East) | — | none — `MethodScript` is a static `IsAnvil()` marker only | `anvil/method.src` |
| 0x4254-0x4257 (Soulforge Anvil) | All 4 variants | — | none (same static marker) | `anvil/method.src` |
| 0xA2A4-0xA2AB (Wood Stove) | — | East: idle 0xA2A4 ↔ lit 0xA2A5. South: idle 0xA2A8 ↔ lit 0xA2A9 | **Binary toggle only**, not multi-frame | `light/use.src` `Toggle()` + `ChangeTo` field |
| 0xA52E-0xA537 (Glassblowing) | 0xA52E/0xA52F not registered | South: idle 0xA530 ↔ lit 0xA531. East: idle 0xA534 ↔ lit 0xA535 | **Binary toggle only** — 0xA532/0xA533/0xA536/0xA537 not registered | Same `light/use.src` mechanism |
| All other ranges | — | — | — | **Not registered in any itemdesc.cfg** — no item exists in-repo for these IDs, so no animation logic of any kind |

**Key findings:**
- **Glassblowing is a binary on/off toggle in this repo, not a 3-frame cycling animation.** 0xA530/0xA531 and 0xA534/0xA535 are simple `ChangeTo`-linked idle↔lit pairs. 0xA532, 0xA533, 0xA536, 0xA537 don't exist as items at all.
- **Only the Flour Mill (0x1920-0x1937) has genuine multi-frame in-use animation** — real `item.graphic :=` swaps through a short sequence and back.
- **Forge** animates via the engine's native `.Animate()` call (client-side animdata flicker), not objtype cycling.
- **Elven Anvil / Soulforge Anvil** have a `MethodScript` field but it's just a static `IsAnvil()` identity check — no animation logic despite having a method script.
- **41 of the 53 ranges have zero itemdesc.cfg entries anywhere** in the repo — unused/reserved tile IDs or art-only references, never wired into a spawnable EScript item.
