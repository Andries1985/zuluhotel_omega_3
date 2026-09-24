// spellgrants.inc -- 2026-09-22 (area 5 part 2 review).
//
// Two spell effects write DURABLE character properties that only their own
// sleeping script used to remove, so a fumble or a restart left them behind:
//
//  - the Dragon Skin verse adds its points to PhysicalProtection (the property
//    equipment grants; combat cuts melee damage 5% per point) and remembered
//    that only in the runtime #DragonSkin flag -- the points stayed for good
//    and stacked after every restart;
//  - Antidote and Sanctuary give the caster PermPoisonImmunity (again the
//    equipment property, read by the poison check) for up to 300 s.
//
// Both now leave a plain marker (DragonSkinGrant = points given,
// SpellPoisonImmunity = 1). These two functions take the grant back; the
// spells call them on their normal end and scripts/misc/logon.src calls
// them at every login, so nothing survives a restart.
use uo;

function ReclaimDragonSkinGrant( person )

	var grant := CInt( GetObjProperty( person, "DragonSkinGrant" ) );
	if( !grant )
		return 0;
	endif

	var currlvl := CInt( GetObjProperty( person, "PhysicalProtection" ) ) - grant;
	if( currlvl < 1 )
		EraseObjProperty( person, "PhysicalProtection" );
	else
		SetObjProperty( person, "PhysicalProtection", currlvl );
	endif
	EraseObjProperty( person, "DragonSkinGrant" );
	EraseObjProperty( person, "#DragonSkin" );
	return 1;

endfunction

function ReclaimSpellPoisonImmunity( who )

	if( !GetObjProperty( who, "SpellPoisonImmunity" ) )
		return 0;
	endif
	EraseObjProperty( who, "SpellPoisonImmunity" );

	// back to what worn items give: the highest level among them, the same
	// rule scripts/control/skilladvancerequip.src applies on equip
	var level := 0;
	foreach thing in ListEquippedItems( who )
		var itemlevel := CInt( GetObjProperty( thing, "PermPoisonImmunity" ) );
		if( itemlevel > level )
			level := itemlevel;
		endif
	endforeach

	if( level )
		SetObjProperty( who, "PermPoisonImmunity", level );
	else
		EraseObjProperty( who, "PermPoisonImmunity" );
	endif
	return 1;

endfunction
