# Code review fix log

Started 2026-09-21. Branch `Patch-3.1.2`. Kept by Claude during the whole-codebase review of ZH3.0.

This is the running record of every change made during the review, in the order it was made. A new round is appended each time a set of picks is applied. The exact line-level changes are in the companion file `code-review-fixlog-20260921.diff`, regenerated at the end of every round.

**Status of everything below: edited in the working tree, not committed, not compiled and not tested in game by Claude.** Findings come from reading the scripts and, where an entry says so, the engine source in `_reference-repos/engine`.

## What is not part of this review

These show up in `git status` next to the review's changes but were not made by it:

- `pol.cfg`: eight debug flags flipped 0 to 1 (`WatchRPM`, `WatchSysLoad`, `LogSysLoad`, `ReportRunToCompletionScripts`, `ReportCriticalScripts`, `ShowRealmInfo`, `ProfileCProps`, `EnforceMountObjtype`). Your pick was to keep them locally and leave `pol.cfg` out of the commit. Not touched.
- `pkg/opt/alchemyplus/alchemyplus.src`: Tamla potion recipe work (+69 −16). Not touched, not reviewed.
- The original Autoloom feature from 2026-09-20: the split of `make_cloth_items.src` into `tailoringfunctions.inc`, `autoloom_use.src`, `autoloom_animate.src`, and the objtype move to `0xA82D`/`0xA82F`. The review changed parts of it (section 1); the feature itself predates the review.

## How to read an entry

Each entry gives the file and line, what the code did (**Was**), what it does now (**Now**), and the reason (**Why**). **Players** is there only when someone in game will notice. Every edit site in the code carries a dated `// 2026-09-21:` comment saying the same thing in short; the index at the bottom lists them.

---

## 0. Before the review started (2026-09-20)

### 0.1 `.setph` and `.ph` crashed for a character with no power hour history
- Files: `pkg/opt/powerhour/textcmd/player/setph.src:28`, `ph.src:15`
- Was: `var use_time := GetObjProperty(who, "pph_use_time");` then `use_time + week`. For a character who never started a power hour, or after `.resetph`, the property is missing, so this was `error + integer` and the script died with "Operator + not supported for Error and Integer".
- Now: both reads (`pph_use_time`, `pph_use_weekday`) are wrapped in `CInt()`, so a missing property is 0 and the character counts as eligible.
- **Already committed by you** as `e632446` "PH Fixes".

---

## 1. Autoloom fix plan

Picks page: https://claude.ai/artifact/SfYjpnCuDdhqjsEm9hdju2. Source: the `/code-review` run over the uncommitted working tree, 13 findings.

### 1.A One writer for the loom's graphic (findings 1, 2, 3, 8, 10, 11)
- Files: `pkg/std/tailoring/tailoringfunctions.inc:67-107`, `autoloom_use.src` (rewritten), `autoloom_watchdog.src` (new), `autoloom_animate.src` (deleted with its compiled outputs)
- Was: two scripts wrote `station.graphic` with no coordination. `autoloom_use.src` opened and reverted; the fire-and-forget `autoloom_animate.src` stepped 8 frames server-side and then wrote the open graphic. Facing was guessed from that same changing graphic, by an exact match in one place and a range check in another.
- Now:
  - `GetAutoloomGraphics(station)` returns `{anim_start, open_graphic, stored_graphic}` keyed on `station.objtype` (`0xA82D` South, `0xA82F` East), 0 for any other station. It replaces the six `AUTOLOOM_*` consts and both graphic-sniffing blocks.
  - `AutoloomSetWeaving(station, weaving)` replaces `AutoloomReplayAnimation`. It writes the ANIM start id (`0xA81D` / `0xA825`) once and lets the client cycle the frames, since both are client-animated tiles. Called inline around `_Play_Sound` in `TryToMakeItem` and around `CraftClothBulk`.
  - `autoloom_use.src` is the only writer during a session: open graphic on double-click, stored graphic on exit. It sets `#AutoloomOwner` to its pid and starts the watchdog.
  - `autoloom_watchdog.src` sleeps until the owner pid is gone, then, inside `Set_Critical`, reverts to the stored graphic only if `#AutoloomOwner` still names that session.
- Why: an East loom caught on a non-stored frame was read as South and rewritten to South art for good; a disconnect killed the use script before its revert, leaving the loom open; a quick gump close let the animation's last write land after the revert; two replays could interleave; server-stepped frames landed on `0xA81F`/`0xA829`, which the client animates itself, flashing wrong-facing art.
- Your pick: unfold straight to the open pose, no opening flourish.
- Players: the weave now loops at the client's own frame rate during each craft, so it looks a little different from the hand-stepped version.

### 1.B Looms placed under 3.1.1 (finding 4)
- File: `pkg/std/tailoring/itemdesc.cfg:2929-2955`
- Was: `Item 0xA81D`/`0xA81E` deleted with no migration, so already-placed looms loaded with no `Script`.
- Now: `OldObjType 0xA81D` on `Item 0xA82D`, `OldObjType 0xA81E` on `Item 0xA82F`. The engine converts them at world load; the saved graphic normalizes on first use.
- Checked: the engine only refuses an `OldObjType` that an explicit itemdesc entry already defines, and none does. `.redeed` works from CProps (`DeedObjType`, `OtherItems`), not objtype.
- Players: an old East loom shows real East art after its first use.

### 1.C Tailoring material consumption (findings 5, 6, 9)
- File: `pkg/std/tailoring/tailoringfunctions.inc:279-406` (`TryToMakeItem`), `:448-500` (`CraftClothBulk`)
- Was: the item was created first and `ConsumeResource`'s result ignored; on a full backpack materials were consumed twice and a `return` skipped `ReleaseResourceLease`; success and fame messages fired before the full-backpack check; Bandages ignored the requested count and converted all cloth, backpack plus Omega Cache, at 4 to 1.
- Now: consume first and `break` on failure; a full backpack charges once and `break`s so the lease is released; exceptional fame and both success messages only fire when the item exists; Bandages make one per AutoLoop pass at the cfg rate (2 cloth) with the colour set on the item descriptor; `CraftClothBulk` has the same reorder, a "Your backpack is full." message, and descriptor colour.
- Why the descriptor colour: the engine merges a created stackable into an existing stack only when `item.color == descriptor.color`. Recolouring afterwards either repainted an existing plain stack or left unstackable singles.
- Your picks: honour the requested count for Bandages; tailoring now, other crafting scripts audited (section 2).
- Players: Make Now / Number / Max on Bandages makes the asked-for count. Make Bulk and scissors are still the fast paths.

### 1.E Cleanup (findings 12, 13)
- `pkg/std/tailoring/make_cloth_items.src`: six `use` lines dropped; `tailoringfunctions.inc` declares them.
- `pkg/items/deed/built/autoloom.cfg:1-3`, `pkg/std/tailoring/itemdesc.cfg:2929`: the "pixel-identical duplicate art" comments reworded. `0xA81D`/`0xA81E` are frames 0 and 1 of the South weave animation.
- `pkg/std/carpentry/carpentry.cfg:5574`: recipe re-keyed `Carpentry 0xA81D` to `Carpentry 0xA82D`. Checked every reader first: the craft menu draws no art from the key, `MakeDeed` drives the output, bulk orders skip recipes with no `Type` field, salvage only takes backpack items.

### 1.D `pol.cfg` (finding 7): left alone, by your pick.

---

## 2. Area 1, round 1: crafting audit and pkg/systems

Picks page: https://claude.ai/artifact/1u3rUZBXReDemtxrrhg6tK (Version 1, doc `plan/picks`).

### 2.0 New helper
- File: `scripts/include/resourcemanager.inc:148-175`
- Now: `HasCraftingResources(who, req1, amt1, req2 := 0, amt2 := 0, req3 := 0, amt3 := 0)` re-checks up to three requests. It uses nested `if`s because this repo compiles with `ShortCircuitEvaluation=0`.
- Why: `ConsumeResource` has no refund path, so every material has to be confirmed present before the first one is charged.

### 2.C1 Bulk paths that needed no timing
- `pkg/std/bowcraft/bowcraft.src:438` `CraftLogBulk`, `:513` `CraftAmmoBulk`: consume first with every result checked, then create; "Your backpack is full." when the create fails.
- `pkg/std/blacksmithy/blacksmithgump.inc:353` `MakeSmeltedIngots`: ingots are only created if `ConsumeResource` returned 1.
- Was: the quantity was capped against materials counted before the "Make how many" prompt and never re-checked. Moving the materials away while the prompt was open produced the whole batch for free. Smelting slept 2 seconds between its check and an unchecked consume.

### 2.C2 Per-item window during the craft delay
- `pkg/std/blacksmithy/blacksmithgump.inc:199-246` `MakeBlacksmithItems`: consume first and checked; the create is now checked too (`!product` gives "Your backpack is full." and `break`); the two `ApplyMaterialProperties` calls merged behind an `is_excep` flag so messages and fame come after the item exists.
- `pkg/std/carpentry/carpentry.src:476` `TryToCreateItem`, `:639` `MakeYoungOakStaff`.
- `pkg/std/tinkering/tinkeringfunctions.inc:469`.
- `pkg/std/bowcraft/bowcraft.src:215` `CraftLogRecipe`.
- `pkg/opt/crafterboost/refinement_gump.src:185`.
- `pkg/systems/crafting/include/multicraft.inc:245`: every part re-checked, each consume checked, leases released on the early exit.
- `pkg/std/inscription/inscription.src:1191`, `pkg/std/cartography/cartography.src:181, :192, :226` (`ConsumeMap` now returns the result).
- Was: create, then consume with the result ignored. Blacksmithy never checked the create, so a full backpack on Make Max burned ingots every pass while announcing success.
- Correction to the audit: inscription and cartography never had a real window (no blocking call between check and consume; cartography reserves the blank). They were tightened for consistency only. Cooking was traced and is safe; not touched.

### 2.C3 Make Bulk above 60,000
- `bowcraft.src:403, :513`, `blacksmithgump.inc:336`, `tailoringfunctions.inc:448`: `qty` clamped to 60000.
- Why: `CreateItemInBackpack` refuses a larger amount (`item_create_params_ok`), which charged the whole batch for nothing.

### 2.1 Player vendors could mint gold
- File: `pkg/systems/playervendor/playermerchant.src:1420` (`AddThingsToBuy`), `:1601` (`BuyItem`)
- Was: `if(!price)` rejected only 0, so a negative buy price was stored. In `BuyItem` a negative total passed the affordability check, the payout failed, and `mygold - theprice` grew the pool, which `cheque` pays out. `price * amount` could also overflow to a negative.
- Now: `if(price <= 0)`. `BuyItem` computes the total as a double, refuses `unitprice <= 0`, `total < 1` or `total > 2,000,000,000`, and tells the owner the buy price is invalid.
- Your pick: block new ones only; old stored prices are not purged at vendor start.

### 2.2 Melee weapons with no miss sound never missed
- File: `pkg/systems/combat/hooks/omegaattack.inc:74`
- Was: the miss `return` sat inside `if(attackerweaponType.MissSound)`. Now it is outside, matching the ranged branch.
- Affected: Pickaxe, GhostWeapon, BatWeapon, plus everything in 2.3.

