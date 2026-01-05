/////////////////////////////////////////////////////////////////////////////
//
// include/objtype.inc - Object Type definitions
//
// TODO: Put some order in there, this is a true mess
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x0000 to 0x3FFF - "Standard" objects
//
/////////////////////////////////////////////////////////////////////////////

const UOBJ_STONEWALL_NS := 0x0058;
const UOBJ_STONEWALL_EW := 0x0057;

const UOBJ_FISHSTEAK    := 0x097A;

const UOBJ_BOX          := 0x09aa;
const UOBJ_GOLD_COIN   := 0x0EED; // A gold coin

const UOBJ_FISH_START   := 0x09CC;
const UOBJ_FISH_END     := 0x09D0;

const UOBJ_SFISH_START  := 0x0DD6;
const UOBJ_SFISH_END    := 0x0DD9;

const UOBJ_HAIRS_END			:= 0x20d4;
const UOBJ_HAIRS_START			:= 0x203b;

const UOBJ_KINDLING     := 0x0DE1;
const UOBJ_CAMPFIRE     := 0x0DE3;   
const UOBJ_EMBERS       := 0x0DE9;       
const UOBJ_ASHES        := 0x0DEA;

const UOBJ_BANDAGE      := 0x0E21;
const UOBJ_BLANK_SCROLL := 0x0E34;

const UOBJ_BANKBOX      := 0x0E7C;

const UOBJ_DRUM         := 0x0E9C;
const UOBJ_TAMB         := 0x0E9D;
const UOBJ_HARP         := 0x0EB2;
const UOBJ_LUTE         := 0x0EB3;

const UOBJ_SPELLBOOK        := 0x0EFA;

const UOBJ_POISON_POTION    := 0x0F0A;
const UOBJ_EMPTY_BOTTLE     := 0x0F0E;

const UOBJ_GEM_START        := 0x0F0F;
const UOBJ_GEM_END          := 0x0f30;

const UOBJ_SHOVEL           := 0x0F39;

const UOBJ_XBOW             := 0x0F4F;

const UOBJ_BLACK_MOONGATE   := 0x1FD4;
const UOBJ_BLACK_MOONGATE2  := 0x7012;
const UOBJ_BLUE_MOONGATE    := 0x0F6C;
const UOBJ_BLUE_MOONGATE2   := 0x7010;
const UOBJ_RED_MOONGATE     := 0x0DDA;
const UOBJ_RED_MOONGATE2    := 0x7011;
const UOBJ_WHITE_MOONGATE   := 0x1FE7;
const UOBJ_WHITE_MOONGATE2  := 0x7013;

const UOBJ_CLAY		    := 0xee19;
const UOBJ_SAND		    := 0xee18;
const UOBJ_WOOL		    := 0x0df8;


const UOBJ_REAGENT_START    := 0x0f78;
const UOBJ_REAGENT_END	    := 0x0f91;

const UOBJ_BATWING	    := 0x0F78;
const UOBJ_BLACKMOOR	    := 0x0F79;
const UOBJ_BLACK_PEARL      := 0x0F7A;
const UOBJ_BLOOD_MOSS       := 0x0F7B;
const UOBJ_BLOODSPAWN	    := 0x0F7C;
const UOBJ_VIAL_OF_BLOOD    := 0x0F7D;
const UOBJ_BONE		    := 0x0F7E;
const UOBJ_BRIMSTONE        := 0x0F7F;
const UOBJ_DAEMON_BONE	    := 0x0F80;
const UOBJ_FERTILE_DIRT     := 0x0F81;
const UOBJ_DRAGONS_BLOOD    := 0x0F82;
const UOBJ_EXECUTIONERS_CAP := 0x0F83;
const UOBJ_GARLIC           := 0x0F84;
const UOBJ_GINSENG          := 0x0F85;
const UOBJ_MANDRAKE_ROOT    := 0x0F86;
const UOBJ_EYE_OF_NEWT	    := 0x0F87;
const UOBJ_NIGHTSHADE       := 0x0F88;
const UOBJ_OBSIDIAN         := 0x0F89;
const UOBJ_PIG_IRON         := 0x0F8A;
const UOBJ_PUMICE	    := 0x0F8B;
const UOBJ_SULPHUROUS_ASH   := 0x0F8C;
const UOBJ_SPIDER_SILK      := 0x0F8D;
const UOBJ_SERPENT_SCALE  := 0x0F8E;
const UOBJ_VOLCANIC_ASH     := 0x0F8F;
const UOBJ_DEAD_WOOD	    := 0x0F90;
const UOBJ_WYRMS_HEART	    := 0x0F91;


const UOBJ_SEWING_KIT   := 0x0F9D;
const UOBJ_CLOTH        := 0x0F95;         

const UOBJ_THREAD_START := 0x0FA0;            
const UOBJ_THREAD_END   := 0x0FA1;
const UOBJ_BOLT_START   := 0x0F95;
const UOBJ_BOLT_END     := 0x0F9C;
const UOBJ_MUTLI_CLOTH  := 0x1765;

const UOBJ_HIDE1	:= 0x1078;
const UOBJ_HIDE2	:= 0x1079;

const UOBJ_HIDES_START	:= 0x7020;
const UOBJ_HIDES_END	:= 0x7030; // Missing Orc, Terathan, Angel and Silverdragon

//Human Crafting
const UOBJ_ANVIL1       := 0x0FAF;
const UOBJ_ANVIL2       := 0x0FB0;
const UOBJ_SMALL_FORGE  := 0x0FB1;  // see also UOBJ_FORGE_START..UOBJ_FORGE_END
const UOBJ_FORGE_START  := 0x197A;  // see also UOBJ_SMALL_FORGE          
const UOBJ_FORGE_END    := 0x19A9;

//Elven Crafting
const UOBJ_ELVEN_ANVIL1 := 0x2DD5;
const UOBJ_ELVEN_ANVIL2 := 0x2DD6;
const UOBJ_ELVEN_FORGE  := 0x2DD8;

//Gargoyle Crafting
const UOBJ_SOULFORGE_ANVIL_START 	:= 0x4254; // Small Sized Soulforged Anvil
const UOBJ_SOULFORGE_ANVIL_END 		:= 0x4257; // Small Sized Soulforged Anvil

const UOBJ_MINISOULFORGE_START	:= 0x44C7; // Mini Soulforge
const UOBJ_MINISOULFORGE_END		:= 0x44CA; // Mini Soulforge

