/*
        Maintened by *Edwards

        2010-11-11
*/
use uo;
use os;

include "include/client";
include "../pkg/utils/itemUtils/include/itemUtil";
include "include/sysevent";
include ":mdgumps:yesNo";
include "include/attributes";
//include ":mounts:mounts";
include ":karmafame:karmafame";
include "../pkg/opt/rituals/include/settings";
include "include/spelldata";
include "include/classes";

var magic_circle := array{},
    captor;

// The full 15-property jewelry-exclusive family, used only for the
// ritual-side "already has a different enchantment" check. The shared
// `immunities` array in namingbyenchant.inc only has 12 of these 15 - it's
// missing PoisonImmunity/MagicImmunity/SpellReflect because those three use
// RenameByProt instead of RenameByImmunity for naming purposes, a naming-
// only distinction that correctly stays as-is and isn't touched here.
var ritual_immunity_family := { "AirProtection", "EarthProtection", "FireProtection", "WaterProtection",
                                 "HolyProtection", "NecroProtection", "FreeAction", "HealingIncrease",
                                 "PhysicalProtection", "PoisonImmunity", "MagicImmunity", "SpellReflect",
                                 "PermPoisonImmunity", "PermMagicImmunity", "Permmr" };

// True if item already carries one of the 15-property family other than
// own_property - the Group 1 exclusivity rule every ritual in that family
// must check before granting or reinforcing its own property.
function RitualHasOtherImmunity( item, own_property )

        foreach prop in ritual_immunity_family
                if( prop != own_property && GetObjProperty( item, prop ))
                        return 1;
                endif
                SleepMS(5);
        endforeach

        return 0;
endfunction

// Group 2 (hitscript/onhitscript) is isolated from Group 3 (the weapon/
// armor "physical" trio - damage/AR/durability/stat mods) entirely: an item
// can carry one or the other, never both. Checked from both directions.
function RitualHasHitscript( item )

        if( item.IsA( POLCLASS_WEAPON ))
                return item.hitscript != "";
        endif

        return CInt( GetObjProperty( item, "OnHitscriptNumber" )) > 0;
endfunction

function RitualHasGroup3Property( item )

        return CInt( item.dmg_mod ) || CInt( item.ar_mod ) || CInt( item.maxhp_mod ) ||
               GetObjProperty( item, "str" ) || GetObjProperty( item, "int" ) || GetObjProperty( item, "dex" ) ||
               GetObjProperty( item, "ArBonus" );
endfunction

function PerformRitual( mobile, spell_id )

        SetObjProperty( mobile, "#Casting", GetPid() );

	if( !CheckRitualEquip( mobile, spell_id ))
		return 0;
        endif

        if( !CheckRitualCircle( mobile ))
		return 0;
	endif

        // Target selection moved ahead of circle-drawing so Gate 1 (the hard
        // Magery floor) can be checked - and can fail cleanly, before any of
        // the chant or mana cost - now that it needs to know the target's
        // current tier for the 8 reinforcement-style rituals.
        var item := GetRitualItem( mobile, spell_id );
        if( !item )
                return 0;
        endif

        if( RitualIsAlreadyMaxed( mobile, spell_id, item ))
                return 0;
        endif

        var gate_info := GetRitualGateInfo( spell_id, item );
        if( mobile.cmdlevel < 4 && GetEffectiveSkill( mobile, SKILLID_MAGERY ) < gate_info[1] )
                SendSysMessage( mobile, "Your skill with Magery isn't strong enough to attempt this ritual." );
                return 0;
        endif

        if( !DrawRitualCircle( mobile, spell_id ))
                DestroyDone( mobile, 1 );
		return 0;
	endif

        var result := ProcessRitual( mobile, spell_id, item, gate_info );
        if( result == 0 )
                DestroyDone( mobile, 1, item );
		return 0;
	elseif( result == -1 )
                // Gate 2 already resolved (fizzle/backfire/catastrophic) and
                // cleaned up the circle/item/caster itself - don't double it.
		return 0;
	endif

        DestroyDone( mobile );

	return item;
endfunction

function CheckRitualEquip( mobile, spell_id )

        if( mobile.cmdlevel > 3 )
                return 1;
        endif
        
        if( spell_id != 134 )
		var staff := GetEquipmentByLayer( mobile, LAYER_HAND2 ),
	   	    robe  := GetEquipmentByLayer( mobile, LAYER_ROBE );
     
        	if( !robe.IsRitualRobe() )
			SendSysMessage( mobile, "You must wear a consecrated ritual robe to perform a ritual." );
			return 0;
		elseif( !staff.IsOakStaff() )
			SendSysMessage( mobile, "You must wear a consecrated young oak staff to perform a ritual." );
			return 0;
		elseif( GetObjProperty( robe, "Consecrated" ) != mobile.serial )
			SendSysMessage( mobile, "Your ritual robe is not consecrated." );
			return 0;
		elseif( GetObjProperty( staff, "Consecrated" ) != mobile.serial )
			SendSysMessage( mobile, "Your ritual oak staff isn't consecrated." );
			return 0;
		endif
        endif
         
	// if( MP_GetMount( mobile ))
	// 	SendSysMessage( mobile, "You can't perform a ritual while mounted." );
	// 	return 0;
	// endif

	return 1;
endfunction

