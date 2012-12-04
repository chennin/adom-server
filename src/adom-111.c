#define _BSD_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <termios.h>
#include <ctype.h>
#include <time.h>

//#define ADOM_111
//#define ADOM_100
//#define ADOM_120p3
//#define ADOM_120p4
//#define ADOM_120p5

//#define LEAGUE

// Minimum seconds between announcing a new player location
#define SEC_BET_ANC 1800
// Minimum loop count before announcing a new player location
//  so as to not announce on the load menu, and to wait a few turns.
//  1000 seems to be about 4 actions.
#define COUNT_BEF_ANC 1000

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

/* 
  LEVELID is where the code for the current level is stores in memory.
  To find it yourself, start ADOM, then start a memory trainer (such as
  scanmem).  Search for '4' for the wilderness, then go to the small cave
  and search for 28. There's usually only one result.  This address is the one
  that should go in LEVELID. (If you need another, Terinyo is 5).
*/
#ifdef ADOM_111
  #define LEVELID 0x082add1c
#elif defined ADOM_100
  #define LEVELID 0x829e21c
#elif defined ADOM_120p3
  #define LEVELID 0x827b0bc 
#elif defined ADOM_120p4
  #define LEVELID 0x827d0bc
#elif defined ADOM_120p5
  #define LEVELID 0x827dcbc 
#endif

struct termios old_stdin_tio, old_stdout_tio;

int return_wrapper(int retval) {
  tcsetattr(STDIN_FILENO, TCSANOW, &old_stdin_tio);
  tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdout_tio);
  return retval;
}

void exit_wrapper(int exitval) {
  exit(return_wrapper(exitval));
}

typedef union {
  char bytes[4];
  long w;
} word;

void getdata(pid_t child, long addr, char *buf, int words)
{ 
  int i;
  word w;

  for(i=0; i<words; i++) {
    w.w = ptrace(PTRACE_PEEKDATA, child, addr + i*4, NULL);
    memcpy(buf + i*4, w.bytes, 4);
  }
}

void putdata(pid_t child, long addr, char *buf, int words)
{
  int i;
  word w;

  for(i=0; i<words; i++) {
    memcpy(w.bytes, buf + i*4, 4);
    ptrace(PTRACE_POKEDATA, child, addr + i*4, w.w);
  }
}

void handle_sig(int pid, int status) {
  if(WIFEXITED(status)) {
    exit_wrapper(0);
  }

  else if(WIFSIGNALED(status)) {
    kill(pid, 9);
    exit_wrapper(1);
  }
}

int is_fatal_sig(int sig) {
  char buf[512];
  FILE *f;

  switch(sig) {

  case SIGTRAP:
  case SIGWINCH:

    return 0;

  default:
    snprintf(buf, 512, "/tmp/fatal_sig_UID%u", getuid());
    f = fopen(buf, "w");
    
    if(f) {
      fprintf(f, "Received fatal signal %d\n", sig);
      fclose(f);
    }

    return 1;
  }
}