const UOBJ_LARGESOULFORGE_START := 0x4263; //Soulforge and Anvil
const UOBJ_LARGESOULFORGE_END   := 0x4272; //Soulforge and Anvil


const UOBJ_COPPER_KEY   := 0x100E;
const UOBJ_GOLD_KEY     := 0x100F;
const UOBJ_IRON_KEY     := 0x1010;
const UOBJ_KEY_RING     := 0x1011;
const UOBJ_MAGIC_KEY    := 0x1012;
const UOBJ_RUSTY_IRON_KEY := 0x1013;

const UOBJ_CLOCK		:= 0x104c;
const UOBJ_CLOCK_FRAME		:= 0x104e;
const UOBJ_CLOCK_PARTS		:= 0x104f;
const UOBJ_AXLE_AND_GEARS	:= 0x1051;
const UOBJ_GEARS		:= 0x1053;
const UOBJ_HINGE		:= 0x1055;
const UOBJ_SEXTANT		:= 0x1057;
const UOBJ_SEXTANT_PARTS	:= 0x1059;
const UOBJ_AXLE			:= 0x105b;
const UOBJ_SPRINGS		:= 0x105d;

const UOBJ_KEG		:= 0x0E7F;
const UOBJ_BARREL	:= 0x0E77;
const UOBJ_BARREL_TAP	:= 0x1004;
const UOBJ_BARREL_LID	:= 0x1DB8;
const UOBJ_BARREL_STAVE	:= 0x1EB1;
const UOBJ_BARREL_HOOPS	:= 0x10E1;

const UOBJ_POTION_KEG	:= 0x1940;

const UOBJ_LOOM_START   := 0x105F;
const UOBJ_LOOM_END     := 0x1066;            

const UOBJ_DUMMYSWING_NS     := 0x1071;
const UOBJ_DUMMYFREEZE_NS    := 0x1070;
const UOBJ_DUMMYSWING_EW     := 0x1075;
const UOBJ_DUMMYFREEZE_EW    := 0x1074;

const UOBJ_BLOOD        := 0x122D;

const UOBJ_BOW          := 0x13B2;
const UOBJ_HEAVY_XBOW   := 0x13FD;

const UOBJ_MAP1         := 0x14EB;
const UOBJ_MAP2         := 0x14EC;
const UOBJ_ROLLED_MAP1  := 0x14ED;
const UOBJ_ROLLED_MAP2  := 0x14EE;

const UOBJ_HOUSE_DEED1  := 0x14ef;
const UOBJ_SHIP_DEED1   := 0x14F1;
const UOBJ_SHIP_DEED2   := 0x14F2;

const UOBJ_CHEST        := 0x0E40;

const UOBJ_FEATHER      := 0x1BD1;
const UOBJ_SHAFTS       := 0x1BD4;
const UOBJ_LOGS         := 0x1BDD;
const UOBJ_LOGS_GODDESS	:= 0x1BDE;
const UOBJ_LOGS_START	:= 0x7300;
const UOBJ_LOGS_END	:= 0x7316;

const UOBJ_CLOTH_START	:= 0x175D;
const UOBJ_CLOTH_END	:= 0x1768;

const UOBJ_CARPET_START	:= 0x0AD1;
const UOBJ_CARPET_END	:= 0x0AD9;

const UOBJ_GLASS        := 0xee20;

const UOBJ_ARM          := 0x1CEC;
const UOBJ_LEG          := 0x1CE2;
const UOBJ_TRUNK        := 0x1CEE;
const UOBJ_HEART        := 0x1CEF;
const UOBJ_HEAD         := 0x1DAE;

const UOBJ_TINKER_TOOLS := 0x1EBC;


const UOBJ_RUNE			:= 0x1F14;
const UOBJ_GATE_SCROLL		:= 0x1f60;
const UOBJ_RECALL_SCROLL	:= 0x1f4c;
const UOBJ_CODEX_DAMNORUM	:= 0xa100;
const UOBJ_EARTH_BOOK		:= 0xb100;
const UOBJ_TOME_OF_ENCHANTMENT	:= 0xc100;
const UOBJ_RUNEBOOK		:= 0x7100;

const UOBJ_CORPSE       := 0x2006;

const UOBJ_PARAFIELD_NS   := 0x3967;
const UOBJ_PARAFIELD_EW   := 0x3979;
const UOBJ_POISONFIELD_NS := 0x3915;
const UOBJ_POISONFIELD_EW := 0x3920;
const UOBJ_EFIELD_EW      := 0x3955;
const UOBJ_EFIELD_NS      := 0x3946;
const UOBJ_FIREFIELD_NS   := 0x398c;
const UOBJ_FIREFIELD_EW   := 0x3996;

const GID_PLANK_EXTENDED_FACING_WEST   := 0x3ed5;
const GID_PLANK_RETRACTED_FACING_WEST  := 0x3eb1;
const GID_PLANK_EXTENDED_FACING_EAST   := 0x3ed4;
const GID_PLANK_RETRACTED_FACING_EAST  := 0x3eb2;
const GID_PLANK_EXTENDED_FACING_SOUTH  := 0x3e84;
const GID_PLANK_RETRACTED_FACING_SOUTH := 0x3e85;
const GID_PLANK_EXTENDED_FACING_NORTH  := 0x3e89;
const GID_PLANK_RETRACTED_FACING_NORTH := 0x3e8a;

// ick! these are fine for humans, but not true for items..
// these are more of a CID.
const UOBJ_HUMAN_MALE   := 0x190;
const UOBJ_HUMAN_FEMALE := 0x191;

/////////////////////////////////////////////////////////////////////////////
//
// 0x4000 to 0x4FFF - Multis
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x5000 to 0xEFFF - To Be Determined
//
/////////////////////////////////////////////////////////////////////////////

	//
	// TODO: reserve some of these ranges for weapons, armor, wands
	//       explicitly leave an area for end users to play with
	//

// recommendation: start these out somewhere like 0x6300, and reserve
// enough space for N (say, 64) colors of ore and ingots
// they don't all have to be defined yet, just reserved.
const UOBJ_ORE_START	    := 0x6009;
const UOBJ_IRON_ORE         := 0x6009;
const UOBJ_BRONZE_ORE       := 0x600a;  
const UOBJ_DULLCOPPER_ORE   := 0x600b;
const UOBJ_COPPER_ORE       := 0x600c;
const UOBJ_ONYX_ORE         := 0x600d;
const UOBJ_PYRITE_ORE       := 0x600e;
const UOBJ_MALACHITE_ORE    := 0x600f;
const UOBJ_AZURITE_ORE      := 0x6010;
const UOBJ_ORE_END	    := 0x6010;