function CheckRitualCircle( mobile )

	//Check if the caster want to use his personnal circle
        var existing_circle;
	foreach thing in ListItemsAtLocation( mobile.x, mobile.y, mobile.z, mobile.realm );
		if( thing.IsRitualRune() )
			existing_circle := thing;
			break;
		endif
  
                SleepMS(5);
	endforeach
 
        if( existing_circle )
		SendSysMessage( mobile, "You can't perform a ritual by using an existing magic circle." );
                return 0;
        elseif( Len( ListItemsNearLocation( mobile.x, mobile.y+5, mobile.z, 6, mobile.realm )) && mobile.cmdlevel < 4 )
		SendSysMessage( mobile, "You don't have the necessary space to draw a magic circle here." );
		return 0;
	endif

	return 1;
endfunction

// Every failure branch in DrawRitualCircle/UndoRitualCircle used to just
// silently return 0 - no feedback at all about what went wrong, which made
// the ritual choreography impossible to learn from a failed attempt. This
// puts the reason in bright red text above the caster's head every time.
function RITUAL_FailFeedback( mobile, message )

        PrintTextAbove( mobile, message, FONT_NORMAL, FONTCOLOR_RED );

endfunction

function DrawRitualCircle( mobile, spell_id )

        var elem_drawing := RITUAL_GetSettingsCfgElem( "DrawCircle" ),
            elem_settings := RITUAL_GetSettingsCfgElem( "General" ),
            speech_delay := elem_settings.DelaySpeech,
            base_light := elem_settings.LightLevel,
            //base_hp := AP_GetVital( mobile, HITS ),
			base_hp := GetHP( mobile ),
            current_speech := 1;

	var x := mobile.x,
	    y := mobile.y,
	    z := mobile.z,
            realm := mobile.realm,
	    center_y := y+5;

        CreateSpeechCaptor( mobile, x, center_y, z, spell_id );

	SendSysMessage( mobile, "You can start to draw the circle" );
	while( mobile.connected )
                
                var speech := GetConfigString( elem_drawing, "Speech"+current_speech );
                if( !speech )
                	break;
                elseif( !captor )
                	RITUAL_FailFeedback( mobile, "*The ritual circle collapses - something disrupted it!*" );
                	return 0;
                endif

		var text := GetConfigString( elem_drawing, "Print"+current_speech ),
		    x_mod := GetConfigInt( elem_drawing, "Xmod"+current_speech ),
		    y_mod := GetConfigInt( elem_drawing, "Ymod"+current_speech ),
		    facing := GetConfigInt( elem_drawing, "Facing"+current_speech ),
		    appear := GetConfigInt( elem_drawing, "Appear"+current_speech );

                SendSysMessage( mobile, "Ritual: "+speech, 3, 553 );

                var starting_delay := ReadGameClock();
		while( mobile.connected )

                	if( !captor )
                		RITUAL_FailFeedback( mobile, "*The ritual circle collapses - something disrupted it!*" );
                		return 0;
                	elseif( ReadGameClock() - starting_delay > speech_delay )
				RITUAL_FailFeedback( mobile, "*You took too long to speak the words!*" );
				return 0;
			//elseif( AP_GetVital( mobile, HITS ) < base_hp )
			elseif( GetHP( mobile ) < base_hp )
				RITUAL_FailFeedback( mobile, "*You were hurt and lost your concentration!*" );
				return 0;
			endif

		        var event := os::Wait_For_Event(1);
			if( event && event.source == mobile )
				if( event.type == SYSEVENT_SPEECH )
					if( event.text[speech] )
                                                break;
                                        else
						RITUAL_FailFeedback( mobile, "*Those weren't the right words!*" );
						return 0;
                                        endif
				endif
			endif
		endwhile

                Sleep(1);

                if( mobile.x != ( x+x_mod ) || mobile.y != ( y+y_mod ))
			RITUAL_FailFeedback( mobile, "*You are standing in the wrong spot for this step!*" );
			return 0;
                elseif( facing != mobile.facing )
			RITUAL_FailFeedback( mobile, "*You are facing the wrong direction for this step!*" );
			return 0;
                endif

		if( text )
			PrintTextAbove( mobile, text );
		endif
		if( appear )
			var itemx_mod := GetConfigInt( elem_drawing, "Xmod"+( current_speech+1 ));
			var itemy_mod := GetConfigInt( elem_drawing, "Ymod"+( current_speech+1 ));

			magic_circle.Append( CreateItemAtLocation( x+itemx_mod, y+itemy_mod, z, appear, 1, realm ));
		endif

		ChangeLightLevel( mobile, CInt( base_light*current_speech ));

		current_speech += 1;
	endwhile

	CreateMagicCircle( x, y, z, realm );
	CreateCandles( mobile, x, y, z, realm );
	CircleUnDecayable( mobile );

	return 1;
endfunction

