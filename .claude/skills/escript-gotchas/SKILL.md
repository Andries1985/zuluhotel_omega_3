---
name: escript-gotchas
description: "EScript (POL2.5/POL3.0) compiler gotchas and this repo's own conventions - foreach dotted-RHS restriction, module-name collisions (target/Target), include resolution rules, cfgfile keyless-block lookups, DataFile open/create/unload pattern, objtype relocation, new deed-item workflow, textcmd synopsis sync, print-vs-sysmessage, ecompile policy, no short-circuit evaluation in and/or conditions, crafting consume-first-then-create pattern, SaveOnExit 0 for temporary items that must not survive a save or reboot, engine-attached use scripts (RequiresAttention) serialize item use per character, item.container of a worn item is the wearing mobile, two unset values (.party, absent CProps) compare equal so guard with truthiness first. TRIGGER before writing or editing any .src/.inc file in this repo, before adding a new craftable/deed item, before relocating an objtype, before adding/renaming a textcmd script, or before reading a .cfg block via cfgfile.em., file-scope var initialisers in includes run at every including script's start (use a lazy Get...() getter)"
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

**5a. There is NO short-circuit evaluation in this repo.** `scripts/ecompile.cfg` has `ShortCircuitEvaluation=0` (the engine default), so BOTH operands of `and`/`or`/`&&`/`||` are always evaluated. Two consequences:
- Never put a call with side effects in a compound condition — `if( !HasStuff() or !ConsumeResource(...) )` consumes even when the first half already failed. Use nested `if`s.
- `if( x and SomeCall(x) )` does NOT skip `SomeCall` when `x` is 0/error. Usually harmless (the call just returns an error), but it has real side effects when the call awards or writes something: `carpentry.src`'s `( skillid2 ) && !CheckSkill( character, cint(skillid2), ... )` runs a `CheckSkill` on skill id 0 (Alchemy) for every craft that has no co-skill. When reading old code, don't assume the right-hand side was skipped.

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

**16. Crafting scripts: consume first, check the result, then create.** `ConsumeResource()` (`scripts/include/resourcemanager.inc`) returns 0 when it comes up short (or only partly consumed) and has **no refund path**. Backpack material stacks are not reserved by the resource-manager flow, so a player can lift them out during a craft delay (`Sleep`/`_Play_Sound`) or while a quantity prompt is open. Create-then-consume with the result ignored therefore handed out free items, and a bulk path that capped its quantity before the prompt and never re-checked handed out whole free batches (found and fixed across ~10 scripts, 2026-09-21). The pattern:
```
if( CheckSkill( ... ) )
    if( !HasCraftingResources( character, req1, amt1, req2, amt2 ) )   // only needed for 2+ materials
        SendSysMessage( character, "You no longer have the materials for that." );
        break;      // or return -- but release any lease first
    endif
    if( !ConsumeResource( character, req1, amt1 ) )
        break;
    endif
    ...
    var theitem := CreateItemInBackpack( ... );
    if( !theitem )
        SendSysMessage( character, "Your backpack is full." );
        break;
    endif
```
- `HasCraftingResources()` must sit immediately before the consumes — no `Sleep`, gump or `Target` in between — so every material is confirmed before the first is charged.
- A full backpack still costs one set of materials, on purpose: `CheckSkill` has already paid out the skill gain, so a free failure would be a no-cost training loop.
- `break` out of an AutoLoop rather than `return`, so `ReleaseResourceLease` + `AutoLoop_finish` after the loop still run.
- Announce success (and award fame) only after the item really exists.
- Clamp bulk quantities to 60000 before charging — `CreateItemInBackpack` refuses a larger amount (`item_create_params_ok`), which used to charge the whole batch for nothing.
- For stackable output, set the colour on the item descriptor (`desc.Color := ...`) before creating, not on the item afterwards: the engine only merges into an existing stack when `item.color == descriptor.color`, so recolouring afterwards either repaints an existing plain stack or leaves a pile of unstackable singles.

