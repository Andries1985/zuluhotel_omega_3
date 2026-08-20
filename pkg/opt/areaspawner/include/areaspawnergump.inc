use os;
use uo;
use cfgfile;
use datafile;
use basic;

include "include/constants/gumpids";
include "include/client";

const AS_BTN_OK				:= 1;
const AS_BTN_DELETE			:= 2;
const AS_BTN_CANCEL			:= 3;
const AS_BTN_PICKAREA			:= 20;
const AS_BTN_ENTRIES			:= 21;
const AS_BTN_FULLRESPAWN		:= 22;
const AS_BTN_DELETESPAWNER		:= 23;
const AS_BTN_BROWSEGROUPS		:= 24;
const AS_BTN_SAVE			:= 25;
const AS_BTN_LOADTEMPLATE		:= 26;
const AS_BTN_PICKMINXY			:= 27;
const AS_BTN_PICKMAXXY			:= 28;
// Custom NPC editor's 4 category sub-pages (Appearance/Stats/Prots/Other) and
// the slaytype picker they open into - see ShowCustomNpc*Editor below
const AS_BTN_CATAPPEARANCE		:= 29;
const AS_BTN_CATSTATS			:= 30;
const AS_BTN_CATPROTS			:= 31;
const AS_BTN_CATOTHER			:= 32;
const AS_BTN_PICKSLAYTYPE		:= 33;
const AS_BTN_SLAYTYPE_BASE		:= 400;	// one option per slaytype, id = base + index (1-based) into AreaSpawnerSlayTypes()
const AS_BTN_ROW_DELETE_BASE		:= 200;	// per-row Delete on the list gump, id = base + row slot (1-8)
const AS_BTN_ADD			:= 9001;
const AS_BTN_PREV			:= 9002;
const AS_BTN_NEXT			:= 9003;
const AS_BTN_CLOSE			:= 9004;

const AS_CHK_ISGROUP			:= 52;
const AS_CHK_ALLOWWATER		:= 53;
// Generic reusable checkbox slots for the 4 custom-npc category sub-pages -
// each page is its own separate modal SendDialogGump call, so ids are safe to
// reuse across pages (G1/G2/G3 mean Gender/LesserBoss/SuperBoss on the
// Appearance page, but FreeAction/Looter on Prots/Other - never both at once)
const AS_CHK_G1				:= 54;
const AS_CHK_G2				:= 55;
const AS_CHK_G3				:= 56;
const AS_CHK_G4				:= 57;
const AS_CHK_ROW_BASE			:= 60;	// per-row Enabled toggle on the list gump, id = base + row slot (1-8)

const AS_TXT_NAME			:= 100;
const AS_TXT_MINX			:= 101;
const AS_TXT_MAXX			:= 102;
const AS_TXT_MINY			:= 103;
const AS_TXT_MAXY			:= 104;
const AS_TXT_REF			:= 105;
const AS_TXT_MIN			:= 106;
const AS_TXT_MAX			:= 107;
const AS_TXT_FREQ			:= 108;
const AS_TXT_CNAME			:= 109;
const AS_TXT_CSTR			:= 110;
const AS_TXT_CDEX			:= 111;
const AS_TXT_CINT			:= 112;
const AS_TXT_CHITS			:= 113;
const AS_TXT_CSTAM			:= 114;
const AS_TXT_CMANA			:= 115;
const AS_TXT_CCOLOR			:= 116;
const AS_TXT_CRUNSPEED			:= 117;
// Generic reusable textentry slots for the 4 custom-npc category sub-pages -
// same reuse-across-separate-modal-pages reasoning as AS_CHK_G1-3 above.
// Prots is the densest page (7 elemental + 3 alt-prot fields), so 10 slots.
const AS_TXT_G1				:= 150;
const AS_TXT_G2				:= 151;
const AS_TXT_G3				:= 152;
const AS_TXT_G4				:= 153;
const AS_TXT_G5				:= 154;
const AS_TXT_G6				:= 155;
const AS_TXT_G7				:= 156;
const AS_TXT_G8				:= 157;
const AS_TXT_G9				:= 158;
const AS_TXT_G10			:= 159;

const AS_GFX_CHECKBOX_OFF		:= 0x16D3;
const AS_GFX_CHECKBOX_ON		:= 0x16D5;
const AS_GFX_ROWBUTTON_0		:= 0x647;
const AS_GFX_ROWBUTTON_1		:= 0x648;
const AS_GFX_DELETEBUTTON		:= 0x644;


function AreaSpawnerTypeLabel(kind)
	case( kind )
		"area":		return "Area";
		"individual":	return "Individual Spawn";
		"customnpc":	return "Custom NPC";
		"item":		return "Item";
		"chest":	return "Chest";
		default:	return kind;
	endcase
endfunction


// Regular vs Event spawners live in separate datafiles (different name prefix)
// so admins can wipe/browse event content without it mixing into the main list.
// Only special-cased "area" (bare "eventareaspawner"/"areaspawner", no kind
// suffix) before - every other kind silently fell back to the generic "Area
// Spawner" label, so a Chest/Item/Custom NPC/Individual spawner's edit gump
// title never actually said which kind it was, and event-side non-area kinds
// lost the "Event" qualifier too. AreaSpawnerKindFromPrefix/TypeLabel/
// IsEventPrefix already resolve both correctly (areaspawner.inc), so just
// build the label from those instead of hand-special-casing one prefix.
function AreaSpawnerPrefixLabel(prefix)
	var kind := AreaSpawnerKindFromPrefix(prefix);
	var label := AreaSpawnerTypeLabel(kind)+" Spawner";
	if( AreaSpawnerIsEventPrefix(prefix) )
		label := "Event "+label;
	endif
	return label;
endfunction


