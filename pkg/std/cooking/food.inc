///////////////////
//  This function returns a dictionary of all foodstuffs used in cooking.  the dictionary is used to look
//  up the objtypes of names given to food.  it's used to make defining recipes easier.
///////////////////

function define_food_dictionary ()
	var food := dictionary;

	//some tools used in cooking
	food [ "ITEM_MIXING_BOWL" ]		:= 0x15f8;
	food [ "ITEM_ROLLING_PIN" ]		:= 0x1043;
	food [ "ITEM_FRYING_PAN" ]		:= 0x097f;
	food [ "ITEM_EMPTY_PITCHER" ]		:= 0x09a7;
	food [ "ITEM_EMPTY_GLASS" ]		:= 0x1f81;

	//bottles, pitchers and glasses of liquid
	food [ "ITEM_BOTTLE_OF_LIQUOR" ]	:= 0x099B;
	food [ "ITEM_BOTTLE_OF_ALE" ]		:= 0x099F;
	food [ "ITEM_BOTTLE_OF_WINE" ]	:= 0x09C7;
	food [ "ITEM_JUG_OF_CIDER" ]		:= 0x09c8;

	food [ "ITEM_PITCHER_OF_LIQUOR" ]	:= 0x1f9a;
	food [ "ITEM_PITCHER_OF_ALE" ]	:= 0x1f96;
	// 2026-09-15: 0x1f9c/0x1f98/0x1f9e were removed as exact-duplicate
	// objtypes 2026-08-29 (see itemdesc.cfg's dedup note) in favor of
	// 0x1f9b/0x1f97/0x1f9d -- this dictionary was never updated to match,
	// so recipes calling for wine/cider/water were resolving to dead
	// objtypes with no itemdesc entry.
	food [ "ITEM_PITCHER_OF_WINE" ]	:= 0x1f9b;
	food [ "ITEM_PITCHER_OF_CIDER" ]	:= 0x1f97;
	food [ "ITEM_PITCHER_OF_WATER" ]	:= 0x1f9d;
	food [ "ITEM_PITCHER_OF_MILK" ]	:= 0x09ad;

	food [ "ITEM_GLASS_OF_LIQUOR" ]	:= 0x1F85;
	food [ "ITEM_GLASS_OF_ALE" ]		:= 0x09EE;
	food [ "ITEM_GLASS_OF_WINE" ]		:= 0x1F8D;
	food [ "ITEM_GLASS_OF_CIDER" ]	:= 0x1F7D;
	food [ "ITEM_GLASS_OF_WATER" ]	:= 0x1F91;
	food [ "ITEM_GLASS_OF_MILK" ]		:= 0x1F89;

	//These items are special.  If they're called for in a recipe, it will call a special function that will
	//permit multiple forms of that item
	food [ "ITEM_GENERAL_WATER" ]		:= 0x1f9d;
	food [ "ITEM_GENERAL_MILK" ]		:= 0x09ad;
	food [ "ITEM_GENERAL_CHEESE" ]	:= 0x097d;
	// 2026-09-14: same pattern -- any Regular/Rare-tier fish from
	// pkg/std/fishing/catchtable.cfg satisfies these, not one fixed
	// objtype. Fallback values below are just representative (the
	// lowest objtype in each tier); the real resolution is
	// IsRegularFishObjtype()/IsRareFishObjtype() and the
	// special_find_/special_destroy_ functions further down this file.
	food [ "ITEM_REGULAR_FISH" ]		:= 0x30300;
	food [ "ITEM_RARE_FISH" ]		:= 0x3031D;

	//primary food ingredients
	food [ "ITEM_FLOUR" ]			:= 0x1045;
	food [ "ITEM_CORNMEAL" ]			:= 0xc800;
	food [ "ITEM_RAW_RIB" ]			:= 0x09f1;
	food [ "ITEM_RAW_FISHSTEAK" ]		:= 0x097a;
	food [ "ITEM_RAW_BIRD" ]			:= 0x09b9;
	food [ "ITEM_RAW_HAM" ]			:= 0x09c9;
	food [ "ITEM_BACON_SLAB" ]		:= 0x0976;
	food [ "ITEM_RAW_SAUSAGE" ]		:= 0x09c0;
	// ITEM_GREEN_FISH/BROWN_FISH/BLUE_FISH/YELLOW_FISH (0x09cc-0x09cf)
	// removed 2026-09-14 -- dead placeholder objtypes, nothing creates
	// them anymore. Replaced by ITEM_REGULAR_FISH/ITEM_RARE_FISH above.
	food [ "ITEM_LARD" ]			:= 0x1e8a;
	food [ "ITEM_EGG" ]				:= 0x09b5;
	food [ "ITEM_GARLIC" ]			:= 0x0f84;
	food [ "ITEM_HONEY" ]			:= 0x09ec;
	food [ "ITEM_CARROT" ]			:= 0x0c77;
	food [ "ITEM_APPLE" ]			:= 0x09d0;
	food [ "ITEM_PEACH" ]			:= 0x172c;
	food [ "ITEM_LEMON" ]			:= 0x1728;
	food [ "ITEM_LIME" ]			:= 0x172a;
	food [ "ITEM_BANANA" ]			:= 0x171f;
	food [ "ITEM_PEAR" ]			:= 0x172d;
	food [ "ITEM_GRAPES" ]			:= 0x0d1a;
	food [ "ITEM_DATES" ]			:= 0x1727;
	food [ "ITEM_PUMPKIN" ]			:= 0x0c6a;
	food [ "ITEM_YELLOW_GOURD" ]		:= 0x0c65;
	food [ "ITEM_GREEN_GOURD" ]		:= 0x0c66;
	food [ "ITEM_SQUASH" ]			:= 0x0c72;
	food [ "ITEM_WATERMELON" ]		:= 0x0c5c;
	food [ "ITEM_HONEYDEW_MELON" ]	:= 0x0c75;
	food [ "ITEM_CANTELOUPE" ]		:= 0x0c7a;
	food [ "ITEM_LETTUCE" ]			:= 0x0c70;
	food [ "ITEM_CABBAGE" ]			:= 0x0c7c;
	food [ "ITEM_ONION" ]			:= 0x0c6d;
	food [ "ITEM_TURNIP" ]			:= 0x0d3a;
	food [ "ITEM_COCONUT" ]			:= 0x1726;
	food [ "ITEM_RAW_CHICKEN_LEG" ]	:= 0x1607;
	food [ "ITEM_RAW_LEG_OF_LAMB" ]	:= 0x1609;
	food [ "ITEM_HOPS_CONES" ]	:= 0x1aa2;
	food [ "ITEM_GRAPES2" ]		:= 0x09d1;
	food [ "ITEM_GREEN_TEA_LEAF" ]	:= 0x284b;
	food [ "ITEM_CORN" ]			:= 0x0c82;

	//products that are used as ingredients in other recipes
	food [ "ITEM_BALL_OF_DOUGH" ]		:= 0xc805;
	food [ "ITEM_SWEET_DOUGH" ]		:= 0xc801;
	food [ "ITEM_CAKE_BATTER" ]		:= 0xc802;
	food [ "ITEM_PIZZA_CRUST" ]		:= 0xc803;
	food [ "ITEM_PIE_CRUST" ]		:= 0xc804;
	food [ "ITEM_BATTER" ]			:= 0xc806;

	return food;
