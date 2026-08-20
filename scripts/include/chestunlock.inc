// Shared "a chest just got unlocked" entry point - called right after any
// unlock path sets .locked := 0 (currently: regular lockpicking's
// PickTreasureChest). Only handles the pkg/opt/areaspawner half directly,
// as a speed optimization (kicks the refill immediately instead of waiting
// for the eventual destroy) - pkg/opt/spawnpoint deliberately does NOT get
// an immediate call here: its spawndeath.src isn't safe to fire twice for
// the same object (no duplicate guard, unlike RemoveAreaSpawnerSerial), and
// it already gets notified correctly once, at actual destroy time, via
// pkg/items/containers/container/canDestroy.src's WarnSpawnPoint - the
// same generic per-container DestroyScript hook that file now also uses to
// notify areaspawner. No-ops for any chest neither package spawned.

include ":areaspawner:areaspawner";	// NotifyAreaSpawnerChestGone

function NotifyChestUnlocked(chest)
	return NotifyAreaSpawnerChestGone(chest);
endfunction
