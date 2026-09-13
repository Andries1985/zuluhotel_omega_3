include "include/random";

function process_combat_event(ev)

	ThrowPotion(ev.source);

        if (ev.type == SYSEVENT_DAMAGED)
		if ((GetHP(me) * 100/GetMaxHP(me)) < flee_point)
			EraseObjProperty(me,"#flees");
			flee(ev.source);
		endif
        endif

endfunction

function in_combat_event_loop(opponent)

	ThrowPotion(opponent);

endfunction

function post_combat()

	DisableCombatEvents();
	EnableMainEvents();

	SetWarMode( 0 );
	SetOpponent( 0 );

	sleep(1);
	look_around();

endfunction

function prepare_for_fight(opponent)

	DisableMainEvents();
	EnableCombatEvents();

endfunction

function ThrowPotion(you)

	if (!GetObjProperty(me,"#lastbreath"))
		SetObjProperty(me,"#lastbreath",ReadGameClock() + 5);
	endif

	if (GetObjProperty(me,"#lastbreath") > ReadGameClock() )
		return;
	endif

	SetObjProperty(me,"#lastbreath",ReadGameClock() + 5);

	if ( (distance(me,you)>2) and Random(2) )

		var potiontype := GetObjProperty(me,"ExplosionPotionType");
		if (!potiontype)
			potiontype := 0xdc09; // ExplosionPotion (Strength 3) -- default tier
		endif

		var potion := CreateItemInInventory(me.backpack, potiontype, 1);
		if (!potion)
			return;
		endif

		var where := array;
		where.+x;
		where.+y;
		where.+z;
		where.+realm;
		where.x := you.x;
		where.y := you.y;
		where.z := you.z;
		where.realm := you.realm;

		PlayMovingEffectXYZ( me.x, me.y, me.z+15, where.x, where.y, where.z, 0x0f0d, 8, 0, 0, where.realm );

		var parms := {};
		parms[1] := me;
		parms[2] := potion;
		parms[3] := where;

		start_script( ":alchemy:exploder", parms );

	endif

endfunction
