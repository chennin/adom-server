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
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>

//#define ADOM_111
//#define ADOM_100
//#define ADOM_120p3
//#define ADOM_120p4
//#define ADOM_120p5
//#define ADOM_120p6

//#define LEAGUE

//#define ETR /* Eternium, SMC only, til level 50 */
//#define IRO /* Iron, ID only, always descend, retrieve sceptre */
//#define LTH /* Lithium, CoC and ToEF only, win */
//#define STE /* Steel, Wilderness only, til level 50 */
//#define BRM /* Brimstone, ToEF only, retrieve orb */

#if defined ETR || defined IRO || defined LTH || defined STE || defined BRM
 #define LOCCHA /* location-based challenge game */
#endif

// Minimum seconds between announcing a new player location
#define SEC_BET_ANC 1800
#define TURNS_BEF_ANC 5
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
    exit_wrapper(WEXITSTATUS(status));
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
    case SIGCHLD:

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
  struct passwd *p;
  if ((p = getpwuid(getuid())) == NULL) {
    perror("getpwuid() error");
  }
  char *me = p->pw_name;
  long prev_turn = 2147483647;
  int prevloc_v1 = 0, prevloc_v2 = 0;
  int loaded = 0, announced = 0, delaycounter = 0;
#ifdef LOCCHA
  int entered_loc = 0; // entered challenge location
  int never_die = 0; // won
#endif
#ifdef IRO
  int idlvl = 0;
#endif 
#if defined ETR || defined STE
  int explvl = 0;
#endif

#ifdef ETR
  printf("%s", ETR_WARN);
#endif
#ifdef LOCCHA
  printf("%s", CHAL_WARN);
#ifndef STEEL
  printf("%s", WILD_WARN);
#endif
  printf("\n[PRESS ENTER TO CONTINUE]\n");
  getchar();
#endif


  char *BINLOC = "/var/lib/adom/bin/";

  char *SAGEPATH = NULL, *SAGESO = NULL, *ADOMBIN = NULL;
  int e = 0, f = 0, g = 0;

  e = asprintf(&SAGESO, "%s%s", BINLOC, "adom-sage-0.9.3.so");
  f = asprintf(&SAGEPATH, "%s%s", BINLOC, "adom-sage-0.9.3"); 

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
#elif defined ADOM_120p6
  g = asprintf(&ADOMBIN, "%s%s", BINLOC, "adom-120p6-bin");
#endif

  if (e < 0 || f < 0 || g < 0)
  {
    perror("Failed to asprintf");
    exit_wrapper(1);
  }

  char *STATUSDIR_PATH = NULL;
  if (asprintf(&STATUSDIR_PATH, "%s%s", BINLOC, "../tmp/player_locations") < 0) 
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
      free(ADOMBIN); free(SAGEPATH); free(SAGESO);

      break;

    default: /* parent process */
      free(ADOMBIN); free(SAGEPATH); free(SAGESO);
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
        if(ptrace(PTRACE_SYSCALL, pid, 0, 0) != 0) {
                perror("ptrace_syscall");
        }

        wait(&wait_val);
        handle_sig(pid, wait_val);

        if(WIFSTOPPED(wait_val) && !is_fatal_sig(WSTOPSIG(wait_val))) {
          int curloc_v1 = 0, curloc_v2 = 0;
          getdata(pid, LEVELID, (char*)&curloc_v1, 1);
          getdata(pid, LEVELID+4, (char*)&curloc_v2, 1);
          long cur_turn = 0;	
          getdata(pid, TURNCOUNTER, (char*)&cur_turn, 1);

          //only check file once per turn count change
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
              delaycounter = 0;
            }
            // Announce if we haven't announced this one already
            // and if it's been at least one turn on the level
            if (announced == 0 && cur_turn > prev_turn) { 
              if (curloc_v1 == TF4_1 && curloc_v2 == TF4_2) { desc = "the top of the Tower of Eternal Flames"; }
#ifdef SMC_TEST
              else if (curloc_v1 == SMC_1 && curloc_v2 == SMC_2) { desc = "the Small Cave"; }
#endif
              else if (curloc_v1 == D50_1 && curloc_v2 == D50_2) { desc = "D:50"; }
              else if (curloc_v1 == CHAOS_1 && curloc_v2 == CHAOS_2) { desc = "the ChAoS plane"; }
              else if (curloc_v1 == MANATEMP_1 && curloc_v2 == MANATEMP_2) { desc = "the Mana Temple"; }
              else if (curloc_v1 == BDCBOT_1 && curloc_v2 == BDCBOT_2) { desc = "the bottom of the Blue Dragon Caves"; }
              else if (curloc_v1 == SIL_1 && curloc_v2 == SIL_2) { desc = "the SIL"; }
            }
            if (desc != NULL) {
              delaycounter++;
              if (delaycounter >= TURNS_BEF_ANC) {
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

#ifdef LOCCHA
            int die = 0;
            if ((curloc_v1 == WILDERNESS_1 && curloc_v2 == WILDERNESS_2) ||
               (curloc_v1 == WILDENT_1 && curloc_v2 == WILDENT_2)) { 
                // Allow encounters, but you should not kill anything
                // Saving goes through the wilderness
                if ((entered_loc == 1) && (cur_turn != prev_turn)) { die = 1; }
            }
#ifdef ETR
            else if (curloc_v1 == SMC_1 && curloc_v2 == SMC_2) {
                entered_loc = 1;
                if (explvl >= 50) { never_die = 1; }
            }
#endif
            else { // some other forbidden location
                die = 1;
            }
            if(!never_die && die) { 
                ptrace(PTRACE_KILL, pid, NULL, NULL);
                int sys = system("setterm -reset");
                printf("\r\n\r\n\r\nWhoops! This location (0x%x,0x%x) is not allowed for your challenge game.\r\nYou are being terminated ...\r\n", 
                       (unsigned int)curloc_v1, (unsigned int)curloc_v2);
                sleep(3);
                return return_wrapper(0);
            }
#endif /*LOCCHA*/
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
