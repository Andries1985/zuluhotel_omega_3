///////////////////
//  This function returns a string that is the name of the keg, dependant on the type of potion inside it
//////////////////

function get_keg_name (potion_objtype, potion_subtype)
	var the_string := 0;
	case (potion_objtype)
		0xdc01:		the_string := "a keg of light healing potions";
		0xdc02:		the_string := "a keg of healing potions";
		0xdc03:		the_string := "a keg of greater healing potions";
		0xdc04:		the_string := "a keg of lesser poison potions";
		0xdc05:		the_string := "a keg of poison potions";
		0xdc06:		the_string := "a keg of greater poison potions";
		0xdc07:		the_string := "a keg of greater explosion potions";
		0xdc08:		the_string := "a keg of lesser explosion potions";
		0xdc09:		the_string := "a keg of explosion potions";
		0xdc0b:		the_string := "a keg of lesser agility potions";
		0xdc0c:		the_string := "a keg of agility potions";
		0xdc0d:		the_string := "a keg of greater agility potions";
		0xdc0e:		the_string := "a keg of refresh potions";
		0xdc0f:		the_string := "a keg of greater refresh potions";
		0xdc10:		the_string := "a keg of lesser strength potions";
		0xdc11:		the_string := "a keg of strength potions";
		0xdc12:		the_string := "a keg of greater strength potions";
		0xdc14:		the_string := "a keg of lesser cure potions";
		0xdc15:		the_string := "a keg of cure potions";
		0xdc16:		the_string := "a keg of greater cure potions";
		0xdc17:		the_string := "a keg of deadly poison potions";
		0xdce6:		the_string := "a keg of nightsight potions";
		0x7059:
			case (potion_subtype)
				23:	the_string := "a keg of invisibility potions";
				24:	the_string := "a keg of Phandel's Fine Intellect potions";
				25:	the_string := "a keg of Phandel's Fabulous Intellect potions";
				26:	the_string := "a keg of Phandel's Fantastic Intellect potions";
				27:	the_string := "a keg of lesser Mego Invulnerability potions";
				28:	the_string := "a keg of Mego Invulnerability potions";
				29:	the_string := "a keg of greater Mego Invulnerability potions";
				31:	the_string := "a keg of Homeric Might potions";
				32:	the_string := "a keg of Greater Homeric Might potions";
				33:	the_string := "a keg of Tamla Heal potions";
				34:	the_string := "a keg of Taint's Minor Transmutation potions";
				35:	the_string := "a keg of Taint's Major Transmutation potions";
			endcase;
	endcase;

	return the_string;
endfunction




///////////////////
//  This function returns a string that is the name of the potion
//////////////////

function get_potion_name (potion_objtype, potion_subtype)
	var the_string := 0;
	case (potion_objtype)
		0xdc01:		the_string := "a light healing potion";
		0xdc02:		the_string := "a healing potion";
		0xdc03:		the_string := "a greater healing potion";
		0xdc04:		the_string := "a lesser poison potion";
		0xdc05:		the_string := "a poison potion";
		0xdc06:		the_string := "a greater poison potion";
		0xdc07:		the_string := "a greater explosion potion";
		0xdc08:		the_string := "a lesser explosion potion";
		0xdc09:		the_string := "an explosion potion";
		0xdc0b:		the_string := "a lesser agility potion";
		0xdc0c:		the_string := "an agility potion";
		0xdc0d:		the_string := "a greater agility potion";
		0xdc0e:		the_string := "a refresh potion";
		0xdc0f:		the_string := "a greater refresh potion";
		0xdc10:		the_string := "a lesser strength potion";
		0xdc11:		the_string := "a strength potion";
		0xdc12:		the_string := "a greater strength potion";
		0xdc14:		the_string := "a lesser cure potion";
		0xdc15:		the_string := "a cure potion";
		0xdc16:		the_string := "a greater cure potion";
		0xdc17:		the_string := "a deadly poison potion";
		0xdce6:		the_string := "a nightsight potion";
		0x7059:
			case (potion_subtype)
				23:	the_string := "an invisibility potion";
				24:	the_string := "Phandel's Fine Intellect";
				25:	the_string := "Phandel's Fabulous Intellect";
				26:	the_string := "Phandel's Fantastic Intellect";
				27:	the_string := "a lesser Mego Invulnerability potion";
				28:	the_string := "a Mego Invulnerability potion";
				29:	the_string := "a greater Mego Invulnerability potion";
				31:	the_string := "a Homeric Might potion";
				32:	the_string := "a Greater Homeric Might potion";
				33:	the_string := "a Tamla Heal potion";
				34:	the_string := "Taint's Minor Transmutation";
				35:	the_string := "Taint's Major Transmutation";
			endcase;
	endcase;

	return the_string;
endfunction




///////////////////
//  This function returns a dictionary of all reagents used in alchemy relating them to their names.
//  its used in the potionbook script to translate names
///////////////////

