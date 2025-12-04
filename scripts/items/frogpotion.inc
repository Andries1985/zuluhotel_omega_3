use uo;
include "include/client"l

program frog_potion (character, potion)
Const POISON_SOUND := 0x247;
Const DRINK_SOUND := 0x32;
Const UOBJ_EMPTY_BOTTLE := 0x0f0e;
Const EFFECT_BLUE_SPARKLE  := 0x373a;
Const SOUND_EFFECT_CURE	:= 0x1e1; 
Const EFFECT_SPARKLE  := 0x374a;
Const HIT_ACTION := 0x148b;
Const MOVING_EFFECT_PURPLE_POTION  := 0x0F0D; 
Const SOUND_EFFECT_EXPLOSION	:= 0x208; 
Const SOUND_EFFECT_HEAL	:= 0x1f3; 
Const EFFECT_EXPLOSION := 0x36b0;
Const SKILLID_TASTEID := 36;


sendsysmessage(character, "You feel funny!");
    PlaySoundEffect (character, DRINK_SOUND);
    CreateItemInBackpack(character, UOBJ_EMPTY_BOTTLE, 1);
    DestroyItem(potion);
    PlayObjectCenteredEffect( character, EFFECT_BLUE_SPARKLE, 10,10);  