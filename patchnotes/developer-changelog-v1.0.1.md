# Developer Changelog - v1.0.1

Range: a1ec62b..2059290
Branch: Patch-1.0.1
Date: 2026-06-03

---

## Scope Summary

- Files changed: 326
- 326 files changed, 19025 insertions(+), 6238 deletions(-)

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - |
|---|---|---:|---:|
| M | config/cmds.cfg | 22 | 16 |
| A | config/command_synopses.cfg | 1843 | 0 |
| A | config/golocs_by_id.cfg | 2077 | 0 |
| M | config/mrcspawn.cfg | 118 | 25 |
| M | config/npcdesc.cfg | 141 | 13 |
| A | patchnotes/command_synopses.ZH2_5.baseline.cfg | 9 | 0 |
| A | patchnotes/command_synopses.ZH3_0.generated.cfg | 1843 | 0 |
| A | patchnotes/developer-changelog-v1.0.0.md | 1010 | 0 |
| A | patchnotes/patch-v1.0.0.md | 81 | 0 |
| M | pkg/items/armor/include/armorZones.inc | 48 | 6 |
| M | pkg/items/keys/textcmd/gm/fixkeyring.src | 1 | 0 |
| M | pkg/items/keys/textcmd/gm/showKeyRing.src | 1 | 0 |
| M | pkg/opt/Donator/textcmd/test/createdonatorbear.src | 1 | 0 |
| M | pkg/opt/Donator/textcmd/test/createdonatorhorse.src | 1 | 0 |
| M | pkg/opt/Donator/textcmd/test/createdonatorllama.src | 1 | 0 |
| M | pkg/opt/Donator/textcmd/test/createdonatorostard.src | 1 | 0 |
| M | pkg/opt/Donator/textcmd/test/makedonator.src | 1 | 0 |
| M | pkg/opt/Events/textcmd/seer/createEventBag.src | 2 | 1 |
| M | pkg/opt/areas/areas.cfg | 232 | 102 |
| A | pkg/opt/areas/include/areapolicy.inc | 559 | 0 |
| M | pkg/opt/areas/textcmd/admin/areas.src | 397 | 139 |
| M | pkg/opt/champspawns/textcmd/test/createchampionspawn.src | 1 | 0 |
| D | pkg/opt/colorwars/commands/admin/cwstone.src | 0 | 22 |
| D | pkg/opt/colorwars/commands/gm/cleancw.src | 0 | 76 |
| D | pkg/opt/colorwars/commands/player/cwready.src | 0 | 46 |
| D | pkg/opt/colorwars/cwars.src | 0 | 1523 |
| D | pkg/opt/colorwars/cwprize.src | 0 | 42 |
| D | pkg/opt/colorwars/itemdesc.cfg | 0 | 54 |
| D | pkg/opt/colorwars/pkg.cfg | 0 | 4 |
| A | pkg/opt/guilds/commands/test/changeguildownership.src | 102 | 0 |
| M | pkg/opt/ipban/textcmd/admin/ipban.src | 1 | 0 |
| M | pkg/opt/karmafame/textcmd/admin/setkf.src | 1 | 0 |
| M | pkg/opt/karmafame/textcmd/player/karma.src | 1 | 0 |
| M | pkg/opt/karmafame/textcmd/test/resetkf.src | 1 | 0 |
| M | pkg/opt/karmafame/textcmd/test/updatekf.src | 1 | 0 |
| M | pkg/opt/powerhour/textcmd/player/ph.src | 2 | 1 |
| M | pkg/opt/powerhour/textcmd/player/setph.src | 1 | 0 |
| M | pkg/opt/powerhour/textcmd/test/resetph.src | 1 | 0 |
| M | pkg/opt/powerscrolls/textcmd/admin/raisecaps.src | 2 | 1 |
| M | pkg/opt/powerscrolls/textcmd/player/showcaps.src | 2 | 1 |
| A | pkg/opt/powerscrolls/textcmd/test/lowerallchosencaps.src | 131 | 0 |
| A | pkg/opt/powerscrolls/textcmd/test/lowerallchosenstatcaps.src | 105 | 0 |
| A | pkg/opt/powerscrolls/textcmd/test/lowercaps.src | 102 | 0 |
| A | pkg/opt/powerscrolls/textcmd/test/raiseallchosencaps.src | 131 | 0 |
| A | pkg/opt/powerscrolls/textcmd/test/raiseallchosenstatcaps.src | 105 | 0 |
| M | pkg/opt/randomero/textcmd/test/testinternal.src | 2 | 1 |
| M | pkg/opt/randomero/textcmd/test/testnormal.src | 2 | 1 |
| M | pkg/opt/randomero/textcmd/test/testprng.src | 2 | 1 |
| M | pkg/opt/roleplaying/textcmd/admin/fixstartgear.src | 2 | 1 |
| M | pkg/opt/roleplaying/textcmd/coun/macrotest.src | 2 | 1 |
| M | pkg/opt/roleplaying/textcmd/player/rc.src | 1 | 0 |
| M | pkg/opt/roleplaying/textcmd/seer/makerpergate.src | 1 | 0 |
| M | pkg/opt/roleplaying/textcmd/seer/rperoff.src | 2 | 1 |
| M | pkg/opt/roleplaying/textcmd/seer/rperon.src | 2 | 1 |
| M | pkg/opt/roleplaying/textcmd/seer/setrper.src | 2 | 1 |
| M | pkg/opt/shilhook/textcmd/admin/setglobalmultipliers.src | 2 | 1 |
| M | pkg/opt/shilhook/textcmd/admin/setplayermultipliers.src | 2 | 1 |
| M | pkg/opt/shrink/textcmd/test/shrink.src | 2 | 1 |
| M | pkg/opt/spawnpoint/textcmd/admin/createspawntrigger.src | 1 | 0 |
| M | pkg/opt/spawnpoint/textcmd/admin/despawn.src | 1 | 0 |
| M | pkg/opt/spawnpoint/textcmd/admin/forcespawn.src | 1 | 0 |
| A | pkg/opt/spawnpoint/textcmd/admin/forcespawnarea.src | 93 | 0 |
| M | pkg/opt/spawnpoint/textcmd/admin/gotomobtype.src | 1 | 0 |
| A | pkg/opt/spawnpoint/textcmd/admin/gotonearestspawnpoint.src | 47 | 0 |
| M | pkg/opt/spawnpoint/textcmd/admin/gotospawnpoint.src | 2 | 1 |
| M | pkg/opt/spawnpoint/textcmd/admin/newmobedit.src | 2 | 1 |
| M | pkg/opt/spawnpoint/textcmd/admin/primespawn.src | 1 | 0 |
| M | pkg/opt/townstones/textcmd/admin/createstone.src | 1 | 0 |
| M | pkg/opt/townstones/textcmd/admin/fixstone.src | 1 | 0 |
| M | pkg/opt/zuluitems/itemdesc.cfg | 461 | 10 |
| D | pkg/opt/zuluitems/vendordeed.src | 0 | 63 |
| M | pkg/packethooks/megacliloc/itemdata.src | 30 | 14 |
| M | pkg/packethooks/megacliloc/mobiledata.src | 175 | 69 |
| M | pkg/packethooks/versionHook/config/clients.cfg | 1 | 1 |
| A | pkg/systems/playervendor/commands/admin/playermerchantstatus.src | 95 | 0 |
| A | pkg/systems/playervendor/commands/player/escrow.src | 430 | 0 |
| A | pkg/systems/playervendor/commands/test/migratevendorstorage.src | 165 | 0 |
| A | pkg/systems/playervendor/commands/test/pmescrowtest.src | 346 | 0 |
| A | pkg/systems/playervendor/commands/test/pmfireme.src | 66 | 0 |
| A | pkg/systems/playervendor/include/escrow.inc | 40 | 0 |
| A | pkg/systems/playervendor/itemdesc.cfg | 29 | 0 |
| A | pkg/systems/playervendor/pkg.cfg | 5 | 0 |
| A | pkg/systems/playervendor/playermerchant.src | 1732 | 0 |
| A | pkg/systems/playervendor/vendordeed.src | 80 | 0 |
| A | pythonscripts/_gen_command_synopses_cfg.py | 96 | 0 |
| A | pythonscripts/generate_golocs_by_id.py | 135 | 0 |
| M | regions/regions.cfg | 3101 | 791 |
| M | scripts/ai/merchant.src | 43 | 37 |
| M | scripts/ai/minstrel.src | 53 | 170 |
| M | scripts/ai/noble.src | 54 | 132 |
| M | scripts/ai/performer.src | 35 | 101 |
| M | scripts/ai/person.src | 106 | 180 |
| D | scripts/ai/playermerchant.src | 0 | 1148 |
| M | scripts/ai/townperson.src | 42 | 112 |
| M | scripts/include/anchors.inc | 175 | 0 |
| M | scripts/include/areas.inc | 35 | 410 |
| M | scripts/include/constants/locations.inc | 11 | 9 |
| M | scripts/include/randname.inc | 6 | 1 |
| A | scripts/include/townsfolk.inc | 161 | 0 |
| M | scripts/misc/death.src | 2 | 2 |
| M | scripts/misc/dressme.src | 51 | 38 |
| M | scripts/textcmd/admin/admin.src | 1 | 0 |
| M | scripts/textcmd/admin/akill.src | 36 | 0 |
| M | scripts/textcmd/admin/buzz.src | 1 | 0 |
| M | scripts/textcmd/admin/changecolor.src | 1 | 0 |
| M | scripts/textcmd/admin/changegraphic.src | 1 | 0 |
| M | scripts/textcmd/admin/checkspeed.src | 1 | 0 |
| M | scripts/textcmd/admin/class.src | 2 | 1 |
| M | scripts/textcmd/admin/colorrect.src | 2 | 1 |
| M | scripts/textcmd/admin/concealhim.src | 1 | 0 |
| M | scripts/textcmd/admin/concealmobs.src | 1 | 0 |
| M | scripts/textcmd/admin/createdungteles.src | 2 | 1 |
| M | scripts/textcmd/admin/createspawnpoint.src | 1 | 0 |
| M | scripts/textcmd/admin/deathgate.src | 1 | 0 |
| M | scripts/textcmd/admin/destroymulti.src | 1 | 0 |
| M | scripts/textcmd/admin/destroyradius.src | 1 | 0 |
| M | scripts/textcmd/admin/dress.src | 1 | 0 |
| M | scripts/textcmd/admin/dupe.src | 1 | 0 |
| M | scripts/textcmd/admin/dyebeard.src | 1 | 0 |
| M | scripts/textcmd/admin/dyehair.src | 1 | 0 |
| M | scripts/textcmd/admin/dyerect.src | 2 | 1 |
| M | scripts/textcmd/admin/equip.src | 1 | 0 |
| M | scripts/textcmd/admin/eraseglobalprop.src | 2 | 1 |
| M | scripts/textcmd/admin/eraseobjproperty.src | 2 | 1 |
| M | scripts/textcmd/admin/gbs.src | 1 | 0 |
| M | scripts/textcmd/admin/gcmds.src | 2 | 1 |
| M | scripts/textcmd/admin/getglobal.src | 1 | 0 |
| M | scripts/textcmd/admin/globalnoloot.src | 2 | 1 |
| M | scripts/textcmd/admin/goxyz.src | 1 | 0 |
| M | scripts/textcmd/admin/hidemobs.src | 1 | 0 |
| M | scripts/textcmd/admin/identify.src | 2 | 1 |
| M | scripts/textcmd/admin/ip.src | 1 | 0 |
| M | scripts/textcmd/admin/iteminfo.src | 1 | 0 |
| M | scripts/textcmd/admin/listen.src | 1 | 0 |
| M | scripts/textcmd/admin/lock.src | 2 | 1 |
| M | scripts/textcmd/admin/makemoongates.src | 1 | 0 |
| M | scripts/textcmd/admin/makeregs.src | 1 | 0 |
| M | scripts/textcmd/admin/maxcaps.src | 1 | 0 |
| M | scripts/textcmd/admin/mazegate.src | 2 | 1 |
| M | scripts/textcmd/admin/mkaccount.src | 1 | 0 |
| M | scripts/textcmd/admin/music.src | 1 | 0 |
| M | scripts/textcmd/admin/removechristmas.src | 1 | 0 |
| M | scripts/textcmd/admin/removerper.src | 1 | 0 |
| M | scripts/textcmd/admin/resetpw.src | 1 | 0 |
| M | scripts/textcmd/admin/restart.src | 2 | 1 |
| M | scripts/textcmd/admin/savenow.src | 1 | 0 |
| M | scripts/textcmd/admin/setallskills.src | 1 | 0 |
| M | scripts/textcmd/admin/setclass.src | 2 | 1 |
| M | scripts/textcmd/admin/setname.src | 1 | 0 |
| M | scripts/textcmd/admin/setupchristmas.src | 1 | 0 |
| M | scripts/textcmd/admin/setupsanta.src | 1 | 0 |
| M | scripts/textcmd/admin/sfx.src | 1 | 0 |
| M | scripts/textcmd/admin/spellbook.src | 2 | 1 |
| M | scripts/textcmd/admin/status.src | 1 | 0 |
| M | scripts/textcmd/admin/summon.src | 1 | 0 |
| M | scripts/textcmd/admin/tile.src | 1 | 0 |
| M | scripts/textcmd/admin/unconcealmobs.src | 1 | 0 |
| M | scripts/textcmd/admin/unhidemobs.src | 1 | 0 |
| M | scripts/textcmd/admin/unlock.src | 1 | 0 |
| M | scripts/textcmd/admin/untile.src | 1 | 0 |
| M | scripts/textcmd/admin/zulushutdown.src | 1 | 0 |
| M | scripts/textcmd/coun/chattimeout.src | 2 | 1 |
| D | scripts/textcmd/coun/commands.src | 0 | 42 |
| M | scripts/textcmd/coun/concealme.src | 2 | 1 |
| M | scripts/textcmd/coun/createnpc.src | 1 | 0 |
| M | scripts/textcmd/coun/cwstone.src | 2 | 1 |
| M | scripts/textcmd/coun/findtotem.src | 1 | 0 |
| M | scripts/textcmd/coun/getlooters.src | 2 | 1 |
| M | scripts/textcmd/coun/go.src | 352 | 103 |
| M | scripts/textcmd/coun/goob.src | 1 | 0 |
| M | scripts/textcmd/coun/gorealm.src | 2 | 1 |
| M | scripts/textcmd/coun/goto.src | 77 | 81 |
| M | scripts/textcmd/coun/goxyz.src | 1 | 0 |
| M | scripts/textcmd/coun/home.src | 1 | 0 |
| M | scripts/textcmd/coun/jail.src | 2 | 1 |
| M | scripts/textcmd/coun/light.src | 2 | 1 |
| M | scripts/textcmd/coun/makegate.src | 1 | 0 |
| M | scripts/textcmd/coun/mazegate.src | 2 | 1 |
| M | scripts/textcmd/coun/notes.src | 2 | 1 |
| M | scripts/textcmd/coun/openpack.src | 2 | 1 |
| M | scripts/textcmd/coun/page.src | 1 | 0 |
| M | scripts/textcmd/coun/privs.src | 2 | 1 |
| M | scripts/textcmd/coun/refreshme.src | 1 | 0 |
| M | scripts/textcmd/coun/release.src | 2 | 1 |
| M | scripts/textcmd/coun/releaseinfo.src | 2 | 1 |
| M | scripts/textcmd/coun/res.src | 1 | 0 |
| M | scripts/textcmd/coun/resme.src | 1 | 0 |
| M | scripts/textcmd/coun/sayabove.src | 1 | 0 |
| M | scripts/textcmd/coun/staff.src | 1 | 0 |
| M | scripts/textcmd/coun/thaw.src | 1 | 0 |
| M | scripts/textcmd/coun/unconcealme.src | 1 | 0 |
| M | scripts/textcmd/coun/unparalyze.src | 1 | 0 |
| M | scripts/textcmd/coun/visit.src | 1 | 0 |
| M | scripts/textcmd/coun/warning.src | 1 | 0 |
| M | scripts/textcmd/gm/changename.src | 1 | 0 |
| M | scripts/textcmd/gm/changesex.src | 1 | 0 |
| M | scripts/textcmd/gm/chatglobaltimeout.src | 2 | 1 |
| M | scripts/textcmd/gm/cleartrashlb.src | 2 | 1 |
| M | scripts/textcmd/gm/create.src | 1 | 0 |
| M | scripts/textcmd/gm/createstack.src | 1 | 0 |
| M | scripts/textcmd/gm/createtourneybag.src | 1 | 0 |
| M | scripts/textcmd/gm/getobjproperty.src | 2 | 1 |
| M | scripts/textcmd/gm/getprop.src | 1 | 0 |
| D | scripts/textcmd/gm/goto.src | 0 | 110 |
| M | scripts/textcmd/gm/gotoserial.src | 2 | 1 |
| M | scripts/textcmd/gm/kick.src | 2 | 1 |
| M | scripts/textcmd/gm/lockdown.src | 2 | 1 |
| M | scripts/textcmd/gm/makekey.src | 1 | 0 |
| M | scripts/textcmd/gm/moveitem.src | 1 | 0 |
| M | scripts/textcmd/gm/mx.src | 2 | 1 |
| M | scripts/textcmd/gm/my.src | 2 | 1 |
| M | scripts/textcmd/gm/mz.src | 2 | 1 |
| M | scripts/textcmd/gm/newiteminfo.src | 2 | 1 |
| M | scripts/textcmd/gm/openbank.src | 2 | 1 |
| M | scripts/textcmd/gm/openit.src | 2 | 1 |
| M | scripts/textcmd/gm/openpack.src | 2 | 1 |
| M | scripts/textcmd/gm/poo.src | 2 | 1 |
| M | scripts/textcmd/gm/props.src | 2 | 1 |
| M | scripts/textcmd/gm/px.src | 2 | 1 |
| M | scripts/textcmd/gm/py.src | 2 | 1 |
| M | scripts/textcmd/gm/pz.src | 1 | 0 |
| M | scripts/textcmd/gm/raiserect.src | 2 | 1 |
| M | scripts/textcmd/gm/setDestination.src | 1 | 0 |
| M | scripts/textcmd/gm/setobjproperty.src | 2 | 1 |
| M | scripts/textcmd/gm/setprop.src | 1 | 0 |
| M | scripts/textcmd/gm/shave.src | 1 | 0 |
| M | scripts/textcmd/gm/silence.src | 2 | 1 |
| M | scripts/textcmd/gm/unconcealhim.src | 2 | 1 |
| M | scripts/textcmd/gm/unlockdown.src | 2 | 1 |
| M | scripts/textcmd/player/arm.src | 2 | 1 |
| M | scripts/textcmd/player/autoloop.src | 1 | 0 |
| M | scripts/textcmd/player/cast.src | 2 | 1 |
| M | scripts/textcmd/player/chat.src | 2 | 1 |
| M | scripts/textcmd/player/clearmsglog.src | 2 | 1 |
| M | scripts/textcmd/player/commands.src | 137 | 20 |
| M | scripts/textcmd/player/consider.src | 2 | 1 |
| M | scripts/textcmd/player/count.src | 2 | 1 |
| M | scripts/textcmd/player/disarm.src | 1 | 0 |
| M | scripts/textcmd/player/dropskills.src | 1 | 0 |
| M | scripts/textcmd/player/fame.src | 2 | 1 |
| M | scripts/textcmd/player/gateprompt.src | 2 | 1 |
| M | scripts/textcmd/player/guards.src | 1 | 0 |
| M | scripts/textcmd/player/guilds.src | 2 | 1 |
| M | scripts/textcmd/player/hairshop.src | 437 | 86 |
| M | scripts/textcmd/player/hungry.src | 1 | 0 |
| M | scripts/textcmd/player/infovault.src | 2 | 1 |
| M | scripts/textcmd/player/move.src | 2 | 1 |
| M | scripts/textcmd/player/online.src | 2 | 1 |
| M | scripts/textcmd/player/pagans.src | 1 | 0 |
| M | scripts/textcmd/player/password.src | 2 | 1 |
| M | scripts/textcmd/player/prots.src | 2 | 1 |
| M | scripts/textcmd/player/reags.src | 1 | 0 |
| M | scripts/textcmd/player/recalltotem.src | 2 | 1 |
| M | scripts/textcmd/player/removejewels.src | 1 | 0 |
| M | scripts/textcmd/player/setemail.src | 1 | 0 |
| M | scripts/textcmd/player/showclasse.src | 1 | 0 |
| M | scripts/textcmd/player/skills.src | 2 | 1 |
| M | scripts/textcmd/player/suicide.src | 1 | 0 |
| M | scripts/textcmd/player/togglebuildmark.src | 2 | 1 |
| M | scripts/textcmd/player/togglediff.src | 1 | 0 |
| M | scripts/textcmd/player/trashlb.src | 1 | 0 |
| M | scripts/textcmd/player/undressme.src | 1 | 0 |
| M | scripts/textcmd/player/where.src | 1 | 0 |
| M | scripts/textcmd/player/whereboat.src | 1 | 0 |
| M | scripts/textcmd/player/wheretotem.src | 1 | 0 |
| M | scripts/textcmd/seer/action.src | 2 | 1 |
| M | scripts/textcmd/seer/bank.src | 1 | 0 |
| M | scripts/textcmd/seer/bc.src | 1 | 0 |
| M | scripts/textcmd/seer/bow.src | 1 | 0 |
| M | scripts/textcmd/seer/chatban.src | 2 | 1 |
| M | scripts/textcmd/seer/createnpc.src | 1 | 0 |
| M | scripts/textcmd/seer/destroy.src | 2 | 1 |
| M | scripts/textcmd/seer/distance.src | 2 | 1 |
| M | scripts/textcmd/seer/findboat.src | 1 | 0 |
| M | scripts/textcmd/seer/freeze.src | 1 | 0 |
| D | scripts/textcmd/seer/go.src | 0 | 189 |
| D | scripts/textcmd/seer/goto.src | 0 | 110 |
| M | scripts/textcmd/seer/goxyz.src | 1 | 0 |
| M | scripts/textcmd/seer/info.src | 1 | 0 |
| M | scripts/textcmd/seer/kill.src | 30 | 0 |
| M | scripts/textcmd/seer/makegate.src | 1 | 0 |
| M | scripts/textcmd/seer/mark.src | 1 | 0 |
| M | scripts/textcmd/seer/mazegate.src | 2 | 1 |
| M | scripts/textcmd/seer/mtele.src | 2 | 1 |
| M | scripts/textcmd/seer/npclist.src | 2 | 1 |
| M | scripts/textcmd/seer/refresh.src | 1 | 0 |
| M | scripts/textcmd/seer/sayabove.src | 1 | 0 |
| M | scripts/textcmd/seer/speedwalk.src | 1 | 0 |
| M | scripts/textcmd/seer/tame.src | 1 | 0 |
| M | scripts/textcmd/seer/tele.src | 2 | 1 |
| M | scripts/textcmd/seer/teleto.src | 2 | 1 |
| M | scripts/textcmd/seer/thawme.src | 1 | 0 |
| M | scripts/textcmd/seer/turn.src | 2 | 1 |
| M | scripts/textcmd/seer/untamable.src | 2 | 1 |
| M | scripts/textcmd/test/animationtest.src | 2 | 1 |
| M | scripts/textcmd/test/checksys.src | 2 | 1 |
| M | scripts/textcmd/test/colorstest.src | 1 | 0 |
| A | scripts/textcmd/test/createinbag.src | 59 | 0 |
| M | scripts/textcmd/test/dungtele.src | 1 | 0 |
| A | scripts/textcmd/test/dupebag.src | 63 | 0 |
| A | scripts/textcmd/test/editcharacter.src | 223 | 0 |
| M | scripts/textcmd/test/eraseglobalprop.src | 2 | 1 |
| M | scripts/textcmd/test/extralogin.src | 1 | 0 |
| M | scripts/textcmd/test/findspnm.src | 2 | 1 |
| M | scripts/textcmd/test/goteles.src | 2 | 1 |
| M | scripts/textcmd/test/goxyz.src | 1 | 0 |
| M | scripts/textcmd/test/killpid.src | 2 | 1 |
| M | scripts/textcmd/test/listcontents.src | 1 | 0 |
| M | scripts/textcmd/test/makesigns.src | 2 | 1 |
| M | scripts/textcmd/test/makestaff.src | 2 | 1 |
| M | scripts/textcmd/test/restartall.src | 1 | 0 |
| A | scripts/textcmd/test/restartscript.src | 25 | 0 |
| M | scripts/textcmd/test/sendpacket.src | 1 | 0 |
| M | scripts/textcmd/test/setglobal.src | 2 | 1 |
| M | scripts/textcmd/test/skillstest.src | 1 | 0 |
| M | scripts/textcmd/test/startscript.src | 2 | 1 |
| M | scripts/textcmd/test/sysload.src | 1 | 0 |
| M | scripts/textcmd/test/test.src | 2 | 1 |
| M | scripts/textcmd/test/unbanacc.src | 1 | 0 |
| M | scripts/textcmd/test/unload.src | 1 | 0 |
| M | scripts/textcmd/test/unloadall.src | 1 | 0 |
| M | scripts/textcmd/test/unloadcfg.src | 1 | 0 |
| M | scripts/textcmd/test/whereat.src | 1 | 0 |
| M | scripts/textcmd/test/wipemods.src | 1 | 0 |
| A | spawnpoint_container_objtypes.txt | 274 | 0 |
| D | zho_changelog.txt | 0 | 36 |

---

## Commit Subjects In Range

- 2059290 Merchant Backpack also moved to master on strip command
- ab165f0 Townperson, noble, minstril, performer, and person updates to limit to be within town borders updated clilocs removed death prints fixed .go
- f4acd82 Fix for player merchant payouts Final fix for hairshop
- 244db33 Player vendors can now be elven or gargoyle kill and akill now properly deletes player merchants hairshop updates to work with elves and gargoyles and the new hairstyles
- 040f86c Go command updated Player Merchant more bug fixes for storage regions updated goto updated
- 0f45252 Player Vendor Changes Start
- 8815cbe More commands updated
- fed6e13 Updated Commands Added synopsis to all commands added python script to run when cmds change fixed the .commands to make it work
- dad4b9b Areas migrated to data storage instead of using global cprops
- b2da0a3 Areas Fixes
- c2f3966 Areas Fix for pagefile error
- 8b62b72 Sosaria Areas and Regions Update Animation Test update
- 6e79828 Patch Notes for v1.0.0

---

Generated from git diff and git log for exhaustive, file-complete coverage of this range.
