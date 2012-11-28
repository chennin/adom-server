/* XXX XXX XXX THIS IS A SETUID ROOT PROGRAM! XXX XXX XXX */

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	struct passwd *pw;
	char fname[22];
	int fidx = getpid();
	int origuid = getuid();

	if (argc != 3) {
		fprintf(stderr, "Three arguments required\n");
		return 1;
	}

	pw = getpwnam(argv[1]);

	if (!pw) {
		fprintf(stderr, "No such account exists\n");
		return 1;
	}

	if (strcmp(pw->pw_shell, "/var/lib/adom/server/user_login_leagues") &&
			strcmp(pw->pw_shell, "/var/lib/adom/server/user_login")) {
		fprintf(stderr, "This does not seem to be a player account!\n");
		return 1;
	}

	setuid(pw->pw_uid);
	
	if (chdir(pw->pw_dir) < 0) {
		perror("chdir");
		return 1;
	}

	if (access("silence", F_OK) == 0) {
		fprintf(stderr, "%s has denied sending messages.  Sorry.\n",
			pw->pw_name);
		return 1;
	}

	if (mkdir(".messages", 0777) < 0 && errno != EEXIST) {
		perror("mkdir");
		return 1;
	}

	/* XXX pw reused */
	if (!(pw = getpwuid(origuid))) {
		fprintf(stderr, "You don't exist.  Go away.\n");
		return 1;
	}

	while (1) {
		int mfile;

		snprintf(fname, sizeof(fname), ".messages/%d", fidx++);
		mfile = open(fname, O_CREAT|O_EXCL|O_WRONLY, 0666);

		if (mfile < 0) {
			if (errno == EEXIST)
				continue;
			perror("creat");
			return 1;
		}

#define PUT(a) (write(mfile, (a), strlen(a)) == (int)strlen(a))

		if (!PUT(pw->pw_name) || !PUT(" ") || !PUT(argv[2]) || !PUT("\n")) {
			perror("write");
			return 1;
		}
		
		if (close(mfile) < 0) {
			perror("close");
			return 1;
		}

		break;
	}

	/* XXX having to temporarily disable errors sucks big time */
	system("screen -S adom -X eval 'msgminwait 0' 'msgwait 0' 'exec /var/lib/adom/server/message_display' 'msgminwait 1' 'msgwait 5'");
	/* if message_display is running or started, it'll see the message,
	 * we do this to kill (mostly) a race */

	usleep(200000);
	system("screen -S adom -X eval 'msgminwait 0' 'msgwait 0' 'exec /var/lib/adom/server/message_display' 'msgminwait 1' 'msgwait 5'");
	return 0;
}