function UndoRitualCircle( mobile, item:=0, practice:=0 )

        var north_rune := GetRitualRuneNorth( mobile );
        if( !north_rune )
		SendSysMessage( mobile, "Your circle was corrupted!" );
                DestroyDone( mobile, !practice, item );
                return 0;
        endif
            
        var elem_drawing := RITUAL_GetSettingsCfgElem( "UndoCircle" ),
            elem_settings := RITUAL_GetSettingsCfgElem( "General" ),
            speech_delay := elem_settings.DelaySpeech,
            //base_hp := AP_GetVital( mobile, HITS ),
			base_hp := GetHP( mobile ),
            current_speech := 1,
            x := north_rune.x,
	    y := north_rune.y;;

	SendSysMessage( mobile, "You must now undo the magic circle to successfully finish the ritual." );
	while( mobile.connected )

                var speech := GetConfigString( elem_drawing, "Speech"+current_speech );
                if( !speech )
                	break;
                elseif( !captor )
                	RITUAL_FailFeedback( mobile, "*The ritual circle collapses - something disrupted it!*" );
                	return 0;
                endif

		var text := GetConfigString( elem_drawing, "Print"+current_speech ),
		    x_mod := GetConfigInt( elem_drawing, "Xmod"+current_speech ),
		    y_mod := GetConfigInt( elem_drawing, "Ymod"+current_speech ),
		    facing := GetConfigInt( elem_drawing, "Facing"+current_speech );

                SendSysMessage( mobile, "Ritual: "+speech, 3, 553 );

                var starting_delay := ReadGameClock();
		while( mobile.connected )

                	if( !captor )
                		RITUAL_FailFeedback( mobile, "*The ritual circle collapses - something disrupted it!*" );
                		return 0;
                	elseif( ReadGameClock() - starting_delay > speech_delay )
                                RITUAL_FailFeedback( mobile, "*You took too long to speak the words!*" );
                                DestroyDone( mobile, !practice, item );
				return 0;
			//elseif( AP_GetVital( mobile, HITS ) < base_hp )
			elseif( GetHP( mobile ) < base_hp )
                                RITUAL_FailFeedback( mobile, "*You were hurt and lost your concentration!*" );
                                DestroyDone( mobile, !practice, item );
				return 0;
			endif

		        var event := os::Wait_For_Event(1);
			if( event && event.source == mobile )
				if( event.type == SYSEVENT_SPEECH )
					if( event.text[speech] )
                                                break;
                                        else
                                                RITUAL_FailFeedback( mobile, "*Those weren't the right words!*" );
                                                DestroyDone( mobile, !practice, item );
						return 0;
                                        endif
				endif
			endif
		endwhile

                Sleep(1);

                if( mobile.x != ( x+x_mod ) || mobile.y != ( y+y_mod ))
                        RITUAL_FailFeedback( mobile, "*You are standing in the wrong spot for this step!*" );
                        DestroyDone( mobile, !practice, item );
			return 0;
                elseif( facing != mobile.facing )
                        RITUAL_FailFeedback( mobile, "*You are facing the wrong direction for this step!*" );
                        DestroyDone( mobile, !practice, item );
			return 0;
                endif

		if( text )
			PrintTextAbove( mobile, text );
		endif

		current_speech += 1;
	endwhile
 
        Sleep(2);
        
        DestroyDone( mobile );

	return 1;
endfunction

function CreateMagicCircle( x, y, z, realm )

	magic_circle.Append( CreateItemAtLocation( x+2, y+7, z, 0x1cf1, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+7, z, 0x1cf2, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+7, z, 0x1cf3, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+7, z, 0x1cf4, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-2, y+7, z, 0x1cf5, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-2, y+6, z, 0x1cf6, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-2, y+5, z, 0x1cf7, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-2, y+4, z, 0x1cf8, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-2, y+3, z, 0x1cf9, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+3, z, 0x1cfa, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+2, z, 0x1cfb, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+2, z, 0x1cfc, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+2, z, 0x1cfd, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+2, y+2, z, 0x1cfe, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+3, y+2, z, 0x1cff, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+3, y+3, z, 0x1d00, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+3, y+4, z, 0x1d01, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+3, y+5, z, 0x1d02, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+3, y+6, z, 0x1d03, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+2, y+6, z, 0x1d04, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+6, z, 0x1d05, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+6, z, 0x1d06, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+6, z, 0x1d07, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+5, z, 0x1d08, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x-1, y+4, z, 0x1d09, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+3, z, 0x1d0a, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+3, z, 0x1d0b, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+2, y+3, z, 0x1d0c, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+2, y+4, z, 0x1d0d, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+2, y+5, z, 0x1d0e, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+5, z, 0x1d0f, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+5, z, 0x1d10, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x, y+4, z, 0x1d11, 1, realm ));
	magic_circle.Append( CreateItemAtLocation( x+1, y+4, z, 0x1d12, 1, realm ));

        return 1;
endfunction

function CreateCandles( mobile, x, y, z, realm )

	PerformAction( mobile, ANIM_CAST_AREA );
        Sleep(1);
	magic_circle.Append( CreateItemAtLocation(  x, y+2, z, 0x1854, 1, realm ));
	SetName( magic_circle[ Len( magic_circle )], "magic candle" );
	PlaySoundEffect( magic_circle[Len( magic_circle )], 0x022b );

	PerformAction( mobile, ANIM_CAST_AREA );
        Sleep(1);
	magic_circle.Append( CreateItemAtLocation( x+3, y+3, z, 0x1854, 1, realm ));
	SetName( magic_circle[ Len( magic_circle )], "magic candle" );
	PlaySoundEffect( magic_circle[Len( magic_circle )], 0x022b );

	PerformAction( mobile, ANIM_CAST_AREA );
        Sleep(1);
	magic_circle.Append( CreateItemAtLocation( x+2, y+6, z, 0x1854, 1, realm ));
	SetName( magic_circle[ Len( magic_circle )], "magic candle" );
	PlaySoundEffect( magic_circle[Len( magic_circle )], 0x022b );

	PerformAction( mobile, ANIM_CAST_AREA );
        Sleep(1);
	magic_circle.Append( CreateItemAtLocation( x-1, y+7, z, 0x1854, 1, realm ));
	SetName( magic_circle[ Len( magic_circle )], "magic candle" );
	PlaySoundEffect( magic_circle[Len( magic_circle )], 0x022b );

	PerformAction( mobile, ANIM_CAST_AREA );
        Sleep(1);
	magic_circle.Append( CreateItemAtLocation( x-2, y+4, z, 0x1854, 1, realm ));
	SetName( magic_circle[ Len( magic_circle )], "magic candle" );
	PlaySoundEffect( magic_circle[Len( magic_circle )], 0x022b );
 
	Sleep(1);

        return 1;
