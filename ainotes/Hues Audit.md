# Hues Audit

Running log of shard hue migrations. Each section below is a self-contained audit for one
hue swap: search method, every live location found, false positives excluded, and (once
applied) the revert reference. New hue swaps get a new `##` section appended to this file —
don't create separate per-hue files.

---

## Hue 1765 → 2669 ("New Zulu" signature hue) — 2026-08-14

**STATUS: APPLIED 2026-08-14, RE-TARGETED 2026-08-14.** Original color was `1765`. First
applied to `2233`, then the shard owner corrected the target: "the whole zulu package also
have to be moved to 2669 for zulu color. instead of 2233." Every location below is now
`2669` in the working tree (not yet committed). The tables/line numbers below still describe
the original `1765` state (search results weren't re-run against `2233`, since the retarget
was a straight find-and-replace of `2233` → `2669` across the exact same locations — nothing
new was searched for). This section is kept as the revert reference. Since these edits aren't
committed yet, `git diff` / `git checkout -- <file>` also works for a full or partial rollback
before commit.

Full-shard search for every live use of color/hue **1765** (0x6E5 in hex — nothing in the
repo actually stores it in hex form, always decimal `1765`). This was a pre-change snapshot
so the color move could be reverted item-by-item if needed.

Search method: `\b1765\b` plus `i1765\b` (to catch `CProp X iNNNN`-style integer-encoded
values, which `\b1765\b` alone misses since there's no word boundary between a letter and a
digit) across all `.cfg`, `.src`, `.inc` files. Each hit was opened in context and classified
as a real color/hue assignment or a false positive (coordinate, objtype, or array index that
happens to equal 1765). Only confirmed color usages are listed below.

**Revision note (2026-08-14):** cross-checked against the shard owner's own list. Two entries
were missed on the first pass (TekagiOmega's `CProp EnchantColor i1765`, and the guild color
picker array) and were added. Two names from the owner's list — CWReadyStone and townstone —
turned out not to be live matches; see "Not currently live" below.

### NPCs — config/npcdesc.cfg

All of these set both `Color` and `TrueColor` to 1765 on the NPC body:

| NPC | Objtype | Color line | TrueColor line |
|---|---|---|---|
| a Mountain Ostard | 0xdb | 8405 | 8406 |
| a Mountain Frenzied Ostard | 0xda | 9981 | 9982 |
| an Earthmag | 0x1f | 22252 | 22253 |
| a Rock Drake | 0x3d | 26824 | 26825 |
| a Rock Dragon | 0xc | 27392 | 27393 |
| Master of the Earth | 0x3de | 29984 | 29985 |
| <random> the Archangel | 0x190 | 30225 | 30226 |
| a Nemesis | 0x190 | 30296 | 30297 |
| a Glutton | 256 | 34606 | 34607 |

Also: **Master of the Earth**'s mount is colored 1765 too —
`config/npcdesc.cfg:30017` → `mount   0x3ea3 1765`

False positive (do not touch): `config/npcdesc.cfg:39242`, `NpcTemplate horsewhite` uses
`objtype 1765` — that's a graphic ID, not a color; coincidence only.

### Items — config/itemdesc.cfg

| Item | Graphic | Color line |
|---|---|---|
| QuestStone | 0x0ede | 1216 |
| QuestPrizeStone | 0x0ede | 1227 |
| warriorforhiredeed | 0x14ef | 1238 |
| silmaril | 0x1870 | 1284 |
| antigate | 6180 | 1346 |

### Crafting resource items

| Item | File | Graphic | Color line |
|---|---|---|---|
| NewZuluOre | pkg/std/mining/itemdesc.cfg | 0x19B9 | 205 |
| NewZuluIngot | pkg/std/mining/itemdesc.cfg | 0x1bf2 | 715 |
| zululog | pkg/std/lumberjacking/itemdesc.cfg | 0x1bdd | 249 |
| balroghide | pkg/std/tailoring/itemdesc.cfg | 0x1078 | 2724 |

False positive (do not touch): `pkg/std/tailoring/itemdesc.cfg:85`, `Item 0x1765` — that's
the item's hex objtype key, unrelated to color.

### Weapon enchant color

`pkg/systems/combat/config/itemdesc.cfg:7649` — `Weapon 0x27AB` (TekagiOmega) has
`CProp EnchantColor i1765`. This is a custom property, not the plain `Color` field, which is
why the first pass of this audit missed it.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array (the full set of hues
guild leaders can choose as their guild color) includes `1765` as one of its ~140 entries.
Removing/replacing 1765 here just drops it from the guild color picker's options; it's not a
default or assigned to any specific guild by itself.

### Fishing SOS area marker

`pkg/std/fishing/sosarea.cfg:6` — `color 1765` (map-marker color for an SOS bottle spawn
area at x 2634, y 3201, range 20)

### NPC default outfit

`config/equip.cfg:972` — `Equip 0x0efa 1765` in the `mage` equip template (robe default color)

### Scripts (hardcoded `.color := 1765`)

- `pkg/opt/zuluitems/use_racegate.src` — the Omega race gate's signature colors:
  - line 48: `OMEGA.+GateColour := 1765;`
  - line 49: `OMEGA.+RaceColour := 1765;`
  - line 52: `OMEGA.+GumpColour := 1765;`
- `scripts/items/pvp2vs2.src` — PvP arena fence posts, 10 occurrences, all `fnc.color := 1765;`
  at lines 830, 833, 836, 839, 842, 850, 853, 856, 859, 862
- `pkg/std/fishing/fishingnet.src` — fishing trophy items, 3 occurrences, all
  `it.color := 1765;` at lines 67, 74, 80

### Scripts (1765 as one option in a random-color pool)

`scripts/misc/dressme.src` included 1765 as one of many random NPC outfit hues (not a
dedicated "signature" use, just happened to be in the palette list). Changed anyway per the
shard owner's "change all those colors" instruction:
- line 316: `orecolor := 1765;` — option 10 of 34 in `DressWar`'s ore-color case
- line 362: `hidecolor := 1765;` — option 16 of 23 in `DressWar`'s hide-color case
- line 690: `hidecolor := 1765;` — same option 16 of 23, duplicated in `DressMage`

### Not currently live (from the owner's list, but not found in the working tree)

- **CWReadyStone** — belonged to `pkg/opt/colorwars/commands/player/cwready.src`, part of the
  "colorwars" package. Per `patchnotes/developer-changelog-v1.0.0.md:595` it was added, then
  per `patchnotes/developer-changelog-v1.0.1.md:44` it was deleted (`D`, 46 → 0 lines). The
  whole `pkg/opt/colorwars` directory is gone from this repo. Nothing to find or revert.
- **townstone** — `pkg/opt/townstones/itemdesc.cfg:5` currently reads `Color 1160`, not 1765.
  It shows up as 1765 in the stale `Used Colors.txt` / `Converted_Used_Colors.txt` dumps at
  the repo root, but the live file has already moved off 1765 (or that dump was never
  accurate for it). Not part of the live 1765 footprint.

### Confirmed false positives (excluded, no color relationship to 1765)

- `config/guardedareas.cfg:14-15` — Britain/British Castle area max-Y bound of 1765
- `scripts/include/areas.inc` (multiple) — same Britain/British Castle bounding box
- `scripts/include/objtype.inc:451`, `pkg/std/tailoring/scissors.src:10` —
  `UOBJ_CLOTH9` / `UOBJ_MUTLI_CLOTH` = `0x1765` (hex graphic ID, not decimal color)
- `pkg/utils/itemUtils/config/sets.cfg` — `1765 1765 ...` are X/Y placement coordinates
  in decoration sets, not colors
- `pkg/opt/decoratefacets/decorations/{britannia_alt/doors.cfg, ilshenar/decorations.cfg,
  tokuno/decorations.cfg}` — `Decor 1765` is just the sequential element index for that
  decor entry; the entries' actual `Color` fields are `0x48`/`0x0`, unrelated
- `Used Colors.txt` / `Converted_Used_Colors.txt` (repo root) — these are pre-existing
  aggregated reference dumps, not live source; not part of this audit's revert set

### Post-change verification

After applying the swap, re-ran the full-repo `1765` sweep (including a plain substring
search, not just word-boundary) across every `.cfg`/`.src`/`.inc` file. Only the pre-known
false positives above remain (Britain's map bounds, `UOBJ_CLOTH9`/`UOBJ_MUTLI_CLOTH` hex
graphic `0x1765`, decoration-set coordinates, `Decor 1765` element indices, `horsewhite`'s
objtype, plus a handful of unrelated cliloc IDs and a name-list entry `Name1765` that also
just happen to contain the substring "1765"). No genuine color usage of 1765 remains.

### Revert procedure (1765 → 2669)

To roll back this hue move entirely, restore each line above from `2669` to `1765` in its
file (not `2233` — that was a superseded interim value, never the live end state), or use
`git diff` / `git checkout -- <file>` against the working tree before this change is
committed.

---

## Hue 1158 → 2733 ("Fire"/lava signature hue) — 2026-08-14

**STATUS: APPLIED 2026-08-14, RE-TARGETED 2026-08-14.** Original color was `1158` (hex
`0x486`). First applied to `2231` (hex `0x8b7`) — chosen because the shard's
`ainotes/Hue names list.txt` reference labels that slot `#ZHOmega: Lavarock`. The owner then
picked a different, more correct "Lavarock" entry further down that same reference file
(`#ZHO: Lavarock#`) and asked to re-target the *entire* group there, not just the two items
literally named "Lavarock" — confirmed explicitly rather than assumed, since the 1158 group
also covers unrelated fire-creature NPCs, a weapon, a potion, and the guild color picker.
Every location below is now `2733` (hex `0xaad`) in the working tree (not yet committed). The
tables/line numbers below still describe the original `1158`/`0x486` state (the retarget was
a straight find-and-replace of `2231`/`0x8b7` → `2733`/`0xaad` across the exact same
locations — nothing new was searched for). This section is the revert reference.