// Added by GM Alcatraz - Zuluhotel.dynip.com
const UOBJ_ORE2_START	    := 0xc500;
const UOBJ_PLATINUM_ORE     := 0xc500;
const UOBJ_LAVAROCK_ORE     := 0xc501;
const UOBJ_MYSTIC_ORE       := 0xc502;
const UOBJ_ZULU_ORE         := 0xc503;
const UOBJ_SPIKE_ORE        := 0xc504;
const UOBJ_FRUITY_ORE       := 0xc505;
const UOBJ_GREYVIXEN_ORE    := 0xc506;
const UOBJ_SILVERROCK_ORE   := 0xc507;
const UOBJ_SPECTRAL_ORE     := 0xc508;
const UOBJ_UNDEAD_ORE       := 0xc509;
const UOBJ_DARKPAGAN_ORE    := 0xc50a;
const UOBJ_OLDBRITAIN_ORE   := 0xc50b;
const UOBJ_VIRGINITY_ORE    := 0xc50c;
const UOBJ_BLACKDWARF_ORE   := 0xc50d;
const UOBJ_REDELVEN_ORE     := 0xc50e;
const UOBJ_ORE2_END	    := 0xc50e;

const UOBJ_ORE3_START	    := 0xc530;
const UOBJ_DRIPSTONE_ORE    := 0xc533;
const UOBJ_EXECUTOR_ORE     := 0xc534;
const UOBJ_PEACHBLUE_ORE    := 0xc535;
const UOBJ_DESTRUCTION_ORE  := 0xc536;
const UOBJ_ANRA_ORE         := 0xc537;
const UOBJ_GODDESS_ORE      := 0xc538;
const UOBJ_CRYSTAL_ORE      := 0xc539;
const UOBJ_DOOM_ORE         := 0xc53a;
const UOBJ_SAPPHIRE_ORE     := 0xc53b;
const UOBJ_RUBY_ORE         := 0xc53c;
const UOBJ_DIAMOND_ORE      := 0xc53d;
const UOBJ_DAMASIN_ORE      := 0xc53e;
const UOBJ_KORGAN_ORE	    := 0Xc53f;
const UOBJ_AURORA_ORE	    := 0Xc540;
const UOBJ_ARASARN_ORE	    := 0Xc541;
const UOBJ_ORACLE_ORE	    := 0Xc542;
const UOBJ_VULCAN_ORE	    := 0Xc543;
const UOBJ_SHING_ORE	    := 0Xc544;
const UOBJ_LEVIATHAN_ORE    := 0Xc545;
const UOBJ_SANCTUARY_ORE    := 0Xc546;
const UOBJ_ORE3_END	    := 0xc546;
//
//
const UOBJ_GOLD_INGOT       := 0x1be9;
const UOBJ_INGOTS_START     := 0x6011;       
const UOBJ_IRON_INGOT       := 0x6011;
const UOBJ_BRONZE_INGOT     := 0x6012;  
const UOBJ_DULLCOPPER_INGOT := 0x6013;
const UOBJ_COPPER_INGOT     := 0x6014;
const UOBJ_ONYX_INGOT       := 0x6015;
const UOBJ_PYRITE_INGOT     := 0x6016;
const UOBJ_MALACHITE_INGOT  := 0x6017;
const UOBJ_AZURITE_INGOT    := 0x6018;
const UOBJ_INGOTS_END       := 0x6018;   
// Added by GM Alcatraz - Zuluhotel.dynip.com
const UOBJ_INGOTS2_START    := 0xc50f;
const UOBJ_PLATINUM_INGOT   := 0xc510;
const UOBJ_LAVAROCK_INGOT   := 0xc511;
const UOBJ_MYSTIC_INGOT     := 0xc512;
const UOBJ_ZULU_INGOT       := 0xc513;
const UOBJ_SPIKE_INGOT      := 0xc514;
const UOBJ_FRUITY_INGOT     := 0xc515;
const UOBJ_GREYVIXEN_INGOT  := 0xc516;
const UOBJ_SILVERROCK_INGOT := 0xc517;
const UOBJ_SPECTRAL_INGOT   := 0xc518;
const UOBJ_UNDEAD_INGOT     := 0xc519;
const UOBJ_DARKPAGAN_INGOT  := 0xc51a;
const UOBJ_OLDBRITAIN_INGOT := 0xc51b;
const UOBJ_VIRGINITY_INGOT  := 0xc51c;
const UOBJ_BLACKDWARF_INGOT := 0xc51d;
const UOBJ_REDELVEN_INGOT   := 0xc51e;
const UOBJ_DRIPSTONE_INGOT  := 0xc51f;
const UOBJ_EXECUTOR_INGOT   := 0xc520;
const UOBJ_PEACHBLUE_INGOT  := 0xc521;
const UOBJ_DESTRUCTION_INGOT := 0xc522;
const UOBJ_ANRA_INGOT       := 0xc523;
const UOBJ_GODDESS_INGOT    := 0xc524;
const UOBJ_CRYSTAL_INGOT    := 0xc525;
const UOBJ_DOOM_INGOT       := 0xc526;
const UOBJ_SAPPHIRE_INGOT   := 0xc527;
const UOBJ_RUBY_INGOT       := 0xc528;
const UOBJ_DIAMOND_INGOT    := 0xc529;
const UOBJ_DAMASIN_INGOT    := 0xc52a;
const UOBJ_KORGAN_INGOT     := 0Xc52b;
const UOBJ_AURORA_INGOT	    := 0Xc52c;
const UOBJ_ARASARN_INGOT    := 0Xc52d;
const UOBJ_ORACLE_INGOT     := 0Xc52e;
const UOBJ_VULCAN_INGOT	    := 0Xc52f;
const UOBJ_SHING_INGOT	    := 0Xc530;
const UOBJ_LEVIATHAN_INGOT  := 0Xc531;
const UOBJ_SANCTUARY_INGOT  := 0Xc532;
const UOBJ_INGOTS2_END      := 0xc532;
//


/////////////////////////////////////////////////////////////////////////////
//
// 0xF000 to 0xFFFF - Reserved by system
//
/////////////////////////////////////////////////////////////////////////////

	//
	// Ship Components
	//
const EXTOBJ_TILLERMAN       := 0xF010;
const EXTOBJ_PORT_PLANK      := 0xF011;
const EXTOBJ_STARBOARD_PLANK := 0xF012;
const EXTOBJ_HOLD            := 0xF013;