endfunction

function GetRitualRuneNorth( mobile )

	foreach thing in ( ListItemsNearLocation( mobile.x, mobile.y+5, mobile.z, 6, mobile.realm ))
		if( thing.IsRitualRune() == "north" )
			return thing;
		endif

                SleepMS(5);
	endforeach
 
        return 0;
endfunction

function CreateSpeechCaptor( mobile, x, y, z, spell_id )

	captor := CreateItemAtLocation( x, y, z, 0xBA2B, 1, mobile.realm );
        if( captor.errortext )
                SendSysMessage( mobile, "Error creating ritual captor -->"+captor.errortext );
                return 0;
        endif

	RegisterForSpeechEvents( captor, 6 );
	EnableEvents( SYSEVENT_SPEECH, 6 );
        
        SetObjProperty( captor, "#MasterSerial", mobile.serial );
        SetObjProperty( captor, "SpellId", spell_id );
 
	magic_circle.Append( captor );

        return 1;
endfunction

function CircleUnDecayable( mobile )

	foreach item in magic_circle
 
                SetObjProperty( item, "#PermMagicCircle", mobile.serial );
                item.movable := 0;
		item.DecayAt := 0;
                item.SaveOnExit := 0;
                
                SleepMS(5);
	endforeach

        return 1;
endfunction

function DestroyDone( mobile, forced:=0, item:=0 )

	foreach item in magic_circle
		DestroyItem( item );
                SleepMS(5);
	endforeach
 
        if( item )
                DestroyItem( item );
        endif
        
        if( forced )
		PrintTextAbove( mobile, "*The mystic energy go out of control and unleash itself upon "+mobile.name+"!*" );
                if( mobile.cmdlevel < 4 )
		        mobile.Kill();
                endif
        endif
 
        return 1;
endfunction

function ChangeLightLevel( mobile, level )

        foreach player in ListMobilesNearLocation( mobile.x, mobile.y, mobile.z, 10, mobile.realm );
                SleepMS(5);
                if( player.IsA( POLCLASS_NPC ))
                        continue;
                endif

                player.SetLightLevel( level, -1 );
        endforeach
        
        return 1;
endfunction

function GetRitualItem( mobile, spell_id )

	var ritual_cfg := RITUAL_GetSettingsCfgElem( spell_id ),
	    targ_options := GetConfigString( ritual_cfg, "TargOptions" ),
            item;

	while( mobile.connected )

		SendSysMessage( mobile, "Choose the item you wish to enchant." );
		item := Target( mobile );
                if( !item )
			SendSysMessage( mobile, "The ritual is cancelled." );
                        return 0;
       		elseif( !useItem_check( mobile, item, ITEM_INCONTAINER ))
			continue;
                elseif( GetObjProperty( item, "UnEnchantable" ) || item.newbie )
			SendSysMessage( mobile, "You can't enchant this item." );
			continue;
                endif

                case( targ_options )
                        "crystal":
                                if( !item.IsCrystalMana() )
				        SendSysMessage( mobile, "You must target a mana crystal." );
                                        continue;
                                endif
                                break;
                        "ritual":
                                if( !item.IsRitualRobe() && !item.IsOakStaff() )
				        SendSysMessage( mobile, "You must target a ritual robe or a young oak staff." );
                                        continue;
                                endif
                                break;
                        "jewel":
                                if( !item.IsJewel() )
				        SendSysMessage( mobile, "You must target a jewel." );
                                        continue;
                                endif
                                break;
                        "armor":
                                if( !item.IsBoneArmor() && !item.IsChainmailArmor() && !item.IsPlatemailArmor() &&
                                    !item.IsLeatherArmor() && !item.IsRingmailArmor() && !item.IsShield() && !item.IsStuddedLeatherArmor() )
				        SendSysMessage( mobile, "You must target an armor." );
                                        continue;
                                endif
                                break;
                        "hp":
                                if( !item.hp )
				        SendSysMessage( mobile, "You must target an item with HP." );
                                        continue;
                                endif
                                break;
                        "jewelArmor":
                                if( !item.IsBoneArmor() && !item.IsChainmailArmor() && !item.IsPlatemailArmor() && !item.IsJewel() &&
                                    !item.IsLeatherArmor() && !item.IsRingmailArmor() && !item.IsShield() && !item.IsStuddedLeatherArmor() )
				        SendSysMessage( mobile, "You must target a piece of armor or a jewel." );
                                        continue;
                                endif
                                break;
                        "weapon":
                                if( !item.IsA( POLCLASS_WEAPON ))
				        SendSysMessage( mobile, "You must target a weapon." );
                                        continue;
                                endif
                                break;
                        "cloth":
                                if( !item.IsCloth() )
				        SendSysMessage( mobile, "You must target clothes." );
                                        continue;
                                endif
                                break;
                        "equippable":
                                if( !item.IsA( POLCLASS_EQUIPMENT ))
				        SendSysMessage( mobile, "You must target an equipment." );
                                        continue;
                                endif
                                break;
                        "clothJewel":
                                if( !item.IsCloth() && !item.IsJewel() )
				        SendSysMessage( mobile, "You must target clothing or a jewel." );
                                        continue;
                                endif
                                break;
                        "wearable":
                                if( !item.IsBoneArmor() && !item.IsChainmailArmor() && !item.IsPlatemailArmor() && !item.IsJewel() &&
                                    !item.IsLeatherArmor() && !item.IsRingmailArmor() && !item.IsShield() && !item.IsStuddedLeatherArmor() && !item.IsCloth() )
				        SendSysMessage( mobile, "You must target a piece of armor, clothing, or a jewel." );
                                        continue;
                                endif
                                break;
                        "weaponArmor":
                                if( !item.IsBoneArmor() && !item.IsChainmailArmor() && !item.IsPlatemailArmor() && !item.IsJewel() && !item.IsA( POLCLASS_WEAPON )
                                    && !item.IsLeatherArmor() && !item.IsRingmailArmor() && !item.IsShield() && !item.IsStuddedLeatherArmor() )
				        SendSysMessage( mobile, "You must target a piece of armor or a jewel." );
                                        continue;
                                endif
                                break;
                        default:
                                break;
                endcase

                var cursed := GetObjProperty( item, "Cursed" );
		if( cursed && targ_options != "cursed" )
			SendSysMessage( mobile, "You can't target cursed items." );
			continue;
		endif

                return item;
	endwhile

        return 0;
