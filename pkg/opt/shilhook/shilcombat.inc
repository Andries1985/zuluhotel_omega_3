use uo;
use os;
use math;
use cfgfile;

const DELAY_TO_GET_COMBATSKILL := 300;

function IdentifyAttacker( who )

	var newcombat_hit_list := GetObjProperty( who, "Hitlist" );

	if( newcombat_hit_list )
		EraseObjProperty( who, "Hitlist" );

		foreach arr in newcombat_hit_list
			// Specify that you're dealing with the most recent kill on the hitlist
			if( ReadGameClock() < Cint(arr[3]) + DELAY_TO_GET_COMBATSKILL ) 
				var attacker := SystemFindObjectBySerial( Cint(arr[1]) );
				if( !attacker )
					attacker := SystemFindObjectBySerial( Cint(arr[1]), SYSFIND_SEARCH_OFFLINE_MOBILES );
				endif
				
				if( attacker )
					AffectCombatSkillForKill( attacker, defender, weapon.skillid );
				endif
			endif

		endforeach
	endif

endfunction


function AffectCombatSkillForKill ( attacker, defender, weapon )

	var skillused := GetObjProperty ( who, weapon.skillid);
	var delta_expgain := expgain;
	var delta_fencing := 0;
	var delta_macing := 0;

		if ( attacker )
			if ( skillused := 42 )
				delta_fencing := delta_expgain;
			endif

			if ( skillused := 41 )
				delta_macing := delta_expgain;
			endif
		endif

	if ( delta_fencing != 0 )
		AwardFencingSkill ( attacker , delta_fencing );
	endif

	if ( delta_macing  != 0 )
		AwardMacingSkill ( attacker , delta_macing );
	endif
endfunction

function AwardFencingSkill ( attacker , amt )
	var rawfencing := GetRawSkill (who, CInt(42));

	SetRawSkill ( attacker , 42, Cint(BaseSkillToRawSkill(rawfencing + delta_expgain)) );

endfunction

function AwardMacingSkill ( who , amt )
	var rawmacing := GetRawSkill (who, CInt(41));

	SetRawSkill ( attacker , 42, Cint(BaseSkillToRawSkill(rawmacing + delta_expgain)) );

endfunction