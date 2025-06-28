#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p[2]) __attribute__((noreturn));

void
primes(int p[2])
{
  int prime;
  int n;
  int p2[2];
  
  // Close write end of parent pipe
  close(p[1]);
  
  // Read first number from parent pipe
  if (read(p[0], &prime, sizeof(int)) != sizeof(int)) {
    // No more numbers, exit
    close(p[0]);
    exit(0);
  }
  
  // Print the prime number
  printf("prime %d\n", prime);
  
  // Create pipe for next process
  if (pipe(p2) < 0) {
    fprintf(2, "primes: pipe failed\n");
    exit(1);
  }
  
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "primes: fork failed\n");
    exit(1);
  }
  
  if (pid == 0) {
    // Child process - continue the sieve
    close(p[0]);  // Close parent pipe read end
    primes(p2);
  } else {
    // Parent process - filter out multiples of prime
    close(p2[0]);  // Close read end of child pipe
    
    // Read numbers from parent and filter
    while (read(p[0], &n, sizeof(int)) == sizeof(int)) {
      if (n % prime != 0) {
        // Not a multiple of prime, pass it to child
        if (write(p2[1], &n, sizeof(int)) != sizeof(int)) {
          fprintf(2, "primes: write failed\n");
          exit(1);
        }
      }
    }
    
    // Close pipes and wait for child
    close(p[0]);
    close(p2[1]);
    wait(0);
    exit(0);
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  
  // Create initial pipe
  if (pipe(p) < 0) {
    fprintf(2, "primes: pipe failed\n");
    exit(1);
  }
  
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "primes: fork failed\n");
    exit(1);
  }
  
  if (pid == 0) {
    // Child process - start the sieve
    primes(p);
  } else {
    // Parent process - feed numbers 2-280 into the pipeline
    close(p[0]);  // Close read end
    
    for (int i = 2; i <= 280; i++) {
      if (write(p[1], &i, sizeof(int)) != sizeof(int)) {
        fprintf(2, "primes: write failed\n");
        exit(1);
      }
    }
    
    // Close write end to signal end of input
    close(p[1]);
    
    // Wait for all children to finish
    wait(0);
    exit(0);
  }
} 