### 2.3 Weapons defined outside the combat package
- File: `omegaattack.inc:17`. `ReadConfigFile(":combat:itemdesc")` became `":*:itemdesc"`.
- Was: 14 weapons in other packages (8 astral weapons, shepherd's crook, young oak staff, `0x13e3`, `0x5002`, `0x30000`, `0x9000`) resolved to nothing: no swing animation, never a miss, base damage forced to `1d1`.
- Players: those weapons deal their real damage and can miss. The astral Black Staff goes from `1d1` to `4d5+1`.

### 2.4 Town NPC swings and the staff spam
- File: `omegaattack.inc:84, :113`
- Was: the 64 town templates with `AttackSpeed`/`AttackDamage 5d100` and no `AttackHitScript` get a built-in weapon (objtype `0x1F020`) with an empty hit script (engine: `weapon.cpp`, `create_intrinsic_weapon_from_npctemplate`). Every swing sent "Can't find a weapon hitscript" to staff and did nothing. The cfg lookup by objtype also returned the player Wrestling entry (`1d5`).
- Now: an intrinsic weapon with no hit script defaults to `:combat:mainhit`; for an NPC with an intrinsic weapon the dice come from `::npcdesc`'s `AttackDamage`.
- Your pick: real damage. Players: merchants, bankers and shrines hit for 5d100.

### 2.5 Weapon effects in no-PK and safe areas
- Files: `pkg/systems/combat/include/hitscriptinc.inc:65` new `IsPvPBlockedByArea(attacker, defender)`; `paralyzehit.src:33`, `thiefpoisonhit.src:33`, `spellstrikescript.src:45`.
- Was: those scripts applied paralysis, poison and the struck spell before the area check in `RecalcDmg`. `thiefpoisonhit.src` had no check at all and passed no attacker to `SetPoison`.
- Now: all three call the shared check first; the thief poison passes the attacker so a poison kill is credited.

### 2.6 A poisoned weapon's last charge ate the hit
- File: `hitscriptinc.inc:786`
- Was: `PoisonCharges` at 0 with `SkillPoisoned` still set made `DealDamage` erase both and `return 0`. That swing dealt nothing and skipped reactive armor and on-hit. Now it clears the poison and carries on.

### 2.7 Cursed poisoned weapons hit their wielder for everything
- File: `hitscriptinc.inc:794`
- Was: `defender := attacker`, and `defender` was reused for reactive armor, on-hit and `ApplyTheDamage`. Now a separate `poison_target`; only the poison backfires. Your pick.

### 2.8 A skill set to "down" always failed
- File: `pkg/systems/attributes/hooks/shilhook.src:24, :119`
- Was: `return AwardSkillPoints(who, skillid, 0);` which returns 0, so every check on that skill failed.
- Now: it drops once per use, zeroes `points`, sets `dropping`, and rolls normally. `SkillAsPercentSkillCheck` takes `dropping` and skips its two unguarded half-point awards, because every `AwardSkillPoints` call on a "down" skill is another −1.
- Your pick. Players: a skill being dropped still works while it drains.

### 2.9 Dead code
- `pkg/systems/combat/shilcombat.inc` deleted; its include removed from `scripts/misc/death.src:17`. None of its functions was called or could have compiled. The unused `CombatAdvancement` hook is left, by your pick.

### Left alone by your pick: the Mage melee "reduction" that nets ×1.0.

---

## 3. Area 1, round 2

Same page, Version 2, doc `plan/picks_round2`.

### 3.0 Email package disabled (your instruction in chat)
- File: `pkg/systems/email/pkg.cfg:1-4`. `Enabled 1` became `Enabled 0`.
- Checked: nothing outside the package references `:email:` and no package requires it. Takes `.email`, `.inspectmail` and the package's logon, reconnect and chardelete hooks with it. The `Emails` datafile is untouched. Finding A3 (mail stored for nonexistent boxes) was skipped for this reason.

### 3.A1 The account watcher deleted accounts it had just spared
- File: `pkg/systems/accounts/acctWatcher/acctWatcher.src:54`
- Was: for an empty account with no `LastLogin` it logged "has been set now though", stamped it, then tested the stale local `last_used` and deleted the account in the same pass. Now it `return 0`s after stamping.

### 3.A2 Login lockout
- Files: `pkg/systems/accounts/hook/onLogin.src:169-280` (`AcctHackChecks` rewritten), `pkg/systems/accounts/config/settings.cfg:46`
- Was: the failure counter was only looked at when the password was wrong, so a locked-out address with the right password got in. `FailureInterval 3` is seconds, and failures only count inside that window, so the limit of 5 was almost never reached. One test compared the lock's clock time to a duration and was always true.
- Now: the lockout is checked before the password; a good login clears that address's failures; reaching `MaxLoginFailures` locks that account and address for `DisableLength`; `DisableGrace` works as its comment describes. `FailureInterval` is still seconds, as documented, and its value went 3 to 180.
- Correction: I first described `FailureInterval` as a units slip. The cfg documents seconds, so the unit was kept and the value changed.

### 3.A4 Undeliverable vendor payout
- File: `playermerchant.src:1078` (`CashOut`), `:1113` (`CashCheque`). When the bank box and escrow both fail, a GM page is queued with the pack serial. The pack sits at 5288,1176.

### 3.S1 Carpentry's false "Your backpack is full."
- File: `pkg/std/carpentry/carpentry.src:511`
- Was: two nested `if`s with the "make a normal item" `else` on the outer one. When the exceptional chance came up and the exceptional `CheckSkill` failed, no item was made: false message, materials lost, loop stopped. Roughly 1 successful craft in 15 on recipes that can be exceptional.
- Now: a `make_excep` flag decided up front; a failed exceptional check makes the normal item.

### 3.S2 Every plain carpentry craft rolled an Alchemy check
- File: `carpentry.src:433`. `( skillid2 ) && !CheckSkill(...)` split into nested `if`s behind `second_check_failed`. Nothing short-circuits, so a craft with no co-skill ran `CheckSkill` on skill id 0.

### 3.S3 `pkg/std/cooking/cooking.src:173`: loop condition `==` became `<=`.

### 3.S4 `pkg/std/bowcraft/bowcraft.src:455`: bulk shaft colour set on the descriptor.

---

## 4. Area 2, part 1: theft, stealth, gathering

Picks page: https://claude.ai/artifact/RrByQPjd3VVppJoSDFYgAE (doc `plan/picks_part1`).

### 4.1 Trap items worked on any container in view
- File: `pkg/std/traps/trapScripts/setTrap.src:30, :65`
- Now: `Distance > 2`, `!Accessible` and the new `CanSetTrapHere(who, trap_cont)` refuse the target. `CanSetTrapHere` mirrors `pkg/std/spells/magictrap.src`: objtypes `0x7100`, `0xefa`, `0x9c16`, `0x9c17`, a player's corpse, and any container in a house unless the player is its owner, a co-owner or a friend.

### 4.2 Remove Trap from any distance
- File: `pkg/std/removetrap/removetrap.src:31`. `Target(character)` became `Target(character, TGTOPT_CHECK_LOS)`, plus `Distance > 2` and `!Accessible` refusals.

### 4.4 Snooping container keyed by name
- File: `pkg/std/snooping/snooping.src:51`. Keyed by `Hex(who.serial) + " Snooping"`; the old name-keyed container is removed at the start of each snoop.

### 4.5 Lockpicking
- File: `pkg/std/lockpicking/use/picklock.src:15, :140, :197, :250, :263`
- Now: `LOCKPICK_DELAY_SUCCESS := 5`, `LOCKPICK_DELAY_FAILURE := 10`, stored in `#LockpickDelay` and checked before the target prompt; the pick-break roll passes 0 points.
- Your pick, with the 5 second success delay from your note. The old script set its delay on the player and checked it on the chest, so it never applied.

### Skipped by your pick: SOS bottles (about 29% point at 0,0 because `Random(sosnum)` can be 0 and `sosarea.cfg` starts at 1).

---

## 5. Area 2, part 2: bulk orders

Doc `plan/picks_part2`.

### 5.1 A Large deed could consume anyone's completed Small
- File: `pkg/std/bulkorders/bulkorderdeed.src:334, :364`. New `BOD_IsInOwnBackpack(who, item)` walks the container chain; the Small is also reserved before use.

### 5.2 Large payouts above 60,000 paid nothing
- File: `pkg/std/bulkorders/bulkorder_matching.inc:242`. New `BOD_PayGold(who, gold)`: stacks of up to 60,000 into the backpack, overflow to the bank box, then to the player's feet, with a message each time. The file now includes `:containers:storageAreas`.
- Why: 6 slots × 3,500 × 3 = 63,000 for New Zulu, Elven wood, Angel and Silver Dragon; the seven-slot template goes over from about difficulty 120.

### 5.3 Rewards paid before the deed was destroyed
- File: `bulkorder_matching.inc:63` (`BOD_TryFill`), `:185` (`BOD_TryFillLargeSlot`), `:337` (`BOD_TurnIn`). Destroy first; nothing is paid or counted if it fails.

### 5.4 A stack was eaten for one credit
- File: `bulkorder_matching.inc:63-76`. A stack fills as many units as the order still needs. `DestroyItem` for a whole item so its destroy script still runs, `SubtractAmount` when part of a stack stays behind.

### 5.5 Reward table
- File: `pkg/std/bulkorders/bulkorderrewards.src:34, :179`. `Cost < 1` is refused and written to the server log; the gump loops in `program bulkorderrewards` and no longer re-opens itself from inside `BOD_RedeemReward`.

---

## 6. Area 2, part 3: spells

Doc `plan/picks_part3`.

### 6.1 `pkg/opt/moongates/itemdesc.cfg:60-67`: `Item 0x99b1` `SaveOnExit 1` became `0`.
- Why: the Gate spell and the runebook's gate create the pair, sleep 30 seconds and destroy it. A save plus a restart inside that window left permanent immovable gates. Nothing else creates `0x99b1`.

### 6.2 `pkg/std/spells/mark.src:84, :101`
- The staff-made tag `"LFucker"` (read by `merchant.src` and `shrink.src`) went onto the targeted blank rune; it now goes onto the new marked rune. The blank is no longer used up when the marked rune could not be created.

### 6.3 `pkg/std/spells/unlock.src:68`: refuses `houseserial` or `.multi`, like lockpicking and Magic Lock.

---

## 7. Area 2, part 3b: every spell read line by line

Doc `plan/picks_part3b`.

### 7.1 Healing spells damaged Liche-form players anywhere
- Files: `scripts/include/spelldata.inc:105` new `IsSpellPvPBlockedByArea(caster, cast_on)`; `heal.src:83`, `gheal.src:79`, `resurrect.src:77`.
- Was: the Undead branch called `ApplyTheDamage`, which has no area check, through a neutral target cursor. `pkg/opt/necro/liche.src:86` sets `Type` "Undead" on a player in Liche form. Resurrection deals max HP + 3.
- Now: no such damage between players when either has `NOPKAREA` or `InSafeArea`. Your pick.

### 7.2 Summons survived a restart as permanent pets
- `pkg/opt/summoning/summoning.src:82`, `npcsummoning.src:27`, `pkg/std/spells/blade_spirit.src:59`, `vortex.src:57`: `thecritter.saveonexit := 0`. Engine: `NPC::set_script_member_id` handles `saveonexit`.

### 7.3 `dispel.src:94`: the summoned branch applied `GetMaxHp(caster) + 3`. Now `KilledBy`, `KilledBySerial`, `.kill()`.

### 7.4 `massdispel.src:63`: `return` became `continue` for an immune victim (twice); each victim gets its own `dispel_power`.

### 7.5 `dispel_field.src:54`: realm argument added to `ListItemsNearLocation`.

### 7.6 `mindblast.src:104`: `ApplyRawDamage` became `ApplyTheDamage(..., DMGID_MAGIC)`. Players: kill credit works; Mind Blast hits players 40% softer.

### 7.7 Timers
- `reactivearmor.src:26, :50`: `CanTargetSpell` replaces the bare `Target()`; `#ReactiveArmorPid` stamp, cleared only by its own cast.
- `invisibility.src:62`: `#InvisPid` stamp, a 1 second poll that ends the timer when the target is revealed or the stamp changes. `pkg/std/hiding/hiding.src:51` erases `#InvisPid` on a successful skill hide.

### 7.8 `cure.src:54`, `invisibility.src:47`: the `Reflected()` block commented out, matching the other helpful spells.

### 7.9 `polymorph.src:78`: Warrior `critter` clamped at 0.

### 7.10 `teleport.src:121`: the stand check always returns. `scripts/include/client.inc:24`: `DEBUG_MODE := 0`.

### 7.10b Poison scaling against players
- File: `pkg/opt/summoning/processpoisonmod.src:105`
- Was: `!who.IsA(POLCLASS_NPC) && GetGlobalProperty("ReducePoison") && DEBUG_MODE`. Ticks are dealt with `ApplyRawDamage`, so this was the only scaling poison had.
- Now: any player victim is scaled by `ReducePoison` if a GM set it with `.reducepoison`, otherwise 0.6. Victim-based like the old line, so monster poison on a player is scaled too.

### 7.11 Deleted `bless timer.src` and `protection with timer.src` with their compiled outputs.

---

## 8. Area 2, part 4: the rest of pkg/std (last part)

Doc `plan/picks_part4`. All of `pkg/std` is now read. Applied 2026-09-21 (late evening).

### Correction to two of my own findings first
- **Bandages (finding 1) was wrong**, and you spotted it. I claimed split stacks could run several heals at once. The engine already forbids it: every item-use script starts *attached* to the character (`Item::double_click` → `start_itemuse_script(prog, item, itemdesc.requires_attention)`, and `RequiresAttention` defaults to true), and `Item::double_click` refuses any further double-click while one is attached — that is the "I am already doing something else" you see in game. The healing script never calls `Detach()`, so a second stack is blocked for the whole heal. `Attach()` in the vet script also fails if anything else is attached. What I found is real but harmless: the 5 s cooldown code in `CheckIfCanUseBandage` is dead (the timer is only read inside the magic-fish branch), but every bandage action already takes ≥ 5 s attached, so it would add nothing even if it worked. **Left alone, by your pick and rightly so.**
- **Finding 4b (stacked instrument play loops) is moot for the same reason**: the play loop runs attached, so a second double-click is refused. Not implemented. 4a stands.
- **Finding 14's "as fast as you can click" was overstated**: the pickpocket dip sleeps 3.6 s attached per use, so it was one roll per 3.6 s. The missing skill cap was the real problem. Implemented per your pick.

### 8.3 Power Hour "half" of 1 was 0
- Files (24 sites): `blacksmithgump.inc:138`, `bowcraft.src:162, :405`, `carpentry.src:326, :624`, `tailoringfunctions.inc:244, :460`, `tinkeringfunctions.inc:364, :396, :746`, `multicraft.inc:86`, `refinement_gump.src:148`, `make_crafter_boosts.src:119`, `cartography.src:20`, `cooking.src:210, :391`, `alchemyfunctions.inc:169, :278`.
- Was: `CInt(Ceil(material/2))`. Both operands are integers, so the engine divides as whole numbers first (`blong.cpp`, `wrap_div`): 1/2 = 0 and 25/2 = 12, and `Ceil` had nothing left to round. A cost of 0 passed every check and consumed nothing: 45 of 88 cooking recipes, 18 tinkering recipes and single arrows and bolts were free during a Crafting Power Hour.
- Now: `/2.0` at every site, so it rounds up as `Ceil` was meant to: 1 stays 1, 25 becomes 13. Your pick.
- Players: half-cost items with an odd cost pay one more unit than before (12 → 13).

### 8.4a Instruments unlocked themselves when played
- File: `pkg/std/musicianship/musicianship.src:38`
- Was: `if( instrument.movable == 0 ) instrument.movable := 1;` at the top of the script. A house lockdown is exactly `movable := 0`, and the pickup packet hook only blocks people outside the house, so anyone inside could play a locked-down harp and carry it off. POL2.5 has the same line.
- Now: the lines are gone; an immovable instrument still plays where it stands. The `newbie := 0` line beside it is untouched.

### 8.5 Alchemy at a station made no potion
- File: `pkg/std/alchemy/alchemyfunctions.inc:198, :250` and new `AlchemyCreatePotion(user, mortar, product)` at `:316`.
- Was: `CreateItemInContainer( mortar.container, product, 1 )` with the result ignored. A placed station is a world item, and the engine returns 0 for its `.container`, so the create failed silently after the reagents, bottle and skill roll were spent. With no bottle the mixture was stored on the station.
- Now: the helper creates in the mortar's container when it has one, otherwise in the brewer's backpack. A failed create says "Your backpack is full." and stops; the stored mixture is only erased once the potion exists.

### 8.6 Dig with a map you do not hold
- File: `pkg/std/treasuremap/digtreasure.src:78`. The map must be in the digger's backpack (`IsInContainer`); "The map must be in your backpack." otherwise.

### 8.7 Runebook crafting
- File: `pkg/std/inscription/inscription.src:1031, :1068`
- Was: the targeted blank rune (`item`) fell off the component list in the resource-manager port, so a runebook cost no rune (POL2.5 charges it). Mana was checked after the scrolls were already consumed.
- Now: mana availability is checked before anything is consumed; the rune is charged with `SubtractAmount(item, 1)` alongside the scrolls; mana is deducted after the components. Your pick.
- Players: a runebook costs a blank rune again.

### 8.8 Hunger never drained anything
- Files: `pkg/std/cooking/hunger.src:20, :67, :144, :160`, `hungerdamage.src:22` (+ `HungerDrainDone` at the end).
- Was: `Start_Script(":cooking:hungerdamage", {chr})` passed a one-item list; `hungerdamage.src` used it as the character, read `hunger` off a list, got 0, and its loop never ran once. The drain also only started at the tick hunger went 9 → 10 and exited on relog, so a relog switched it off until the next meal.
- Now: the character is passed. `hungerdamage.src` stamps its pid in `#HungerDrainPid` and clears it on exit (only if still its own). `hunger.src` has `EnsureHungerDrain(chr)` (starts one only if none is alive; called at login when hunger ≥ 10 and after every hourly tick at ≥ 10) and `StopHungerDrain(chr)` (kills a drain left from the previous login, at login). Your pick ("as designed, including at login").
- Players: this is new in practice. With `DEBUG_MODE` at 0, `logon.src` no longer resets hunger to 1 at login, so hunger climbs 1 per hour online; from 10 stamina drains 2 per 6 s, from 12 mana too, from 14 health too. Eating brings it down; at 14 the character auto-eats cooked food from the backpack.

### 8.9a A gate wrote back an old rune list
- Files: `pkg/std/runebook/runebook.src:308`, `runicatlas.src:157, :175`, `runebookactions.inc:14, :80, :230, :313`.
- Was: the book is released before a gate and the script waits ~38 s, then saved the `RuneDefs` it read *before* the gate: a rune dropped in meanwhile was destroyed and wiped from the list, and edits from a second window were lost. The dupe guard was a saved `opengate` 0/1 property, so a crash inside the window left the book unable to drop runes for good.
- Now: both gate paths `return` without that save; `DestroyRune` saves its own change (on the list it just re-read). The guard is `#opengate` = clock + `OPENGATE_GUARD_SECS` (60), checked by `DropRune` with a message, never saved, and a leftover saved `opengate` is erased when seen.

### 8.9b Charged recall's saved flag
- Files: `runebookactions.inc:80`, `customspells.inc:99, :126`
- Was: `RecallWithCharges` set a saved `betatest` flag on the character, cleared late in the cast; an interrupted charged recall left it set and the next recall from a book skipped reagents, mana and the Magery check. `CustomRecall` already had a `from_charge` parameter that was passed and never read.
- Now: `CustomRecall` reads `from_charge`; the flag is no longer written, and any leftover is erased in passing. The charge is still taken up front, by your pick.

### 8.10 Small fixes
- `pkg/std/dundee/globeofsosaria.src:20`: cooldown property `#GlobeOfSosariaCooldown` → `GlobeOfSosariaCooldown` (saved; the engine never writes `#` properties, so every restart gave a fresh use).
- `pkg/std/tracking/tracking.src:29`: the `unloadconfigfile("::npcdesc")` on every use is gone.
- `pkg/std/help/help.src:176`: the colour-war refusal clears the "already paging" flag.
- `pkg/std/taunt/enticeai.src:35`: with no bard (restart wiped the `#` markers, or the player is gone) the NPC restores its real AI script instead of returning and staying motionless. `herd.src:23`: "You can't see that!" now returns. `herdedai.src:13, :25`: erases `locationx`/`locationy` (was misspelled `loctionx`/`loctiony`).
- `pkg/std/healing/healing.src:227, :299`, `pkg/std/veterinary/vet.src:118, :203`: "Your patient is dead." / "The creature is dead." stop there instead of rolling and burning the bandage.
- `pkg/std/camping/camping.src:76` (fire and ashes), `pkg/std/treasuremap/digtreasure.src:339` (guardians): `saveonexit := 0`.
- `pkg/std/tasteid/tasteid.src:43`: only a poison potion's strength is read as a poison level.

### 8.12 Item ID delay
- Files: `pkg/std/itemid/itemid.src:10`, `itemid.inc:11` (new `ItemIDClassSkill(who)`, which also replaces the if-chain in `SelectItemToID`).
- Was: `BASE_DELAY := 0` since commit `e450f84` (2025-12-23); POL2.5 has 10. The 65-100 skill ramp had already been removed before that.
- Now: 10 s between identifications, skipped at 100+ in the class's ID skill (the tier `SelectItemToID` already treated as no-delay). Your pick.

### 8.14 Pickpocket dip
- File: `pkg/std/training/dummy_pickpocket.src:31`. Stops at 25 Stealing ("You cannot learn any more from simply picking a dummy's pocket.") and refuses a second user while one is in progress (`#picking` on the dummy), matching the combat dummy. Your pick.

### Left alone by your pick
- 2 Power Hour + salvage refund (materials printing) — leave.
- 11 vet resurrection uses 1 bandage, not the 5 it asks for — leave.
- 13 Rangers cannot bandage themselves — leave.

---

## 9. Area 3: pkg/multis and housing

Picks page: https://claude.ai/artifact/7SSPc5Ky1QuxSuHirLFQnb (doc `plan/picks_area3`). Applied 2026-09-22. Your notes changed two of these: the custom house teardown pays 50% and no deed (not 90% capped at 10,000 plus a deed), and a full backpack on a classic demolish puts the deed on the ground instead of refusing.

### 9.1 Eject teleported anyone in view onto the sign
- Files: `pkg/multis/house/multiSign/use.src:839`, `pkg/multis/customhousing/sign.src:1049`, `pkg/multis/staticHousing/sign/use.src:1198`
- Was: classic and custom refused only when the target was outside every house AND the ejector more than 15 tiles from the sign; static checked that the ejector stood in a house. Any visible player could be dropped onto the sign.
- Now: mobiles only, never staff or a player vendor, never the owner or a co-owner, and only someone `IsObjectInsideHouse` says is in this house. The ejected player is told.

### 9.2 Any passenger could sail the boat
- Files: `pkg/multis/boat/multi/listener.src:138, :157`, `pkg/multis/boat/tiller/methods.src:96`
- Was: `ProcessEvent` ran every spoken command from anyone on deck; `CanCommand` (owner, crew, staff) existed and was never called; crew mates matched by name.
- Now: commands and maps handed to the tillerman need `CanCommand` ("Ye ain't the captain o' this ship!" otherwise); crew mates match by serial.

### 9.3 Custom house co-owners could transfer or demolish it
- Files: `pkg/multis/customhousing/sign.src:104, :115`, `include/house.inc:636`
- Was: cases 13 and 14 were open to every full house manager (co-owners included), and `ChangeOwner` kept the old co-owner, friend and ban lists, so a co-owner could target themselves and keep everything.
- Now: owner only (staff excepted), like the classic sign; a transfer wipes co-owners, friends, bans and their permissions.

### 9.4 Secure containers were only guarded when opened
- Files: `scripts/include/housing.inc:282` (new `FindHouseSecureContainer`, `HouseSecureAccessAllowed`), `pkg/items/containers/container/canRemove.src:49`, `canInsert.src:45`, `use.src:74`
- Was: the hooks checked `container.IsSecured()`, a `secured` struct with a `.level` no package writes (classic and static: `secure 1`; custom: `secured` = sign serial), against the affiliation to the house the *mobile* stood in. They never refused. The engine hands a ground-container item to anyone in reach who asks for it by serial, so a chest gump left open after access was revoked, or an assistant macro with a known serial, could empty a secure; anyone could drop items in.
- Now: the hooks walk up to the secure container (a bag inside a secure counts), resolve its own house sign (custom: `secured`; classic: `houseserial` → multi → `signserial`; static: `houseserial` is the sign) and apply that package's open rule: staff always, a ban never, a static `SecuredLevel` letter by the owner/co-owner/friend ladder, otherwise `HasHousePermission(who, "secure")`. A secure whose house cannot be found refuses everyone but staff, as the open scripts do.

### 9.5 The houses-per-account cap forgot classic houses at every restart
- File: `pkg/multis/house/multiSign/control.src:28`
- Was: the registry `#housing_of_<acct>` is a `#` global the engine never saves; static and custom signs re-register at control-script start, the classic sign did not.
- Now: it registers its owner's account when its control script starts.

### 9.6 No key opened a custom house door
- Files: `pkg/multis/customhousing/scripts/customhousedeed.src:100`, `sign.src:26, :415` (`ChangeHouseLocks`), `:250` (key sweep on demolish), `include/house.inc:15, :286` (`ReKeyCustomHouseDoors`), `:313` (`MigrateCustomHouseLockIds`), `:679`, `syshook/closecustomhouse.src:11`
- Was: custom housing wrote a lowercase `lockid` (value: the house serial) on keys, sign and doors; the key package reads `LockID`, so `KeyMatchesLock` saw no lock id on the key. The close hook looked for a 0x0BD0/0x0BD2 sign a custom house never has and locked every door with 0. "Change the locks" re-keyed the key (to serial + 1) and no door.
- Now: `AllocateLockID` and `LockID` everywhere; the close hook takes the lock id from the house's own 0xFFF4 sign (through `signserial`) and only re-locks when it has a real id; "Change the locks" needs a real blank key, allocates a fresh id and re-keys every door; the demolish key sweep uses `KP_DestroyOwnedKeysForLockIDs`. `MigrateCustomHouseLockIds` runs on every sign use: a sign without `LockID` gets its old lowercase value (or a fresh id), the doors are re-keyed to it, and the keys in that character's backpack and bank are converted.
- Players: custom house doors open with the house key from the first time the owner clicks the sign.

### 9.7 Demolishing a custom house returned nothing
- File: `pkg/multis/customhousing/sign.src:449` (`MakeADeed`, now takes the sign; `CustomHousePayGold`, `CUSTOMHOUSE_TEARDOWN_PRICE_PER_TILE`)
- Was: the price was read off a 0x0BD0/0x0BD2 component sign, never found, so no gold, no deed, success reported.
- Now (your note): the owner is paid half of what the house cost, the foundation price on the 0xFFF4 sign plus every tile paid through the customisation tool (`customhouse_paidparts` × 500, matching `scripts/misc/customhousecommit.src`), with no cap and no deed, the way a house deed sells back to a vendor at half (`VendorBuysFor` is half of `VendorSellsFor` on every house deed). Gold goes to the backpack in stacks of up to 60,000, to the owner's feet when the backpack is full. A classic multi placed through this package still gets its deed, at the owner's feet when the backpack is full.

### 9.8 Classic house decay destroyed the deed and left the house
- File: `pkg/multis/house/multiSign/control.src:54, :493`
- Was: `demolish(house)` where the function expects the sign; its own house lookup failed, the footprint sweep and `DestroyMulti` did nothing, the built deed was destroyed anyway. One-year timeout, so latent.
- Now: `demolish(sign)`, the listener returns after it, and the account registry is updated.

### 9.10a Classic demolish with a full backpack
- File: `pkg/multis/house/multiSign/use.src:932` (deed type resolved into `deedtype` before anything is touched), `:1198`, `:1220`
- Was: 134 `CreateItemInBackpack` calls inside a `case` with the result ignored; a full backpack demolished the house with no deed; a house type missing from the list got none either.
- Now (your note): unknown type refuses ("no deed on record, page a GM"); the deed is created first, in the backpack or on the ground at the owner's feet when it is full; if it cannot be created anywhere nothing is demolished; if the multi refuses to go, the new deed is destroyed again so the house is not duplicated.

### 9.11 Two buyers of the same static house both paid
- File: `pkg/multis/staticHousing/sign/use.src:274, :325`
- Was: the "buy?" question blocked with the sign still for sale; both confirmations were charged, the second `ownerserial` write won.
- Now: the sale is re-checked after the confirmation, the payment and hand-over run critical (the only yield, a `Sleep(2)` before the bank check, is gone), and the sign is marked sold the moment the gold is taken.

### 9.12 Small
- `pkg/multis/house/multiSign/use.src:540`, `pkg/multis/customhousing/sign.src:971`: "Ban someone" refuses staff (cmdlevel 2+), as the static sign did.
- `pkg/multis/staticHousing/logon.src:10`: `BootBannedFromHouse` instead of a move plus `Start_Script("houseBan")`, a script that does not exist.
- `pkg/multis/staticHousing/sign/destroy.src:121`: `LockID` erased from the component object, not its serial.
- `pkg/multis/boat/tiller/canInsert.src:8`: a map handed to the tillerman goes back to the backpack (`mobile.container` does not exist).
- `pkg/multis/house/multiSign/use.src:439`, `pkg/multis/customhousing/sign.src:933`: the owner's own account is refused before the friend is added, not after.

### 9.13 Static decay refresh (your call: owner and co-owners only)
- `pkg/multis/staticHousing/sign/use.src:395`, `sign/control.src:96`: the abandonment timer was refreshed for whoever clicked the sign and on any speech inside; now only for the owner and co-owners.

### 9.14 `pkg/multis/staticHousing/config/settings.cfg:93`: `DebugLogging 1` became `0`.

### Left alone by your pick
- 9 static release of legacy items (banners, anvils, forges and the rest of that list) still destroys them; the 0x2B00x deeds do not exist.
- 10b locked-down and secured items on a classic demolish still get the one-second decay.

---

---

## 10. Area 4: pkg/packethooks

Picks page: https://claude.ai/artifact/8YooLh21ii1oNopkKTbsPH (doc `plan/picks_area4`). Applied 2026-09-22. Your notes parked two items to look into later (the wear-item hook, the dungeon-drop lookup), asked for an explanation and a choice in chat before the staff speech log is touched, and for more information on the double-click block before anything was deleted. That last one turned out to be a wrong finding, below.

### Correction first
- The page called the double-click hook's "warrior for hire" block (`pkg/packethooks/doubleclick/doubleclick.src:33-38`) dead because "an item's container is never an NPC". Wrong: for a worn item the engine hands back the wearer (`WornItemsContainer::make_ref()` returns the character, `pol/mobile/wornitems.cpp:117`), so `item.container.isa(POLCLASS_NPC)` is true for gear a hireling wears. The block is the master's undress path: double-click a piece the hireling is wearing and it drops into the hireling's backpack. Nothing was removed. Your request for more information before deleting is what caught it; the fact is now gotcha 19 in the skill and a line in the subagent briefing.

### 10.2 Staff speech with a keyword was logged as "1" and echoed back (your choice in chat)
- File: `pkg/packethooks/speech/receivespeechhook.src:31`
- Was: the keyword-speech branch (client packet type 0xC0, sent when the text matched a client keyword such as "bank" or "guards") did `speech := SendSysMessage(character, CStr(...))`: the staff member saw their own line repeated as a system message and the log stored the function's return value, 1.
- Now: `speech := CStr(packet.GetString(speechstart, speechlen));`, the same decode without the send. Staff stop seeing the echo; the log gets the words.

### 10.3 Weapon tooltip damage and DPS
- File: `pkg/packethooks/megacliloc/itemdata.src:507` (`Calcdamage`, new helper `TooltipAvgSkill`), `:601` (`calcdps`)
- Was: a copy of the 2024 damage formula. Archery ×0.005 for Rangers and Bards, Tactics ×0.002 for Warriors, Power Players and Paladins, Ranger level bonus from level 3; melee Anatomy ×0.005 (Warrior, PP), Eval Int ×0.005 plus an INT term (Paladin), level bonus from level 5 for everyone, Mage divided by `ClasseBonus`; no durability term. DPS divided Dex by 10 and added the speed mod after the multiply.
- Now: the attacker side of today's `CalcPhysicalDamage` (`pkg/systems/combat/include/hitscriptinc.inc:339`): quality, durability under 50%; Archery: Ranger avg(Archery, Animal Lore) ×0.007, Bard class average ×0.007, Power Player avg(Archery, Tactics) ×0.003, Mystic Archer avg(Archery, Eval Int) ×0.004, Ranger/Mystic Archer level bonus from level 4; Throwing: Thief avg(Throwing, Tactics) ×0.007, Power Player ×0.003; melee: NPC Tactics ×0.00025, Warrior/PP avg(Anatomy, Tactics) ×0.005, Bladesinger ×0.00375, Paladin avg(Eval Int, Tactics) ×0.002, the fighter classes' level bonus from level 4 and everyone else's from level 5 (the vs-NPC branch), Mage divided by `ClasseSmallBonusByLevel`. Everything that needs a defender (Paladin/Warrior/Bladesinger reductions, Mage victims, the Thief poison bonus, the PvP halving) is left out and the comment says so. DPS uses the engine's swing time, 15000 / ((Dex + 100) × (Speed + speed mod)) seconds (`Character::check_attack_time`).
- Players: weapon tooltips show truer numbers; the DPS line roughly halves for a 100-Dex character. No combat change.

### 10.5 `.updatetp` did nothing
- File: `pkg/packethooks/megacliloc/commands/player/updatetp.src`
- Was: `TargetCoordinates` (a position struct) passed to `IncRevision`, which needs an object; the call errored silently.
- Now: `Target(who)` then `IncRevision`; a `// Synopsis:` line added and `config/command_synopses.cfg` regenerated, so the command is listed.

### 10.6 Small
- `pkg/packethooks/versionHook/versionhook.src:32`: the two `SleepMS(5)` calls removed. A packet hook runs to completion and the engine ignores a suspend in that state (`UOExecutor::suspend`, `Executor::exec`), so they never delayed anything. No behaviour change.
- `pkg/packethooks/megacliloc/itemdata.src:175`: the hit-script tooltip lines match the item's own `hitscript` (the engine reports it as `:combat:banishscript`) with the package prefix stripped, falling back to the template; was the template's local name, which only matched weapons defined inside the combat package. `:216`: the tri-elemental label "Blinds the target" became "Chance to strike with lightning".
- `pkg/packethooks/speech/receivespeechhook.src:58`: the staff speech log re-opens its data file after creating it and returns if that fails; the first line for a new staff account went through the failed handle and was lost.

### 10.8 Legacy drop hook removed (your pick)
- Files: `pkg/packethooks/packethook/packethook.src:67` (the 14-byte `CheckDrop` function, 56 lines, gone), `pkg/packethooks/packethook/uopacket.cfg:1`
- Was: two 0x08 hooks registered, the 14-byte one for clients before 6.0.1.7. The version gate only admits 7.0.115.0, so it never ran; its stray-drop counter also had no time window.
- Now: only the 15-byte hook is registered. A pre-6.0.1.7 client would get the core's own drop handling (`get_packethook_for_client` falls back to the default when no version-1 entry exists).

### Left alone by your picks and notes
- 1 wear-item hook equips any item onto your pet: parked to look into further (memory `project_wearitem_hook_equip_any_item`).
- 4 dungeon-drop lookup by objtype: parked to look into further (memory `project_dungeon_drop_objtype_lookup`).
- 7 container contents line visible to everyone.

---

## 11. Area 5, part 1: pkg/opt money and economy packages

Picks page: https://claude.ai/artifact/MqMp5PPceXhZuU6AqxEueo (doc `plan/picks_area5_part1`). Applied 2026-09-22. Your picks left the guild uniform shop (3) as is, and asked for an explanation first on the town stone handlers (4), the stale donator/reset references (15) and the corpse-loot guild exemption (16); after the explanations in chat you chose to fix all three (15 with player towns counting, 16 wired to real guilds), applied below as 11.4, 11.15 and 11.16.

### 11.1 Omega Cache deposit credited the store before destroying the stack
- Files: `pkg/opt/omegacache/omegacache.inc:1122` (`DepositSingleItem`), `:1149` (`DepositFromContainer`), `:469` (new `DepositItemData`; `DepositItem` is now a wrapper for callers that still hold the item)
- Was: `DepositItem` wrote the amount into the store, then `DestroyItem` ran with its result ignored. The engine refuses to destroy an item another script has reserved (`mf_DestroyItem`: "That item is being used"), and the cache gump runs detached, so a bandage stack mid-heal, ingredients mid-cook, reagents at the mortar, ore at the forge or blank scrolls could be targeted for deposit: the store was credited and the stack stayed.
- Now: the item is reserved first (refused with "That item is in use." when another script holds it), the objtype, amount and non-default properties are read off it, it is destroyed, and only a successful destroy writes the store. The container path skips a held stack and counts it among the items that could not be stored. The drag-and-drop path (`cacheinsert.src`) goes through the same function.

### 11.2 Omega Cache withdraw paid out more than it debited while a craft leased the item
- Files: `pkg/opt/omegacache/omegacache.inc:1488` (`DoWithdraw`), `:728` (new `SnapshotCacheElement`, `RestoreCacheElement`, `RecreateItemFromProps`; `RecreateItem` kept as a wrapper)
- Was: the available amount was the raw stored quantity, every stack was created before the lease-aware `WithdrawItem`, and its (possibly smaller) return value was dropped. Crafting from the cache leases the shortfall (`scripts/include/resourcemanager.inc` `LeaseResource`, 60 s renewed per item), so a withdrawal of the leased units created them without debiting anything.
- Now: the available amount excludes leases (`GetStoredAmount`); each stack is debited first and exactly the debited amount is created from a snapshot of the element's properties taken before the debit; when creation fails the debit is put back, recreating the element (properties included) if `WithdrawItem` had emptied and deleted it. A fully leased item refuses with "reserved by a craft in progress".

### 11.5 Town cheque donation destroyed any targetable cheque
- File: `pkg/opt/townstones/tstone.src:537` (check), `:555` (new `IsInDonorBackpack`)
- Was: `Target` then `DestroyItem`, no reach or ownership check; a cheque in a trade window, on a vendor or on a house table could be donated by anyone at the stone.
- Now: the cheque must be inside the donor's own backpack.

### 11.6 Guild formation spent the gold before checking the house, and accepted a house another guild held
- Files: `pkg/opt/guilds/commands/player/guilds.src:2273` (`FormGuild`), `pkg/opt/guilds/include/guilds.inc:409` (`GetOwnedGuildCandidateHouses`)
- Was: the house list accepted houses owned by the same account (`IsGuildHouseOwner`), then `FormGuild` took the 120,000 gold and only afterwards compared `ownerserial` to the character, so a sibling character's house cost the gold and formed no guild. Neither the list nor `FormGuild` looked at an existing `GuildHouse` tag, which "Change guild house" did filter.
- Now: the ownership check uses the same rule as the list and runs before any gold moves, a house already tagged by a guild is refused, both are re-checked after the confirmation gump, and the house list itself no longer offers houses another guild holds.

### 11.7 An offline guild master's replacement could be the master being removed
- Files: `pkg/opt/guilds/include/guilds.inc:587` (`ChangeGuildMaster`), `pkg/opt/guilds/commands/test/changeguildownership.src:28`
- Was: `guild.members[1]` became the master whether or not that was the leaving master (character deletion runs this for the offline master), and every member was told they themselves were the new master.
- Now: the first member who is not the one leaving; members are told the successor's name, the successor is told directly. The Dev command also refuses an OK with nobody selected instead of writing an error value as the master.

### 11.8 Vanity shop bundles kept the pieces that fit when the pack filled
- File: `pkg/opt/vanityshop/vanityshop.src:251`
- Was: x5/x10 bundles were moved into the backpack one item at a time and the tokens taken only after all moved; a pack that filled part-way aborted the purchase without charging and left the moved items in the pack.
- Now: the pieces already moved are destroyed together with the bag when one does not fit, so an aborted purchase hands over nothing. (Charging first and refunding would have needed a free slot for the refund in the very pack that was full.)

### 11.9 Election watcher scanned the world every 5 seconds
- File: `pkg/opt/townstones/electionwatch.src:61` (`ProcessActiveTownstones`), `:162` (new `FindTownstonesFromDatafile`, `FindTownstonesByWorldScan`), interval constant `:13`
- Was: a 10,000-tile `ListItemsNearLocationOfType` in six realms every 5 seconds for as long as any election or poll ran (a week each).
- Now: the stones are resolved from the `stone_serial` each region keeps in the townstone datafile; the world scan only runs when none of them resolve; the tick is 60 seconds.

### 11.10 Random Ancient Tome destroyed before its Power Tome existed
- File: `pkg/opt/powerscrolls/randomTome.src:10`
- Was: destroy, then create; a full backpack lost the tome.
- Now: create first, refuse with "You need more room in your backpack." when that fails, destroy the Random Tome only afterwards (and take the new tome back if the destroy fails).

### 11.11 Guild loose ends
- `pkg/opt/guilds/commands/player/guilds.src:3142`: `JoinGuildRequest` takes `show_list`; the recruit path passes 0, so recruiting no longer opens the guild-list gump on the recruit, and `RecruitMember` (`:2126`) only announces "now a recruit" when the request was registered.
- `:675`: `DisplayGuildInfo` keeps the master object; the member list no longer includes the master (the compare was against a name string's `.serial`).
- `:2028`: `DisplayGuildMembers` lists applicants from the live `JoinGuildRequest` list; `:2156`: the "Recruits" scan in `AddGuildMember` removed; `:2142`: `AddRecruit` deleted (never called; wrote the legacy "Recruits" list nothing reads).
- `pkg/opt/guilds/ondelete.src:36`: a deleted character's serial is removed from every guild's `JoinGuildRequest` list (was a scan of the dead "Recruits" property).

### 11.12 Leaving a town stripped the name with find-and-cut arithmetic
- File: `pkg/opt/townstones/tstone.src:1059`
- Was: `find(name, "of " + city)` minus 2 as a length, negative whenever the suffix was absent (renamed since, or a town name with an apostrophe, which the join strips and this did not).
- Now: `StripTownSuffix`, the helper the admin tools already use.

### 11.13 Omega Dye / Soul Pen / Runic Dye Tub
- `pkg/opt/vanityshop/customitemdye.src:21`, `customitemname.src:20`, `runebookdye.src:21`: an item with no charges left destroyed itself and carried on through the targets and the confirmation; each now returns after the "disintegrates" message.
- `customitemname.src:58`: the Soul Pen refuses stackable items (a renamed pile no longer stacked with the plain ones and lost the name through the cache).

### 11.14 `.cfglotto`
- File: `pkg/opt/lootlottery/commands/GM/cfglotto.src:101`, `:125`
- Was: the Single Winner checkbox was initialised from `Lotto_item` (a string, so always ticked once an item was set) and both checkboxes were stored as the raw gump values (an unticked box came back as an error value).
- Now: initialised from `Lotto_single`, stored as 0/1.

### 11.4 Town stone handlers trusted the gump (your answer in chat: fix)
- Files: `pkg/opt/townstones/tstone.inc:165` (new `IsTownCitizen`), `:173` (`candidato`), `:206` (`voteto`), `:338` (`ConvocarEleicoes`), `:381` (`ConvocarPleibicito`); `pkg/opt/townstones/tstone.src:1102` (`Citizenship`), `:763` (`CanselCityzenship`), `:1084` (population floor), `:585` (`OpenVoteSelectionGump`)
- Was: the gump drew buttons by role but the handlers did not check; any reply id let a non-citizen vote, declare candidacy, call an election or leave a town they never joined (one off the population each time), anyone start a poll, and a citizen of another town join a second one.
- Now: citizen of this town (`town` property equals the stone's id) for voting, candidacy, calling an election and leaving; mayor for polls; joining refused for anyone who already has a town; population never below zero. Players using the drawn buttons see no difference.

### 11.15 Stale references (your answer in chat: fix, and make player towns count)
- `pkg/opt/Donator/include/playertown.inc` (new): `IsInPlayerCity` is now "standing in a City region that has a town stone registered in the townstone datafile" (`GetRegionName` + `stone_serial`), shared by `donatorhorsestone.src:105`, `donatorbearstone.src`, `donatorllamastone.src`, `donatorostardstone.src`, whose own copies (a 250-tile search for objtype 0x7566 with MinX/MaxX/MinY/MaxY properties, an item this repo never had) are removed. Donator mounts now work in player-run towns such as Zento as well as the hardcoded Britannia cities of `IsInACity`.
- `pkg/opt/powerhour/textcmd/test/resetph.src:18`: `mobile.isa(POLCLASS_NPC)` instead of comparing the target with the number.
- `pkg/opt/vanityshop/include/mountFunctions.src` deleted (included by nothing; its one loop iterated a variable it never declared), with its stray compiled outputs.

### 11.16 Corpse-loot guild exemption (your answer in chat: wire to real guilds)
- File: `pkg/opt/loot/antiloot.inc:69`
- Was: read a `guild_id` property nothing sets, from the looter for both sides; the exemption never applied and guild-war looting in town reported or jailed like any other looting.
- Now: the looter's guild against the corpse owner's (found by the corpse's `CorpseOf` serial, offline included): the same guild, a guild at war with it, or an allied guild loots the corpse without the criminal flag, the report or the auto-jail; the looter is still recorded on the corpse.

### Left by your picks
- 3 guild uniform shop creating any objtype named in the gump reply (left as is; confirmed in chat).

## 12. Area 5, part 2: spell books and summoning (2026-09-22)

Page: "Area 5 Fix Plan, Part 2" (https://claude.ai/artifact/EhbVX4jb1QfrxQwHXBMNtF, picks in db doc `plan/picks_area5_part2`). Packages read line by line: summoning, necro, MagicWands, earth, holybook, songbook, versebook, alchemyplus (read only, nothing changed there). You picked fix for 4, 5, 7, 8, 9, 10, 12, 13, 14 and 15, asked for an explanation first on 1, 2, 3, 6, 11 and 17, and left 16 (Holy Bolt healing the living). The ten fixes come first; the six explained items were answered in chat (1 fix, 2 full fix, 3 fix, 6 full fix, 11 fix, 17 classic wands) and follow as 12.1, 12.2, 12.3, 12.6, 12.11 and 12.17. Not compiled or tested by me.

### 12.4 Verse area effects skip safe areas
- File: `pkg/opt/versebook/include/versefunctions.inc:652` (`SmartSongAoE`)
- Was: the verse book's own area helper dropped party-mates, own pets and faction/noPK cases but never anyone standing in a safe area (the song book's `SmartAoE` does); a bard outside a bank could hit the people inside with Sonic Disturbance, Bardic Boulders or an exploding corpse from about ten tiles.
- Now: anyone in a safe area (`IsInSafeArea`, areas.inc, already included) is not a verse target. Used by Sonic Disturbance, Bardic Boulders, explode_corpse and the Spirit Flock goats.

### 12.5 Wrath of God's reflected hit is self-inflicted
- File: `pkg/opt/holybook/wrathofgod.src:96`
- Was: when the target had more karma, the damage was applied with the target as the attacker; ApplyTheDamage then recorded the target on the caster's hitlist as aggressor and, when the reflect killed, wrote the target as the killer.
- Now: `ApplyPlanarDamage( caster , caster , ... )`; the damage helper treats attacker == victim as raw damage with no hitlist entry and no kill credit. Same damage, same message.

### 12.7 Song of Sirens stops when out of stamina
- File: `pkg/opt/songbook/songofsirens.src:77`
- Was: the "Out of Stamina" branch had no return and the paralysis loop sat after the `endif`, so a bard below the stamina cost paralyzed every valid target for free and without the two-second cast time.
- Now: the branch clears the casting flag and returns, like every other song.

### 12.8 Stuck casting flag: Earth Portal and the books
- Files: `pkg/opt/earth/earthportal.src:22` and `:30`; `pkg/opt/earth/bookofearth.src:106`, `pkg/opt/holybook/holybook.src:111`, `pkg/opt/songbook/songbook.src:103`, `pkg/opt/necro/codexdamnorum.src:172`, `pkg/opt/songbook/songscroll.src:40` (the druid, holy and necro scrolls got the same guard in 12.15)
- Was: Earth Portal returned on "You can't gate from there" (the recall rules) and on an anti-gate item nearby before clearing `#Casting`; the book's use script waits on that flag, never Detaches, and the engine refuses every double-click while an attached use script runs, so the player was locked out until relog (logoff is the only thing that clears the flag). A spell script that failed to start (unknown id, script error) trapped the player the same way in all four books and the scrolls.
- Now: both early returns clear the flag; the books and scrolls check the `Start_Script` result (`spell_process.errortext`, the pattern the rituals scroll and the guild stone use) and clear the flag instead of waiting.

### 12.9 Verses clear the casting flag on every exit
- Files: `pkg/opt/versebook/include/versefunctions.inc:717` (new `EndVerse`); called from `Bardic_Boulders.src:106`, `Corpse_Distention.src:102`, `Dragon_Skin.src:119`, `Lesser_Healing.src:103`, `Life_Balance.src:101`, `Shadows.src:105`, `Sonic_Disturbance.src:103`, `Spirit_Flock.src:98`, `Beastal_Bond.src:166`, `Not_Implemented.src:79` (after each loop and before the fumbled re-play `return 0`)
- Was: `SucceedPlayingVerse` set `#Casting` and only `InterruptedPlaying` cleared it; a verse that ended on its own (no boulder target, a fumbled re-play check, the beast-bond cancel, a break) left it set, and the song, earth, holy and necro books and `.cast` answered "You are already casting something!" until relog.
- Now: every verse clears it on the way out. Dragon Skin's fumble path only clears the flag here; its protection cleanup is item 2, pending your answer.

### 12.10 Temporary gates never saved
- Files: `pkg/opt/earth/earthportal.src:165`, `pkg/opt/holybook/angelicgate.src:25`, `config/itemdesc.cfg:356` (0x7012 blackmoongate2)
- Was: Earth Portal's far gate 0x7012 was explicitly `SaveOnExit 1` (its near twin 0x1fd4 is 0) and Angelic Gate's "Gate of Life" 0x7002 had no save flag; a save inside the 60 s / 150 s window brought them back after a restart with nobody left to remove them: a permanent one-way gate, or a permanent public resurrection gate wherever it was cast.
- Now: `saveonexit := 0` on all three gates when created, and 0x7012 is `SaveOnExit 0` in itemdesc (only earthportal.src uses it).

### 12.12 Song of Beckon's fairy never saved, killed like the other summons
- File: `pkg/opt/songbook/songofbeckon.src:121`, `:137`
- Was: created directly (not through `SummonCreature`), so it missed the `saveonexit := 0` the summon scripts got on 2026-09-21; only this script's sleep of up to 375 s killed it, and the tamed AI skips hunger and release for anything marked summoned, so a restart left a permanent 300-hp pet. The kill was `ApplyRawDamage(hp+3)`, which the engine drops above 65535 hp.
- Now: `saveonexit := 0` right after creation; the end plays the summon-fade sound and effect and calls `.kill()`.

### 12.13 Area spells no longer hit the caster
- File: `scripts/include/spelldata.inc:1478` (`SmartAoE`)
- Was: the shared area helper removed the caster only through the party rule, so a solo caster inside their own area of effect was a victim: Rising Fire, two of Apocalypse's three parts, Wraith's Breath (self-freeze), Abyssal Flame, Plague and the main book's Chain Lightning.
- Now: the caster is skipped first, for every spell that uses the helper. `SmartAoEPartyAgnostic` (its party-hitting twin, with the pet/party lines commented out) was left alone.

### 12.14 Song of Cloaking reveals only the people it cloaked
- File: `pkg/opt/songbook/songofcloaking.src:114`, `:121`
- Was: one shared flag and a reveal loop over everyone in sight at cast time, so someone skipped by the party check who hid on their own in the meantime was un-hidden 100 to 250 s later.
- Now: the people actually cloaked are collected (the unused `proped` array) and only those still carrying the `cloaked` mark are revealed.

### 12.15 Small fixes and dead files
- `pkg/opt/earth/druidscroll.src:28`, `pkg/opt/holybook/holyscroll.src:27`, `pkg/opt/necro/necroscroll.src:25`: the "already casting" check now comes before the scroll is subtracted (a scroll used with a cast pending vanished with no cast); the scroll casting options (`CastingOpts`: no reagents, scroll skill gain) are cleared after the wait, as songscroll.src already did, because a spell that stopped before `TryToCast` left them on the character and the next book cast was reagent-free; and the `Start_Script` guard from 12.8.
- `pkg/opt/necro/codexdamnorum.src` and necroscroll.src: `CastingNecro`, set and read by nothing, removed.
- `pkg/opt/necro/sunderingsword.src` deleted: no item in any itemdesc used it (it killed any summoned creature in sight with no ownership check); its compiled outputs removed too.
- `pkg/opt/earth/shapechange.cfg` deleted: unused twin of shapeshift.cfg (the script reads shapeshift), with a `graphix` typo.
- `pkg/opt/versebook/Beastal_Bond.src:98`: the start of `:versebook:Beastal_Bond_Reporting`, which never existed, removed (the wisp AI reports).
- `pkg/opt/holybook/angelicfeast.src:23`: `loop < amount` (was `<=`, one food item too many).
- `pkg/opt/versebook/Corpse_Distention.src:115`: critical mode turned off before the unsuitable-corpse return.

### 12.1 Books check the reply against what they hold (your answer in chat: fix)
- Files: `pkg/opt/versebook/versebook.src:194`, `pkg/opt/songbook/songbook.src:91`, `pkg/opt/earth/bookofearth.src:94`, `pkg/opt/holybook/holybook.src:99`, `pkg/opt/necro/codexdamnorum.src:153`
- Was: each book drew a button only for the spells it held, then took the reply id straight to the spell; a forged reply performed any verse or song (songs are learned from 25,000-100,000 gold scrolls with the pen at difficulty 80-115, verses from verse scrolls with a skill check), and in the codex an unlearned slot became spell id 0, which left the player waiting on `#Casting` until relog.
- Now: the verse book performs only a verse marked learned in its `Verses` list (staff keep the testing bypass they already had in `CanPlay`); the song, earth and holy books accept ids 1-16 whose Lesser/Greater bit is set; the codex accepts only slots 1-16 that hold a spell. Anything else is refused with a message. Holy scroll inscription, the other reader of those bits, is untouched.

### 12.2 Dragon Skin takes its points back on every exit and after a restart (your answer in chat: full fix)
- Files: `scripts/include/spellgrants.inc` (new: `ReclaimDragonSkinGrant`, `ReclaimSpellPoisonImmunity`), `pkg/opt/versebook/Dragon_Skin.src:70` (fumble exit), `:145` (grant recorded), `:154` (`RemoveDragonSkin`), `scripts/misc/logon.src:78`
- Was: the verse added one point per bard level to the durable `PhysicalProtection` property (combat: 5% less melee damage per point) and remembered it only in the runtime `#DragonSkin` flag; a fumbled re-play check returned early and skipped the removal, a restart skipped it too, and after a restart the flag was gone while the points stayed, so the next Dragon Skin stacked another set.
- Now: the fumble exit `break`s into the normal cleanup; the grant is recorded in a plain `DragonSkinGrant` property; the removal takes back the recorded amount and clears both marks; a leftover grant is taken back at the start of the next Dragon Skin and at every login. Players carrying leaked points lose them at their next login.

### 12.3 Boost helpers require a real party (your answer in chat: fix)
- Files: `scripts/include/bard.inc:70` (`ValidSongBoost`), `pkg/opt/versebook/include/versefunctions.inc:684` (`SmartSongBoost`)
- Was: "same party" was `a.party == b.party`; a character with no party gets an error value from the engine and two error values compare equal, so every party-less stranger counted as a party-mate: Life Balance evened out health with strangers by setting HP directly, and Lesser Healing, Shadows, Dragon Skin and the songs of Glory, Haste, Defense, Life, Remedy, Light and Cloaking landed on them.
- Now: the bard themself and their own pets always count; another player counts only when the bard has a party and it is the same one. Partied bards see no change. Recorded as gotcha 20 in the escript-gotchas skill.

### 12.6 Undead halving: bosses exempt, Salvation through the damage helper (your answer in chat: full fix)
- Files: `pkg/opt/songbook/songofsalvation.src:110`, `pkg/opt/holybook/revive.src:58`
- Was: Salvation set every undead in sight to half its current health with `SetHp` (no resist, no boss exemption, no kill credit, no immunity check, no loot-lottery damage tracking); Revive on an undead dealt its whole health plus three or, on a passed resist roll, half of it, with no Boss/SuperBoss exemption.
- Now: both skip Boss and SuperBoss (Revive says "The nature of the target shields it from the spell!"); Salvation deals its half through `ApplyTheDamage(..., DMGID_MAGIC)` so kill credit, immunities and the loot lottery apply. Ordinary undead are hit as hard as before.

### 12.11 Antidote and Sanctuary poison immunity is restart-safe (your answer in chat: fix)
- Files: `pkg/opt/earth/antidote.src:86`, `pkg/opt/holybook/sanctuary.src:73`, `scripts/include/spellgrants.inc` (`ReclaimSpellPoisonImmunity`), `scripts/misc/logon.src:78`
- Was: both wrote the durable `PermPoisonImmunity` property (the equipment one, read by the poison check) and relied on the script sleeping up to 300 s to erase it; a restart inside the window made the immunity permanent (level up to 6 from Antidote, class level + 1 from Sanctuary).
- Now: the grant is marked with a plain `SpellPoisonImmunity` property; the spell's normal end and every login drop the marker and set `PermPoisonImmunity` back to the highest level among worn items (the equip script's rule), or erase it.

### 12.17 Classic wands (your answer in chat: classic wands)
- Files: `pkg/opt/MagicWands/magicwands.src:65`, `scripts/include/spelldata.inc:330` (`can_cast`)
- Was: a wand took a charge and then started the ordinary spell script with no casting options, so the user still paid mana and reagents, rolled the skill check and needed the circle; same code in POL2.5.
- Now: the wand sets the scroll-style casting options `NOREGS`, `NOMANA`, `NOSKILL` (consumed by `TryToCast`) and a 30-second `#WandCast` stamp that lets `can_cast` skip the circle limit, so any character can use any wand; the charge is the cost. The golem, toxic cloud and wall of death wands have their own effects and were already free.

### Left by your picks
- 16 Holy Bolt heals anything that is not an undead NPC (left as is).

## 13. Area 5, part 3: items and rituals (2026-09-22)

Page: "Area 5 Fix Plan, Part 3" (https://claude.ai/artifact/3nZBkrXLmoU99ZifStCgBm, picks in db doc `plan/picks_area5_part3`). You picked fix for 4, 5, 6, 8 and 9, asked for an explanation first on 2, 10 and 11, and left 1, 3, 7, 12 and 13. The five fixes are below; the three explained items are answered in chat and recorded as 13.x sections as they are decided. Not compiled or tested by me.

### 13.4 Artifact box moved off the orc boat hull objtype
- Files: `pkg/opt/ArtifactSystem/itemdesc.cfg:4`, `pkg/opt/ArtifactSystem/artifactbox.src:13`
- Was: the box was objtype 0x7990, which config/boats.cfg also uses for an orc boat hull component; an objtype with no itemdesc block of its own is built from tiledata, so that hull piece took this block, script included, and double-clicking it handed out an artifact and destroyed the piece. The script also destroyed the box before knowing the move into the backpack had worked, so a full or too-heavy backpack left the artifact in the store and the player with nothing.
- Now: objtype 0x303C8 (custom range, verified free in hex and decimal; graphic 0x0E80, colour, name and destroy script unchanged). The catalog files that list 0x7990 as a graphic (upgrades.cfg, offset.cfg) are untouched. The box is destroyed only once the artifact is in the backpack; otherwise "You cannot carry what is inside the box." and the box stays. Note for the live shard: boxes already in the save keep objtype 0x7990, which after this change has no itemdesc block, so they lose the script and show as the hull piece; replace any that turn up with `.create artifactbox`.

### 13.5 Ritual darkness expires on its own
- File: `pkg/opt/rituals/include/rituals.inc:568` (`ChangeLightLevel`), constant at `:24`
- Was: each drawing step set a light override on every player within ten tiles that lasted until logoff (`SetLightLevel( level, -1 )`). A bystander who walked away stayed in the dark, a failed ritual never lifted it, and the success path set another until-logoff override (level 0) instead of clearing.
- Now: each step's override lasts 120 seconds (`RITUAL_LIGHT_STEP_SECONDS`) and the next step refreshes it; level 0 clears the override outright (`SetLightLevel( 0, 0 )`: the engine ends a duration-0 override on the light check the call itself triggers). Both teardown helpers (`DestroyDone` at `:544`, `RITUAL_EndCircleSurvivingItem` at `:1007`) now clear the light as well, so every failure path lifts it.

### 13.6 Ritual casting flag cleared on every exit
- Files: `pkg/opt/rituals/include/rituals.inc:209` (`RITUAL_ClearCasting`), PerformRitual early exits at `:79`, `:84`, `:94`, `:99`, `:106`, `DestroyDone` at `:545`, `RITUAL_EndCircleSurvivingItem` at `:1008`, `UndoRitualCircle` at `:338` and `:354`; `pkg/opt/rituals/rituals/demonstration.src:26`, `:33`, `:71`, `:99`
- Was: PerformRitual and the demonstration wrote their process id into `#Casting` and nothing erased it, so after any ritual, finished or not, the spell books, the scrolls and .cast refused the player until relog. Two exits leaked more than the flag: when the captor vanished during the undo phase, UndoRitualCircle returned without tearing anything down, leaving the circle items and the ritual item (movable 0, SaveOnExit 0) on the ground with the caster free to walk off; the demonstration chant did the same with the circle and the practice trinket.
- Now: `RITUAL_ClearCasting` erases the flag. It runs inside both teardown helpers (which covers every failure path that already tore down), on the five PerformRitual checks that return before a circle exists, and on the demonstration's three exits that bypass the helpers. The two undo-phase captor exits call `DestroyDone( mobile, !practice, item )` like the four sibling exits in the same loop: for a real ritual that is the forced collapse (caster killed unless staff, item destroyed), the same outcome a vanished captor already had during the drawing and the chant; for the demonstration nothing is at risk. The demonstration chant's captor exit calls `DestroyDone( mobile, 0, trinket )`.

### 13.8 Hatched dragons and ostards get an engine master
- Files: `pkg/opt/zuluitems/dragoneggs.src:126`, `pkg/opt/zuluitems/ostardeggs.src:102`
- Was: the hatchling got the `master` property and the tamed script but never `SetMaster`, so its engine `.master` was empty; the area-spell and song helpers changed in part 2 test `.master`, so the owner's own area spells and songs treated the pet as wild. frenziedeggs.src already set it.
- Now: `SetObjProperty( newpet, "script", newpet.script )` and `newpet.SetMaster( who )` before the script swap, exactly as frenziedeggs.src does.

### 13.9 Cannon and catapult: full range clamp, line of sight, cannon no longer sticks
- Files: `pkg/opt/zuluitems/cannon.src:85` (program), `:101` (`fireworks`), `:61`; `pkg/opt/zuluitems/catapult.src:33` (program), `:62` (`fireworks`)
- Was: the impact point was clamped only to the east and south (`Min` against x+10 and y+10), so a target far to the west or north was used as sent, and nothing checked that the shooter could see the spot. The cannon's cannonball-too-far exit was also the only one that did not clear `#inuse`, leaving the cannon "already being used" until restart.
- Now: x and y are clamped to plus or minus 10 of the piece in every direction in the caller, and the shot is refused with "You cannot see where that would land." unless `CheckLosAt` passes from the shooter (the cannon keeps its powder and ball loaded and clears in-use). `fireworks` uses the point as given and the `Min` calls are gone. Cannon powder (0xe7f) and cannonballs (0xe73) are still not obtainable anywhere, so this stays latent until they are.

### 13.10 Small fixes (your answer in chat: Santa limiter and the dead dye script; the other two are decided below)
- Files: `pkg/opt/christmas/Christmasgifts.src:15`; `pkg/opt/zuluitems/dyecheck.src` deleted with its compiled outputs
- Was: Santa's "one present per day" stamp was kept in the runtime key `#GiftedAlready`, which the server forgets at every restart, so each restart handed out another round. `zuluitems/dyecheck.src` was an older copy of the dye tub script that nothing bound (the tub uses the dyteitems copy).
- Now: the stamp is the plain property `GiftedAlready` (game-clock based, so it survives restarts). The dead script is gone.

### Explained first, decided in chat
- 2 "of Vengeance" raw damage and 11 "of Recalling" charges: LEFT as unreachable by your answer. The pen enchant branch in `pkg/std/inscription/inscription.src:66-74` has been commented out since the initial commit (and in POL2.5), so no scribe can make an "of Vengeance", "of Recalling" or "of LifeStream" item; the alchemical symbols (tinker.cfg, category Clay: Foci, Tinkering 110) are still craftable but inert. Only staff tools create these items today.
- 10 small fixes: Santa limiter and the dead dye script applied (13.10). The staff picker entry for saferecall (`pkg/opt/shilitems/usescriptdesc.cfg:45-51`, labelled Regeneration and assigning the regeneration script) and the silver bow argument lists (`pkg/opt/sunshine/silverhit.src:44-46`; the bow and its arrows are handed out by nothing, staff .create only) LEFT as staff-only or unreachable by your answer.

### Left by your picks
- 1 ritual captor kills bystanders and the caster dies with the item destroyed, 3 test stones and shrink doll without a staff check, 7 kryss 0xda16 bound to the missing `:GMItems:kryss_usescript`, 12 LifeStream recipe pointing at a missing script, 13 Trash Can of Wonders junk farm (left as is).

## 14. Area 5, part 4: world systems and staff tools (2026-09-23)

Page: "Area 5 Fix Plan, Part 4" (https://claude.ai/artifact/C27591aWcbgPHc5JFbT9r4, picks in db doc `plan/picks_area5_part4`). Packages read line by line: areaspawner, spawnpoint, areas, champspawns, questpkg, moongates, roleplaying, warriorforhire, decoratefacets, Events, zulugames, msg, ipban, Staff, admin, moons, OrionClient; `alryc` by synopsis and command level, its two player-level commands in full. You picked fix for 1, 3, 4, 5, 6 (full circle at the same total), 9, 11 (every 30 s) and 13, asked for an explanation first on 2, 7, 10 and 12, and left 8. The eight fixes come first; the four explained items were answered in chat (2 fix after a deeper look, 7 cache it, 10 on login and reconnect, 12 all five) and follow as 14.2, 14.7, 14.10 and 14.12. Not compiled or tested by me.

### 14.1 `.mounttest` is staff-only again
- Files: `pkg/opt/alryc/textcmd/test/mounttest.src` (moved from `pkg/opt/alryc/textcmd/player/`; the compiled outputs left in the player folder were removed so the old command cannot keep running), `config/command_synopses.cfg` regenerated
- Was: the dev command sat in the player command folder with no command-level check. Any player could kill their own mount (dismount, guard-kill flag, raw damage) and then equip any of 68 mounts for free as a bare mount item with that graphic and colour, donator-mounted flag included.
- Now: the file lives in the test folder like the rest of the package; nothing else changed in it. A dated note at the top says why it moved.

### 14.3 The guard call judges each pet by its own owner
- File: `pkg/opt/areas/callguards.src:44` (LookAround)
- Was: the "criminal master" flag was set the first time a tamed creature with a criminal or murderer owner was scanned and never reset, so every tamed creature scanned after it in the same call got a guard too.
- Now: the flag is reset at the top of the loop, per creature.

### 14.4 The RPer stone refuses OKAY without a class
- File: `pkg/opt/roleplaying/rperstone.src:106`
- Was: the starter-class gump took its choice from the last returned key; with no radio ticked the code fell into the default branch, which only set a comment string, and carried on to zero every skill, set the class skills of an unset class and push the stats to 100. The character ended with no skills and no gear.
- Now: the default branch says "Pick a starting class first." and returns before anything is touched.

### 14.5 Quest rewards are created before the quest completes
- File: `pkg/opt/questpkg/include/queststate.inc:490` (QP_CompleteQuest), `:538` (new `QP_CreateReward`)
- Was: turning a quest in recorded it as completed, destroyed the turn-in trophy, and only then created the reward gold and item with unchecked calls; a full or overweight pack lost the reward for good.
- Now: each reward is created first, into the pack or, when that refuses, at the player's feet with a message. If a reward cannot be created at all, anything already made is destroyed, the player is told to make room, and the quest stays ready to turn in. Only once every reward exists is the quest recorded complete and the trophy destroyed.

### 14.6 Champion gold covers the full circle at the same total
- File: `pkg/opt/champspawns/include/rewards.inc:12` (RainDownMoney)
- Was: the map check was called with the loop offsets as coordinates, so it failed for every negative offset and gold landed on the 121 tiles of one quadrant (500-2,499 a pile, about 180,000 a champion).
- Now: the check uses the corpse position plus the offset, so all 441 tiles are considered, and each pile is 137-685 gold, which keeps the total per champion where it was (your pick: same total). Raising it again is one number.

### 14.9 Macro check: gump moved on screen, a disconnect is reported instead of jailed
- Files: `pkg/opt/roleplaying/textcmd/coun/macrotest.src:15`, `pkg/opt/roleplaying/macrotimer.src:22`
- Was: the check gump was drawn at x 860-956, off-screen on a client narrower than about 1,000 pixels, so the OKAY button could not be reached; and a player whose client dropped during the fifteen minutes was jailed (offence counted, release stamped on the account) as if they had ignored it.
- Now: the gump sits near the top-left (x 60-156, y 50-170). When the timer expires and the player is offline, the start flag is cleared, every online staff member is told the player disconnected during the test and to re-run it when they return, and nothing is jailed. A connected player who ignored the check is jailed exactly as before.

### 14.11 Staff positions are logged at most every 30 seconds
- File: `pkg/opt/Staff/RecordXYZ.src:19`
- Was: every staff login started a loop that checked the position every 2 seconds and, whenever it had changed, opened the account's datafile, added a timestamped property and unloaded it (a disk write per step).
- Now: the loop runs every 30 seconds; positions are still written only when they changed. The header comment says 30 seconds too.

### 14.13 Small fixes
- `pkg/opt/spawnpoint/include/customnpc.inc:97`: the saved override sets the spawned custom NPC's objtype to `npc.objtype` (was its strength value; the engine's `CreateNpcFromTemplate` overrides replace the template's own objtype).
- `pkg/opt/spawnpoint/textcmd/admin/newmobedit.src:313` (HP, stamina, mana), `:351` (AR): the AR edit subtracts the target's own armour (was the staff member's); the three vital edits set the NPC's CustomHitsLevel / CustomStaminaLevel / CustomManaLevel (hundredths, then `RecalcVitals`) and then the current value through the scaled helpers (was `SetVital` with raw hundredths on the current value only, so "500" became 5). For a player only the current value is set, as before but correctly scaled.
- `pkg/opt/areas/textcmd/admin/areas.src:197`: a page's checkbox state is applied on any real button press (page turn or commit) instead of only when some box on that page came back ticked, so the last ticked box on a page can be cleared. The unused `HasCurrentPageCheckboxInput` helper is deleted.
- `pkg/opt/roleplaying/textcmd/admin/fixstartgear.src:79`: the class name is read back after the gear is given, so staff see "Ranger starter gear has been given." instead of "Something went wrong."
- `pkg/opt/warriorforhire/warrior.src:697`: the "stop following staff" check tests `cmdlevel` (was `.cmd`, not a member, so it never fired).
- `pkg/opt/msg/commands/player/msg.src:169`: stored messages are numbered from 0, the display array from 1; the first message ever logged now shows.
- `pkg/opt/decoratefacets/commands/test/udestroymany.src:32`: range mode tests `DecorateFacetsStatic` (was `Static`, which nothing sets).
- `pkg/opt/Events/textcmd/seer/createEventBag.src:6`: the command includes the package's `bag.inc` and calls its `GetPrize`; the local 105-row copy of the table is deleted.
- `pkg/opt/champspawns/scripts/control.src:96`: the "less than 2 hours" and "less than 1 hour" windows are as wide as the idle sleep (600 s), so exactly one tick lands in each (they were 300 s wide and missed about half the time).
- `pkg/opt/questpkg/include/queststate.inc:20`: new `QUESTPKG_DEBUG` (0) and `QP_Debug`; 28 informational `Print` lines across questdeath.inc, questfishing.inc, questjournal.inc, questnpcgump.inc and queststate.inc go through it (the three "could not open/create" error prints stay). `:155`: the cooldown text rounds up (3,600 s reads "1h", was "2h").

### 14.2 Spawn points keep the template's hit points on a retried placement (your answer in chat: fix)
- File: `pkg/opt/spawnpoint/checkpoint.src:553` (CreateSpawnPointNpc)
- Was: for an NPC, Custom NPC or Quest NPC point with an appear range, a first random spot that fails the engine's stand check (blocked tiles, water, lower ground carrying a static; a plain map tile always passes) triggered a single retry that also restored the point's saved vitals with `SetHP(critter, vits.hits)`. A normal point has no saved vitals, so the value was an error and the repo's `SetHp` turned it into the minimum: the creature was placed with 1 hit point, and nothing refills it (no AI start resets hit points, the vitals hook only fills custom-level NPCs, default regeneration is 12 a minute). Narrow, as you said: single-NPC points with an appear range in dungeons and towns.
- Now: the vitals restore runs only for a point flagged `CustomPoint`; a normal point keeps the template's full health. The placement tries up to ten spots before giving up (was two). Group points still retry once; they never had the vitals line.

### 14.7 Area policy masks are cached in memory (your answer in chat: cache it)
- File: `pkg/opt/areas/include/areapolicy.inc:21` (constant), `:161` (GetPolicyMask), `:179` (new `RefreshRealmPolicyCache`), `:144` (SaveRealmPolicies), `:224` (SetPolicyMask), `:393` (PruneStaleRealmPolicyEntries)
- Was: every safe-area, no-PK, guarded, anti-magic and recall-rule question opened the realm's policy datafile from disk, read one mask and unloaded it again, per victim of an area spell, per cast, per swing, per recall.
- Now: each realm's masks live in the global property `AreaPolicyCache_<realm>` (a dictionary keyed by area id, saved with the world). `GetPolicyMask` reads it and only rebuilds it from the datafile when the property is missing; every save path (the `.areas` commit through `SaveRealmPolicies`, `SetPolicyMask`, and a prune that opened the file itself) rewrites it. The datafile stays the durable copy. The areas.cfg box lookup is unchanged (in-memory config, no disk).

### 14.10 Safe-zone and no-PK flags decided at login and reconnect (your answer in chat: on login and reconnect)
- Files: `scripts/misc/logon.src:37`, `scripts/misc/reconnect.src:24`, `pkg/opt/areas/textcmd/admin/areas.src:456` (RefreshOnlineAreaStateForRealm)
- Was: login stripped the safe-zone protection and never re-checked the position, racing the region enter script the engine fires at login (which hands off to a delayed script); reconnect did nothing. The `.areas` commit granted the protection to everyone inside any area ticked Safe, including areas.cfg boxes that are not scripted regions, where no leave script ever takes it back.
- Now: login and reconnect strip, then decide: inside a safe area the protection is granted, inside a no-PK area the no-PK flag is set, otherwise a stale no-PK flag is cleared (both scripts include the areas helpers for this). The `.areas` refresh grants only to players standing inside a scripted region (`GetRegionName` non-empty); removals still apply everywhere.

### 14.12 Spawn point admin fixes (your answer in chat: all)
- Files: `pkg/opt/spawnpoint/checkpoint.src:52` (time expire), `:685` (group placement), `pkg/opt/spawnpoint/spawnpointmanager.src:101`, `pkg/opt/spawnpoint/textcmd/admin/gotospawnpoint.src:78`, `despawn.src:13`, `forcespawn.src:13`, `primespawn.src:13`, `forcespawnarea.src` (usage text), `pkg/opt/spawnpoint/spawntriggerwalkon.src:8`
- Time expire: the creation time is stamped when the manager registers a new point (never was), an older point gets its stamp on its first check, and the comparison runs the right way round; an expired point is deleted through the manager's destroy event so "despawn on destroy" is honoured.
- Group placement: a Group point with no appear range at an unstandable spot now returns the same "Invalid spawning location" error as the NPC branch (which disables the point for review) instead of sleeping five seconds, printing two console lines and retrying forever with a hidden creature parked at the staging spot.
- Hidden delete: the `.gotospawnpoint <template> 27347` branch that destroyed every matching point is gone.
- Target checks: `.despawn`, `.forcespawn` and `.primespawn` refuse a target that is not a spawn point (they used to write spawn settings onto any item); `.forcespawnarea` prints its real name in the usage text.
- Trigger tiles: the walk-on ignores NPCs and dead characters, so only living players fire a triggered spawn (dead NPCs and ghosts used to).

### Left by your picks
- 8 RPer gates and the Randorin teleport stay commented out (left as is).

## 15. Area 6, part 1 (scripts: misc, player commands, items, control, top-level), 2026-09-23

Picks from the "Area 6 Fix Plan, Part 1" page (db doc `plan/picks_area6_part1`): fix 1-10, explain first 11, leave 12, then part 2 (scripts/include). The ten fixes are below; item 11 was answered in chat (messages only) and follows as 15.11. Nothing compiled by me.

### 15.1 A failed .cast no longer leaves the casting flag set
- File: `scripts/textcmd/player/cast.src:67` (program)
- Was: the command set `#Casting` before calling CastSpellFunction; that function's nineteen early exits (book missing, spell not in the book, non-bard song, number out of range) returned without clearing it, and every spellbook, ritual scroll and spellstrike then refused with "already casting" until the player relogged (logoff.src erases it).
- Now: the early set is gone; every spell branch already sets the flag right before Start_Script and the spell script clears it, and the verse path is handled by SucceedPlayingVerse/EndVerse.

### 15.2 .recalltotem looks the totem up by serial
- File: `scripts/textcmd/player/recalltotem.src:14`
- Was: the "humuc" property (the totem's serial) was used as the creature itself; unhide, move and speech all failed silently and the 24-hour cooldown was stamped anyway.
- Now: SystemFindObjectBySerial; "You have no totem." and "Your totem is dead." exits; the move is checked ("Your totem cannot reach you here.") and the cooldown is stamped only after it succeeds.

### 15.3 Trash cans destroy unstamped items
- File: `scripts/control/trashControl.src:9`
- Was: only items whose runtime-only `#DestroyAt` stamp was at or before now were destroyed, so anything in a can at a restart (or moved in by a script) stayed forever.
- Now: a missing or zero stamp counts as due.

### 15.4 .online shows each row's account age
- File: `scripts/textcmd/player/online.src:97` (player list), `:169` (staff list)
- Was: both loops read CreatedAt from the viewer.
- Now: from the row's character.

### 15.5 First level-4 Bladesinger saved under its own key
- File: `scripts/textcmd/player/showclasse.src:355`
- Was: the block checked "4bs" and saved "4pa" (the Paladin key), so the broadcast repeated for every level-4 Bladesinger and overwrote the Paladin record.
- Now: "4bs".

### 15.6 Shield-hand curse checks read the shield hand
- Files: `scripts/textcmd/player/disarm.src:38`, `:47`; `scripts/textcmd/player/undressme.src:39`
- Was: wtwocurse was read from weaponone in both commands, and .disarm tested wonecurse in the shield branch.
- Now: weapontwo / wtwocurse.

### 15.7 .clearmsglog: the archive keeps the first message, an empty log means nothing to clear, files reopened after create
- File: `scripts/textcmd/player/clearmsglog.src:19` (program), `:37` (reopen), `:48` (index), `:69` (empty log), `:83` (archive reopen)
- Was: `props2[CInt(prop)]` dropped message 0 from the archive copy; an empty log came back as a 28-character sentence, so "Nothing to clear" never ran and an empty wipe was archived and started the 24-hour cooldown; both datafiles were created without being reopened.
- Now: index +1; RetrieveMessages returns "" when empty and the program stops there with "Nothing to clear." (no archive, no cooldown); OpenDataFile after CreateDataFile in both functions.

### 15.8 Login hunger clamp writes the property
- File: `scripts/playermanager.src:36`
- Was: `who.hunger := 15` (not a character field; no-op).
- Now: SetObjProperty(who, "hunger", 15).

### 15.9 Quest kill print behind the debug switch
- File: `scripts/misc/death.src:238`
- Was: Print for every quest-tagged kill.
- Now: QP_Debug (QUESTPKG_DEBUG, see 14.13).

### 15.10 .trashlb builds the board once and caches it
- File: `scripts/textcmd/player/trashlb.src:11` (constant), `:49` (FillInArrays), `:117` (GetTrashLeaderboard, replaces GCharList)
- Was: two full passes over every account (five slots each, 5 ms per slot) per press, one for names and one for counts.
- Now: one pass, kept in the global property `TrashLeaderboardCache` for ten minutes (TRASHLB_CACHE_SECONDS); the sort and the gump are unchanged.

### 15.11 Two message fixes (your answer in chat: messages only)
- Files: `scripts/textcmd/player/password.src:69`, `scripts/textcmd/player/removejewels.src:1`
- Was: the too-long message said the limit is 10 characters (the check is 16); the jewelry message said "equipepd".
- Now: "max 16 characters"; "All your equipped jewelry was moved to your backpack."

### Left by your picks
- 11 `scripts/items/frogpotion.inc` stays (messages only).
- 12 The twenty unreferenced scripts in items/, control/ and the top level stay (list in the review notes).

## 16. Area 6, part 2 (scripts/include), 2026-09-23

Picks from the "Area 6 Fix Plan, Part 2" page (db doc `plan/picks_area6_part2`): fix 1, 2, 3, 5, 6, 8, 9, 10, 11, 12, 13; explain first 4, 7, 15; leave 14, 16; then part 3 (scripts/ai). The eleven fixes are below, numbered by page item; the three explain-first items are answered in chat and appended here when decided. Nothing compiled by me.

### 16.1 (item 1) Throwing's powerscroll cap reads the right matrix slot
- File: `scripts/include/attributes.inc:450` (AwardRawAttributePoints)
- Was: the 50-slot cap matrix was read by raw skill id, with only Alchemy (0) remapped to slot 49; Throwing (57) read past the end, the arithmetic on the error value stayed an error, and "new value above the cap" was always false, so Throwing had no cap at all for anyone holding a matrix.
- Now: the slot comes from NormalizeSkillAndSlot (49 for Alchemy, 50 for Throwing, the id itself for 1-48), read through CInt so an empty slot means the base cap; the hand-written Alchemy special case and a stray semicolon on the 1500 clamp are gone.

### 16.2 (item 2) Astral damage drains mana and stamina through the vitals functions
- File: `scripts/include/damages.inc:208`, `:213`, `:223` (ApplyTheAstralDamage)
- Was: the test read `who.mana` and the two zeroing writes wrote `who.mana`/`who.stamina`, none of which are character members in this engine; the test was always false, the damage minus the target's mana (negative when mana exceeded the hit) went to the stamina step and raised stamina, and mana was never touched.
- Now: GetMana for the test, SetMana(who, 0) and SetStamina(who, 0) for the writes.

### 16.3 (item 3) The necro self-burn on a fizzle deals damage again
- File: `scripts/include/spelldata.inc:261` (BurnSelf; Kill, Liche, Sorcerer's Bane)
- Was: Resisted was called with the circle and the target swapped, every step of the resist maths errored, the burn was 0.
- Now: arguments in the right order; the caster takes the resisted half of the spell damage as intended.

### 16.4 (item 5) Staff-marked items are priced at zero in the vendor sell list
- File: `scripts/include/mrcspawn.inc:477` (ModifyPCSellList)
- Was: the LFucker sweep was `foreach item in EnumerateItemsInContainer(backpackitems)`, an enumerate of an array nested inside the pricing loop; it errored and never ran.
- Now: one loop over the backpack list after the pricing loop, `packitem.buyprice := 0` for marked items.

### 16.5 (item 6) The quest-item direction hint reaches the player
- File: `scripts/include/speech.inc:470`, `:496` (GiveQuestieDirections)
- Was: both private whispers named the item as the listener.
- Now: the player.

### 16.6 (item 8) Skill title 43 is Wrestler
- File: `scripts/include/skilltitles.inc:52`
- Was: entry 42 written twice (Fencer, then Wrestler), 43 never set.
- Now: 43.

### 16.7 (item 9) Townsfolk sayings, damage check and the minstrel's tune
- File: `scripts/include/townsfolk.inc:13` (SayRandomFromArray), `:18` (tables), `:98` (flee loop), `:141` (PlayMidi)
- Was: the three tables were filled from index 0 (refused; each held nine lines) and drawn with Random(10); the flee loop compared the event type with the word "damage"; PlayMidi reused the listener's name for every online player and tested the first listener's distance for all of them.
- Now: tables 1-10, draw Random(len)+1; SYSEVENT_DAMAGED; each online character on the singer's realm within 15 tiles gets the packet (the listener parameter is marked unused).

### 16.8 (item 10) Numeric MoveSpeed values are accepted
- File: `scripts/include/attributes.inc:2274` (GetSpeed)
- Was: the digit loop started at position 0, an error, so every numeric value was rejected with a console print and the slowest speed.
- Now: starts at 1.

### 16.9 (item 11) Teleporter placement retries stop after a hundred tries
- File: `scripts/include/teleporters.inc:2309`, `:2331` (CreateTeleporters)
- Was: `while(!teleporter || count >= 100)` and `while(teleporter.decayat != 0 || count >= 100)`, endless once the counter reached 100.
- Now: `&& count < 100` in both.

### 16.10 (item 12) Jail checks use the current jail
- Files: `scripts/include/jailcheck.inc:9` (Jailcheck, used by misc/logofftest.src), `scripts/include/skillpoints.inc:270` (AwardRawSkillPoints)
- Was: both tested the old-map box britannia 5272-5310/1160-1190.
- Now: Jailcheck tests a box of JAIL_BOX_RADIUS (40) tiles around DEFAULT_LOCATION_JAIL_X/Y on DEFAULT_LOCATION_JAIL_R (locations.inc, included by jailcheck.inc); skillpoints.inc includes jailcheck and calls it, so skill gain stops in the jail again. The 40-tile extent is my assumption.

### 16.11 (item 13) Two cleanups
- Files: `scripts/include/npccast.inc:593`, `:650` (rising fire, spectre touch); `scripts/include/privs.inc:69` (AlterPrivsBase)
- Was: "Cant cast in safe area" printed to the console per attempt; `who.disable(who, priv)` on demotion.
- Now: prints removed; `who.disable(priv)`.

### 16.12 (item 4, answered in chat) Loot stacks double on either hunting power hour
- File: `scripts/include/starteqp.inc:1570` (CreateFromStackString)
- Was: `!shard hour || !personal hour` gave the normal amount unless both hours were on; the item chance rolls in the same file double on either.
- Now: `&&`, so a stack doubles when either hour is on (your pick: "either").

### 16.13 (item 15, answered in chat) The four Ter Mur teleporter rows point at Ter Mur
- File: `scripts/include/teleporters.inc:2185` (four rows, 511-514, 584, 11)
- Was: destination realm "Felucca", which does not exist on this shard, so the tiles did nothing.
- Now: destination realm "termur" (your call: the links are on the Ter Mur realm). The self-target row (951,2885,35 to itself) and the thirteen duplicate sources are unchanged; the duplicates' second destinations are the same tile or one tile off.

### 16.14 (item 7, answered in chat) The class-bonus getters no longer sweep the paperdoll
- File: `scripts/include/classes.inc:36` (ClasseBonus), `:64` (ClasseBonusBySkillId)
- Was: both ran unequipRestrictedItems (every worn item, two 5 ms pauses each, class restriction lists rebuilt per item) before returning the multiplier, from 127 call sites including the combat hit script and the spell resist roll.
- Now: they only return the multiplier. The sweep still runs where the class is actually set: AssignClasse (the 10-minute class-check loop in pkg/opt/summoning/checkclasse.src, .showclasse, the RPer and class boost stones), and the equip hook (scripts/control/skilladvancerequip.src) refuses prohibited items up front. Your question about dropping skills, equipping prohibited items and scrolling back: the class property only returns through AssignClasse, which strips the items as it sets it, so that path stays closed; a bypass-equipped item while classed is now caught by the next loop pass instead of the next swing.

### Left by your picks
- 14 The class item rule at classes.inc:461 (general-skill enchantments count as class-restricted) stays as the standing rule.
- 16 The twenty-seven unreferenced includes stay (list in the review notes).

## 17. Area 6, part 3 (scripts/ai), 2026-09-23

Picks from the "Area 6 Fix Plan, Part 3" page (db doc `plan/picks_area6_part3`): fix 2, 6, 7, 8, 9, 10, 11, 12, 14, 16; explain first 4, 15; leave 1, 3, 5, 13, 17; then part 4 (staff commands). The ten fixes are below, numbered by page item; the two explain-first items are answered in chat and appended here when decided. Nothing compiled by me.

### 17.1 (item 2) Spawn-point anchors are applied again
- File: `scripts/ai/setup/modsetup.inc:171`
- Was: the "Anchor" cprop was read as a struct (`anchor.x`, `.y`, `.range`, `.psub`), but the live spawner (`pkg/opt/spawnpoint/checkpoint.src:502-509`, `:640-647`) writes a six-slot array {x, y, range, psub, z, realm}; the four reads gave nothing, CInt made them 0, and SetAnchor with a range of 0 switches the anchor off. Spawned NPCs were never leashed to their point (and for the 36 scripts that include modsetup after their setup include, the template's own dstart anchor was wiped as well).
- Now: struct fields when `anchor.x` is set (treasure-map guardians), else slots 1-4. Engine rule (npc.cpp anchor_allows_move): outside the range each step further away is refused with probability (distance - range) x psub percent, floor 5%, and not at all while in war mode; the spawner default is range 4, psub 25.

### 17.2 (item 6) Old field names in four AI files
- Files: `scripts/ai/chaosmultikillpcs.src:178`, `:206`, `:265`, `:445`, `:467`; `scripts/ai/main/vortexloopkill.inc:87`; `scripts/ai/sum.src:99`; `scripts/ai/soulwhisperer.src:439`
- Was: `me.mana := me.mana + N` (three no-ops), `me.mana > 20` (never true: no Earth Blessing self-buff), `opponent.mana > 20` (never true: mages met in melee), `max_mana := critter.mana` (error, so the vortex always took the first mobile in the list), `CInt(me.dexterity)` (0, so a fixed 300 ms step delay). None of these are members on a mobile in this engine.
- Now: SetMana/GetMana and GetDexterity.

### 17.3 (item 7) Hidden-player and field sweeps on the NPC's realm
- Files: `scripts/ai/main/chaoskillpcsloop.inc:31` (look_around), `scripts/ai/soulwhisperer.src:415`
- Was: no realm argument, so both list calls searched britannia (the engine default) while the NPC stood on britannia_alt.
- Now: `me.realm`.

### 17.4 (item 8) The totem's "vamp" works on a fresh totem
- File: `scripts/ai/humuc.src:286`
- Was: with "lastvamp" unset, `now - lastvamp` was an error and "error > 600" never true, so the success branch (the only writer of "lastvamp") was unreachable; `master.mana` (not a member) never clamped the drain; with "IsMage" unset the drain amount was an error too, which would have set both mana values to the minimum.
- Now: CInt on both cprops, "never vamped" counts as ready, the clamp reads GetMana(master).

### 17.5 (item 9) Loke and Thor no longer carry a spare boss weapon
- Files: `scripts/ai/loke.src:45`, `scripts/ai/thor.src:45` (CloseDistance)
- Was: on the first call both a melee weapon and a bow were created in the pack (flags named with a slur and a rude word); each range swap then created a fresh copy to equip, so one pre-made copy was never used and went to the corpse. The old weapon was also moved into a pack variable that was 0.
- Now: the function reads what is in each hand and swaps only when the wrong weapon (or none) is held: melee within 7 tiles, bow beyond. Whatever is in either hand at a swap is destroyed, as the old code did for the main hand. The flags and the pack variable are gone. Deviation from the page wording ("create only the bow up front"): nothing is pre-created; each weapon is made when first needed.

### 17.6 (item 10) Rooted monsters report "in place"
- File: `scripts/ai/immobile.src:83` (CloseDistance; reaper, corpser, carnivorous plant)
- Was: slept and returned nothing; fight.inc's case on the result matched no label, so the wait stayed 0, no counter advanced, the NPC never turned toward its target and the loop spun every sleepdelay ms (about 150 ms at their dexterity) while any opponent was within 20 tiles.
- Now: returns 1 after the pause; the fight loop waits a second per pass and faces the target.

### 17.7 (item 11) Blackjack double-down flag
- File: `scripts/ai/gambler.src:229` (blackjack)
- Was: a second `dd` declared inside blackjack hid the file-level flag; the double-down set the local, and NewHand/LookAtHands (which read the file-level one) never halved the bet back, so every hand after a double-down was charged the doubled bet.
- Now: the local declaration is gone; all three functions share the file-level flag.

### 17.8 (item 12) Doppel form-change console print
- File: `scripts/ai/combat/doppelcombatevent.inc:112` (line before)
- Was: `print("graphic is " + form)` on every shape change.
- Now: removed.

### 17.9 (item 14) Water shrine "claim" checks the shells without writing one
- File: `scripts/ai/water.src:248` (claim), `:474` (checkforallshells)
- Was: claim called setshells(who, 0), which wrote a "shell0" cprop on the player and said "You found Shell piece 0 of 9." before testing the set.
- Now: the all-nine test is its own function, checkforallshells; setshells writes the piece, says the line and returns that test; claim calls the test directly. Players who already carry a "shell0" cprop keep it (harmless).

### 17.10 (item 16) Town guard old-map kill boxes
- File: `scripts/ai/townguard.src:34` (program start)
- Was: a guard standing in 1385-1414/3729-3758 or 4413-4428/1152-1162 at start killed itself, with no realm test; the boxes are old britannia coordinates and guards live on britannia_alt.
- Now: the two boxes are gone (the killme function stays, unused).

### 17.11 (item 15, answered in chat) The high priest's forgiveness fine has the same floor and cap as the other prices
- File: `scripts/ai/highpriest.src:465` (CheckWhyHeGave)
- Was: the donation was compared against the raw class level x 2500 (0 for a classless player, so one coin forgave), and the clamp lines after the comparison clamped the level instead of the price.
- Now: a `fine` of level x 2500, floored to 1000 when below 2500 and capped at 60000 (the remove-curse rule), compared before the "blessed" line.

### 17.12 (item 4, answered in chat) The vendor training window calls the keys method
- File: `scripts/ai/merchant.src:1135` (TrainGump), `:1220` (TrainEntry)
- Was: `foreach key in (data.keys)`; the compiled listing shows that as `get-member keys`, a member read, which in the stock engine looks up a dictionary key named "keys" and finds nothing, so the loop ran zero times and nothing was charged or taught. The shard runs its own engine build (POL100.3.0, core-changes.txt at the repo root; source not on this machine) and you reported training working, so whether that build treats the bare form as the method is unverified; the same bare form sits in animaltrainer.src:718, textcmd/player/dropskills.src:103, textcmd/coun/privs.src:215 and :242, textcmd/admin/gcmds.src:159, items/mboard.src:134 and include/reportmurder.inc:69, left as they are by your pick. TrainEntry also added a double to the skill setter.
- Now: `data.keys()`, correct on any engine and a no-op if the bare form already worked; the typed level is rounded to whole tenths before the setter.

### Left by your picks
- 1 NPC looting stays off: `scripts/ai/main/loot.inc:13` and `lootblockers.inc:10` keep their local `me`, recorded as intentional (looting has been off on this shard since the first commit).
- 3 Menog guards keep `Fight(ev.src)` and the inverted warning time.
- 5 "Sell all" and "sell bag" keep buying staff-marked items.
- 13 The minstrel keeps `me.frozen := 0`.
- 17 The 13 unreferenced scripts, 15 unused includes, the merchant's dead sell functions and the questie/listener/Rabbit template entries stay (list in the review notes).

## 18. Area 6, part 4 (staff commands, util, console, www), 2026-09-23

Picks from the "Area 6 Fix Plan, Part 4" page (db doc `plan/picks_area6_part4`): fix 1-16 (3 as ".makekey only"); 18 "set every cap slot to 20"; leave 19; explain first 17, 20, 21; then the closing summary of the review. The seventeen fixes are below, numbered by page item; the explain-first items are answered in chat and appended here when decided. `config/command_synopses.cfg` regenerated (items 14, 15, 18). Nothing compiled by me.

### 18.1 (item 1) .globalnoloot lasts the minutes typed
- File: `scripts/textcmd/admin/globalnoloot.src:20`
- Was: the minutes were multiplied by 60 when read and again at the Sleep, so `.globalnoloot 60` broadcast "3600 minutes" and held the flag (read by can_remove_container.src, antiloot.inc and the doubleclick packethook) for 60 hours.
- Now: read as minutes, multiplied once at the Sleep; the broadcast shows the minutes typed.

### 18.2 (item 2) Thawed mobiles stay thawed across a restart
- Files: `scripts/textcmd/coun/unparalyze.src:16`, `scripts/textcmd/coun/thaw.src:25`, `scripts/textcmd/seer/info.src:643` (unfreeze button), `scripts/textcmd/seer/thawme.src:6`
- Was: `.freeze` sets a "frozen" cprop that 61 AI scripts read at start to re-freeze themselves; `.unparalyze` set it to 1 after unfreezing and the other three never erased it, so a thawed NPC froze again on the next script start.
- Now: all four erase the cprop.

### 18.3 (item 3) .makekey uses the lock-id name every lock check reads
- File: `scripts/textcmd/gm/makekey.src:19` and the two writes below it
- Was: "lockid" on the lock and the key; the key package, container use, planks, ropes and redeed read "LockID", and cprop names are case-sensitive (std::map on the name), so the key never fit and an existing LockID was ignored. The same lowercase name in scripts/include/chests.inc:29 and scripts/items/shipdeed.src:92-96 is left by your pick ("makekey only").
- Now: reads and writes "LockID".

### 18.4 (item 4) The .info cage cleans itself up
- File: `scripts/textcmd/seer/info.src:973` (first of eight bars)
- Was: `tempitem.decay := 20` on each bar; items have no `decay` member (objmembers.h has MBR_DECAYAT only), so the assignment was dropped and the bars were permanent.
- Now: `decayat := ReadGameClock() + 20`, the pattern .admin already used.

### 18.5 (item 5) .admin resurrect recolours the player
- File: `scripts/textcmd/admin/admin.src:376`, `:379`
- Was: `who.truecolor` / `who.color`, the staff member.
- Now: `client`, the resurrected player.

### 18.6 (item 6) .unconcealhim works
- File: `scripts/textcmd/gm/unconcealhim.src:14`
- Was: `targ.cmdlvl` (no such field) gave an error; `error >= number` is always true in this engine (operator>= is !(<) and an error is never "less than" a number: OTError 8 > OTLong 3), so every target was refused and told someone had tried.
- Now: `cmdlevel`.

### 18.7 (item 7) .untile removes tiles
- File: `scripts/textcmd/admin/untile.src:97`
- Was: ListItemsAtLocation without a realm; the module default realm (_DEFAULT_REALM in uo.em) is britannia, the unused old map, so the loop never saw an item on britannia_alt.
- Now: `Item1.realm`.

### 18.8 (item 8) The .releaseinfo Go buttons move the staff member
- File: `scripts/textcmd/coun/releaseinfo.src:135` (case), `:151` (new ReleaseInfoGoTo)
- Was: MOVEOBJECT_FORCELOCATION was passed as the realm and the flag omitted, so the engine rejected the move while "Moved to..." printed.
- Now: a helper takes x, y, z and the realm (word 7 of the "Player: <serial> XYZR: x y z realm" record; the caller's realm when a record has none) and the flag; the message says "Could not move there." when the move fails.

### 18.9 (item 9) .newiteminfo shows the use-script section
- File: `scripts/textcmd/gm/newiteminfo.src:1032`, `:2488` (GetMiscPropForUseScript, uncalled)
- Was: ReadConfigFile(":magic:usescriptdesc") and ":usescripts:usescriptdesc"; no package has either name, so the read failed and the script name and charge lines never appeared.
- Now: ":shilitems:usescriptdesc", where the file lives (line 1901 already read it from there).

### 18.10 (item 10) .gorealm accepts "x y" and "x y z"
- File: `scripts/textcmd/coun/gorealm.src:93`
- Was: with two words the GetMapInfo record itself was stored as the z and nothing was passed as the realm; with three the realm was still missing; only "x y z realm" moved.
- Now: the realm defaults to the caller's and the z to the map's z at that spot when not typed.

### 18.11 (item 11) Two console prints removed
- Files: `scripts/textcmd/admin/deathgate.src:84` (the Print of the gate settings array, formerly the line above), `scripts/textcmd/admin/getglobal.src:6` (the "Property Name:" print, formerly the line above)

### 18.12 (item 12) .resetpw stops on an unknown account
- File: `scripts/textcmd/admin/resetpw.src:13`
- Was: no return after "Could not find that account!", so a password was generated and "set" on the error value.
- Now: returns.

### 18.13 (item 13) .setallskills covers every skill id
- File: `scripts/textcmd/admin/setallskills.src:17`
- Was: `for i := 0 to 48`, which skipped the ids added above 48 (Throwing is 57).
- Now: loops over GetSkillIds() like .setclass.

### 18.14 (item 14) .px / .py / .pz synopses, .pz realm
- Files: `scripts/textcmd/gm/px.src:1`, `scripts/textcmd/gm/py.src:1`, `scripts/textcmd/gm/pz.src:1` and `:20`
- Was: the synopses (shown by the command help) said "Print the X coordinate of a targeted object to console"; the commands move the object N tiles. `.pz` moved it in the caller's realm.
- Now: "Move a targeted object N tiles along the X axis (default 1, negative allowed)" (Y likewise; Z: "up or down"); `.pz` uses the object's realm.

### 18.15 (item 15) .class synopsis
- File: `scripts/textcmd/admin/class.src:1`
- Was: "Display or set the class of a targeted player"; the program acts on the caller.
- Now: "Choose a class from a gump and set your own skills: the class skills to N (default 150), all others to 0".

### 18.16 (item 16) .npclist script names
- File: `scripts/textcmd/seer/npclist.src:41`, `:55`
- Was: "mennoguard" (the script is menogguard) and "formationskillpcs" (formationkillpcs), so those NPCs were counted under "other".
- Now: the real names.

### 18.17 (item 18) .maxcaps raises every cap to 150 instead of wiping the matrix
- File: `scripts/textcmd/admin/maxcaps.src:12` (rewritten, 22 lines before)
- Was: filled a local copy of the target's "pScrollMatrix" in slots 0..48, then saved the number 20 as the whole property; attributes.inc:446-453 reads a slot from a truthy matrix, a slot read on a number is an error, CInt makes it 0, so every cap on the target fell to BASE_CAP (130) and the scroll gains were lost. The synopsis said "Display".
- Now: targets a player (mobiles only, no NPCs), writes a fresh SKILLID_MAX-slot matrix of 20s (each slot +20 = the 150 cap), tells both parties and logs the command; synopsis updated. The unused mdgumps/spelldata includes are gone.

### 18.18 (item 17, answered in chat) The skill-id guard in the .info skill editor can fire
- File: `scripts/textcmd/seer/info.src:1267` (EditSkill)
- Was: `skill>48 and skill<0`, which no number satisfies; 48 was also stale (ids go to 57).
- Now: rejects any id that is not in GetSkillIds().

### 18.19 (item 21, answered in chat) .restartall lists each realm once
- File: `scripts/textcmd/test/restartall.src:17` (rewritten, 24 lines before)
- Was: a sweep of the caller's realm in 32-tile squares at five heights, about 123,000 ListMobilesNearLocation calls; NPCs on the other five realms were never restarted and RestartScript was also called on players.
- Now: one ListMobilesInBox over each realm from Realms() (its width and height), NPCs only (npctemplate), a 1 ms pause every 100 restarts and a per-realm progress line. The seven unreferenced util includes stay by your answer.

### 18.20 (item 20, answered in chat) .makemoongates deleted
- Files: `scripts/textcmd/admin/makemoongates.src` and its `.ecl .dbg .dbg.txt .dep .lst`; `config/command_synopses.cfg` regenerated (344 commands).
- Was: created the classic 0x6002 gate item at the eight Felucca town spots from scripts/include/moongate.inc, with no realm (so on britannia, the unused old map, via the uo.em default), and stamped GateDest cprops that the live walk-on script never reads. The moongates package (pkg/opt/moongates/start.src -> CreateMoongates()) builds the real network, 0x6201 items at the live table's spots, at every server start.
- Now: gone. scripts/include/moongate.inc stays (mazegate and cwstone still include it). The other four old-map commands stay by your answer.

### Left by your picks
- 19 chat mute commands (.chattimeout, .chatglobaltimeout, .chatban) stay as they are: their flags have no reader and the engine has no chat system; listed in the part-4 notes.
- 20 old-map commands: .home, .setupchristmas, .setupsanta and .removechristmas stay (your answer in chat); .makemoongates deleted (18.20).
- 21 the seven unreferenced util includes stay (your answer in chat: rewrite .restartall only, see 18.19).

## Docs and conventions changed

- `.claude/skills/escript-gotchas/SKILL.md`: #5a no short-circuit evaluation, #16 consume first then create, #17 `SaveOnExit 0` for temporary items; trigger line updated.
- `.claude/subagent-briefing.md`: the same three rules in short form.
- 2026-09-22 (area 4): gotcha 19 in `.claude/skills/escript-gotchas/SKILL.md` and a bullet in `.claude/subagent-briefing.md`: `item.container` of a worn item is the wearer (`WornItemsContainer::make_ref()` returns the character), not a container. `config/command_synopses.cfg` regenerated for `.updatetp`.

- 2026-09-22 (area 5 part 2): gotcha 20 in `.claude/skills/escript-gotchas/SKILL.md` and a bullet in `.claude/subagent-briefing.md`: two unset values compare equal (`.party` on someone with no party, an absent CProp on both sides), guard with truthiness first; new include `scripts/include/spellgrants.inc` (Dragon Skin grant and spell poison immunity reclaim, called from logon.src).

- 2026-09-23 (area 6 parts 3 and 4): gotchas 21 (mobiles have no hp/mana/stamina/dexterity members; use the vitals functions), 22 (call dictionary methods with parentheses), 23 (never omit the realm argument: the uo.em default is britannia, the unused old map) and 24 (cprop names are case-sensitive) in `.claude/skills/escript-gotchas/SKILL.md`, with matching bullets in `.claude/subagent-briefing.md`. `config/command_synopses.cfg` regenerated for the part-4 synopsis changes and the .makemoongates deletion.

## Files deleted

- `pkg/std/tailoring/autoloom_animate.src` (untracked) and its `.ecl .dbg .dbg.txt .dep .lst`
- `pkg/systems/combat/shilcombat.inc`
- `pkg/std/spells/bless timer.src`, `pkg/std/spells/protection with timer.src` and their compiled outputs
- `pkg/opt/vanityshop/include/mountFunctions.src` (2026-09-22, area 5 part 1): included by nothing, one broken loop; its untracked .ecl/.dep/.lst/.dbg outputs removed too.
- `pkg/opt/necro/sunderingsword.src` (2026-09-22, area 5 part 2): used by no item in any itemdesc; compiled outputs removed too.
- `pkg/opt/earth/shapechange.cfg` (2026-09-22, area 5 part 2): unused twin of shapeshift.cfg.
- `pkg/opt/zuluitems/dyecheck.src` (2026-09-22, area 5 part 3): older unbound copy of the dye tub script (the tub uses the dyteitems copy); compiled outputs removed too.
- `scripts/textcmd/admin/makemoongates.src` (2026-09-23, area 6 part 4, your answer in chat): obsolete classic-map gate builder; compiled outputs removed too; command help regenerated.

## Edit-site index

Line numbers of the dated `2026-09-21` / `2026-09-22` comments, as of the end of section 9 (regenerated by scanning the files).

| File | Lines |
|---|---|
| .claude/skills/escript-gotchas/SKILL.md | 111 |
| config/itemdesc.cfg | 356 |
| pkg/items/containers/container/canInsert.src | 45 |
| pkg/items/containers/container/canRemove.src | 49 |
| pkg/items/containers/container/use.src | 74 |
| pkg/multis/boat/multi/listener.src | 138, 157 |
| pkg/multis/boat/tiller/canInsert.src | 8 |
| pkg/multis/boat/tiller/methods.src | 96 |
| pkg/multis/customhousing/include/house.inc | 15, 286, 313, 636, 679 |
| pkg/multis/customhousing/scripts/customhousedeed.src | 100 |
| pkg/multis/customhousing/sign.src | 26, 104, 115, 250, 415, 449, 933, 971, 1049 |
| pkg/multis/customhousing/syshook/closecustomhouse.src | 11 |
| pkg/multis/house/multiSign/control.src | 28, 54, 493 |
| pkg/multis/house/multiSign/use.src | 439, 540, 839, 932, 1198, 1220 |
| pkg/multis/staticHousing/config/settings.cfg | 93 |
| pkg/multis/staticHousing/logon.src | 10 |
| pkg/multis/staticHousing/sign/control.src | 96 |
| pkg/multis/staticHousing/sign/destroy.src | 121 |
| pkg/multis/staticHousing/sign/use.src | 274, 325, 395, 1198 |
| pkg/opt/ArtifactSystem/artifactbox.src | 10 |
| pkg/opt/ArtifactSystem/itemdesc.cfg | 1 |
| pkg/opt/Donator/donatorbearstone.src | 105 |
| pkg/opt/Donator/donatorhorsestone.src | 105 |
| pkg/opt/Donator/donatorllamastone.src | 104 |
| pkg/opt/Donator/donatorostardstone.src | 106 |
| pkg/opt/Donator/include/playertown.inc | 3 |
| pkg/opt/Events/textcmd/seer/createEventBag.src | 6 |
| pkg/opt/MagicWands/magicwands.src | 61 |
| pkg/opt/Staff/RecordXYZ.src | 19 |
| pkg/opt/alryc/textcmd/test/mounttest.src | 2 |
| pkg/opt/areas/callguards.src | 44 |
| pkg/opt/areas/include/areapolicy.inc | 17, 161, 177, 393 |
| pkg/opt/areas/textcmd/admin/areas.src | 194, 454 |
| pkg/opt/champspawns/include/rewards.inc | 7, 12 |
| pkg/opt/champspawns/scripts/control.src | 96 |
| pkg/opt/christmas/Christmasgifts.src | 14 |
| pkg/opt/crafterboost/make_crafter_boosts.src | 119 |
| pkg/opt/crafterboost/refinement_gump.src | 148, 186 |
| pkg/opt/decoratefacets/commands/test/udestroymany.src | 39 |
| pkg/opt/earth/antidote.src | 86, 88 |
| pkg/opt/earth/bookofearth.src | 91, 118 |
| pkg/opt/earth/druidscroll.src | 23 |
| pkg/opt/earth/earthportal.src | 19, 163 |
| pkg/opt/guilds/commands/player/guilds.src | 673, 2028, 2124, 2142, 2156, 2273, 3139 |
| pkg/opt/guilds/commands/test/changeguildownership.src | 28, 97 |
| pkg/opt/guilds/include/guilds.inc | 409, 587 |
| pkg/opt/guilds/ondelete.src | 36 |
| pkg/opt/holybook/angelicfeast.src | 23 |
| pkg/opt/holybook/angelicgate.src | 22 |
| pkg/opt/holybook/holybook.src | 96, 123 |
| pkg/opt/holybook/holyscroll.src | 22 |
| pkg/opt/holybook/revive.src | 56 |
| pkg/opt/holybook/sanctuary.src | 73, 76 |
| pkg/opt/holybook/wrathofgod.src | 93 |
| pkg/opt/loot/antiloot.inc | 69 |
| pkg/opt/lootlottery/commands/GM/cfglotto.src | 101, 125 |
| pkg/opt/moongates/itemdesc.cfg | 60 |
| pkg/opt/msg/commands/player/msg.src | 169 |
| pkg/opt/necro/codexdamnorum.src | 154, 179 |
| pkg/opt/necro/necroscroll.src | 20 |
| pkg/opt/omegacache/omegacache.inc | 466, 725, 1116, 1166, 1482 |
| pkg/opt/powerhour/textcmd/test/resetph.src | 18 |
| pkg/opt/powerscrolls/createpowerscroll.src | 6 |
| pkg/opt/powerscrolls/randomTome.src | 10 |
| pkg/opt/questpkg/include/queststate.inc | 18, 155, 490, 537 |
| pkg/opt/rituals/include/rituals.inc | 21, 206, 338, 354, 542, 565, 1007 |
| pkg/opt/rituals/rituals/demonstration.src | 99 |
| pkg/opt/roleplaying/macrotimer.src | 20 |
| pkg/opt/roleplaying/rperstone.src | 106 |
| pkg/opt/roleplaying/textcmd/admin/fixstartgear.src | 79 |
| pkg/opt/roleplaying/textcmd/coun/macrotest.src | 15 |
| pkg/opt/songbook/songbook.src | 88, 115 |
| pkg/opt/songbook/songofbeckon.src | 118, 137 |
| pkg/opt/songbook/songofcloaking.src | 96 |
| pkg/opt/songbook/songofsalvation.src | 104 |
| pkg/opt/songbook/songofsirens.src | 76 |
| pkg/opt/songbook/songscroll.src | 38 |
| pkg/opt/spawnpoint/checkpoint.src | 52, 553, 685 |
| pkg/opt/spawnpoint/include/customnpc.inc | 97 |
| pkg/opt/spawnpoint/spawnpointmanager.src | 101 |
| pkg/opt/spawnpoint/spawntriggerwalkon.src | 8 |
| pkg/opt/spawnpoint/textcmd/admin/despawn.src | 13 |
| pkg/opt/spawnpoint/textcmd/admin/forcespawn.src | 13 |
| pkg/opt/spawnpoint/textcmd/admin/gotospawnpoint.src | 78 |
| pkg/opt/spawnpoint/textcmd/admin/newmobedit.src | 309, 351 |
| pkg/opt/spawnpoint/textcmd/admin/primespawn.src | 13 |
| pkg/opt/summoning/npcsummoning.src | 27 |
| pkg/opt/summoning/processpoisonmod.src | 105 |
| pkg/opt/summoning/summoning.src | 82 |
| pkg/opt/townstones/electionwatch.src | 11, 62, 161 |
| pkg/opt/townstones/tstone.inc | 162 |
| pkg/opt/townstones/tstone.src | 535, 554, 585, 1032, 1069, 1098 |
| pkg/opt/vanityshop/customitemdye.src | 21 |
| pkg/opt/vanityshop/customitemname.src | 20, 56 |
| pkg/opt/vanityshop/runebookdye.src | 21 |
| pkg/opt/vanityshop/vanityshop.src | 251 |
| pkg/opt/versebook/Bardic_Boulders.src | 106 |
| pkg/opt/versebook/Beastal_Bond.src | 98, 166 |
| pkg/opt/versebook/Corpse_Distention.src | 102, 115 |
| pkg/opt/versebook/Dragon_Skin.src | 70, 119, 135, 145, 160 |
| pkg/opt/versebook/Lesser_Healing.src | 103 |
| pkg/opt/versebook/Life_Balance.src | 101 |
| pkg/opt/versebook/Not_Implemented.src | 79 |
| pkg/opt/versebook/Shadows.src | 105 |
| pkg/opt/versebook/Sonic_Disturbance.src | 103 |
| pkg/opt/versebook/Spirit_Flock.src | 98 |
| pkg/opt/versebook/include/versefunctions.inc | 650, 686, 713 |
| pkg/opt/versebook/versebook.src | 192 |
| pkg/opt/warriorforhire/warrior.src | 697 |
| pkg/opt/zuluitems/cannon.src | 61, 78, 101 |
| pkg/opt/zuluitems/catapult.src | 26, 62 |
| pkg/opt/zuluitems/dragoneggs.src | 123 |
| pkg/opt/zuluitems/ostardeggs.src | 99 |
| pkg/packethooks/megacliloc/commands/player/updatetp.src | 6 |
| pkg/packethooks/megacliloc/itemdata.src | 171, 216, 508, 596, 602 |
| pkg/packethooks/packethook/packethook.src | 67 |
| pkg/packethooks/packethook/uopacket.cfg | 1 |
| pkg/packethooks/speech/receivespeechhook.src | 31, 58 |
| pkg/packethooks/versionHook/versionhook.src | 32 |
| pkg/std/alchemy/alchemyfunctions.inc | 169, 198, 250, 278, 316 |
| pkg/std/blacksmithy/blacksmithgump.inc | 138, 201, 224, 338, 355 |
| pkg/std/bowcraft/bowcraft.src | 162, 217, 405, 412, 441, 458, 516, 525 |
| pkg/std/bulkorders/bulkorder_matching.inc | 63, 76, 185, 337 |
| pkg/std/bulkorders/bulkorderdeed.src | 334, 364 |
| pkg/std/bulkorders/bulkorderrewards.src | 34, 179 |
| pkg/std/camping/camping.src | 76 |
| pkg/std/carpentry/carpentry.src | 326, 435, 478, 513, 624, 641 |
| pkg/std/cartography/cartography.src | 20, 192, 226 |
| pkg/std/cooking/cooking.src | 173, 210, 391 |
| pkg/std/cooking/hunger.src | 20, 67, 144, 160 |
| pkg/std/cooking/hungerdamage.src | 22 |
| pkg/std/dundee/globeofsosaria.src | 20 |
| pkg/std/healing/healing.src | 227, 299 |
| pkg/std/help/help.src | 176 |
| pkg/std/herding/herd.src | 23 |
| pkg/std/herding/herdedai.src | 13 |
| pkg/std/hiding/hiding.src | 51 |
| pkg/std/inscription/inscription.src | 1031, 1068, 1202 |
| pkg/std/itemid/itemid.inc | 11 |
| pkg/std/itemid/itemid.src | 10 |
| pkg/std/lockpicking/use/picklock.src | 15, 140, 197, 250, 263 |
| pkg/std/musicianship/musicianship.src | 38 |
| pkg/std/removetrap/removetrap.src | 31 |
| pkg/std/runebook/customspells.inc | 99, 126 |
| pkg/std/runebook/runebook.src | 308 |
| pkg/std/runebook/runebookactions.inc | 14, 80, 230, 313 |
| pkg/std/runebook/runicatlas.src | 157, 175 |
| pkg/std/snooping/snooping.src | 51 |
| pkg/std/spells/blade_spirit.src | 59 |
| pkg/std/spells/cure.src | 54 |
| pkg/std/spells/dispel.src | 94 |
| pkg/std/spells/dispel_field.src | 54 |
| pkg/std/spells/gheal.src | 79 |
| pkg/std/spells/heal.src | 83 |
| pkg/std/spells/invisibility.src | 47, 62 |
| pkg/std/spells/mark.src | 84, 101 |
| pkg/std/spells/massdispel.src | 63 |
| pkg/std/spells/mindblast.src | 104 |
| pkg/std/spells/polymorph.src | 78 |
| pkg/std/spells/reactivearmor.src | 26, 50 |
| pkg/std/spells/resurrect.src | 77 |
| pkg/std/spells/teleport.src | 121 |
| pkg/std/spells/unlock.src | 68 |
| pkg/std/spells/vortex.src | 57 |
| pkg/std/tailoring/autoloom_use.src | 16 |
| pkg/std/tailoring/tailoringfunctions.inc | 244, 286, 295, 330, 396, 450, 460, 477 |
| pkg/std/tasteid/tasteid.src | 43 |
| pkg/std/taunt/enticeai.src | 35 |
| pkg/std/tinkering/tinkeringfunctions.inc | 364, 396, 472, 746 |
| pkg/std/tracking/tracking.src | 29 |
| pkg/std/training/dummy_pickpocket.src | 31 |
| pkg/std/traps/trapScripts/setTrap.src | 30, 65 |
| pkg/std/treasuremap/digtreasure.src | 78, 339 |
| pkg/std/veterinary/vet.src | 118, 203 |
| pkg/systems/accounts/acctWatcher/acctWatcher.src | 54 |
| pkg/systems/accounts/config/settings.cfg | 46 |
| pkg/systems/attributes/hooks/shilhook.src | 24 |
| pkg/systems/combat/hooks/omegaattack.inc | 17, 74, 84, 113 |
| pkg/systems/combat/include/hitscriptinc.inc | 65, 786, 794 |
| pkg/systems/combat/paralyzehit.src | 33 |
| pkg/systems/combat/spellstrikescript.src | 45 |
| pkg/systems/combat/thiefpoisonhit.src | 33 |
| pkg/systems/crafting/include/multicraft.inc | 86, 246 |
| pkg/systems/playervendor/playermerchant.src | 1078, 1113, 1420, 1601 |
| scripts/ai/chaosmultikillpcs.src | 178, 206, 265, 445, 467 |
| scripts/ai/gambler.src | 229 |
| scripts/ai/highpriest.src | 465 |
| scripts/ai/humuc.src | 284, 292 |
| scripts/ai/immobile.src | 81 |
| scripts/ai/loke.src | 42 |
| scripts/ai/main/chaoskillpcsloop.inc | 122 |
| scripts/ai/main/vortexloopkill.inc | 87 |
| scripts/ai/merchant.src | 1135, 1220 |
| scripts/ai/setup/modsetup.inc | 166 |
| scripts/ai/soulwhisperer.src | 415, 439 |
| scripts/ai/sum.src | 99 |
| scripts/ai/thor.src | 42 |
| scripts/ai/townguard.src | 34 |
| scripts/ai/water.src | 248, 473 |
| scripts/control/trashControl.src | 9 |
| scripts/include/attributes.inc | 447, 2274 |
| scripts/include/bard.inc | 71 |
| scripts/include/classes.inc | 39, 66 |
| scripts/include/damages.inc | 208, 213, 223 |
| scripts/include/housing.inc | 282 |
| scripts/include/jailcheck.inc | 5 |
| scripts/include/mrcspawn.inc | 475 |
| scripts/include/privs.inc | 69 |
| scripts/include/skillpoints.inc | 268 |
| scripts/include/skilltitles.inc | 52 |
| scripts/include/speech.inc | 470, 496 |
| scripts/include/spelldata.inc | 105, 260, 329, 1483 |
| scripts/include/starteqp.inc | 1568 |
| scripts/include/teleporters.inc | 2184, 2310, 2332 |
| scripts/include/townsfolk.inc | 11, 98, 142 |
| scripts/misc/death.src | 17, 238 |
| scripts/misc/logon.src | 8, 37, 85 |
| scripts/misc/reconnect.src | 6, 24 |
| scripts/playermanager.src | 36 |
| scripts/textcmd/admin/admin.src | 376, 379 |
| scripts/textcmd/admin/globalnoloot.src | 20 |
| scripts/textcmd/admin/maxcaps.src | 8 |
| scripts/textcmd/admin/resetpw.src | 13 |
| scripts/textcmd/admin/setallskills.src | 16 |
| scripts/textcmd/admin/untile.src | 97 |
| scripts/textcmd/coun/gorealm.src | 88 |
| scripts/textcmd/coun/releaseinfo.src | 149 |
| scripts/textcmd/coun/thaw.src | 25, 26 |
| scripts/textcmd/coun/unparalyze.src | 16 |
| scripts/textcmd/gm/makekey.src | 18 |
| scripts/textcmd/gm/newiteminfo.src | 1032, 2488 |
| scripts/textcmd/gm/pz.src | 20 |
| scripts/textcmd/gm/unconcealhim.src | 14 |
| scripts/textcmd/player/cast.src | 67 |
| scripts/textcmd/player/clearmsglog.src | 19, 37, 48, 69, 83 |
| scripts/textcmd/player/disarm.src | 38, 47 |
| scripts/textcmd/player/online.src | 97, 169 |
| scripts/textcmd/player/password.src | 69 |
| scripts/textcmd/player/recalltotem.src | 14 |
| scripts/textcmd/player/removejewels.src | 21 |
| scripts/textcmd/player/showclasse.src | 355 |
| scripts/textcmd/player/trashlb.src | 11, 49, 119 |
| scripts/textcmd/player/undressme.src | 39 |
| scripts/textcmd/seer/info.src | 643, 644, 971, 1267 |
| scripts/textcmd/seer/npclist.src | 41, 55 |
| scripts/textcmd/seer/thawme.src | 6, 7 |
| scripts/textcmd/test/restartall.src | 10 |
