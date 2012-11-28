#include <stdio.h>

int main(int argc, char **argv) {
	FILE *f = fopen(argv[1], "w");
	if (f != NULL)
	{
		rewind(f);
		fputc(0xFF, f);
		fclose(f);
	}
	else {
		perror("Unable to modify cnt");
		return 1;
	}
	return 0;
}
