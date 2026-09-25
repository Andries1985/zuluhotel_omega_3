# Subagent Briefing — Zuluhotel Omega 3 (POL3.0)

Reference this file at the top of every subagent prompt so it doesn't have to rediscover repo-wide facts. Paste its path, e.g.:
`Read D:\Zuluhotel2\zuluhotel_omega_3\.claude\subagent-briefing.md first, then: <task>`

## Repo identity
- This repo is POL3.0 (Zuluhotel Omega 3). POL2.5 is a **separate sibling project**, not a branch or older version of this one — don't assume a fix here applies there (or vice versa) without checking both.
- Engine-level behavior (core polserver mechanics, not EScript) lives upstream in `polserver/polserver`, not in this repo. Don't guess at engine internals — look them up or say unverified.

## EScript gotchas — read before touching any .src/.inc
- `foreach x in record.entries` fails on a dotted RHS. Pre-assign to a local var first: `var e := record.entries; foreach x in e`.
- Flat global namespace: never name a function/const/var/struct-field the same as an .em module builtin OR any other function anywhere in this repo. `target`/`Target` is a repeat offender — use `targetted`.
- `foreach x in arr` implicitly creates `_x_iter` — that's not a stray/dead variable if you see it.
- cfgfile.em: `FindConfigElem(cfg, "Global")` fails on a one-token block header — the real key is `""`, not the type word.
- No short-circuit evaluation: this repo compiles with `ShortCircuitEvaluation=0` (`scripts/ecompile.cfg`), so BOTH sides of `and`/`or`/`&&`/`||` always run. Never put a call with side effects (`ConsumeResource`, `CheckSkill`, `SubtractAmount`, anything that writes) in a compound condition, and don't rely on `if( x and x.member... )` to skip the right side — use nested `if`s.
- If you have the Skill tool available, invoke the `escript-gotchas` skill before editing .src/.inc files — it has the full list, this is just the high-frequency subset.

## Repo conventions
- Never run `ecompile.exe` — the user compiles their own changes, always.
- Debug output = `Print()` (needs `use basicio;`) to the server console, not `SendSysmessage()` to a client.
- Default to no code comments, EXCEPT short what/why comments at the exact lines you change — this repo overrides the usual no-comment default.
- New craftable items: always a `0xDEED`-based deed + itemdesc.cfg entry. Never the legacy redeed system.
- Crafting scripts: consume first, check the result, then create. `ConsumeResource()` returns 0 when it comes up short and has no refund path, and backpack stacks aren't reserved (they can be lifted during a craft delay or an open quantity prompt) — so charge materials BEFORE `CreateItemInBackpack`, stop on a 0 result, and for several materials call `HasCraftingResources()` (`scripts/include/resourcemanager.inc`) immediately before the consumes with no `Sleep`/gump/`Target` in between. A full backpack still costs one set of materials (`CheckSkill` has already paid the gain). Clamp bulk quantities to 60000 — the engine won't create more in one call.
- Adding/editing a textcmd script: re-run `pythonscripts/_gen_command_synopses_cfg.py` after.
- Temporary items: anything a script creates and later destroys itself (gates, effect tiles, markers) must not be saved, or a restart mid-script leaves it in the world forever. Use `SaveOnExit 0` in the itemdesc block when the objtype is only ever temporary, or `item.saveonexit := 0;` on the one instance when it isn't. Grep every creator of an objtype before flipping its itemdesc to 0.
- Item use is serialized per character by the engine: a double-click use script runs attached (itemdesc `RequiresAttention` defaults to true) and any further double-click is refused with "I am already doing something else" until it ends or calls `Detach()`. Don't report "double-click two stacks for two parallel actions" as an exploit unless the script Detaches early, the action is started via `Start_Script` without `Attach`, the itemdesc has `RequiresAttention 0`, or another character starts the second copy. Conversely, a long-running use script that should leave the player free must `Detach()` early.
- `item.container` of an equipped item is the wearing mobile (engine `WornItemsContainer::make_ref()` returns the character), so `.isa(POLCLASS_NPC)` / `.isa(POLCLASS_MOBILE)` on it is true for gear worn by an NPC or player; only an unworn item has a container item there. Don't call code that tests `item.container.isa(POLCLASS_NPC)` dead.
- Two unset values compare EQUAL: `.party` on someone with no party is an error value and error == error is true, so `a.party == b.party` is true for two party-less characters (same for two absent CProps). Guard with truthiness first: `a.party && (a.party == b.party)`.
- A mobile has no hp/maxhp/mana/stamina/dexterity/strength/intelligence member (`ar` exists): the read is an error, a write is a no-op, `CInt(error)` is 0. Use the Get*/Set* functions from include/attributes.
- Call dictionary methods with parentheses (`d.keys()`); bare `d.keys` compiles to a member read that yields nothing on the stock engine (the shard's own engine build may differ; the bracketed form is right everywhere).
- Never omit the realm argument: uo.em defaults it to "britannia", the unused old map, and nothing errors; pass `thing.realm`.
- CProp names are case-sensitive ("lockid" and "LockID" are different properties); match the spelling the readers use.
- A file-scope `var X := SomeCall();` in an include runs at the start of every script that includes it (before `program`, used or not), and the equip/unequip control scripts run per equipped item at world load. Keep top-level include vars to literals or `ReadConfigFile` handles; build anything computed lazily through a `Get...()` that fills a `var X := 0;` global on first call (see `GetGuildColours()` in `pkg/opt/guilds/include/guildconstants.inc`, 2026-09-25).
- ZH's skill cap is 150, not vanilla UO's 100 — don't "fix" values toward 100.
- Don't assume a stat/property is static just because there's no stored override — check whether it's a live-computed getter first.

## Reporting back
- Report findings and `file:line` citations, not full file dumps.
- If a task produces a lot of output (audit results, long lists), write it to a scratch file and report the path instead of pasting it inline.
- Keep reports short. Mark anything you couldn't verify as unverified rather than guessing.

## Escalation
- If you get stuck — root cause unclear, contradictory evidence, behavior differs between POL2.5/POL3.0 in a way that doesn't make sense — stop and report exactly what you tried and what's inconsistent. Don't guess past that point.