function define_reagent_name_dictionary ()
	var reagent := dictionary;

	//normal reagents
	reagent [ 0x0F78 ]		:= "bat wing";
	reagent [ 0x0F79 ]		:= "blackmoor";
	reagent [ 0x0F7a ]		:= "black pearl";
	reagent [ 0x0F7b ]		:= "blood moss";
	reagent [ 0x0F7c ]		:= "blood spawn";
	reagent [ 0x0F7d ]		:= "vial of blood";
	reagent [ 0x0F7e ]		:= "bone";
	reagent [ 0x0F7f ]		:= "brimstome";
	reagent [ 0x0F80 ]		:= "deamon bone";
	reagent [ 0x0F81 ]		:= "fertile dirt";
	reagent [ 0x0F82 ]		:= "dragon's blood";
	reagent [ 0x0F83 ]		:= "executioner's cap";
	reagent [ 0x0F84 ]		:= "garlic";
	reagent [ 0x0F85 ]		:= "ginseng";
	reagent [ 0x0F86 ]		:= "mandrake root";
	reagent [ 0x0F87 ]		:= "eye of newt";
	reagent [ 0x0F88 ]		:= "nightshade";
	reagent [ 0x0F89 ]		:= "obsidian";
	reagent [ 0x0F8a ]		:= "pig iron";
	reagent [ 0x0F8b ]		:= "pumice";
	reagent [ 0x0F8c ]		:= "sulfurous ash";
	reagent [ 0x0F8d ]		:= "spider's silk";
	reagent [ 0x0F8e ]		:= "serpent scale";
	reagent [ 0x0F8f ]		:= "volcanic ash";
	reagent [ 0x0F90 ]		:= "dead wood";
	reagent [ 0x0F91 ]		:= "wyrm's heart";

	//other potions
	reagent [ 0xDC01 ]		:= "l. heal potion";
	reagent [ 0xDC02 ]		:= "heal potion";
	reagent [ 0xDC03 ]		:= "gr. heal potion";
	reagent [ 0xDC04 ]		:= "l. poison potion";
	reagent [ 0xDC05 ]		:= "poison potion";
	reagent [ 0xDC06 ]		:= "gr. poison potion";
	reagent [ 0xDC07 ]		:= "gr. explosion pot.";
	reagent [ 0xDC08 ]		:= "l. explosion pot.";
	reagent [ 0xDC09 ]		:= "exposion potion";
	reagent [ 0xDC0b ]		:= "l. agility potion";
	reagent [ 0xDC0c ]		:= "agility potion";
	reagent [ 0xDC0d ]		:= "gr. agility pot.";
	reagent [ 0xDC0e ]		:= "refresh potion";
	reagent [ 0xDC0f ]		:= "full refresh pot.";
	reagent [ 0xDC10 ]		:= "l. strength pot.";
	reagent [ 0xDC11 ]		:= "strength potion";
	reagent [ 0xDC12 ]		:= "gr. strength pot.";
	reagent [ 0xDC14 ]		:= "l. cure potion";
	reagent [ 0xDC15 ]		:= "cure potion";
	reagent [ 0xDC16 ]		:= "gr. cure potion";
	reagent [ 0xDC17 ]		:= "deadly poison pot.";
	reagent [ 0xDCE6 ]		:= "nightsight potion";
	reagent [ 0xff86 ]		:= "tamla heal potion";
	reagent [ 0xff93 ]		:= "elixer";

	//gems
	reagent [ 0x0F0F ]		:= "star sapphire";
	reagent [ 0x0F10 ]		:= "emerald";
	reagent [ 0x0F11 ]		:= "sapphire";
	reagent [ 0x0F12 ]		:= "sapphire";
	reagent [ 0x0F13 ]		:= "ruby";
	reagent [ 0x0F14 ]		:= "ruby";
	reagent [ 0x0F15 ]		:= "citrine";
	reagent [ 0x0F16 ]		:= "amethyst";
	reagent [ 0x0F17 ]		:= "amethyst";
	reagent [ 0x0F18 ]		:= "tourmaline";
	reagent [ 0x0F19 ]		:= "sapphire";
	reagent [ 0x0F1a ]		:= "ruby";
	reagent [ 0x0F1b ]		:= "star sapphire";
	reagent [ 0x0F1c ]		:= "ruby";
	reagent [ 0x0F1d ]		:= "ruby";
	reagent [ 0x0F1e ]		:= "tourmaline";
	reagent [ 0x0F1f ]		:= "sapphire";
	reagent [ 0x0F20 ]		:= "tourmaline";
	reagent [ 0x0F21 ]		:= "star sapphire";
	reagent [ 0x0F22 ]		:= "amethyst";
	reagent [ 0x0F23 ]		:= "citrine";
	reagent [ 0x0F24 ]		:= "citrine";
	reagent [ 0x0F25 ]		:= "amber";
	reagent [ 0x0F26 ]		:= "diamond";
	reagent [ 0x0F27 ]		:= "diamond";
	reagent [ 0x0F28 ]		:= "diamond";
	reagent [ 0x0F29 ]		:= "diamond";
	reagent [ 0x0F2a ]		:= "ruby";
	reagent [ 0x0F2b ]		:= "ruby";
	reagent [ 0x0F2c ]		:= "citrine";
	reagent [ 0x0F2d ]		:= "tourmaline";
	reagent [ 0x0F2e ]		:= "amethyst";
	reagent [ 0x0F2f ]		:= "emerald";
	reagent [ 0x0F30 ]		:= "diamond";

	//other stuff
	reagent [ 0x0F0E ]		:= "empty bottle";
	reagent [ 0x7052 ]		:= "troll hide";

	return reagent;
endfunction

