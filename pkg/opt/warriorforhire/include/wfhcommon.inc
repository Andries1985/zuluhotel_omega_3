use uo;

function AbandonWarrior(warriorObj)

	SetObjProperty(warriorObj, "AbandonedByOwner", 1);
	SetObjProperty(warriorObj, "guardkill", 1);
	warriorObj.kill();

endfunction
