use uo;

// 2026-10-10: ported from ZH2.5 Patch 1.1.4. A warrior can be brought back from each of its first
// ten deaths, by its heart or by the High Priest's memory (WFHBackup), and either way it spends the
// same life. The eleventh death is final.
const WFH_MAX_DEATHS := 10;

// 2026-10-19: moved here from warrior.src (where ZH2.5 keeps it too) so .maxwfh shares it.
const WFH_SKILL_CAP_TENTHS := 1300; // 130.0 displayed -- hard ceiling regardless of the master's own skill

// 2026-10-10: Anatomy, Parry, Healing, Tactics, Archery, Swordsmanship, Macefighting, Fencing,
// Wrestling, Throwing. warrior.src (TRACKED_SKILLS) and textcmd .maxwfh both read it here.
function WfhTrackedSkills()

	return {1, 5, 17, 27, 31, 40, 41, 42, 43, 57};

endfunction

// 2026-10-10: one point of each stat per death so far (the old "- (resnum-1)+1" gained a point on the
// first revival), floored at 10, the lowest stat .maxwfh accepts. Heart and priest revivals both use it
// (the priest's memory revival since 2026-10-19, your pick; it used to restore full stats).
function WfhStatAfterLoss(stat, resnum)

	var result := CInt(stat) - CInt(resnum);
	if (result < 10)
		result := 10;
	endif
	return result;

endfunction

// 2026-10-10: a heart revives only the death the owner's WFHBackup was written for. death.src stamps
// both with the corpse serial (WFHDeathId) and either revival erases the backup, so a heart held
// back from an earlier death cannot be used later to roll the death count back.
// 2026-10-19 (your pick): hearts written before the stamp existed are refused outright. They used to
// pass whenever the owner had no backup, which every revival now erases, so one kept in a chest was
// a free extra life.
function IsCurrentWfhHeart(heart, owner)

	var heartid := CInt(GetObjProperty(heart, "WFHDeathId"));
	if (!heartid)
		return 0;
	endif

	var backup := GetObjProperty(owner, "WFHBackup");
	if (!backup)
		return 0;
	endif
	return (heartid == CInt(backup["WFHDeathId"]));

endfunction

// Kills a warrior its owner has given up. AbandonedByOwner tells death.src not to drop a heart or
// write a backup; death.src escrows the gear straight away (2026-10-10), so the owner can recover
// it from the High Priest for 90 days as after any other death.
function AbandonWarrior(warriorObj)

	SetObjProperty(warriorObj, "AbandonedByOwner", 1);
	SetObjProperty(warriorObj, "guardkill", 1);
	warriorObj.kill();

endfunction
