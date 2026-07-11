CreateSpawnPointContainer Reference
Source: Container entries parsed from all itemdesc.cfg files in this repo.

How CreateSpawnPointContainer validates:
- pt_data[2] may be a template name or numeric objtype.
- If template name is given, GetObjtypeByName(template) must resolve.
- Final created item must satisfy IsA(POLCLASS_CONTAINER).

SECTION 1: Recommended "safe for spawn loot chests" shortlist
Notes:
- This shortlist excludes clearly internal/system/special-purpose containers.
- Kept entries are physical world containers/furniture generally suitable for spawned loot.
- Format: HEX | TEMPLATE_NAME

0x0a2d | Drawer1
0x0a2e | Drawer2
0x0a2f | Drawer3
0x0a31 | DrawerDark1
0x0a32 | DrawerDark2
0x0a33 | DrawerDark3
0x0a35 | DrawersSW1
0x0a36 | DrawersSW2
0x0a37 | DrawersSW3
0x0a39 | DrawersDarkSW1
0x0a3a | DrawersDarkSW2
0x0a3b | DrawersDarkSW3
0x0a3e | Dresser3
0x0a3f | Dresser4
0x0a40 | Dresser5
0x0a41 | Dresser6
0x0a42 | Dresser7
0x0a43 | Dresser8
0x0a46 | Dresser11
0x0a47 | Dresser12
0x0a48 | Dresser13
0x0a49 | Dresser14
0x0a4a | Dresser15
0x0a4b | Dresser16
0x0A9B | FullBookCaseSouth3
0x0A9C | FullBookCaseEast3
0x0B49 | Writingdesk1
0x0B4A | Writingdesk2
0x0B4B | Writingdesk3
0x0B4C | Writingdesk4
0x1882 | WinnowingBasket
0x1e80 | SmallWoodenBox1
0x1e81 | SmallWoodenBox2
0x2256 | SmallBagBall
0x2257 | LargeBagBall
0x232A | GiftBoxSouth
0x232B | GiftBoxEast
0x24D5 | SquareBasketSouth
0x24D6 | SquareBasketEast
0x24D7 | Basket
0x24D8 | TallRoundBasket
0x24D9 | SmallSquareBasketSouth
0x24DA | SmallSquareBasketEast
0x24DD | SmallRoundBasket
0x2D05 | SimpleElvenArmoireSouth
0x2D06 | SimpleElvenArmoireEast
0x2D07 | FancyElvenArmoireSouth
0x2D08 | FancyElvenArmoireEast
0x2DF1 | RarewoodChestEast
0x2DF2 | RarewoodChestSouth
0x2DF3 | DecorativeBoxEast
0x2DF4 | DecorativeBoxSouth
0x3084 | ArcaneBookshelfPartSouth1
0x3086 | ArcaneBookshelfPartEast1
0x30E4 | ElvenDresserPartEast2
0x30E6 | ElvenDresserPartSouth2
0x4102 | PirateChestSouth
0x4106 | PirateChestEast
0x990 | RoundBasket
0x9A8 | StrongBoxSouth
0x9A9 | SmallCrateSouth
0x9AA | WoodenBoxSouth
0x9AB | SilverChestSouth
0x9AC | RoundBasketWithHandles
0x9B0 | BeltPouchSouth
0x9B1 | SmallBushel
0x9B2 | Backpack
0xA2C | BrownChestOfDrawersSouth
0xA30 | RedChestOfDrawersSouth
0xA34 | BrownChestOfDrawersEast
0xA38 | RedChestOfDrawersEast
0xA3C | VanitySouthRight
0xA3D | VanitySouthLeft
0xA44 | VanityEastRight
0xA45 | VanityEastLeft
0xA4C | RedArmoireOpenSouth
0xA4D | RedArmoireClosedSouth
0xA4E | BrownArmoireOpenSouth
0xA4F | BrownArmoireClosedSouth
0xA50 | RedArmoireOpenEast
0xA51 | RedArmoireClosedEast
0xA52 | BrownArmoireOpenEast
0xA53 | BrownArmooireClosedEast
0xA97 | FullBookCaseSouth1
0xA98 | FullBookCaseSouth2
0xA99 | FullBookCaseEast1
0xA9A | FullBookCaseEast2
0xA9D | EmptyBookCaseSouth
0xA9E | EmptyBookCaseEast
0xE3C | LargeCrateEast
0xE3D | LargeCrateSouth
0xE3E | MediumCrateEast
0xE3F | MediumCrateSouth
0xE40 | MetalChestEast
0xE41 | MetalChestSouth
0xE42 | WoodenChestEast
0xE43 | WoodenChestSouth
0xE75 | BackPackCore
0xE75 | BackPack2
0xE76 | Bag
0xE77 | OpenBarrel
0xE7A | PicnicBasket
0xE7C | SilverChestEast
0xE7D | WoodenBoxEast
0xE7E | SmallCrateEast
0xE7F | OpenKeg
0xE80 | StrongBoxEast
0xFAE | ClosedBarrel

