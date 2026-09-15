//Edited September 6, 2024

use uo;
use os;
use util;
use cfgfile;

include "include/classes";
include ":combat:hitscriptinc";
include ":armor:armorZones";
include "include/random";

function OmegaAttack(attacker, defender)

	//var attackerweaponType := cfg[attacker.weapon.objtype];

	var weapfile := ReadConfigFile( ":combat:itemdesc" );
	var attackerweaponType := weapfile[attacker.weapon.objtype];

//	var listofanim	:=	GetConfigIntArray( attackerweaponType, "Anim" );

//	Print($"listofanim " +listofanim);

//	var Anim := listofanim[(Random(3)+1)];
//	Print($"animation " +Anim);

	if(attacker.weapon.attribute == ATTRIBUTEID_ARCHERY || attacker.weapon.attribute == ATTRIBUTEID_THROWING)
		if (!checkForAndDecrementAmmunition( attacker, attackerweaponType))
			return;
		endif
		if(attacker.weapon.attribute == ATTRIBUTEID_ARCHERY)
			if (!ValidArcheryWeapon(attackerweaponType))
				return;
			endif
		else
			if (!ValidThrowingWeapon(attackerweaponType))
				return;
			endif
		endif
		PerformAction( attacker, attackerweaponType.Anim);
		if(attacker.weapon.attribute == ATTRIBUTEID_ARCHERY)
			PlayArcheryAnimation(attacker, defender, attackerweaponType);
		else
			PlayThrowAnimation(attacker, defender, attackerweaponType);
		endif
		if(CheckHitChance(attacker, defender))
			if(attackerweaponType.HitSound)
				PlaySoundEffect( attacker, attackerweaponType.ProjectileSound );
				PlaySoundEffect( defender, attackerweaponType.ProjectileSound );
			endif
		else
			if(attackerweaponType.MissSound)
				PlaySoundEffect( attacker, attackerweaponType.MissSound );
				PlaySoundEffect( defender, attackerweaponType.MissSound );
			endif
			return;
		endif
	else
		PerformAction( attacker, attackerweaponType.Anim);
		if(CheckHitChance(attacker, defender))
			if(attackerweaponType.HitSound)
				PlaySoundEffect( attacker, attackerweaponType.HitSound );
				PlaySoundEffect( defender, attackerweaponType.HitSound );
			endif
		else
			if(attackerweaponType.MissSound)
				PlaySoundEffect( attacker, attackerweaponType.MissSound );
				PlaySoundEffect( defender, attackerweaponType.MissSound );
				return;
			endif
		endif
	endif

	var hitscript := attacker.weapon.hitscript;

	// This is currently spamming the staff. Not sure how it's triggered though.
 	if(!hitscript)
		// Broadcast to staff
		var msg := "Can't find a weapon hitscript: " + hitscript +"From attacker: "+ attacker.name + " using item: "+attackerweaponType;
		foreach staff in EnumerateOnlineCharacters()
			if( staff.cmdlevel >= 4 )
				SendSysMessage( staff, msg, 3, 2581 );
			endif
		endforeach
		return;
	endif 
	//now we're sure there's a hit so check for damage on the weapon
	CheckEquipmentDamage(attacker, attacker.weapon);

	var weapDMG := attackerweaponType.damage;
	if(!weapDMG)
		weapDMG := "1d1";
	endif

	var wbasedamage:= RandomDiceRoll( weapDMG ); 
	if(attacker.weapon.dmg_mod)
		wbasedamage += attacker.weapon.dmg_mod;
	endif

	//find random piece of armor to hit -- need this for onhit script and damage
	var def_armor:=GetHitArmor(defender);	
	
	start_script(hitscript, {attacker, defender, attacker.weapon, def_armor, wbasedamage, 0});
	
endfunction