**17. Use `SaveOnExit 0` for anything that must not outlive a save or a reboot.** The engine writes an item to the world save only when BOTH its itemdesc `SaveOnExit` (default 1) and the item's own `.saveonexit` member are true (`pol-core/pol/savedata.cpp`) — this applies to every periodic save, not just shutdown. Two ways to use it:
- **Whole item type:** `SaveOnExit 0` in its `itemdesc.cfg` block. Right for an objtype that is ONLY ever temporary — spell gates, splash/effect items, field tiles, markers a script creates and destroys itself.
- **One instance:** `item.saveonexit := 0;` right after creating it (the member is writable; NPCs have it too). Right when the objtype is also used for permanent items.

Why it matters: the usual "create it, `Sleep(n)`, destroy it" pattern has no cleanup if the script never reaches its destroy line — a server restart kills the script, but a saved item comes back without it. Real case (fixed 2026-09-21): the Gate spell's `Item 0x99b1` had `SaveOnExit 1`, so a save + restart inside its 30 s window left a permanent, immovable, never-decaying two-way gate. When writing a script that creates a short-lived item, check the objtype's `SaveOnExit` before relying on the script's own cleanup. Before flipping an existing objtype to 0, grep every creator of it (hex, case-insensitive) — anything that places it permanently would silently vanish on the next restart.

**18. The engine already serializes item use per character — don't invent "parallel use" exploits.** Every double-click use script starts *attached* to the character (`Item::double_click` → `start_itemuse_script(..., requires_attention)`, and `RequiresAttention` defaults to true in itemdesc), and while any script is attached (`chr->script_ex` set, or a spell in progress) a further double-click of a `RequiresAttention` item is refused with "I am already doing something else." `Attach(who)` fails the same way, and `UseItem()` goes through the same check. So a use script with a `Sleep`/loop and no `Detach()` blocks every other item use for its whole duration — "split the stack and double-click each pile" cannot run two heals at once (wrong finding, 2026-09-21). Only treat overlap as possible when the script `Detach()`es before its sensitive work, the action is reached via `Start_Script` without `Attach`, the itemdesc sets `RequiresAttention 0`, or the second copy is started by another character. The flip side: a script that *should* let the player keep acting (long background effects) must `Detach()` early, or it locks them out of every item.

**19. `item.container` of a worn item is the wearer, not a container.** The engine's `WornItemsContainer::make_ref()` returns the character (`pol-core/pol/mobile/wornitems.cpp`), so for equipped gear `item.container` is a mobile ref: `item.container.isa(POLCLASS_NPC)` is TRUE for something a hireling or pet wears, `.serial` is the wearer's serial, `.backpack` works. Don't call a `wfh := item.container; if( wfh.isa(POLCLASS_NPC) )` block dead (wrong finding, 2026-09-22 — it was the hireling undress path in the double-click packet hook). Only an unworn item has a container item there; when you need the top-level holder, walk `item.container` until it is no longer a container and treat a mobile at the top as "equipped on".

**20. Two "unset" values compare EQUAL: `.party`, `.master`, an absent CProp.** A character with no party gets `BError("Not a member of a party")` from `.party` (engine `uoscrobj.cpp` MBR_PARTY), and `BError == BError` is true, so `a.party == b.party` is TRUE for two party-less characters; the same holds for `GetObjProperty(a,"x") == GetObjProperty(b,"x")` when neither has the property. Guard with the truthiness first: `a.party && (a.party == b.party)`, or compare serials/values you know are set. Found 2026-09-22 in `scripts/include/bard.inc` ValidSongBoost and `pkg/opt/versebook/include/versefunctions.inc` SmartSongBoost (a solo bard boosted, hid and drained every party-less stranger in sight).

