#include <sys/ptrace.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv) {
    pid_t p = fork();
    if (p == 0) {
        printf("Hi I'm a child");

    }
    else {
        printf("Hi I'm a parent");
    }

    return 0;
}