endfunction




///////////////////
//  This function returns a dictionary of all foodstuffs used in cooking relating them to their names.  its used in
//  the cookbook script to translate names
///////////////////

function define_food_name_dictionary ()
	var food := dictionary;

	//some tools used in cooking
	food [ "ITEM_MIXING_BOWL" ]		:= "mixing bowl";
	food [ "ITEM_ROLLING_PIN" ]		:= "rolling pin";
	food [ "ITEM_FRYING_PAN" ]		:= "frying pan";
	food [ "ITEM_EMPTY_PITCHER" ]		:= "empty pitcher";
	food [ "ITEM_EMPTY_GLASS" ]		:= "empty glass";

	//bottles, pitchers and glasses of liquid
	food [ "ITEM_BOTTLE_OF_LIQUOR" ]	:= "bottle of liquor";
	food [ "ITEM_BOTTLE_OF_ALE" ]		:= "bottle of ale";
	food [ "ITEM_BOTTLE_OF_WINE" ]	:= "bottle of wine";
	food [ "ITEM_JUG_OF_CIDER" ]		:= "jug of cider";

	food [ "ITEM_PITCHER_OF_LIQUOR" ]	:= "pitcher of liquor";
	food [ "ITEM_PITCHER_OF_ALE" ]	:= "pitcher of ale";
	food [ "ITEM_PITCHER_OF_WINE" ]	:= "pitcher of wine";
	food [ "ITEM_PITCHER_OF_CIDER" ]	:= "pitcher of cider";
	food [ "ITEM_PITCHER_OF_WATER" ]	:= "pitcher of water";
	food [ "ITEM_PITCHER_OF_MILK" ]	:= "pitcher of milk";

	food [ "ITEM_GLASS_OF_LIQUOR" ]	:= "glass of liquor";
	food [ "ITEM_GLASS_OF_ALE" ]		:= "glass of ale";
	food [ "ITEM_GLASS_OF_WINE" ]		:= "glass of wine";
	food [ "ITEM_GLASS_OF_CIDER" ]	:= "glass of cider";
	food [ "ITEM_GLASS_OF_WATER" ]	:= "glass of water";
	food [ "ITEM_GLASS_OF_MILK" ]		:= "glass of milk";

	//These items are special.  If they're called for in a recipe, it will call a special function that will
	//permit multiple forms of that item
	food [ "ITEM_GENERAL_WATER" ]		:= "cup of water";
	food [ "ITEM_GENERAL_MILK" ]		:= "cup of milk";
	food [ "ITEM_GENERAL_CHEESE" ]	:= "wedge of cheese";
	food [ "ITEM_REGULAR_FISH" ]		:= "regular fish";
	food [ "ITEM_RARE_FISH" ]		:= "rare fish";

	//primary food ingredients
	food [ "ITEM_FLOUR" ]			:= "cup of flour";
	food [ "ITEM_CORNMEAL" ]			:= "cup of cornmeal";
	food [ "ITEM_RAW_RIB" ]			:= "raw rib";
	food [ "ITEM_RAW_FISHSTEAK" ]		:= "raw fishsteak";
	food [ "ITEM_RAW_BIRD" ]			:= "raw bird";
	food [ "ITEM_RAW_HAM" ]			:= "raw ham";
	food [ "ITEM_BACON_SLAB" ]		:= "bacon slab";
	food [ "ITEM_RAW_SAUSAGE" ]		:= "raw sausage";
	food [ "ITEM_LARD" ]			:= "glob of lard";
	food [ "ITEM_EGG" ]				:= "egg";
	food [ "ITEM_GARLIC" ]			:= "clove of garlic";
	food [ "ITEM_HONEY" ]			:= "honey";
	food [ "ITEM_CARROT" ]			:= "carrot";
	food [ "ITEM_APPLE" ]			:= "apple";
	food [ "ITEM_PEACH" ]			:= "peach";
	food [ "ITEM_LEMON" ]			:= "lemon";
	food [ "ITEM_LIME" ]			:= "lime";
	food [ "ITEM_BANANA" ]			:= "banana";
	food [ "ITEM_PEAR" ]			:= "pear";
	food [ "ITEM_GRAPES" ]			:= "bunch of grapes";
	food [ "ITEM_DATES" ]			:= "dates";
	food [ "ITEM_PUMPKIN" ]			:= "pumpkin";
	food [ "ITEM_YELLOW_GOURD" ]		:= "yellow gourd";
	food [ "ITEM_GREEN_GOURD" ]		:= "green gourd";
	food [ "ITEM_SQUASH" ]			:= "squash";
	food [ "ITEM_WATERMELON" ]		:= "watermelon";
	food [ "ITEM_HONEYDEW_MELON" ]	:= "honeydew melon";
	food [ "ITEM_CANTELOUPE" ]		:= "canteloupe";
	food [ "ITEM_LETTUCE" ]			:= "head of lettuce";
	food [ "ITEM_CABBAGE" ]			:= "head of cabbage";
	food [ "ITEM_ONION" ]			:= "onion";
	food [ "ITEM_TURNIP" ]			:= "turnip";
	food [ "ITEM_COCONUT" ]			:= "coconut";
	food [ "ITEM_RAW_CHICKEN_LEG" ]	:= "raw chicken leg";
	food [ "ITEM_RAW_LEG_OF_LAMB" ]	:= "raw leg of lamb";
        food [ "ITEM_HOPS_CONES" ]      := "hops cones";
	food [ "ITEM_GRAPES2" ]		:= "bunch of grapes";
	food [ "ITEM_GREEN_TEA_LEAF" ]	:= "green tea leaf";
	food [ "ITEM_CORN" ]			:= "corn";

	//products that are used as ingredients in other recipes
	food [ "ITEM_BALL_OF_DOUGH" ]		:= "ball of dough";
	food [ "ITEM_SWEET_DOUGH" ]		:= "sweet dough";
	food [ "ITEM_CAKE_BATTER" ]		:= "cake batter";
	food [ "ITEM_PIZZA_CRUST" ]		:= "pizza crust";
	food [ "ITEM_PIE_CRUST" ]		:= "pie crust";
	food [ "ITEM_BATTER" ]			:= "batter";

	return food;