///////////////////
//  Containers
///////////////////

Const UOBJ_BACKPACK			:= 0x0E75;
Const UOBJ_BAG				:= 0x0E76;
Const UOBJ_POUCH			:= 0x0E79;
Const UOBJ_BASKET			:= 0x0990;
Const UOBJ_PICNIC_BASKET		:= 0x0E7A;
Const UOBJ_WOODEN_BOX_1		:= 0x0E7D;
Const UOBJ_WOODEN_BOX_2		:= 0x09AA;
Const UOBJ_TRASHBARREL		:= 0x7007;
const UOBJ_METALCHEST := 0x0E7C; // A Metal Chest

//////////////////////////////////////////////////////////////////////////
//
// From OBJTYPES.INC
//
//////////////////////////////////////////////////////////////////////////

const UOBJ_AGILITY_POTION		:= 0xdc0c;
const UOBJ_AMBER			:= 0x0f25;
const UOBJ_ARROW_NORMAL			:= 0x0f3f;
const UOBJ_AUCTION_BOX			:= 0xbf00;
const UOBJ_BLANK_MAP			:= 0x6001;
const UOBJ_BLANK_SCROLL2		:= 0x0Ef3;
const UOBJ_BLOODY_BANDAGE		:= 0x0e20;
const UOBJ_BOLT_OF_CLOTH1		:= 0x0f95;
const UOBJ_BOLT_OF_CLOTH2		:= 0x0f96;
const UOBJ_BOLT_OF_CLOTH3		:= 0x0f97;
const UOBJ_BOLT_OF_CLOTH4		:= 0x0f98;
const UOBJ_BOLT_OF_CLOTH5		:= 0x0f99;
const UOBJ_BOLT_OF_CLOTH6		:= 0x0f9a;
const UOBJ_BOLT_OF_CLOTH7		:= 0x0f9b;
const UOBJ_BOLT_OF_CLOTH8		:= 0x0f9c;
const UOBJ_BOOK1			:= 0x0fbd;
const UOBJ_BOOK2			:= 0x0fbe;
const UOBJ_BOOK3			:= 0x0fef;
const UOBJ_BOOK4			:= 0x0ff0;
const UOBJ_BOOK5			:= 0x0ff1;
const UOBJ_BOOK6			:= 0x0ff2;
const UOBJ_BOOK7			:= 0x0ff3;
const UOBJ_BOOK8			:= 0x0ff4;
const UOBJ_BOOK_OF_TRUTH		:= 0x1c13;
const UOBJ_BOOKCASE1			:= 0x0a97;
const UOBJ_BOOKCASE2			:= 0x0a98;
const UOBJ_BOOKCASE3			:= 0x0a99;
const UOBJ_BOOKCASE4			:= 0x0a9a;
const UOBJ_BOOKCASE5			:= 0x0a9b;
const UOBJ_BOOKCASE6			:= 0x0a9c;
const UOBJ_BRASS_SIGN			:= 0x0bd2;
const UOBJ_BRASS_SIGN2			:= 0x0bd1;
const UOBJ_CHEST_OF_DRAWERS		:= 0x0a2c;
const UOBJ_CLOTH1			:= 0x175d;
const UOBJ_CLOTH2			:= 0x175e;
const UOBJ_CLOTH3			:= 0x175f;
const UOBJ_CLOTH4			:= 0x1760;
const UOBJ_CLOTH5			:= 0x1761;
const UOBJ_CLOTH6			:= 0x1762;
const UOBJ_CLOTH7			:= 0x1763;
const UOBJ_CLOTH8			:= 0x1764;
const UOBJ_CLOTH9			:= 0x1765;
const UOBJ_CLOTH10			:= 0x1767;
const UOBJ_CROSSBOW_BOLT_NORMAL		:= 0x1bfb;
const UOBJ_CURE_POTION			:= 0xdc15;
const UOBJ_DEADLY_POISON_POTION		:= 0xdc17;
const UOBJ_DEATH_SHROUD			:= 0x204e;
const UOBJ_DIAMOND			:= 0x0f30;
const UOBJ_DOORS_END			:= 0x06f4;
const UOBJ_DOORS_START			:= 0x0675;
const UOBJ_EFIELD_EW_TEMP		:= 0x7105;
const UOBJ_EFIELD_NS_TEMP		:= 0x7104;
const UOBJ_EMPTY_BOOK_SHELF1		:= 0x0a9d;
const UOBJ_EMPTY_BOOK_SHELF2		:= 0x0a9e;
const UOBJ_EXPLORER			:= 0x6101;
const UOBJ_EXPLOSION_POTION		:= 0xdc08;
const UOBJ_FIREFIELD_EW_TEMP		:= 0x7103;
const UOBJ_FIREFIELD_NS_TEMP		:= 0x7102;
const UOBJ_GRASS_1			:= 0x177d;
const UOBJ_GREATER_AGILITY_POTION	:= 0xdc0d;
const UOBJ_GREATER_CURE_POTION		:= 0xdc16;
const UOBJ_GREATER_EXPLOSION_POTION	:= 0xdc09;
const UOBJ_GREATER_HEAL_POTION		:= 0xdc03;
const UOBJ_GREATER_POISON_POTION	:= 0xdc06;
const UOBJ_GREATER_STRENGTH_POTION	:= 0xdc12;
const UOBJ_HARP_BIG			:= 0x0eb1;
const UOBJ_HEAL_POTION			:= 0xdc02;
const UOBJ_HIDES_BALROG			:= 0x702f;
const UOBJ_HIDES_BALRON			:= 0x7026;
const UOBJ_HIDES_BEAR			:= 0x702d;
const UOBJ_HIDES_DRAGON			:= 0x7021;
const UOBJ_HIDES_GOLDEN_DRAGON		:= 0x702b;
const UOBJ_HIDES_ICE_CRYSTAL		:= 0x702e;
const UOBJ_HIDES_LAVA			:= 0x7027;
const UOBJ_HIDES_LICHE			:= 0x7029;
const UOBJ_HIDES_LIZARD			:= 0x7023;
const UOBJ_HIDES_NECROMANCER		:= 0x7028;
const UOBJ_HIDES_NORMAL1		:= 0x1078;
const UOBJ_HIDES_NORMAL2		:= 0x1079;
const UOBJ_HIDES_OSTARD			:= 0x7025;
const UOBJ_HIDES_RAT			:= 0x7020;
const UOBJ_HIDES_SERPENT		:= 0x7024;
const UOBJ_HIDES_TROLL			:= 0x7022;
const UOBJ_HIDES_WOLF			:= 0x702c;
const UOBJ_HIDES_WYRM			:= 0x702a;
const UOBJ_HOUSE_BUILT_DEED		:= 0x61fc;
const UOBJ_LESSER_AGILITY_POTION	:= 0xdc0b;
const UOBJ_LESSER_CURE_POTION		:= 0xdc14;
const UOBJ_LESSER_EXPLOSION_POTION	:= 0xdc07;
const UOBJ_LESSER_POISON_POTION		:= 0xdc04;
const UOBJ_LESSER_STRENGTH_POTION	:= 0xdc10;
const UOBJ_LIGHT_HEAL_POTION		:= 0xdc01;
const UOBJ_METAL_CHEST_EAST		:= 0x0e40;
const UOBJ_METAL_CHEST_SOUTH		:= 0x0e41;
const UOBJ_MORTAR			:= 0x0e9b;
const UOBJ_NIGHTSIGHT_POTION		:= 0xdc13;
const UOBJ_PAGAN_POTION			:= 0x7059;
const UOBJ_PARAFIELD_EW_TEMP		:= 0x7109;
const UOBJ_PARAFIELD_NS_TEMP		:= 0x7108;
const UOBJ_POISONFIELD_EW_TEMP		:= 0x7107;
const UOBJ_POISONFIELD_NS_TEMP		:= 0x7106;
const UOBJ_REFRESH_FULL_POTION		:= 0xdc0f;
const UOBJ_REFRESH_POTION		:= 0xdc0e;
const UOBJ_RITUAL_ROBE			:= 0xba29;
const UOBJ_ROBE1			:= 0x1f03;
const UOBJ_ROBE2			:= 0x1f04;
const UOBJ_ROPE_LADDER_EW		:= 0x08a6;
const UOBJ_ROPE_LADDER_NS		:= 0x08a5;
const UOBJ_RUINED_BOOKCASE1		:= 0x0c14;
const UOBJ_RUINED_BOOKCASE2		:= 0x0c15;
const UOBJ_SCROLL1			:= 0x0e35;
const UOBJ_SCROLL2			:= 0x0e36;
const UOBJ_SCROLL3			:= 0x0e37;
const UOBJ_SCROLL4			:= 0x0e38;
const UOBJ_SCROLL5			:= 0x0e39;
const UOBJ_SCROLL6			:= 0x0e3a;
const UOBJ_SCROLL7			:= 0x0ef4;
const UOBJ_SCROLL8			:= 0x0ef5;
const UOBJ_SCROLL9			:= 0x0ef6;
const UOBJ_SCROLL10			:= 0x0ef7;
const UOBJ_SCROLL11			:= 0x0ef8;
const UOBJ_SCROLL12			:= 0x0ef9;
const UOBJ_SECURE_CONTAINER_EAST	:= 0x61fd;
const UOBJ_SECURE_CONTAINER_SOUTH	:= 0x61fe;
const UOBJ_SHRUNK_DOLLY			:= 0xba32;
const UOBJ_SILVER_CHEST_EAST		:= 0x0e7c;
const UOBJ_SILVER_CHEST_SOUTH		:= 0x09ab;
const UOBJ_SPAWNPOINT			:= 0xa300;
const UOBJ_SPELLBOOK2			:= 0x0e3b;
const UOBJ_STAFF_ROBE			:= 0x204f;
const UOBJ_STAR_SAPPHIRE		:= 0x0f0f;
const UOBJ_STONEWALL_EW_TEMP		:= 0x7100;
const UOBJ_STONEWALL_NS_TEMP		:= 0x7101;
const UOBJ_STRENGTH_POTION		:= 0xdc11;
const UOBJ_SYSTEM_MOONGATE		:= 0x6200;
const UOBJ_SYSTEM_PILLAR		:= 0x6203;
const UOBJ_SYSTEM_SHRINE_ENTRANCE	:= 0x6205;
const UOBJ_SYSTEM_SHRINE_SYMBOL		:= 0x6204;
const UOBJ_SYSTEM_TELEPORTER		:= 0x6201;
const UOBJ_SYSTEM_WORD_TELEPORTER	:= 0x6202;
const UOBJ_TREASURE_MAP_LEVEL1		:= 0x6003;
const UOBJ_TREASURE_MAP_LEVEL2		:= 0x6004;
const UOBJ_TREASURE_MAP_LEVEL3		:= 0x6005;
const UOBJ_TREASURE_MAP_LEVEL4		:= 0x6006;
const UOBJ_TREASURE_MAP_LEVEL5		:= 0x6007;
const UOBJ_TREE1			:= 0x0cca;
const UOBJ_TREE2			:= 0x0ccb;
const UOBJ_TREE3			:= 0x0ccc;
const UOBJ_TREE4			:= 0x0ccd;
const UOBJ_TREE5			:= 0x0cd0;
const UOBJ_TREE6			:= 0x0cd3;
const UOBJ_TREE7			:= 0x0d41;
const UOBJ_TREE8			:= 0x0d42;
const UOBJ_TREE9			:= 0x0d43;
const UOBJ_TREE10			:= 0x0d44;
const UOBJ_TREE11			:= 0x0d57;
const UOBJ_TREE12			:= 0x0d58;
const UOBJ_TREE13			:= 0x0d59;
const UOBJ_TREE14			:= 0x0d5a;
const UOBJ_TREE15			:= 0x0d5b;
const UOBJ_TREE16			:= 0x0d6e;
const UOBJ_TREE17			:= 0x0d6f;
const UOBJ_TREE18			:= 0x0d70;
const UOBJ_TREE19			:= 0x0d71;
const UOBJ_TREE20			:= 0x0d72;
const UOBJ_TREE21			:= 0x0d84;
const UOBJ_TREE22			:= 0x0d85;
const UOBJ_TREE23			:= 0x0d86;
const UOBJ_TREE_APPLE1			:= 0x0d94;
const UOBJ_TREE_APPLE2			:= 0x0d98;
const UOBJ_TREE_BANANA1			:= 0x0caa;
const UOBJ_TREE_BANANA2			:= 0x0cab;
const UOBJ_TREE_BANANA_SMALL		:= 0x0ca8;
const UOBJ_TREE_CEDAR1			:= 0x0cd6;
const UOBJ_TREE_CEDAR2			:= 0x0cd8;
const UOBJ_TREE_CYPRESS1		:= 0x0cf8;
const UOBJ_TREE_CYPRESS2		:= 0x0cfb;
const UOBJ_TREE_CYPRESS3		:= 0x0cfe;
const UOBJ_TREE_CYPRESS4		:= 0x0d01;
const UOBJ_TREE_OAK1			:= 0x0cda;
const UOBJ_TREE_OAK2			:= 0x0cdd;
const UOBJ_TREE_O_HII			:= 0x0c9e;
const UOBJ_TREE_PEACH1			:= 0x0d9c;
const UOBJ_TREE_PEACH2			:= 0x0da0;
const UOBJ_TREE_PEAR1			:= 0x0da4;
const UOBJ_TREE_PEAR2			:= 0x0da8;
const UOBJ_TREE_SPIDER			:= 0x0cc9;
const UOBJ_TREE_STUMP1			:= 0x0dac;
const UOBJ_TREE_STUMP2			:= 0x0dad;
const UOBJ_TREE_TUSCANY_PINE		:= 0x1b7e;
const UOBJ_TREE_WALNUT1			:= 0x0ce0;
const UOBJ_TREE_WALNUT2			:= 0x0ce3;
const UOBJ_TREE_WILLOW			:= 0x0ce6;
const UOBJ_TREE_YEW1			:= 0x12b6;
const UOBJ_TREE_YEW2			:= 0x12b7;
const UOBJ_TREE_YEW3			:= 0x12b8;
const UOBJ_TREE_YEW4			:= 0x12b9;
const UOBJ_TREE_YEW5			:= 0x12ba;
const UOBJ_TREE_YEW6			:= 0x12bb;
const UOBJ_TREE_YEW7			:= 0x12bc;
const UOBJ_TREE_YEW8			:= 0x12bd;
const UOBJ_WATER_TROUGH_EW_1		:= 0x0b41;
const UOBJ_WATER_TROUGH_EW_2		:= 0x0b42;
const UOBJ_WATER_TROUGH_NS_1		:= 0x0b43;
const UOBJ_WATER_TROUGH_NS_2		:= 0x0b44;
const UOBJ_WOODEN_CHEST_EAST		:= 0x0e42;
const UOBJ_WOODEN_CHEST_SOUTH		:= 0x0e43;
const UOBJ_WOODEN_DOOR_NS_W		:= 0x06ad;
const UOBJ_WOODEN_LADDER_EW		:= 0x08a0;
const UOBJ_WOODEN_LADDER_NS		:= 0x08a3;
const UOBJ_WOODEN_LADDER_NS_2		:= 0x08a4;
const UOBJ_WOODEN_SIGN_1		:= 0x0bcf;
const UOBJ_WOODEN_SIGN_2		:= 0x0bd0;
const UOBJ_YOUNG_OAK_STAFF		:= 0xba28;
const UOBJ_YOUNG_OAK_WOOD		:= 0xba2a;
const UOBJ_ZULU_BOOK			:= 0x7901;


