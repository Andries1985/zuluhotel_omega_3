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

  RainDownMoney(corpse);
  sleep(600); // Destroy the corpse in 10 minutes.

  var altar := GetAltarByCorpse(corpse);
  EraseObjProperty(altar, "players");
  DestroyItem(corpse);
endfunction

function RegisterChampionSpawnKill(corpse)
  if(!GetObjProperty(corpse, "iChampSpawn"))
    return;
  endif

  var killer := SystemFindObjectBySerial(GetObjProperty(corpse, "KilledBySerial"));
  var altar := SystemFindObjectBySerial(GetObjProperty(corpse, "sAltarSerial"));
  var players := GetObjProperty(altar, "players");

  if(players[killer.serial])
    players[killer.serial] := CInt(players[killer.serial]) + 1;
  else
    players[killer.serial] := 1;
  endif

  SetObjProperty(altar, "players", players);
endfunction

function GetAltarByCorpse(corpse)
  return SystemFindObjectBySerial(GetObjProperty(corpse, "altar"));
endfunction
