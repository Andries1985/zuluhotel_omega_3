use uo;

function smallhousearray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 2);
  box1.append(y - 2);
  box1.append(z + 5);
  box1.append(x + 2);
  box1.append(y + 2);
  box1.append(z + 25);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-0, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function gardenshedarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 2);
  box1.append(y - 2);
  box1.append(z + 5);
  box1.append(x + 2);
  box1.append(y + 2);
  box1.append(z + 25);
  boxarray[1] := box1;
  SetObjProperty(house, "bantiles", {});
  return boxarray;
endfunction

function smallstoneworkshoparray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 2);
  box1.append(y - 2);
  box1.append(z + 7);
  box1.append(x + 2);
  box1.append(y + 2);
  box1.append(z + 27);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // Shift workshop front ban tiles one step left on X.
  bantile := CreateItemAtLocation(x-2, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-0, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function smallmarbleworkshoparray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 2);
  box1.append(y - 2);
  // Marble workshop uses multiple usable floor elevations.
  box1.append(z - 2);
  box1.append(x + 2);
  box1.append(y + 2);
  box1.append(z + 60);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-0, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function largehousearray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x + 6);
  box1.append(y + 5);
  box1.append(z + 27);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-2, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function largepatioarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x + 6);
  box1.append(y + 5);
  box1.append(z + 27);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-5, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-4, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-3, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-2, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function largemarblepatioarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x + 6);
  box1.append(y + 5);
  box1.append(z + 27);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-6, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-5, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-4, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-3, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-2, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+7, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+5, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+4, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function smalltowerarray(house, x, y, z)
  // Small Stone Tower (multi 0x98) is a 7x7 footprint, ~2 stories tall.
  // Sign is on the south face at (x-1, y+4); the door sits at (x+1, y+3)
  // with steps directly south at (x+1, y+4). Place ban tiles directly in
  // front of the door and one tile to each side of the door.
  var boxarray := {};
  var box1 := {};
  box1.append(x - 3);
  box1.append(y - 3);
  box1.append(z + 7);
  box1.append(x + 3);
  box1.append(y + 3);
  box1.append(z + 47);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x+2, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+3, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+4, y+4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function twostoryarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x    );
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[1] := box1;
  box1 := {};
  box1.append(x    );
  box1.append(y    );
  box1.append(z + 7);
  box1.append(x + 5);
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[2] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-4, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-3, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-2, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+7, Z+4, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function twostoryvillaarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x    );
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[1] := box1;
  box1 := {};
  box1.append(x    );
  box1.append(y    );
  box1.append(z + 7);
  box1.append(x + 5);
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[2] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x+2, y+6, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+3, y+6, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+4, y+6, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+5, y+6, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function sandstonepatioarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 4);
  box1.append(y - 4);
  box1.append(z + 2);
  box1.append(x + 6);
  box1.append(y + 4);
  box1.append(z + 60);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-2, y+4, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+4, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+4, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+3, y+3, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+4, y+3, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+5, y+3, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y+3, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y+2, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y+1, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y-1, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y-2, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y-3, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+6, y-4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+5, y-4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+4, y-4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+3, y-4, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function logcabinarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 7);
  box1.append(x    );
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[1] := box1;
  box1 := {};
  box1.append(x    );
  box1.append(y    );
  box1.append(z + 7);
  box1.append(x + 5);
  box1.append(y + 5);
  box1.append(z + 47);
  boxarray[2] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x+1, y+6, Z+2, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function bigtowerarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 6);
  box1.append(y - 6);
  box1.append(z + 6);
  box1.append(x + 7);
  box1.append(y + 5);
  box1.append(z + 65);
  boxarray[1] := box1;
  box1 := {};
  box1.append(x - 10);
  box1.append(y - 6);
  box1.append(z + 66);
  box1.append(x + 11);
  box1.append(y + 5);
  box1.append(z + 86);
  boxarray[2] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+8, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+8, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+8, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+8, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+7, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+7, Z+6, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+7, Z+6, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+7, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function castlearray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 14);
  box1.append(y - 14);
  box1.append(z     );
  box1.append(x + 14);
  box1.append(y + 14);
  box1.append(z + 47);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y+10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1,  y+6, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x,  y+6, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1,  y+6, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2,  y+6, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-2, y-10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x-1, y-10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y-10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y-10, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function gothicfortressarray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 15);
  box1.append(y - 15);
  box1.append(z     );
  box1.append(x + 15);
  box1.append(y + 16);
  box1.append(z + 67);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+16, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function keeparray(house, x, y, z)
  var boxarray := {};
  var box1 := {};
  box1.append(x - 10);
  box1.append(y - 10);
  box1.append(z     );
  box1.append(x + 11);
  box1.append(y + 11);
  box1.append(z + 47);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  bantile := CreateItemAtLocation(x-1, y+11, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x, y+11, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+1, y+11, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x+2, y+11, Z+3, "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

// ---------------------------------------------------------------------------
// Auto-generated ban-tile arrays for patch 3.0.5 multis (2026-08-04).
// Derived directly from real mul/multi.mul component data + tiledata.mul Door
// flag detection. Ban strips are placed one tile beyond the nearest structure
// bounding edge from each detected door cluster -- this is a HEURISTIC, not
// verified in-client. Spot-check each entrance in-game, especially clusters
// with unusual Z offsets (upper floors/balconies), before relying on it.
// ---------------------------------------------------------------------------

function multi147ehousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x147e from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 6 door tile(s) near (x+8, y+2, z+20)
  bantile := CreateItemAtLocation(x + (13), y + (1), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (2), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (3), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 6 door tile(s) near (x-7, y+2, z+20)
  bantile := CreateItemAtLocation(x + (-12), y + (1), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (2), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (3), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x+0, y-2, z+47)
  bantile := CreateItemAtLocation(x + (-1), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1480housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1480 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 1 door tile(s) near (x+12, y+14, z+7)
  bantile := CreateItemAtLocation(x + (11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (12), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-12, y+14, z+7)
  bantile := CreateItemAtLocation(x + (-13), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+14, y+9, z+17)
  bantile := CreateItemAtLocation(x + (16), y + (8), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (10), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-13, y+9, z+17)
  bantile := CreateItemAtLocation(x + (-16), y + (8), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (9), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (10), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x+6, y+6, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (6), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x-8, y+6, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 3 door tile(s) near (x+5, y-11, z+20)
  bantile := CreateItemAtLocation(x + (4), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (6), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 2 door tile(s) near (x-8, y-10, z+7)
  bantile := CreateItemAtLocation(x + (-9), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-8), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 9: 1 door tile(s) near (x+4, y+14, z+7)
  bantile := CreateItemAtLocation(x + (3), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (4), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 10: 1 door tile(s) near (x-3, y+14, z+7)
  bantile := CreateItemAtLocation(x + (-4), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-3), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 11: 2 door tile(s) near (x+0, y+9, z+27)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 12: 2 door tile(s) near (x+8, y-1, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (-2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (0), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 13: 2 door tile(s) near (x-6, y-1, z+27)
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (0), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1481housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1481 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+12, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-7, y+13, z+27)
  bantile := CreateItemAtLocation(x + (-8), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+7, y+6, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (5), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (6), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (7), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x+14, y+1, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (0), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 1 door tile(s) near (x-13, y+12, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (11), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (13), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x-13, y+0, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (0), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (1), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 2 door tile(s) near (x+7, y-4, z+47)
  bantile := CreateItemAtLocation(x + (16), y + (-5), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-4), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-3), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 1 door tile(s) near (x-13, y-12, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (-13), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-11), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 9: 1 door tile(s) near (x+4, y-13, z+47)
  bantile := CreateItemAtLocation(x + (3), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (4), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 10: 1 door tile(s) near (x-7, y-13, z+47)
  bantile := CreateItemAtLocation(x + (-8), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1482housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1482 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 13 door tile(s) near (x+7, y-6, z+30)
  bantile := CreateItemAtLocation(x + (16), y + (-7), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-6), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-5), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 13 door tile(s) near (x-8, y-6, z+29)
  bantile := CreateItemAtLocation(x + (-16), y + (-7), Z + (29), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-6), Z + (29), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-5), Z + (29), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1483housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1483 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x-4, y-1, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 3 door tile(s) near (x-11, y-1, z+20)
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (0), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 4 door tile(s) near (x+5, y-9, z+22)
  bantile := CreateItemAtLocation(x + (4), y + (-16), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (-16), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (6), y + (-16), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1484housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1484 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x-2, y+11, z+7)
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 4 door tile(s) near (x+7, y+4, z+27)
  bantile := CreateItemAtLocation(x + (13), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (4), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (5), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+10, y-2, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-2), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-2, y+5, z+27)
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1485housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1485 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 1 door tile(s) near (x+11, y+12, z+7)
  bantile := CreateItemAtLocation(x + (10), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (11), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (12), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-9, y+12, z+7)
  bantile := CreateItemAtLocation(x + (-10), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-9), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-8), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+0, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-8, y-4, z+7)
  bantile := CreateItemAtLocation(x + (-12), y + (-5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 1 door tile(s) near (x+11, y-4, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 1 door tile(s) near (x-5, y-10, z+7)
  bantile := CreateItemAtLocation(x + (-6), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-5), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-4), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1486housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1486 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 6 door tile(s) near (x+0, y-2, z+30)
  bantile := CreateItemAtLocation(x + (-1), y + (-12), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-12), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-12), Z + (30), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-10, y+4, z+7)
  bantile := CreateItemAtLocation(x + (-12), y + (3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 1 door tile(s) near (x-6, y-4, z+47)
  bantile := CreateItemAtLocation(x + (-12), y + (-5), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-4), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-3), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x+8, y-9, z+47)
  bantile := CreateItemAtLocation(x + (7), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (8), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1487housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1487 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 4 door tile(s) near (x+0, y+8, z+17)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1488housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1488 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 3 door tile(s) near (x+1, y+5, z+20)
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (2), y + (14), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+10, y+0, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 3 door tile(s) near (x+9, y-8, z+7)
  bantile := CreateItemAtLocation(x + (8), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (10), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-6, y-8, z+7)
  bantile := CreateItemAtLocation(x + (-7), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-5), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x+8, y+12, z+7)
  bantile := CreateItemAtLocation(x + (7), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (8), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x-6, y+12, z+7)
  bantile := CreateItemAtLocation(x + (-7), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-5), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 1 door tile(s) near (x+11, y+6, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (6), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 2 door tile(s) near (x-10, y+2, z+27)
  bantile := CreateItemAtLocation(x + (-12), y + (1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1489housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1489 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 3 door tile(s) near (x+0, y+7, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+3, y-8, z+7)
  bantile := CreateItemAtLocation(x + (2), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (3), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (4), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x-2, y-8, z+7)
  bantile := CreateItemAtLocation(x + (-3), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148ahousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148a from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+13, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x+14, y+10, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 4 door tile(s) near (x+12, y-2, z+17)
  bantile := CreateItemAtLocation(x + (16), y + (-3), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-2), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x+14, y-13, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-13), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-12, y-13, z+27)
  bantile := CreateItemAtLocation(x + (-13), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 4 door tile(s) near (x+3, y+6, z+37)
  bantile := CreateItemAtLocation(x + (2), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (3), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (4), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 4 door tile(s) near (x-6, y+6, z+37)
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 2 door tile(s) near (x+2, y-4, z+47)
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (2), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (3), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 9: 2 door tile(s) near (x-5, y-4, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (-5), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-4), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-3), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148bhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148b from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+12, y+15, z+7)
  bantile := CreateItemAtLocation(x + (11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (12), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x-12, y+15, z+7)
  bantile := CreateItemAtLocation(x + (-13), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 4 door tile(s) near (x+0, y+8, z+17)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x+12, y+8, z+47)
  bantile := CreateItemAtLocation(x + (16), y + (7), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (8), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-12, y+8, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (8), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (9), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148chousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148c from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 4 door tile(s) near (x+10, y+13, z+17)
  bantile := CreateItemAtLocation(x + (9), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (10), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (11), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 4 door tile(s) near (x-10, y+13, z+17)
  bantile := CreateItemAtLocation(x + (-11), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-10), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-9), y + (17), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 4 door tile(s) near (x+0, y+7, z+37)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148dhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148d from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 4 door tile(s) near (x-10, y+9, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 37 door tile(s) near (x+1, y-5, z+24)
  bantile := CreateItemAtLocation(x + (0), y + (-16), Z + (24), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (24), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (2), y + (-16), Z + (24), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148ehousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148e from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 3 door tile(s) near (x+0, y+14, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 8 door tile(s) near (x+11, y+11, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 8 door tile(s) near (x-11, y+11, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 3 door tile(s) near (x+14, y+0, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 3 door tile(s) near (x-14, y+0, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 8 door tile(s) near (x+11, y-11, z+7)
  bantile := CreateItemAtLocation(x + (10), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (11), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (12), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 8 door tile(s) near (x-11, y-11, z+7)
  bantile := CreateItemAtLocation(x + (-12), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-10), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 3 door tile(s) near (x+0, y-14, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi148fhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x148f from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+1, y+13, z+7)
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (2), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+12, y+6, z+17)
  bantile := CreateItemAtLocation(x + (16), y + (5), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (6), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (7), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x-11, y+6, z+17)
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x+7, y+13, z+37)
  bantile := CreateItemAtLocation(x + (6), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (7), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (8), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-7, y+13, z+37)
  bantile := CreateItemAtLocation(x + (-8), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (17), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 1 door tile(s) near (x+12, y-2, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (-3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 1 door tile(s) near (x-11, y-2, z+27)
  bantile := CreateItemAtLocation(x + (-16), y + (-3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 2 door tile(s) near (x+0, y-5, z+27)
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 9: 1 door tile(s) near (x-13, y-14, z+27)
  bantile := CreateItemAtLocation(x + (-14), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-13), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1490housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1490 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x+11, y+8, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 4 door tile(s) near (x+9, y+0, z+17)
  bantile := CreateItemAtLocation(x + (13), y + (-1), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (0), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (1), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x+11, y-9, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-2, y+5, z+27)
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1491housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1491 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 5 door tile(s) near (x+8, y+11, z+15)
  bantile := CreateItemAtLocation(x + (7), y + (14), Z + (15), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (8), y + (14), Z + (15), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (14), Z + (15), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-3, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-4), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 5 door tile(s) near (x+9, y+3, z+27)
  bantile := CreateItemAtLocation(x + (13), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (4), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x-6, y+5, z+7)
  bantile := CreateItemAtLocation(x + (-12), y + (4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (6), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x+11, y-6, z+17)
  bantile := CreateItemAtLocation(x + (13), y + (-7), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-6), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-5), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 3 door tile(s) near (x-8, y+11, z+40)
  bantile := CreateItemAtLocation(x + (-9), y + (14), Z + (40), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-8), y + (14), Z + (40), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (14), Z + (40), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1492housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1492 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 8 door tile(s) near (x+10, y+10, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 6 door tile(s) near (x-9, y+10, z+7)
  bantile := CreateItemAtLocation(x + (-12), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+0, y+8, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x+9, y+0, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 6 door tile(s) near (x+10, y-9, z+7)
  bantile := CreateItemAtLocation(x + (9), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (10), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (11), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x-5, y-6, z+47)
  bantile := CreateItemAtLocation(x + (-6), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-5), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-4), y + (-12), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1493housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1493 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 5 door tile(s) near (x-3, y+7, z+7)
  bantile := CreateItemAtLocation(x + (-4), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+10, y-8, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 1 door tile(s) near (x+11, y+6, z+27)
  bantile := CreateItemAtLocation(x + (13), y + (5), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (6), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (7), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-8, y-10, z+67)
  bantile := CreateItemAtLocation(x + (-9), y + (-12), Z + (67), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-8), y + (-12), Z + (67), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (-12), Z + (67), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1494housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1494 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 14 door tile(s) near (x-5, y+5, z+18)
  bantile := CreateItemAtLocation(x + (-12), y + (4), Z + (18), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (5), Z + (18), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (6), Z + (18), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 4 door tile(s) near (x+10, y-4, z+7)
  bantile := CreateItemAtLocation(x + (13), y + (-5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 1 door tile(s) near (x+4, y-4, z+7)
  bantile := CreateItemAtLocation(x + (3), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (4), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 4 door tile(s) near (x-1, y-4, z+37)
  bantile := CreateItemAtLocation(x + (-2), y + (-12), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (-12), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-12), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1495housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1495 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 12 door tile(s) near (x-4, y+1, z+27)
  bantile := CreateItemAtLocation(x + (-12), y + (0), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+8, y+4, z+47)
  bantile := CreateItemAtLocation(x + (13), y + (3), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (4), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (5), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1496housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1496 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+12, y+9, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 1 door tile(s) near (x-10, y+11, z+7)
  bantile := CreateItemAtLocation(x + (-11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-10), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-9), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 3 door tile(s) near (x-3, y+9, z+14)
  bantile := CreateItemAtLocation(x + (-4), y + (17), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-3), y + (17), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (17), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 4 door tile(s) near (x+9, y-3, z+32)
  bantile := CreateItemAtLocation(x + (16), y + (-4), Z + (32), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-3), Z + (32), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-2), Z + (32), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 1 door tile(s) near (x-8, y-3, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (-4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 1 door tile(s) near (x-2, y-9, z+7)
  bantile := CreateItemAtLocation(x + (-3), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 1 door tile(s) near (x+13, y-10, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1497housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1497 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+12, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+0, y+5, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+12, y+0, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-8, y+6, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-8, y-2, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (-3), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-2), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-1), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x+0, y-4, z+47)
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1498housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1498 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+9, y+10, z+7)
  bantile := CreateItemAtLocation(x + (8), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (10), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 62 door tile(s) near (x+0, y-2, z+20)
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (-16), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi1499housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x1499 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+13, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+0, y+5, z+27)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi149ahousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x149a from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 4 door tile(s) near (x+12, y+12, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (13), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x-12, y+13, z+7)
  bantile := CreateItemAtLocation(x + (-13), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+13, y-12, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-13), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-12), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-7, y-4, z+7)
  bantile := CreateItemAtLocation(x + (-16), y + (-5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (-3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-7, y-10, z+7)
  bantile := CreateItemAtLocation(x + (-8), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (-16), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 1 door tile(s) near (x-9, y+5, z+27)
  bantile := CreateItemAtLocation(x + (-16), y + (4), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 2 door tile(s) near (x+7, y+4, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (4), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (5), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 8: 1 door tile(s) near (x-12, y-13, z+47)
  bantile := CreateItemAtLocation(x + (-13), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-11), y + (-16), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi149bhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x149b from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 2 door tile(s) near (x+0, y+15, z+7)
  bantile := CreateItemAtLocation(x + (-1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+14, y-10, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-11), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-10), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi149dhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x149d from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 4 door tile(s) near (x+0, y+6, z+17)
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (0), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 6 door tile(s) near (x+8, y+2, z+27)
  bantile := CreateItemAtLocation(x + (13), y + (1), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x-8, y+3, z+27)
  bantile := CreateItemAtLocation(x + (-12), y + (2), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (3), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (4), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi149ehousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x149e from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-11));
  box1.append(y + (-11));
  box1.append(z - 8);
  box1.append(x + (12));
  box1.append(y + (13));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 6 door tile(s) near (x+8, y+8, z+17)
  bantile := CreateItemAtLocation(x + (13), y + (7), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (8), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (13), y + (9), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 6 door tile(s) near (x-2, y+8, z+17)
  bantile := CreateItemAtLocation(x + (-3), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (14), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 3 door tile(s) near (x-10, y+8, z+27)
  bantile := CreateItemAtLocation(x + (-12), y + (7), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (8), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-12), y + (9), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi149fhousearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x149f from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 1 door tile(s) near (x+5, y+14, z+7)
  bantile := CreateItemAtLocation(x + (4), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (5), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (6), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 2 door tile(s) near (x+8, y+8, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (7), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (8), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 5 door tile(s) near (x-7, y+6, z+23)
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (23), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (6), Z + (23), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (7), Z + (23), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x+1, y+7, z+7)
  bantile := CreateItemAtLocation(x + (0), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (1), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (2), y + (17), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi14a0housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x14a0 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 3 door tile(s) near (x+11, y+4, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (3), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (4), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (5), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 6 door tile(s) near (x-7, y-8, z+14)
  bantile := CreateItemAtLocation(x + (-8), y + (-16), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (-16), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-6), y + (-16), Z + (14), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 2 door tile(s) near (x+8, y+9, z+27)
  bantile := CreateItemAtLocation(x + (7), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (8), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (9), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 2 door tile(s) near (x-4, y+6, z+27)
  bantile := CreateItemAtLocation(x + (-5), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-4), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-3), y + (17), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 1 door tile(s) near (x+9, y-7, z+27)
  bantile := CreateItemAtLocation(x + (16), y + (-8), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-7), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-6), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 2 door tile(s) near (x+13, y-12, z+37)
  bantile := CreateItemAtLocation(x + (16), y + (-13), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-12), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-11), Z + (37), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 7: 2 door tile(s) near (x-10, y+10, z+47)
  bantile := CreateItemAtLocation(x + (-16), y + (9), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (10), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (11), Z + (47), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction

function multi14a1housearray(house, x, y, z)
  // AUTO-GENERATED for MultiID 0x14a1 from real multi.mul component data.
  // Heuristic: door clusters detected via tiledata Door flag, ban strip placed
  // one tile beyond the nearest structure edge. REVIEW IN-GAME BEFORE TRUSTING.
  var boxarray := {};
  var box1 := {};
  box1.append(x + (-15));
  box1.append(y + (-15));
  box1.append(z - 8);
  box1.append(x + (15));
  box1.append(y + (16));
  box1.append(z + 80);
  boxarray[1] := box1;
  var banarray := {};
  var bantile;
  // doorway cluster 1: 8 door tile(s) near (x+10, y+10, z+22)
  bantile := CreateItemAtLocation(x + (16), y + (9), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (10), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (11), Z + (22), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 2: 10 door tile(s) near (x-8, y+10, z+19)
  bantile := CreateItemAtLocation(x + (-9), y + (17), Z + (19), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-8), y + (17), Z + (19), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-7), y + (17), Z + (19), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 3: 3 door tile(s) near (x-8, y+4, z+20)
  bantile := CreateItemAtLocation(x + (-16), y + (3), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (4), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-16), y + (5), Z + (20), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 4: 1 door tile(s) near (x+8, y-1, z+7)
  bantile := CreateItemAtLocation(x + (16), y + (-2), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (-1), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (16), y + (0), Z + (7), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 5: 2 door tile(s) near (x-2, y-2, z+17)
  bantile := CreateItemAtLocation(x + (-3), y + (-16), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (-16), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (17), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  // doorway cluster 6: 1 door tile(s) near (x-2, y-10, z+27)
  bantile := CreateItemAtLocation(x + (-3), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-2), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  bantile := CreateItemAtLocation(x + (-1), y + (-16), Z + (27), "bantile", 1, house.realm);
  banarray.append(bantile.serial);
  SetObjProperty(bantile, "houseserial", house.serial);
  SetObjProperty(house, "bantiles", banarray);
  return boxarray;
endfunction
