// kegvat.inc -- shared Fill/Empty/Transfer operate gump for Potion Kegs and
// Potion Vats (2026-09-15). Replaces potionkeg.src's old "Autoloop: yes/no,
// repeat once a second" pattern with the same 1/Number/Max picker Smelting
// uses (AutoLoop_init/AutoLoop_initFixed). One shared entry point for both
// container types -- a Vat is just a Keg with 5x the capacity and (per the
// Multi-Item Crafting plan) open/closed decided permanently at placement,
// not by double-click, so double-click is free here for both.
//
// Storage model (unchanged from the original potionkeg.src): ObjProperties
// "type" (potion objtype, 0 = empty), "subtype", "amount" (units stored).

use math;
use uo;
use polsys;

include "include/client";
include "include/itemutil";
include "include/objtype";
include "include/autoloop";
include "include/resourcemanager";
include ":mdgumps:gumps";
include "alchemy";

const KEGVAT_SOUND_POUR	:= 0x0241;

function KegVat_Capacity( container )
	if( container.objtype == UOBJ_POTION_KEG )
		return 100;
	endif
	return 500;
endfunction

// get_keg_name (alchemy.inc) always returns a full "a keg of X potions"
// sentence, even when called for a Vat -- swap the leading noun so a Vat
// doesn't get renamed to "a keg of...". Returns 0 (matching get_keg_name's
// own falsy-on-unrecognized behavior) if nothing matched.
function KegVat_ContentsName( container, potion_objtype, potion_subtype )
	var raw := get_keg_name( potion_objtype, potion_subtype );
	if( !raw )
		return 0;
	endif
	var full := CStr(raw);
	if( container.objtype != UOBJ_POTION_KEG and full[1,8] == "a keg of" )
		return "a vat of" + full[9, len(full)-8];
	endif
	return full;
endfunction

function KegVat_IsKegOrVat( objtype )
	if( objtype == UOBJ_POTION_KEG )
		return 1;
	elseif( objtype >= UOBJ_POTION_VAT_SOUTH_CLOSED and objtype <= UOBJ_POTION_VAT_EAST_OPEN )
		return 1;
	endif
	return 0;
endfunction

function KegVat_BaseName( container )
	if( container.objtype == UOBJ_POTION_KEG )
		return "keg";
	endif
	return "vat";
endfunction

// Small "how many?" picker -- mirrors the Make Now/Number/Max pattern from
// craftmenu.inc/MakeSmeltedIngots, just for fill/empty/transfer quantities.
function KegVat_PickQty( character, maxAllowed, label )
	if( maxAllowed < 1 )
		SendSysMessage( character, "There's nothing to do." );
		return 0;
	endif

	var gump := GFCreateGump( 100, 100 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9200, 260, 150 );
	GFHTMLArea( gump, 20, 15, 220, 20, "<B>" + label + "</B> (up to " + maxAllowed + ")", 0, 0 );
	GFAddButton( gump, 20, 55, 4005, 4007, GF_CLOSE_BTN, 1 );
	GFHTMLArea( gump, 55, 55, 180, 20, "Just 1", 0, 0 );
	GFAddButton( gump, 20, 90, 4005, 4007, GF_CLOSE_BTN, 2 );
	GFHTMLArea( gump, 55, 90, 180, 20, "A number...", 0, 0 );
	GFAddButton( gump, 20, 125, 4005, 4007, GF_CLOSE_BTN, 3 );
	GFHTMLArea( gump, 55, 125, 180, 20, "Max (" + maxAllowed + ")", 0, 0 );

	var input := GFSendGump( character, gump );
	if( !input )
		return 0;
	endif
	var choice := CInt(input[0]);

	if( choice == 1 )
		AutoLoop_initFixed( character, 1 );
	elseif( choice == 2 )
		AutoLoop_init( character, maxAllowed );
	elseif( choice == 3 )
		AutoLoop_initFixed( character, maxAllowed );
	else
		return 0;
	endif

	return CInt(AL_number);
endfunction

function KegVat_IsPotionObjtype( objtype )
	if( (objtype >= 0xdc01 and objtype <= 0xdc17) or objtype == 0xdce6 or objtype == 0x7059
		or (objtype >= 0xff19 and objtype <= 0xff90) or (objtype <= 0xff95 and objtype >= 0xff93) )
		return 1;
	endif
	return 0;
endfunction