///////////////////////////////////////////////////////////////
//Systemreserved
///////////////////////////////////////////////////////////////

const SYSOBJ_MOUNT			:= 0x1F021;

/////////////////////////////////////////////////////////////////////////////
//
// New Const Include Types from Modern Distro
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x0000 to 0x3FFF - "Standard" objects
//
/////////////////////////////////////////////////////////////////////////////


const UOBJ_GUARDS       := 0xa340;
const UOBJ_MAGICBAG     := 0x6698;
const UOBJ_GMPACK		:= 0x19669;


///////////////////
//  Reagents
///////////////////

const UOBJ_REAPER_HEARTWOOD   := 0x1706f;
const UOBJ_ENT_HEARTWOOD      := 0x1706e;
const UOBJ_ENCHANTED_BONE     := 0x1a153;

///////////
//  Gems //
///////////

const UOBJ_GEM_AMBER		        := 0x0f25;
const UOBJ_GEM_STAR_SAPPHIRE	        := 0x0f0f;
const UOBJ_GEM_EMERALD		        := 0x0f10;
const UOBJ_GEM_SAPPHIRE		        := 0x0f11;
const UOBJ_GEM_RUBY			:= 0x0f13;
const UOBJ_GEM_CITRINE		        := 0x0f15;
const UOBJ_GEM_AMETHYST		        := 0x0f16;
const UOBJ_GEM_TOURMALINE	        := 0x0f18;
const UOBJ_GEM_DIAMOND		        := 0x0f30;

