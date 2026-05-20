# Developer Changelog — v1.0.0

**Range:** `8efb7ef2f60e657bc2b188b0a226185aa73e4832` (initial commit) → `HEAD`  
**Branch:** main  
**Date:** [TBD]

---

## Table of Contents

0. [Areas Policy System Stabilization (May 2026)](#0-areas-policy-system-stabilization-may-2026)
1. [Combat System](#1-combat-system)
2. [Crafting System](#2-crafting-system)
3. [Optional Gameplay Packages](#3-optional-gameplay-packages)
4. [Item Packages](#4-item-packages)
5. [Standard Skill Packages](#5-standard-skill-packages)
6. [Utility Packages](#6-utility-packages)
7. [System Packages](#7-system-packages)
8. [AI & NPC Behavior](#8-ai--npc-behavior)
9. [Shared Scripts & Includes](#9-shared-scripts--includes)
10. [Command Trees](#10-command-trees)
11. [World Data & Regions](#11-world-data--regions)
12. [Root Config, Builds, and Tooling](#12-root-config-builds-and-tooling)

---

## 0. Areas Policy System Stabilization (May 2026)

Summary:
Area policy storage and resolution were fully migrated to a persistent, realm-scoped datafile model. Legacy global-property fallback paths were removed from active runtime checks. The staff areas gump now writes policy state by stable AreaId and persistence across reboot is verified.

Details:
- Added persistent per-realm policy storage in areas package datafiles using descriptor format :areas:area_policies_<realm>.
- Adopted per-area element keys based on strict AreaId tokens from areas.cfg.
- Enforced strict area syntax: every Area line must include id=<value> in token position after coordinates.
- Normalized area ids to short lowercase alphanumeric values and migrated areas.cfg entries accordingly.
- Removed runtime legacy fallback branches from scripts/include/areas.inc (global arrays/properties are no longer used in active area-policy checks).
- Updated admin areas command to:
	- prune stale datastore elements against current areas.cfg,
	- save only current-page changes by AreaId keys,
	- reload after save, and
	- display clean area names without id= prefix in the gump.
- Added robust realm/area input validation for .areas command to prevent silent failures.
- Completed debug hardening and removed temporary parser traces after validation.

Key Files Changed:
- pkg/opt/areas/include/areapolicy.inc
- pkg/opt/areas/textcmd/admin/areas.src
- scripts/include/areas.inc
- pkg/opt/areas/areas.cfg

Validation Notes:
- ecompile clean for all affected files.
- .areas gump open/save flow verified.
- persistence after reboot verified.

## 1. Combat System

**Summary:**
Major overhaul and implementation of the combat system, including on-hit effects, critical hits, elemental damage, and combat event handling. Added support for new combat scripts, configuration files, and modular on-hit logic.

**Details:**
- Added modular on-hit behavior and combat script routing.
- Extended hit, damage, and death handling for combat-driven effects.
- Added combat-related constants, spell hooks, and item interactions.

**Key Files Changed:**
- pkg/systems/combat/* (all combat scripts and configs)
- scripts/ai/combat/* (combat event includes)
- scripts/include/constants/hitscripts.inc
- scripts/include/constants/onhitscripts.inc
- scripts/include/constants/skillids.inc
- scripts/include/constants/skillnames.inc
- scripts/include/constants/propids.inc
- scripts/include/constants/privilegeids.inc
- scripts/include/constants/settings.inc
- scripts/include/constants/syseventids.inc
- scripts/include/constants/timeids.inc
- scripts/include/constants/usescriptids.inc
- scripts/include/creatureRoutines.inc
- scripts/include/creature_spellcast.inc
- scripts/include/damages.inc
- scripts/include/events.inc
- scripts/include/fields.inc
- scripts/include/skilllists.inc
- scripts/include/skillpoints.inc
- scripts/include/skilltitles.inc
- scripts/include/spawn.inc
- scripts/include/spawnnet.inc
- scripts/include/spawnpoint.inc
- scripts/include/spelldata.inc
- scripts/include/statmod.inc
- scripts/include/std.inc
- scripts/include/string.inc
- scripts/include/stringarrays.inc
- scripts/include/sysevent.inc
- scripts/include/utility.inc
- scripts/items/bladed.src
- scripts/items/fletch.src
- scripts/items/warriorforhire.src
- scripts/misc/chrdeath.src
- scripts/misc/death.src
- scripts/misc/skilladv.src
- scripts/misc/skillwin.src

---

## 2. Crafting System

**Summary:**
Major overhaul and implementation of the crafting system, including new crafting options, improved crafting mechanics, and support for custom crafting configurations.

**Details:**
- Added and expanded crafting skill packages and helper includes.
- Updated item, resource, and spawn-related constants used by crafting flows.
- Added utility scripts for repair, loot, potion handling, and related support.

**Key Files Changed:**
- pkg/systems/crafting/* (all crafting scripts and configs)
- scripts/include/constants/cfgfiles.inc
- scripts/include/constants/cids.inc
- scripts/include/constants/cmdlevels.inc
- scripts/include/constants/colors.inc
- scripts/include/constants/creaturetypes.inc
- scripts/include/constants/datastorefiles.inc
- scripts/include/constants/eventids.inc
- scripts/include/constants/facings.inc
- scripts/include/constants/fonts.inc
- scripts/include/constants/fxs.inc
- scripts/include/constants/gumpids.inc
- scripts/include/constants/hitscripts.inc
- scripts/include/constants/itemids.inc
- scripts/include/constants/landtiles.inc
- scripts/include/constants/layers.inc
- scripts/include/constants/locations.inc
- scripts/include/constants/merchanttypes.inc
- scripts/include/constants/mountids.inc
- scripts/include/constants/multiids.inc
- scripts/include/constants/npcai.inc
- scripts/include/constants/npcdata.inc
- scripts/include/constants/npctemplates.inc
- scripts/include/constants/objtypes.zip
- scripts/include/constants/onhitscripts.inc
- scripts/include/constants/privilegeids.inc
- scripts/include/constants/propids.inc
- scripts/include/constants/raceids.inc
- scripts/include/constants/resourceids.inc
- scripts/include/constants/scriptids.inc
- scripts/include/constants/settings.inc
- scripts/include/constants/sfxs.inc
- scripts/include/constants/skillids.inc
- scripts/include/constants/skillnames.inc
- scripts/include/constants/statids.inc
- scripts/include/constants/storageareas.zip
- scripts/include/constants/syseventids.inc
- scripts/include/constants/timeids.inc
- scripts/include/constants/usescriptids.inc
- scripts/include/createnpc.inc
- scripts/include/creatureRoutines.inc
- scripts/include/creature_spellcast.inc
- scripts/include/damages.inc
- scripts/include/events.inc
- scripts/include/fields.inc
- scripts/include/skilllists.inc
- scripts/include/skillpoints.inc
- scripts/include/skilltitles.inc
- scripts/include/spawn.inc
- scripts/include/spawnnet.inc
- scripts/include/spawnpoint.inc
- scripts/include/spelldata.inc
- scripts/include/statmod.inc
- scripts/include/std.inc
- scripts/include/string.inc
- scripts/include/stringarrays.inc
- scripts/include/sysevent.inc
- scripts/include/utility.inc
- scripts/util/bank.zip
- scripts/util/buddies.inc
- scripts/util/conflicts.inc
- scripts/util/creature.inc
- scripts/util/homeTurf.inc
- scripts/util/key.zip
- scripts/util/lookForTrouble_AI.inc
- scripts/util/loot.inc
- scripts/util/potion_stuff.inc
- scripts/util/repair.inc
- scripts/util/spawn.inc
- scripts/vcomp120.dll
- scripts/www/webstuff.src
- scripts/www/www.rar

### Includes
- scripts/include/NameChecker.inc
- scripts/include/account.inc
- scripts/include/alchemy.inc
- scripts/include/all.inc
- scripts/include/anchors.inc
- scripts/include/animal.inc
- scripts/include/areas.inc
- scripts/include/arrays.inc
- scripts/include/astralfights.inc
- scripts/include/attributes.inc
- scripts/include/autoloop.inc
- scripts/include/bard.inc
- scripts/include/bitwise.inc
- scripts/include/buddies.inc
- scripts/include/camouflage.inc
- scripts/include/canAccess.inc
- scripts/include/cfglogging.inc
- scripts/include/change.inc
- scripts/include/chaoseffects.inc
- scripts/include/checkcity.inc
- scripts/include/chests.inc
- scripts/include/classes.inc
- scripts/include/client.inc
- scripts/include/clock.inc
- scripts/include/constants.inc
- scripts/include/constants/anims.inc
- scripts/include/constants/cfgfiles.inc
- scripts/include/constants/cids.inc
- scripts/include/constants/cmdlevels.inc
- scripts/include/constants/colors.inc
- scripts/include/constants/creaturetypes.inc
- scripts/include/constants/datastorefiles.inc
- scripts/include/constants/eventids.inc
- scripts/include/constants/facings.inc
- scripts/include/constants/fonts.inc
- scripts/include/constants/fxs.inc
- scripts/include/constants/gumpids.inc
- scripts/include/constants/hitscripts.inc
- scripts/include/constants/itemids.inc
- scripts/include/constants/landtiles.inc
- scripts/include/constants/layers.inc
- scripts/include/constants/locations.inc
- scripts/include/constants/merchanttypes.inc
- scripts/include/constants/mountids.inc
- scripts/include/constants/multiids.inc
- scripts/include/constants/npcai.inc
- scripts/include/constants/npcdata.inc
- scripts/include/constants/npctemplates.inc
- scripts/include/constants/objtypes.zip
- scripts/include/constants/onhitscripts.inc
- scripts/include/constants/privilegeids.inc
- scripts/include/constants/propids.inc
- scripts/include/constants/raceids.inc
- scripts/include/constants/resourceids.inc
- scripts/include/constants/scriptids.inc
- scripts/include/constants/settings.inc
- scripts/include/constants/sfxs.inc
- scripts/include/constants/skillids.inc
- scripts/include/constants/skillnames.inc
- scripts/include/constants/statids.inc
- scripts/include/constants/storageareas.zip
- scripts/include/constants/syseventids.inc
- scripts/include/constants/timeids.inc
- scripts/include/constants/usescriptids.inc
- scripts/include/coords.inc
- scripts/include/createnpc.inc
- scripts/include/creatureRoutines.inc
- scripts/include/creature_spellcast.inc
- scripts/include/damages.inc
- scripts/include/datafile.inc
- scripts/include/difficulty.inc
- scripts/include/dismount.inc
- scripts/include/doors.inc
- scripts/include/dotempmods.inc
- scripts/include/drinkpotion.inc
- scripts/include/eventid.inc
- scripts/include/events.inc
- scripts/include/facings.inc
- scripts/include/fields.inc
- scripts/include/findcity.inc
- scripts/include/fix.inc
- scripts/include/housecheck.inc
- scripts/include/housing.inc
- scripts/include/inncheck.inc
- scripts/include/irc.inc
- scripts/include/itemutil.inc
- scripts/include/jailcheck.inc
- scripts/include/listex.inc
- scripts/include/managers.inc
- scripts/include/math.inc
- scripts/include/mobileutil.inc
- scripts/include/moongate.inc
- scripts/include/moongates.inc
- scripts/include/mrcspawn.inc
- scripts/include/multicommands.inc
- scripts/include/multiutil.inc
- scripts/include/myutil.inc
- scripts/include/names.inc
- scripts/include/namingbyenchant.inc
- scripts/include/npcbackpacks.zip
- scripts/include/npcboosts.inc
- scripts/include/npccast.inc
- scripts/include/npccastspells.inc
- scripts/include/npcutil.inc
- scripts/include/objtype.inc
- scripts/include/packets.inc
- scripts/include/parsewords.inc
- scripts/include/possess.inc
- scripts/include/privs.inc
- scripts/include/randname.inc
- scripts/include/randname_util.inc
- scripts/include/random.inc
- scripts/include/randomreg.inc
- scripts/include/recalling.inc
- scripts/include/report.inc
- scripts/include/reportmurder.inc
- scripts/include/res.inc
- scripts/include/rituals.zip
- scripts/include/security.inc
- scripts/include/selchar.inc
- scripts/include/shard.inc
- scripts/include/skilllists.inc
- scripts/include/skillpoints.inc
- scripts/include/skilltitles.inc
- scripts/include/snoop.inc
- scripts/include/sounds.inc
- scripts/include/spawn.inc
- scripts/include/spawnnet.inc
- scripts/include/spawnpoint.inc
- scripts/include/speech.inc
- scripts/include/spelldata.inc
- scripts/include/starteqp.inc
- scripts/include/statmod.inc
- scripts/include/std.inc
- scripts/include/string.inc
- scripts/include/stringarrays.inc
- scripts/include/sysevent.inc
- scripts/include/teleporters.inc
- scripts/include/tempmod.inc
- scripts/include/time.inc
- scripts/include/uo_extend.inc
- scripts/include/utility.inc
- scripts/include/vetement.inc
- scripts/include/virtue.inc
- scripts/include/weather.inc

### Modules
- scripts/modules/attributes.em
- scripts/modules/basic.em
- scripts/modules/basicio.em
- scripts/modules/boat.em
- scripts/modules/cfgfile.em
- scripts/modules/cliloc.em
- scripts/modules/datafile.em
- scripts/modules/file.em
- scripts/modules/guilds.em
- scripts/modules/http.em
- scripts/modules/math.em
- scripts/modules/npc.em
- scripts/modules/os.em
- scripts/modules/party.em
- scripts/modules/polsys.em
- scripts/modules/sql.em
- scripts/modules/storage.em
- scripts/modules/unicode.em
- scripts/modules/uo.em
- scripts/modules/util.em
- scripts/modules/vitals.em
- scripts/playermanager.src
- scripts/poltool.pdb
- scripts/runecl.exe
- scripts/runecl.pdb
- scripts/start.src
- scripts/storagewipe.zip

## Packages

### Standard Packages (pkg/std)
- pkg/std/spells/dispel_field.src
- pkg/std/spells/earthquake.src
- pkg/std/spells/ebolt.src
- pkg/std/spells/energy_field.src
- pkg/std/spells/explosion.src
- pkg/std/spells/feeblemind.src
- pkg/std/spells/fireball.src
- pkg/std/spells/firefield.src
- pkg/std/spells/fstrike.src
- pkg/std/spells/gate.src
- pkg/std/spells/getspellid.inc
- pkg/std/spells/gheal.src
- pkg/std/spells/harm.src
- pkg/std/spells/heal.src
- pkg/std/spells/incognito.src
- pkg/std/spells/invisibility.src
- pkg/std/spells/itemdesc.cfg
- pkg/std/spells/lightning.src
- pkg/std/spells/magicarrow.src
- pkg/std/spells/magiclock.src
- pkg/std/spells/magictrap.src
- pkg/std/spells/magicuntrap.src
- pkg/std/spells/manadrain.src
- pkg/std/spells/manavamp.src
- pkg/std/spells/mark.src
- pkg/std/spells/masscurse.src
- pkg/std/spells/massdispel.src
- pkg/std/spells/meteor_swarm.src
- pkg/std/spells/mindblast.src
- pkg/std/spells/nightsight.src
- pkg/std/spells/parafield.src
- pkg/std/spells/paralyze.src
- pkg/std/spells/pkg.cfg
- pkg/std/spells/poison.src
- pkg/std/spells/poisonfield.src
- pkg/std/spells/protection with timer.src
- pkg/std/spells/protection.src
- pkg/std/spells/reactivearmor.src
- pkg/std/spells/recall.src
- pkg/std/spells/reflect.src
- pkg/std/spells/resurrect.src
- pkg/std/spells/reveal.src
- pkg/std/spells/scroll.src
- pkg/std/spells/spells.cfg
- pkg/std/spells/strength.src
- pkg/std/spells/summon_air.src
- pkg/std/spells/summon_creature.src
- pkg/std/spells/summon_daemon.src
- pkg/std/spells/summon_earth.src
- pkg/std/spells/summon_fire.src
- pkg/std/spells/summon_water.src
- pkg/std/spells/telekinesis.src
- pkg/std/spells/teleport.src
- pkg/std/spells/thaw.src
- pkg/std/spells/unlock.src
- pkg/std/spells/vortex.src
- pkg/std/spells/wallofstone.src
- pkg/std/spells/weaken.src
- pkg/std/spiritspeak/pkg.cfg
- pkg/std/spiritspeak/spiritspeak.src
- pkg/std/stealing/itemdesc.cfg
- pkg/std/stealing/pkg.cfg
- pkg/std/stealing/prestealing.src
- pkg/std/stealing/stealing.src
- pkg/std/stealth/pkg.cfg
- pkg/std/stealth/stealth.src
- pkg/std/tailoring/bridle.src
- pkg/std/tailoring/itemdesc.cfg
- pkg/std/tailoring/make_cloth_items.src
- pkg/std/tailoring/pkg.cfg
- pkg/std/tailoring/scissors.src
- pkg/std/tailoring/tailoring.cfg
- pkg/std/tailoring/tailoring.src
- pkg/std/tailoring/unstitch.src
- pkg/std/taming/pkg.cfg
- pkg/std/taming/taming.src
- pkg/std/tasteid/pkg.cfg
- pkg/std/tasteid/tasteid.src
- pkg/std/taunt/enticeai.src
- pkg/std/taunt/pkg.cfg
- pkg/std/taunt/taunt.src
- pkg/std/tinkering/candlemaking.src
- pkg/std/tinkering/itemdesc.cfg
- pkg/std/tinkering/pkg.cfg
- pkg/std/tinkering/tarot.src
- pkg/std/tinkering/tinker.cfg
- pkg/std/tinkering/tinkering.src
- pkg/std/tracking/pkg.cfg
- pkg/std/tracking/tracking.cfg
- pkg/std/tracking/tracking.src
- pkg/std/training/archery_butte.src
- pkg/std/training/dummy.src
- pkg/std/training/dummy_pickpocket.src
- pkg/std/training/itemdesc.cfg
- pkg/std/training/pkg.cfg
- pkg/std/traps/hiddentrap.src
- pkg/std/traps/itemdesc.cfg
- pkg/std/traps/pkg.cfg
- pkg/std/traps/traps.src
- pkg/std/treasuremap/decodemap.src
- pkg/std/treasuremap/digtreasure.src
- pkg/std/treasuremap/guardians.cfg
- pkg/std/treasuremap/itemdesc.cfg
- pkg/std/treasuremap/pkg.cfg
- pkg/std/treasuremap/treasure.cfg
- pkg/std/veterinary/pkg.cfg
- pkg/std/veterinary/vet.src

### System Packages (pkg/systems)
- pkg/systems/accounts/acctWatcher/acctWatcher.src
- pkg/systems/accounts/commands/dev/eraseEmptyAccounts.src
- pkg/systems/accounts/config/icp.cfg
- pkg/systems/accounts/config/settings.cfg
- pkg/systems/accounts/config/uopacket.cfg
- pkg/systems/accounts/hook/onLogin.src
- pkg/systems/accounts/include/accounts.inc
- pkg/systems/accounts/include/mailSystem.inc
- pkg/systems/accounts/include/settings.inc
- pkg/systems/accounts/logon.src
- pkg/systems/accounts/pkg.cfg
- pkg/systems/accounts/reconnect.src
- pkg/systems/combat/avengingonhit.src
- pkg/systems/combat/balanceddagger.src
- pkg/systems/combat/banishonhit.src
- pkg/systems/combat/banishscript.src
- pkg/systems/combat/blackrockscript.src
- pkg/systems/combat/blindingonhit.src
- pkg/systems/combat/blindingscript.src
- pkg/systems/combat/bouncingonhit.src
- pkg/systems/combat/config/Stuff to add.txt
- pkg/systems/combat/config/enchantableitems.cfg
- pkg/systems/combat/config/hitscriptdesc.cfg
- pkg/systems/combat/config/itemdesc.cfg
- pkg/systems/combat/config/modenchantdesc.cfg
- pkg/systems/combat/config/onhitscriptdesc.cfg
- pkg/systems/combat/config/settings.cfg
- pkg/systems/combat/crithit.src
- pkg/systems/combat/deflectiononhit.src
- pkg/systems/combat/dualplanaronhit.src
- pkg/systems/combat/dualplanarscript.src
- pkg/systems/combat/explosionlauncherscript.src
- pkg/systems/combat/guardhitscript.src
- pkg/systems/combat/herd.src
- pkg/systems/combat/include/hitscriptinc.inc
- pkg/systems/combat/invisibleonhit.src
- pkg/systems/combat/lifedrainscript.src
- pkg/systems/combat/mainhit.src
- pkg/systems/combat/manadrainonhit.src
- pkg/systems/combat/manadrainscript.src
- pkg/systems/combat/paralyzehit.src
- pkg/systems/combat/paralyzeonhit.src
- pkg/systems/combat/piercingonhit.src
- pkg/systems/combat/piercingscript.src
- pkg/systems/combat/pkg.cfg
- pkg/systems/combat/poisonhit.src
- pkg/systems/combat/poisononhit.src
- pkg/systems/combat/raceresistonhit.src
- pkg/systems/combat/raxweapon.src
- pkg/systems/combat/reactivearmoronhit.src
- pkg/systems/combat/slayerscript.src
- pkg/systems/combat/sparhit.src
- pkg/systems/combat/spellonhit.src
- pkg/systems/combat/spellstrikescript.src
- pkg/systems/combat/staminadrainonhit.src
- pkg/systems/combat/staminadrainscript.src
- pkg/systems/combat/thiefpoisonhit.src
- pkg/systems/combat/trielementalonhit.src
- pkg/systems/combat/trielementalscript.src
- pkg/systems/combat/voidscript.src
- pkg/systems/crafting/include/craftingfunctions.inc
- pkg/systems/crafting/pkg.cfg
- pkg/systems/email/chardelete.src
- pkg/systems/email/commands/gm/inspectmail.src
- pkg/systems/email/commands/player/email.src
- pkg/systems/email/config/icp.cfg
- pkg/systems/email/email.src
- pkg/systems/email/emailMessage/newEmail.src
- pkg/systems/email/include/email.inc
- pkg/systems/email/include/email_const.inc
- pkg/systems/email/logon.src
- pkg/systems/email/pkg.cfg
- pkg/systems/email/reconnect.src
- pkg/systems/email/webmail/webmail.src
- pkg/systems/onlineCount/cheatCount.src
- pkg/systems/onlineCount/commands/dev/connectBot.src
- pkg/systems/onlineCount/commands/dev/disconnectBot.src
- pkg/systems/onlineCount/commands/dev/restartCheatCount.src
- pkg/systems/onlineCount/config/icp.cfg
- pkg/systems/onlineCount/config/settings.cfg
- pkg/systems/onlineCount/include/bots.inc
- pkg/systems/onlineCount/include/settings.inc
- pkg/systems/onlineCount/pkg.cfg
- pkg/systems/onlineCount/start.src
- pkg/systems/reputation/config/icp.cfg
- pkg/systems/reputation/hook/namedyes.src
- pkg/systems/reputation/pkg.cfg

### Utility Packages (pkg/utils)
- pkg/utils/clilocs/commands/test/clilocTest.src
- pkg/utils/clilocs/config/backup_of_clilocs.cfg
- pkg/utils/clilocs/config/clilocs.cfg
- pkg/utils/clilocs/config/icp.cfg
- pkg/utils/clilocs/include/clilocs.inc
- pkg/utils/clilocs/pkg.cfg
- pkg/utils/datafile/config/icp.cfg
- pkg/utils/datafile/include/datafile.inc
- pkg/utils/datafile/include/datafile_ex.inc
- pkg/utils/datafile/pkg.cfg
- pkg/utils/gumps/changelog.txt
- pkg/utils/gumps/commands/admin/gumpprompt.src
- pkg/utils/gumps/commands/admin/htmlgump.src
- pkg/utils/gumps/commands/admin/requestgump.src
- pkg/utils/gumps/commands/admin/resizepic.src
- pkg/utils/gumps/commands/admin/samplegump.src
- pkg/utils/gumps/commands/admin/selectiongump.src
- pkg/utils/gumps/commands/admin/yesno.src
- pkg/utils/gumps/config/GumpInfo - Copy.cfg
- pkg/utils/gumps/config/GumpInfo.cfg
- pkg/utils/gumps/config/fontSize.cfg
- pkg/utils/gumps/config/icp.cfg
- pkg/utils/gumps/include/autoClose.inc
- pkg/utils/gumps/include/gumpprompt.inc
- pkg/utils/gumps/include/gumps.inc
- pkg/utils/gumps/include/gumps_ex.inc
- pkg/utils/gumps/include/htmlgump.inc
- pkg/utils/gumps/include/old-gumps.inc
- pkg/utils/gumps/include/old/old-gumps.inc
- pkg/utils/gumps/include/playerselectiongump.inc
- pkg/utils/gumps/include/requestgump.inc
- pkg/utils/gumps/include/selectiongump.inc
- pkg/utils/gumps/include/textConsts.inc
- pkg/utils/gumps/include/yesNoSizable.inc
- pkg/utils/gumps/include/yesno.inc
- pkg/utils/gumps/pkg.cfg
- pkg/utils/gumps/scripts/autoClose/autoClose.src
- pkg/utils/gumps/scripts/autoClose/autoCloseOnLeaveArea.src
- pkg/utils/gumps/scripts/autoClose/autoCloseOnMovedCoordinateDistance.src
- pkg/utils/gumps/scripts/autoClose/autoCloseOnMovedDistance.src
- pkg/utils/gumps/scripts/yesNo/yesNoGump.src
- pkg/utils/gumps/scripts/yesNo/yesNoMiniGump.src
- pkg/utils/itemUtils/config/icp.cfg
- pkg/utils/itemUtils/config/offset.cfg
- pkg/utils/itemUtils/config/sets.cfg
- pkg/utils/itemUtils/config/stairs.cfg
- pkg/utils/itemUtils/include/canAccess.inc
- pkg/utils/itemUtils/include/colors.inc
- pkg/utils/itemUtils/include/desc.inc
- pkg/utils/itemUtils/include/itemInfo.inc
- pkg/utils/itemUtils/include/itemProps.inc
- pkg/utils/itemUtils/include/itemUtil.inc
- pkg/utils/itemUtils/include/itemUtil_ex.inc
- pkg/utils/itemUtils/include/itemdesc.inc
- pkg/utils/itemUtils/include/itemtypes.inc
- pkg/utils/itemUtils/include/layers.inc
- pkg/utils/itemUtils/include/offsets.inc
- pkg/utils/itemUtils/include/toolWear.inc
- pkg/utils/itemUtils/pkg.cfg
- pkg/utils/itemUtils/rotate/rotate.src
- pkg/utils/mdgumps/changelog.txt
- pkg/utils/mdgumps/commands/test/gfchart.src
- pkg/utils/mdgumps/commands/test/gumpprompt.src
- pkg/utils/mdgumps/commands/test/htmlgump.src
- pkg/utils/mdgumps/commands/test/requestgump.src
- pkg/utils/mdgumps/commands/test/resizepic.src
- pkg/utils/mdgumps/commands/test/samplegump.src
- pkg/utils/mdgumps/commands/test/selectiongump.src
- pkg/utils/mdgumps/commands/test/yesno.src
- pkg/utils/mdgumps/config/GumpInfo.cfg
- pkg/utils/mdgumps/config/fontSize.cfg
- pkg/utils/mdgumps/config/icp.cfg
- pkg/utils/mdgumps/include/autoClose.inc
- pkg/utils/mdgumps/include/confirmationSizable.inc
- pkg/utils/mdgumps/include/gumpCaching.inc
- pkg/utils/mdgumps/include/gumpPrompt.inc
- pkg/utils/mdgumps/include/gumps.inc
- pkg/utils/mdgumps/include/gumps_ex.inc
- pkg/utils/mdgumps/include/htmlGump.inc
- pkg/utils/mdgumps/include/old-gumps.inc
- pkg/utils/mdgumps/include/old/old-gumps.inc
- pkg/utils/mdgumps/include/playerSelectionGump.inc
- pkg/utils/mdgumps/include/requestGump.inc
- pkg/utils/mdgumps/include/selectionGump.inc
- pkg/utils/mdgumps/include/textConsts.inc
- pkg/utils/mdgumps/include/yesNo.inc
- pkg/utils/mdgumps/pkg.cfg
- pkg/utils/mdgumps/scripts/autoClose/autoClose.src
- pkg/utils/mdgumps/scripts/autoClose/autoCloseOnLeaveArea.src
- pkg/utils/mdgumps/scripts/autoClose/autoCloseOnMovedCoordinateDistance.src
- pkg/utils/mdgumps/scripts/autoClose/autoCloseOnMovedDistance.src
- pkg/utils/mdgumps/scripts/yesNo/binaryChoice.src
- pkg/utils/mdgumps/scripts/yesNo/yesNoGump.src
- pkg/utils/mdgumps/scripts/yesNo/yesNoMiniGump.src
- pkg/utils/objClassMethods/config/icp.cfg
- pkg/utils/objClassMethods/config/syshook.cfg
- pkg/utils/objClassMethods/pkg.cfg
- pkg/utils/objClassMethods/scripts/character.src
- pkg/utils/security/config/icp.cfg
- pkg/utils/security/include/attributesReport.inc
- pkg/utils/security/include/commandReport.inc
- pkg/utils/security/include/damageReport.inc
- pkg/utils/security/include/itemReport.inc
- pkg/utils/security/include/report.inc
- pkg/utils/security/include/speechReport.inc
- pkg/utils/security/pkg.cfg
- pkg/utils/timeutils/commands/player/serverTime.src
- pkg/utils/timeutils/commands/test/timetest.src
- pkg/utils/timeutils/config/icp.cfg
- pkg/utils/timeutils/config/settings.cfg
- pkg/utils/timeutils/include/gameTime.inc
- pkg/utils/timeutils/include/settings.inc
- pkg/utils/timeutils/include/time.inc
- pkg/utils/timeutils/pkg.cfg

### Template Packages
- pkg/template/pkg.cfg

---

## 3. Optional Gameplay Packages

**Summary:**
Introduced a large set of optional gameplay systems and content packs, including housing, rituals, books, songs, moongates, spawnpoints, farming, guilds, vanity, seasonal content, and shard-specific gameplay hooks.

**Details:**
- Added optional content modules that can be enabled per shard ruleset.
- Expanded roleplay, seasonal, and event-driven systems.
- Added support packages for moongates, books, rituals, farming, and custom item behaviors.

**Key Files Changed:**
- pkg/opt/ArtifactSystem/*
- pkg/opt/astralfights/*
- pkg/opt/areas/*
- pkg/opt/alchemyplus/*
- pkg/opt/botanik/*
- pkg/opt/champspawns/*
- pkg/opt/christmas/*
- pkg/opt/colorwars/*
- pkg/opt/crafterboost/*
- pkg/opt/decoratefacets/*
- pkg/opt/Donator/*
- pkg/opt/dyteitems/*
- pkg/opt/earth/*
- pkg/opt/Events/*
- pkg/opt/farming/*
- pkg/opt/guilds/*
- pkg/opt/holybook/*
- pkg/opt/karmafame/*
- pkg/opt/lighting/*
- pkg/opt/loot/*
- pkg/opt/lootlottery/*
- pkg/opt/MagicWands/*
- pkg/opt/msg/*
- pkg/opt/moongates/*
- pkg/opt/moons/*
- pkg/opt/necro/*
- pkg/opt/pillar/*
- pkg/opt/powerhour/*
- pkg/opt/powerscrolls/*
- pkg/opt/randomero/*
- pkg/opt/rituals/*
- pkg/opt/roleplaying/*
- pkg/opt/shilhook/*
- pkg/opt/shilitems/*
- pkg/opt/songbook/*
- pkg/opt/spawnpoint/*
- pkg/opt/Staff/*
- pkg/opt/summoning/*
- pkg/opt/sunshine/*
- pkg/opt/timer/*
- pkg/opt/townstones/*
- pkg/opt/vanityshop/*
- pkg/opt/versebook/*
- pkg/opt/zulugames/*
- pkg/opt/zuluitems/*

---

## 4. Item Packages

**Summary:**
Expanded the item package set with containers, deeds, doors, currency, house extras, armor support, and a large catalog of placeable or scripted world items.

**Details:**
- Added scripted item behaviors for world objects, furnishings, and deployable items.
- Expanded deed-backed placement and house decoration content.
- Added container, currency, and key-related item logic.

**Key Files Changed:**
- pkg/items/abbatoir/*
- pkg/items/ankh/*
- pkg/items/anvil/*
- pkg/items/arcaneCircle/*
- pkg/items/armor/*
- pkg/items/beds/*
- pkg/items/bloodPentagram/*
- pkg/items/carpets/*
- pkg/items/containers/*
- pkg/items/crystalThemePack/*
- pkg/items/currency/*
- pkg/items/curtains/*
- pkg/items/deed/*
- pkg/items/doors/*
- pkg/items/elvenFurniture/*
- pkg/items/houseExtras/*
- pkg/items/keys/*
- pkg/items/pentagram/*
- pkg/items/sysbook/*

---

## 5. Standard Skill Packages

**Summary:**
Added and updated the standard skill and gameplay packages, including spells, crafting skills, gathering, stealth, combat support, training content, and skill-specific item definitions.

**Details:**
- Implemented the standard spellbook and supporting spell scripts.
- Added skill packages for gathering, combat utility, crafting, and stealth.
- Expanded training, treasure map, trap, and veterinary gameplay support.

**Key Files Changed:**
- pkg/std/alchemy/*
- pkg/std/animallore/*
- pkg/std/anatomy/*
- pkg/std/armslore/*
- pkg/std/begging/*
- pkg/std/blacksmithy/*
- pkg/std/boat.zip
- pkg/std/camping/*
- pkg/std/carpentry/*
- pkg/std/cartography/*
- pkg/std/cooking/*
- pkg/std/daynight/*
- pkg/std/detecthidden/*
- pkg/std/doors.zip
- pkg/std/dundee/*
- pkg/std/evalint/*
- pkg/std/fishing/*
- pkg/std/forensicevaluation/*
- pkg/std/healing/*
- pkg/std/hiding/*
- pkg/std/housing.zip
- pkg/std/inscription/*
- pkg/std/itemid/*
- pkg/std/lockpicking/*
- pkg/std/lumberjacking/*
- pkg/std/meditation/*
- pkg/std/mining/*
- pkg/std/musicianship/*
- pkg/std/peacemaking/*
- pkg/std/poisoning/*
- pkg/std/provocation/*
- pkg/std/removetrap/*
- pkg/std/runebook/*
- pkg/std/saver/*
- pkg/std/snooping/*
- pkg/std/stealing/*
- pkg/std/stealth/*
- pkg/std/spells/*
- pkg/std/spiritspeak/*
- pkg/std/tailoring/*
- pkg/std/taming/*
- pkg/std/tasteid/*
- pkg/std/taunt/*
- pkg/std/tracking/*
- pkg/std/traps/*
- pkg/std/training/*
- pkg/std/treasuremap/*
- pkg/std/veterinary/*

---

## 6. Utility Packages

**Summary:**
Updated the shared utility packages that support UI, data handling, item utilities, time helpers, security reports, and gump presentation logic.

**Details:**
- Added reusable gump and dialog helpers for shared UI flows.
- Expanded item inspection, class-method, and security reporting utilities.
- Added time and cliloc helpers used by higher-level gameplay systems.

**Key Files Changed:**
- pkg/utils/clilocs/*
- pkg/utils/datafile/*
- pkg/utils/gumps/*
- pkg/utils/itemUtils/*
- pkg/utils/mdgumps/*
- pkg/utils/objClassMethods/*
- pkg/utils/security/*
- pkg/utils/timeutils/*

---

## 7. System Packages

**Summary:**
Added and expanded system-level packages for accounts, combat, email, online-count tracking, and reputation hooks.

**Details:**
- Added account lifecycle, reconnect, and login hook support.
- Added email delivery, mail UI, and webmail system hooks.
- Added online count, cheat count, and reputation integration systems.

**Key Files Changed:**
- pkg/systems/accounts/*
- pkg/systems/combat/*
- pkg/systems/crafting/*
- pkg/systems/email/*
- pkg/systems/onlineCount/*
- pkg/systems/reputation/*

---

## 8. AI & NPC Behavior

**Summary:**
Refined the AI package tree for monsters, vendors, guards, townsfolk, special encounters, and combat-driven NPC behaviors.

**Details:**
- Added AI scripts for combat, merchant, town, and encounter roles.
- Expanded setup and behavior variants for specialized NPC types.
- Added creature-specific scripts for unique encounters and guard logic.

**Key Files Changed:**
- scripts/ai/combat/*
- scripts/ai/main/*
- scripts/ai/setup/*
- scripts/ai/*.src
- scripts/ai/*.zip
- pkg/mobiles/*

---

## 9. Shared Scripts & Includes

**Summary:**
Expanded the shared script layer that supports core runtime behavior, item interactions, creature logic, utility helpers, modules, and general include files used across the shard.

**Details:**
- Added core include files for rules, math, events, and utility helpers.
- Expanded item, mobile, spell, spawn, and creature support routines.
- Added module bindings and executable support scripts used by the runtime.

**Key Files Changed:**
- scripts/include/*
- scripts/modules/*
- scripts/items/*
- scripts/misc/*
- scripts/util/*
- scripts/console/*
- scripts/www/*

---

## 10. Command Trees

**Summary:**
Implemented and updated the command tree for players, GMs, seers, counselors, admins, and test/debug workflows.

**Details:**
- Added player-facing commands for common interactions and utilities.
- Expanded GM and seer command coverage for debugging and world management.
- Added test commands for packet, script, bank, and system validation.

**Key Files Changed:**
- scripts/textcmd/admin/*
- scripts/textcmd/coun/*
- scripts/textcmd/gm/*
- scripts/textcmd/player/*
- scripts/textcmd/seer/*
- scripts/textcmd/test/*
- pkg/commands/*

---

## 11. World Data & Regions

**Summary:**
Updated region data, map-generation support files, multis, and packet-hook related assets that drive world layout and placement behavior.

**Details:**
- Added regional resource and terrain definitions.
- Updated mapgen outputs for Trammel and related placement passes.
- Added multis and packet hook assets used by world layout systems.

**Key Files Changed:**
- mapgen/trammel/*
- pkg/multis/*
- pkg/packethooks/*
- regions/*

---

## 12. Root Config, Builds, and Tooling

**Summary:**
Captured the repository-level configuration, build artifacts, conversion tools, backups, documentation, and generated binaries that were added or refreshed during the initial project import.

**Details:**
- Added root configuration files and import-time documentation.
- Included generated binaries, debug artifacts, and tooling outputs.
- Added backup, conversion, and changelog support files used during setup.

**Key Files Changed:**
- .gitignore
- .vscode/settings.json
- ConfigPath.zip
- Converted_Used_Colors.txt
- POLConfigurator.dat
- README.md
- README.txt
- Used Colors.txt
- ZHO-DataBackup.ps1
- breaking-changes.txt
- config/*
- core-changes.txt
- libmysql.dll
- loogroups.txt
- orphans.txt
- packets.zip
- pol.cfg
- pol.cfg.example
- pol.exe
- pol.map
- pol.obj
- pol.pch
- pol.pdb
- pol.res
- poltool.exe
- poltool.pdb
- uoconvert.cfg
- uoconvert.exe
- uoconvert.pdb
- uoconvert.txt
- uotool.exe
- uotool.pdb
- vc120.pdb
- vcomp120.dll
- zho_changelog.txt
