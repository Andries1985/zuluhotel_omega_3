include "include/random";

function CreateBagWithPrize(who)
	var prize := GetPrize();
	var graphic := prize[1];
	if(!graphic)
		return 0;
  endif

	var bag := CreateItemInBackpack(who, 0x0E76);
	bag.name := "an Event Reward Bag";
	bag.color := Random(3000);

  var item := CreateItemInContainer(bag, graphic);
  item.movable := 1;

	var name := prize[2];
	if(name)
		item.name := name;
	endif

	var color := prize[3];
	if(color)
		item.color := color;
	endif

  return bag;
endfunction

// TODO: This would be better served with an indexed config file for items.
function GetPrize()
  var chance := Random(105), graphic, name, color;

  case (chance)
  1:
  2:
  3:
  4:
  5:
    graphic := 0x3DC4;
    name := "Baby Panda Statue";
  6:
    graphic := 0x3F3E;
    name := "Vase";
  7:
    graphic := 0x10DF;
    name := "Kitty Statue";
  8:
    graphic := 0x10E0;
    name := "Puppy Statue";
  9:
    graphic := 0x3CC7;
    name := "Gnome Statue";
  10:
    graphic := 0x3CC6;
    name := "Gnome Statue";
  11:
    graphic := 0x3D4F;
    name := "Doormat";
  12:
    graphic := 0x3D51;
    name := "Doormat";
  13:
    graphic := 0x3E24;
    name := "Banner";
  14:
    graphic := 0x1FDD;
    name := "Banner";
  15:
    graphic := 0x291D;
  16:
    graphic := 0x291F;
  17:
    graphic := 0x2A94;
  18:
    graphic := 0x2AED;
  19:
    graphic := 0x02DA;
  20:
    graphic := 0x0428;
  21:
    graphic := 0x0429;
  22:
    graphic := 0x041F;
  23:
    graphic := 0x0420;
  24:
    graphic := 0x081C;
    name := "Polar Bear Statue";
  25:
    graphic := 0x1387;
    name := "Bull Mask";
  26:
    graphic := 0x1388;
    name := "Bull Mask";
  27:
    graphic := 0x1389;
    name := "Shaman Mask";
  28:
    graphic := 0x138A;
    name := "Shaman Mask";
  29:
    graphic := 0x138B;
    name := "Jester Mask";
  30:
    graphic := 0x138C;
    name := "Jester Mask";
  31:
  32:
    graphic := 0x1390;
    name := "Monkey Mask";
  33:
    graphic := 0x18C8;
    name := "Walrus Trophy";
  34:
    graphic := 0x18C9;
    name := "Walrus Trophy";
  35:
    graphic := 0x18CA;
    name := "Ostard Trophy";
  36:
    graphic := 0x18CB;
    name := "Ostard Trophy";
  37:
    graphic := 0x18CC;
    name := "Crocodile Trophy";
  38:
    graphic := 0x18CD;
    name := "Crocodile Trophy";
  39:
    graphic := 0x18C6;
    name := "Wolf Trophy";
  40:
    graphic := 0x18C7;
    name := "Wolf Trophy";
  41:
    graphic := 0x1950;
    name := "Mongbat Statue";
  42:
    graphic := 0x1953;
    name := "Mongbat Statue";
  43:
    graphic := 0x194D;
    name := "Dragon Brazier";
  44:
    graphic := 0x1A0F;
    name := "Weapon Barrel";
  45:
    graphic := 0x1A10;
    name := "Weapon Barrel";
  46:
    graphic := 0x1A11;
    name := "Weapon Barrel";
  47:
    graphic := 0x1A12;
    name := "Weapon Barrel";
  48:
    graphic := 0x1A13;
    name := "Weapon Barrel";
  49:
    graphic := 0x1A14;
    name := "Weapon Barrel";
  50:
    graphic := 0x1A15;
    name := "Weapon Barrel";
  51:
    graphic := 0x1A16;
    name := "Weapon Barrel";
  52:
    graphic := 0x1B5E;
    name := "Banner";
  53:
    graphic := 0x1B96;
    name := "Banner";
  54:
    graphic := 0x1BB3;
    name := "Banner";
  55:
    graphic := 0x1BBE;
    name := "Banner";
  56:
    graphic := 0x1BC2;
    name := "Banner";
  57:
    graphic := 0x1BD0;
    name := "Banner";
  58:
    graphic := 0x1BFF;
    name := "Banner";
  59:
    graphic := 0x1C17;
    name := "Banner";
  60:
    graphic := 0x1E63;
    name := "Gorilla Trophy";
  61:
    graphic := 0x1E6A;
    name := "Gorilla Trophy";
  62:
    graphic := 0x1E64;
    name := "Orc Trophy";
  63:
    graphic := 0x1E6B;
    name := "Orc Trophy";
  64:
    graphic := 0x1E65;
    name := "Polar Bear Trophy";
  65:
    graphic := 0x1E6C;
    name := "Polar Bear Trophy";
  66:
    graphic := 0x1E66;
    name := "Troll Trophy";
  67:
    graphic := 0x1E6D;
    name := "Troll Trophy";
  68:
    graphic := 0x1E67;
    name := "Bear Trophy";
  69:
    graphic := 0x1E60;
    name := "Bear Trophy";
  70:
    graphic := 0x1E61;
    name := "Deer Trophy";
  71:
    graphic := 0x1E68;
    name := "Deer Trophy";
  72:
    graphic := 0x222F;
    name := "Mushroom";
  73:
    graphic := 0x2611;
    name := "Blueslaad Statue";
  74:
    graphic := 0x2605;
    name := "Arcane Daemon Statue";
  75:
    graphic := 0x2608;
    name := "Bog Statue";
  76:
    graphic := 0x260E;
    name := "Water Daemon Statue";
  77:
    graphic := 0x2614;
    name := "Quagmire Statue";
  78:
    graphic := 0x262C;
    name := "Abyssmal Horror Statue";
  79:
    graphic := 0x281C;
    name := "Golden Beetle Statue";
  80:
    graphic := 0x2636;
    name := "Daemon Trophy";
  81:
    graphic := 0x2637;
    name := "Daemon Trophy";
  82:
    graphic := 0x2855;
  83:
    graphic := 0x2856;
  84:
    graphic := 0x2A54;
    name := "Poisoned Arms Masters Assassin's Stygian Excalibur of Devastation";
  85:
    graphic := 0x2A7C;
    name := "Haunted Mirror";
  86:
    graphic := 0x2A7E;
    name := "Haunted Mirror";
  87:
    graphic := 0x32B2;
    name := "Aquarium";
  88:
    graphic := 0x32DC;
    name := "Mailbox";
  89:
    graphic := 0x32DE;
    name := "Mailbox";
  90:
    graphic := 0x3822;
    name := "Mirror";
  91:
    graphic := 0x3823;
    name := "Dream Catcher";
  92:
    graphic := 0x3824;
    name := "Dream Catcher";
  93:
    graphic := 0x3D21;
    name := "Glass Table";
  94:
    graphic := 0x3D45;
    name := "Banner";
  95:
    graphic := 0x3D46;
    name := "Banner";
  96:
    graphic := 0x3F8F;
    name := "Fountain";
  97:
    graphic := 0x3C1C;
    name := "Weapons Rack";
  98:
    graphic := 0x3C1D;
    name := "Weapons Rack";
  99:
    graphic := 0x3C20;
    name := "Weapons Rack";
  100:
    graphic := 0x3C21;
    name := "Weapons Rack";
  101:
    graphic := 0x3C1E;
    name := "Shields Rack";
  102:
    graphic := 0x3C1F;
    name := "Shields Rack";
  103:
    graphic := 0x32CD;
    name := "Aquarium";
  default:
    graphic := 9859;
    color := 1151 + random(33);
  endcase

  return { graphic, name, color };
endfunction