endfunction

function ProcessRitual( mobile, spell_id, item, gate_info )

        var north_rune := GetRitualRuneNorth( mobile );
        if( !north_rune )
		SendSysMessage( mobile, "Your circle was corrupted!" );
                return 0;
        endif

	var x := north_rune.x,
	    y := north_rune.y,
	    z := north_rune.z,
            realm := north_rune.realm;
 
        if( item )
		item.movable := 1;
		MoveObjectToLocation( item, x, y+5, z, realm, MOVEOBJECT_FORCELOCATION );
		item.movable := 0;
        endif
        
	if( YesNo( mobile, "Use mana crystals?" ))
		while( mobile.connected )
			SendSysMessage( mobile, "Select a mana crystal to access, Hit Esc to cancel." );
			var crystal := Target( mobile );
			if( !crystal )
                                SendSysMessage( mobile, "Cancelled the use of a mana crystal." );
				break;
       			elseif( !useItem_check( mobile, crystal, ITEM_INCONTAINER ))
				continue;
                        elseif( !crystal.IsCrystalMana() )
				SendSysMessage( mobile, "That is not a mana crystal." );
				continue;
                        endif

                        var crystal_level := CInt( GetObjProperty( crystal, "ManaLevel" ));
                        if( !crystal_level )
				SendSysMessage( mobile, "This crystal have no mystic energy left to use." );
				continue;
			endif

                        SetMana( mobile, GetMana( mobile ) + crystal_level );
                        SendSysMessage( mobile, "The crystal's stored energy flows into you." );
                        EraseObjProperty( crystal, "ManaLevel" );
		endwhile
	endif

        // Gate 2's mana floor, checked here (after the crystal offer, before
        // the chant below starts draining a quarter of current mana per
        // line) rather than at the very end against whatever the chant left
        // behind - checking it post-drain made it fail rituals the caster
        // had clearly brought enough mana for, and did so via a forced
        // destroy+kill rather than the rung this ritual is actually supposed
        // to use. Running short on mana isn't a failed casting, so nothing
        // is destroyed and no one is hurt - the ritual is just called off.
        var spell_elem := MS_GetSpellsCfgElem( spell_id );
        if( mobile.cmdlevel < 4 && GetMana( mobile ) < spell_elem.ManaCost )
                SendSysMessage( mobile, "You don't have enough mana to continue this ritual!" );
                RITUAL_EndCircleSurvivingItem( mobile, item, 0 );
                return -1;
        endif

	PrintTextAbove( item, "*The ritual has started*" );
	Sleep(5);

        var ritual_cfg := RITUAL_GetSettingsCfgElem( spell_id ),
            elem_settings := RITUAL_GetSettingsCfgElem( "General" ),
            speech_delay := elem_settings.DelaySpeech,
            //base_hp := AP_GetVital( mobile, HITS ),
			base_hp := GetHP( mobile ),
            current_speech := 1;
        
	var cloud_z := 1;
	while( GetConfigString( ritual_cfg, "Speech"+cloud_z ))
		cloud_z += 1;
                SleepMS(5);
	endwhile

	var mana_cloud := CreateItemAtLocation( item.x, item.y, item.z+( 4 * cloud_z ), 0x3789, 1, realm );
        if( mana_cloud.errortext )
                PrintTextAbove( item, "Error creating mana cloud -->"+mana_cloud.errortext );
        else
		SetName( mana_cloud, "mana cloud" );
  
		mana_cloud.movable := 0;
		mana_cloud.saveonexit := 0;
        	magic_circle.Append( mana_cloud );
        endif

	while( mobile.connected )

                var speech := GetConfigString( ritual_cfg, "Speech"+current_speech );
                if( !speech )
                	break;
                elseif( !captor )
                	return 0;
                endif

		var text := GetConfigString( ritual_cfg, "Print"+current_speech ),
		    x_mod := GetConfigInt( ritual_cfg, "Xmod"+current_speech ),
		    y_mod := GetConfigInt( ritual_cfg, "Ymod"+current_speech ),
		    facing := GetConfigInt( ritual_cfg, "Facing"+current_speech ),
		    action := GetConfigInt( ritual_cfg, "Action"+current_speech ),
		    appear := GetConfigString( ritual_cfg, "Appear"+current_speech ),
		    anim := GetConfigInt( ritual_cfg, "Anim"+current_speech ),
		    sfx := GetConfigInt( ritual_cfg, "Sfx"+current_speech );

                SendSysMessage( mobile, "Ritual: "+speech, 3, 553 );

                var starting_delay := ReadGameClock();
		while( mobile.connected )

                	if( !captor )
                		return 0;
                	elseif( ReadGameClock() - starting_delay > speech_delay )
				return 0;
			//elseif( AP_GetVital( mobile, HITS ) < base_hp )
			elseif( GetHP( mobile ) < base_hp )
				return 0;
			endif

		        var event := os::Wait_For_Event(1);
			if( event && event.source == mobile )
				if( event.type == SYSEVENT_SPEECH )
					if( event.text[speech] )
                                                break;
                                        else
						return 0;
                                        endif
				endif
			endif
		endwhile

                Sleep(1);

                if( mobile.x != ( x+x_mod ) || mobile.y != ( y+y_mod ))
			return 0;
                elseif( facing != mobile.facing )
			return 0;
                endif
                
		if( text )
			PrintTextAbove( mobile, text );
		endif
		if( action )
			PerformAction( mobile, action );
		endif
		if( anim )
			PlayObjectCenteredEffect( mobile, anim, 0xa, 0xa );
		endif
		if( sfx )
			PlaySoundEffect( mobile, sfx );
		endif

                
		//var mana_amount := CInt( AP_GetVital( mobile, MANA ) / 4 );
		var mana_amount := CInt( GetMana( mobile ) / 4 );
        //AP_ConsumeVital( mobile, MANA, mana_amount );
		SetMana( mobile, CInt( GetMana( mobile ) - mana_amount ));
  
		PlayMovingEffect( mobile, mana_cloud, 0x3789, 0xa, 0xa );
  
		if( item )
			PrintTextAbove( item, "*the mystic energy channels above the item*" );
		endif

		if( appear )
			var the_items := SplitWords( appear ),
			    item_x_mods := SplitWords( GetConfigString( ritual_cfg, "IXmod"+current_speech )),
			    item_y_mods := SplitWords( GetConfigString( ritual_cfg, "IYmod"+current_speech ));

			var i;
			for( i:=1; i<=Len( the_items ); i+=1 )
				var to_be_created := CInt( the_items[i] ),
				    item_x_mod := CInt( item_x_mods[i] ),
				    item_y_mod := CInt( item_y_mods[i] );
         
				if( !to_be_created )
					to_be_created := GetObjtypeByName( the_items[i] );
				endif

				magic_circle.Append( CreateItemAtLocation( x+item_x_mod, y+item_y_mod, z, to_be_created, 1, realm ));
				magic_circle[Len( magic_circle )].movable := 0;

				SleepMS(5);
			endfor
		endif

		if( item )
			item.movable := 1;
			MoveObjectToLocation( item, item.x, item.y, item.z+2, realm, MOVEOBJECT_FORCELOCATION );
			item.movable := 0;
		endif

		mana_cloud.movable := 1;
		MoveObjectToLocation( mana_cloud, mana_cloud.x, mana_cloud.y, mana_cloud.z-2, realm, MOVEOBJECT_FORCELOCATION );
		mana_cloud.movable := 0;

		current_speech += 1;
		Sleep(2);
	endwhile
 
	Sleep(7);

	PrintTextAbove( item, "*All the mystic energy goes through "+item.desc+" as the final ritual process begins*" );
	PlaySoundEffect( mobile, 0x01fe );

        // Finalization is purely the visual mana-cloud effect now - the chant
        // loop above already drained the caster's own mana once per line, and
        // any crystal charge was never taken from the caster in the first
        // place. Draining total_mana a second time here re-billed the caster
        // for the full pooled total on top of what they'd already paid.
        var i;
	for( i:=1; i<=10; i+=1 )
		PlayMovingEffect( mana_cloud, mobile, 0x3789, 0xa, 0xa );
		SleepMS(500);
	endfor

	DestroyItem( mana_cloud );

        var gate_result := RitualSkillCheck( mobile, item, gate_info );
        if( gate_result == 0 )
                return 0;
        elseif( gate_result == -1 )
                return -1;
        endif

        var undo_ok := UndoRitualCircle( mobile, item );
        if( !undo_ok )
                return -1;
        endif
        ChangeLightLevel( mobile, 0 );

	item.movable := 1;
	MoveItemToContainer( item, mobile.backpack );

	return 1;
