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
- If you have the Skill tool available, invoke the `escript-gotchas` skill before editing .src/.inc files — it has the full list, this is just the high-frequency subset.

## Repo conventions
- Never run `ecompile.exe` — the user compiles their own changes, always.
- Debug output = `Print()` (needs `use basicio;`) to the server console, not `SendSysmessage()` to a client.
- Default to no code comments, EXCEPT short what/why comments at the exact lines you change — this repo overrides the usual no-comment default.
- New craftable items: always a `0xDEED`-based deed + itemdesc.cfg entry. Never the legacy redeed system.
- Adding/editing a textcmd script: re-run `pythonscripts/_gen_command_synopses_cfg.py` after.
- ZH's skill cap is 150, not vanilla UO's 100 — don't "fix" values toward 100.
- Don't assume a stat/property is static just because there's no stored override — check whether it's a live-computed getter first.

## Reporting back
- Report findings and `file:line` citations, not full file dumps.
- If a task produces a lot of output (audit results, long lists), write it to a scratch file and report the path instead of pasting it inline.
- Keep reports short. Mark anything you couldn't verify as unverified rather than guessing.

## Escalation
- If you get stuck — root cause unclear, contradictory evidence, behavior differs between POL2.5/POL3.0 in a way that doesn't make sense — stop and report exactly what you tried and what's inconsistent. Don't guess past that point.