Search method: same as the 1765 audit, but this hue exposed a gap the first audit didn't hit
— several items store the color as **hex `0x486`** instead of decimal `1158`, which a plain
`\b1158\b` search completely misses. Search covered `\b1158\b`, `i1158\b` (CProp integer
form), and `0x486\b` (hex form) across all `.cfg`, `.src`, `.inc` files.

**Revision note:** cross-checked against the shard owner's list of 12 names. 10 matched
directly (3 of those — lavahide, Grand Mage Refresh Elixir, an Evil Spellbook — only surfaced
via the hex-form search). Two extra live locations were found that weren't on the owner's
list: NPC **Joe, The Soul Whisperer**, and a **second config location** for Grand Mage
Refresh Elixir in `alchemyplus.cfg` (its recipe definition, separate from its `itemdesc.cfg`
item entry). A first edit pass also missed 5 leveled variants ([Level 1]-[Level 5]) of Grand
Mage Refresh Elixir in `itemdesc.cfg`, caught on the post-change verification sweep and fixed.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Encoding | Color line | TrueColor line |
|---|---|---|---|---|
| a Fire Serpent | 0x15 | decimal | 7650 | 7651 |
| a Fire Ostard | 0xdb | decimal | 8764 | 8765 |
| a Fire Frenzied Ostard | 0xda | decimal | 10327 | 10328 |
| an Evil Spellbook | 0x3d9 | hex `0x486` | 14812 | 14813 |
| an Inferno Wisp | 0x3a | decimal | 18409 | 18410 |
| a Fire Drake | 0x3d | decimal | 26178 | 26179 |
| Joe, The Soul Whisperer | 769 | decimal | 31162 | 31163 |

Joe, The Soul Whisperer was not on the owner's original list — found during the search, not
requested by name, but included since it's a genuine live use of the hue.

False positive (do not touch): `config/npcdesc.cfg:46164`, `NpcTemplate hairydaemon` uses
`objtype 1158` — that's a graphic ID, not a color; coincidence only.

### Crafting resource items

| Item | File | Encoding | Color line |
|---|---|---|---|
| LavarockOre | pkg/std/mining/itemdesc.cfg | decimal | 171 |
| LavarockIngot | pkg/std/mining/itemdesc.cfg | decimal | 693 |
| lavahide | pkg/std/tailoring/itemdesc.cfg | hex `0x486` | 2636 |

### Alchemy — Grand Mage Refresh Elixir (two files, six locations)

Not on the owner's list as a "second location," but the same logical item is defined twice:

- `pkg/opt/alchemyplus/itemdesc.cfg` — base item (line 629, hex `0x486`) plus 5 leveled
  variants ([Level 1] through [Level 5]) at lines 646, 663, 680, 697, 714 — all hex `0x486`.
  The leveled variants were missed on the first edit pass (the context window used to read
  the file cut off before their `color` line) and were only caught by the post-change sweep.
- `pkg/opt/alchemyplus/alchemyplus.cfg:592` — the crafting recipe definition for the same
  item, also hex `0x486`. Both files needed the change or the recipe and the finished potion
  would drift apart.

### Weapon enchant color

`pkg/systems/combat/config/itemdesc.cfg:7593` — `Weapon 0x757b` (AxeofAnias) has
`CProp EnchantColor i1158`.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array includes `1158` as one
of its ~140 entries, same array touched in the 1765 audit.

### Scripts (1158 as one option in a random-color pool)

`scripts/misc/dressme.src` included 1158 as one of many random NPC outfit hues:
- line 314: `orecolor := 1158;` — option 8 of 34 in `DressWar`'s ore-color case (decimal)
- line 353: `hidecolor := 0x486;` — option 7 of 23 in `DressWar`'s hide-color case (hex)
- line 681: `hidecolor := 0x486;` — same option 7 of 23, duplicated in `DressMage` (hex)

### Bug fix: trash-can-of-wonders rare dye range

`pkg/opt/shilitems/trashcanofwonders.src:197` — `AddRareDye()` rolled
`var color := RandomInt(26)+1158;`, a random `EnchantColor` in the range 1158–1183 for
legendary loot from the trash can of wonders. This wasn't a fixed color, it was the *base of
a random range* tied to the old hue. Changed to `RandomInt(26)+2733;` (new range 2733–2758,
after the 2231→2733 retarget) per the shard owner's explicit "fix the trash can as well"
instruction, so the loot's random color range moves with the hue migration instead of staying
anchored to the retired hue.

### Confirmed false positives (excluded, no color relationship to 1158/0x486)

- `scripts/playermanager.src:71` — `if( who.y >= 1158 )`, a map-coordinate check
- `config/landtiles.cfg:9802` (`landtile 0x486`), `config/tiles.cfg:13979` (`tile 0x486`),
  `pkg/opt/alryc/config/nonanimatedgraphics.cfg:1243` (`Tile 0x486`) — all the graphic/tile ID
  `0x486`, not a color
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg` —
  `Decor 1158` is a sequential element index (same false-positive pattern as the 1765 audit)
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — `Keyi1158` is a decoration filename
  key, not a color
- `Used Colors.txt` / `Converted_Used_Colors.txt` (repo root) — stale aggregated dumps, not
  live source

### Post-change verification

Re-ran the full sweep (`\b1158\b`, `i1158\b`, `0x486\b`, all case-insensitive, substring where
relevant) across every `.cfg`/`.src`/`.inc` file after applying the edits, which is what
caught the 5 missed leveled-elixir entries above. After fixing those, only the false
positives listed above remain. No genuine color usage of 1158/0x486 remains.

### Revert procedure (1158/0x486 → 2733/0xaad)

To roll back this hue move entirely, restore each decimal `2733` above to `1158`, and each
hex `0xaad` above to `0x486`, at the cited file/line (not `2231`/`0x8b7` — that was a
superseded interim value, never the live end state). Uncommitted, so `git diff` /
`git checkout -- <file>` also works for a full or partial rollback before commit.

---

## Hue 1155 → 2730 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Part of a 6-hue batch the shard owner supplied at once
(1155, 1156, 1157, 1159, 1160, 1161 — 1158 was already covered above). Each hue in the batch
is audited and applied separately, one confirmation at a time, not as a single bulk pass.

Search method: `\b1155\b`, `i1155\b` (CProp integer form), `0x483\b` (hex form) across all
`.cfg`, `.src`, `.inc` files.

**Revision note:** cross-checked against the shard owner's list of 5 names. All 5 matched
directly, no hex-encoded instances this time. One extra location found that wasn't on the
list: `scripts/misc/dressme.src`'s random ore-color pool. Starting with this hue, the owner
confirmed dressme.src's pool entries should be included in every audit going forward (not
asked about case-by-case), since those colors have been moved and the random palette
shouldn't keep offering a retired hue.

**Guild color picker note:** `guildconstants.inc`'s `COLOURS` array has a long pre-existing
block of reserved values `2706`–`2755` (with a few gaps) that overlaps several targets in
this batch — `2730` (this hue), `2732`, `2733`, `2735`, `2736` are already present in that
block; `2731` and `2734` are gaps. Swapping the old hue into the array's original slot can
therefore produce a duplicate value elsewhere in the same array (it already did for the 1158
audit — 2733 now appears twice). Asked the owner how to handle this: **decision is to allow
duplicates and always rewrite in place**, matching every other file's swap pattern, rather
than deleting the old slot when the target is already present. No further action needed on
the existing 1158/2733 duplicate.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Color line | TrueColor line |
|---|---|---|---|
| a Tropical Ostard | 0xdb | 8660 | 8661 |
| a Tropical Frenzied Ostard | 0xda | 10228 | 10229 |

False positive (do not touch): `config/npcdesc.cfg:46134`, `NpcTemplate daemon10` uses
`objtype 1155` — graphic ID, not a color; coincidence only.

### Crafting resource items

| Item | File | Color line |
|---|---|---|
| SpectralOre | pkg/std/mining/itemdesc.cfg | 288 |
| SpectralIngot | pkg/std/mining/itemdesc.cfg | 770 |

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array includes `1155` as one
of its ~140 entries. See the guild color picker note above re: the resulting duplicate `2730`.

### Scripts (1155 as one option in a random-color pool)

`scripts/misc/dressme.src:321` — `orecolor := 1155;`, option 15 of 34 in `DressWar`'s
ore-color case. Not on the owner's original list; included per the standing instruction to
fix dressme.src's pool entries in every audit.

### Confirmed false positives (excluded, no color relationship to 1155/0x483)

- `scripts/include/teleporters.inc`, `scripts/include/areas.inc` — map coordinates that
  happen to equal 1155
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,malas,tokuno}/*.cfg` —
  `Decor 1155` is a sequential element index (same false-positive pattern as prior audits)
- `config/landtiles.cfg:9769` (`landtile 0x483`), `pkg/opt/alryc/config/nonanimatedgraphics.cfg`
  (`Tile 0x483`) — graphic/tile ID, not a color
- `scripts/include/client.inc:1184` — `SFX_483 := 0x483`, a sound-effect constant, not a color
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — `Keyi1155` is a decoration filename
  key, not a color

### Post-change verification