endfunction

// Gate 2 - the real risk, at the very end of the chant. The mana floor is
// checked earlier in ProcessRitual, before the chant starts draining mana;
// this only rolls the success percentage. Returns 1 on success, or -1 once
// a fizzle/backfire/catastrophic outcome has already been fully resolved
// (circle cleaned up, item and caster handled) so the caller must not
// destroy anything a second time.
function RitualSkillCheck( mobile, item, gate_info )

	var success_pct := gate_info[2],
            rung := gate_info[3];

        if( mobile.cmdlevel < 4 )
		if( Random( 100 ) + 1 > success_pct )
                        RITUAL_ResolveFailure( mobile, item, rung );
                        return -1;
		endif
	endif

	Sleep(5);

	PrintTextAbove( item, "The item absorbed the enchantment successfully!" );

        PlaySoundEffect( item, 0x1FB );
        PlayObjectCenteredEffectEx( item, 0x373A, 4, 14 );

	AwardFame( mobile, CInt( gate_info[1] ));

	return 1;
endfunction

// The three-rung consequence ladder. rung 1 = Fizzle (item and caster both
// untouched), rung 2 = Backfire (item survives, 50% chance the caster dies
// anyway), rung 3 = Catastrophic Failure (item destroyed, caster force-
// killed - the old uniform behavior, now reserved for the highest-risk
// casts instead of every failure).
function RITUAL_ResolveFailure( mobile, item, rung )

        if( rung == 1 )
                SendSysMessage( mobile, "The ritual fizzles - the enchantment fails to take hold, but nothing is lost." );
                RITUAL_EndCircleSurvivingItem( mobile, item, 0 );
        elseif( rung == 2 )
                SendSysMessage( mobile, "The ritual backfires!" );
                RITUAL_EndCircleSurvivingItem( mobile, item, 50 );
        else
                PrintTextAbove( mobile, "*The mystic energy spirals catastrophically out of control!*" );
                DestroyDone( mobile, 1, item );
        endif

        return 1;
