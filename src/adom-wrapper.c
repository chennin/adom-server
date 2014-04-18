#define _BSD_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
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
//#define ADOM_120p7

//#define LEAGUE

#define ETR 0x1 /* Eternium, SMC only, til level 50 */
#define IRO 0x2 /* Iron, ID only, always descend, retrieve sceptre */
#define LTH 0x3 /* Lithium, CoC and ToEF only, win */
#define STE 0x4 /* Steel, Wilderness only, til level 50 */
#define BRM 0x5 /* Brimstone, ToEF only, retrieve orb */

// Minimum seconds between announcing a new player location
#define SEC_BET_ANC 1800
#define TURNS_BEF_ANC 5
#define NAMELEN 13

#include "adom-loc-codes.h"
#include "adom-addresses.h"

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
  //decide what game we're called as
  char *calledas = basename(argv[0]);
#define CHALLEN 4 /* eta, ste, ..., + null */
#define VERSLEN 7 /* 111, 120p3, ..., + null */
  char chal[CHALLEN], vers[VERSLEN];
  memset(chal,'\0',CHALLEN);
  memset(vers,'\0',VERSLEN);
  char *tmp = NULL;

  // examples: adom-111, adom-111-etr
  tmp = strtok(calledas, "-");
  if (tmp != NULL) { tmp = strtok(NULL, "-"); } // ignore "adom"
  if (tmp != NULL) { // grab version
    strncpy(vers, tmp, VERSLEN);
    if ((!isdigit(vers[0])) || (!isdigit(vers[1])) || (!isdigit(vers[2]))) {
      fprintf(stderr, "\"%s\" doesn't look like a valid ADOM version to me\n"
          "Name should be: adom-VER[-CHA]\n"
          "VER should be 3 numbers then two optional chars\n", vers);
      exit(1);
    }
    tmp = strtok(NULL, "-");
  }
  if (tmp != NULL) { // grab challenge
    strncpy(chal, tmp, CHALLEN);
    if ((!isalpha(chal[0])) || (!isalpha(chal[1])) || (!isalpha(chal[2]))) {
      fprintf(stderr, "\"%s\" doesn't look like a valid challenge game to me\n"
          "Name should be: adom-VER[-CHA]\n"
          "CHA should be 3 letters\n", chal);
      exit(1);
    }
  }
  if (strcmp(vers, "") == 0) {
    perror("Unable to determine what version of ADOM to run");
    exit(1);
  }

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
  // Used in challenge games
  int entered_loc = 0; // entered challenge location
  int never_die = 0; // won
  int nchal = 0;
#define CHALNALEN 64
  char chalname[CHALNALEN];
  // Used in ETR/STE
  int explvl = 0;
  // Used in IRO
  int idlvl = 0, newid = 0, got_to_sil = 0;

  // Print challenge banners
  if (strcmp(chal, "") != 0) {
    if (strcmp(chal,"etr") == 0) {
      printf("%s", ETR_WARN);
      nchal = ETR; snprintf(chalname, CHALNALEN, ETR_NAME);
    }
    else if (strcmp(chal,"ste") == 0) {
      printf("%s", STE_WARN);
      nchal = STE; snprintf(chalname, CHALNALEN, STE_NAME);
    }
    else if (strcmp(chal,"brm") == 0) {
      printf("%s", BRM_WARN);
      nchal = BRM; snprintf(chalname, CHALNALEN, BRM_NAME);
    }
    else if (strcmp(chal,"iro") == 0) {
      printf("%s", IRO_WARN);
      nchal = IRO; snprintf(chalname, CHALNALEN, IRO_NAME);
    }
    else if (strcmp(chal,"lth") == 0) {
      printf("%s", LTH_WARN);
      nchal = LTH; snprintf(chalname, CHALNALEN, LTH_NAME);
    }
    else {
      fprintf(stderr, "Unknown challenge game \"%s\"\n", chal);
      exit(1);
    }

    if (nchal != STE) {
      printf("%s", WILD_WARN);
    }
    printf("%s", CHAL_WARN);
    printf("\n[PRESS ENTER TO CONTINUE]\n");
    getchar();
  }

  const char *BINLOC = "/var/lib/adom/bin/";

  char *SAGEPATH = NULL, *SAGESO = NULL, *ADOMBIN = NULL;
  int e = 0, f = 0, g = 0;

  // Fill out what to run
