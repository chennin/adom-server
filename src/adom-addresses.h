/*
  LEVELID is where the code for the current level is stores in memory.
  To find it yourself, start ADOM, then start a memory trainer (such as
  scanmem).  Search for '4' for the wilderness, then go to the small cave
  and search for 28. There's usually only one result.  This address is the one
  that should go in LEVELID. (If you need another, Terinyo is 5).

  EXPLVL is the player's experience level.  Can be found with any trainer.

  TURNCOUNTER is the number of elapsed game turns. Also easy to find.

  IDCOUNT is the level of the ID the player is on.  Use your trainer and move
  between ID 1/2/3/4/.. until there are only two results left.

  CHARNAME is the PC's name.  To find this address, do a string search in your
  memory trainer for the name, and take note of the addresses. Then save, quit,
  rerun ADOM, and STOP AT THE LOAD SCREEN.  Only one of the addresses found 
  earlier will be filled at this point.  That is the one you want.
*/
#ifdef ADOM_111
  #define LEVELID 0x082add1c
  #define EXPLVL 0x082b16dc
  #define TURNCOUNTER 0x082b16e0
  #define IDCOUNT 0x82b55cc /* and 0x82b621c */
  #define CHARNAME 0x82add00
#elif defined ADOM_100
  #define LEVELID 0x829e21c
  #define EXPLVL 0x82a1bdc
  #define TURNCOUNTER 0x82a1be0
  #define IDCOUNT 0x82a5acc /* and 0x82a6710 */
  #define CHARNAME 0x829e200
#elif defined ADOM_120p3
  #define LEVELID 0x827b0bc
  #define EXPLVL 0x827eb0c
  #define TURNCOUNTER 0x827eb10
  #define IDCOUNT 0x8282f9c /* and 0x8283d70 */
  #define CHARNAME 0x827b0a0
#elif defined ADOM_120p4
  #define LEVELID 0x827d0bc
  #define EXPLVL 0x8280b0c
  #define TURNCOUNTER 0x8280b10
  #define IDCOUNT 0x8284f9c /* and 0x8285d70 */
  #define CHARNAME 0x827d0a0
#elif defined ADOM_120p5
  #define LEVELID 0x827dcbc
  #define EXPLVL 0x828170c
  #define TURNCOUNTER 0x8281710
  #define IDCOUNT 0x8285ba0 /* and 0x8286978 */
  #define CHARNAME 0x827dca0
#elif defined ADOM_120p6
  #define LEVELID 0x828201c  
  #define EXPLVL 0x8285a74
  #define TURNCOUNTER 0x8285a78
  #define IDCOUNT 0x8289f08 /* and 0x828acec */
  #define CHARNAME 0x8282000
#elif defined ADOM_120p7
  #define LEVELID 0x828955c
  #define EXPLVL 0x828cfb4
  #define TURNCOUNTER 0x828cfb8
  #define IDCOUNT 0x8291448 /* and 0x829222c */
  #define CHARNAME 0x8289540
#elif defined ADOM_120p8
  #define LEVELID 0x828cdbc
  #define EXPLVL 0x8290814
  #define TURNCOUNTER 0x8290818
  #define IDCOUNT 0x8294ca8 /* and 0x8295a90 */
  #define CHARNAME 0x828cda0
#elif defined ADOM_120p9
  #define LEVELID 0x829f09c
  #define EXPLVL 0x82a2af4
  #define TURNCOUNTER 0x82a2af8
  #define IDCOUNT 0x82a6f88 /* and 0x82a7d70 */
  #define CHARNAME 0x829f080
#elif defined ADOM_120p10
  #define LEVELID 0x829f19c
  #define EXPLVL 0x82a2bf4
  #define TURNCOUNTER 0x82a2bf8
  #define IDCOUNT 0x82a7088 /* and 0x82a7e70 */
  #define CHARNAME 0x829f180
#elif defined ADOM_120p11
  #define LEVELID 0x829e07c
  #define EXPLVL 0x82a1ad4
  #define TURNCOUNTER 0x82a1ad8
  #define IDCOUNT 0x82a5f68 /* and 0x82a6d50 */
  #define CHARNAME 0x829e060
#elif defined ADOM_120p12
  #define LEVELID 0x82aef9c
  #define EXPLVL 0x82b29f4
  #define TURNCOUNTER 0x82b29f8
  #define IDCOUNT 0x82b7c5c /* and 0x82b8c60 */
  #define CHARNAME 0x82aef80
