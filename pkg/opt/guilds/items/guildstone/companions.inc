use uo;
use cfgfile;

function DestroyGuildstoneCompanions(item)

	var companions := GetObjProperty(item, "GSCompanions");
	if (companions)
		foreach serial in companions
			var piece := SystemFindObjectBySerial(serial);
			if (piece)
				DestroyItem(piece);
			endif
		endforeach
	endif

	EraseObjProperty(item, "GSCompanions");
	EraseObjProperty(item, "GSSetNumber");

endfunction

function SpawnGuildstoneSetCompanions(item, elem, piece_count)

	var companions := array;
	var piece;
	for (piece := 2; piece <= piece_count; piece := piece + 1)
		var graphic := GetConfigInt(elem, "Piece" + piece + "Graphic");
		var dx := GetConfigInt(elem, "Piece" + piece + "DX");
		var dy := GetConfigInt(elem, "Piece" + piece + "DY");

		var piece_item := CreateItemAtLocation(item.x + dx, item.y + dy, item.z, graphic, 1, item.realm);
		if (piece_item)
			piece_item.movable := 0;
			// Same usescript as the guildstone itself, so double-clicking any
			// companion piece opens the guild menu too, not just the anchor tile.
			piece_item.usescript := ":guilds:items/guildstone/use";
			// Match the anchor tile's name (rather than each piece's own
			// statue-graphic name) so the whole set reads as one guildstone.
			SetName(piece_item, item.name);
			SetObjProperty(piece_item, "GSOwnerSerial", item.serial);
			companions.append(piece_item.serial);
		endif
	endfor

	SetObjProperty(item, "GSCompanions", companions);
	SetObjProperty(item, "GSSetNumber", GetConfigInt(elem, "SetNumber"));

endfunction

// Recolors the guildstone's spawned companion pieces to match, e.g. after
// applying the guild colour to the anchor tile.
function SetGuildstoneCompanionsColour(item, colour)

	var companions := GetObjProperty(item, "GSCompanions");
	if (companions)
		foreach serial in companions
			var piece := SystemFindObjectBySerial(serial);
			if (piece)
				piece.color := colour;
			endif
		endforeach
	endif

endfunction