////////////////
// Food Items //
////////////////

const UOBJ_EGG				:= 0x09b5;
const UOBJ_RAW_RIB			:= 0x09f1;
const UOBJ_RAW_FISHSTEAK	:= 0x097a;
const UOBJ_RAW_BIRD			:= 0x09b9;
const UOBJ_RAW_HAM			:= 0x09c9;
const UOBJ_BACON_SLAB		:= 0x0976;
const UOBJ_RAW_SAUSAGE		:= 0x09c0;
const UOBJ_HONEY			:= 0x09ec;
const UOBJ_CARROT			:= 0x0c77;
const UOBJ_APPLE			:= 0x09d0;
const UOBJ_PEACH			:= 0x172c;
const UOBJ_LEMON			:= 0x1728;
const UOBJ_LIME				:= 0x172a;
const UOBJ_BANANA			:= 0x171f;
const UOBJ_PEAR				:= 0x172d;
const UOBJ_GRAPES			:= 0x0d1a;
const UOBJ_DATES			:= 0x1727;
const UOBJ_PUMPKIN			:= 0x0c6a;
const UOBJ_LETTUCE			:= 0x0c70;
const UOBJ_CABBAGE			:= 0x0c7c;
const UOBJ_ONION			:= 0x0c6d;
const UOBJ_TURNIP			:= 0x0d3a;
const UOBJ_COCONUT			:= 0x1726;
const UOBJ_RAW_CHICKEN_LEG	:= 0x1607;
const UOBJ_RAW_LEG_OF_LAMB	:= 0x1609;


//////////
// Keys //
//////////

const UOBJ_WAX                          := 0x1422;
const UOBJ_EMPTY_KETTLE                 := 0x09ed;
const UOBJ_KETTLE_OF_WAX                := 0x142a;
const UOBJ_DIPPING_STICK                := 0x1428;
const UOBJ_SKULL                        := 0x1ae2;



/////////////////////
// Tailoring items //
/////////////////////