function KegVat_DoFill( character, container )
	var ptype := CInt(GetObjProperty(container, "type"));
	var pamount := CInt(GetObjProperty(container, "amount"));
	var capacity := KegVat_Capacity(container);
	var room := capacity - pamount;
	if( room < 1 )
		SendSysMessage( character, "That's already full." );
		return;
	endif

	SendSysMessage( character, "Select a potion to fill from." );
	var targetted := Target( character, TGTOPT_CHECK_LOS );
	if( !targetted or !Accessible(character, targetted) or !ReserveItem(targetted) )
		SendSysMessage( character, "Canceled." );
		return;
	elseif( !IsInContainer(character.backpack, targetted) )
		SendSysMessage( character, "That has to be in your backpack." );
		return;
	elseif( !KegVat_IsPotionObjtype(targetted.objtype) )
		SendSysMessage( character, "That's not a potion." );
		return;
	elseif( ptype and targetted.objtype != ptype )
		SendSysMessage( character, "This already has another type of potion in it." );
		return;
	endif

	// Target()/the qty-picker gump block on player input -- re-check the
	// stack is still there and still has enough after the wait, since
	// ReserveItem alone doesn't stop it being consumed elsewhere meanwhile.
	if( !targetted or targetted.amount < 1 )
		SendSysMessage( character, "That's gone now." );
		return;
	endif

	var maxAllowed := Min( room, CInt(targetted.amount) );
	var qty := KegVat_PickQty( character, maxAllowed, "Fill" );
	if( qty < 1 )
		return;
	endif

	if( !ptype )
		SetObjProperty( container, "type", targetted.objtype );
		ptype := CInt(targetted.objtype);
	endif

	PlaySoundEffect( character, KEGVAT_SOUND_POUR );
	SubtractAmount( targetted, qty );
	CreateItemInBackpack( character, 0x0F0E, qty );
	pamount := pamount + qty;
	SetObjProperty( container, "amount", pamount );
	var kname := KegVat_ContentsName( container, ptype, CInt(GetObjProperty(container, "subtype")) );
	if( kname )
		container.name := cstr( kname + " [" + pamount + "]" );
	endif
	SendSysMessage( character, "You fill it with " + qty + "." );
	IncRevision( container );
endfunction

function KegVat_DoEmpty( character, container )
	var ptype := CInt(GetObjProperty(container, "type"));
	var pamount := CInt(GetObjProperty(container, "amount"));
	if( !ptype or pamount < 1 )
		SendSysMessage( character, "There's nothing in it to empty." );
		return;
	endif

	var bottle := FindObjtypeInContainer( character.backpack, 0x0F0E );
	if( !bottle or !ReserveItem(bottle) )
		SendSysMessage( character, "You need empty bottles." );
		return;
	endif

	var maxAllowed := Min( pamount, CInt(bottle.amount) );
	var qty := KegVat_PickQty( character, maxAllowed, "Empty" );
	if( qty < 1 )
		return;
	endif

	// The picker gump blocks on player input -- re-check the bottle stack
	// (and this container's own contents) are still what we counted.
	if( !bottle or bottle.amount < 1 )
		SendSysMessage( character, "You're out of bottles now." );
		return;
	endif
	pamount := CInt(GetObjProperty(container, "amount"));
	qty := Min( qty, Min(pamount, CInt(bottle.amount)) );
	if( qty < 1 )
		SendSysMessage( character, "There's nothing left in it to empty." );
		return;
	endif

	PlaySoundEffect( character, KEGVAT_SOUND_POUR );
	SubtractAmount( bottle, qty );
	CreateItemInBackpack( character, ptype, qty );
	pamount := pamount - qty;

	if( pamount < 1 )
		EraseObjProperty( container, "type" );
		EraseObjProperty( container, "amount" );
		EraseObjProperty( container, "subtype" );
		container.name := "a " + KegVat_BaseName(container);
		SendSysMessage( character, "It's empty now." );
	else
		SetObjProperty( container, "amount", pamount );
		var kname := KegVat_ContentsName( container, ptype, CInt(GetObjProperty(container, "subtype")) );
		if( kname )
			container.name := cstr( kname + " [" + pamount + "]" );
		endif
	endif
	SendSysMessage( character, "You fill " + qty + " bottles." );
	IncRevision( container );
endfunction

