#!/usr/bin/env python
"""
Generate pkg/opt/alryc/config/animatedgraphics.cfg from config/tiles.cfg
or an animdata JSON export.

When using tiles.cfg, the script scans tile blocks, selects tiles where
UoFlags contains the animated bit (0x01000000), then writes groups of N
tiles (default 50) using this format:

Group 1
{
    Tile 0x21c
    Tile 0x3ae
    ...
}

Run from repository root:
    python pythonscripts/_gen_alryc_animatedgraphics_cfg.py

Optional arguments:
    --input config/tiles.cfg
    --animdata-json C:/path/to/animdata.json
    --exclude-cfg pkg/opt/alryc/config/animatedgraphics.cfg
    --min-tile 0x1
    --max-tile 0xB4E3
    --output pkg/opt/alryc/config/animatedgraphics.cfg
    --group-size 50
    --flag 0x01000000
"""

from __future__ import annotations

import argparse
import json
import os
import re
from dataclasses import dataclass
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent
ROOT = SCRIPT_DIR.parent if SCRIPT_DIR.name.lower() == "pythonscripts" else SCRIPT_DIR

DEFAULT_INPUT = ROOT / "config" / "tiles.cfg"
DEFAULT_OUTPUT = ROOT / "pkg" / "opt" / "alryc" / "config" / "animatedgraphics.cfg"

TILE_RE = re.compile(r"^\s*tile\s+(0x[0-9a-fA-F]+)\s*$")
UOFLAGS_RE = re.compile(r"^\s*UoFlags\s+([^\s]+)\s*$")
BLOCK_END_RE = re.compile(r"^\s*}\s*$")
DESC_RE = re.compile(r"^\s*Desc\s+(.+?)\s*$")
LAYER_RE = re.compile(r"^\s*Layer\s+([^\s]+)\s*$")


@dataclass
class TileEntry:
    tile_id: int
    flags: int
    desc: str
    has_layer: bool
    layer: int | None

    @property
    def is_animated(self) -> bool:
        return bool(self.flags & 0x01000000)

    @property
    def signature(self) -> tuple[str, int, bool, int | None]:
        # Use exact metadata signature to separate adjacent animation families.
        return (self.desc, self.flags, self.has_layer, self.layer)


def parse_int(value: str) -> int:
    return int(value, 0)


def collect_tiles(tiles_cfg: Path) -> list[TileEntry]:
    tiles: list[TileEntry] = []
    current_tile: int | None = None
    current_flags = 0
    current_desc = ""
    current_layer: int | None = None
    current_has_layer = False

    with tiles_cfg.open("r", encoding="utf-8", errors="replace") as file_handle:
        for raw in file_handle:
            line = raw.rstrip("\r\n")

            m_tile = TILE_RE.match(line)
            if m_tile:
                current_tile = parse_int(m_tile.group(1))
                current_flags = 0
                current_desc = ""
                current_layer = None
                current_has_layer = False
                continue

            if current_tile is None:
                continue

            m_flags = UOFLAGS_RE.match(line)
            if m_flags:
                try:
                    current_flags = parse_int(m_flags.group(1))
                except ValueError:
                    current_flags = 0
                continue

            m_desc = DESC_RE.match(line)
            if m_desc:
                current_desc = m_desc.group(1).strip()
                continue

            m_layer = LAYER_RE.match(line)
            if m_layer:
                try:
                    current_layer = parse_int(m_layer.group(1))
                    current_has_layer = True
                except ValueError:
                    current_layer = None
                    current_has_layer = False
                continue

            if BLOCK_END_RE.match(line):
                tiles.append(
                    TileEntry(
                        tile_id=current_tile,
                        flags=current_flags,
                        desc=current_desc,
                        has_layer=current_has_layer,
                        layer=current_layer,
                    )
                )
                current_tile = None
                current_flags = 0
                current_desc = ""
                current_layer = None
                current_has_layer = False

    return tiles


def _same_run(prev_tile: TileEntry, tile: TileEntry) -> bool:
    return (
        prev_tile.tile_id + 1 == tile.tile_id
        and prev_tile.is_animated
        and tile.is_animated
        and prev_tile.signature == tile.signature
    )


def collect_animated_run_starts(tiles_cfg: Path) -> list[int]:
    entries = collect_tiles(tiles_cfg)
    starts: list[int] = []

    for idx, tile in enumerate(entries):
        if not tile.is_animated:
            continue

        prev_tile = entries[idx - 1] if idx > 0 else None
        next_tile = entries[idx + 1] if idx + 1 < len(entries) else None

        is_run_start = prev_tile is None or not _same_run(prev_tile, tile)
        has_run_continuation = next_tile is not None and _same_run(tile, next_tile)

        # Keep only the first tile for multi-frame runs.
        if is_run_start and has_run_continuation:
            starts.append(tile.tile_id)

    return starts