int main(int argc, char **argv)
{
  int wait_val;
  int pid;
  char sage=0;
  sigset_t mask;
  long orig_eax;
  struct user_regs_struct regs;
  char *me = getlogin();

  char *BINLOC = "/var/lib/adom/bin/";

  char *SAGEPATH = NULL, *SAGESO = NULL, *ADOMBIN = NULL;
  int e = 0, f = 0, g = 0;

  e = asprintf(&SAGESO, "%s%s", BINLOC, "adom-sage-092.so");
  f = asprintf(&SAGEPATH, "%s%s", BINLOC, "adom-sage-092"); 

#ifdef ADOM_111
  e = asprintf(&SAGESO, "%s%s", BINLOC, "adom-sage-jaakkos.so");
  f = asprintf(&SAGEPATH, "%s%s", BINLOC, "adom-sage");

  #ifndef LEAGUE
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-111-bin");
  #else
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-lea-bin");
  #endif
#elif defined ADOM_100
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-100-bin");
#elif defined ADOM_120p3
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-120p3-bin");
#elif defined ADOM_120p4
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-120p4-bin");
#elif defined ADOM_120p5
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-120p5-bin");
#endif

  if (e < 0 || f < 0 || g < 0)
  {
    perror("Failed to asprintf");
    exit_wrapper(1);
  }

  char *STATUSDIR_PATH = NULL;
  asprintf(&STATUSDIR_PATH, "%s%s", BINLOC, "../player_locations");

  if(argc > 1 && !strcmp(argv[1], "--enable-sage"))
     sage = 1;

  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTSTP);
  sigaddset(&mask, SIGQUIT);
  sigprocmask(SIG_BLOCK, &mask, NULL);

  tcgetattr(STDIN_FILENO, &old_stdin_tio);
  tcgetattr(STDOUT_FILENO, &old_stdout_tio);

  srand(time(NULL));

  switch(pid = fork()) {
    
  case -1:
    perror("fork");
    break;
    
  case 0: /*  child process */
    ptrace(PTRACE_TRACEME, 0, 0, 0);

    if(!sage) {
      execl(ADOMBIN, ADOMBIN, NULL);
    }

    else {
      execl(SAGEPATH, SAGEPATH, "-a", ADOMBIN, "-s", SAGESO, NULL);
    }

    break;
    
  default: /* parent process */
    wait(&wait_val);
    handle_sig(pid, wait_val);
  
    do {
      ptrace(PTRACE_SYSCALL, pid, 0, 0);
      wait(&wait_val);

      handle_sig(pid, wait_val);
      orig_eax = ptrace(PTRACE_PEEKUSER, pid, 4*ORIG_EAX, NULL);
    } while(orig_eax != SYS_time);
    
    //ADOM seems to load the location into memory on the save screen!
    //HACK
    int counter = 0;
    char *desc = NULL;
    while(1) {
      if(ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {

      }
      
      wait(&wait_val);
      handle_sig(pid, wait_val);
      
      if(WIFSTOPPED(wait_val) && !is_fatal_sig(WSTOPSIG(wait_val))) {
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	
	long level_val1, level_val2;
        getdata(pid, LEVELID, (char*)&level_val1, 1);
        getdata(pid, LEVELID+4, (char*)&level_val2, 1);

	//set desc if this is a level we should announce
        if (level_val1 == TOEF_VAL1 && level_val2 == TOEF_VAL2) { desc = "top of the Tower of Eternal Flames"; }
#ifdef SMC_TEST
        else if (level_val1 == SMC_1 && level_val2 == SMC_2) { desc = "Small Cave"; }
#endif
	else if (level_val1 == D50_1 && level_val2 == D50_2) { desc = "D:50"; }
	else if (level_val1 == MANATEMP_1 && level_val2 == MANATEMP_2) { desc = "Mana Temple"; }
	else if (level_val1 == BDCBOT_1 && level_val2 == BDCBOT_2) { desc = "the bottom of the Blue Dragon Caves"; }
	else { counter = 0; desc = NULL; } // unset if if it's not

	// if desc is set, announce it
	if (desc != NULL) {
	  // use shared memory?
	  struct stat locfinfo;
	  counter++;
	  if (counter >= COUNT_BEF_ANC) {
	    char fname[1024];
	    FILE *tmpf;
	    snprintf(fname, 1024, "%s/%s", STATUSDIR_PATH, me);

            time_t now = time(0);
	    time_t mtime = 0;

	    if (stat(fname, &locfinfo) >= 0) {
	      mtime = locfinfo.st_mtim.tv_sec;
	    }
	    if (now > mtime + SEC_BET_ANC) {
	      tmpf = fopen(fname, "w");
	      if(tmpf) {
	        fprintf(tmpf, "%s", desc);
	        fclose(tmpf);
	        counter = 0;
	      }
	    }
	  }
        }
      }
      else if (WIFSTOPPED(wait_val)) {
	ptrace(PTRACE_CONT, pid, 0, WSTOPSIG(wait_val));
      }
      else {
	kill(pid, 9);
	return return_wrapper(1);
      }
      
    }
  }
  
  return return_wrapper(0);
}