endfunction
///////////////////
//  This function searches for water in the player's backpack.  It starts by looking for a glass of water.  If that's
//  not found, it then looks for a pitcher of water
///////////////////

function special_find_water (character, amount)
	var found_ingredient;

	//first check to see if amount is a positive number
	if (amount < 0)
		return 0;

	//then check to see if only 1 unit of water is needed.  If so, we can check for it a much faster way
	elseif (amount == 1 or amount == 0)
		//first try searching for a glass of water
		var found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_GLASS_OF_WATER"] );
		if (found_ingredient)
			if (!ReserveItem(found_ingredient)) return 0; endif
			return 1;
		endif

		//otherwise try looking for a pitcher of water
		found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_PITCHER_OF_WATER"] );
		if (found_ingredient)
			if (!ReserveItem(found_ingredient)) return 0; endif
			return 1;
		endif

	//if more than 1 unit of water is needed, we'll have to do it a slower way
	else
		var water_count := 0;
		foreach item in enumerateitemsincontainer(character.backpack)
			if (item.objtype == food_dictionary ["ITEM_GLASS_OF_WATER"])
				water_count := water_count + 1;
				if (!ReserveItem(item)) return 0; endif
			elseif (item.objtype == food_dictionary ["ITEM_PITCHER_OF_WATER"])
				if ( GetObjProperty (item, "fullness") )
					water_count := water_count + GetObjProperty (item, "fullness");
				else
					water_count := water_count + 4;
				endif
				if (!ReserveItem(item)) return 0; endif
			endif
			if (water_count >= amount)
				return 1;
			endif
		endforeach
	endif

	//if we still haven't found what we're looking for, fail
	return 0;

