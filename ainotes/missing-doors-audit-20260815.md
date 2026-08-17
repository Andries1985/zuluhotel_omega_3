# Missing doors audit — pkg/items/doors/config/itemdesc.cfg vs tiledata.mul

Generated 2026-08-15 while investigating why graphic `0x41CF` (Gargish Grey Door,
16847 decimal) doesn't open. See [[project_305_multis_door_open_graphic_bug]]
memory for the mechanism (no `Door 0xNNNN {}` block ⇒ engine never classifies
the objtype as `POLCLASS_DOOR` ⇒ item.Open()/Close() and the `:door` command
never fire).

## Method

Parsed `mul/tiledata.mul` directly for every item id `0x0000`-`0xFFFF` with the
Door flag (`0x20000000`) set — 558 total. Parsed `pkg/items/doors/config/itemdesc.cfg`
for every `Door 0xNNNN` block (192) and every `OpenGraphic 0xNNNN` value they
reference (192 — one per closed door). A tile is only a genuine gap if it's
**neither** registered as a closed door **nor** referenced as someone else's
open-state graphic — 182 tiles fail both checks. (368 tiles look "missing" if
you only check the `Door` blocks, but 186 of those are just the correctly-unregistered
open-graphic companions of doors that already work fine — don't re-add those.)

## Result: 182 door-flagged tiles with zero itemdesc.cfg registration

Every one of these is a real, correctly door-flagged tiledata tile that will
sit inert in-game — clicking it does nothing, `:door` command skips it, no
sound, no swing. All are newer-era (post-classic) door art; the classic T2A/AOS
door catalog is fully registered.

| tiledata name | count | ids |
|---|---|---|
<!-- | bar door | 2 | `0x190E`-`0x190F` | -->
| door | 24 | `0x241F`-`0x2424`, `0x26F4`, `0x26F6`, `0x31A0`-`0x31AF` |
<!-- | moon door | 4 | `0x319C`-`0x319F` |
| crystal wall | 2 | `0x35E7`-`0x35E8` |
| shadow door | 7 | `0x3640`-`0x3646` | -->
| Wallset3 DoorLeft South | 2 | `0x409B`-`0x409C` |
| Wallset3 DoorRight South | 2 | `0x409D`-`0x409E` |
| Wallset3 DoorLeft East | 2 | `0x409F`-`0x40A0` |
| Wallset3 DoorRight East | 2 | `0x40A1`-`0x40A2` |
| GargoyleDoor South | 4 | `0x410C`-`0x410F` |
| GargoyleDoor East | 4 | `0x4110`-`0x4113` |
| Wallset2 Doors South (Sun Door) | 4 | `0x41C2`-`0x41C5` |
| Wallset2 Doors East (Sun Door) | 4 | `0x41C6`-`0x41C9` |
| **Wallset1 DoorLeft South (Gargish Grey Door)** | 2 | **`0x41CF`-`0x41D0`** |
| Wallset1 DoorRight South | 2 | `0x41D1`-`0x41D2` |
| Wallset1 DoorLeft East | 2 | `0x41D3`-`0x41D4` |
| Wallset1 DoorRight East | 2 | `0x41D5`-`0x41D6` |
| Door A1a-D2b South/East (Gargish Set Door) | 16 | `0x436E`-`0x437D` |
| RuinDoor South | 4 | `0x46DD`-`0x46E0` |
| RuinDoor East | 4 | `0x46E1`-`0x46E4` |
| QueenDoorH South | 4 | `0x4D1A`-`0x4D1D` |
| QueenDoorH East | 4 | `0x4D1E`-`0x4D21` |
| QueenDoor South (Gargish Blue Door) | 4 | `0x4D22`-`0x4D25` |
| QueenDoor East (Gargish Blue Door) | 4 | `0x4D26`-`0x4D29` |
| Door South (Gargish Red Door + friends) | 13 | `0x50C8`-`0x50CB`, `0x50D0`-`0x50D3`, `0x5142`, `0x5144`-`0x5146`, `0x5148` |
| Door East (Gargish Red Door + friends) | 10 | `0x50CC`-`0x50CF`, `0x50D4`-`0x50D7`, `0x5147`, `0x5149` |
<!-- | QC Wall b East | 1 | `0x5128` |
| QC Wall b South | 1 | `0x5129` | -->
| Door South01 | 1 | `0x5143` |
| metal door (castle-era) | 32 | `0x9AD7`-`0x9AE6`, `0x9B3C`-`0x9B4B` |
| wooden gate (castle-era) | 13 | `0xA4BF`-`0xA4CB` |

This lines up with the multis already flagged broken in [[project_305_multis_door_open_graphic_bug]]
(Gothic Rose Castle, Castle of Oceania, Sandalwood Keep, Keep Incarcerated,
Sally Trees Refurbished Keep, Clovers Keep, Terrace Gardens) — every door
graphic on that list falls in one of the families above.

## Closed/open pairing — confirmed vs guessed

**Do not assume tiledata's same-name-adjacency = closed/open pair.** For the
Wallset1 (Gargish Grey Door) family the user checked the actual client art and
found the pairing crosses the adjacent-name grouping:

- `0x41CF` (DoorLeft South, closed) → open state is `0x41D2`
- `0x41D1` (DoorRight South, closed) → open state is `0x41D0`

i.e. NOT simply `+1`/`-1` from each other, and not what the tiledata name
pairing would suggest (`CF`+`D0` share a name, `D1`+`D2` share a name — the
real open/closed correspondence is the opposite of that). East-facing
(`0x41D3`/`0x41D5`) likely follows the same left/right-swap pattern by
structural symmetry (`0x41D3`→`0x41D6`, `0x41D5`→`0x41D4`) but this is an
unverified guess — confirm in UOFiddler before using it.

Every other family in the table above has **no confirmed pairing yet** — get
it from the actual art (UOFiddler, see [[reference_uofiddler_repo]]) per
family before writing `Door` blocks. Don't reuse the Wallset1 crossed-pairing
pattern for other families without checking; there's no reason to assume they
all share the same art-authoring convention.

## Next step

For each family: confirm closed↔open pairing and XMod/YMod slide direction in
UOFiddler, then add a `Door 0xNNNN { ... }` block per closed graphic to
`pkg/items/doors/config/itemdesc.cfg`, following the existing block format
(see `Door 0x00E8` at the top of that file for the template).