const UOBJ_BOLT_OF_CLOTH                := 0x0f95;
const UOBJ_CLOTH_BOLT                   := 0x0f95;
const UOBJ_HIDE                         := 0x1078;
const UOBJ_THREAD1			:= 0x0fa0;
const UOBJ_THREAD2			:= 0x0fa1;
const UOBJ_YARN			        := 0x0e1d;
const UOBJ_COLORED_HIDE_START	        := 0x17050;
const UOBJ_COLORED_HIDE_END	        := 0x17054;
const UOBJ_TGOLD_INGOT                  := 0x1be9;

//////////////////////////
// Practice Dummy stuff //
//////////////////////////

const UOBJ_ARROW			:= 0x0f3f;
const UOBJ_BOLT		        	:= 0x1bfb;
const UOBJ_FIRE_ARROW		        := 0x16051;

const UOBJ_WOOD_YEW                     := 0x16351;
const UOBJ_WOOD_ASH                     := 0x16352;
const UOBJ_WOOD_CYPRESS                 := 0x16353;
const UOBJ_WOOD_OAK                     := 0x16354;
const UOBJ_WOOD_WORMWOOD                := 0x16355;

const UOBJ_SHAFT	                := 0x1bd4;


////////////////////////////////////////////////
// Types of wood available from various trees //
////////////////////////////////////////////////

const UOBJ_LOG                         := 0x1bdd;
const UOBJ_YEW_LOG                      := 0x16351;
const UOBJ_ASH_LOG                      := 0x16352;
const UOBJ_CYPRESS_LOG                  := 0x16353;
const UOBJ_OAK_LOG                      := 0x16354;
const UOBJ_WALNUT_LOG                   := 0x16358;
const UOBJ_CEDAR_LOG                    := 0x16359;
const UOBJ_MAPLE_LOG                    := 0x16361;
const UOBJ_PINE_LOG                     := 0x16364;
const UOBJ_GOSSAMER_LOG                 := 0x16365;
const UOBJ_WILLOW_LOG                   := 0x16363;
const UOBJ_BANANA_LOG                   := 0x16362;
const UOBJ_SPIDER_LOG                   := 0x16366;
const UOBJ_JUNIPER_LOG                  := 0x16367;

// These logs are not naturally occurring logs.
const UOBJ_VENOMOUS_LOG                 := 0x1635B;
const UOBJ_WORMWOOD_LOG                 := 0x16355;

/////////////////////////////////////////////////////////////////////////
// Types of boards are available from various trees when logs are sawn //
/////////////////////////////////////////////////////////////////////////

const UOBJ_BOARD                          := 0x1bd7;
const UOBJ_YEW_BOARD                      := 0x16370;
const UOBJ_ASH_BOARD                      := 0x16371;
const UOBJ_CYPRESS_BOARD                  := 0x16372;
const UOBJ_OAK_BOARD                      := 0x16373;
const UOBJ_WALNUT_BOARd                   := 0x16377;
const UOBJ_CEDAR_BOARD                    := 0x16378;
const UOBJ_MAPLE_BOARD                    := 0x16380;
const UOBJ_PINE_BOARD                     := 0x16381;
const UOBJ_GOSSAMER_BOARD                 := 0x16382;
const UOBJ_WILLOW_BOARD                   := 0x16383;
const UOBJ_BANANA_BOARD                   := 0x16384;
const UOBJ_SPIDER_BOARD                   := 0x16385;
const UOBJ_JUNIPER_BOARD                  := 0x16386;

// These boards are not naturally occurring boards.
// That is to say they cannont be found on standard trees.
const UOBJ_WORMWOOD_BOARD                 := 0x16374;
const UOBJ_HEARTWOOD_BOARD                := 0x16375;
const UOBJ_BONEWOOD_BOARD                 := 0x16376;
const UOBJ_IRONWOOD_BOARD                 := 0x16379;
const UOBJ_VENOMOUS_BOARD                 := 0x1637A;
const UOBJ_SPIRITWOOD_BOARD               := 0x1637B;
const UOBJ_SILVERWOOD_BOARD               := 0x1637C;
const UOBJ_ROSEWOOD_BOARD                 := 0x1637D;
const UOBJ_REAPERWOOD_BOARD               := 0x1637E;
const UOBJ_ENTHEARTWOOD_BOARD             := 0x1637F;


//////////////////////////////////////////
//  Ore and ingotsand Blacksmithy stuff //
//////////////////////////////////////////

const UOBJ_FORGE1       := 0x19a2;
const UOBJ_FORGE2       := 0x19a6;
const UOBJ_FORGE3       := 0x1982;
const UOBJ_FORGE4       := 0x1996;
const UOBJ_FORGE5       := 0x199a;
const UOBJ_FORGE6       := 0x198e;
const UOBJ_FORGE7       := 0x1992;
const UOBJ_FORGE8       := 0x1986;
const UOBJ_FORGE9       := 0x198a;


const UOBJ_SHADOW_ORE		:= 0x16302;
const UOBJ_KINDRED_ORE          := 0x16305;
const UOBJ_AGAPITE_ORE          := 0x16306;
const UOBJ_VERITE_ORE           := 0x16307;
const UOBJ_VALORITE_ORE         := 0x16308;
const UOBJ_SHADOW_INGOT		:= 0x1630b;
const UOBJ_KINDRED_INGOT        := 0x1630e;
const UOBJ_AGAPITE_INGOT        := 0x1630f;
const UOBJ_VERITE_INGOT         := 0x16310;
const UOBJ_VALORITE_INGOT       := 0x16311;

/////////////////////////
// musical instruments //
/////////////////////////

const UOBJ_TAMBORINE		        := 0x0e9d;
const UOBJ_TAMBORINE2		        := 0x0e9e;
const UOBJ_LUTE2			:= 0x0eb4;
const UOBJ_STANDING_HARP		:= 0x0eb1;

//////////////
// Footwear //
//////////////

const UOBJ_SANDALS			:= 0x170d;
const UOBJ_SHOES			:= 0x170f;
const UOBJ_BOOTS			:= 0x170b;
const UOBJ_THIGH_BOOTS		        := 0x1711;


///////////////////
// leather armor //
///////////////////

const UOBJ_LEATHER_GORGET	        := 0x13c7;
const UBOJ_LEATHER_SLEEVES	        := 0x13c5;
const UOBJ_LEATHER_SLEEVES2	        := 0x13cd;
const UOBJ_LEATHER_GLOVES	        := 0x13c6;
const UOBJ_LEATHER_CAP		        := 0x1db9;
const UOBJ_LEATHER_LEGGINGS	        := 0x13cb;
const UOBJ_LEATHER_TUNIC		:= 0x13cc;
const UOBJ_LEATHER_TUNIC2	        := 0x13d3;
const UOBJ_LEATHER_FEMALE	        := 0x1c06;
const UOBJ_LEATHER_SKIRT		:= 0x1c08;
const UOBJ_LEATHER_BUSTIER	        := 0x1c0a;
const UOBJ_LEATHER_SHORTS	        := 0x1c00;

