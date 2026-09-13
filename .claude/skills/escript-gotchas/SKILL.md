---
name: escript-gotchas
description: "EScript (POL2.5/POL3.0) compiler gotchas and this repo's own conventions - foreach dotted-RHS restriction, module-name collisions (target/Target), include resolution rules, cfgfile keyless-block lookups, DataFile open/create/unload pattern, objtype relocation, new deed-item workflow, textcmd synopsis sync, print-vs-sysmessage, ecompile policy. TRIGGER before writing or editing any .src/.inc file in this repo, before adding a new craftable/deed item, before relocating an objtype, before adding/renaming a textcmd script, or before reading a .cfg block via cfgfile.em."
---

# EScript gotchas & repo conventions (POL3.0 / zuluhotel_omega_3)

Checklist to run through before/while writing or editing EScript in this repo. These are mistakes that have actually been made and corrected multiple times in this codebase — check them proactively, don't wait for a compile error or user correction to catch them.

## Compiler-level gotchas (apply to POL2.5 too)

**1. `foreach x in EXPR` — EXPR must be a plain variable, never a dotted expression.**
`foreach x in record.entries` or `foreach x in who.backpack` fails to compile ("mismatched input '.' expecting 'endforeach'"). Always pre-assign first:
```
var entries := record.entries;
foreach entry in entries
    ...
endforeach
```
Hit 10+ times historically — check every new `foreach ... in ...` for a dotted RHS before considering the edit done.

Also: `foreach x in someArray` auto-provides `_x_iter` (1-based index) with no declaration needed. Don't mistake a bare `_x_iter` reference for a dead/undeclared variable — check for an enclosing `foreach x in ...` first.

**2. Never name a function or local var the same as a `.em` builtin (case-insensitive).**
Classic repeat offender: `var target := Target(character);` collides with `uo.em`'s `Target()` and fails to compile. Use `targetted` instead, every time, on the first pass — don't write `target` and plan to fix it later. For other builtins, pick a specific name rather than a generic verb.

**3. `include` resolution:**
- `include "name";` — next to the calling `.src`.
- `include "include/name";` — under `scripts/include`.
- `include ":pkgName:name";` — resolves to `name.inc` inside that package — **always `.inc`, never `.src`**, even if the target `.src` file exists and has an `exported function`. If a colon-qualified include target has a `program` block, split it: a real `.inc` (no `program`, holding the exported function + everything it calls) plus a thinned `.src` that includes the new `.inc`. Cheaper alternative to check first: can the new caller just reuse the same `Script :pkg:name` value directly (no dispatch needed)? Many crafting-tool programs are just `ReserveItem` + open a gump with no per-item logic.
- Include paths are case-sensitive against the actual filename on disk even on Windows — `include "include/eventID"` fails with "Case mismatch" if the real file is `eventid.inc`.
- No confirmed cycle protection — keep the include graph a strict DAG by hand.
- Global vars declared in an include become global in the including script — don't reuse global var names across includes used together.

**4. `cfgfile.em` block lookups: a one-token block header keys as `""`, not the header word.**
`Global\n{` (used by `regions/{fish,ore,wood,sand,clay}.cfg`) has no name after `Global`, so `FindConfigElem(cfg, "Global")` always fails — use `FindConfigElem(cfg, "")`. Only a two-line header like `Region Background\n{` keys as `"Background"`. Check for a bare single-word header before assuming the word itself is the key.

**5. Data type choice:** array (ordered/multi-return), struct (fixed known named fields, dot-access, `.+field` to add a new member), dictionary (dynamic key-value, `.Exists()`/`.Keys()`). Structs/dicts are the standard way to shape CProp/GProp data.

## This repo's own conventions

**6. DataFile (`use datafile;`) open/create/unload pattern** — always: `OpenDataFile` first, `CreateDataFile` only if that failed, then `UnloadDataFile` on **every** return path of the function that opened it.
```
var df := OpenDataFile(FILESPEC);
if(!df)
    CreateDataFile(FILESPEC, flags := DF_KEYTYPE_STRING);
    df := OpenDataFile(FILESPEC);
endif
```

**7. Debug/trace output means `Print(...)` to the server console, not `SendSysmessage`.** Don't add `use basicio;` defensively — match whatever the file already declares (`use uo;`/`use os;` is enough in practice).

**8. Don't run `ecompile.exe`** (or a Bash/PowerShell equivalent) on the user's behalf, and don't append a "please compile" reminder — the user compiles to launch the shard anyway. No standing exception currently applies.