function KegVat_DoTransfer( character, container )
	var ptype := CInt(GetObjProperty(container, "type"));
	var pamount := CInt(GetObjProperty(container, "amount"));
	if( !ptype or pamount < 1 )
		SendSysMessage( character, "There's nothing in it to transfer." );
		return;
	endif

	SendSysMessage( character, "Select the Keg or Vat to transfer into." );
	var targetted := Target( character, TGTOPT_CHECK_LOS );
	if( !targetted or !Accessible(character, targetted) or !ReserveItem(targetted) )
		SendSysMessage( character, "Canceled." );
		return;
	elseif( !KegVat_IsKegOrVat(targetted.objtype) )
		SendSysMessage( character, "That's not a Keg or a Vat." );
		return;
	elseif( targetted.serial == container.serial )
		SendSysMessage( character, "You can't transfer it into itself." );
		return;
	endif

	var desttype := CInt(GetObjProperty(targetted, "type"));
	if( desttype and desttype != ptype )
		SendSysMessage( character, "That already has a different potion in it." );
		return;
	endif

	var destamount := CInt(GetObjProperty(targetted, "amount"));
	var room := KegVat_Capacity(targetted) - destamount;
	var maxAllowed := Min( pamount, room );
	var qty := KegVat_PickQty( character, maxAllowed, "Transfer" );
	if( qty < 1 )
		return;
	endif

	// The picker gump blocks on player input -- re-read both sides fresh
	// rather than trust the counts from before the wait.
	pamount := CInt(GetObjProperty(container, "amount"));
	destamount := CInt(GetObjProperty(targetted, "amount"));
	desttype := CInt(GetObjProperty(targetted, "type"));
	if( desttype and desttype != ptype )
		SendSysMessage( character, "That already has a different potion in it." );
		return;
	endif
	room := KegVat_Capacity(targetted) - destamount;
	qty := Min( qty, Min(pamount, room) );
	if( qty < 1 )
		SendSysMessage( character, "There's nothing left to transfer." );
		return;
	endif

	var psubtype := CInt(GetObjProperty(container, "subtype"));

	PlaySoundEffect( character, KEGVAT_SOUND_POUR );
	if( !desttype )
		SetObjProperty( targetted, "type", ptype );
		if( psubtype )
			SetObjProperty( targetted, "subtype", psubtype );
		endif
	endif
	destamount := destamount + qty;
	SetObjProperty( targetted, "amount", destamount );
	var destname := KegVat_ContentsName( targetted, ptype, psubtype );
	if( destname )
		targetted.name := cstr( destname + " [" + destamount + "]" );
	endif
	IncRevision( targetted );

	pamount := pamount - qty;
	if( pamount < 1 )
		EraseObjProperty( container, "type" );
		EraseObjProperty( container, "amount" );
		EraseObjProperty( container, "subtype" );
		container.name := "a " + KegVat_BaseName(container);
	else
		SetObjProperty( container, "amount", pamount );
		var srcname := KegVat_ContentsName( container, ptype, psubtype );
		if( srcname )
			container.name := cstr( srcname + " [" + pamount + "]" );
		endif
	endif
	SendSysMessage( character, "You transfer " + qty + "." );
	IncRevision( container );
endfunction

exported function KegVat_ShowOperateGump( character, container )
	if( !ReserveItem(container) )
		return;
	endif

	var ptype := CInt(GetObjProperty(container, "type"));
	var pamount := CInt(GetObjProperty(container, "amount"));
	var capacity := KegVat_Capacity(container);

	var gump := GFCreateGump( 80, 80 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9200, 300, 180 );

	if( ptype )
		var displayname := CStr(GetItemDisplayName(ptype));
		if( !displayname )
			displayname := "Unknown potion";
		endif
		GFHTMLArea( gump, 20, 15, 260, 20, "<B>" + displayname + "</B> -- " + pamount + " / " + capacity + " stored", 0, 0 );
	else
		GFHTMLArea( gump, 20, 15, 260, 20, "<B>Empty</B> -- 0 / " + capacity, 0, 0 );
	endif

	GFAddButton( gump, 20, 55, 4005, 4007, GF_CLOSE_BTN, 1 );
	GFHTMLArea( gump, 55, 55, 220, 20, "Fill from backpack", 0, 0 );
	GFAddButton( gump, 20, 90, 4005, 4007, GF_CLOSE_BTN, 2 );
	GFHTMLArea( gump, 55, 90, 220, 20, "Empty into bottles", 0, 0 );
	GFAddButton( gump, 20, 125, 4005, 4007, GF_CLOSE_BTN, 3 );
	GFHTMLArea( gump, 55, 125, 220, 20, "Transfer to another Keg/Vat", 0, 0 );

	var input := GFSendGump( character, gump );
	if( !input )
		return;
	endif
	var choice := CInt(input[0]);

	if( choice == 1 )
		KegVat_DoFill( character, container );
	elseif( choice == 2 )
		KegVat_DoEmpty( character, container );
	elseif( choice == 3 )
		KegVat_DoTransfer( character, container );
	endif
endfunction
