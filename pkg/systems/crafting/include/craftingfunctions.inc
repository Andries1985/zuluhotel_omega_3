use uo;
use cfgfile;

include ":gumps:yesno";
include "include/client";
include "include/classes";
include "include/math";

const GOLD_COLOR	:= 0x0033;

function ApplyMaterialProperties(byref product_desc, material, is_exceptional, character, skill_id)

	var material_cfg;
	//Print("Material Objtype: " + material.objtype + "And name: " + material.Name);

	case(skill_id)
		SKILLID_BLACKSMITHY: 
			material_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );
		SKILLID_TAILORING:
			material_cfg := ReadConfigFile( ":tailoring:tailoring" );
		SKILLID_CARPENTRY:
			material_cfg := ReadConfigFile( ":carpentry:carpentry" );
		SKILLID_BOWCRAFT:
			material_cfg := ReadConfigFile( ":carpentry:carpentry" );
		SKILLID_TINKERING:
			if( IsIngot( material ) );
				material_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );
				Print("Ingot Material detected for Tinkering.");
			elseif( IsLog( material ) );
				material_cfg := ReadConfigFile( ":carpentry:carpentry" );
				Print("Log Material detected for Tinkering.");
			endif
		default:
			return 0;
	endcase
		
	var material_desc := material_cfg[material.objtype];
	Print(material_desc.Name);
	Print(product_desc.Name);
	Print(product_desc.desc);

	// if (skill_id == SKILLID_CARPENTRY)
	// 	product_desc.Desc := product_desc.Desc;
	// 	Print("Carpentry detected, not adding material name to desc.");
	// else
		if( !material_desc.Name || material_desc.Name == "" )
			product_desc.Desc := product_desc.Desc;
			Print("No material name found, leaving desc as is.");
		else
			product_desc.Material := material_desc.Name;
			product_desc.Desc := material_desc.Name +" "+product_desc.Desc;
			Print("Material Name: " + material_desc.Name);
			Print("Product Desc Material: " + product_desc.Material);
		endif
		//product_desc.Desc := material_desc.Name +" "+product_desc.Desc;
	//endif

	Print(product_desc.desc);
	product_desc.Quality := material_desc.Quality;

	if( material.objtype == UOBJ_GOLD_INGOT )
		product_desc.Color := GOLD_COLOR;
	else
		product_desc.Color := material.Color;
	endif

	//Specific Exceptional properties
	if(is_exceptional)

		product_desc.Cprops.insert( "IsExceptional", 1 );
		product_desc.Desc := "Exceptional " + product_desc.Desc;
		Print(product_desc.desc);

		if(GetObjProperty(character, "ToggleBuildMark"))
			if( YesNo( character , "Add Builder Mark?" ))
				product_desc.Cprops.insert( "CraftedBy", character.name );
				//product_desc.Desc := product_desc.Desc + " (Crafted by " + character.name + ")";
			endif
		else
			product_desc.Cprops.insert( "CraftedBy", character.name );
			//product_desc.Desc := product_desc.Desc + " (Crafted by " + character.name + ")";
		endif

		var multiplier := 5 + Cint(GetEffectiveSkill(character, SKILLID_ARMSLORE) / 10 );
		
		if(GetObjProperty(character , CLASSEID_CRAFTER))
			multiplier := Cint(multiplier * ClasseBonus(character, CLASSEID_CRAFTER));
		endif

		multiplier := (100 + multiplier);
        product_desc.quality := Trunc((product_desc.quality * multiplier) / 100.0, 2);

	endif

	product_desc.Cprops.insert("CraftedWith", material.objtype);
	product_desc.Cprops.insert("BaseName", product_desc.Desc);

	if(product_desc.hitscript)// only weapons have hitscripts, right??
		if (material_desc.ElemDamage)
			product_desc.Cprops.insert("ElementalDamage",material_desc.ElemDamage);
		endif
		if (material_desc.SlayType)
			product_desc.Cprops.insert("SlayType",material_desc.SlayType);
			product_desc.Hitscript := ":combat:slayerscript";
		endif
	elseif(product_desc.ar)
		foreach cprop in GetConfigStringArray( material_desc , "CProp" );
			var split_prop := SplitWords( cprop );
			var prop_name := split_prop[1];
			var prop_mod  := Cint(split_prop[2]);
			
			var presently := Cint(product_desc.Cprops[prop_name]);
			if( !presently )
				presently := 0;
			endif

			if( presently && (abs(prop_mod) > abs(presently) ))
				prop_mod := 0 - presently;
			endif

			var resulting := presently + prop_mod;
			if( resulting > 0 and prop_name == "DefaultDex" )
				resulting := 0;
			endif
			product_desc.Cprops[prop_name] := resulting;
		endforeach
	endif

endfunction

function NearASoulforge(me)

	foreach item in ListItemsNearLocation( me.x, me.y, me.z, 2, me.realm )
		if (item.objtype >= UOBJ_MINISOULFORGE_START and item.objtype <= UOBJ_MINISOULFORGE_END)
			return 1;
		elseif (item.objtype >= UOBJ_LARGESOULFORGE_START and item.objtype <= UOBJ_LARGESOULFORGE_END)
			return 1;
		endif
	endforeach

	return 0;

