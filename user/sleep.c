#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(2, "Usage: sleep seconds...\n");
    exit(1);
  }

  int secs = atoi(argv[1]); // passing pointer
  secs *= 10;

  if (secs < 0) {
    fprintf(2, "Sleep: only positive time allowed...\n");
    exit(1);
  }

  sleep(secs);

  exit(0);
}

