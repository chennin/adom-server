#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r+");
  rewind(f);
  fputc(0xFF, f);
  fclose(f);

  return 0;
}
