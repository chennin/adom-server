/*
  Each level is specified by two codes, stored right next to each other.
  To find others, attach to ADOM with GDB, and print the address of LEVELID
  below; this is the first value.  Then add 4 to LEVELID and print it; this
  address is the second value.
*/
#define WILDERNESS_1 0x04
#define WILDERNESS_2 0x01
#define WILDENT_1 0x2d
#define WILDENT_2 0x01

#define SMC_1 0x1C
#define SMC_2 0x01

#define ID_1 0x2c
#define ID_2 0x01

#define UD1_1 0x8
#define UD1_2 0x2

#define TF1_1 0x17
#define TF1_2 0x01
#define TF2_1 0x18
#define TF2_2 0x01
#define TF3_1 0x19
#define TF3_2 0x01
#define TF4_1 0x1A
#define TF4_2 0x01

#define D48_1 0x30
#define D48_2 0x00
#define D49_1 0x31 //?
#define D49_2 0x00 //??
#define D50_1 0x32 //?
#define D50_2 0x00 //??

#define MANATEMP_1 0x23
#define MANATEMP_2 0x02

#define BDCBOT_1 0x28
#define BDCBOT_2 0x02

/*                                                                                LEVELID is where the code for the current level is stores in memory.
  To find it yourself, start ADOM, then start a memory trainer (such as
  scanmem).  Search for '4' for the wilderness, then go to the small cave
  and search for 28. There's usually only one result.  This address is the one
  that should go in LEVELID. (If you need another, Terinyo is 5).
*/
#ifdef ADOM_111
  #define LEVELID 0x082add1c
  #define EXPLVL 0x082b16dc
  #define TURNCOUNTER 0x082b16e0
  #define IDCOUNT 0x82b55cc
  #define CHARNAME 0x82add00
  /*#define IDCOUNT 0x82b621c //stored in two places?*/
#elif defined ADOM_100
  #define LEVELID 0x829e21c
#elif defined ADOM_120p3
  #define LEVELID 0x827b0bc
#elif defined ADOM_120p4
  #define LEVELID 0x827d0bc
#elif defined ADOM_120p5
  #define LEVELID 0x827dcbc
#endif