function GetLowestClassSkillValue(who)
	var classe := GetClass(who);
	var skills := GetClasseSkills(classe);
	var lowest_skill := 0;

	foreach s in (skills)
		var sval := GetEffectiveSkill(who, s);
		if (lowest_skill == 0)
			lowest_skill := sval;
		elseif (sval < lowest_skill)
			lowest_skill := sval;
		endif
	endforeach

	return lowest_skill;

endfunction


function CheckHitChance(attacker, defender)

	var skill_factor := 214.285714285714;
	var class_factor := 0.0238;
	// 2026-09-14: kept at 0.05 (not rescaled) -- hunger's ceiling moved from
	// 10 to 15 (pkg/std/cooking/hunger.src), so max hunger now gives a 75%
	// hit-chance penalty (15 * 0.05) instead of the old 50% (10 * 0.05).
	// Deliberate: being famished should hurt more now.
	var hunger_factor := 0.05;
	var min_hit_chance := 0.1;
	var weapon := attacker.weapon;
	var wep_attr := attacker.weapon.attribute;
	var wep_skill := GetSkillIdByAttributeId(wep_attr);
	var hunger := CDBl(GetObjProperty(attacker, "hunger"));
	var class_level := CDbl(GetClasseLevel(attacker));
	var attacker_wep_skill;
	//award skill points regardless of hit or miss
	CombatAdvancement( attacker, weapon, defender );
	// Thief moved off the lowest-of-8 blend as of the Throwing rework -- with a
	// real weapon skill now (Throwing, same as every other combat class), hit
	// chance should track what they actually trained for the weapon in hand,
	// not get capped by whichever unrelated class skill (Lockpicking, Stealth...)
	// they happen to have neglected. Mage/Bard are untouched, out of scope here.
	if(GetObjProperty( attacker , CLASSEID_MAGE) || GetObjProperty( attacker , CLASSEID_BARD))
		attacker_wep_skill := CDbl(GetLowestClassSkillValue(attacker));
	else
		attacker_wep_skill := CDbl(GetEffectiveSkill(attacker, wep_skill));
	endif

	var class_chance := 1 + (class_level * class_factor);
	var skill_chance := attacker_wep_skill / skill_factor;
	var hunger_penalty := 1 - (hunger * hunger_factor);

	var hit_chance := skill_chance * class_chance * hunger_penalty;
	var chance := RandomFloat(1.0);

	if (hit_chance < min_hit_chance)
		hit_chance := min_hit_chance;
	endif
	
	if( chance < hit_chance )
		return 1;
	else 
		return 0;
	endif
	
endfunction


function ValidArcheryWeapon(weaponType)

	//returns true if itemdesc elem contains the miniumum required props for processing
	if(!weaponType.ProjectileAnim || !weaponType.Anim || !weaponType.hitSound || !weaponType.projectileSound ||
		!weaponType.hitscript || !weaponType.damage || !weaponType.Projectile || !weaponType.ProjectileType )
		return 0;
	endif

	return 1;

endfunction

// Same shape as ValidArcheryWeapon(), minus ProjectileType -- a NonConsumingProjectile
// throwing weapon has no real ammo, so it shouldn't be forced to fake one just to pass.
function ValidThrowingWeapon(weaponType)

	if(!weaponType.ProjectileAnim || !weaponType.Anim || !weaponType.hitSound || !weaponType.projectileSound ||
		!weaponType.hitscript || !weaponType.damage || !weaponType.Projectile )
		return 0;
	endif

	return 1;

endfunction


function checkForAndDecrementAmmunition( who, weaponType )

	if (GetObjProperty(who.weapon, "NonConsumingProjectile"))
		return 1;
	endif

	var mypack := EnumerateItemsInContainer( who.backpack );
	var expotions := {0xdc08, 0xdc09, 0xdc07, 0xff2f, 0xff30, 0xff31, 0xff32, 0xff33, 0xff34, 0xff35, 0xff36};
	var type;

	foreach item in mypack
		if( item.objtype == weaponType.ProjectileType )
			if(SubtractAmount( item, weaponType.Projectile))
				return 1;
			endif
		elseif ( weaponType.ProjectileType == 1 && item.objtype in expotions)
			type := item.objtype;
			if(SubtractAmount( item, weaponType.Projectile))
				SetObjProperty(who, "#LastExplosionShot", type);
				return 1;
			endif
		endif
	endforeach

	return 0;