endfunction

function NearAnElvenAnvilOrForge(me)

	foreach item in ListItemsNearLocation( me.x, me.y, me.z, 2, me.realm )
		if (item.objtype == UOBJ_ELVEN_ANVIL1 or item.objtype == UOBJ_ELVEN_ANVIL2 or item.objtype == UOBJ_ELVEN_FORGE)
			return 1;
		endif
	endforeach

	return 0;

endfunction

function ApplyMaterialPropertiesDeed(byref product_desc, material, is_exceptional, character, skill_id, deedname)

	var material_cfg;
	//Print("Material Objtype: " + material.objtype + "And name: " + material.Name);
	product_desc.deedname := deedname;

	case(skill_id)
		SKILLID_BLACKSMITHY: 
			material_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );
		SKILLID_TAILORING:
			material_cfg := ReadConfigFile( ":tailoring:tailoring" );
		SKILLID_CARPENTRY:
			material_cfg := ReadConfigFile( ":carpentry:carpentry" );
		SKILLID_BOWCRAFT:
			material_cfg := ReadConfigFile( ":carpentry:carpentry" );
		SKILLID_TINKERING:
			if( IsIngot( material ) );
				material_cfg := ReadConfigFile( ":blacksmithy:blacksmithy" );
			elseif( IsLog( material ) );
				material_cfg := ReadConfigFile( ":carpentry:carpentry" );
			endif
		default:
			return 0;
	endcase
		
	var material_desc := material_cfg[material.objtype];
	//Print(material_desc.Name);
	//Print(product_desc.Name);
	//Print(product_desc.deedname);

	// if (skill_id == SKILLID_CARPENTRY)
	// 	product_desc.Desc := product_desc.Desc;
	// else
		if( !material_desc.Name || material_desc.Name == "" )
			product_desc.Desc := product_desc.Desc;
		else
			product_desc.Desc := material_desc.Name +" "+product_desc.Desc;
			product_desc.Material := material_desc.Name;
			// Print("Material Name: " + material_desc.Name);
			// Print("Product Desc Material: " + product_desc.Material);
			product_desc.deedname := material_desc.Name + " " + product_desc.deedname;
		endif
		//product_desc.Desc := material_desc.Name +" "+product_desc.Desc;
	// endif

	product_desc.Quality := material_desc.Quality;

	if( material.objtype == UOBJ_GOLD_INGOT )
		product_desc.Color := GOLD_COLOR;
	else
		product_desc.Color := material.Color;
	endif

	//Specific Exceptional properties
	if(is_exceptional)

		product_desc.Cprops.insert( "IsExceptional", 1 );
		product_desc.Desc := "Exceptional " + product_desc.Desc;
		product_desc.deedname := "Exceptional " + product_desc.deedname;
		Print("Deed Name after exceptional: " + product_desc.deedname);

		if(GetObjProperty(character, "ToggleBuildMark"))
			if( YesNo( character , "Add Builder Mark?" ))
				product_desc.Cprops.insert( "CraftedBy", character.name );
				//product_desc.Desc := product_desc.Desc + " (Crafted by " + character.name + ")";
			endif
		else
			product_desc.Cprops.insert( "CraftedBy", character.name );
			//product_desc.Desc := product_desc.Desc + " (Crafted by " + character.name + ")";
		endif

		var multiplier := 5 + Cint(GetEffectiveSkill(character, SKILLID_ARMSLORE) / 10 );
		
		if(GetObjProperty(character , CLASSEID_CRAFTER))
			multiplier := Cint(multiplier * ClasseBonus(character, CLASSEID_CRAFTER));
		endif

		multiplier := (100 + multiplier);
        product_desc.quality := Trunc((product_desc.quality * multiplier) / 100.0, 2);

	endif

	product_desc.Cprops.insert("CraftedWith", material.objtype);
	product_desc.Cprops.insert("BaseName", product_desc.Desc);
	product_desc.Cprops.insert("DeedName", product_desc.deedname);

	// if(product_desc.hitscript)// only weapons have hitscripts, right??
	// 	if (material_desc.ElemDamage)
	// 		product_desc.Cprops.insert("ElementalDamage",material_desc.ElemDamage);
	// 	endif
	// 	if (material_desc.SlayType)
	// 		product_desc.Cprops.insert("SlayType",material_desc.SlayType);
	// 		product_desc.Hitscript := ":combat:slayerscript";
	// 	endif
	// elseif(product_desc.ar)
	// 	foreach cprop in GetConfigStringArray( material_desc , "CProp" );
	// 		var split_prop := SplitWords( cprop );
	// 		var prop_name := split_prop[1];
	// 		var prop_mod  := Cint(split_prop[2]);
			
	// 		var presently := Cint(product_desc.Cprops[prop_name]);
	// 		if( !presently )
	// 			presently := 0;
	// 		endif

	// 		if( presently && (abs(prop_mod) > abs(presently) ))
	// 			prop_mod := 0 - presently;
	// 		endif

	// 		var resulting := presently + prop_mod;
	// 		if( resulting > 0 and prop_name == "DefaultDex" )
	// 			resulting := 0;
	// 		endif
	// 		product_desc.Cprops[prop_name] := resulting;
	// 	endforeach
	// endif

endfunction