#define BINLEN 17
#define SOLEN 22
#define SAGELEN 20
  char binname[BINLEN];
  char sagesoname[SOLEN];
  char sagename[SAGELEN];
  memset(binname,'\0',BINLEN);
  memset(sagesoname,'\0',SOLEN);
  memset(sagename,'\0',SAGELEN);

  strncpy(sagesoname, "adom-sage-0.9.19.so", SOLEN);
  strncpy(sagename, "adom-sage-0.9.19", SAGELEN);

  snprintf(binname, BINLEN, "adom-%s-bin", vers);
  if (strcmp(vers,"111") == 0) {
    if (strcmp(chal, "lea") == 0) {
      strncpy(binname, "adom-lea-bin", BINLEN);
    }
  }

  g = asprintf(&ADOMBIN, "%s%s", BINLOC, binname);
  e = asprintf(&SAGESO, "%s%s", BINLOC, sagesoname);
  f = asprintf(&SAGEPATH, "%s%s", BINLOC, sagename);
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
      // and run.
      if(!sage) {
        FILE *file = fopen(ADOMBIN, "r");
        if (file == NULL) {
          fprintf(stderr, "Can't find or read ADOM at %s\n", ADOMBIN);
          exit(1);
        }
        fclose(file);
        execl(ADOMBIN, ADOMBIN, NULL);
      }

      else {
        FILE *file = fopen(SAGEPATH, "r");
        if (file == NULL) {
          fprintf(stderr, "Can't find or read Sage at %s\n", SAGEPATH);
          exit(1);
        }
        fclose(file);
        file = fopen(SAGESO, "r");
        if (file == NULL) {
          fprintf(stderr, "Can't find or read Sage library at %s\n", SAGESO);
          exit(1);
        }
        fclose(file);
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
          getdata(pid, EXPLVL, (char*)&explvl, 1);

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
              else if (curloc_v1 == ChAoS_1 && curloc_v2 == ChAoS_2) { desc = "the ChAoS plane"; }
              else if (curloc_v1 == MT_1 && curloc_v2 == MT_2) { desc = "the Mana Temple"; }
              else if (curloc_v1 == DL5_1 && curloc_v2 == DL5_2) { desc = "the bottom of the Blue Dragon Caves"; }
              else if (curloc_v1 == SIL_1 && curloc_v2 == SIL_2) { desc = "the SIL"; }
              else if (curloc_v1 == SC9_1 && curloc_v2 == SC9_2) { desc = "the bottom of the Scintillating Cave"; }
              else if (curloc_v1 == CDL_1 && curloc_v2 == CDL_2) { desc = "the Stone Circle"; }
              else if (curloc_v1 == MC_1 && curloc_v2 == MC_2) { desc = "the bottom of the Minotaur Maze"; }
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
            // Playing a challenge game?
            if (nchal > 0) {
              int die = 0;
              if ((curloc_v1 == DrCh_1 && curloc_v2 == DrCh_2) ||
                  (curloc_v1 == WE_1 && curloc_v2 == WE_2)) { 
                // Allow encounters, but you should not kill anything
                // Saving goes through the wilderness
                if ((entered_loc == 1) && (cur_turn != prev_turn)) { die = 1; }
                // for Steel, do not set entered_loc, as above will kill
                if ((nchal == STE) && (explvl >= 50)) { never_die = 1; }
              }
              // enforce Eternium = SMC
              else if ((nchal == ETR) && (curloc_v1 == SMC_1) && (curloc_v2 == SMC_2)) {
                entered_loc = 1;
                if (explvl >= 50) { never_die = 1; }
              }
              // enforce Brimstone = ToEF
              else if ((nchal == BRM) && ( 
                (curloc_v1 == TF1_1 && curloc_v2 == TF1_2) || 
                (curloc_v1 == TF2_1 && curloc_v2 == TF2_2) ||
                (curloc_v1 == TF3_1 && curloc_v2 == TF3_2) ||
                (curloc_v1 == TF4_1 && curloc_v2 == TF4_2)                 
                ) )  {
                ;
                //entered_loc = 1; // No way to determine Orb retrieval? So don't kill on wilderness entry
              }
              // enforce Iron = ID and always go down
              else if ((nchal == IRO) && (
                (curloc_v1 == ID_1 && curloc_v2 == ID_2) ||
                (curloc_v1 == SIL_1 && curloc_v2 ==SIL_2)
                ) ) {
                //entered_loc = 1; // No way to determine Sceptre retrieval? So don't kill on wilderness entry
                getdata(pid, IDCOUNT, (char*)&newid, 1);
                // SIL == ID 66 ?
                if ((newid == 66 || (curloc_v1 == SIL_1 && curloc_v2 ==SIL_2))) { got_to_sil = 1; }
                if ((newid < idlvl) && (!got_to_sil)) { die = 1; } // went up before retrieving sceptre
              }
              // Enforce Lithium = D, S, TF, UL, MT, OC, and DL only
              else if ((nchal == LTH) && (
                (curloc_v2 == 0x00) ||  // 0x00 == D
                ((curloc_v2 == TF1_2) && ( // 0x01
                  (curloc_v1 == TF1_1) || (curloc_v1 == TF2_1) || (curloc_v1 == TF3_1) || (curloc_v1 == TF4_1) ||
                  (curloc_v1 == OC_1) || (curloc_v1 == DH1_1) || (curloc_v1 == DH2_1) || (curloc_v1 == S_1)
                )) ||
                ((curloc_v2 == UL1_2) && ( // 0x02
                  (curloc_v1 == UL1_1) || (curloc_v1 == UL2_1) || (curloc_v1 == UL3_1) || (curloc_v1 == UL4_1) || 
                  (curloc_v1 == UL5_1) || (curloc_v1 == UL6_1) || (curloc_v1 == MT_1) ||
                  (curloc_v1 == DL1_1) || (curloc_v1 == DL2_1) || (curloc_v1 == DL3_1) || (curloc_v1 == DL4_1) ||
                  (curloc_v1 == DL5_1)
                ))
                )) {
                   ; // have to leave CoC for ToEF, don't kill on wilderness entry
                }

              else { // some other forbidden location
                die = 1;
              }
              if(!never_die && die) { 
                ptrace(PTRACE_KILL, pid, NULL, NULL);
                int sys = system("setterm -reset");
                if (sys < 0) { perror("setterm failed"); }
                printf("\r\n\r\n\r\nWhoops! This location (0x%x,0x%x) (%d, %d) is not allowed for an honest %s.    \r\nYou are being terminated ...                            \r\n", 
                    (unsigned int)curloc_v1, (unsigned int)curloc_v2, idlvl, newid, chalname);
                sleep(8);
                return return_wrapper(63);
              }
            }
          }

          idlvl = newid;
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
