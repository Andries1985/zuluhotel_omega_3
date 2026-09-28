# Ritual Quests — the "24 Rituals Chant Books" quest system

All 24 rituals now have a quest-line. This doc exists so the full picture
(who gives what, what guards it, where it lives, and why it's named what
it's named) survives even if `config/npcdesc.cfg`'s own comments get
trimmed down. Quest ids 2-25 (1-99 is the "core" range in `quests.cfg`;
1 was a deleted test quest; 100+ is the separate fishing quest-line, see
`FishingQuests.md`).

## How the system works (shared engine, built once)

- **Two-piece altar**: every ritual altar is two linked world items (an
  anchor + a companion piece) sharing one config via CProps
  (`RitualAltarQuestId`, `RitualAltarRitualId`, `RitualAltarBossTemplate`,
  `RitualAltarTrophyObjtype`, `RitualAltarReagent<n>`/`RitualAltarReagentAmt<n>`).
  `pkg/opt/rituals/config/itemdesc.cfg`, scripts in `pkg/opt/rituals/altar/`.
- **Offer/Summon**: players target-cursor the required reagents onto the
  altar gump, then Summon spawns the boss template named in
  `RitualAltarBossTemplate`.
- **Trophy**: every boss drops the same shared `QuestSkull` (objtype
  `0xBA3F`, decimal `47679`), dynamically colored/named to match whichever
  ritual it belongs to.
- **Turn-in**: the trophy is handed to the quest-giver for `RewardGold` +
  `RewardItem` (a fixed-objtype chant book unique to that ritual).
- **Quest-giver NPCs**: all use the same generic AI shape (say "quest" to
  open the gump; `CProp QuestGiverTag` ties them to their `Giver` field in
  `quests.cfg`; the "Quest Giver" megacliloc mouseover tag and paperdoll
  "Emissary" title both key off `QuestGiverTag` too). Scripts live in
  `pkg/opt/quests/ai/`.
- **`.questmap`** (GM command) discovers every altar/giver live in the
  world and teleports you to it — no manual location bookkeeping needed.

---

## Dracham Cemetery — Quests 2-5

**Giver: Arch-Mage Mariah, Emissary** (`NpcTemplate mariah`,
`config/npcdesc.cfg`). ***Tied to real Ultima lore*** — Mariah is the
series' recurring archmage companion (Ultima V/VI/VII). Placed in Dracham.

All 4 bosses here are the same "dracoliche difficulty" tier: stats/skills/
spells copied verbatim from `NpcTemplate dracoliche` (2,000 real HP via
`CustomHitsLevel i200000`, `IsMage i2`, `spellkillpcs` AI), differentiated
only by graphic/gear/color to match each one's chant book. *None of the 4
boss names are tied to Ultima lore — all 4 are original invented names*
following an invented "\<Name\> the \<Epithet\>" convention.

| Quest | Ritual | Boss | Reagents (@50 each) | Reward book |
|---|---|---|---|---|
| 2 | Consecration (134) | Voranth the Unhallowed — lore: unknown corrupted thing squatting on ground that was once hallowed. Liche Lord body. | Bloodmoss, Pig Iron | `0xBA40` |
| 3 | Create Focus (135) | Kethrys the Earthbound — lore: a geomancer who died trying to bind Dracham's ley-energy into a mana crystal; the unfinished working curdled hostile. Wraith Lord body. | Ginseng, Black Pearl | `0xBA49` |
| 4 | Attunement (132) | Vaelis the Oathbroken — lore: a guardian spirit bound to a fallen knight's blade, broke its oath once its charge died heirless. Abyssal Liche body. | Nightshade, Spider Silk | `0xBA4A` |
| 5 | Purification (146) | Malgrave the Fouled — lore: an old caster buried at Dracham, corroded past recognition by centuries of ambient curse-magic. Ancient Lich body. | Garlic, Sulphurous Ash | `0xBA4B` |

Altars: `0xBA41`-`0xBA48` (ew/ns pairs). All non-repeatable.

---

## Winterwyn's Everfrost Catacombs — Quests 6-8

**Giver: Frostkeeper Jaana, Emissary** (`NpcTemplate jaana`).
***Tied to real Ultima lore*** — Jaana is the series' recurring healer
companion (Ultima V/VII), though no specific ice/frost tie — more a "safe
canon name" than a strict theme match. Renamed from the original "Ysolde"
2026-09-28. Placed in Winterwyn.

Same dracoliche-tier stat copy as Dracham (2,000 HP, `IsMage i2`,
`spellkillpcs`). *None of the 3 boss names are tied to Ultima lore.*

| Quest | Ritual | Boss | Reagents (@50 each) | Reward book |
|---|---|---|---|---|
| 6 | Mana Dissimal (140) | Zerith the Discordant — lore: an arcane demon whose presence throws nearby spellcasting into chaotic dissonance. Arcane Demon body. | Mandrake Root, Black Pearl | `0xBA52` |
| 7 | Mana Flux (141) | Vornax the Fluxbound — lore: a demon prince embodying raw magical chaos/paradox. Daemon Prince body. | Sulphurous Ash, Nightshade | `0xBA53` |
| 8 | Venom Bane (152) | Skarn the Frostfang — lore: an ice demon whose fangs run with venom despite the cold. Ice Daemon Lord body. | Garlic, Spider Silk | `0xBA54` |

Altars: `0xBA4C`-`0xBA51` (ew/ns pairs). All non-repeatable.

---

## Arcanus (twin-ruin + standing-stone islands) — Quests 9-11

**Giver: Nystul the Enchanter, Emissary** (`NpcTemplate nystul`).
***Tied to real Ultima lore*** — Nystul is a recurring archmage character
across the Ultima series. Placed in Arcanus (Region Arcanus's GoLoc
`1875,502,7`, britannia_alt).

Genuine Balron-tier: copied verbatim from `NpcTemplate balron` (14,000
real HP, `200` flat across every combat/magic skill, the full 20-spell
Balron kit, `chaosspellkillpcs` AI). ***All 3 boss names are tied to real
Ultima lore*** — Astaroth, Nosfentor, and Faulinei are the three
Shadowlords (Falsehood, Hatred, Cowardice) from **Ultima IV**.

| Quest | Ritual | Boss | Reagents (@100 each) | Reward book |
|---|---|---|---|---|
| 9 | Advanced Theurgy (131) | Astaroth, Shadow of Falsehood — southern twin-ruin structure | Eye of Newt, Obsidian, Ginseng, Black Pearl | `0xBA5B` |
| 10 | Protective Aura (145) | Nosfentor, Shadow of Hatred — northern twin-ruin structure | Pumice, Fertile Dirt, Mandrake Root, Spider Silk | `0xBA5C` |
| 11 | Free Movement (138) | Faulinei, Shadow of Cowardice — the second, uninhabited standing-stone island. Only boss with an added `spell paralyze` on top of the shared kit, mirroring its own lore (freezes you where you stand). | Batwing, Pig Iron, Garlic, Nightshade | `0xBA5D` |

Altars: `0xBA55`-`0xBA5A` (ew/ns pairs). All non-repeatable.

---

## Alryc's fire dungeon chain — Quests 12-15

**Giver: Cinderwarden Rudyom, Emissary** (`NpcTemplate rudyom`).
***Tied to real Ultima lore*** — Rudyom is the fire-magic wizard from
Ultima VII's "Forge of Virtue," a near-perfect thematic fit for a fire
dungeon. Placed at Faymoor.

Dungeon chain (real, pre-existing): Aetherfall Bridge → **Brimstone
Catacombs of Alryc** → **Alryc's Magma Halls** → **Alryc's Hellfire
Abyss** → **Alryc's Inferno** "Alryc" is not in-game lore — it's the shard's own developer/admin namespace, so
this was a blank slate for original lore.

Endgame `SuperBoss` tier, escalating per level. All 4 boss names are
original — *none tied to Ultima lore*.

| Quest | Ritual | Boss | Location | Real HP | Reagents (@100 each) | Reward book |
|---|---|---|---|---|---|---|
| 12 | Physical Ward (143) | Korrath the Ironscaled | Brimstone Catacombs | 25,000 | Garlic, Sulphurous Ash, Obsidian, Brimstone | `0xBA66` |
| 13 | Quick Healing (147) | Vraxthel the Self-Mending | Magma Halls | 35,000 | Ginseng, Mandrake Root, Dragon's Blood, Volcanic Ash | `0xBA67` |
| 14 | Vital Infusion (154) | Ashkelon the Soulrent | Hellfire Abyss | 50,000 | Black Pearl, Nightshade, Daemon Bone, Pumice | `0xBA68` |
| 15 | Blood Seeking (133) | Nethrazul the Bloodwyrm | Inferno (final) | 70,000 | Blood Moss, Spider Silk, Executioner's Cap, Bone | `0xBA69` |

Altars: `0xBA5E`-`0xBA65` (ew/ns pairs). All non-repeatable.

---

## Necropolis of Nagash — Quests 16-17

**Giver: Inquisitor Erethian, Emissary** (`NpcTemplate erethian`).
***Tied to real Ultima lore*** — Erethian is the wizard from Ultima IX
who overreached into forbidden magic and created the Guardian; usually an
antagonist rather than a helpful inquisitor, so the fit is thematic
(forbidden/dark magic) rather than a role match. Placed near Bayfort (Region Bayfort's
GoLoc `2036,2732,-7`, britannia_alt).

Dungeon: **Necropolis of Nagash**, a real pre-existing dungeon
(`scripts/include/teleporters.inc`, with its own Boss Room). "Nagash" is
not in-game lore either — it's another shard developer's own name from
the dungeon's original build, not a canon character (and not connected to
the Warhammer character of the same name — original lore was written
fresh for these 2 bosses). *Neither boss name is tied to Ultima lore.*

