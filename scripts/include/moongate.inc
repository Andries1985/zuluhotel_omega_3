//moongate.inc

Const UOBJECT_GATE := 0x6002;

Const MOONGLOW_X := 4467;
Const MOONGLOW_Y := 1283;
Const MOONGLOW_Z := 5;

Const BRITAIN_X := 1336;
Const BRITAIN_Y := 1997;
Const BRITAIN_Z := 5;

Const JHELOM_X := 1499;
Const JHELOM_Y := 3771;
Const JHELOM_Z := 5;

Const YEW_X := 771;
Const YEW_Y := 753;
Const YEW_Z := 5;

Const MINOC_X := 2701;
Const MINOC_Y := 693;
Const MINOC_Z := 5;

Const TRINSIC_X := 1828;
Const TRINSIC_Y := 2948;
Const TRINSIC_Z := -20;

Const SKARA_X := 643;
Const SKARA_Y := 2067;
Const SKARA_Z := 5;

Const MAGINCIA_X := 3564;
Const MAGINCIA_Y := 2140;
Const MAGINCIA_Z := 34;

var a_moonglow 	:= array( MOONGLOW_X , MOONGLOW_Y , MOONGLOW_Z );
var a_britain 	:= array( BRITAIN_X , BRITAIN_Y , BRITAIN_Z );
var a_jhelom 	:= array( JHELOM_X , JHELOM_Y , JHELOM_Z );
var a_yew 		:= array( YEW_X , YEW_Y , YEW_Z );
var a_minoc 		:= array( MINOC_X , MINOC_Y , MINOC_Z );
var a_trinsic 	:= array( TRINSIC_X , TRINSIC_Y , TRINSIC_Z );
var a_skara 		:= array( SKARA_X , SKARA_Y , SKARA_Z );
var a_magincia 	:= array( MAGINCIA_X , MAGINCIA_Y , MAGINCIA_Z );

var gates := array( a_moonglow , a_britain , a_jhelom , a_yew , a_minoc , a_trinsic , a_skara , a_magincia);

var seqdest_MOON  :=array(  0,	7,	6,	5,	4,	3,	2,	1 );
var seqdest_BRIT  :=array(  1,	0,	7,	6,	5,	4,	3,	2 );
var seqdest_JHEL  :=array(  2,	1,	0,	7,	6,	5,	4,	3 );
var seqdest_YEW   :=array(  3,	2,	1,	0,	7,	6,	5,	4 );
var seqdest_MINOC :=array(  4,	3,	2,	1,	0,	7,	6,	5 );
var seqdest_TRINS :=array(  5,	4,	3,	2,	1,	0,	7,	6 );
var seqdest_SKARA :=array(  6,	5,	4,	3,	2,	1,	0,	7 );
var seqdest_MAGI  :=array(  7,	6,	5,	4,	3,	2,	1,	0 );

var dests := array( seqdest_MOON , seqdest_BRIT , seqdest_JHEL , seqdest_YEW , seqdest_MINOC , seqdest_TRINS , seqdest_SKARA , seqdest_MAGI );


