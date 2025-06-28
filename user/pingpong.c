#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];  // Two pipes: p1 for parent->child, p2 for child->parent
  char buf[1];
  
  // Create the pipes
  if (pipe(p1) < 0 || pipe(p2) < 0) {
    fprintf(2, "pingpong: pipe failed\n");
    exit(1);
  }
  
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "pingpong: fork failed\n");
    exit(1);
  }
  
  if (pid == 0) {
    // Child process
    close(p1[1]);  // Close write end of p1
    close(p2[0]);  // Close read end of p2
    
    // Read byte from parent
    if (read(p1[0], buf, 1) != 1) {
      fprintf(2, "pingpong: child read failed\n");
      exit(1);
    }
    
    printf("%d: received ping\n", getpid());
    
    // Write byte back to parent
    if (write(p2[1], buf, 1) != 1) {
      fprintf(2, "pingpong: child write failed\n");
      exit(1);
    }
    
    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    // Parent process
    close(p1[0]);  // Close read end of p1
    close(p2[1]);  // Close write end of p2
    
    // Send byte to child
    buf[0] = 'x';
    if (write(p1[1], buf, 1) != 1) {
      fprintf(2, "pingpong: parent write failed\n");
      exit(1);
    }
    
    // Read byte from child
    if (read(p2[0], buf, 1) != 1) {
      fprintf(2, "pingpong: parent read failed\n");
      exit(1);
    }
    
    printf("%d: received pong\n", getpid());
    
    close(p1[1]);
    close(p2[0]);
    exit(0);
  }
} 