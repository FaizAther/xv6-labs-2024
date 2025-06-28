#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "Usage: uptime\n");
    exit(1);
  }
  
  int ticks = uptime();
  printf("uptime: %d ticks\n", ticks);
  exit(0);
} 