endfunction

// Tears down the circle and returns the item to the caster's backpack
// unenchanted, with an optional percent chance of a forced kill (used by
// Backfire's coin-flip; pass 0 for Fizzle's no-risk outcome).
function RITUAL_EndCircleSurvivingItem( mobile, item, kill_pct )

        foreach circle_item in magic_circle
                DestroyItem( circle_item );
                SleepMS(5);
        endforeach

        if( item )
                item.movable := 1;
                MoveItemToContainer( item, mobile.backpack );
        endif

        if( kill_pct && Random( 100 ) + 1 <= kill_pct )
                PrintTextAbove( mobile, "*The backlash proves too much for "+mobile.name+" to bear!*" );
                if( mobile.cmdlevel < 4 )
                        mobile.Kill();
                endif
        endif

        return 1;
endfunction

// 2026-09-01: previously each ritual's own item cap was only checked in its
// ApplyRitualEffect(), which runs after PerformRitual() has already spent
// the mana, drawn the circle, and resolved the Gate 2 risk roll -- so
// casting a tiered ritual on an already-maxed item could still destroy the
// item and kill the caster via a Catastrophic failure for zero possible
// benefit. Called from PerformRitual() right after the item is targeted,
// before any mana/circle cost. Mirrors each ritual's own existing cap
// condition and message (still the authoritative check inside
// ApplyRitualEffect, unchanged, as a safety net) rather than replacing it
// with a new formula. Rituals whose outcome depends on an interactive
// choice (which stat/element to bind) only reject here when every option is
// already maxed; if any option is still open, this passes through and lets
// ApplyRitualEffect's own logic handle the choice exactly as before.
function RitualIsAlreadyMaxed( mobile, spell_id, item )

        var item_desc := TruncateArticle( item.desc );

        case( spell_id )
                133: // Blood Seeking - dmg_mod cap
                        if( CInt( item.dmg_mod ) >= 30 )
                                SendSysMessage( mobile, "There's already a stronger enchantment on this item." );
                                return 1;
                        endif
                139: // Hardening - ar_mod cap
                        if( CInt( item.ar_mod ) >= 6 )
                                SendSysMessage( mobile, "There's already a stronger enchantment on "+item_desc+"." );
                                return 1;
                        endif
                149: // Resilience - maxhp_mod cap
                        if( CInt( item.maxhp_mod ) >= 30 )
                                SendSysMessage( mobile, "There's already have a more powerfull enchantment on "+item_desc+"." );
                                return 1;
                        endif
                154: // Vital Infusion - only reject if the item's already-bound stat (if any) is capped
                        if( (CInt( GetObjProperty( item, "str" )) >= 12) ||
                            (CInt( GetObjProperty( item, "int" )) >= 12) ||
                            (CInt( GetObjProperty( item, "dex" )) >= 12) )
                                SendSysMessage( mobile, "There's already a stronger enchantment on "+item_desc+"." );
                                return 1;
                        endif
                137: // Elemental Ward - only reject if the item's already-bound element (if any) is at 95
                        if( Max( Max( CInt( GetObjProperty( item, "FireProtection" )), CInt( GetObjProperty( item, "WaterProtection" ))),
                                 Max( CInt( GetObjProperty( item, "AirProtection" )), CInt( GetObjProperty( item, "EarthProtection" )))) >= 95 )
                                SendSysMessage( mobile, "There's already a stronger enchantment on "+item_desc+"." );
                                return 1;
                        endif
                144: // Planard Ward - same, Holy/NecroProtection
                        if( Max( CInt( GetObjProperty( item, "HolyProtection" )), CInt( GetObjProperty( item, "NecroProtection" ))) >= 95 )
                                SendSysMessage( mobile, "There's already a stronger enchantment on "+item_desc+"." );
                                return 1;
                        endif
                147: // Quick Healing - single-shot; any existing value blocks a recast
                        if( GetObjProperty( item, "HealingIncrease" ))
                                SendSysMessage( mobile, "This "+item_desc+" already have a powerful enchantment on it." );
                                return 1;
                        endif
                143: // Physical Ward - capped at the caster's own current class level
                        if( CInt( GetObjProperty( item, "PhysicalProtection" )) >= GetClasseLevel( mobile ))
                                SendSysMessage( mobile, "This "+item_desc+" already have a more powerful enchantment on it." );
                                return 1;
                        endif
        endcase

        return 0;
endfunction

