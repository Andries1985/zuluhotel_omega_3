//////////////////////////////////////////////////////////////////////////////////////////////////////////
// So i prefer .inc to declare things more than .cfg!
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////

include ":gumps:include/gumps";
include ":gumps:include/gumps_ex";
include "versefunctions";

function GetVerseTypeInfo(gump, type)

	case(type)

	// First level verses
	1:

	GFTilePic(gump, 220, 30, 3274);
	GFTextLine(gump, 325, 49, 64, "A");
	GFTextLine(gump, 334, 50, 0, "ll apprentice");
	GFTextLine(gump, 314, 67, 0, "bards learn the");
	GFTextLine(gump, 300, 84, 0, "prosaic verses at");
	GFTextLine(gump, 292, 101, 0, "the start of their");
	GFTextLine(gump, 292, 118, 0, "training.");
	GFTextLine(gump, 236, 145, 0, "Deceivingly effective, prosaic");
	GFTextLine(gump, 236, 162, 0, "verses are even favoured");
	GFTextLine(gump, 236, 179, 0, "by the experienced bards.");

	2:

	GFTilePic(gump, 377, 70, 9);
	GFTilePic(gump, 360, 76, 2970);
	GFTilePic(gump, 360, 96, 3004);

	GFTextLine(gump, 236, 49, 64, "N");
	GFTextLine(gump, 245, 50, 0, "ot much more difficult");
	GFTextLine(gump, 236, 67, 0, "than prosiac verses, the");
	GFTextLine(gump, 236, 84, 0, "prevalent verses are");
	GFTextLine(gump, 236, 101, 0, "similar in usefulness.");
	GFTextLine(gump, 236, 128, 0, "Defensive and offensive,");
	GFTextLine(gump, 236, 142, 0, "prevalent verses offer");
	GFTextLine(gump, 236, 156, 0, "a variety of choice for");
	GFTextLine(gump, 236, 170, 0, "the fickle bard. ");

	GFTilePic(gump, 347, 185, 3742);

	endcase

	return gump;

endfunction

function GetVerseInfo(gump, verse)

	var info := ReturnVerseInfo(verse);

	case(verse)
	
	1: // Sonic Disturbance
	GFTextLine(gump, 245, 25, 64, "Sonic Disturbance");
	GFHTMLAREA(gump, 245, 50, 160, 100, "<BASEFONT SIZE=4>Emits an ear piercing sound wave.  While the damage isn't high, the verse is a simple one to master", 0, 0);
	GFGumpPic(gump, 352, 160, 2270);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	2: // Lesser Healing
	GFTextLine(gump, 245, 25, 64, "Lesser Healing");
	GFHTMLAREA(gump, 240, 50, 160, 100, "<BASEFONT SIZE=4>Lightly heals nearby friends.  Useful for helping the lightly wounded without a high stamina consumption.", 0, 0);
	GFTilePic(gump, 323, 140, 2);
	GFTilePic(gump, 345, 140, 3);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	3: // Shadows
	GFTextLine(gump, 245, 25, 64, "Shadows");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>All nearby friends are instantly cast into the shadows.  Nothing can be seen, but music is always heard...", 0, 0);
	GFGumpPic(gump, 352, 160, 2274);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	4: // corpse distention
	GFTextLine(gump, 245, 25, 64, "Corpse Distention");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Increases the pressure inside nearby corpses. The result is messy, and in certain situations, very deadly.", 0, 0);
	GFTilePic(gump, 330, 170, 15722);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	5: // Spirit Flock
	GFTextLine(gump, 245, 25, 64, "Spirit Flock");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Summons a flock of spirit goats. Protective of their master, the goats flock together to attack their enemies. ", 0, 0);
	GFTilePic(gump, 350, 165, 9600);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	6: // Dragon Skin
	GFTextLine(gump, 245, 25, 64, "Dragon Skin");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Strengthens nearby allies skin with the strength of dragon skin.  Enabling them to sustain greater physical damage.", 0, 0);
	GFTilePic(gump, 355, 155, 8406);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	7: // Life Balance
	GFTextLine(gump, 245, 25, 64, "Life Balance");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Evenly balances all nearby allies health between them. A very effective verse at keeping everyone in the group alive.", 0, 0);
	GFTilePic(gump, 355, 155, 6225);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	8: // Bardic Boulders
	GFTextLine(gump, 245, 25, 64, "Bardic Boulders");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Summons a boulder to be dropped on a targets head, very painfull. The first verse allowing selective targetting.", 0, 0);
	GFTilePic(gump, 350, 150, 6006);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	9: // Beastal Bond
	GFTextLine(gump, 245, 25, 64, "Beastal Bond");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>Creates a bond between the bard and the beast. The beast acts as a 'spy' of sorts for the bard.", 0, 0);
	GFTilePic(gump, 350, 150, 8484);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	10: // Not Implemented
	GFTextLine(gump, 245, 25, 64, "Not Implemented");
	GFHTMLAREA(gump, 243, 50, 160, 100, "<BASEFONT SIZE=4>This verse hasn't been created yet. The below fellow is someone who has been waiting for it...", 0, 0);
	GFTilePic(gump, 350, 150, 6659);
	GFTextLine(gump, 245, 160, 0, "Stamina:");
	GFTextLine(gump, 310, 160, 44, info[1]);
	GFTextLine(gump, 245, 180, 0, "Difficulty:");
	GFTextLine(gump, 310, 180, 44, info[2]);

	endcase

	return gump;

endfunction