| Quest | Ritual | Boss | Real HP | Reagents (@100 each) | Reward book |
|---|---|---|---|---|---|
| 16 | Perilous Theurgy (142) | Malachar the Bone Sovereign | 75,000 | Nightshade, Sulphurous Ash, Daemon Bone, Bone | `0xBA6E` |
| 17 | Racial Theurgy (148) | Vharesh, the Deathless Tyrant — Boss Room, hardest of the two | 90,000 (highest HP in the whole quest package at the time it was built) | Mandrake Root, Black Pearl, Executioner's Cap, Eye of Newt | `0xBA6F` |

Altars: `0xBA6A`-`0xBA6D` (ew/ns pairs). Both `Type sUndead`, both
non-repeatable.

---

## Rikktor's Lair — Quests 18-21

**Giver: Stonewarden Thragg, Emissary** (`NpcTemplate thragg`).
*Not tied to any specific Ultima character — an original name.*
Ogre graphic (objtype `0x01`, `Equip ogre` — same body as the real stock
`ogre`/`ogrelord`/`ogrerockthrower` templates), `CProp Type sGiantkin`.
Placed in Mountain Town (GoLoc `4607,2397,0`, britannia_alt).

Dungeon: **Rikktor's Lair**, the real dungeon hosting the `champspawns`
Dragonkin champion spawn (Spawn 2, `pkg/opt/champspawns/config/spawns.cfg`).
Each boss's body/Color/Equip is taken directly from a real stock dragon
template already in this file (`silverdragon`/`goldendragon`/
`waterdragon`/`greatwyrm`), scaled up to `SuperBoss` tier. *None of the 4
boss names are tied to Ultima lore.*

