#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  char buf[1024];
  char *args[MAXARG];
  int i, n;
  char c;
  
  if(argc < 2){
    fprintf(2, "Usage: xargs <command> [args...]\n");
    exit(1);
  }
  
  // Copy command and its arguments
  for(i = 1; i < argc; i++){
    args[i-1] = argv[i];
  }
  
  // Read lines from stdin and execute command for each line
  while(1){
    i = 0;
    // Read a line
    while(1){
      n = read(0, &c, 1);
      if(n <= 0 || c == '\n')
        break;
      if(i < sizeof(buf) - 1)
        buf[i++] = c;
    }
    
    if(n <= 0)  // EOF
      break;
    
    if(i == 0)  // Empty line
      continue;
    
    buf[i] = '\0';  // Null terminate the string
    
    // Add the line as the last argument
    args[argc-1] = buf;
    args[argc] = 0;  // Null terminate args array
    
    int pid = fork();
    if(pid < 0){
      fprintf(2, "xargs: fork failed\n");
      exit(1);
    }
    
    if(pid == 0){
      // Child process - execute the command
      exec(args[0], args);
      fprintf(2, "xargs: exec %s failed\n", args[0]);
      exit(1);
    } else {
      // Parent process - wait for child
      wait(0);
    }
  }
  
  exit(0);
} 