endfunction




///////////////////
//  similar to the previous function, this does pretty much the same thing with milk (doesn't yet do multiples)
///////////////////

function special_find_milk (character)

	//first try searching for a glass of milk
	var found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_GLASS_OF_MILK"] );
	if (found_ingredient)
		if (!ReserveItem(found_ingredient)) return 0; endif
		return 1;
	endif

	//otherwise try looking for a pitcher of water
	found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_PITCHER_OF_MILK"] );
	if (found_ingredient)
		if (!ReserveItem(found_ingredient)) return 0; endif
		return 1;
	endif

	//otherwise fail
	return 0;

endfunction




///////////////////
//  again for cheese
///////////////////

function special_find_cheese (character, amount)

	//count of all cheese found;
	var cheese_count := 0;

	//search for a wheel of cheese
	var found_ingredient := FindObjtypeInContainer (character.backpack, 0x97e );
	if (found_ingredient)
		if (!ReserveItem(found_ingredient)) return 0; endif
		cheese_count := found_ingredient.amount * 3;
	endif

	//then look for a large wedge of cheese
	found_ingredient := FindObjtypeInContainer (character.backpack, 0x97d );
	if (found_ingredient)
		if (!ReserveItem(found_ingredient)) return 0; endif
		cheese_count := cheese_count + found_ingredient.amount * 2;
	endif

	//then look for a small wedge of cheese
	found_ingredient := FindObjtypeInContainer (character.backpack, 0x97c );
	if (found_ingredient)
		if (!ReserveItem(found_ingredient)) return 0; endif
		cheese_count := cheese_count + found_ingredient.amount;
	endif

	//now check to see if enough cheese was found
	if (cheese_count >= amount)
		return 1;
	else
		return 0;
	endif

