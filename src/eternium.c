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
#include <termios.h>
#include <ctype.h>
#include <time.h>

#define WILDERNESS 0x04
#define SMC 0x1C

#define TURNCOUNTER 0x082b16e0
#define LEVELID 0x082add1c

#define EXPLVL 0x082b16dc

#define BIRTHSIGN_ADDR 0x0813EE03 // 1.1.1
//#define BIRTHSIGN_ADDR 0x0813AC63 // 1.0.0

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

int select_month(void) {
  int month;

  do {
    printf("\033[2J\033[f");

    printf("Select the month of your birth:\r\n"
           "\r\n"
           "  ? - random\r\n"
           "\r\n"
           "  A - Raven\r\n"
           "  B - Book\r\n"
           "  C - Wand\r\n"
           "  D - Unicorn\r\n"
           "  E - Salamander\r\n"
           "  F - Dragon\r\n"
           "  G - Sword\r\n"
           "  H - Falcon\r\n"
           "  I - Cup\r\n"
           "  J - Candle\r\n"
           "  K - Wolf\r\n"
           "  L - Tree\r\n"
           "\r\n"
           "> ");

    fflush(stdout);
    month = toupper(fgetc(stdin));
  } while(month != '?' && (month < 'A' || month > 'L'));

  printf("\033[2J");
  fflush(stdout);

  return month == '?' ? 0 : month-'A'+1;
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
  char entered_smc = 0;
  char sage=0;
  long prev_turn=0;
  sigset_t mask;
  long orig_eax;
  unsigned long breakpoint;
  struct user_regs_struct regs;
  int month;
  char never_die = 0;

  word code, backup;

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
  breakpoint = BIRTHSIGN_ADDR;

  if(argc > 1 && !strcmp(argv[1], "--enable-sage"))
     sage = 1;
  
  switch(pid = fork()) {
    
  case -1:
    perror("fork");
    break;
    
  case 0: /*  child process */
    ptrace(PTRACE_TRACEME, 0, 0, 0);

    if(!sage)
      execl("/usr/games/adom-etr-bin", "/usr/games/adom-etr-bin", NULL);

    else execl("/var/lib/adom/server/adom-sage", "/var/lib/adom/server/adom-sage", "-a", "/usr/games/adom-etr-bin", "-s", "/var/lib/adom/server/adom-sage.so", NULL);

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

    getdata(pid, breakpoint, backup.bytes, 1);
    code = backup;
    code.bytes[0] = 0xCC;
    putdata(pid, breakpoint, code.bytes, 1);
    
    while(1) {
      if(ptrace(PTRACE_SYSCALL, pid, 0, 0) != 0)
	perror("ptrace");
      
      wait(&wait_val);
      handle_sig(pid, wait_val);
      
      if(WIFSTOPPED(wait_val) && !is_fatal_sig(WSTOPSIG(wait_val))) {
	ptrace(PTRACE_GETREGS, pid, NULL, &regs);
	
	if(regs.eip == (breakpoint + 1)) {
	  month = select_month();
	  if(!month) month = rand() % 12;
	  else month--;
	  
	  regs.eip = breakpoint;
	  regs.eax &= 0xFFFF0000;
	  regs.eax |= (unsigned int)(30*month + (rand()%30));
	  
	  ptrace(PTRACE_SETREGS, pid, NULL, &regs);
	  putdata(pid, breakpoint, backup.bytes, 1);
	}

	else {
	  long level = ptrace(PTRACE_PEEKDATA, pid, LEVELID, NULL);
	  long turn = ptrace(PTRACE_PEEKDATA, pid, TURNCOUNTER, NULL);
	  char die = 0; /* die or not? */

	  long explvl = ptrace(PTRACE_PEEKDATA, pid, EXPLVL, NULL);
	  if(explvl >= 50) never_die = 1;
	  
	  switch(level) {
	  case WILDERNESS:
	    /* if the PC visited SMC and is in wilderness this might be bad
	       but could also indicate saving. */
	    if(entered_smc && turn != prev_turn)
	      die = 1;
	    
	    break;
	    
	  case SMC:
	    entered_smc = 1;
	    break;
	    
	  default:
	    /* ADOM maybe initializing? */
	    if(level <= 0 || level > 0xFF)
	      break;
	    
	    /* Some weird place? */
	    die = 1;
	  }
	  
	  if(!never_die && die) { /* Poor bastard... */
	    ptrace(PTRACE_KILL, pid, NULL, NULL);
	    system("setterm -reset");
	    printf("\r\n\r\n\r\nWhoops! This (0x%x) is not a place for honest eternium men...\r\n", (unsigned int)level);
	    printf("Please wait 10 seconds...\r\n");
	    sleep(10);
	    return return_wrapper(0);
	  }
	  
	  prev_turn = turn;
	}
      }

      else {
	kill(pid, 9);
	return return_wrapper(1);
      }
      
    }
  }
  
  return return_wrapper(0);
}