function ShowSpawnerTypePicker(who, title)

	var types := array{ "area", "individual", "customnpc", "item", "chest" };
	var labels := array{
		"Area (Group spawn in area)",
		"Individual Spawn (Single Spawn at single location)",
		"Custom NPC (Single Spawn at single location)",
		"Item (Single Spawn at single location)",
		"Chest (Group spawn in area)" };

	var layout := array{ "page 0",
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 420 "+CStr(40 + types.size()*30) };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( title );
	tid := tid + 1;

	var i;
	var y := 45;
	for( i := 1; i <= types.size(); i := i + 1 )
		layout.append( "text 55 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( labels[i] );
		tid := tid + 1;
		layout.append( "button 20 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(i) );
		y := y + 30;
	endfor

	// no Cancel button / NO_DISPOSE - right-click closes this gump instead
	var keyret := SendDialogGump( who, layout, data );
	if( !keyret )
		return "";
	endif

	for( i := 1; i <= types.size(); i := i + 1 )
		if( keyret[i] )
			return types[i];
		endif
	endfor

	return "";

endfunction


function ShowRealmPicker(who, title := "Area Spawner - Choose Realm")

	var realms := AreaSpawnerRealms();
	var labels := array{ "Britannia", "Britannia Alt", "Malas", "Tokuno", "Ilshenar", "Termur" };

	var layout := array{ "page 0",
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 260 "+CStr(40 + realms.size()*30) };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( title );
	tid := tid + 1;

	var i;
	var y := 45;
	for( i := 1; i <= realms.size(); i := i + 1 )
		layout.append( "text 55 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( labels[i] );
		tid := tid + 1;
		layout.append( "button 20 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(i) );
		y := y + 30;
	endfor

	// no Cancel button / NO_DISPOSE - right-click closes this gump instead
	var keyret := SendDialogGump( who, layout, data );
	if( !keyret )
		return "";
	endif

	for( i := 1; i <= realms.size(); i := i + 1 )
		if( keyret[i] )
			return realms[i];
		endif
	endfor

	return "";

endfunction


function ShowSpawnerList(who, realm, prefix := "areaspawner", title := "Area Spawners", kind := "area")

	var page := 1;
	var perpage := 8;

	while(1)
		var names := GetAreaSpawnerNames(realm, prefix);

		var totalpages := 1;
		if( names.size() )
			totalpages := CInt((names.size()-1)/perpage) + 1;
		endif
		if( page > totalpages )
			page := totalpages;
		endif

		var startidx := ((page-1)*perpage) + 1;
		var endidx := startidx + perpage - 1;
		if( endidx > names.size() )
			endidx := names.size();
		endif

		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 520 "+CStr(170 + perpage*40) };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( title+": "+realm+" (page "+CStr(page)+"/"+CStr(totalpages)+")" );
		tid := tid + 1;

		// ADD graphic already renders its own text, no extra label needed
		layout.append( "button 20 45 "+CStr(GUMPID_BUTTON_ADD_0)+" "+CStr(GUMPID_BUTTON_ADD_1)+" 1 0 "+CStr(AS_BTN_ADD) );
		layout.append( "button 190 45 "+CStr(GUMPID_BUTTON_PREVIOUS_0)+" "+CStr(GUMPID_BUTTON_PREVIOUS_1)+" 1 0 "+CStr(AS_BTN_PREV) );
		layout.append( "button 230 45 "+CStr(GUMPID_BUTTON_NEXT_0)+" "+CStr(GUMPID_BUTTON_NEXT_1)+" 1 0 "+CStr(AS_BTN_NEXT) );

		var rownames := array{};
		var rowdrawn := array{};	// isenabled as actually drawn this render, for detecting a genuine toggle below
		var y := 85;
		var slot := 0;
		var idx;
		for( idx := startidx; idx <= endidx; idx := idx + 1 )
			slot := slot + 1;
			var sname := names[idx];
			var record := LoadAreaSpawner(realm, sname, prefix);
			var status := "off";
			var pop := "0/0";
			var isenabled := 0;
			var rowcolor := Cint(COLOR_BLUE_LIGHT);
			var reasonline := "";
			var reasoncolor := Cint(COLOR_RED);
			if( record )
				if( record.enabled )
					status := "ON";
					isenabled := 1;
					// still enabled/working overall, but one entry gave up
					// after repeated placement failures (see
					// AREASPAWNER_ENTRY_FAIL_THRESHOLD) - flag it without
					// treating the whole spawner as down, since e.g. a
					// walrus entry can be filling fine in the same box a
					// land-only horse entry can't place in
					var pausedreason := AreaSpawnerFindPausedReason(record);
					if( pausedreason )
						status := "ON (paused)";
						rowcolor := Cint(COLOR_YELLOW);
						reasoncolor := Cint(COLOR_YELLOW);
						reasonline := pausedreason;
						if( Len(reasonline) > 55 )
							reasonline := SubStr(reasonline, 1, 55)+"...";
						endif
					endif
				elseif( record.disabled_reason )
					// flags a spawner the code itself shut off (bad box, etc.)
					// so it doesn't just look like a normal manual "off" - kept
					// short since it shares the line with name/pop; the full
					// reason gets its own line below, with the full row width
					status := "OFF (AUTO)";
					rowcolor := Cint(COLOR_RED);
					reasonline := record.disabled_reason;
					if( Len(reasonline) > 55 )
						reasonline := SubStr(reasonline, 1, 55)+"...";
					endif
				endif
				pop := AreaSpawnerPopulationSummary(record);
			endif

			// column order: enabled checkbox, delete, select-row, text - keeps
			// the text column clear of anything anchored to the panel's right
			// edge, so it has room to breathe instead of getting clipped there
			layout.append( "checkbox 20 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(isenabled)+" "+CStr(AS_CHK_ROW_BASE+slot) );
			layout.append( "button 50 "+CStr(y-2)+" "+CStr(AS_GFX_DELETEBUTTON)+" "+CStr(AS_GFX_DELETEBUTTON)+" 1 0 "+CStr(AS_BTN_ROW_DELETE_BASE+slot) );
			layout.append( "button 80 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(slot) );
			layout.append( "text 115 "+CStr(y)+" "+CStr(rowcolor)+" "+CStr(tid) );
			data.append( sname+"  ["+status+"]  pop:"+pop );
			tid := tid + 1;

			// full-width second line for the reason, instead of squeezing it
			// onto the name/pop line where it kept running off the panel
			if( reasonline )
				layout.append( "text 115 "+CStr(y+14)+" "+CStr(reasoncolor)+" "+CStr(tid) );
				data.append( reasonline );
				tid := tid + 1;
			endif

			rownames.append(sname);
			rowdrawn.append(isenabled);
			y := y + 40;
		endfor

		// OKAY/CANCEL graphics already render their own text, no extra labels needed
		layout.append( "button 20 "+CStr(y+15)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_SAVE) );
		layout.append( "button 140 "+CStr(y+15)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CLOSE) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CLOSE] )
			return;	// Cancel (or connection loss) discards any checkbox changes too
		endif

		// Enabled checkboxes only commit on Save (or any button that keeps
		// working in this gump - Add/Prev/Next/row-select/row-delete), never
		// on Cancel, which already returned above. Only counts as a toggle if
		// it differs from what was actually drawn this render, NOT from a
		// fresh reload - a background script (full respawn auto-disabling on
		// a bad template, etc.) can change the real state while this gump
		// sits open, and the checkbox the client submits back is whatever
		// was drawn, not the admin's intent.
		var s;
		for( s := 1; s <= rownames.size(); s := s + 1 )
			var newenabled := Cint(keyret[AS_CHK_ROW_BASE+s]);
			if( newenabled == rowdrawn[s] )
				continue;
			endif
			var toggled := LoadAreaSpawner(realm, rownames[s], prefix);
			if( toggled && Cint(toggled.enabled) != newenabled )
				toggled.enabled := newenabled;
				toggled.disabled_reason := "";	// manual toggle overrides any auto-disable reason
				SaveAreaSpawner(realm, rownames[s], toggled, who, prefix);
				AreaSpawnerDebug("'"+rownames[s]+"' ("+realm+") enabled set to "+CStr(newenabled)+" via list toggle ["+prefix+"]");
				// turning it on is exactly when an idle, event-driven spawner
				// needs a kick - nothing else would start filling it otherwise.
				// Also clear any stale cooldown left over from failed attempts
				// before it was last disabled, or the kick can silently no-op.
				if( newenabled )
					AreaSpawnerResetEntryCooldowns(toggled);
					AreaSpawnerKickAllEntries(realm, rownames[s], prefix, toggled, 0);
				endif
			endif
		endfor

		if( keyret[AS_BTN_SAVE] )
			continue;	// commits above, then redraw this same gump - only Cancel actually leaves
		endif

		var delslot;
		var deletedone := 0;
		for( delslot := 1; delslot <= rownames.size(); delslot := delslot + 1 )
			if( keyret[AS_BTN_ROW_DELETE_BASE+delslot] )
				if( ShowConfirm(who, "Delete area spawner '"+rownames[delslot]+"'? This despawns everything it's tracking.") )
					DeleteAreaSpawner(realm, rownames[delslot], prefix);
					SendSysMessage(who, "Deleted '"+rownames[delslot]+"'.");
				endif
				deletedone := 1;
				break;
			endif
		endfor
		if( deletedone )
			continue;
		endif

		if( keyret[AS_BTN_ADD] )
			var newname := ShowNamePrompt(who);
			if( newname )
				if( CreateAreaSpawner(realm, newname, who, prefix) )
					ShowSpawnerEdit(who, realm, newname, prefix, kind);
				else
					SendSysMessage(who, "A spawner named '"+newname+"' already exists (or couldn't be created).");
				endif
			endif
			continue;
		endif

		if( keyret[AS_BTN_PREV] )
			if( page > 1 )
				page := page - 1;
			endif
			continue;
		endif

		if( keyret[AS_BTN_NEXT] )
			if( page < totalpages )
				page := page + 1;
			endif
			continue;
		endif

		for( s := 1; s <= rownames.size(); s := s + 1 )
			if( keyret[s] )
				ShowSpawnerEdit(who, realm, rownames[s], prefix, kind);
				break;
			endif
		endfor
	endwhile

endfunction


function ShowNamePrompt(who)

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 300 120",
		"text 20 15 "+CStr(Cint(COLOR_YELLOW))+" 0",
		"textentry 20 45 250 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_NAME)+" 1",
		// OKAY/CANCEL graphics already render their own text, no extra labels needed
		"button 20 80 "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK),
		"button 150 80 "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) };
	var data := array{ "New Area Spawner - Enter a Name", "" };

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || keyret[AS_BTN_CANCEL] || !keyret[AS_BTN_OK] )
		return "";
	endif

	var name := keyret[AS_TXT_NAME] - (CStr(AS_TXT_NAME)+": ");
	if( !name )
		return "";
	endif

	return name;

endfunction


// kind drives two things here: whether the location UI is a roamed box (Pick
// Area + minx/maxx/miny/maxy, area/chest) or a single point (Pick Location +
// one X/Y pair, individual/item/customnpc - stored in the same minx/miny
// fields with maxx:=minx/maxy:=miny so the rest of the placement/tick code
// doesn't need to know the difference), and whether Allow Water Spawns is
// shown at all (items don't have a MoveMode, so it's meaningless for item/chest).
function ShowSpawnerEdit(who, realm, name, prefix := "areaspawner", kind := "area")

	var usesbox := AreaSpawnerKindUsesBox(kind);
	var showswater := (kind != "item" && kind != "chest");

	var done := 0;
	while( !done )
		var record := LoadAreaSpawner(realm, name, prefix);
		if( !record )
			SendSysMessage(who, "That area spawner no longer exists.");
			return;
		endif

		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 420 455" };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( AreaSpawnerPrefixLabel(prefix)+": "+name+" ("+realm+") - use the list to toggle Enabled" );
		tid := tid + 1;

		if( usesbox )
			layout.append( "text 20 50 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( "Last picked area:" );
			tid := tid + 1;
			layout.append( "text 150 50 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( record.area_label ? record.area_label : "(custom box)" );
			tid := tid + 1;
			layout.append( "button 300 48 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_PICKAREA) );
			layout.append( "text 325 50 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( "Pick Area" );
			tid := tid + 1;

			// per-corner target pickers - fills just minX/minY or just maxX/
			// maxY from wherever the admin targets, so a custom (non-Pick
			// Area) box can be walked out and clicked instead of typed
			layout.append( "button 20 83 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_PICKMINXY) );
			layout.append( "text 55 85 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( "Pick Min X/Y" );
			tid := tid + 1;
			layout.append( "button 220 83 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_PICKMAXXY) );
			layout.append( "text 255 85 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( "Pick Max X/Y" );
			tid := tid + 1;

			layout.append( "text 20 120 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( "Box minX/maxX/minY/maxY (Pick fills these in, or edit directly):" );
			tid := tid + 1;

			layout.append( "textentry 20 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MINX)+" "+CStr(tid) );
			data.append( CStr(record.minx) );
			tid := tid + 1;
			layout.append( "textentry 90 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MAXX)+" "+CStr(tid) );
			data.append( CStr(record.maxx) );
			tid := tid + 1;
			layout.append( "textentry 160 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MINY)+" "+CStr(tid) );
			data.append( CStr(record.miny) );
			tid := tid + 1;
			layout.append( "textentry 230 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MAXY)+" "+CStr(tid) );
			data.append( CStr(record.maxy) );
			tid := tid + 1;
		else
			layout.append( "text 20 50 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( "Location:" );
			tid := tid + 1;
			layout.append( "text 150 50 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( (record.minx==0 && record.miny==0) ? "(not set)" : CStr(record.minx)+","+CStr(record.miny) );
			tid := tid + 1;
			layout.append( "button 300 48 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_PICKAREA) );
			layout.append( "text 325 50 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( "Pick Location" );
			tid := tid + 1;

			layout.append( "text 20 120 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( "X / Y (Pick Location targets where you're standing, or edit directly):" );
			tid := tid + 1;

			layout.append( "textentry 20 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MINX)+" "+CStr(tid) );
			data.append( CStr(record.minx) );
			tid := tid + 1;
			layout.append( "textentry 90 145 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MINY)+" "+CStr(tid) );
			data.append( CStr(record.miny) );
			tid := tid + 1;
		endif

		layout.append( "text 20 180 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Population:" );
		tid := tid + 1;
		layout.append( "text 220 180 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
		data.append( AreaSpawnerPopulationSummary(record) );
		tid := tid + 1;

		// Off by default - GetStandingHeight land-only placement, same as
		// spawnpoint, so a mixed-terrain box never drops a swim-capable
		// template in open water just because its MoveMode allows it. Only
		// affects THIS spawner's placement, applies to every group/npc entry
		// it has - not a per-entry setting. Not shown at all for item/chest
		// kinds, which place plain items with no MoveMode concept.
		if( showswater )
			layout.append( "checkbox 20 203 "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(record.allow_water?1:0)+" "+CStr(AS_CHK_ALLOWWATER) );
			layout.append( "text 55 205 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( "Allow Water Spawns (movemode-aware placement)" );
			tid := tid + 1;
		endif

		// OKAY/APPLY/DELETE graphics are wider than they look and were colliding
		// with an adjacent label, so the label goes on its own line above the
		// button instead of beside it - avoids the overlap regardless of the
		// graphic's actual pixel width.
		layout.append( "text 20 240 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
		data.append( "Entries / Edit Groups & NPCs" );
		tid := tid + 1;
		layout.append( "button 20 263 "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_ENTRIES) );

		layout.append( "text 20 298 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( "Full Respawn (despawn + refill to max)" );
		tid := tid + 1;
		layout.append( "button 20 321 "+CStr(GUMPID_BUTTON_APPLY_0)+" "+CStr(GUMPID_BUTTON_APPLY_1)+" 1 0 "+CStr(AS_BTN_FULLRESPAWN) );

		layout.append( "text 20 356 "+CStr(Cint(COLOR_RED))+" "+CStr(tid) );
		data.append( "Delete Spawner" );
		tid := tid + 1;
		layout.append( "button 20 379 "+CStr(GUMPID_BUTTON_DELETE_0)+" "+CStr(GUMPID_BUTTON_DELETE_1)+" 1 0 "+CStr(AS_BTN_DELETESPAWNER) );

		// OKAY/CANCEL graphics already render their own text, no extra labels needed
		layout.append( "button 20 417 "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_SAVE) );
		layout.append( "button 140 417 "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CANCEL] )
			return;
		endif

		if( usesbox )
			record.minx := Cint(keyret[AS_TXT_MINX] - (CStr(AS_TXT_MINX)+": "));
			record.maxx := Cint(keyret[AS_TXT_MAXX] - (CStr(AS_TXT_MAXX)+": "));
			record.miny := Cint(keyret[AS_TXT_MINY] - (CStr(AS_TXT_MINY)+": "));
			record.maxy := Cint(keyret[AS_TXT_MAXY] - (CStr(AS_TXT_MAXY)+": "));
		else
			var px := Cint(keyret[AS_TXT_MINX] - (CStr(AS_TXT_MINX)+": "));
			var py := Cint(keyret[AS_TXT_MINY] - (CStr(AS_TXT_MINY)+": "));
			record.minx := px;
			record.maxx := px;
			record.miny := py;
			record.maxy := py;
		endif
		if( showswater )
			record.allow_water := Cint(keyret[AS_CHK_ALLOWWATER]);
		endif

		SaveAreaSpawner(realm, name, record, who, prefix);

		if( keyret[AS_BTN_PICKAREA] )
			if( usesbox )
				var picked := ShowAreaPicker(who, realm);
				if( picked )
					var picked_box := LookupAreaCfgBox(realm, picked);
					if( picked_box )
						record.minx := picked_box.minx;
						record.maxx := picked_box.maxx;
						record.miny := picked_box.miny;
						record.maxy := picked_box.maxy;
						record.area_label := picked_box.label ? picked_box.label : picked;
						SaveAreaSpawner(realm, name, record, who, prefix);
					endif
				endif
			else
				SendSysMessage(who, "Target the ground where this spawner should place its NPC/item.");
				var picked_loc := TargetCoordinates(who);
				if( picked_loc )
					record.minx := CInt(picked_loc.x);
					record.maxx := CInt(picked_loc.x);
					record.miny := CInt(picked_loc.y);
					record.maxy := CInt(picked_loc.y);
					SaveAreaSpawner(realm, name, record, who, prefix);
				else
					SendSysMessage(who, "Cancelled.");
				endif
			endif
		elseif( usesbox && keyret[AS_BTN_PICKMINXY] )
			SendSysMessage(who, "Target the MIN X/Y corner.");
			var picked_min := TargetCoordinates(who);
			if( picked_min )
				record.minx := CInt(picked_min.x);
				record.miny := CInt(picked_min.y);
				record.area_label := "";	// no longer matches whatever Pick Area last set
				SaveAreaSpawner(realm, name, record, who, prefix);
			else
				SendSysMessage(who, "Cancelled.");
			endif
		elseif( usesbox && keyret[AS_BTN_PICKMAXXY] )
			SendSysMessage(who, "Target the MAX X/Y corner.");
			var picked_max := TargetCoordinates(who);
			if( picked_max )
				record.maxx := CInt(picked_max.x);
				record.maxy := CInt(picked_max.y);
				record.area_label := "";
				SaveAreaSpawner(realm, name, record, who, prefix);
			else
				SendSysMessage(who, "Cancelled.");
			endif
		elseif( keyret[AS_BTN_ENTRIES] )
			ShowEntriesList(who, realm, name, prefix, kind);
		elseif( keyret[AS_BTN_FULLRESPAWN] )
			Start_Script( ":areaspawner:areaspawnerfullrespawn", array{realm,name,prefix} );
			SendSysMessage(who, "Full respawn triggered for '"+name+"'.");
			// close instead of looping back to reopen this same gump - full
			// respawn runs async, so re-loading immediately would show stale
			// data, and this gump unconditionally re-saves the box fields on
			// every submit, which would silently overwrite whatever the
			// async respawn saves (including any auto-disable it applies)
			// if the admin interacted with the stale reopened gump at all
			done := 1;
		elseif( keyret[AS_BTN_DELETESPAWNER] )
			if( ShowConfirm(who, "Delete area spawner '"+name+"'? This despawns everything it's tracking.") )
				// synchronous, not Start_Script - the list gump we return to
				// reloads the name list immediately, so this has to actually be
				// done by the time we get there or it still shows up (stale)
				DeleteAreaSpawner(realm, name, prefix);
				SendSysMessage(who, "Deleted '"+name+"'.");
				done := 1;
			endif
		elseif( keyret[AS_BTN_SAVE] )
			// a box edit can turn a previously-stuck spawner (invalid/zero box,
			// or just short of target) into one that can place again - clear
			// any stale cooldown left over from failed attempts and kick every
			// entry now instead of waiting on its own next scheduled tick.
			// AreaSpawnerKickAllEntries already no-ops for anything at target
			// or already mid-refill.
			if( record.enabled && record.entries.size() )
				AreaSpawnerResetEntryCooldowns(record);
				AreaSpawnerKickAllEntries(realm, name, prefix, record, 0);
			endif
			SendSysMessage(who, "Saved '"+name+"'.");
			done := 1;
		else
			done := 1;
		endif
	endwhile

endfunction


function ShowAreaPicker(who, realm)

	var lines := GetRealmAreaLines(realm);
	if( lines == error || !lines )
		SendSysMessage(who, "No areas configured for realm '"+realm+"'.");
		return "";
	endif

	var infos := array{};
	foreach line in lines
		var info := ParseAreaLine(line);
		if( info != error )
			infos.append(info);
		endif
	endforeach

	var page := 1;
	var perpage := 10;

	while(1)
		var totalpages := 1;
		if( infos.size() )
			totalpages := CInt((infos.size()-1)/perpage) + 1;
		endif
		if( page > totalpages )
			page := totalpages;
		endif

		var startidx := ((page-1)*perpage) + 1;
		var endidx := startidx + perpage - 1;
		if( endidx > infos.size() )
			endidx := infos.size();
		endif

		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 420 "+CStr(80 + perpage*24) };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( "Pick Area ("+realm+") page "+CStr(page)+"/"+CStr(totalpages) );
		tid := tid + 1;

		var rowids := array{};
		var y := 50;
		var slot := 0;
		var idx;
		for( idx := startidx; idx <= endidx; idx := idx + 1 )
			slot := slot + 1;
			var info := infos[idx];
			layout.append( "text 55 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( info.area_id+"  -  "+info.name );
			tid := tid + 1;
			layout.append( "button 20 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(slot) );
			rowids.append(info.area_id);
			y := y + 24;
		endfor

		layout.append( "button 190 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_PREVIOUS_0)+" "+CStr(GUMPID_BUTTON_PREVIOUS_1)+" 1 0 "+CStr(AS_BTN_PREV) );
		layout.append( "button 230 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_NEXT_0)+" "+CStr(GUMPID_BUTTON_NEXT_1)+" 1 0 "+CStr(AS_BTN_NEXT) );
		// CANCEL graphic already renders its own "Cancel" text, no extra label needed
		layout.append( "button 300 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CLOSE) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CLOSE] )
			return "";
		endif

		if( keyret[AS_BTN_PREV] )
			if( page > 1 )
				page := page - 1;
			endif
			continue;
		endif

		if( keyret[AS_BTN_NEXT] )
			if( page < totalpages )
				page := page + 1;
			endif
			continue;
		endif

		var s;
		for( s := 1; s <= rowids.size(); s := s + 1 )
			if( keyret[s] )
				return rowids[s];
			endif
		endfor
	endwhile

endfunction


function ShowEntriesList(who, realm, name, prefix := "areaspawner", kind := "area")

	var page := 1;
	var perpage := 8;
	// only "area" (npc groups) and "chest" (curated chest templates) have a
	// reference browser to open - individual/item entries are plain typed
	// npc/item template names, nothing to browse
	var showsbrowse := (kind == "area" || kind == "chest");
	var browselabel := (kind == "chest") ? "Chests" : "Groups";

	while(1)
		var record := LoadAreaSpawner(realm, name, prefix);
		if( !record )
			return;
		endif

		var totalpages := 1;
		if( record.entries.size() )
			totalpages := CInt((record.entries.size()-1)/perpage) + 1;
		endif
		if( page > totalpages )
			page := totalpages;
		endif

		var startidx := ((page-1)*perpage) + 1;
		var endidx := startidx + perpage - 1;
		if( endidx > record.entries.size() )
			endidx := record.entries.size();
		endif

		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 460 "+CStr(90 + perpage*40) };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( "Entries: "+name+" (page "+CStr(page)+"/"+CStr(totalpages)+")" );
		tid := tid + 1;

		// opens a standalone reference gump (its own script/thread) so it can
		// stay on screen while this Entries gump keeps working
		if( showsbrowse )
			layout.append( "button 360 10 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_BROWSEGROUPS) );
			layout.append( "text 395 13 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( browselabel );
			tid := tid + 1;
		endif

		var rowids := array{};
		var y := 50;
		var slot := 0;
		var idx;
		for( idx := startidx; idx <= endidx; idx := idx + 1 )
			slot := slot + 1;
			var entry := record.entries[idx];
			var entrylabel := entry.source+":"+CStr(entry.ref);
			// customnpc's ref is just the scaffold template - the overridden
			// name (if the admin set one) is more useful to see at a glance
			if( entry.source == "customnpc" && entry.overrides && entry.overrides.name )
				entrylabel := "customnpc:"+entry.overrides.name+" ("+CStr(entry.ref)+")";
			endif
			var summary := entrylabel+"  "+CStr(entry.min)+"-"+CStr(entry.max)+" @"+CStr(entry.frequency/60)+"m  ("+CStr(entry.serials.size())+"/"+CStr(entry.target)+")";

			layout.append( "text 55 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
			data.append( summary );
			tid := tid + 1;
			layout.append( "button 20 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(slot) );

			// points at exactly which entry is the problem, instead of just
			// the spawner-level reason on the list gump one screen up
			var entryissue := "";
			var issuecolor := Cint(COLOR_RED);
			if( entry.config_error )
				entryissue := entry.config_error;
			elseif( entry.paused_reason )
				entryissue := entry.paused_reason;
				issuecolor := Cint(COLOR_YELLOW);
			endif
			if( entryissue )
				if( Len(entryissue) > 60 )
					entryissue := SubStr(entryissue, 1, 60)+"...";
				endif
				layout.append( "text 55 "+CStr(y+14)+" "+CStr(issuecolor)+" "+CStr(tid) );
				data.append( entryissue );
				tid := tid + 1;
			endif

			rowids.append(entry.id);
			y := y + 40;
		endfor

		// ADD graphic already renders its own text, no extra label needed
		layout.append( "button 20 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_ADD_0)+" "+CStr(GUMPID_BUTTON_ADD_1)+" 1 0 "+CStr(AS_BTN_ADD) );

		layout.append( "button 190 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_PREVIOUS_0)+" "+CStr(GUMPID_BUTTON_PREVIOUS_1)+" 1 0 "+CStr(AS_BTN_PREV) );
		layout.append( "button 230 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_NEXT_0)+" "+CStr(GUMPID_BUTTON_NEXT_1)+" 1 0 "+CStr(AS_BTN_NEXT) );
		// CANCEL graphic already renders its own "Cancel" text, no extra label needed
		layout.append( "button 300 "+CStr(y+10)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CLOSE) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CLOSE] )
			return;
		endif

		if( keyret[AS_BTN_ADD] )
			if( kind == "customnpc" )
				ShowCustomNpcEntryEditor(who, realm, name, 0, prefix);
			else
				ShowEntryEditor(who, realm, name, 0, prefix, kind);
			endif
			continue;
		endif

		if( showsbrowse && keyret[AS_BTN_BROWSEGROUPS] )
			if( kind == "chest" )
				if( AreaSpawnerIsEventPrefix(prefix) )
					Start_Script( ":areaspawner:eventareaspawnerchestbrowser", who );
				else
					Start_Script( ":areaspawner:areaspawnerchestbrowser", who );
				endif
			else
				Start_Script( ":areaspawner:areaspawnergroupbrowser", who );
			endif
			continue;
		endif

		if( keyret[AS_BTN_PREV] )
			if( page > 1 )
				page := page - 1;
			endif
			continue;
		endif

		if( keyret[AS_BTN_NEXT] )
			if( page < totalpages )
				page := page + 1;
			endif
			continue;
		endif

		var s;
		for( s := 1; s <= rowids.size(); s := s + 1 )
			if( keyret[s] )
				if( kind == "customnpc" )
					ShowCustomNpcEntryEditor(who, realm, name, rowids[s], prefix);
				else
					ShowEntryEditor(who, realm, name, rowids[s], prefix, kind);
				endif
				break;
			endif
		endfor
	endwhile

endfunction


function ShowEntryEditor(who, realm, name, entry_id, prefix := "areaspawner", kind := "area")

	var record := LoadAreaSpawner(realm, name, prefix);
	if( !record )
		return;
	endif

	// default source per kind - only "area" lets the admin choose group vs
	// npc, every other kind is locked to one source
	var source := "group";
	case( kind )
		"individual":	source := "npc";
		"item":		source := "item";
		"chest":	source := "chestgroup";
	endcase
	var ref := "";
	var emin := 1;
	var emax := 1;
	var efreq := 5;	// minutes - shown/entered in minutes, stored as seconds
	var isnew := 1;

	if( entry_id )
		var idx := FindAreaSpawnerEntryIndex(record, entry_id);
		if( idx )
			var entry := record.entries[idx];
			source := entry.source;
			ref := CStr(entry.ref);
			emin := entry.min;
			emax := entry.max;
			efreq := entry.frequency/60;
			isnew := 0;
		endif
	endif

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 340 330" };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( isnew ? "New Entry" : "Edit Entry" );
	tid := tid + 1;

	if( kind == "area" )
		layout.append( "text 20 50 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Group (checked) / NPC template (unchecked):" );
		tid := tid + 1;
		layout.append( "checkbox 20 80 "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(source=="group"?1:0)+" "+CStr(AS_CHK_ISGROUP) );

		layout.append( "text 20 110 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Group# or NPC Template:" );
		tid := tid + 1;
		layout.append( "textentry 200 108 100 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_REF)+" "+CStr(tid) );
		data.append( ref );
		tid := tid + 1;
	else
		var reflabel := "NPC Template:";
		if( kind == "item" )
			reflabel := "Item Template:";
		elseif( kind == "chest" )
			reflabel := "Chest # (see Chests browser):";
		endif
		layout.append( "text 20 60 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( reflabel );
		tid := tid + 1;
		layout.append( "textentry 200 58 100 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_REF)+" "+CStr(tid) );
		data.append( ref );
		tid := tid + 1;
	endif

	layout.append( "text 20 140 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Min:" );
	tid := tid + 1;
	layout.append( "textentry 200 138 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MIN)+" "+CStr(tid) );
	data.append( CStr(emin) );
	tid := tid + 1;

	layout.append( "text 20 170 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Max:" );
	tid := tid + 1;
	layout.append( "textentry 200 168 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MAX)+" "+CStr(tid) );
	data.append( CStr(emax) );
	tid := tid + 1;

	layout.append( "text 20 200 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Frequency (mins):" );
	tid := tid + 1;
	layout.append( "textentry 200 198 60 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_FREQ)+" "+CStr(tid) );
	data.append( CStr(efreq) );
	tid := tid + 1;

	// OKAY/DELETE/CANCEL graphics already render their own text, no extra
	// labels needed (a plain "Delete" label here would just repeat what the
	// DELETE graphic already says, unlike the more specific "Delete Spawner"
	// label used one gump up).
	layout.append( "button 20 250 "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
	layout.append( "button 140 250 "+CStr(GUMPID_BUTTON_DELETE_0)+" "+CStr(GUMPID_BUTTON_DELETE_1)+" 1 0 "+CStr(AS_BTN_DELETE) );
	layout.append( "button 260 250 "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || keyret[AS_BTN_CANCEL] )
		return;
	endif

	if( keyret[AS_BTN_DELETE] && !isnew )
		if( ShowConfirm(who, "Remove this entry? Any mobs it spawned will be despawned.") )
			record := RemoveAreaSpawnerEntryAndDespawn(record, entry_id);
			SaveAreaSpawner(realm, name, record, who, prefix);
		endif
		return;
	endif

	if( !keyret[AS_BTN_OK] )
		return;
	endif

	var newsource := source;
	if( kind == "area" )
		newsource := keyret[AS_CHK_ISGROUP] ? "group" : "npc";
	endif
	var newref := keyret[AS_TXT_REF] - (CStr(AS_TXT_REF)+": ");
	if( newsource == "group" || newsource == "chestgroup" )
		newref := CInt(newref);
	endif

	// reject at save time instead of letting a bad template silently
	// auto-disable the whole spawner on its first tick - matches
	// pkg/opt/spawnpoint's IsValidTemplate() gate
	if( newsource == "item" && !ResolveItemDescObjtype(newref) )
		SendSysMessage(who, "'"+CStr(newref)+"' is not a valid item template.");
		return;
	endif
	if( newsource == "npc" && !FindConfigElem(ReadConfigFile(CFGFILE_NPCS), CStr(newref)) )
		SendSysMessage(who, "'"+CStr(newref)+"' is not a valid npc template.");
		return;
	endif
	if( newsource == "chestgroup" )
		var chestcfgname := AreaSpawnerIsEventPrefix(prefix) ? AREASPAWNER_EVENT_CHESTS_CFG : AREASPAWNER_CHESTS_CFG;
		var chestcfgcheck := ReadConfigFile(chestcfgname);
		if( !chestcfgcheck || !chestcfgcheck[CInt(newref)] )
			SendSysMessage(who, "Chest #"+CStr(newref)+" is not in the Chests list.");
			return;
		endif
	endif

	var newmin := Cint(keyret[AS_TXT_MIN] - (CStr(AS_TXT_MIN)+": "));
	var newmax := Cint(keyret[AS_TXT_MAX] - (CStr(AS_TXT_MAX)+": "));
	var newfreq := Cint(keyret[AS_TXT_FREQ] - (CStr(AS_TXT_FREQ)+": ")) * 60;	// entered in minutes, stored in seconds

	var kickid := entry_id;
	if( isnew )
		record := AddAreaSpawnerEntry(record, newsource, newref, newmin, newmax, newfreq);
		kickid := record.entries[record.entries.size()].id;
	else
		record := UpdateAreaSpawnerEntry(record, entry_id, newsource, newref, newmin, newmax, newfreq);
	endif

	// a brand new / widened entry on an already-enabled spawner needs its own
	// kick - the passive side only wakes up on deaths and explicit triggers.
	// Skipped if a chain's already running for this entry (rare, but possible
	// if it was mid-refill when edited) - mutates record before the one Save.
	if( record.enabled )
		AreaSpawnerMarkAndKickEntry(realm, name, prefix, record, kickid, 0);
	endif

	SaveAreaSpawner(realm, name, record, who, prefix);

endfunction


// Custom NPC kind's entry editor - separate from ShowEntryEditor since the
// shape is different (a scaffold template name + a full stat-override block,
// instead of a single ref field). "Load" fetches the typed template's base
// stats (GetNpcTemplateBaseStats) and refills the stat fields without saving/
// closing, so the GM can review/adjust before OK - hand-edited fields survive
// a Load only if the GM re-edits them after, since Load always overwrites all
// of them from the template (this mirrors "pick a template, then edit its
// stats" rather than a partial-merge, keeping the behavior predictable).
function ShowCustomNpcEntryEditor(who, realm, name, entry_id, prefix := "areaspawner")

	var record := LoadAreaSpawner(realm, name, prefix);
	if( !record )
		return;
	endif

	var template := "";
	var ov := NewCustomNpcOverrides();
	var emin := 1;
	var emax := 1;
	var efreq := 5;	// minutes - shown/entered in minutes, stored as seconds
	var isnew := 1;

	if( entry_id )
		var idx := FindAreaSpawnerEntryIndex(record, entry_id);
		if( idx )
			var entry := record.entries[idx];
			template := CStr(entry.ref);
			if( entry.overrides )
				// Copy field-by-field onto the NewCustomNpcOverrides() base
				// from above rather than "ov := entry.overrides" wholesale -
				// an entry saved before the Appearance/Prots/Other fields
				// existed only has the original 9 fields on its struct, and
				// every sub-editor below writes new fields onto ov with
				// plain ":=" (not the struct's field-adding "+" operator),
				// which needs the field to already be declared on ov. A
				// missing field reads back falsy either way, so this is a
				// safe no-op for any field an old entry never had.
				var loaded := entry.overrides;
				ov.name := loaded.name;
				ov.str := loaded.str;
				ov.dex := loaded.dex;
				ov.intel := loaded.intel;
				ov.hits := loaded.hits;
				ov.stam := loaded.stam;
				ov.mana := loaded.mana;
				ov.hpregen := loaded.hpregen;
				ov.manaregen := loaded.manaregen;
				ov.color := loaded.color;
				ov.truecolor := loaded.truecolor;
				ov.objtype := loaded.objtype;
				ov.gender := loaded.gender;
				ov.lesserboss := loaded.lesserboss;
				ov.boss := loaded.boss;
				ov.superboss := loaded.superboss;
				ov.runspeed := loaded.runspeed;
				ov.movespeed := loaded.movespeed;
				ov.snoopme := loaded.snoopme;
				ov.stealme := loaded.stealme;
				ov.looter := loaded.looter;
				ov.slaytype := loaded.slaytype;
				ov.fireprot := loaded.fireprot;
				ov.waterprot := loaded.waterprot;
				ov.airprot := loaded.airprot;
				ov.earthprot := loaded.earthprot;
				ov.holyprot := loaded.holyprot;
				ov.necroprot := loaded.necroprot;
				ov.physicalprot := loaded.physicalprot;
				ov.permpoisonimmunity := loaded.permpoisonimmunity;
				ov.permmagicimmunity := loaded.permmagicimmunity;
				ov.freeaction := loaded.freeaction;
				ov.permmr := loaded.permmr;
			endif
			emin := entry.min;
			emax := entry.max;
			efreq := entry.frequency/60;
			isnew := 0;
		endif
	endif

	while(1)
		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 380 415" };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( isnew ? "New Custom NPC" : "Edit Custom NPC" );
		tid := tid + 1;

		layout.append( "text 20 45 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "NPC Template to copy (type name, then Load):" );
		tid := tid + 1;
		layout.append( "textentry 20 68 160 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_REF)+" "+CStr(tid) );
		data.append( template );
		tid := tid + 1;
		layout.append( "button 200 66 "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_LOADTEMPLATE) );
		layout.append( "text 225 68 "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
		data.append( "Load" );
		tid := tid + 1;

		// Informational only - whatever the currently-typed template's own
		// loot roll looks like in npcdesc.cfg, same 3 fields spawnpoint's
		// customnpc.inc gump shows. Not tied to Load/an override field, so
		// it stays in sync with whatever's in the template box even before
		// hitting Load, and goes blank for a name that isn't a real template.
		var lootinfo := template ? AreaSpawnerNpcLootInfo(template) : 0;
		layout.append( "text 20 92 "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( lootinfo ? "LG: "+CStr(lootinfo.lootgroup)+"   MIL: "+CStr(lootinfo.miclevel)+"   MIC: "+CStr(lootinfo.micchance) : "LG: -   MIL: -   MIC: -" );
		tid := tid + 1;

		// The old flat Name/STR/DEX/INT/.../RunSpeed field list has moved
		// into 4 category sub-pages (Appearance/Stats/Prots/Other) - Name
		// lives on Appearance now, next to Graphic/Color, since it's part of
		// how the NPC presents rather than a spawn-entry setting like Min/
		// Max/Frequency below. This mirrors how the box-edit gump already
		// splits off into pickers rather than growing indefinitely.
		var y := 130;
		var catlabels := array{ "Appearance...", "Stats...", "Protections...", "Other..." };
		var catids := array{ AS_BTN_CATAPPEARANCE, AS_BTN_CATSTATS, AS_BTN_CATPROTS, AS_BTN_CATOTHER };
		var c;
		for( c := 1; c <= catlabels.size(); c := c + 1 )
			layout.append( "button 20 "+CStr(y)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(catids[c]) );
			layout.append( "text 55 "+CStr(y+2)+" "+CStr(Cint(COLOR_GREEN))+" "+CStr(tid) );
			data.append( catlabels[c] );
			tid := tid + 1;
			y := y + 30;
		endfor
		y := y + 6;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Min:" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MIN)+" "+CStr(tid) );
		data.append( CStr(emin) );
		tid := tid + 1;
		y := y + 26;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Max:" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_MAX)+" "+CStr(tid) );
		data.append( CStr(emax) );
		tid := tid + 1;
		y := y + 26;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Frequency (mins):" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_FREQ)+" "+CStr(tid) );
		data.append( CStr(efreq) );
		tid := tid + 1;
		y := y + 36;

		layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
		layout.append( "button 140 "+CStr(y)+" "+CStr(GUMPID_BUTTON_DELETE_0)+" "+CStr(GUMPID_BUTTON_DELETE_1)+" 1 0 "+CStr(AS_BTN_DELETE) );
		layout.append( "button 260 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CANCEL] )
			return;
		endif

		if( keyret[AS_BTN_DELETE] && !isnew )
			if( ShowConfirm(who, "Remove this entry? Any mobs it spawned will be despawned.") )
				record := RemoveAreaSpawnerEntryAndDespawn(record, entry_id);
				SaveAreaSpawner(realm, name, record, who, prefix);
			endif
			return;
		endif

		// pull whatever's currently in the fields (typed template/name/
		// min/max/freq) before deciding what to do next - Load Template and
		// the category buttons below only refill/branch from here, they
		// don't discard this read
		template := keyret[AS_TXT_REF] - (CStr(AS_TXT_REF)+": ");
		emin := Cint(keyret[AS_TXT_MIN] - (CStr(AS_TXT_MIN)+": "));
		emax := Cint(keyret[AS_TXT_MAX] - (CStr(AS_TXT_MAX)+": "));
		efreq := Cint(keyret[AS_TXT_FREQ] - (CStr(AS_TXT_FREQ)+": "));

		if( keyret[AS_BTN_LOADTEMPLATE] )
			if( !template )
				SendSysMessage(who, "Type a template name first.");
			else
				var basestats := GetNpcTemplateBaseStats(template);
				if( !basestats )
					SendSysMessage(who, "'"+template+"' is not a valid npc template.");
				else
					ov := basestats;
					SendSysMessage(who, "Loaded base stats from '"+template+"'.");
				endif
			endif
			continue;
		endif

		if( keyret[AS_BTN_CATAPPEARANCE] )
			ov := ShowCustomNpcAppearanceEditor(who, ov);
			continue;
		endif
		if( keyret[AS_BTN_CATSTATS] )
			ov := ShowCustomNpcStatsEditor(who, ov);
			continue;
		endif
		if( keyret[AS_BTN_CATPROTS] )
			ov := ShowCustomNpcProtsEditor(who, ov);
			continue;
		endif
		if( keyret[AS_BTN_CATOTHER] )
			ov := ShowCustomNpcOtherEditor(who, ov);
			continue;
		endif

		if( !keyret[AS_BTN_OK] )
			return;
		endif

		if( !FindConfigElem(ReadConfigFile(CFGFILE_NPCS), template) )
			SendSysMessage(who, "'"+template+"' is not a valid npc template.");
			continue;
		endif

		var newfreq := efreq * 60;	// entered in minutes, stored in seconds

		var kickid := entry_id;
		if( isnew )
			record := AddAreaSpawnerEntry(record, "customnpc", template, emin, emax, newfreq, ov);
			kickid := record.entries[record.entries.size()].id;
		else
			record := UpdateAreaSpawnerEntry(record, entry_id, "customnpc", template, emin, emax, newfreq, ov);
		endif

		if( record.enabled )
			AreaSpawnerMarkAndKickEntry(realm, name, prefix, record, kickid, 0);
		endif

		SaveAreaSpawner(realm, name, record, who, prefix);
		return;
	endwhile

endfunction


// Appearance sub-page of the Custom NPC editor - name, graphic/color/
// truecolor/gender, plus the LesserBoss/Boss/SuperBoss display CProps (see
// pkg/std/tracking/tracking.src and pkg/systems/reputation/hook/namedyes.src
// for what those actually change in-game: name-color highlighting and
// tracking visibility, not stats). Cancel discards changes; OK returns the
// updated struct. Same reused AS_TXT_G*/AS_CHK_G* ids as the other 3
// category pages - see their consts' comment for why that's safe.
function ShowCustomNpcAppearanceEditor(who, ov)

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 380 390" };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Custom NPC - Appearance" );
	tid := tid + 1;

	var y := 55;
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Name (blank = template default):" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_CNAME)+" "+CStr(tid) );
	data.append( CStr(ov.name) );
	tid := tid + 1;
	y := y + 26;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Graphic (0 = template default):" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G1)+" "+CStr(tid) );
	data.append( CStr(ov.objtype) );
	tid := tid + 1;
	y := y + 26;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Color (0 = template default):" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_CCOLOR)+" "+CStr(tid) );
	data.append( CStr(ov.color) );
	tid := tid + 1;
	y := y + 26;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "True Color (0 = template default):" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G2)+" "+CStr(tid) );
	data.append( CStr(ov.truecolor) );
	tid := tid + 1;
	y := y + 34;

	layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.gender?1:0)+" "+CStr(AS_CHK_G1) );
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Female (unchecked = Male):" );
	tid := tid + 1;
	y := y + 30;

	layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.lesserboss?1:0)+" "+CStr(AS_CHK_G2) );
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Lesser Boss:" );
	tid := tid + 1;
	y := y + 30;

	layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.boss?1:0)+" "+CStr(AS_CHK_G4) );
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Boss:" );
	tid := tid + 1;
	y := y + 30;

	layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.superboss?1:0)+" "+CStr(AS_CHK_G3) );
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Super Boss:" );
	tid := tid + 1;
	y := y + 40;

	layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
	layout.append( "button 260 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || !keyret[AS_BTN_OK] )
		return ov;
	endif

	ov.name := keyret[AS_TXT_CNAME] - (CStr(AS_TXT_CNAME)+": ");
	ov.objtype := Cint(keyret[AS_TXT_G1] - (CStr(AS_TXT_G1)+": "));
	ov.color := Cint(keyret[AS_TXT_CCOLOR] - (CStr(AS_TXT_CCOLOR)+": "));
	ov.truecolor := Cint(keyret[AS_TXT_G2] - (CStr(AS_TXT_G2)+": "));
	ov.gender := keyret[AS_CHK_G1] ? 1 : 0;
	ov.lesserboss := keyret[AS_CHK_G2] ? 1 : 0;
	ov.boss := keyret[AS_CHK_G4] ? 1 : 0;
	ov.superboss := keyret[AS_CHK_G3] ? 1 : 0;

	return ov;

endfunction


// Stats sub-page of the Custom NPC editor - str/dex/int, max hits/stam/mana,
// and the regen bonuses (BaseHpRegen/BaseManaRegen - see
// scripts/ai/setup/modsetup.inc, which is what actually turns these into a
// regen rate and a Karma/Fame bonus once the critter's AI restarts).
function ShowCustomNpcStatsEditor(who, ov)

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 380 330" };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Custom NPC - Stats" );
	tid := tid + 1;

	var y := 55;
	var fieldlabels := array{ "STR:", "DEX:", "INT:", "Max Hits:", "Max Stam:", "Max Mana:", "HP Regen Bonus:", "Mana Regen Bonus:" };
	var fieldids := array{ AS_TXT_CSTR, AS_TXT_CDEX, AS_TXT_CINT, AS_TXT_CHITS, AS_TXT_CSTAM, AS_TXT_CMANA, AS_TXT_G1, AS_TXT_G2 };
	var fieldvals := array{ CStr(ov.str), CStr(ov.dex), CStr(ov.intel), CStr(ov.hits), CStr(ov.stam), CStr(ov.mana), CStr(ov.hpregen), CStr(ov.manaregen) };

	var f;
	for( f := 1; f <= fieldlabels.size(); f := f + 1 )
		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( fieldlabels[f] );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(fieldids[f])+" "+CStr(tid) );
		data.append( fieldvals[f] );
		tid := tid + 1;
		y := y + 26;
	endfor
	y := y + 10;

	layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
	layout.append( "button 260 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || !keyret[AS_BTN_OK] )
		return ov;
	endif

	ov.str := Cint(keyret[AS_TXT_CSTR] - (CStr(AS_TXT_CSTR)+": "));
	ov.dex := Cint(keyret[AS_TXT_CDEX] - (CStr(AS_TXT_CDEX)+": "));
	ov.intel := Cint(keyret[AS_TXT_CINT] - (CStr(AS_TXT_CINT)+": "));
	ov.hits := Cint(keyret[AS_TXT_CHITS] - (CStr(AS_TXT_CHITS)+": "));
	ov.stam := Cint(keyret[AS_TXT_CSTAM] - (CStr(AS_TXT_CSTAM)+": "));
	ov.mana := Cint(keyret[AS_TXT_CMANA] - (CStr(AS_TXT_CMANA)+": "));
	ov.hpregen := Cint(keyret[AS_TXT_G1] - (CStr(AS_TXT_G1)+": "));
	ov.manaregen := Cint(keyret[AS_TXT_G2] - (CStr(AS_TXT_G2)+": "));

	return ov;

endfunction


// Protections sub-page of the Custom NPC editor - the 7 elemental
// protections (fire/water/air/earth/holy/necro/physical - this shard's
// custom magic schools, not stock UO's resist types) plus the "alternative"
// immunity CProps. Bounds per the CProp's own meaning, not enforced here
// (matches every other numeric field in this editor - GM is trusted, same as
// hand-editing npcdesc.cfg directly): elemental protections run roughly
// -200 to 200, PhysicalProtection roughly -20 to 20, and
// PermPoisonImmunity/PermMagicImmunity/Permmr are 0-10 strength levels while
// FreeAction is a plain on/off. See scripts/ai/setup/modsetup.inc's
// possible_prots list for the authoritative set of CProp names this reads.
function ShowCustomNpcProtsEditor(who, ov)

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 380 420" };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Custom NPC - Protections" );
	tid := tid + 1;

	var y := 50;
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Elemental (-200 to 200, Physical -20 to 20):" );
	tid := tid + 1;
	y := y + 22;

	var fieldlabels := array{ "Fire:", "Water:", "Air:", "Earth:", "Holy:", "Necro:", "Physical:" };
	var fieldids := array{ AS_TXT_G1, AS_TXT_G2, AS_TXT_G3, AS_TXT_G4, AS_TXT_G5, AS_TXT_G6, AS_TXT_G7 };
	var fieldvals := array{ CStr(ov.fireprot), CStr(ov.waterprot), CStr(ov.airprot), CStr(ov.earthprot), CStr(ov.holyprot), CStr(ov.necroprot), CStr(ov.physicalprot) };

	var f;
	for( f := 1; f <= fieldlabels.size(); f := f + 1 )
		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( fieldlabels[f] );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(fieldids[f])+" "+CStr(tid) );
		data.append( fieldvals[f] );
		tid := tid + 1;
		y := y + 24;
	endfor
	y := y + 6;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Alternative (0 to 10):" );
	tid := tid + 1;
	y := y + 22;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Poison Immunity:" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G8)+" "+CStr(tid) );
	data.append( CStr(ov.permpoisonimmunity) );
	tid := tid + 1;
	y := y + 24;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Magic Immunity:" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G9)+" "+CStr(tid) );
	data.append( CStr(ov.permmagicimmunity) );
	tid := tid + 1;
	y := y + 24;

	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Magic Resist:" );
	tid := tid + 1;
	layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G10)+" "+CStr(tid) );
	data.append( CStr(ov.permmr) );
	tid := tid + 1;
	y := y + 30;

	layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.freeaction?1:0)+" "+CStr(AS_CHK_G1) );
	layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
	data.append( "Free Action:" );
	tid := tid + 1;
	y := y + 36;

	layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
	layout.append( "button 260 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || !keyret[AS_BTN_OK] )
		return ov;
	endif

	ov.fireprot := Cint(keyret[AS_TXT_G1] - (CStr(AS_TXT_G1)+": "));
	ov.waterprot := Cint(keyret[AS_TXT_G2] - (CStr(AS_TXT_G2)+": "));
	ov.airprot := Cint(keyret[AS_TXT_G3] - (CStr(AS_TXT_G3)+": "));
	ov.earthprot := Cint(keyret[AS_TXT_G4] - (CStr(AS_TXT_G4)+": "));
	ov.holyprot := Cint(keyret[AS_TXT_G5] - (CStr(AS_TXT_G5)+": "));
	ov.necroprot := Cint(keyret[AS_TXT_G6] - (CStr(AS_TXT_G6)+": "));
	ov.physicalprot := Cint(keyret[AS_TXT_G7] - (CStr(AS_TXT_G7)+": "));
	ov.permpoisonimmunity := Cint(keyret[AS_TXT_G8] - (CStr(AS_TXT_G8)+": "));
	ov.permmagicimmunity := Cint(keyret[AS_TXT_G9] - (CStr(AS_TXT_G9)+": "));
	ov.permmr := Cint(keyret[AS_TXT_G10] - (CStr(AS_TXT_G10)+": "));
	ov.freeaction := keyret[AS_CHK_G1] ? 1 : 0;

	return ov;

endfunction


// Other sub-page of the Custom NPC editor - movement (MoveSpeed/RunSpeed),
// snoop/steal detect chances, Looter, and the slayer Type (opens
// ShowCustomNpcSlayTypePicker rather than a free-typed field, since a typo
// there silently just never matches any slayer weapon rather than erroring).
function ShowCustomNpcOtherEditor(who, ov)

	while(1)
		var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
			"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 380 330" };
		var data := array{};
		var tid := 0;

		layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
		data.append( "Custom NPC - Other" );
		tid := tid + 1;

		var y := 55;
		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Move Speed (0-300, 0 = dex-based default):" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G1)+" "+CStr(tid) );
		data.append( CStr(ov.movespeed) );
		tid := tid + 1;
		y := y + 26;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Run Speed (0 = template default):" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_CRUNSPEED)+" "+CStr(tid) );
		data.append( CStr(ov.runspeed) );
		tid := tid + 1;
		y := y + 26;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Snoop Chance (0 = template default):" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G2)+" "+CStr(tid) );
		data.append( CStr(ov.snoopme) );
		tid := tid + 1;
		y := y + 26;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Steal Chance (0 = template default):" );
		tid := tid + 1;
		layout.append( "textentry 260 "+CStr(y-2)+" 90 20 "+CStr(Cint(COLOR_GREEN))+" "+CStr(AS_TXT_G3)+" "+CStr(tid) );
		data.append( CStr(ov.stealme) );
		tid := tid + 1;
		y := y + 34;

		layout.append( "checkbox 260 "+CStr(y-2)+" "+CStr(AS_GFX_CHECKBOX_OFF)+" "+CStr(AS_GFX_CHECKBOX_ON)+" "+CStr(ov.looter?1:0)+" "+CStr(AS_CHK_G1) );
		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Looter:" );
		tid := tid + 1;
		y := y + 34;

		layout.append( "text 20 "+CStr(y)+" "+CStr(Cint(COLOR_BLUE_LIGHT))+" "+CStr(tid) );
		data.append( "Slayer Type: "+(ov.slaytype ? ov.slaytype : "(template default)") );
		tid := tid + 1;
		layout.append( "button 300 "+CStr(y-2)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(AS_BTN_PICKSLAYTYPE) );
		y := y + 40;

		layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK) );
		layout.append( "button 260 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

		var keyret := SendDialogGump( who, layout, data );
		if( !keyret || keyret[AS_BTN_CANCEL] )
			return ov;
		endif

		// same "read whatever's in the fields before branching" rule as the
		// main editor, so Pick Slayer Type doesn't discard hand-typed values
		ov.movespeed := Cint(keyret[AS_TXT_G1] - (CStr(AS_TXT_G1)+": "));
		ov.runspeed := Cint(keyret[AS_TXT_CRUNSPEED] - (CStr(AS_TXT_CRUNSPEED)+": "));
		ov.snoopme := Cint(keyret[AS_TXT_G2] - (CStr(AS_TXT_G2)+": "));
		ov.stealme := Cint(keyret[AS_TXT_G3] - (CStr(AS_TXT_G3)+": "));
		ov.looter := keyret[AS_CHK_G1] ? 1 : 0;

		if( keyret[AS_BTN_PICKSLAYTYPE] )
			var picked := ShowCustomNpcSlayTypePicker(who, ov.slaytype);
			if( picked )
				ov.slaytype := picked;
			endif
			continue;
		endif

		if( !keyret[AS_BTN_OK] )
			return ov;
		endif

		return ov;
	endwhile

endfunction


// Simple one-button-per-option picker for the NPC "Type" CProp (slayer
// category) - see AreaSpawnerSlayTypes() for why this list has to be
// hand-kept in sync with pkg/systems/combat/include/hitscriptinc.inc rather
// than read live from anywhere. Returns the chosen string, or 0 if the admin
// cancelled (caller should leave the existing value alone on a 0 return).
function ShowCustomNpcSlayTypePicker(who, current)

	var types := AreaSpawnerSlayTypes();

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 260 "+CStr(70 + (types.size()*24) + 50) };
	var data := array{};
	var tid := 0;

	layout.append( "text 20 15 "+CStr(Cint(COLOR_YELLOW))+" "+CStr(tid) );
	data.append( "Pick Slayer Type" );
	tid := tid + 1;

	var y := 50;
	var i;
	for( i := 1; i <= types.size(); i := i + 1 )
		var btnid := AS_BTN_SLAYTYPE_BASE + i;
		layout.append( "button 20 "+CStr(y)+" "+CStr(AS_GFX_ROWBUTTON_0)+" "+CStr(AS_GFX_ROWBUTTON_1)+" 1 0 "+CStr(btnid) );
		layout.append( "text 55 "+CStr(y+2)+" "+CStr((types[i]==current) ? Cint(COLOR_YELLOW) : Cint(COLOR_GREEN))+" "+CStr(tid) );
		data.append( types[i] );
		tid := tid + 1;
		y := y + 24;
	endfor
	y := y + 10;

	layout.append( "button 20 "+CStr(y)+" "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) );

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || keyret[AS_BTN_CANCEL] )
		return 0;
	endif

	for( i := 1; i <= types.size(); i := i + 1 )
		if( keyret[AS_BTN_SLAYTYPE_BASE + i] )
			return types[i];
		endif
	endfor

	return 0;

endfunction


function ShowConfirm(who, prompt)

	// prompt often has a GM-chosen spawner name spliced into it - truncate
	// so a long one can't run off the panel edge the way disabled-reason
	// text used to on the list gump
	if( Len(prompt) > 70 )
		prompt := SubStr(prompt, 1, 70)+"...";
	endif

	var layout := array{ "page 0", GUMPID_SETTING_NO_DISPOSE,
		"resizepic 0 0 "+CStr(GUMPID_BKGND_STONESLAB)+" 420 140",
		"text 20 15 "+CStr(Cint(COLOR_RED))+" 0",
		// OKAY/CANCEL graphics already render their own text, no extra labels needed
		"button 60 90 "+CStr(GUMPID_BUTTON_OKAY_0)+" "+CStr(GUMPID_BUTTON_OKAY_1)+" 1 0 "+CStr(AS_BTN_OK),
		"button 240 90 "+CStr(GUMPID_BUTTON_CANCEL_0)+" "+CStr(GUMPID_BUTTON_CANCEL_1)+" 1 0 "+CStr(AS_BTN_CANCEL) };
	var data := array{ prompt };

	var keyret := SendDialogGump( who, layout, data );
	if( !keyret || !keyret[AS_BTN_OK] )
		return 0;
	endif

	return 1;

endfunction


// Read-only HTML-dump style list (same chrome as areaspawnergroupbrowser.src /
// .commands), driving the single .aserrorall command. Used to be 7 separate
// per-kind/per-event-status commands (.aserrorarea, .aserrorchest, etc), each
// silently only covering its own narrow slice - that's exactly what caused a
// real bug: the login/staff notice's disabled-spawner count comes from
// ListAreaSpawnerAutoDisabled() with no filters (every kind, event and
// regular, every realm), so a GM running the one command that sounded right
// could get "nothing needs attention" while the count said otherwise.
// kindfilter/eventfilter parameters are kept (still forwarded straight
// through to ListAreaSpawnerAutoDisabled) in case a narrower view is ever
// wanted again, but .aserrorall is the only caller now, passing no filters -
// which is why item.kind is always rendered below rather than only for the
// combined case.
function ShowAreaSpawnerErrorList(who, title, kindfilter := "", eventfilter := -1)

	var found := ListAreaSpawnerAutoDisabled(kindfilter, eventfilter);

	var html := "<BASEFONT SIZE=17>";
	if( !found || !found.size() )
		html := html+"Nothing currently needs attention.";
	else
		foreach item in found
			var tag := (item.status == "disabled") ? "<b>[DISABLED]</b>" : "[PAUSED]";
			var where := item.realm + (item.is_event ? " - event" : "");
			html := html+"<br><br>"+tag+" ["+AreaSpawnerTypeLabel(item.kind)+"] '"+item.name+"' ("+where+")<br>    "+item.reason;
		endforeach
	endif

	var layout := array{ "page 0",
		"resizepic 20 20 "+CStr(GUMPID_BKGND_PAPER_EX)+" 700 635",
		"text 300 35 67 0",
		"htmlgump 35 55 660 575 1 1 1" };
	var data := array{ title, html };

	SendDialogGump( who, layout, data );

endfunction


// Facet-wide census - every area spawner of every kind (regular, not event)
// in one chosen realm, with its region, status/error, population, and each
// entry's spawn timer/contents. Same read-only htmlgump chrome as
// ShowAreaSpawnerErrorList, just not filtered down to only-broken ones - the
// smaller, single-facet version of the "all facets at once" overview that
// got deferred earlier. Region is derived from the box's center point, which
// is fine even for single-point kinds (individual/item/customnpc) since
// those just store the same value in min and max.
function ShowAreaSpawnerFacetCensus(who, realm)

	var clock := ReadGameClock();
	var kinds := AreaSpawnerKinds();
	var html := "<BASEFONT SIZE=17>";
	var any := 0;

	foreach kind in kinds
		var prefix := AreaSpawnerKindPrefix(kind, 0);
		var names := GetAreaSpawnerNames(realm, prefix);
		if( !names || !names.size() )
			continue;
		endif

		foreach sname in names
			var record := LoadAreaSpawner(realm, sname, prefix);
			if( !record )
				continue;
			endif
			any := 1;

			var cx := CInt((record.minx + record.maxx) / 2);
			var cy := CInt((record.miny + record.maxy) / 2);
			var region := GetRegionNameAtLocation(cx, cy, realm);
			if( !region )
				region := "(unknown region)";
			endif

			var status := "OFF";
			var statuscolor := "808080";
			if( record.enabled )
				status := "ON";
				statuscolor := "00FF00";
				if( AreaSpawnerFindPausedReason(record) )
					status := "ON (paused)";
					statuscolor := "FFFF00";
				endif
			elseif( record.disabled_reason )
				status := "OFF (AUTO)";
				statuscolor := "FF0000";
			endif

			html := html+"<br><br><basefont color=#FFFF00>["+AreaSpawnerTypeLabel(kind)+"]</basefont> '"+sname+"' - "+region
				+"  <basefont color=#"+statuscolor+">["+status+"]</basefont>  pop:"+AreaSpawnerPopulationSummary(record);

			if( record.disabled_reason )
				html := html+"<br>    <basefont color=#FF0000>"+record.disabled_reason+"</basefont>";
			endif

			var entries := record.entries;
			foreach entry in entries
				var alive := entry.serials.size();
				var timerstr;
				var timercolor := "FFFFFF";
				if( entry.paused_reason )
					timerstr := "paused - "+entry.paused_reason;
					timercolor := "FFFF00";
				elseif( alive >= entry.target )
					timerstr := "full";
				elseif( entry.next_spawn_time <= clock )
					timerstr := "due now";
				else
					timerstr := CStr(entry.next_spawn_time - clock)+"s";
				endif

				// refilling should only ever be 1 while a kick chain is
				// genuinely mid-Sleep, pacing toward its next attempt - if
				// it's 1 alongside "due now"/pop below target and just sits
				// there across repeated checks, the chain died without
				// clearing it (see AreaSpawnerResetEntryCooldowns) and
				// nothing will retry this entry until something explicitly
				// re-kicks it (toggle Enabled off/on, edit+save the entry,
				// or the safety-net sweep)
				var chaintag := entry.refilling ? " [chain active]" : "";

				html := html+"<br>    #"+CStr(entry.id)+" "+entry.source+":"+CStr(entry.ref)
					+"  pop "+CStr(alive)+"/"+CStr(entry.target)+"  every "+CStr(entry.frequency)+"s"
					+"  <basefont color=#"+timercolor+">next: "+timerstr+"</basefont>"+chaintag;
			endforeach
		endforeach
	endforeach

	if( !any )
		html := html+"No area spawners of any kind in "+realm+".";
	endif

	var layout2 := array{ "page 0",
		"resizepic 20 20 "+CStr(GUMPID_BKGND_PAPER_EX)+" 700 635",
		"text 300 35 67 0",
		"htmlgump 35 55 660 575 1 1 1" };
	var data2 := array{ "Area Spawner Census - "+realm, html };

	SendDialogGump( who, layout2, data2 );

endfunction