endfunction




///////////////////
//  This function searches the player's backpack for water and destroys it.
///////////////////

function special_destroy_water (character, amount)
	var found_ingredient;
	var i := 0;

	//check to make sure amount is at least 1
	if (amount <= 0)
		return;
	endif

	repeat
		//first see if they have a glass of water
		found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_GLASS_OF_WATER"] );
		if (found_ingredient)
			//if so, create an empty glass
			var the_glass := CreateItemInBackpack (character, food_dictionary ["ITEM_EMPTY_GLASS"] );

			//if the full glass has a saved name, rename the new glass
			if (GetObjProperty (found_ingredient, "EmptyName") )
				the_glass.name := GetObjProperty (found_ingredient, "EmptyName");
			endif

			//destroy the drink
			SubtractAmount( found_ingredient, 1 );

			i :=  i + 1;

		else
			//otherwise try looking for a pitcher of water
			found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_PITCHER_OF_WATER"] );
			if (found_ingredient)

				//if we found one, reduce the amount of water in the pitcher by 1
				reduce_container_fullness (character, found_ingredient);

				i :=  i + 1;

			//emergency escape exit.  if it couldn't find a pitcher or a glass of water and it still needed more.
			//this should prevent an infinite loop.  Since the water items were already found and reserved, 
			//this really shouldn't happen.  Better safe than sorry...
			else
				return;
			endif

		endif
	until (i >= amount);
endfunction




///////////////////
//  Once again similar to the previous function, this one used up milk
///////////////////

function special_destroy_milk (character)

	//first see if they have a glass of milk
	var found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_GLASS_OF_MILK"] );
	if (found_ingredient)
		//if so, create an empty glass
		var the_glass := CreateItemInBackpack (character, food_dictionary ["ITEM_EMPTY_GLASS"] );

		//if the full glass has a saved name, rename the new glass
		if (GetObjProperty (found_ingredient, "EmptyName") )
			the_glass.name := GetObjProperty (found_ingredient, "EmptyName");
		endif

		//destroy the drink
		SubtractAmount( found_ingredient, 1 );
		return;

	endif

	//otherwise try looking for a pitcher of milk
	found_ingredient := FindObjtypeInContainer (character.backpack, food_dictionary ["ITEM_PITCHER_OF_MILK"]);
	if (found_ingredient)
		//if we found one, reduce the amount of water in the pitcher by 1
		reduce_container_fullness (character, found_ingredient);
	endif
endfunction




///////////////////
//  again for cheese
///////////////////

function special_destroy_cheese (character, amount)

	repeat
		//start with a small wedge of cheese
		var found_ingredient := FindObjtypeInContainer (character.backpack, 0x97c );
		if (found_ingredient)
			SubtractAmount (found_ingredient, 1);
			amount := amount - 1;

		//search for a large wedge of cheese
		else
			found_ingredient := FindObjtypeInContainer (character.backpack, 0x97d );
			if (found_ingredient)
				SubtractAmount (found_ingredient, 1);
				CreateItemInBackpack (character, 0x97c);
				amount := amount - 1;

			//then look for a wheel of cheese
			else
				found_ingredient := FindObjtypeInContainer (character.backpack, 0x97e );
				if (found_ingredient)
					SubtractAmount (found_ingredient, 1);
					CreateItemInBackpack (character, 0x97d);
					amount := amount - 1;
				endif
			endif
		endif
	until (amount <= 0);
endfunction


///////////////////
//  2026-09-14: fish-family ingredients -- same "special" pattern as water/
//  milk/cheese above, but for "any Regular-tier fish" / "any Rare-tier
//  fish" (pkg/std/fishing/catchtable.cfg) instead of a fixed objtype.
//  Objtype lists below are every Regular/Rare Fish Objtype/ObjtypeEast/
//  ObjtypeSouth value in that file (Legendary and Crustaceans intentionally
//  excluded -- not part of any cooking recipe).
///////////////////