| Quest | Ritual | Boss | Based on | Real HP | Reagents (@100 each) | Reward book |
|---|---|---|---|---|---|---|
| 18 | Resilience (149) | Argentyr, the Unbroken | `silverdragon` | 40,000 | Black Pearl, Mandrake Root, Fertile Dirt, Bone | `0xBA78` |
| 19 | Hardening (139) | Aurelian, the Gilded Scale | `goldendragon` | 50,000 | Garlic, Sulphurous Ash, Obsidian, Pig Iron | `0xBA79` |
| 20 | Elemental Ward (137) | Maelithra, Tide-Warden | `waterdragon` (keeps its `massvatten` spell) | 60,000 | Ginseng, Nightshade, Volcanic Ash, Dragon's Blood | `0xBA7A` |
| 21 | Planard Ward (144) | Zorgathrax, the World-Eater (final) | `greatwyrm` | 80,000 | Spider Silk, Blood Moss, Eye of Newt, Executioner's Cap | `0xBA7B` |

Altars: `0xBA70`-`0xBA77` (ew/ns pairs). All non-repeatable.

---

## Bonecrusher Stronghold — Quests 22-24

**Giver: Warden Julia, Emissary** (`NpcTemplate julia`).
***Tied to real Ultima lore*** — Julia is the gypsy/thief companion from
Ultima VII, fitting for someone who tracks what's stolen. Placed in Erilyn (Region
Erilyn's GoLoc `246,1484,25`, britannia_alt).