**21. A mobile has no `hp`, `maxhp`, `mana`, `stamina`, `strength`, `dexterity` or `intelligence` member in this engine.** `objmembers.h` has no MBR_DEXTERITY/STRENGTH/INTELLIGENCE at all, and MBR_HP/MBR_MAXHP/MBR_MANA/MBR_STAMINA are handled only in the Item block of `uoscrobj.cpp` (durability); on a character the read is an error and an assignment is a silent no-op. `ar` does exist (MBR_AR, the live armour rating). Use `GetHP/GetMaxHP/GetMana/GetStamina/GetDexterity` and `SetMana/SetStamina/...` from `include/attributes`. Because `CInt(error)` is 0 and `number > error` is always false, a `me.dexterity`-based delay silently collapses to a constant and an `x.mana > 20` test is never true (found 2026-09-23 in damages.inc, chaosmultikillpcs, vortexloopkill, sum, soulwhisperer, humuc).

**22. Call dictionary and array methods WITH parentheses: `d.keys()`, never `d.keys`.** The compiler turns a bare `x.keys` into `get-member keys` (visible in any `.lst` listing), a member read: on a dictionary that looks up a KEY named "keys" (`bdict.cpp` `BDictionary::get_member`) and yields nothing, and a `foreach` over nothing runs zero times. The shard runs its own engine build (POL100.3.0 with local "AsYlum" changes, `core-changes.txt` at the repo root; source not on this machine), and the user reported the merchant training window working, so whether that build treats the bare form as the method is unverified; write the bracketed form regardless, it is correct on every engine. Bare sites deliberately left 2026-09-23: animaltrainer.src:718, textcmd/player/dropskills.src:103, textcmd/coun/privs.src:215 and :242, textcmd/admin/gcmds.src:159, items/mboard.src:134, include/reportmurder.inc:69.

**23. Never leave out the realm argument.** `uo.em` declares `const _DEFAULT_REALM := "britannia"` as the default for every List*/Create*/GetMapInfo call, and on this shard britannia is the unused old map (play is on britannia_alt, plus ilshenar, malas, termur, tokuno). A call without a realm silently searches or creates on the wrong map: `ListItemsAtLocation(x, y, z)` finds nothing, `CreateItemAtLocation(x, y, z, objtype, 1)` litters the old map, and nothing errors. Always pass `thing.realm` (found 2026-09-23 in .untile, .makemoongates, chaoskillpcsloop.inc and soulwhisperer.src).

**24. CProp names are case-sensitive.** The engine keeps custom properties in a std::map keyed on the exact string (proplist.h), so `"lockid"` and `"LockID"` are two different properties. Grep for the spelling every reader uses before writing a cprop (2026-09-23: .makekey wrote "lockid" while the key package, containers, planks and redeed read "LockID", so its keys never fit; scripts/include/chests.inc:29 and scripts/items/shipdeed.src:92-96 still write the lowercase name, left by pick).

**25. A file-scope `var X := SomeCall();` in an include runs at the start of EVERY script that includes it.** Top-level vars in an include become globals of each including program and their initialisers run before `program` is entered, used or not. The equip/unequip control scripts also run once per equipped item at world load (`loaddata.cpp equip_loaded_item` re-runs the itemdesc `equipscript` with startup=1 to rebuild temp mods), so anything they include is on the boot path. `guildconstants.inc` built the 1,291-hue guild colour table that way (2,000 hues x 15 exclusion bands, ~30k interpreted steps) and `skilladvancerequip.src` includes `guilds.inc` for one function, so every equipped item on every NPC paid it at boot and every equip, unequip, guild chat message, verse book use and house sign control script paid it at runtime (found 2026-09-25 by AsYlum, polserver). Rule: a top-level `var` in an include may hold a literal or a config handle (`ReadConfigFile` is engine-cached), never a computed value. Wrap the build in a `Get...()` that fills a `var X := 0;` global on first call and returns it afterwards (`GetGuildColours()`), and read the getter, not the global.

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
