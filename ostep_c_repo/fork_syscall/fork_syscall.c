#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Hello World, i am (pid:%d)\n", getpid());
  int rc = fork();
  if (rc < 0) {
    fprintf(stderr, "process creation failed");
    exit(1);
  } else if (rc == 0) {
    printf("Hello, i am (pid:%d)\n)", getpid());
  } else {
    printf("Hello i am parent of %d (pid:%d)\n", rc, getpid());
  }

  return 0;
}