SECTION 2: Full valid template list (all current Container definitions)
Notes:
- These all satisfy the definition-level requirement for CreateSpawnPointContainer.
- Some are system/special containers and may be undesirable for gameplay use.
- Format: HEX | TEMPLATE_NAME(S) | SOURCE_FILE
0x0a2d | Drawer1 | ./config/itemdesc.cfg
0x0a2e | Drawer2 | ./config/itemdesc.cfg
0x0a2f | Drawer3 | ./config/itemdesc.cfg
0x0a31 | DrawerDark1 | ./config/itemdesc.cfg
0x0a32 | DrawerDark2 | ./config/itemdesc.cfg
0x0a33 | DrawerDark3 | ./config/itemdesc.cfg
0x0a35 | DrawersSW1 | ./config/itemdesc.cfg
0x0a36 | DrawersSW2 | ./config/itemdesc.cfg
0x0a37 | DrawersSW3 | ./config/itemdesc.cfg
0x0a39 | DrawersDarkSW1 | ./config/itemdesc.cfg
0x0a3a | DrawersDarkSW2 | ./config/itemdesc.cfg
0x0a3b | DrawersDarkSW3 | ./config/itemdesc.cfg
0x0a3e | Dresser3 | ./config/itemdesc.cfg
0x0a3f | Dresser4 | ./config/itemdesc.cfg
0x0a40 | Dresser5 | ./config/itemdesc.cfg
0x0a41 | Dresser6 | ./config/itemdesc.cfg
0x0a42 | Dresser7 | ./config/itemdesc.cfg
0x0a43 | Dresser8 | ./config/itemdesc.cfg
0x0a46 | Dresser11 | ./config/itemdesc.cfg
0x0a47 | Dresser12 | ./config/itemdesc.cfg
0x0a48 | Dresser13 | ./config/itemdesc.cfg
0x0a49 | Dresser14 | ./config/itemdesc.cfg
0x0a4a | Dresser15 | ./config/itemdesc.cfg
0x0a4b | Dresser16 | ./config/itemdesc.cfg
0x0A9B | FullBookCaseSouth3 | ./pkg/items/containers/config/itemdesc.cfg
0x0A9C | FullBookCaseEast3 | ./pkg/items/containers/config/itemdesc.cfg
0x0B49 | Writingdesk1 | ./config/itemdesc.cfg
0x0B4A | Writingdesk2 | ./config/itemdesc.cfg
0x0B4B | Writingdesk3 | ./config/itemdesc.cfg
0x0B4C | Writingdesk4 | ./config/itemdesc.cfg
0x0e78 | Basin | ./config/itemdesc.cfg
0x0e83 | EmptyTub | ./config/itemdesc.cfg
0x1011 | KeyRing | ./pkg/items/keys/config/itemdesc.cfg
0x1048 | globe1 | ./config/itemdesc.cfg
0x1882 | WinnowingBasket | ./pkg/items/containers/config/itemdesc.cfg
0x1966A | vendorbag | ./pkg/items/containers/config/itemdesc.cfg
0x1e80 | SmallWoodenBox1 | ./config/itemdesc.cfg
0x1e81 | SmallWoodenBox2 | ./config/itemdesc.cfg
0x1eba | Toolkit | ./config/itemdesc.cfg
0x1EFFD | BankBox | ./pkg/items/containers/config/itemdesc.cfg
0x1F010 | Tillerman | ./pkg/multis/boat/config/itemdesc.cfg
0x1F013 | ShipsHold | ./pkg/items/containers/config/itemdesc.cfg
0x1F01A | Storage | ./pkg/items/containers/config/itemdesc.cfg
0x1FF01 | SecureTradeContainer | ./pkg/items/containers/config/itemdesc.cfg
0x1FF02 | WornItemsContainer | ./pkg/items/containers/config/itemdesc.cfg
0x2006 | (no Name key) | ./pkg/opt/loot/itemdesc.cfg
0x2256 | SmallBagBall | ./pkg/items/containers/config/itemdesc.cfg
0x2257 | LargeBagBall | ./pkg/items/containers/config/itemdesc.cfg
0x232A | GiftBoxSouth | ./pkg/items/containers/config/itemdesc.cfg
0x232B | GiftBoxEast | ./pkg/items/containers/config/itemdesc.cfg
0x24D5 | SquareBasketSouth | ./pkg/items/containers/config/itemdesc.cfg
0x24D6 | SquareBasketEast | ./pkg/items/containers/config/itemdesc.cfg
0x24D7 | Basket | ./pkg/items/containers/config/itemdesc.cfg
0x24D8 | TallRoundBasket | ./pkg/items/containers/config/itemdesc.cfg
0x24D9 | SmallSquareBasketSouth | ./pkg/items/containers/config/itemdesc.cfg
0x24DA | SmallSquareBasketEast | ./pkg/items/containers/config/itemdesc.cfg
0x24DD | SmallRoundBasket | ./pkg/items/containers/config/itemdesc.cfg
0x2D05 | SimpleElvenArmoireSouth | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2D06 | SimpleElvenArmoireEast | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2D07 | FancyElvenArmoireSouth | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2D08 | FancyElvenArmoireEast | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2DF1 | RarewoodChestEast | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2DF2 | RarewoodChestSouth | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2DF3 | DecorativeBoxEast | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x2DF4 | DecorativeBoxSouth | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x3084 | ArcaneBookshelfPartSouth1 | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x3086 | ArcaneBookshelfPartEast1 | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x30E4 | ElvenDresserPartEast2 | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x30E6 | ElvenDresserPartSouth2 | ./pkg/items/elvenFurniture/config/itemdesc.cfg
0x3d88 | Animal Pack | ./config/itemdesc.cfg
0x4102 | PirateChestSouth | ./pkg/items/containers/config/itemdesc.cfg
0x4106 | PirateChestEast | ./pkg/items/containers/config/itemdesc.cfg
0x620e | TrashCanOfWonders | ./pkg/opt/shilitems/itemdesc.cfg
0x7 | forensicviewcontainer | ./pkg/std/forensicevaluation/itemdesc.cfg
0x7007 | trashcan | ./pkg/opt/zuluitems/itemdesc.cfg
0x7100 | runebook | ./pkg/std/runebook/itemdesc.cfg
0x8 | shrunknpc | ./pkg/opt/shrink/itemdesc.cfg
0x990 | RoundBasket | ./pkg/items/containers/config/itemdesc.cfg
0x9A8 | StrongBoxSouth | ./pkg/items/containers/config/itemdesc.cfg
0x9A9 | SmallCrateSouth | ./pkg/items/containers/config/itemdesc.cfg
0x9A9A | lootbag | ./pkg/opt/lootlottery/itemdesc.cfg
0x9AA | WoodenBoxSouth | ./pkg/items/containers/config/itemdesc.cfg
0x9AB | SilverChestSouth | ./pkg/items/containers/config/itemdesc.cfg
0x9AC | RoundBasketWithHandles | ./pkg/items/containers/config/itemdesc.cfg
0x9B0 | BeltPouchSouth | ./pkg/items/containers/config/itemdesc.cfg
0x9B1 | SmallBushel | ./pkg/items/containers/config/itemdesc.cfg
0x9B2 | Backpack | ./pkg/items/containers/config/itemdesc.cfg
0xA2C | BrownChestOfDrawersSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA30 | RedChestOfDrawersSouth | ./pkg/items/containers/config/itemdesc.cfg
0xa300 | (no Name key) | ./pkg/opt/spawnpoint/config/itemdesc.cfg
0xA34 | BrownChestOfDrawersEast | ./pkg/items/containers/config/itemdesc.cfg
0xA38 | RedChestOfDrawersEast | ./pkg/items/containers/config/itemdesc.cfg
0xA3C | VanitySouthRight | ./pkg/items/containers/config/itemdesc.cfg
0xA3D | VanitySouthLeft | ./pkg/items/containers/config/itemdesc.cfg
0xA44 | VanityEastRight | ./pkg/items/containers/config/itemdesc.cfg
0xA45 | VanityEastLeft | ./pkg/items/containers/config/itemdesc.cfg
0xA4C | RedArmoireOpenSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA4D | RedArmoireClosedSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA4E | BrownArmoireOpenSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA4F | BrownArmoireClosedSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA50 | RedArmoireOpenEast | ./pkg/items/containers/config/itemdesc.cfg
0xA51 | RedArmoireClosedEast | ./pkg/items/containers/config/itemdesc.cfg
0xA52 | BrownArmoireOpenEast | ./pkg/items/containers/config/itemdesc.cfg
0xA53 | BrownArmooireClosedEast | ./pkg/items/containers/config/itemdesc.cfg
0xA97 | FullBookCaseSouth1 | ./pkg/items/containers/config/itemdesc.cfg
0xA98 | FullBookCaseSouth2 | ./pkg/items/containers/config/itemdesc.cfg
0xA99 | FullBookCaseEast1 | ./pkg/items/containers/config/itemdesc.cfg
0xA9A | FullBookCaseEast2 | ./pkg/items/containers/config/itemdesc.cfg
0xA9D | EmptyBookCaseSouth | ./pkg/items/containers/config/itemdesc.cfg
0xA9E | EmptyBookCaseEast | ./pkg/items/containers/config/itemdesc.cfg
0xba30 | BagOfInfiniteReagents | ./pkg/opt/shilitems/itemdesc.cfg
0xba33 | BagOfInfiniteNormalReagents | ./pkg/opt/shilitems/itemdesc.cfg
0xba34 | BagOfInfinitePaganReagents | ./pkg/opt/shilitems/itemdesc.cfg
0xba35 | BagOfInfiniteGems | ./pkg/opt/shilitems/itemdesc.cfg
0xE1C | backgammonboard1 | ./pkg/opt/zulugames/itemdesc.cfg
0xE3C | LargeCrateEast | ./pkg/items/containers/config/itemdesc.cfg
0xE3D | LargeCrateSouth | ./pkg/items/containers/config/itemdesc.cfg
0xE3E | MediumCrateEast | ./pkg/items/containers/config/itemdesc.cfg
0xE3F | MediumCrateSouth | ./pkg/items/containers/config/itemdesc.cfg
0xE40 | MetalChestEast | ./pkg/items/containers/config/itemdesc.cfg
0xE41 | MetalChestSouth | ./pkg/items/containers/config/itemdesc.cfg
0xE42 | WoodenChestEast | ./pkg/items/containers/config/itemdesc.cfg
0xE43 | WoodenChestSouth | ./pkg/items/containers/config/itemdesc.cfg
0xE75 | BackPackCore | ./pkg/items/containers/config/itemdesc.cfg
0xE75 | BackPack2 | ./pkg/items/containers/config/itemdesc.cfg
0xE76 | Bag | ./pkg/items/containers/config/itemdesc.cfg
0xE77 | OpenBarrel | ./pkg/items/containers/config/itemdesc.cfg
0xE79 | BeltPouchEast | ./pkg/items/containers/config/itemdesc.cfg
0xE7A | PicnicBasket | ./pkg/items/containers/config/itemdesc.cfg
0xE7C | SilverChestEast | ./pkg/items/containers/config/itemdesc.cfg
0xE7D | WoodenBoxEast | ./pkg/items/containers/config/itemdesc.cfg
0xE7E | SmallCrateEast | ./pkg/items/containers/config/itemdesc.cfg
0xE7F | OpenKeg | ./pkg/items/containers/config/itemdesc.cfg
0xE80 | StrongBoxEast | ./pkg/items/containers/config/itemdesc.cfg
0xECA | bones9 | ./pkg/items/containers/config/itemdesc.cfg
0xECB | bones8 | ./pkg/items/containers/config/itemdesc.cfg
0xECC | bones7 | ./pkg/items/containers/config/itemdesc.cfg
0xECD | bones6 | ./pkg/items/containers/config/itemdesc.cfg
0xECE | bones5 | ./pkg/items/containers/config/itemdesc.cfg
0xECF | bones4 | ./pkg/items/containers/config/itemdesc.cfg
0xED0 | bones3 | ./pkg/items/containers/config/itemdesc.cfg
0xED1 | bones2 | ./pkg/items/containers/config/itemdesc.cfg
0xED2 | bones | ./pkg/items/containers/config/itemdesc.cfg
0xFA6 | Checkerboard | ./pkg/opt/zulugames/itemdesc.cfg
0xFAD | backgammonboard2 | ./pkg/opt/zulugames/itemdesc.cfg
0xFAE | ClosedBarrel | ./pkg/items/containers/config/itemdesc.cfg
