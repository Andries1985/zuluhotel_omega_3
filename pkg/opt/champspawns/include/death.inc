include ":champspawns:rewards";

function ProcessChampSpawnCorpse(corpse)

  RegisterChampionSpawnKill(corpse);
  // Restored -- this call site (scripts/misc/death.src) only destroys the
  // corpse elsewhere if "nocorpse" is set, which champspawn templates aren't
  // confirmed to set. Without this, champ-spawn corpses could accumulate on
  // the ground instead of clearing ~2s after death like they used to.
  Sleep(2);
  DestroyItem(corpse);

endfunction

function ProcessChampionCorpse(corpse)
  if(!GetObjProperty(corpse, "Champion"))
    return;
  endif

  // 2026-10-29: read the altar now and pay the reward bags at the moment of death, not after the
  // corpse's 10 minutes; a champion with no altar (staff-spawned) only rains gold (ZH2.5 1.1.5 port)
  var altar := GetAltarByCorpse(corpse);
  if(altar)
    GiveRewardsToPlayers(altar);
    EraseObjProperty(altar, "players");
  endif

  RainDownMoney(corpse);
  sleep(600); // Destroy the corpse in 10 minutes.

  DestroyItem(corpse);
endfunction

// 2026-10-29: one kill for the reward bag tally, kept on the altar as players[serial] := {kills, systime
// of the last kill}. Only kills while the altar is active count, and only by players (ZH2.5 1.1.5 port)
function RegisterChampionSpawnKill(corpse)
  if(!GetObjProperty(corpse, "iChampSpawn"))
    return;
  endif

  var altar_serial := CInt(GetObjProperty(corpse, "sAltarSerial"));
  if(!altar_serial)
    return;
  endif
  var altar := SystemFindObjectBySerial(altar_serial);
  if(!altar or !GetObjProperty(altar, "active"))
    return;
  endif

  var killer := FindChampionSpawnKiller(corpse);
  if(!killer)
    return;
  endif

  // Many spawn corpses can be processed at once (area spells): no other script may change the
  // tally between the read and the write.
  set_critical(1);
  var players := GetObjProperty(altar, "players");
  if(TypeOf(players) != "Dictionary")
    players := dictionary;
  endif

  // An entry written before this port is the bare kill count.
  var entry := players[killer.serial];
  var kills;
  if(TypeOf(entry) == "Array")
    kills := CInt(entry[1]);
  else
    kills := CInt(entry);
  endif
  players[killer.serial] := array{kills + 1, POLCore().systime};

  SetObjProperty(altar, "players", players);
  set_critical(0);
endfunction

// 2026-10-29: the player who killed a spawn creature: KilledBySerial, or failing that the player who
// hit it last according to its hitlist ({serial, factor, game clock}). 0 when neither is a player.
function FindChampionSpawnKiller(corpse)
  var serial := CInt(GetObjProperty(corpse, "KilledBySerial"));
  if(serial)
    var killer := SystemFindObjectBySerial(serial, SYSFIND_SEARCH_OFFLINE_MOBILES);
    if(IsChampionCreditable(killer))
      return killer;
    endif
  endif

  var hitlist := GetObjProperty(corpse, PROPID_MOBILE_HITLIST);
  if(TypeOf(hitlist) != "Array")
    return 0;
  endif
  var newest := 0;
  var newest_time := -1;
  foreach hit in hitlist
    if(CInt(hit[3]) > newest_time)
      var mobile := SystemFindObjectBySerial(CInt(hit[1]), SYSFIND_SEARCH_OFFLINE_MOBILES);
      if(IsChampionCreditable(mobile))
        newest := mobile;
        newest_time := CInt(hit[3]);
      endif
    endif
  endforeach
  return newest;
endfunction

function IsChampionCreditable(mobile)
  if(!mobile or !mobile.IsA(POLCLASS_MOBILE))
    return 0;
  endif
  return (!mobile.npctemplate and !mobile.cmdlevel);
endfunction

// 2026-10-29: the altar that spawned this champion, or 0 when it has none (ZH2.5 1.1.5 port)
function GetAltarByCorpse(corpse)
  var serial := CInt(GetObjProperty(corpse, "altar"));
  if(!serial)
    return 0;
  endif
  return SystemFindObjectBySerial(serial);
endfunction