// Gate 1 + Gate 2 + consequence-rung lookup, computed per-ritual so a blank
// item's first push gates low and a near-maxed item's last push gates at
// the top of the scale. Returns array{ difficulty, success_pct, rung }.
// rung: 1 = Fizzle only, 2 = up to Backfire, 3 = always Catastrophic.
function GetRitualGateInfo( spell_id, item )

        if( spell_id == 136 )
                return RITUAL_CursingGateInfo( item );
        endif

        var next_tier := RITUAL_GetNextTier( spell_id, item );
        if( next_tier >= 0 )
                return RITUAL_TierGateInfo( next_tier );
        endif

        // Flat, non-tiered rituals - fixed risk band per ritual.
        case( spell_id )
                132: // Attunement
                134: // Consecration
                135: // Create Focus
                140: // Mana Dissimal
                141: // Mana Flux
                146: // Purification
                152: // Venom Bane
                        return array{ 25, 98, 1 };
                131: // Advanced Theurgy
                138: // Free Movement
                145: // Protective Aura
                        return array{ 55, 90, 1 };
                150: // Spell Bouncing
                151: // Spell Warding
                153: // Venom Mastery
                        return array{ 150, 27, 2 };
                142: // Perilous Theurgy - locked at a flat 10%
                148: // Racial Theurgy - locked at a flat 10%
                        return array{ 150, 10, 3 };
                default:
                        return array{ 25, 98, 1 };
        endcase

endfunction

// Returns the tier (1-6) this cast would push the item to if it's one of
// the 8 reinforcement-style rituals, or -1 if spell_id isn't tier-scaled
// (Cursing is handled separately; everything else falls through to the
// flat risk-band table in GetRitualGateInfo).
function RITUAL_GetNextTier( spell_id, item )

        case( spell_id )
                133: // Blood Seeking - dmg_mod: 0/5/10/15/20/25/30
                        return ( CInt( item.dmg_mod ) / 5 ) + 1;
                139: // Hardening - ar_mod: 0..6, raw
                        return CInt( item.ar_mod ) + 1;
                149: // Resilience - maxhp_mod: 0/5/10/15/20/25/30
                        return ( CInt( item.maxhp_mod ) / 5 ) + 1;
                154: // Vital Infusion - str/int/dex: 0/2/4/6/8/10/12
                        var statval := CInt( GetObjProperty( item, "str" )) +
                                       CInt( GetObjProperty( item, "int" )) +
                                       CInt( GetObjProperty( item, "dex" ));
                        return ( statval / 2 ) + 1;
                147: // Quick Healing - HealingIncrease: raw 0..6, upgrade-only to class level
                        return CInt( GetObjProperty( item, "HealingIncrease" )) + 1;
                143: // Physical Ward - PhysicalProtection: raw 0..6, upgrade-only to class level
                        return CInt( GetObjProperty( item, "PhysicalProtection" )) + 1;
                137: // Elemental Ward - Fire/Water/Air/EarthProtection: 0/25/50/65/75/85/95
                        return RITUAL_ProtLevelToTier( Max( Max( CInt( GetObjProperty( item, "FireProtection" )), CInt( GetObjProperty( item, "WaterProtection" ))),
                                                             Max( CInt( GetObjProperty( item, "AirProtection" )), CInt( GetObjProperty( item, "EarthProtection" ))))) + 1;
                144: // Planard Ward - Holy/NecroProtection: 0/25/50/65/75/85/95
                        return RITUAL_ProtLevelToTier( Max( CInt( GetObjProperty( item, "HolyProtection" )), CInt( GetObjProperty( item, "NecroProtection" )))) + 1;
                default:
                        return -1;
        endcase

endfunction

// Maps a stored protection value (0/25/50/65/75/85/95) to its tier index
// (0-6) on the real shard scale.
function RITUAL_ProtLevelToTier( level )

        case( CInt( level ))
                95: return 6;
                85: return 5;
                75: return 4;
                65: return 3;
                50: return 2;
                25: return 1;
                default: return 0;
        endcase

endfunction

// Gate info from the tier this cast would push the item to (1-6). A blank
// item's first push (tier 1) gates low; the last push into tier 5-6 gates
// at the top of the scale and can only ever be Catastrophic on a miss.
function RITUAL_TierGateInfo( next_tier )

        var difficulty, success_pct, rung;

        if( next_tier <= 1 )
                difficulty := 55;
                success_pct := 90;
        else
                difficulty := 40 + ( 15 * next_tier );
                success_pct := 95 - ( 7 * next_tier );
        endif

        if( next_tier >= 5 )
                rung := 3;
        elseif( next_tier >= 3 )
                rung := 2;
        else
                rung := 1;
        endif

        return array{ difficulty, success_pct, rung };
endfunction

// Cursing's difficulty scales with the target's own existing quality, not a
// cast count - it's a one-shot curse, so there's no tier ladder of its own
// to key off. isRare/isRareProt and isLegendary both carry Catastrophic
// risk on a miss; only their odds differ.
function RITUAL_CursingGateInfo( item )

        var is_legendary := GetObjProperty( item, "isLegendary" ),
            is_rare := GetObjProperty( item, "isRare" ) || GetObjProperty( item, "isRareProt" ),
            is_uncommon := GetObjProperty( item, "isUncommon" ) || GetObjProperty( item, "isUncommonProt" ),
            skilladv := CInt( GetObjProperty( item, "skilladvamount" ));

        if( is_legendary )
                return array{ 165, 2, 3 };
        elseif( is_rare )
                return array{ 150, 28, 3 };
        elseif( is_uncommon || skilladv > 1 )
                return array{ 85, 65, 2 };
        endif

        return array{ 55, 90, 1 };
endfunction