def collect_animdata_tile_ids(animdata_json: Path) -> list[int]:
    with animdata_json.open("r", encoding="utf-8", errors="replace") as file_handle:
        data = json.load(file_handle)

    headers = data.get("Data")
    if not isinstance(headers, dict):
        raise ValueError("animdata JSON does not contain a top-level 'Data' object")

    tile_ids: list[int] = []
    for key in headers:
        try:
            tile_ids.append(int(key))
        except (TypeError, ValueError) as exc:
            raise ValueError(f"animdata JSON contains a non-numeric tile key: {key!r}") from exc

    return sorted(set(tile_ids))


def collect_cfg_tile_ids(cfg_path: Path) -> list[int]:
    text = cfg_path.read_text(encoding="utf-8", errors="replace")
    tile_ids = [int(match.group(1), 16) for match in re.finditer(r"^\s*Tile\s+(0x[0-9a-fA-F]+)\s*$", text, re.MULTILINE)]
    return sorted(set(tile_ids))


def collect_complement_tile_ids(exclude_cfg: Path, min_tile: int, max_tile: int) -> list[int]:
    if min_tile < 0:
        raise ValueError("--min-tile must be >= 0")
    if max_tile < min_tile:
        raise ValueError("--max-tile must be >= --min-tile")

    excluded = set(collect_cfg_tile_ids(exclude_cfg))
    return [tile_id for tile_id in range(min_tile, max_tile + 1) if tile_id not in excluded]


def write_grouped_cfg(output_cfg: Path, tiles: list[int], group_size: int) -> int:
    output_cfg.parent.mkdir(parents=True, exist_ok=True)

    lines: list[str] = []
    lines.append("// Auto-generated by _gen_alryc_animatedgraphics_cfg.py")
    lines.append("// Do not edit by hand. Re-run generator after source data changes.")
    lines.append("")

    group_count = 0
    for start in range(0, len(tiles), group_size):
        group_count += 1
        chunk = tiles[start : start + group_size]
        lines.append(f"Group {group_count}")
        lines.append("{")
        for tile in chunk:
            lines.append(f"    Tile {hex(tile)}")
        lines.append("}")
        lines.append("")

    output_cfg.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    return group_count


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate alryc animated graphics group cfg from tiles.cfg")
    parser.add_argument("--input", default=str(DEFAULT_INPUT), help="Path to tiles.cfg")
    parser.add_argument("--animdata-json", help="Path to animdata JSON export; when provided, uses JSON Data keys instead of tiles.cfg")
    parser.add_argument("--exclude-cfg", help="Path to a cfg whose Tile ids should be excluded from the generated output")
    parser.add_argument("--min-tile", default="0x1", help="Inclusive minimum tile id for --exclude-cfg mode")
    parser.add_argument("--max-tile", default="0xB4E3", help="Inclusive maximum tile id for --exclude-cfg mode")
    parser.add_argument("--output", default=str(DEFAULT_OUTPUT), help="Path to animatedgraphics.cfg")
    parser.add_argument("--group-size", type=int, default=50, help="Tiles per group")
    args = parser.parse_args()

    input_path = Path(args.input)
    output_path = Path(args.output)
    animdata_path = Path(args.animdata_json) if args.animdata_json else None
    exclude_cfg_path = Path(args.exclude_cfg) if args.exclude_cfg else None

    if animdata_path is None and exclude_cfg_path is None and not input_path.exists():
        print(f"ERROR: input file not found: {input_path}")
        return 1

    if animdata_path is not None and not animdata_path.exists():
        print(f"ERROR: animdata JSON file not found: {animdata_path}")
        return 1

    if exclude_cfg_path is not None and not exclude_cfg_path.exists():
        print(f"ERROR: exclude cfg file not found: {exclude_cfg_path}")
        return 1

    if args.group_size <= 0:
        print("ERROR: --group-size must be > 0")
        return 1

    if animdata_path is not None and exclude_cfg_path is not None:
        print("ERROR: use either --animdata-json or --exclude-cfg, not both")
        return 1

    if animdata_path is not None:
        try:
            tiles = collect_animdata_tile_ids(animdata_path)
        except ValueError as exc:
            print(f"ERROR: {exc}")
            return 1
        source_label = os.path.relpath(animdata_path, ROOT) if animdata_path.is_relative_to(ROOT) else str(animdata_path)
    elif exclude_cfg_path is not None:
        try:
            min_tile = parse_int(args.min_tile)
            max_tile = parse_int(args.max_tile)
            tiles = collect_complement_tile_ids(exclude_cfg_path, min_tile, max_tile)
        except ValueError as exc:
            print(f"ERROR: {exc}")
            return 1
        source_label = f"range {hex(min_tile)}..{hex(max_tile)} minus {(os.path.relpath(exclude_cfg_path, ROOT) if exclude_cfg_path.is_relative_to(ROOT) else str(exclude_cfg_path))}"
    else:
        tiles = collect_animated_run_starts(input_path)
        source_label = os.path.relpath(input_path, ROOT)

    group_count = write_grouped_cfg(output_path, tiles, args.group_size)

    rel_output = os.path.relpath(output_path, ROOT)
    print(f"Input: {source_label}")
    print(f"Output: {rel_output}")
    print(f"Animated tile entries found: {len(tiles)}")
    print(f"Groups written: {group_count}")
    print(f"Group size: {args.group_size}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())