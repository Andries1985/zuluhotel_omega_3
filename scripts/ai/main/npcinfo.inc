use uo;
use os;
use util;
use cfgfile;

function imalooter(npctemplate)

	var cfg := ReadConfigFile("::npcdesc");
	if (cfg[npctemplate].noloot) return 0; else return 1; endif

endfunction

function imanundead(npctemplate)

	var cfg := ReadConfigFile("::npcdesc");
	var undead := cfg[npctemplate].undead;
	if (!undead) return 0; else return undead; endif

endfunction

function HasNoCorpse(npctemplate)

	var cfg := ReadConfigFile("::npcdesc");
	if (cfg[npctemplate].nocorpse) return 1; else return 0; endif

endfunction

// 2026-09-30: IsHostile() and IsGood() removed -- nothing ever called them, and the "hostile"
// template field they read was dropped from npcdesc.cfg in the reformat. Target choice lives in
// the combat/fight includes.
