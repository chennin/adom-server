/*
  Each level is specified by two codes, stored right next to each other.
  To find others, attach to ADOM with GDB, and print the address of LEVELID
  this is the first value.  Then add 4 to LEVELID and print it; this
  address is the second value.
*/

#define D1_1 0x01 /* D:1     	L1 */
#define D1_2 0x00 /* D:1     	L2 */
#define D2_1 0x02 /* D:2     	L1 */
#define D2_2 0x00 /* D:2     	L2 */
#define D3_1 0x03 /* D:3     	L1 */
#define D3_2 0x00 /* D:3     	L2 */
#define D4_1 0x04 /* D:4     	L1 */
#define D4_2 0x00 /* D:4     	L2 */
#define D5_1 0x05 /* D:5     	L1 */
#define D5_2 0x00 /* D:5     	L2 */
#define D6_1 0x06 /* D:6     	L1 */
#define D6_2 0x00 /* D:6     	L2 */
#define D7_1 0x07 /* D:7     	L1 */
#define D7_2 0x00 /* D:7     	L2 */
#define D8_1 0x08 /* D:8     	L1 */
#define D8_2 0x00 /* D:8     	L2 */
#define D9_1 0x09 /* D:9     	L1 */
#define D9_2 0x00 /* D:9     	L2 */
#define D10_1 0x0a /* D:10     	L1 */
#define D10_2 0x00 /* D:10     	L2 */
#define D11_1 0x0b /* D:11     	L1 */
#define D11_2 0x00 /* D:11     	L2 */
#define D12_1 0x0c /* D:12     	L1 */
#define D12_2 0x00 /* D:12     	L2 */
#define D13_1 0x0d /* D:13     	L1 */
#define D13_2 0x00 /* D:13     	L2 */
#define D14_1 0x0e /* D:14     	L1 */
#define D14_2 0x00 /* D:14     	L2 */
#define D15_1 0x0f /* D:15     	L1 */
#define D15_2 0x00 /* D:15     	L2 */
#define D16_1 0x10 /* D:16     	L1 */
#define D16_2 0x00 /* D:16     	L2 */
#define D17_1 0x11 /* D:17     	L1 */
#define D17_2 0x00 /* D:17     	L2 */
#define D18_1 0x12 /* D:18     	L1 */
#define D18_2 0x00 /* D:18     	L2 */
#define D19_1 0x13 /* D:19     	L1 */
#define D19_2 0x00 /* D:19     	L2 */
#define D20_1 0x14 /* D:20     	L1 */
#define D20_2 0x00 /* D:20     	L2 */
#define D21_1 0x15 /* D:21     	L1 */
#define D21_2 0x00 /* D:21     	L2 */
#define D22_1 0x16 /* D:22     	L1 */
#define D22_2 0x00 /* D:22     	L2 */
#define D23_1 0x17 /* D:23     	L1 */
#define D23_2 0x00 /* D:23     	L2 */
#define D24_1 0x18 /* D:24     	L1 */
#define D24_2 0x00 /* D:24     	L2 */
#define D25_1 0x19 /* D:25     	L1 */
#define D25_2 0x00 /* D:25     	L2 */
#define D26_1 0x1a /* D:26     	L1 */
#define D26_2 0x00 /* D:26     	L2 */
#define D27_1 0x1b /* D:27     	L1 */
#define D27_2 0x00 /* D:27     	L2 */
#define D28_1 0x1c /* D:28     	L1 */
#define D28_2 0x00 /* D:28     	L2 */
#define D29_1 0x1d /* D:29     	L1 */
#define D29_2 0x00 /* D:29     	L2 */
#define D30_1 0x1e /* D:30     	L1 */
#define D30_2 0x00 /* D:30     	L2 */
#define D31_1 0x1f /* D:31     	L1 */
#define D31_2 0x00 /* D:31     	L2 */
#define D32_1 0x20 /* D:32     	L1 */
#define D32_2 0x00 /* D:32     	L2 */
#define D33_1 0x21 /* D:33     	L1 */
#define D33_2 0x00 /* D:33     	L2 */
#define D34_1 0x22 /* D:34     	L1 */
#define D34_2 0x00 /* D:34     	L2 */
#define D35_1 0x23 /* D:35     	L1 */
#define D35_2 0x00 /* D:35     	L2 */
#define D36_1 0x24 /* D:36     	L1 */
#define D36_2 0x00 /* D:36     	L2 */
#define D37_1 0x25 /* D:37     	L1 */
#define D37_2 0x00 /* D:37     	L2 */
#define D38_1 0x26 /* D:38     	L1 */
#define D38_2 0x00 /* D:38     	L2 */
#define D39_1 0x27 /* D:39     	L1 */
#define D39_2 0x00 /* D:39     	L2 */
#define D40_1 0x28 /* D:40     	L1 */
#define D40_2 0x00 /* D:40     	L2 */
#define D41_1 0x29 /* D:41     	L1 */
#define D41_2 0x00 /* D:41     	L2 */
#define D42_1 0x2a /* D:42     	L1 */
#define D42_2 0x00 /* D:42     	L2 */
#define D43_1 0x2b /* D:43     	L1 */
#define D43_2 0x00 /* D:43     	L2 */
#define D44_1 0x2c /* D:44     	L1 */
#define D44_2 0x00 /* D:44     	L2 */
#define D45_1 0x2d /* D:45     	L1 */
#define D45_2 0x00 /* D:45     	L2 */
#define D46_1 0x2e /* D:46     	L1 */
#define D46_2 0x00 /* D:46     	L2 */
#define D47_1 0x2f /* D:47     	L1 */
#define D47_2 0x00 /* D:47     	L2 */
#define D48_1 0x30 /* D:48     	L1 */
#define D48_2 0x00 /* D:48     	L2 */
#define D49_1 0x31 /* D:49     	L1 */
#define D49_2 0x00 /* D:49     	L2 */
#define D50_1 0x32 /* D:50     	L1 */
#define D50_2 0x00 /* D:50     	L2 */
#define S_1 0x01 /* S2. S3? S1?   	L1 */
#define S_2 0x01 /* S2. S3? S1?   	L2 */
#define DH1_1 0x02 /* Dwarven Halls 1   	L1 */
#define DH1_2 0x01 /* Dwarven Halls 1   	L2 */
#define DH2_1 0x03 /* Dwarven Halls 2   	L1 */
#define DH2_2 0x01 /* Dwarven Halls 2   	L2 */
#define DrCh_1 0x04 /* Wilderness (Drakalor Chain)   	L1 */
#define DrCh_2 0x01 /* Wilderness (Drakalor Chain)   	L2 */
#define Vlge_1 0x05 /* Terinyo     	L1 */
#define Vlge_2 0x01 /* Terinyo     	L2 */
#define WDL_1 0x06 /* Water Dragon's Lair   	L1 */
#define WDL_2 0x01 /* Water Dragon's Lair   	L2 */
#define GY1_1 0x07 /* Dwarven Graveyard 1   	L1 */
#define GY1_2 0x01 /* Dwarven Graveyard 1   	L2 */
#define GY2_1 0x08 /* Dwarven Graveyard 2   	L1 */
#define GY2_2 0x01 /* Dwarven Graveyard 2   	L2 */
#define VD1_1 0x09 /* Village Dungeon 1   	L1 */
#define VD1_2 0x01 /* Village Dungeon 1   	L2 */
#define VD2_1 0x0a /* Village Dungeon 2   	L1 */
#define VD2_2 0x01 /* Village Dungeon 2   	L2 */
#define VD3_1 0x0b /* Village Dungeon 3   	L1 */
#define VD3_2 0x01 /* Village Dungeon 3   	L2 */
#define VD4_1 0x0c /* Village Dungeon 4   	L1 */
#define VD4_2 0x01 /* Village Dungeon 4   	L2 */
#define VD5_1 0x0d /* Village Dungeon 5   	L1 */
#define VD5_2 0x01 /* Village Dungeon 5   	L2 */
#define VD6_1 0x0e /* Village Dungeon 6   	L1 */
#define VD6_2 0x01 /* Village Dungeon 6   	L2 */
#define VD7_1 0x0f /* Village Dungeon 7   	L1 */
#define VD7_2 0x01 /* Village Dungeon 7   	L2 */
#define UCBT_1 0x10 /* High King's Tomb   	L1 */
#define UCBT_2 0x01 /* High King's Tomb   	L2 */
#define GRD_1 0x11 /* Gremlin Cave    	L1 */
#define GRD_2 0x01 /* Gremlin Cave    	L2 */
#define NDC_1 0x12 /* Non-descript cave above Darkforge  	L1 */
#define NDC_2 0x01 /* Non-descript cave above Darkforge  	L2 */
#define DFG_1 0x13 /* Darkforge     	L1 */
#define DFG_2 0x01 /* Darkforge     	L2 */
#define BUG_1 0x14 /* White Unicorn Glade   	L1 */
#define BUG_2 0x01 /* White Unicorn Glade   	L2 */
#define CGL_1 0x15 /* Black Unicorn Glade   	L1 */
#define CGL_2 0x01 /* Black Unicorn Glade   	L2 */
#define BVL_1 0x16 /* Lawenilothehl     	L1 */
#define BVL_2 0x01 /* Lawenilothehl     	L2 */
#define TF1_1 0x17 /* ToEF 1    	L1 */
#define TF1_2 0x01 /* ToEF 1    	L2 */
#define TF2_1 0x18 /* ToEF 2    	L1 */
#define TF2_2 0x01 /* ToEF 2    	L2 */
#define TF3_1 0x19 /* ToEF 3    	L1 */
#define TF3_2 0x01 /* ToEF 3    	L2 */
#define TF4_1 0x1a /* ToEF 4    	L1 */
#define TF4_2 0x01 /* ToEF 4    	L2 */
#define MML_1 0x1b /* Mad Minstrel's Clearing   	L1 */
#define MML_2 0x01 /* Mad Minstrel's Clearing   	L2 */
#define SMC_1 0x1c /* Small Cave    	L1 */
#define SMC_2 0x01 /* Small Cave    	L2 */
#define CCL_1 0x1d /* Old Barbarian's Clearing   	L1 */
#define CCL_2 0x01 /* Old Barbarian's Clearing   	L2 */
#define PL1_1 0x1e /* Pyramid 1    	L1 */
#define PL1_2 0x01 /* Pyramid 1    	L2 */
#define PL2_1 0x1f /* Pyramid 2    	L1 */
#define PL2_2 0x01 /* Pyramid 2    	L2 */
#define PL3_1 0x20 /* Pyramid 3    	L1 */
#define PL3_2 0x01 /* Pyramid 3    	L2 */
#define OC_1 0x21 /* Ogre Cave    	L1 */
#define OC_2 0x01 /* Ogre Cave    	L2 */
#define RL9_1 0x22 /* Rift 9    	L1 */
#define RL9_2 0x01 /* Rift 9    	L2 */
#define RL8_1 0x23 /* Rift 8    	L1 */
#define RL8_2 0x01 /* Rift 8    	L2 */
#define RL7_1 0x24 /* Rift 7    	L1 */
#define RL7_2 0x01 /* Rift 7    	L2 */
#define RL6_1 0x25 /* Rift 6    	L1 */
#define RL6_2 0x01 /* Rift 6    	L2 */
#define RL5_1 0x26 /* Rift 5    	L1 */
#define RL5_2 0x01 /* Rift 5    	L2 */
#define RL4_1 0x27 /* Rift 4    	L1 */
#define RL4_2 0x01 /* Rift 4    	L2 */
#define RL3_1 0x28 /* Rift 3    	L1 */
#define RL3_2 0x01 /* Rift 3    	L2 */
#define RL2_1 0x29 /* Rift 2    	L1 */
#define RL2_2 0x01 /* Rift 2    	L2 */
#define RL1_1 0x2a /* Rift 1    	L1 */
#define RL1_2 0x01 /* Rift 1    	L2 */
#define LIB_1 0x2b /* Sinister Library of Niltrias  	L1 */
#define LIB_2 0x01 /* Sinister Library of Niltrias  	L2 */
#define ID_1 0x2c /* Infinite Dungeon    	L1 */
#define ID_2 0x01 /* Infinite Dungeon    	L2 */
#define WE_1 0x2d /* Wilderness Encounter    	L1 */
#define WE_2 0x01 /* Wilderness Encounter    	L2 */
#define QL_1 0x2e /* Quickling Tree    	L1 */
#define QL_2 0x01 /* Quickling Tree    	L2 */
#define CDL_1 0x2f /* Stone Circle    	L1 */
#define CDL_2 0x01 /* Stone Circle    	L2 */
#define SIL_1 0x30 /* SIL (Special Infinity Level?)  	L1 */
#define SIL_2 0x01 /* SIL (Special Infinity Level?)  	L2 */
#define DDL_1 0x31 /* Dusty Level 1   	L1 */
#define DDL_2 0x01 /* Dusty Level 1   	L2 */
#define VDDL_1 0x32 /* Dusty Level 2   	L1 */
#define VDDL_2 0x01 /* Dusty Level 2   	L2 */
#define DD1_1 0x01 /* Druid Dungeon 1   	L1 */
#define DD1_2 0x02 /* Druid Dungeon 1   	L2 */
#define DD2_1 0x02 /* Druid Dungeon 2   	L1 */
#define DD2_2 0x02 /* Druid Dungeon 2   	L2 */
#define DD3_1 0x03 /* Druid Dungeon 3   	L1 */
#define DD3_2 0x02 /* Druid Dungeon 3   	L2 */
#define DD4_1 0x04 /* Druid Dungeon 4   	L1 */
#define DD4_2 0x02 /* Druid Dungeon 4   	L2 */
#define DD5_1 0x05 /* Druid Dungeon 5   	L1 */
#define DD5_2 0x02 /* Druid Dungeon 5   	L2 */
#define DD6_1 0x06 /* Druid Dungeon 6   	L1 */
#define DD6_2 0x02 /* Druid Dungeon 6   	L2 */
#define DD7_1 0x07 /* Druid Dungeon 7   	L1 */
#define DD7_2 0x02 /* Druid Dungeon 7   	L2 */
#define UD1_1 0x08 /* Unremarkable Dungeon 1   	L1 */
#define UD1_2 0x02 /* Unremarkable Dungeon 1   	L2 */
#define UD2_1 0x09 /* Unremarkable Dungeon 2   	L1 */
#define UD2_2 0x02 /* Unremarkable Dungeon 2   	L2 */
#define UD3_1 0x0a /* Unremarkable Dungeon 3   	L1 */
#define UD3_2 0x02 /* Unremarkable Dungeon 3   	L2 */
#define UD4_1 0x0b /* Unremarkable Dungeon 4   	L1 */
#define UD4_2 0x02 /* Unremarkable Dungeon 4   	L2 */
#define UD5_1 0x0c /* Unremarkable Dungeon 5   	L1 */
#define UD5_2 0x02 /* Unremarkable Dungeon 5   	L2 */
#define UD6_1 0x0d /* Unremarkable Dungeon 6   	L1 */
#define UD6_2 0x02 /* Unremarkable Dungeon 6   	L2 */
#define UD7_1 0x0e /* Unremarkable Dungeon 7   	L1 */
#define UD7_2 0x02 /* Unremarkable Dungeon 7   	L2 */
#define UD8_1 0x0f /* Unremarkable Dungeon 8   	L1 */
#define UD8_2 0x02 /* Unremarkable Dungeon 8   	L2 */
#define HIVI_1 0x10 /* High Mountain Village   	L1 */
#define HIVI_2 0x02 /* High Mountain Village   	L2 */
#define ChAoS_1 0x11 /* Chaos Plane    	L1 */
#define ChAoS_2 0x02 /* Chaos Plane    	L2 */
#define UC1_1 0x12 /* Tomb of the High Kings 1	L1 */
#define UC1_2 0x02 /* Tomb of the High Kings 1	L2 */
#define UC2_1 0x13 /* Tomb of the High Kings 2	L1 */
#define UC2_2 0x02 /* Tomb of the High Kings 2	L2 */
#define UC3_1 0x14 /* Tomb of the High Kings 3	L1 */
#define UC3_2 0x02 /* Tomb of the High Kings 3	L2 */
#define UC4_1 0x15 /* Tomb of the High Kings 4	L1 */
#define UC4_2 0x02 /* Tomb of the High Kings 4	L2 */
#define UC5_1 0x16 /* Tomb of the High Kings 5	L1 */
#define UC5_2 0x02 /* Tomb of the High Kings 5	L2 */
#define PC1_1 0x17 /* Puppy Cave 1   	L1 */
#define PC1_2 0x02 /* Puppy Cave 1   	L2 */
#define PC2_1 0x18 /* Puppy Cave 2   	L1 */
#define PC2_2 0x02 /* Puppy Cave 2   	L2 */
#define PC3_1 0x19 /* Puppy Cave 3   	L1 */
#define PC3_2 0x02 /* Puppy Cave 3   	L2 */
#define PC4_1 0x1a /* Puppy Cave 4   	L1 */
#define PC4_2 0x02 /* Puppy Cave 4   	L2 */
#define PC5_1 0x1b /* Puppy Cave 5   	L1 */
#define PC5_2 0x02 /* Puppy Cave 5   	L2 */
#define PC6_1 0x1c /* Puppy Cave 6   	L1 */
#define PC6_2 0x02 /* Puppy Cave 6   	L2 */
#define UL1_1 0x1d /* Unreal Dungeon 1   	L1 */
#define UL1_2 0x02 /* Unreal Dungeon 1   	L2 */
#define UL2_1 0x1e /* Unreal Dungeon 2   	L1 */
#define UL2_2 0x02 /* Unreal Dungeon 2   	L2 */
#define UL3_1 0x1f /* Unreal Dungeon 3   	L1 */
#define UL3_2 0x02 /* Unreal Dungeon 3   	L2 */
#define UL4_1 0x20 /* Unreal Dungeon 4   	L1 */
#define UL4_2 0x02 /* Unreal Dungeon 4   	L2 */
#define UL5_1 0x21 /* Unreal Dungeon 5   	L1 */
#define UL5_2 0x02 /* Unreal Dungeon 5   	L2 */
#define UL6_1 0x22 /* Unreal Dungeon 6   	L1 */
#define UL6_2 0x02 /* Unreal Dungeon 6   	L2 */
#define MT_1 0x23 /* Mana Temple    	L1 */
#define MT_2 0x02 /* Mana Temple    	L2 */
#define DL1_1 0x24 /* Blue Dragon Caves 1  	L1 */
#define DL1_2 0x02 /* Blue Dragon Caves 1  	L2 */
#define DL2_1 0x25 /* Blue Dragon Caves 2  	L1 */
#define DL2_2 0x02 /* Blue Dragon Caves 2  	L2 */
#define DL3_1 0x26 /* Blue Dragon Caves 3  	L1 */
#define DL3_2 0x02 /* Blue Dragon Caves 3  	L2 */
#define DL4_1 0x27 /* Blue Dragon Caves 4  	L1 */
#define DL4_2 0x02 /* Blue Dragon Caves 4  	L2 */
#define DL5_1 0x28 /* Blue Dragon Caves 5  	L1 */
#define DL5_2 0x02 /* Blue Dragon Caves 5  	L2 */
#define QQQQ_1 0x29 /* Assassin's Guild    	L1 */
#define QQQQ_2 0x02 /* Assassin's Guild    	L2 */
#define SC1_1 0x30 /* Scintillating Cave 1   	L1 */
#define SC1_2 0x02 /* Scintillating Cave 1   	L2 */
#define SC2_1 0x31 /* Scintillating Cave 2   	L1 */
#define SC2_2 0x02 /* Scintillating Cave 2   	L2 */
#define SC3_1 0x32 /* Scintillating Cave 3   	L1 */
#define SC3_2 0x02 /* Scintillating Cave 3   	L2 */
#define SC4_1 0x33 /* Scintillating Cave 4   	L1 */
#define SC4_2 0x02 /* Scintillating Cave 4   	L2 */
#define SC5_1 0x34 /* Scintillating Cave 5   	L1 */
#define SC5_2 0x02 /* Scintillating Cave 5   	L2 */
#define SC6_1 0x35 /* Scintillating Cave 6   	L1 */
#define SC6_2 0x02 /* Scintillating Cave 6   	L2 */
#define SC7_1 0x36 /* Scintillating Cave 7   	L1 */
#define SC7_2 0x02 /* Scintillating Cave 7   	L2 */
#define SC8_1 0x37 /* Scintillating Cave 8   	L1 */
#define SC8_2 0x02 /* Scintillating Cave 8   	L2 */
#define SC9_1 0x38 /* Scintillating Cave 9   	L1 */
#define SC9_2 0x02 /* Scintillating Cave 9   	L2 */
#define AR_1 0x01 /* Ancient City    	L1 */
#define AR_2 0x03 /* Ancient City    	L2 */
#define ML1_1 0x02 /* Minotaur Maze 1   	L1 */
#define ML1_2 0x03 /* Minotaur Maze 1   	L2 */
#define ML2_1 0x03 /* Minotaur Maze 2   	L1 */
#define ML2_2 0x03 /* Minotaur Maze 2   	L2 */
#define ML3_1 0x04 /* Minotaur Maze 3   	L1 */
#define ML3_2 0x03 /* Minotaur Maze 3   	L2 */
#define ML4_1 0x05 /* Minotaur Maze 4   	L1 */
#define ML4_2 0x03 /* Minotaur Maze 4   	L2 */
#define ML5_1 0x06 /* Minotaur Maze 5   	L1 */
#define ML5_2 0x03 /* Minotaur Maze 5   	L2 */
#define ML6_1 0x07 /* Minotaur Maze 6   	L1 */
#define ML6_2 0x03 /* Minotaur Maze 6   	L2 */
#define MC_1 0x08 /* Minotaur Maze 7   	L1 */
#define MC_2 0x03 /* Minotaur Maze 7   	L2 */
#define ML2a_1 0x09 /* False Minotaur Maze?     L1 */
#define ML2a_2 0x03 /* False Minotaur Maze?     L2 */
#define ML3a_1 0x0a /* False Minotaur Maze?     L1 */
#define ML3a_2 0x03 /* False Minotaur Maze?     L2 */
#define ML3b_1 0x0b /* False Minotaur Maze?     L1 */
#define ML3b_2 0x03 /* False Minotaur Maze?     L2 */
#define ML4a_1 0x0c /* False Minotaur Maze?     L1 */
#define ML4a_2 0x03 /* False Minotaur Maze?     L2 */
#define ML4b_1 0x0d /* False Minotaur Maze?     L1 */
#define ML4b_2 0x03 /* False Minotaur Maze?     L2 */
#define ML5a_1 0x0e /* False Minotaur Maze?     L1 */
#define ML5a_2 0x03 /* False Minotaur Maze?     L2 */
#define ML5b_1 0x0f /* False Minotaur Maze?     L1 */
#define ML5b_2 0x03 /* False Minotaur Maze?     L2 */
#define ML5c_1 0x10 /* False Minotaur Maze?     L1 */
#define ML5c_2 0x03 /* False Minotaur Maze?     L2 */
#define ML3c_1 0x11 /* False Minotaur Maze?     L1 */
#define ML3c_2 0x03 /* False Minotaur Maze?     L2 */
#define ML4c_1 0x12 /* False Minotaur Maze?     L1 */
#define ML4c_2 0x03 /* False Minotaur Maze?     L2 */
#define ML4e_1 0x13 /* False Minotaur Maze?     L1 */
#define ML4e_2 0x03 /* False Minotaur Maze?     L2 */
#define ML5d_1 0x14 /* False Minotaur Maze?     L1 */
#define ML5d_2 0x03 /* False Minotaur Maze?     L2 */
#define ML6a_1 0x15 /* False Minotaur Maze?     L1 */
#define ML6a_2 0x03 /* False Minotaur Maze?     L2 */
#define ML6b_1 0x16 /* False Minotaur Maze?     L1 */
#define ML6b_2 0x03 /* False Minotaur Maze?     L2 */
#define BUGVL_1 0x17 /* Bug Infested Temple   	L1 */
#define BUGVL_2 0x03 /* Bug Infested Temple   	L2 */
#define BUGCV_1 0x18 /* Bug Temple Cave   	L1 */
#define BUGCV_2 0x03 /* Bug Temple Cave   	L2 */
#define UCa_1 0x19 /* Introductory Dungeon   	L1 */
#define UCa_2 0x03 /* Introductory Dungeon   	L2 */
#define UCa1_1 0x1a /* Introductory Dungeon - Non-tutorial 1   	L1 */
#define UCa1_2 0x03 /* Introductory Dungeon - Non-tutorial 1 	L2 */
#define UCa2_1 0x1b /* Introductory Dungeon - Non-tutorial 2   	L1 */
#define UCa2_2 0x03 /* Introductory Dungeon - Non-tutorial 2 	L2 */
#define UCa3_1 0x1c /* Introductory Dungeon - Non-tutorial 3   	L1 */
#define UCa3_2 0x03 /* Introductory Dungeon - Non-tutorial 3 	L2 */
#define IC1_1 0x1d /* Ice Queen 1   	L1 */
#define IC1_2 0x03 /* Ice Queen 1   	L2 */
#define IC2_1 0x1e /* Ice Queen 2   	L1 */
#define IC2_2 0x03 /* Ice Queen 2   	L2 */
#define IC3_1 0x1f /* Ice Queen 3   	L1 */
#define IC3_2 0x03 /* Ice Queen 3   	L2 */
#define IC4_1 0x20 /* Ice Queen 4   	L1 */
#define IC4_2 0x03 /* Ice Queen 4   	L2 */
#define IC5_1 0x21 /* Ice Queen 5   	L1 */
#define IC5_2 0x03 /* Ice Queen 5   	L2 */
#define IC6_1 0x22 /* Ice Queen 6   	L1 */
#define IC6_2 0x03 /* Ice Queen 6   	L2 */
#define IC7_1 0x23 /* Ice Queen 7   	L1 */
#define IC7_2 0x03 /* Ice Queen 7   	L2 */
#define IC8_1 0x24 /* Ice Queen 8   	L1 */
#define IC8_2 0x03 /* Ice Queen 8   	L2 */
#define IC9_1 0x25 /* Ice Queen Throne   	L1 */
#define IC9_2 0x03 /* Ice Queen Throne   	L2 */
#define ICD_1 0x26 /* Ice Queen Danger   	L1 */
#define ICD_2 0x03 /* Ice Queen Danger   	L2 */
#define ARENA_1 0x26 /* ARENA (?)   	L1 */
#define ARENA_2 0x03 /* ARENA (?)   	L2 */
#define FGJC1_1 0x28 /* Front Giant Jarl Cave 1   	L1 */
#define FGJC1_2 0x03 /* Front Giant Jarl Cave 1   	L2 */
#define FGJC2_1 0x29 /* Front Giant Jarl Cave 2   	L1 */
#define FGJC2_2 0x03 /* Front Giant Jarl Cave 2   	L2 */
#define FGJC3_1 0x2a /* Front Giant Jarl Cave 3   	L1 */
#define FGJC3_2 0x03 /* Front Giant Jarl Cave 3   	L2 */
#define FGJC4_1 0x2b /* Front Giant Jarl Cave 4   	L1 */
#define FGJC4_2 0x03 /* Front Giant Jarl Cave 4   	L2 */
#define FGJC5_1 0x2c /* Front Giant Jarl Cave 5   	L1 */
#define FGJC5_2 0x03 /* Front Giant Jarl Cave 5   	L2 */
#define FGJC6_1 0x2d /* Front Giant Jarl Cave 6   	L1 */
#define FGJC6_2 0x03 /* Front Giant Jarl Cave 6   	L2 */