Re-ran the full sweep (`\b1155\b`, `i1155\b`, `0x483\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine color
usage of 1155/0x483 remains.

### Revert procedure (1155 → 2730)

To roll back this hue move, restore each `2730` above to `1155` at the cited file/line.
Uncommitted, so `git diff` / `git checkout -- <file>` also works for a full or partial
rollback before commit.

---

## Hue 1156 → 2731 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Part of the same batch as the 1155 audit above.

Search method: `\b1156\b`, `i1156\b`, `0x484\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** cross-checked against the shard owner's list of 3 names. All 3 matched
directly. No `dressme.src` hit this time — 1156 simply isn't one of the values in that
random-color pool, so nothing to add there. 2731 is one of the gaps in the guild array's
reserved `2706`–`2755` block, so this swap doesn't introduce a duplicate.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Color line | TrueColor line |
|---|---|---|---|
| a Snow Ostard | 0xdb | 8700 | 8701 |
| a Snow Frenzied Ostard | 0xda | 10267 | 10268 |

False positive (do not touch): `config/npcdesc.cfg:38740`, `NpcTemplate spinedbeast` uses
`objtype 1156` — graphic ID, not a color; coincidence only.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array includes `1156` as one
of its ~140 entries.

### Correction (found 2026-08-14, during the 1163 audit): NPC equip template

Missed on the original pass because it used a **4-digit zero-padded hex form** (`0x0484`)
that neither the `\b1156\b` nor the `0x484\b` search pattern catches (`0x484\b` requires a
word boundary right after "484", but `0x0484` has another hex digit, `0`, sitting between
`x` and `484`, so the substring `0x484` never actually appears in `0x0484`). Found while
investigating a similar padded-hex miss in the 1163 audit, then confirmed by re-sweeping
`0x0484\b` across the whole repo.

`config/equip.cfg` — `Equipment archangel` template (equipped on <random> the Archangel,
from the 1765 audit): `Weapon 0x9a16 0x0484` and `Armor 0x76ad 0x0484`. Both changed to
`0x0aab`.

From this point on, every remaining hue in this batch is also searched for its 4-digit
zero-padded hex form (`0x0NNN`), not just the 3-digit form, to avoid repeating this miss.

### Confirmed false positives (excluded, no color relationship to 1156/0x484)

- `pkg/opt/areas/areas.cfg:89`, `regions/regions.cfg:1006`,
  `pkg/mobiles/npcs/config/specialNPCs.cfg:603` — the Pirate Ship area/region bounding box
- `config/goloc.cfg:278`, `config/golocs_by_id.cfg` — y-coordinates that happen to equal 1156
- `scripts/include/teleporters.inc` — a teleporter destination coordinate
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg` —
  `Decor 1156` sequential element indices
- `config/landtiles.cfg:9780` (`landtile 0x484`) — graphic tile ID, not a color
- `scripts/include/client.inc:1185` — `SFX_484 := 0x484`, a sound-effect constant, not a color
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — `Keyi1156` is a decoration filename
  key, not a color

### Post-change verification

Re-ran the full sweep (`\b1156\b`, `i1156\b`, `0x484\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine color
usage of 1156/0x484 remains.

### Revert procedure (1156 → 2731)

To roll back this hue move, restore each `2731` above to `1156` at the cited file/line.
Uncommitted, so `git diff` / `git checkout -- <file>` also works for a full or partial
rollback before commit.

---

## Hue 1157 → 2732 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Part of the same batch as the 1155/1156 audits above. This
was the largest hue in the batch by far — the shard owner's list had 44 named items plus
Guild Colors, all of which matched, but the search turned up 9 more real locations not on
that list, several of them structural (equip templates, a functional dependency, spell
color, decoration set).

Search method: `\b1157\b`, `i1157\b`, `0x485\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** all 44 named items confirmed — 9 NPCs (`config/npcdesc.cfg`), 7 tailoring
pieces, 1 lumberjacking log, 9 Shadowpents, and 18 items in
`pkg/systems/combat/config/itemdesc.cfg` (9 as plain `color`, 9 as `CProp EnchantColor
i1157` — this file mixed both encodings for the same "shadow/darkness" item family, which is
why both `\b1157\b` and `i1157\b` were needed to catch all of it). Guild Colors confirmed.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Color line | TrueColor line |
|---|---|---|---|
| the Shadow Element Shrine Lord | 0x0f | 3917 | 3918 |
| a Stygian Constrictor | 0x15 | 7617 | 7618 |
| an Onyx Golem | 111 | 19176 | 19177 |
| a Stone Golem | 111 | 19338 | 19339 |
| a Void | 0x1a | 20967 | 20968 |
| <random> the Tainted One | 400 | 29311 | 29312 |
| a Bringer of Death | 0x3df | 30046 | 30047 |
| Gin the Legendary Hunter | 0x190 | 30386 | 30387 |
| Pestilence | 0x190 | 30659 | 30660 |

Not on the owner's list, found during the search: **a Bringer of Death**'s mount
(`config/npcdesc.cfg:30086`, `mount 0x3ea4 1157`) and **Gin the Legendary Hunter**'s mount
(`config/npcdesc.cfg:30457`, `mount 0x3e9f 1157`) — same pattern as Master of the Earth's
mount in the 1765 audit.

False positive (do not touch): `config/npcdesc.cfg`, `NpcTemplate vulture` uses
`objtype 1157` — graphic ID, not a color; coincidence only.

### Tailoring — pkg/std/tailoring/itemdesc.cfg

| Item | Color line |
|---|---|
| LeatherGorgetofshadow | 1529 |
| LeatherGlovesofshadow | 1546 |
| leatherarmsofshadow | 1563 |
| leatherLegsofshadow | 1580 |
| leatherTunicofshadow | 1597 |
| maskcofshadow | 1612 |
| balronhide | 2702 |

### Other crafting resource

`pkg/std/lumberjacking/itemdesc.cfg:301` — darknesslog.

### Zulu items — pkg/opt/zuluitems/itemdesc.cfg

Shadowpent1 through Shadowpent9 (9 items; note `shadowpent7` is lowercase in the file,
matching the owner's list), all `Color 1157`.

### Weapons/armor — pkg/systems/combat/config/itemdesc.cfg (18 items)

Plain `color`/`Color` field (9): Pitchforkofshadow, breastofshadow, legsofshadow,
helmofshadow, gorgetofshadow, armsofshadow, glovesofshadow, BlackPants, TaintedMageStaff.

`CProp EnchantColor i1157` (9): lanceofthejouster, FemalePlateofDarkness,
PlateHelmofDarkness, PlateArmsofDarkness, PlateLeggingsofDarkness, PlateGlovesofDarkness,
PlateGorgetofDarkness, PlatemailofDarkness, Shieldofdarkness.

### NPC equip templates — config/equip.cfg

Not on the owner's list. Two NPC "Equipment" templates carry the hue:
- `Equipment tribalking` — `Weapon 0x329 1157` and (further down, in a second template block
  reusing the same weapon line pattern) `Weapon 0xff00 1157`
- `Equipment darklancer` — Gin the Legendary Hunter's own gear: 6 `Armor` entries
  (`0x76a7`–`0x76ac`) plus `Weapon 0xda12` (the same objtype as `lanceofthejouster` above),
  all `1157`

### Correction (found 2026-08-14, during the 1163 audit): more equip templates + a script

Same root cause as the 1156 correction above — these used the 4-digit zero-padded hex form
`0x0485`, which the original `0x485\b` search pattern doesn't match. Found and fixed once
the padding gap was discovered; confirmed complete by re-sweeping `0x0485\b` repo-wide.

- `config/equip.cfg` — `Equipment taintedranger`'s `Armor 0x76b1 0x0485` (this template also
  carries hue 1171's `Weapon 0x9a1a 0x0493`, untouched here, on the list for that hue later
  in the batch) and `Equipment taintedmage`'s `Weapon 0x9a16 0x0485`. Both changed to
  `0x0aac`.
- `scripts/control/skilladvancerequip.src:514` — `item.color := 0x0485;`, inside a
  case-branch tied to a `"head"` object-property check (equip-slot visual feedback). Changed
  to `0x0aac`.

### Champion spawn altar — bug-fix-adjacent dependency

Not on the owner's list, and the most important extra find in this hue:
- `pkg/opt/champspawns/scripts/oncreate.src` — spawns 10 altar-piece items (graphics
  `0x750`–`0x75C`) and sets `item.color:=1157;` on each, to visually mark them as this
  champion's altar decoration.
- `pkg/opt/champspawns/include/rewards.inc:15` — **reads that same color back**:
  `if(item.color == 1157)` to recognize the altar pieces for reward logic. This is a
  write/read pair across two files; changing only `oncreate.src` would have silently broken
  the reward check (it would stop recognizing its own altar pieces). Both changed together
  to `2732`.

### Christmas gift and GM item

Not on the owner's list:
- `pkg/opt/christmas/Christmasgifts.src:36` — a "Coal" joke-gift item, `coal.color := 1157;`
- `pkg/opt/GMItems/bowofshadows_usescript.src:23` — a "bow of shadows" use-effect,
  `bow.color := 1157;`

### Necromancer spell-effect color

Not on the owner's list: `scripts/include/npccastspells.inc:349` —
`"NECRO": elecolor := 1157;`, mapping the necro spell school to this hue for cast visuals
(alongside sibling entries for EARTH/WATER/FIRE/AIR/HOLY using other hues).

### Decoration set

Not on the owner's list: `pkg/utils/itemUtils/config/sets.cfg`, the `EvilFireplace` set's
"sandstone" piece (objtype 1900), 3 occurrences (lines 13319, 13330, 13331). Confirmed via
the file's own `//Item ObjType Graphic Color X Y Z DESC` header comment that the third
numeric field is genuinely the color slot here — worth double-checking per-file, since a
structurally similar-looking sets.cfg line was correctly excluded as a false positive
(objtype, not color) during the 1765 audit.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array includes `1157` as one
of its ~140 entries.

### Scripts (1157 as one option in a random-color pool)

`scripts/misc/dressme.src` — `hidecolor := 1157;`, option 6 of 23 in `DressWar`'s hide-color
case, duplicated in `DressMage` (2 occurrences).

### Confirmed false positives (excluded, no color relationship to 1157/0x485)

- `config/golocstokuno.cfg:127` — a "Volcano" goloc x-coordinate
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg` —
  `Decor 1157` sequential element indices
- `pkg/systems/combat/config/hitscriptdesc.cfg:632` — `//	Color		1157` inside a commented-out
  (dead) block, not live code
- `config/landtiles.cfg:9791` (`landtile 0x485`) — graphic tile ID, not a color
- `scripts/include/client.inc:1186` — `SFX_485 := 0x485`, a sound-effect constant, not a color
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — `Keyi1157` is a decoration filename
  key, not a color

### Post-change verification

Re-ran the full sweep (`\b1157\b`, `i1157\b`, `0x485\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine color
usage of 1157/0x485 remains.

### Revert procedure (1157 → 2732)

To roll back this hue move, restore each `2732` above to `1157` (decimal `2732` at most
locations, hex `0x0aac` at the two equip.cfg entries and skilladvancerequip.src added in the
correction) at the cited file/line — including the `rewards.inc` comparison, which must move
in lockstep with `oncreate.src` or the champion spawn reward check breaks. Uncommitted, so
`git diff` / `git checkout -- <file>` also works for a full or partial rollback before
commit.

---

## Hue 1159 → 2734 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Part of the same batch as the 1155/1156/1157 audits above.

Search method: `\b1159\b`, `i1159\b`, `0x487\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** cross-checked against the shard owner's list of 13 names. 12 matched
directly. **a Swamp Dragonling** was on the list but is not a live match — it exists at
`config/npcdesc.cfg:33187` with `Color`/`TrueColor` currently `0`, not 1159 (two similarly
named NPCs nearby, "an Ethereal Armored Swamp Dragonling" and "an Armored Swamp Dragonling",
are `16385` and `0` respectively — none is 1159). Same situation as townstone in the 1765
audit. Owner confirmed: leave it alone. One extra found, not on the list: SnoopingGloves.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Color line | TrueColor line |
|---|---|---|---|
| an Emerald Ostard | 0xdb | 8596 | 8597 |
| an Emerald Frenzied Ostard | 0xda | 10167 | 10168 |
| a Wyrm | 61 | 27008 | 27009 |
| a Poison Dragon | 1090 | 27208 | 27209 |
| a Great Wyrm | 0x3b | 27882 | 27883 |

False positive (do not touch): `config/npcdesc.cfg:46194`, `NpcTemplate hornedsnakedaemon`
uses `objtype 1159` — graphic ID, not a color; coincidence only.

**Not live (left alone per owner's decision):** a Swamp Dragonling — see revision note above.

### Crafting resources

| Item | File | Color line |
|---|---|---|
| MalachiteOre | pkg/std/mining/itemdesc.cfg | 120 |
| MalachiteIngot | pkg/std/mining/itemdesc.cfg | 660 |
| wyrmhide | pkg/std/tailoring/itemdesc.cfg | 2691 |
| emeraldlog | pkg/std/lumberjacking/itemdesc.cfg | 184 |
| botanicscissor | pkg/opt/botanik/itemdesc.cfg | 64 |
| hopseed | pkg/opt/farming/itemdesc.cfg | 343 |

### Extra item — not on the owner's list

`pkg/std/snooping/itemdesc.cfg:16` — SnoopingGloves, `Color 1159`, a snooping-skill tool.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — the `COLOURS` array includes `1159` as one
of its ~140 entries.

### Scripts (1159 as one option in a random-color pool)

`scripts/misc/dressme.src`:
- line 311: `orecolor := 1159;` — option 5 of 34 in `DressWar`'s ore-color case
- line 357: `hidecolor := 1159;` — option 11 of 23 in `DressWar`'s hide-color case
- line 685: same option 11 of 23, duplicated in `DressMage`

### Confirmed false positives (excluded, no color relationship to 1159/0x487)

- `config/innlocation.cfg:52` — a "The Scholar's Inn 2" region `rect` boundary
- `pkg/mobiles/npcs/config/specialNPCs.cfg` — `wanderinghealer` spawn-location y-coordinates
  (2 occurrences)
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg`,
  `pkg/opt/decoratefacets/decorations/ilshenar/doors.cfg` — `Decor 1159` element indices and
  a decoration `X 1159` coordinate (the latter on an item whose `ObjType 0x6e5` is
  coincidentally the same hex value as the 1765 hue from the first audit — unrelated, it's a
  graphic ID here, not a color)
- `config/landtiles.cfg:9813` (`landtile 0x487`) — graphic tile ID, not a color
- `scripts/include/client.inc:1188` — `SFX_487 := 0x487`, a sound-effect constant, not a color
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — decoration filename keys, not colors

### Post-change verification

Re-ran the full sweep (`\b1159\b`, `i1159\b`, `0x487\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above (plus the
intentionally-untouched Swamp Dragonling, which was never live at 1159) remain. No genuine
color usage of 1159/0x487 remains.

### Revert procedure (1159 → 2734)

To roll back this hue move, restore each `2734` above to `1159` at the cited file/line.
Uncommitted, so `git diff` / `git checkout -- <file>` also works for a full or partial
rollback before commit.

---

## Hue 1160 → 2735, except townstone → 2669 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Part of the same batch as the 1155/1156/1157/1159 audits
above. No Guild Colors entry for this hue — checked, `1160` was never in the guild
`COLOURS` array (it jumps from 1159's slot straight to 1655).

Search method: `\b1160\b`, `i1160\b`, `0x488\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** cross-checked against the shard owner's list of 5 names. All 5 matched —
including **a Behemoth**, stored as hex `0x488` (the second hex-encoded NPC found in this
batch, after an Evil Spellbook in the 1158 audit). Three extra locations found, not on the
list: **Carrie, The Soul Whisperer** (sibling to Joe, The Soul Whisperer from the 1158
audit), a `PhysicalProtection` magic-item mod-enchant color, and 10 decoration-set pieces.

**townstone resolved:** this audit also closes out a loose end from the very first (1765)
audit, where `townstone` was found at `Color 1160` and flagged as "not currently live at
1765" without knowing where it *did* belong — it belongs here, at 1160. The shard owner then
redirected it: townstone moves to **2669 (the Zulu hue)** instead of this hue's normal
2735 target, per an explicit instruction while implementing this hue. Every other 1160
location uses 2735 as normal.

### NPCs — config/npcdesc.cfg

| NPC | Objtype | Encoding | Color line | TrueColor line |
|---|---|---|---|---|
| a Behemoth | 0x0e | hex `0x488` | 19731 | 19732 |
| Plague | 0x190 | decimal | 30568 | 30569 |
| Scourge Infiltrator | 0x190 | decimal | 30717 | 30718 |
| Scourge Battlemaster | 0x190 | decimal | 30806 | 30807 |
| Carrie, The Soul Whisperer | 773 | decimal | 30877 | 30878 |

False positive (do not touch): `config/npcdesc.cfg:42499`, `NpcTemplate mutant_orc` uses
`objtype 1160` — graphic ID, not a color; coincidence only.

### Items

| Item | File | Target |
|---|---|---|
| magicfish9 | pkg/std/fishing/itemdesc.cfg:429 | 2735 |
| townstone | pkg/opt/townstones/itemdesc.cfg:5 | **2669** (Zulu, not 2735 — see note above) |

### Magic item mod-enchant color

`pkg/systems/combat/config/modenchantdesc.cfg:1347` — `MiscEn PhysicalProtection`'s
`Color 1160`, the visual color tied to that magic-item property. Not on the owner's list.

### Decoration set pieces — pkg/utils/itemUtils/config/sets.cfg

Not on the owner's list. 10 occurrences across several decoration sets, all confirmed via
the file's `//Item ObjType Graphic Color X Y Z DESC` header format:
- wall sconces, objtype 2562 (6 occurrences, lines 12290-12295)
- wall sconces, objtype 2557 (4 occurrences, lines 12302-12305)
- wall torches, objtype 2567 (2 occurrences, lines 12969-12970)
- a statue, objtype 4824 (2 occurrences, lines 12980-12981)

### No dressme.src hit

1160 isn't one of the values in that random-color pool.

### Confirmed false positives (excluded, no color relationship to 1160/0x488)

- `pkg/systems/combat/config/itemdesc.cfg` — `VendorSellsFor 1160` (a gold price, 3
  occurrences) — coincidence, not a color
- `pkg/opt/townstones/textcmd/admin/townbankstatus.src` — `GFCreateGump`/`GFResizePic` use
  `1160` as a gump pixel width, not a color
- `pkg/opt/areas/areas.cfg`, `regions/regions.cfg`, `config/golocs_by_id.cfg` — the Zento
  area/region/goloc bounding box
- `pkg/mobiles/npcs/config/specialNPCs.cfg` — a `wanderinghealer` spawn-location coordinate
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg` —
  `Decor 1160` element index and a decoration `X 1160` coordinate
- `config/landtiles.cfg:9824` (`landtile 0x488`), `scripts/include/client.inc:1189`
  (`SFX_488 := 0x488`) — graphic tile ID / sound-effect constant, not colors
- Six `.inc` files (`teleporters.inc`, `attributes.inc`, `skillpoints.inc`, `areas.inc`,
  `checkcity.inc`, `findcity.inc`, `jailcheck.inc`) — all `y >= 1160`-style map-boundary
  checks, not colors
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — decoration filename keys

### Post-change verification

Re-ran the full sweep (`\b1160\b`, `i1160\b`, `0x488\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine
color usage of 1160/0x488 remains.

### Revert procedure (1160 → 2735, townstone → 2669)

To roll back this hue move, restore each `2735` above to `1160` at the cited file/line,
**except** townstone, which reverts from `2669` to `1160` (not from 2735 — it was never set
to that value). Uncommitted, so `git diff` / `git checkout -- <file>` also works for a full
or partial rollback before commit.

---

## Hue 1161 → 2736 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Last hue in the 6-hue batch (1155, 1156, 1157, 1159, 1160,
1161 — 1158 was already covered before the batch started). No Guild Colors entry, no
dressme.src hit for this one — checked, `1161` isn't in either.

Search method: `\b1161\b`, `i1161\b`, `0x489\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** the owner's list was just one item, fishshell8, confirmed. One extra:
`scripts/include/npccastspells.inc:348`, `"AIR": elecolor := 1161;` — the sibling entry to
`"NECRO": elecolor := 2732` from the 1157 audit, same spell-school color mapping table
(FIRE/WATER/EARTH/HOLY use other, untouched hues).

### Item

`pkg/std/fishing/itemdesc.cfg:242` — fishshell8, stored as hex `0x489` (the third
hex-encoded item found in this batch, after lavahide/Grand Mage Refresh Elixir/an Evil
Spellbook in the 1158 audit and a Behemoth in the 1160 audit).

### Spell-effect color

`scripts/include/npccastspells.inc:348` — `"AIR": elecolor := 1161;`, not on the owner's
list.

### Confirmed false positives (excluded, no color relationship to 1161/0x489)

- `config/npcdesc.cfg`, `NpcTemplate a red imp` uses `objtype 1161` — graphic ID, not a
  color; coincidence only
- `scripts/textcmd/player/hairshop.src:570` — an `IsInBox` coordinate check
- `scripts/include/teleporters.inc` — two commented-out ("BAD TELEPORTER — REMOVE") dead
  teleporter entries
- `pkg/mobiles/npcs/config/specialNPCs.cfg` — a `wanderinghealer` spawn-location coordinate
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg`,
  `ilshenar/doors.cfg` — `Decor 1161` element indices
- `config/landtiles.cfg:9835` (`landtile 0x489`), `scripts/include/client.inc:1190`
  (`SFX_489 := 0x489`), `pkg/opt/alryc/config/animatedgraphics.cfg` (`Tile 0x489`),
  `pkg/items/deed/built/sandstoneFireplace.cfg` (`Component 0x489 0 0 0`) — all graphic/tile
  IDs, not colors
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — decoration filename keys

### Post-change verification

Re-ran the full sweep (`\b1161\b`, `i1161\b`, `0x489\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine
color usage of 1161/0x489 remains.

### Revert procedure (1161 → 2736)

To roll back this hue move, restore each `2736` above to `1161` at the cited file/line (and
the hex `0xab0` back to `0x489` for fishshell8). Uncommitted, so `git diff` /
`git checkout -- <file>` also works for a full or partial rollback before commit.

---

## Hue 1162 → 2741 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** First hue of a new, larger 20-hue batch supplied by the
shard owner as a table (hue, hex, new hue). All 33 items on the owner's list matched.

Search method: `\b1162\b`, `i1162\b`, `0x48a\b` across all `.cfg`, `.src`, `.inc` files.

**Revision note:** two extras found, not on the list: an SOS area marker and a UI
category-icon color entry, both thematically tied to the same "earth" item family already
on the list.

### Earth scrolls and book — pkg/opt/earth/itemdesc.cfg

| Item | Encoding |
|---|---|
| BookoftheEarth | decimal |
| Antidotescroll, Owlsightscroll, Shiftingearthscroll, Summonmammalsscroll, Calllightningscroll, Earthblessingscroll, Earthportalscroll, Naturestouchscroll, Gustofairscroll, Risingfirescroll, Shapeshiftscroll, Icestrikescroll, Earthspiritscroll, Flamespiritscroll, Stormspiritscroll, Waterspiritscroll (16 scrolls) | hex `0x48a`, all identical `Color 0x48a` lines, changed via a single `replace_all` |

### NPCs — config/npcdesc.cfg

| NPC | Objtype |
|---|---|
| a Bull Frog | 0x51 |
| a Jungle Ostard | 0xdb |
| a Jungle Frenzied Ostard | 0xda |
| a Jade Golem | 111 |

False positive (do not touch): `NpcTemplate bigtoedmonster` uses `objtype 1162`.

### Other crafting resource

`pkg/std/lumberjacking/itemdesc.cfg` — jadelog.

### Weapons/armor (10 items, all `CProp EnchantColor i1162`)

`pkg/std/tailoring/itemdesc.cfg` — CloakofLettenhove (an `Armor 0x1f02`, despite living in
the tailoring file).

`pkg/systems/combat/config/itemdesc.cfg` — XarafaxsAxe plus the 8-piece "…ofDrakon" armor
set (FemalePlateofDrakon, PlateHelmofDrakon, PlateArmsofDrakon, PlateLeggingsofDrakon,
PlateGlovesofDrakon, PlateGorgetofDrakon, PlatemailofDrakon, Shieldofdrakon) — changed with
a single `replace_all` since all 9 lines were identical `CProp EnchantColor i1162`.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — `1162` in the `COLOURS` array.

### Extras — not on the owner's list

- `pkg/std/fishing/sosarea.cfg` — an SOS bottle-spawn area marker (x 2972, y 2360, range 25).
- `pkg/opt/omegacache/categories.cfg:80` — `EarthBookScrolls 1162 // 0x48A`, a UI
  category-icon color entry for the same earth-scroll family; updated the trailing hex
  comment to `// 0xAB5` to match.

### No dressme.src hit

1162 isn't one of the values in that random-color pool.

### Confirmed false positives (excluded, no color relationship to 1162/0x48a)

- `config/npcdesc.cfg`, `NpcTemplate bigtoedmonster` uses `objtype 1162`
- `scripts/ai/townguard.src:38` — a map-boundary check, `y >= 1152 and y <= 1162`
- `pkg/opt/decoratefacets/decorations/{britannia_alt,ilshenar,tokuno}/*.cfg`,
  `ilshenar/doors.cfg` — decoration `Y 1162` coordinates and `Decor 1162` element indices
- `config/landtiles.cfg:9846` (`landtile 0x48a`), `scripts/include/client.inc:1191`
  (`SFX_48A := 0x48A`) — graphic tile ID / sound-effect constant, not colors
- `scripts/include/teleporters.inc:2139` — a termur teleporter coordinate
- Mapgen decoration-key dumps (`mapgen/sosaria_*.txt`) — decoration filename keys

### Post-change verification

Re-ran the full sweep (`\b1162\b`, `i1162\b`, `0x48a\b`) across every `.cfg`/`.src`/`.inc`
file after applying the edits. Only the false positives listed above remain. No genuine
color usage of 1162/0x48a remains.

### Revert procedure (1162 → 2741)

To roll back this hue move, restore each `2741` above to `1162` (decimal) or `0x48a` (hex,
for the earth scrolls) at the cited file/line. The `omegacache/categories.cfg` comment
should revert to `// 0x48A` alongside its value. Uncommitted, so `git diff` /
`git checkout -- <file>` also works for a full or partial rollback before commit.

---

## Hue 1163 → 2742 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** All 6 items on the owner's list matched, plus one extra.
This hue is also where the **4-digit zero-padded hex gap** was discovered: AnraOre/AnraIngot
turned out to be stored as `0x048b` (4 digits), not `0x48b` (3 digits) — a padding variant
the original search method (`\bNNN\b` plus `0xNNN\b`) doesn't catch, since `0x48b\b` never
matches inside `0x048b` (there's an extra `0` digit sitting between `x` and `48b`, so the
literal substring `0x48b` doesn't occur). AnraOre/AnraIngot were only found by searching for
the item names directly, which is what triggered a full re-check of the search method.

That re-check found the same padding gap had caused misses in the already-completed 1156
and 1157 audits (see the correction notes in those sections above) and one more within this
same hue's own random-color pool. **From 1163 onward, every hue in this batch is searched
for three encodings: decimal (`\bNNN\b`), CProp integer (`iNNN\b`), 3-digit hex (`0xNNN\b`),
and 4-digit zero-padded hex (`0x0NNN\b`).**

Search method: `\b1163\b`, `i1163\b`, `0x48b\b`, `0x048b\b` across all `.cfg`, `.src`,
`.inc` files.

### Crafting resources — pkg/std/mining/itemdesc.cfg

AnraOre and AnraIngot, both `Color 0x048b` (4-digit padded hex — the encoding that exposed
the search-method gap above). Changed to `0x0ab6`.

### Other crafting resource

`pkg/opt/farming/itemdesc.cfg:330` — turnipbulb, `color 1163` (decimal; item's `Name` field
is `turnipbulb`, its `Desc` is "turnip seed").

### NPCs — config/npcdesc.cfg

| NPC | Objtype |
|---|---|
| a Plum Ostard | 0xd2 |
| a Plum Frenzied Ostard | 0xda |

False positive (do not touch): `NpcTemplate spikedmonster` uses `objtype 1163`.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — `1163` in the `COLOURS` array.

### Extra — not on the owner's list

`pkg/std/fishing/sosarea.cfg:27` — an SOS bottle-spawn area marker (x 2507, y 2460,
range 25), `color 1163`.

### Random-color pool — the gap found within this hue itself

`scripts/misc/dressme.src:332` — `26: orecolor := 0x048b;`, option 26 of 34 in `DressWar`'s
ore-color case. Also 4-digit padded hex; missed on the first `dressme.src` pass for the same
reason, caught by the padding re-check. Changed to `0x0ab6`.

### Confirmed false positives (excluded, no color relationship to 1163/0x48b/0x048b)

- `NpcTemplate spikedmonster`'s `objtype 1163`
- Decoration `Y 1163`/`X 1163` coordinates and a `Decor 1163` element index
- Two "hedge_tele" town teleporter coordinates in `britannia` (`scripts/include/teleporters.inc`)
- `landtile 0x48b`, `SFX_48B := 0x48B`
- `pkg/opt/townstones/upgrades.cfg` and `pkg/utils/itemUtils/config/offset.cfg` — both files
  contain a large systematic block of `0x048b`-prefixed identifiers
  (`decoration_0x048b0`...`decoration_0x048bf`, `Item 0x048B`, etc.) that are graphic/objtype
  IDs, not colors — confirmed false positives, but worth knowing this pattern exists since
  it will resurface as noise for every hex value in this range as the batch continues

### Post-change verification

Re-ran the full sweep (`\b1163\b`, `i1163\b`, `0x48b\b`, `0x048b\b`) across every
`.cfg`/`.src`/`.inc` file after applying the edits. Only the false positives listed above
remain. No genuine color usage of 1163/0x48b remains.

### Revert procedure (1163 → 2742)

To roll back this hue move, restore each `2742` above to `1163` (decimal), each `0x0ab6`
above to `0x048b` (padded hex, AnraOre/AnraIngot/dressme.src). Uncommitted, so `git diff` /
`git checkout -- <file>` also works for a full or partial rollback before commit.

---

## Hue 1164 → 2743 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Owner's list was just Guild Colors, confirmed. One extra,
not on the list: `pkg/std/fishing/sosarea.cfg:34`, an SOS bottle-spawn area marker (x 2311,
y 2065, range 25), `color 1164`.

Search method: `\b1164\b`, `i1164\b`, `0x48c\b`, `0x048c\b` across all `.cfg`, `.src`,
`.inc` files. No padded-hex hits beyond the usual `townstones/upgrades.cfg` /
`itemUtils/config/offset.cfg` graphic-ID noise. No dressme.src hit.

### Confirmed false positives (excluded, no color relationship to 1164/0x48c)

- `NpcTemplate a flat spiked monster`'s `objtype 1164`
- Two `wanderinghealer` spawn-location coordinates
  (`pkg/mobiles/npcs/config/specialNPCs.cfg`)
- `Decor 1164` element indices (decoratefacets)

### Post-change verification

Re-ran the full sweep across every `.cfg`/`.src`/`.inc` file after applying the edits. Only
the false positives above remain. No genuine color usage of 1164/0x48c remains.

### Revert procedure (1164 → 2743)

To roll back this hue move, restore `2743` to `1164` at `pkg/std/fishing/sosarea.cfg:34` and
in the guild `COLOURS` array (`pkg/opt/guilds/include/guildconstants.inc:124`). Uncommitted,
so `git diff` / `git checkout -- <file>` also works for a full or partial rollback before
commit.

---

## Hue 1165 → 2744 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** All 34 items on the owner's list matched.

Search method: `\b1165\b`, `i1165\b`, `0x48d\b`, `0x048d\b` across all `.cfg`, `.src`,
`.inc` files.

**Revision note:** Rank1Robe carries the hue in two separate fields — the plain `Color`
field and a distinct `CProp StaticColor i1165` a few lines later — both needed changing or
the robe would keep a stale static-color override after its base color moved. Four extras
found, not on the list.

### Songbook — pkg/opt/songbook/itemdesc.cfg

SongBook (`Item 0x6177`) plus songscroll1 through songscroll16 (17 items total), all
identical `Color 1165` lines, changed with a single `replace_all`.

### NPCs — config/npcdesc.cfg

| NPC | Objtype |
|---|---|
| a Water Drake | 0x3d |
| a Tidal Drake | 0x3d |
| a Tidal Dragon | 0xc |

False positive (do not touch): a fully commented-out `// Counselor HAL` NPC block
(`objtype`/`Color`/`TrueColor` all commented out — dead code, not live); `NpcTemplate
wingedscorpiondaemon`'s `objtype 1165`.

### Tailoring — pkg/std/tailoring/itemdesc.cfg

- Rank1Robe — `Color 1165` **and** `CProp StaticColor i1165` (two separate fields, both
  changed)
- Angelhide — `Color 1165`

### Other crafting resource

`pkg/std/lumberjacking/itemdesc.cfg` — elvenlog.

### Fishing item

`pkg/std/fishing/itemdesc.cfg` — fishshell7, hex `0x48D`.

### Weapons/armor (9 items, all `CProp EnchantColor i1165`)

`pkg/systems/combat/config/itemdesc.cfg` — weaponofzulu (`Weapon 0x824d`, note this item
just happens to share the "zulu" name but carries hue 1165, not the Zulu hue from the 1765
audit) plus the 8-piece Elven armor set (ElvenFemalePlate, ElvenPlateHelm, ElvenPlateArms,
ElvenPlateLeggings, ElvenPlateGloves, ElvenPlateGorget, ElvenPlatemail, ElvenShield) —
changed with a single `replace_all`.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — `1165` in the `COLOURS` array.

### Extras — not on the owner's list

- `config/equip.cfg:2830` — `Equipment bardok`'s `Equip 0x6177 1165` — the same objtype as
  the SongBook itself (`Item 0x6177`), confirming this equip template is what puts a
  songbook in the "bardok" NPC's hands.
- `pkg/opt/omegacache/categories.cfg:82` — `SongBookScrolls 1165`, the same UI
  category-icon pattern as `EarthBookScrolls` (1162 audit).
- `pkg/std/fishing/sosarea.cfg:41` — an SOS marker (x 2521, y 1834, range 25).
- `scripts/misc/dressme.src` — `hidecolor := 1165;`, option 19 of 23, 2 occurrences.

### Confirmed false positives (excluded, no color relationship to 1165/0x48d)

- Commented-out `// Counselor HAL` NPC block and `NpcTemplate wingedscorpiondaemon`'s
  `objtype 1165` (see NPC section above)
- `config/signs.cfg:2340` — a `Y 1165` coordinate
- `config/teleporters.cfg:2215` — a `tox 1165` coordinate
- `Decor 1165` element indices (decoratefacets)
- `landtile 0x48d`, `SFX_48D := 0x48D`
- The usual `townstones/upgrades.cfg` / `itemUtils/config/offset.cfg` padded-hex graphic-ID
  noise, and mapgen decoration-key dumps

### Post-change verification

Re-ran the full sweep across every `.cfg`/`.src`/`.inc` file after applying the edits. Only
the false positives listed above remain. No genuine color usage of 1165/0x48d remains.

### Revert procedure (1165 → 2744)

To roll back this hue move, restore each `2744` above to `1165` (decimal) or `0x48D` (hex,
fishshell7) at the cited file/line. Uncommitted, so `git diff` / `git checkout -- <file>`
also works for a full or partial rollback before commit.

---

## Hue 1166 → 2745 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Owner's list was just Guild Colors. One real NPC found
during the search that wasn't on the list, plus the usual SOS-marker extra.

Search method: `\b1166\b`, `i1166\b`, `0x48e\b`, `0x048e\b`.

### NPC — config/npcdesc.cfg

an Acid Elemental (objtype 847), `Color`/`TrueColor` both `1166`. Not on the owner's list.

### Guild color picker

`pkg/opt/guilds/include/guildconstants.inc:124` — `1166` in the `COLOURS` array.

### Extra

`pkg/std/fishing/sosarea.cfg:48` — an SOS marker (x 2521, y 1411, range 25).

### Confirmed false positives (excluded, no color relationship to 1166/0x48e)

- `NpcTemplate a Hydra`'s `objtype 1166`
- `config/combat.cfg:1` — `# $Id: combat.cfg 1166 2008-02-05 ...`, a source-control
  revision-ID header comment, not game data
- `pkg/std/cooking/cooking.src` — two `item.objtype <= 0x048e` range checks (an objtype
  boundary comparison, not a color)
- Teleporter/wanderinghealer/decoration coordinates; `Decor 1166` element indices

### Post-change verification

Re-ran the full sweep after applying the edits. Only the false positives above remain.

### Revert procedure (1166 → 2745)

To roll back this hue move, restore each `2745` above to `1166` at the cited file/line.

---

## Batch: Hues 1168–1182 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Owner requested a more efficient pass after the 1155–1166
run involved many small round-trips: search all remaining hues at once instead of one at a
time. Search covered decimal, `iNNN` (CProp), 3-digit hex, and 4-digit padded hex for all 15
target hues in combined-pattern greps, narrowed to `itemdesc.cfg`/`npcdesc.cfg`/`equip.cfg`/
`dressme.src`/`guildconstants.inc`/`sosarea.cfg`/`categories.cfg` after an initial whole-repo
pass proved too noisy (this decimal range collides heavily with map coordinates in
`regions.cfg`/`teleporters.inc`). All results were compared against the owner's list and
presented as one consolidated report before implementing.

**Two names on the owner's lists are not live** (same pattern as CWReadyStone/townstone in
earlier audits): **CWPStone** (was on the 1172 list) and **ritualbook1–8 / ritualbook25–32**
(1174 and 1177 lists) — none exist anywhere in the repo except the stale `Used Colors.txt`
dumps. Nothing to find or revert for these.

**Post-implementation sweep caught 3 more misses**, found only because a second full sweep
was run after the "complete" edit pass — this is a genuine methodology finding, kept as a
warning for future hue audits: relying on the consolidated report's file list is not a
substitute for a real post-edit sweep, especially on a hue implemented from a large
pre-built plan rather than a fresh individual search:
- `pkg/opt/zuluitems/itemdesc.cfg` — Stygianpent1–9 (9 items, `Color 1174`), the Stygian
  counterpart to Firepent1–9, missed in the original 1174 comparison table
- `config/equip.cfg` — `Equipment lessershadow`'s `Equip 0x76d5 0x0494` /
  `Equip 0x76a4 0x0494` (hue 1172), missed alongside the `lessershadow`/`greatershadow`/
  `shadowlord`/`Equipment archangel` equip-template family scattered through this file
- `scripts/misc/dressme.src` — option 32 of 34 in `DressWar`'s ore-color case,
  `orecolor := 1176;`, sitting immediately after option 31 (1172) which was caught the first
  time

### Hue 1168 → 2746

Owner's list: Guild Colors only, confirmed. No other live occurrences found.

### Hue 1169 → 2747

| Item | Location |
|---|---|
| MassShuriken | `pkg/opt/GMItems/itemdesc.cfg`, `CProp EnchantColor i1169` |
| a Vortex Minion (objtype 756 instance) | `config/npcdesc.cfg` |
| a Poisonmare | `config/npcdesc.cfg`, objtype 0xc8 |
| a Gluttons (objtype 154, 1169 instance — a second "a Gluttons" NPC carries 1172, see below) | `config/npcdesc.cfg` |
| Greenbonehelm, Greenbonegloves | `pkg/systems/combat/config/itemdesc.cfg`, hex `0x0491` |
| Guild Colors | `guildconstants.inc` |

Extra, not on the list: `config/equip.cfg` — `Equipment greatershadow`'s 2 hex entries.

### Hue 1170 → 2748

All 29 owner's-list items matched: icearrow, PvPStone2vs2, JoustStone, MagePvPStone
(`config/itemdesc.cfg`); CrystalOre, CrystalIngot (mining, hex `0x0492`); icecrystalhide
(tailoring, hex); crystallog (lumberjacking); nightshadeseed (farming); lootbag
(`pkg/opt/lootlottery/itemdesc.cfg`); BagOfInfiniteReagents (`pkg/opt/shilitems/itemdesc.cfg`);
Icebow/SIcebow/MIcebow/StygianIcebow/KoboldIcebow/Bluebonehelm/Bluebonegloves (combat, hex
`0x0492`); Corruptcouncyshroud (combat, decimal); a Sapphire Ostard/Frenzied Ostard, a
Magusbane, a Mage Hunter, a Tempest, a Skeleton Protector, <random> the Ice Fiend, an Air
Drake, a Storm Dragon (npcdesc); Guild Colors.

Extras, not on the list: `config/equip.cfg` — `Equipment shadowlord`'s 2 hex entries,
`Equipment Corruptcouncy`'s `Armor 0xf701`; `scripts/items/bladed.src` — a special-log color
branch (`theitem.objtype == 0xb201`).

### Hue 1171 → 2749

All 10 weapon names matched, all `pkg/systems/combat/config/itemdesc.cfg`, hex `0x0493`:
Darkstranglerweapon, Taintedrangerweapon, magestalkerweapon, legendaryhunterweapon,
thorbowweapon, kappaweapon, vortexbowweapon, krakenbowweapon, lokebowweapon,
nagashrangerweapon. No Guild Colors — confirmed absent from the array (1171 is a genuine gap
in the sequence).

Extras, not on the list: `config/equip.cfg` — `Equipment taintedranger`'s `Weapon 0x9a1a` and
`Armor 0x76df`, `Equipment legendaryhunter`'s `Weapon 0x77df` (equip-template copies of the
same weapons above).

### Hue 1172 → 2750 (largest hue in this batch)

All 40 owner's-list items matched: DarkRuby/DarkRuby1 (mining); Rank4Robe (tailoring, +its
own `CProp StaticColor` field); the 5 Leather-of-fire pieces + maskoffire (tailoring);
dragonhide (tailoring, hex); GMFishingPole (`pkg/std/fishing/itemdesc.cfg`, CProp); sushi
(cooking); CWPStone — **not live**; Firepent1–9 (zuluitems); firearrow, mazeTile
(`config/itemdesc.cfg`); the Fire Element Shrine Lord, a Blama, a Vortex Minion (1172
instance), a Terramental, an Undead Flayer, a Gluttons (1172 instance), an Inferno
Drake/Dragon (npcdesc); Katanaoffire + 6 fire-armor pieces (combat, decimal);
firebow/Sfirebow/Mfirebow/Stygianfirebow (combat, hex); AnubissMaceOfDeath (combat, CProp);
Redbonehelm/Redbonegloves (combat, hex); refwep (combat); Guild Colors.

Extras, not on the list: `config/equip.cfg` — `Equipment referee`'s `Weapon 0x7ce9`,
**`Equipment lessershadow`'s 2 hex entries (caught only in the post-implementation sweep)**;
`scripts/misc/death.src` — a death-drop "heart" item color; `scripts/items/bladed.src` — a
special-log color branch (`objtype == 0x6050`); `scripts/items/pvp.src` — 24 occurrences of
`fnc.color := 1172;` (PvP arena fence posts, same pattern as `pvp2vs2.src` from the 1765
audit, just a different arena/file); `scripts/include/npccastspells.inc` — `"FIRE"` spell
color; `dressme.src` pool entry (option 31).

### Hue 1173 → 2751

a Wind Ostard, a Wind Frenzied Ostard, Guild Colors — all confirmed.

Extra, not on the list: `pkg/items/deed/config/itemdesc.cfg` — **8 Crystal-themed
statue/furniture deeds** (CrystalBeggarStatueDeed, CrystalBullStatueDeed,
CrystalRunnerStatueDeed, CrystalSupplicantStatueDeed, CrystalBrazierDeed, CrystalAltarDeed,
CrystalTableDeed, CrystalThroneDeed), all hex `0x495`, entirely new, unrelated in name to
anything else in this hue.

### Hue 1174 → 2752

Rank3Robe (+its own `CProp StaticColor`), StygianRobe, lichehide, fishshell9, an Amethyst
Ostard/Frenzied Ostard, Guild Colors all confirmed. The large Stygian weapon list (~35 items,
all `CProp Enchanted sStygian` + `CProp EnchantColor i1174` pairs in
`pkg/systems/combat/config/itemdesc.cfg`) was bulk-verified by pattern rather than
individually name-matched against the owner's ~40-name list, since every instance uses
identical text and was changed with a single `replace_all`. **ritualbook1–8 not live.**

Extras, not on the list: the Stygian Shrine Lord (npcdesc); `pkg/opt/astralfights/itemdesc.cfg`
— 2 more Stygian items ("Stygian Astral", "Stygian Psychic"); **`pkg/opt/zuluitems/itemdesc.cfg`
— Stygianpent1–9 (9 items), caught only in the post-implementation sweep.**

Note: **StygianIcebow** and **Stygianfirebow** appear on both the 1170 and 1174 owner-supplied
lists, but each only exists once in the file, at hex `0x0492` (1170) and `0x0494` (1172)
respectively — treated as belonging to those hues, not duplicated here.

### Hue 1175 → 2753

an Ancient Wyrm, Guild Colors confirmed.

Extras, not on the list: a Blood Elemental (npcdesc, objtype 853); `config/equip.cfg` — a
leather-boots `Equip 0x170b` entry.

### Hue 1176 → 2729 (note: target breaks the otherwise-sequential numbering)

RadiantDiamond/RadiantDiamond1 (mining), sunlog (lumberjacking), cottonseed (farming),
antihouse (`config/itemdesc.cfg`), a Finntroll, a Frost Dragon (npcdesc), ScalpelofTrevize
(combat, CProp), Guild Colors — all 8 confirmed. 2729 was already present in the guild
array's reserved `2706`–`2755` block before this edit (duplicate now exists there, per the
owner's standing "allow duplicates" decision from the 1155 audit).

### Hue 1177 → 2755

ExecutorOre/ExecutorIngot (mining, hex `0x0499`), fishshell6 (fishing, hex), swamplog
(lumberjacking), bookofverse (`pkg/opt/versebook/itemdesc.cfg`), a Cave Drake/Dragon
(npcdesc), Chaosshieldguard (combat), Guild Colors — all confirmed. **ritualbook25–32 not
live.**

### Hue 1178 → 2756

Guild Colors only, confirmed. No other live occurrences.

### Hue 1179 → 2757

Guild Colors only, confirmed. No other live occurrences.

### Hue 1180 → 2758

PeachBlueOre, PeachBlueIngot (mining, hex `0x049c`), Guild Colors confirmed. "a Werewolf"'s
`objtype 1180` (one of 6 separate "a Werewolf" NPC entries in the file) verified as the usual
false positive — its actual color is unrelated hex `0x0455`.

### Hue 1181 → 2759

QuestToken (`config/itemdesc.cfg`), the Water Element Shrine Lord, a Heavenly
Ostard/Frenzied Ostard/Drake (npcdesc), SpearofRenah + 6 Bone-of-Terror items (combat, all
`CProp EnchantColor i1181`, 7 total), Guild Colors — all 12 confirmed.

Extra, not on the list: `config/equip.cfg` — "Equipment 1"'s `Equip 0x1f03`.

### Hue 1182 → 2760

DripstoneOre, DripstoneIngot (mining, hex `0x049e`), a Spawn of the Storm, <random> the
Fallen Angel, Guild Colors — all confirmed. The Fallen Angel is unusual: its base `Color`
field is `0` and only `TrueColor` carries `1182` — only `TrueColor` was changed, `Color`
stays `0`.

Extras, not on the list: Jill, The Soul Whisperer (npcdesc, hex `0x049e` — third sibling to
Joe [1158] and Carrie [1160]); 3 occurrences of `mount 0x3eaa 1182`, all changed via a single
`replace_all`.

### Standard false positives (all 15 hues)

Consistent with every prior hue in this audit: `NpcTemplate`-block `objtype` coincidences
(one per hue in this batch, all in `config/npcdesc.cfg`); map/region/teleporter coordinates;
`Decor NNNN` element indices; `landtile`/`SFX_` graphic and sound constants;
`townstones/upgrades.cfg` and `itemUtils/config/offset.cfg`'s systematic padded-hex
graphic/objtype ID blocks; mapgen decoration-key dumps; the stale `Used Colors.txt` /
`Converted_Used_Colors.txt` reference dumps.

### Post-change verification

Re-ran the full combined-pattern sweep (decimal, CProp, 3-digit hex, 4-digit padded hex, all
15 hues at once) across `itemdesc.cfg`/`npcdesc.cfg`/`equip.cfg`/`dressme.src` after the
first edit pass, which is what caught the 3 misses listed above. Re-ran again after fixing
those; only the standard false positives remain. No genuine color usage of 1168–1182
remains.

### Revert procedure (batch)

To roll back any hue in this batch, restore its cited locations from the new value back to
the original decimal/hex listed in that hue's subsection above. All edits are uncommitted,
so `git diff` / `git checkout -- <file>` also works for a full or partial rollback.

---

## Hue 1170 re-target: 2748 → 2243 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Hue 1170 was migrated to `2748` in an earlier session (see
the 1168–1182 batch above). The shard owner corrected the target afterward: "Hue audit 1170 →
2748 needs to go to 2243." This section finds and re-points every live `2748` that came from
that migration; it does not touch anything that was `2748` for unrelated reasons.

Search method: `\b2748\b` across `.cfg`/`.src`, narrowed from an initial 28-file whole-repo hit
list down to color-bearing files, since `2748` collides constantly with map coordinates
(`regions.cfg`, `specialNPCs.cfg`) and padded-hex graphic/objtype blocks
(`townstones/upgrades.cfg`, `itemUtils/config/offset.cfg`, decoratefacets decoration dumps).

### Confirmed hue-1170 locations (2748 → 2243)

| File | Item(s) |
|---|---|
| `config/npcdesc.cfg` | Sapphire Ostard, Sapphire Frenzied Ostard, Magusbane, Mage Hunter, Tempest, Skeleton Protector, `<random>` the Ice Fiend, an Air Drake, a Storm Dragon (`Color`+`TrueColor`, 9 templates) |
| `config/equip.cfg` | `Equipment Corruptcouncy` — Armor `0xf701` |
| `pkg/systems/combat/config/itemdesc.cfg` | Corruptcouncyshroud |
| `config/itemdesc.cfg` | PvPStone2vs2, JoustStone, MagePvPStone |
| `pkg/std/lumberjacking/itemdesc.cfg` | crystallog |
| `pkg/opt/farming/itemdesc.cfg` | nightshadeseed |
| `scripts/items/bladed.src` | exceptional-craft color bonus for objtype `0xb201` |
| `pkg/opt/shilitems/itemdesc.cfg` | BagOfInfiniteReagents |
| `pkg/opt/lootlottery/itemdesc.cfg` | LootBag |
| `pkg/opt/guilds/include/guildconstants.inc` | `COLOURS` array — only the entry written in during the original 1170 migration (`...2747, 2748, 1184...` → `...2747, 2243, 1184...`); the unrelated `2748` sitting inside the pre-existing 2706–2755 reserved block later in the same array was left untouched (not part of this hue) |

### Confirmed false positives (unchanged)

Coordinates in `regions.cfg`/`specialNPCs.cfg` (`wanderinghealer 2748 660 0`); padded-hex
graphic/objtype blocks in `townstones/upgrades.cfg` and `itemUtils/config/offset.cfg`;
`landtiles.cfg`/`tiles.cfg`/decoratefacets decoration dumps; `clilocs.cfg` cliloc IDs.

### Post-change verification

Full sweep for `2748` re-run after the edit; only the known false-positive files remain, plus
the one intentionally-untouched reserved-block entry in `guildconstants.inc`.

### Revert procedure

Restore the locations above from `2243` back to `2748`. Uncommitted — `git diff` /
`git checkout -- <file>` also works.

---

## Batch: Hues 1301–1306, 1280, 1281 — 2026-08-14

**STATUS: APPLIED 2026-08-14.** Combined into one pass per the owner's efficiency request
("do it all once"). Search method: combined regex (decimal, `iNNN` CProp form, 3-digit hex,
4-digit padded hex) run once across all eight target values, narrowed to
`itemdesc.cfg`/`npcdesc.cfg`/`equip.cfg`/`dressme.src`/`guildconstants.inc`/other package
`itemdesc.cfg` files after an initial broad pass turned up the usual coordinate/graphic/ID
false-positive noise.

### 1301 (0x515) → 2819

Owner's list: Rank2Robe, a Valley Ostard, a Valley Frenzied Ostard, a Celestial Drake, a
Celestial Dragon, OmerosPickaxe, Guild Colors — **all 7 matched, no extras.**

- `pkg/std/tailoring/itemdesc.cfg` — Rank2Robe
- `config/npcdesc.cfg` — valleyostard, valleyfrenziedostard, celestialdrake, celestialdragon
  (`Color`+`TrueColor`)
- `pkg/systems/combat/config/itemdesc.cfg` — Omero's Pickaxe (`CProp EnchantColor i1301`)
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array

### 1302 (0x516) → 2820

Owner's list: `<random>` the Tainted One (x2), Armor30Shroud, DarkBlueGloves,
DarkBlueStuddedTunic, DarkBlueBoneArms, DarkBlueBoneHelm, DarkBlueBoneGloves,
DarkBlueBoneLeggings, DarkBlueBoneTunic, Guild Colors — **all matched, plus one extra.**

- `config/npcdesc.cfg` — taintedwarrior1, taintedwarrior2 (`<random>` the Tainted One x2,
  `Color`+`TrueColor`)
- **Extra (not on owner's list):** `config/npcdesc.cfg` — `mount 0x3ea4 1302`, the tainted
  mage's mount color (x3 identical lines) — migrated since it's the same family
- `pkg/systems/combat/config/itemdesc.cfg` — Armor30Shroud, DarkBlueGloves,
  DarkBlueStuddedTunic, DarkBlueBoneArms, DarkBlueBoneHelm, DarkBlueBoneGloves,
  DarkBlueBoneLeggings, DarkBlueBoneTunic
- `config/equip.cfg` — `Equipment taintedranger/taintedmage/taintedwarrior1/taintedwarrior2`,
  hex `0x0516` → `0x0B04` (redundant color copies of the armor pieces above)
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array

### 1303 (0x517) → 2821

Owner's list: Guild Colors only — **matched, nothing else live at this value.**

- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array only

### 1304 (0x518) → 2822

Owner's list: a Corrupted Terathan, a Corrupted Shadow — **both matched, plus one extra.**
Note: 1304 was never in the guild `COLOURS` array to begin with (array jumps `1303, 1305`
directly) — consistent with "Guild Colors" not being on this hue's owner-supplied list, no
array edit needed.

- `config/npcdesc.cfg` — corruptedterathan, corruptedshadow (`Color`+`TrueColor`)
- **Extra (not on owner's list):** `pkg/opt/rituals/config/itemdesc.cfg` — RitualRobe
  (Desc "ritual robe") — migrated since it's a genuine live use of the color

### 1305 (0x519) → 2823

Owner's list: fishshell2, shrunknpc, draugrsfang, NavarBloodyBarrier, Guild Colors — **all 5
matched, no extras.**

- `pkg/std/fishing/itemdesc.cfg` — fishshell2, hex `0x519` → `0xB07`
- `pkg/opt/shrink/itemdesc.cfg` — shrunknpc
- `pkg/systems/combat/config/itemdesc.cfg` — Draugr's Fang, Navar's Bloody Barrier
  (`CProp EnchantColor i1305`)
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array

### 1306 (0x51A) → 2824

Owner's list: SparksFemalePlate, SparksPlateHelm, SparksPlateArms, SparksPlateLeggings,
SparksPlateGloves, SparksPlateGorget, SparksPlatemail, SparksShield, Guild Colors.

**All 8 Sparks items exist only as commented-out (`//`) `CProp EnchantColor i1306` lines in
`pkg/systems/combat/config/itemdesc.cfg` — currently inactive/unused code, not live items.**
Updated the value inside the comments anyway (`i1306` → `i2824`) so they're correct if ever
re-enabled, rather than leaving a stale value sitting in disabled code.

- `pkg/systems/combat/config/itemdesc.cfg` — 8x commented `CProp EnchantColor i1306` → `i2824`
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array

### 1280 (0x500) → 2298

Owner's list: DestructionOre, DestructionIngot, a Terror Ostard, a Terror Frenzied Ostard, the
Stomp, Guild Colors — **all 6 matched, no extras.**

- `pkg/std/mining/itemdesc.cfg` — DestructionOre, DestructionIngot, hex `0x0500` → `0x08FA`
- `config/npcdesc.cfg` — terrorostard, terrorfrenziedostard, stomp (`Color`+`TrueColor`)
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array
- **False positive excluded:** `config/npcdesc.cfg` line 51394, `CProp BaseStrmod i1280` — a
  stat modifier on an unrelated `undead_dragon`-adjacent template, not a color

### 1281 (0x501) → 2299

Owner's list: GoddessOre, GoddessIngot, Dreamhide, fishshell5, goddesslog, Guild Colors —
**4 of 5 real items matched; Dreamhide is not live.**

- `pkg/std/mining/itemdesc.cfg` — GoddessOre, GoddessIngot, hex `0x0501` → `0x08FB`
- `pkg/std/lumberjacking/itemdesc.cfg` — goddesslog
- `pkg/std/fishing/itemdesc.cfg` — fishshell5, hex `0x501` → `0x8FB`
- `scripts/misc/dressme.src` — `hidecolor := 1281` (dressme random-color pool, x2 identical
  case entries)
- `pkg/opt/guilds/include/guildconstants.inc` — `COLOURS` array
- **Not currently live:** Dreamhide only exists commented-out in
  `pkg/std/tailoring/itemdesc.cfg` (`# Name Dreamhide ... # Color 1281`) and in the stale
  `Used Colors.txt`/`Converted_Used_Colors.txt` dumps — same pattern as CWReadyStone,
  ritualbook1-8/25-32, etc. from earlier audits. No live edit made.

### Confirmed false positives (all sub-hues, unchanged)

`sets.cfg` lines where the value matched the ObjType/Graphic field, not Color (confirmed
against the file's own `//Item ObjType Graphic Color X Y Z DESC` header — e.g. `item 1306
1306 0 ... stone pavers` has Color `0`); resource-graphic blocks in `config/itemdesc.cfg`
(`rescob1`-`rescob4`, `Graphic 0x515`-`0x518`); `0x512` hits in combat itemdesc.cfg
(CoifOfWonders/TunicOfWonders/etc. and shilitems' Trash Can of Wonders — `0x512` = 1298
decimal, outside this batch's range, caught by an overly broad hex regex and excluded);
`regions.cfg`/`specialNPCs.cfg` `wanderinghealer` coordinates; `townstones/upgrades.cfg` and
`itemUtils/config/offset.cfg` padded-hex graphic/objtype blocks.

### Post-change verification

Re-ran the combined sweep across all touched files after the edit pass; only the false
positives above remain.

### Revert procedure

Restore each file's cited locations from the new value back to the original decimal/hex
listed in its subsection above. Uncommitted — `git diff` / `git checkout -- <file>` also
works.