Dungeon: **Bonecrusher Stronghold**, a real pre-existing 4-stage ogre
dungeon (Ogre Cave → Lvl1 → Lvl2 → Lvl3 → Throneroom,
`scripts/include/teleporters.inc`). Deliberate twist: these ogres aren't
spellcasters and don't use the rituals — they've just been ambushing
travelers and hoarding the stolen chant books like treasure, with no idea
what they are. All 3 bosses are pure melee brutes (`script killpcs`, no
spells, no `IsMage`), body/Color/Equip taken from real stock templates
(`ogrelord`/`ogrerockthrower`/`ogre4`). "Boss" tier only (`CProp Boss i1`,
no `SuperBoss`) — a deliberate step down from the last 3 batches. *None of
the 3 boss names are tied to Ultima lore.*

| Quest | Ritual | Boss | Based on | Location | Real HP | Reagents (@100 each) | Reward book |
|---|---|---|---|---|---|---|---|
| 22 | Spell Bouncing (150) | Grukthar the Bonecrusher | `ogrelord` | Lvl1 | 10,000 | Ginseng, Black Pearl, Fertile Dirt, Pumice | `0xBA82` |
| 23 | Spell Warding (151) | Mogrash Stonehurler | `ogrerockthrower` | Lvl2 | 12,500 | Mandrake Root, Nightshade, Obsidian, Daemon Bone | `0xBA83` |
| 24 | Venom Mastery (153) | Krugnak, the Hoardking (final — sits on the biggest hoard) | `ogre4` | Throneroom | 16,000 | Garlic, Spider Silk, Eye of Newt, Bone | `0xBA84` |

Altars: `0xBA7C`-`0xBA81` (ew/ns pairs). All non-repeatable.

---

## Tartarus — Quest 25 (the last ritual)

**Giver: Kalabar, Emissary** (`NpcTemplate kalabar`). ***Tied to real
Ultima Online lore*** — Kalabar is the mage from UO's own "Kalabar's
Curse" promotional comic, who cursed the town of Cove so every resident
looked identical. Visual is a Gargoyle (objtype `666`), dressed in a pure
black robe and gnarled staff (`Equipment kalabar`, `config/equip.cfg`,
hue `2305` — `colors.cfg`'s own documented "black"). Placed in Sunken
City (britannia_alt).

Dungeon: **Tartarus** (`regions/regions.cfg`), a genuinely large dungeon
directly adjacent to Region Underworld — "below the Underworld" per the
quest's own lore framing.

Boss: **Sutek, the Serpent's Fang**. ***Tied to real Ultima Online
lore*** — Sutek is the undead antagonist from UO's "Trials and
Tribulations" Cabal storyline. Mechanically a genuine "repurposed Soul
Whisperer": `NpcTemplate sutek` mirrors `soulwhisperfire`'s own stat block
(80,000 HP, the strongest tier in the file), and its AI script
(`pkg/opt/quests/ai/sutek.src`) is a **modified copy** of
`scripts/ai/soulwhisperer.src` — only the random name-override in
`KillPlayers()` was deleted (Sutek keeps his fixed name instead of a
randomly generated "\<Name\>, Soul Whisperer"). Everything else, including
the 75%/50%/25% HP portal-summon mechanic (a random extra superboss from
a shared 36-entry pool joins the fight at each threshold, up to 3 total),
is kept exactly as the original script does it — this was a direct,
explicit decision, not an oversight. The real shared `soulwhisperer.src`
itself was never touched, so every other NPC using it (Carrie, the
soulwhisperfire family, etc.) is unaffected.

| Quest | Ritual | Boss | Real HP | Reagents (@100 each) | Reward book |
|---|---|---|---|---|---|
| 25 | Cursing (136) | Sutek, the Serpent's Fang | 80,000 (+ up to 3 random extra superbosses via the portal mechanic) | Nightshade, Garlic, Executioner's Cap, Bone | `0xBA87` |

Altar: `0xBA85`/`0xBA86` (ew/ns pair). Non-repeatable.


