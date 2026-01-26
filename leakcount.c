#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    pid_t p = fork();
    if (p == 0) {
        printf("Hi I'm a child\n");
    }
    else {
        wait(NULL);
        printf("Hi I'm a parent\n");
        
    }

    return 0;
}