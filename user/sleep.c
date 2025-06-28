#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    exit(1);
  }
  unsigned int snooze = atoi(argv[1]);
  printf("sleeping for %u\n", snooze);
  if (snooze >= 0) {
    sleep(snooze);
  }
  exit(0);
}
