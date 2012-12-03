#define _BSD_SOURCE
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

#define ADOM_111
//#define ADOM_100

//#define LEAGUE

#ifdef ADOM_111
#define WILDERNESS 0x04
#define SMC 0x1C

#define TOEF_VAL1 0x1A
#define TOEF_VAL2 0x01

#define SEC_BET_ANC 3600
#define COUNT_BEF_ANC 500

#define CURSES_INITSCR 0x081245ab
#define CURSES_COLS 0x0829e5d0
#define CURSES_LINES 0x0829e5dc

#endif



#ifdef ADOM_111
#define TURNCOUNTER 0x082b16e0
#define LEVELID 0x082add1c
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
  int month;
  char *me = getlogin();

  char *BINLOC = "/var/lib/adom/bin/";

  char *SAGEPATH = NULL, *SAGESO = NULL, *ADOMBIN = NULL;
  asprintf(&SAGEPATH, "%s%s", BINLOC, "adom-sage");
  asprintf(&SAGESO, "%s%s", BINLOC, "adom-sage-jaakkos.so");

#ifdef ADOM_111
#ifndef LEAGUE
  asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-111-bin");
#else
  asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-lea-bin");
#endif
#else
  asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-100-bin");
#endif

  char *STATUSDIR_PATH = NULL;
  asprintf(&STATUSDIR_PATH, "%s%s", BINLOC, "../player_locations");

  word code, backup;
  word initscr_backup;

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

  if(argc > 1 && !strcmp(argv[1], "--enable-sage"))
     sage = 1;
  
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

      /*
      #ifdef ADOM_111
      ptrace(PTRACE_GETREGS, pid, NULL, &regs);
      
      if(regs.eip == (CURSES_INITSCR + 3)) {
        word forced_cols, forced_lines;
        forced_cols.w = 80;
        forced_lines.w = 25;
        putdata(pid, CURSES_COLS, forced_cols.bytes, 1);
        putdata(pid, CURSES_LINES, forced_lines.bytes, 1);
        putdata(pid, CURSES_INITSCR, initscr_backup, 1);
        regs.eip = CURSES_INITSCR;
        ptrace(PTRACE_SETREGS, pid, NULL, &regs);
        continue;
      }
      #endif
      */

      handle_sig(pid, wait_val);
      orig_eax = ptrace(PTRACE_PEEKUSER, pid, 4*ORIG_EAX, NULL);
    } while(orig_eax != SYS_time);
    
    //ADOM seems to load the location into memory on the save screen!
    //HACK
    int counter = 0;
    while(1) {
      if(ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {

      }
      
      wait(&wait_val);
      handle_sig(pid, wait_val);
      
      if(WIFSTOPPED(wait_val) && !is_fatal_sig(WSTOPSIG(wait_val))) {
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	
	
	#ifdef ADOM_111
	  long level_val1, level_val2;
          getdata(pid, LEVELID, (char*)&level_val1, 1);
          getdata(pid, LEVELID+4, (char*)&level_val2, 1);

	  // use shared memory?
	  char fname[1024];
	  FILE *tmpf;

	  struct stat locfinfo;
	   
  	  //what is level_val2? always 1?
	  if(level_val1 == TOEF_VAL1 && level_val2 == TOEF_VAL2) {
	    snprintf(fname, 1024, "%s/%d-toef", STATUSDIR_PATH, pid);
	    tmpf = fopen(fname, "w");
	    if(tmpf) fclose(tmpf);
	  }
          else if (level_val1 == SMC && level_val2 == 0x01) {
            counter++;
            if (counter >= COUNT_BEF_ANC) {
  	      snprintf(fname, 1024, "%s/%s", STATUSDIR_PATH, me);
	    
	      time_t now = time(0);
	      time_t mtime = 0;
              time_t mtimen = 0;
	    
	      if (stat(fname, &locfinfo) >= 0) {
  	        mtime = locfinfo.st_mtim.tv_sec;
	        mtimen = locfinfo.st_mtim.tv_nsec;
              }
	      if (now > mtime + SEC_BET_ANC) {
	        tmpf = fopen(fname, "w");
	        if(tmpf) {
                  fprintf(tmpf,"Small Cave\n");
                  fclose(tmpf);
                  counter = 0;
                }
              }
	    }
          }
	#endif
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