///////////////////
// studded armor //
///////////////////

const UOBJ_STUDDED_GORGET	        := 0x13d6;
const UOBJ_STUDDED_GLOVES	        := 0x13d5;
const UOBJ_STUDDED_SLEEVES	        := 0x13d4;
const UOBJ_STUDDED_SLEEVES2	        := 0x13dc;
const UOBJ_STUDDED_LEGGINGS	        := 0x13da;
const UOBJ_STUDDED_TUNIC		:= 0x13db;
const UOBJ_STUDDED_FEMALE	        := 0x1c02;
const UOBJ_STUDDED_BUSTIER	        := 0x1c0c;

////////////////////
// ringmail armor //
////////////////////

const UOBJ_RINGMAIL_TUNIC	        := 0x13ec;
const UOBJ_RINGMAIL_TUNIC2	        := 0x13ed;
const UOBJ_RINGMAIL_SLEEVES	        := 0x13ee;
const UOBJ_RINGMAIL_SLEEVES2	        := 0x13ef;
const UOBJ_RINGMAIL_LEGGINGS	        := 0x13f0;
const UOBJ_RINGMAIL_LEGGINGS2	        := 0x13f1;
const UOBJ_RINGMAIL_GLOVES	        := 0x13f2;
const UOBJ_RINGMAIL_GLOVES2	        := 0x13eb;

/////////////////////
// chainmail armor //
/////////////////////

const UOBJ_CHAINMAIL_COIF	        := 0x13bb;
const UOBJ_CHAINMAIL_COIF2	        := 0x13c0;
const UOBJ_CHAINMAIL_LEGGINGS	        := 0x13be;
const UOBJ_CHAINMAIL_LEGGINGS2          := 0x13c3;
const UOBJ_CHAINMAIL_TUNIC	        := 0x13bf;
const UOBJ_CHAINMAIL_TUNIC2	        := 0x13c4;
const UOBJ_CHAINMAIL_GLOVES	        := 0x17066;

/////////////////////
// Platemail armor //
/////////////////////

const UOBJ_PLATE_FEMALE		        := 0x1c04;
const UOBJ_PLATE_FEMALE2		:= 0x1c05;
const UOBJ_PLATE_BREASTPLATE	        := 0x1415;
const UOBJ_PLATE_BREASTPLATE2	        := 0x1416;
const UOBJ_PLATE_ARMS		        := 0x1410;
const UOBJ_PLATE_ARMS2		        := 0x1417;
const UOBJ_PLATE_LEGS		        := 0x1411;
const UOBJ_PLATE_LEGS2		        := 0x141a;
const UOBJ_PLATE_GLOVES		        := 0x1414;
const UOBJ_PLATE_GLOVES2		:= 0x1418;
const UOBJ_PLATE_GORGET		        := 0x1413;

////////////////
// Bone armor //
////////////////

const UOBJ_BONE_ARMS		        := 0x144e;
const UOBJ_BONE_TUNIC		        := 0x144f;
const UOBJ_BONE_GLOVES		        := 0x1450;
const UOBJ_BONE_HELM		        := 0x1451;
const UOBJ_BONE_LEGS		        := 0x1452;

///////////
// helms //
///////////

const UOBJ_PLATE_HELM		        := 0x1412;
const UOBJ_PLATE_HELM2		        := 0x1419;
const UOBJ_CLOSEHELM		        := 0x1408;
const UOBJ_CLOSEHELM2		        := 0x1409;
const UOBJ_HELMET					:= 0x140a;
const UOBJ_HELMET2					:= 0x140b;
const UOBJ_BASCINET					:= 0x140c;
const UOBJ_BASCINET2		        := 0x140d;
const UOBJ_NOSEHELM					:= 0x140e;
const UOBJ_NOSEHELM2		        := 0x140f;
const UOBJ_ORC_MASK					:= 0x141b;
const UOBJ_ORC_MASK2				:= 0x141c;
const UOBJ_BEAR_MASK				:= 0x1545;
const UOBJ_BEAR_MASK2				:= 0x1546;
const UOBJ_DEER_MASK				:= 0x1547;
const UOBJ_DEER_MASK2				:= 0x1548;
const UOBJ_TRIBAL_MASK				:= 0x1549;
const UOBJ_TRIBAL_MASK2				:= 0x154a;
const UOBJ_VOODOO_MASK				:= 0x154b;
const UOBJ_VOODOO_MASK2				:= 0x154c;

/////////////
// shields //
/////////////

const UOBJ_BUCKLER			:= 0x1b73;
const UOBJ_BRONZESHIELD		        := 0x1b72;
const UOBJ_METALSHIELD		        := 0x1b7b;
const UOBJ_KITESHIELD		        := 0x1b74;
const UOBJ_KITESHIELD2		        := 0x1b75;
const UOBJ_HEATER			:= 0x1b76;
const UOBJ_HEATER2			:= 0x1b77;
const UOBJ_WOODEN_SHIELD		:= 0x1b7a;
const UOBJ_WOODEN_KITESHIELD	        := 0x1b78;
const UOBJ_WOODEN_KITESHIELD2	        := 0x1b79;
const UOBJ_CHAOS_SHIELD		        := 0x1bc3;
const UOBJ_ORDER_SHIELD		        := 0x1bc4;
const UOBJ_ORDER_SHIELD2		:= 0x1bc5;




////////////////////
// wooden weapons //
////////////////////

const UOBJ_CLUB			        := 0x13b3;
const UOBJ_SHEEPHERDS_CROOK	        := 0x0e81;
const UOBJ_BLACK_STAFF		        := 0x0df0;
const UOBJ_GNARLED_STAFF	        := 0x13f9;
const UOBJ_QUARTER_STAFF	        := 0x0e8a;


const UOBJ_COPPER_COIN		:=0x0eea;
const UOBJ_SILVER_COIN		:=0x0ef0;
const UOBJ_PORCELAIN		:= 0xee21;
const UOBJ_SNOW                 := 0x5001;


/////////////////////////
// More things defined //
/////////////////////////

const UOBJ_STARTHAIR		:= 0x203b;
const UOBJ_ENDHAIR			:= 0x204d;


const UOBJ_RECALL_RUNE          := 0x1f14;