function IsRegularFishObjtype(objtype)
	return objtype in {
		0x30300, 0x30301, 0x30302, 0x30303, 0x30304, 0x30305, 0x30306, 0x30307,
		0x30308, 0x30309, 0x3030A, 0x3030B, 0x3030C, 0x3030D, 0x3030E, 0x3030F,
		0x30310, 0x30311, 0x30312, 0x30313, 0x30314, 0x30315, 0x30316, 0x30317,
		0x30318, 0x30319, 0x3031A, 0x3031B, 0x3031C, 0x30325, 0x30326, 0x30327,
		0x30328, 0x30329, 0x3032A, 0x3032B, 0x3032C, 0x30335, 0x30336, 0x30337,
		0x30338, 0x30339
	};
endfunction

function IsRareFishObjtype(objtype)
	return objtype in {
		0x3032D, 0x3032E, 0x3031D, 0x30321, 0x3031E, 0x30322, 0x3033A, 0x3033B,
		0x3033C, 0x3033D, 0x30342, 0x30343, 0x30344, 0x30345, 0x30346, 0x30347,
		0x3034C, 0x3034D, 0x3034E, 0x3034F, 0x30354, 0x30355, 0x30356, 0x30357,
		0x30360, 0x30361
	};
endfunction

function special_find_regular_fish(character, amount)
	if (amount <= 0)
		return 1;
	endif
	var found_count := 0;
	foreach item in enumerateitemsincontainer(character.backpack)
		if (IsRegularFishObjtype(item.objtype))
			if (!ReserveItem(item)) return 0; endif
			found_count := found_count + item.amount;
			if (found_count >= amount)
				return 1;
			endif
		endif
	endforeach
	return 0;
endfunction

function special_find_rare_fish(character, amount)
	if (amount <= 0)
		return 1;
	endif
	var found_count := 0;
	foreach item in enumerateitemsincontainer(character.backpack)
		if (IsRareFishObjtype(item.objtype))
			if (!ReserveItem(item)) return 0; endif
			found_count := found_count + item.amount;
			if (found_count >= amount)
				return 1;
			endif
		endif
	endforeach
	return 0;
endfunction

function special_destroy_regular_fish(character, amount)
	repeat
		var found_ingredient := 0;
		foreach item in enumerateitemsincontainer(character.backpack)
			if (IsRegularFishObjtype(item.objtype))
				found_ingredient := item;
				break;
			endif
		endforeach
		if (!found_ingredient)
			return;
		endif
		SubtractAmount(found_ingredient, 1);
		amount := amount - 1;
	until (amount <= 0);
endfunction

function special_destroy_rare_fish(character, amount)
	repeat
		var found_ingredient := 0;
		foreach item in enumerateitemsincontainer(character.backpack)
			if (IsRareFishObjtype(item.objtype))
				found_ingredient := item;
				break;
			endif
		endforeach
		if (!found_ingredient)
			return;
		endif
		SubtractAmount(found_ingredient, 1);
		amount := amount - 1;
	until (amount <= 0);
endfunction




///////////////////
//  This function decreases the amount of liquid in a container, such as a bottle or pitcher.  If the container
//  is empty, it destroys the container.  If the item has the property return_type defined, one of that item is
//  then created
///////////////////

function reduce_container_fullness (character, container)
	//get how full the container is
	var fullness := GetObjProperty (container, "fullness");

	//load some data about the container from itemdesc
	var cfgfile := ReadConfigFile( "itemdesc" );
	var elem := FindConfigElem( cfgfile, container.objtype );
	var drink_name := GetConfigString( elem, "drink_name" );
	var return_type := GetConfigInt (elem, "return_type");

	//if the container has no fullness, that means its new.  Give it 3 more uses and rename it
	if (! fullness)
		SetObjProperty (container, "fullness", 3);
		container.name := "a mostly full " + drink_name;

	//otherwise, reduce it's fullness
	elseif (fullness == 3)
		SetObjProperty (container, "fullness", 2);
		container.name := "half a " + drink_name;
	elseif (fullness == 2)
		SetObjProperty (container, "fullness", 1);
		container.name := "an almost empty " + drink_name;

	//if it has no more uses left, return something,  if applicable, and destroy the item
	else
		if (return_type)
			var empty_container := CreateItemInBackpack (character, return_type );

			//if the full container has a saved name, rename the new container
			if (GetObjProperty (container, "EmptyName") )
				empty_container.name := GetObjProperty (container, "EmptyName");
			endif
		endif
		destroyitem (container);
	endif
endfunction