#elif defined ADOM_120p13
  #define LEVELID 0x82aeffc
  #define EXPLVL 0x82b2a54
  #define TURNCOUNTER 0x82b2a58
  #define IDCOUNT 0x82b7cbc /* and 0x82b8cc0 */
  #define CHARNAME 0x82aefe0
#elif defined ADOM_120p14
  #define LEVELID 0x82b229c
  #define EXPLVL 0x82b5cf4
  #define TURNCOUNTER 0x82b5cf8 
  #define IDCOUNT 0x82baf5c /* and 0x82bbf64 */
  #define CHARNAME 0x82b2280
#elif defined ADOM_120p16
  #define LEVELID 0x82b4adc
  #define EXPLVL 0x82b8534
  #define TURNCOUNTER 0x82b8538
  #define IDCOUNT 0x82bd79c /* and 0x82be7a4 */
  #define CHARNAME 0x82b4ac0
#elif defined ADOM_120p17
  #define LEVELID 0x82d791c
  #define EXPLVL 0x82db374
  #define TURNCOUNTER 0x82db378
  #define IDCOUNT 0x82e05dc /* and 0x82e15e8 */
  #define CHARNAME 0x82c79c4 /* INGAME, not loading screen.  Still works. */
#elif defined ADOM_120p18
  #define LEVELID 0x82d7fbc
  #define EXPLVL 0x82dba14
  #define TURNCOUNTER 0x82dba18
  #define IDCOUNT 0x82e1c88 /* and 0x82e0c7c (I64 seems to not be the master) */
  #define CHARNAME 0x82c8064 /* INGAME, not loading screen.  Still works. */
#elif defined ADOM_120p19
  #define LEVELID 0x82d8b5c
  #define EXPLVL 0x82dc5b4
  #define TURNCOUNTER 0x82dc5b8
  #define IDCOUNT 0x82e1820 /* and 0x82e282c (I64 seems to not be the master) */
  #define CHARNAME 0x82c8c04 /* and 0x82c8c64? INGAME. */
#endif

// Challenge game text
#define CHAL_WARN "\033[1;31mWARNING\033[0m: Challenge game locations are STRICTLY ENFORCED.\n"\
                  "You may NOT enter any place except the wilderness and your game's requirement.\n"\
                  "Once in the location, you MUST stay in it until the win condition is achieved.\n"\
                  "Entering ANY OTHER PLACE will get your process killed!\n\n"

#define WILD_WARN "NOTE: If you have any wilderness encounters on the way to your challenge, you\n"\
        "must evade them. If that fails, leave the encounter without killing anything.\n\n"

#define ETR_NAME "Eternium Man"
#define ETR_WARN "\n\n"\
                "                  @@@@@ \033[1m\033[4;37m" ETR_NAME " Challenge Game\033[0m @@@@@\n\n"\
                "                       Location: Small Cave\n"\
                "                       Win Condition: achieve level 50\n\n\n\n"

#define STE_NAME "Steel Man"
#define STE_WARN "\n\n"\
                "                  @@@@@ \033[1m\033[4;37m" STE_NAME " Challenge Game\033[0m @@@@@\n\n"\
                "                       Location: Wilderness\n"\
                "                       Win Condition: achieve level 50\n\n\n\n"

#define BRM_NAME "Brimstone Man"
#define BRM_WARN "\n\n"\
                "                  @@@@@ \033[1m\033[4;37m" BRM_NAME " Challenge Game\033[0m @@@@@\n\n"\
                "                     Location: The Tower of Eternal Flames\n"\
                "                     Win Condition: Retrieve the Chaos Orb of Elemental Fire\n\n\n\n"


#define IRO_NAME "Iron Man"
#define IRO_WARN "\n\n"\
                "                  @@@@@ \033[1m\033[4;37m" IRO_NAME " Challenge Game\033[0m @@@@@\n\n"\
                "                       Location: The Infinite Dungeon\n"\
                "                       Win Condition: Retrieve the Sceptre of Chaos\n\n\n\n"

#define LTH_NAME "Lithium Man"
#define LTH_WARN "\n\n"\
                "                  @@@@@ \033[1m\033[4;37m" LTH_NAME " Challenge Game\033[0m @@@@@\n\n"\
                "            Location: The Caverns of Chaos and Tower of Eternal Flames\n"\
                "            Win Condition: Win the game.\n"\
                "            Note: OC, DH, and DL are OK. The shortcut to the surface is not!!\n\n\n"