endfunction

function PlayArcheryAnimation(attacker, defender, attackerweaponType)
	var animation := attackerweaponType.ProjectileAnim;
	var color := 0;

	// Cains crossbow special!
	if (GetObjProperty(attacker.weapon, "#Activated") && attacker.weapon.objtype == 0xda15)
		color := 1304;
		animation := 0x183e;
	endif

	// Defaults here match what every existing archery weapon already got
	// hardcoded before these became tunable per-weapon itemdesc fields.
	var fxspeed := attackerweaponType.ProjectileSpeed;
	if(!fxspeed)
		fxspeed := 2;
	endif

	var fxduration := attackerweaponType.ProjectileLoop;
	if(!fxduration)
		fxduration := 0;
	endif

	var fxexplode := attackerweaponType.ProjectileExplode;
	if(!fxexplode)
		fxexplode := 0;
	endif

	PlayMovingEffectXYZEx( attacker.x, attacker.y, attacker.z+9, defender.x, defender.y, defender.z+5, defender.realm, animation, fxspeed, fxduration, color, 0, 0, fxexplode);
endfunction

// Isolated from PlayArcheryAnimation() on purpose -- Throwing weapons use the
// object-based moving-effect call (same pattern as fanofknives.src) instead of
// the XYZ-coordinate one Archery uses, so testing/tuning this never touches
// Bow/Crossbow/Dreams Bow's existing, working visual.
function PlayThrowAnimation(attacker, defender, attackerweaponType)
	var animation := attackerweaponType.ProjectileAnim;

	var fxspeed := attackerweaponType.ProjectileSpeed;
	if(!fxspeed)
		fxspeed := 5;
	endif

	var fxloop := attackerweaponType.ProjectileLoop;
	if(!fxloop)
		fxloop := 0;
	endif

	var fxexplode := attackerweaponType.ProjectileExplode;
	if(!fxexplode)
		fxexplode := 0;
	endif

	var fxhue := attackerweaponType.ProjectileHue;
	if(!fxhue)
		fxhue := 0;
	endif

	var fxrender := attackerweaponType.ProjectileRender;
	if(!fxrender)
		fxrender := 0;
	endif

	// Last three (effect3d/effect3dexplode/effect3dsound) are always 0 -- not exposed as options.
	PlayMovingEffectXYZEx( attacker.x, attacker.y, attacker.z+9, defender.x, defender.y, defender.z+5, defender.realm, animation, fxspeed, fxloop, fxhue, fxrender, 0, fxexplode, 0, 0, 0 );
endfunction

function CombatAdvancement( attacker, weapon, defender )
	
	AwardSkillPoints( attacker, GetSkillIdByAttributeId( weapon.attribute ), 20 );

	// Don't raise tactics with archery
	if (weapon.attribute != ATTRIBUTEID_ARCHERY)
		AwardSkillPoints( attacker, SKILLID_TACTICS, 20 );
	endif

endfunction

function GetHitArmor(who)

	var hit_zone := CS_GetRandomArmorZone();
	var armor_hit := CS_GetEquipmentInArmorZone(who, hit_zone);
	
	if ( armor_hit.Size() < 1 )
		return 0;
	endif

	var best_armor := struct;
	best_armor.name := "nothing-"+hit_zone;
	best_armor.desc := "nothing"+hit_zone;
	best_armor.ar := 0;
		
	foreach item in ( armor_hit )
		if ( item.ar > best_armor.ar )
			best_armor := item;
		endif
		SleepMS(2);
	endforeach
	if (best_armor.ar)
		return best_armor;
	else
		return 0;
	endif

endfunction