/*
  Each level is specified by two codes, stored right next to each other.
  To find others, attach to ADOM with GDB, and print the address of LEVELID
  below; this is the first value.  Then add 4 to LEVELID and print it; this
  address is the second value.
*/
#define WILDERNESS 0x04
#define WILDERNESS_2 0x01
#define SMC_1 0x1C
#define SMC_2 0x01
#define TOEF_VAL1 0x1A
#define TOEF_VAL2 0x01
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