**9. Textcmd changes require a regen, and this one you DO run yourself:** creating/renaming a `.src` under `scripts/textcmd/<level>/` or `pkg/**/textcmd|commands/<level>/`, or changing its `// Synopsis:` line, means running `python pythonscripts/_gen_command_synopses_cfg.py` from repo root immediately — proactively, not just flagged. Never hand-edit `config/command_synopses.cfg`.

**10. `.create <name>` spawns an itemdesc item; `.createnpc <name>` spawns an npcdesc NpcTemplate.** They are not interchangeable — check which kind of block the target is before giving a spawn command.

**11. Max skill on this shard is 150, not the vanilla 100.** Use 150 as the GM/max reference point in any skill-scaled formula or example.

**12. Before concluding a stat "never changes," check whether it's a live-computed getter, not just whether anything writes to it or whether save files ever show an override.** POL only serializes a stored field when it diverges from default — a formula evaluated on read (e.g. `UArmor::ar() = ar_base * hp/maxhp()`) will never show up as a save-file override no matter how much it varies. Escalate to the engine source's accessor (not just mutators) before asserting "static."

**13. Don't use a `"#"`-prefixed key for anything that must survive a server restart** (`GetObjProperty`/`SetObjProperty`). Plain (no `#`) keys on the character mobile are confirmed durable (`setph.src`'s `pph_use_time` pattern); `#`-prefixed ones are for short-lived runtime flags only.

**14. When porting a POLMD gump, include `:mdgumps:gumps` / `:mdgumps:gumps_ex`, never `pkg/utils/gumps`.** Keeps POLMD-sourced GF*-function code isolated from this repo's own hand-maintained gump copy.

**15. Watch for leftover test-tuning values in crafting/loot code.** A `Random(N)` roll that doesn't match its sibling skills' pattern (e.g. `Random(150)` for deed-capable exceptional rolls) may be a temporarily-loosened test value that was never reverted — cross-check against sibling implementations, and check `git blame` for a commit that strips a debug `Print()`/commented-out real value without also fixing the live number.

## New craftable/deed item checklist (abbreviated — see memory for full detail)

- Rotate-script items (`Script :itemutils:rotate/rotate`) never get a deed — craft them direct (`ApplyMaterialProperties` + `CreateItemInBackpack`), no `MakeDeed`.
- Before inventing Material/Skill/CoSkill numbers, grep POLMD's actual `pkg/skills/<skill>/config/<skill>.cfg` for the objtype — if it's not there, POLMD never made it craftable; don't invent a recipe.
- Every deeded item needs a `pkg/items/deed/built/<name>.cfg` — `deed/use.src` hard-requires a non-blank `BuildCFG`.
- `CoSkillid` only works via the Ingots/Cloth/Bones branch in `carpentry.src` — never the Logs branch, and never at all in `tinkering.src` (no second-resource mechanism exists there).

## Objtype relocation checklist (abbreviated)

- Verify the target range is free: check every `Item`/`Weapon`/`Armor`/etc. key across all `itemdesc.cfg` repo-wide, then grep hex AND decimal forms of the id, excluding known universal per-graphic catalogs (`config/tiles.cfg`, `landtiles.cfg`, `offset.cfg`, etc.).
- Graphic-ID ceiling is `0xB4E3` (real client graphics sit at/below it). When an old custom/server-only marker item collides with a newer real-graphic item, move the OLD item — unless the old item has its own separate `Graphic` field pointing elsewhere, in which case IT is squatting on a spare number and isn't the one that needs to move. Safe relocation targets: `0x303C8`-`0x3FFFF`.
- After renaming, fix every reference repo-wide (consts, inline equality/range checks, random-offset math) — case-insensitively. Never hand-edit `objtypes.txt` or anything under `data/`.

## Tools available for this repo specifically

- **Local shallow clones** at `D:\Zuluhotel2\_reference-repos\` — `engine\` (polserver/polserver), `pol25\` (zuluhotel_omega_2.5), `polmd\` (ModernDistro). Grep/Read/Glob these directly instead of WebFetch/WebSearch for engine-source questions or POL2.5/POLMD porting comparisons — much faster, works offline. They're stale snapshots from clone time; `git pull` (or `fetch --depth 1` + `reset --hard origin/<branch>`) inside the relevant subfolder first if the task needs current upstream state.
- **Engine behavior questions** (damage caps, vitals scaling, module function semantics) go to the `engine\` clone's `pol-core/pol/module/*.cpp`, not this repo's scripts.
- **POL3.0** = this repo. **POL2.5** = separate shard, don't assume a fix here applies there or vice versa without checking.
