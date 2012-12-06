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
#define NAMELEN 13

#include "adom-locs.h"

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
    if (errno != 0) { perror("PEEKDATA failed"); exit(1); }
    memcpy(buf + i*4, w.bytes, 4);
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
  long prev_turn = 2147483647;
  int prevloc_v1 = 0, prevloc_v2 = 0;
  int loaded = 0, announced = 0;

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
  if (asprintf(&STATUSDIR_PATH, "%s%s", BINLOC, "../player_locations") < 0) 
  {
    perror("Failed to asprintf");
    exit_wrapper(1);
  }

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

      char *desc = NULL;
      while(1) {
        if(ptrace(PTRACE_SYSCALL, pid, 0, 0) == -1) {

        }

        wait(&wait_val);
        handle_sig(pid, wait_val);

        if(WIFSTOPPED(wait_val) && !is_fatal_sig(WSTOPSIG(wait_val))) {
          ptrace(PTRACE_GETREGS, pid, NULL, &regs);

          int curloc_v1 = 0, curloc_v2 = 0;
          getdata(pid, LEVELID, (char*)&curloc_v1, 1);
          getdata(pid, LEVELID+4, (char*)&curloc_v2, 1);
          long cur_turn = 0;	
          getdata(pid, TURNCOUNTER, (char*)&cur_turn, 1);

          //only check once per turn count change
          if (!loaded && cur_turn != prev_turn) {
            // player names can be up to 12 characters long, but the first 8
            // chars have to be unique among your saves. The first 8 are used
            // for the save game name, .svg.
            // If the .svg doesn't exist, we have loaded a game.
            char *player = malloc(NAMELEN); 
            if (player == NULL) { perror("malloc failed"); exit(1); }
            memset(player,'\0',NAMELEN);
            getdata(pid, CHARNAME, player, 2);
            if (strlen(player) > 0) {
              char savename[1024] = "";
              snprintf(savename, 1024, ".adom.data/savedg/%s.svg", player);
              struct stat saveinfo;
              if (stat(savename, &saveinfo) < 0) {
                loaded = 1;
              }
            }
            free(player);
          }
          
          // set desc if this is a level we should announce
          // if a game has been loaded and at least one turn has been spent
          if (loaded == 1) {
            // Only reannounce if the location has changed
            if (announced == 1 && (curloc_v1 != prevloc_v1 || curloc_v2 != prevloc_v2)) {
              announced = 0;
            }
            // Announce if we haven't announced this one already
            // and if it's been at least one turn on the level
            if (announced == 0 && cur_turn > prev_turn) { 
              if (curloc_v1 == TF4_1 && curloc_v2 == TF4_2) { desc = "top of the Tower of Eternal Flames"; }
#ifdef SMC_TEST
              else if (curloc_v1 == SMC_1 && curloc_v2 == SMC_2) { desc = "Small Cave"; }
#endif
              else if (curloc_v1 == D50_1 && curloc_v2 == D50_2) { desc = "D:50"; }
              else if (curloc_v1 == MANATEMP_1 && curloc_v2 == MANATEMP_2) { desc = "Mana Temple"; }
              else if (curloc_v1 == BDCBOT_1 && curloc_v2 == BDCBOT_2) { desc = "the bottom of the Blue Dragon Caves"; }
            }
            if (desc != NULL) {
              // use shared memory?
              struct stat locfinfo;
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
                }
              }
              announced = 1;
            }
          }
          prev_turn = cur_turn;
          prevloc_v1 = curloc_v1;
          prevloc_v2 = curloc_v2;
          desc = NULL;
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
