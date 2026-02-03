#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#define SET_ENV "LD_PRELOAD=./mem_shim.so"

int main(int argc, char** argv) {
    pid_t p = fork();
    char* newArgs[argc];
    char* evs[] = {SET_ENV, NULL};
    if (p == 0) {
        int k = 0;
        for (int i = 1; i < argc; ++i) {
                newArgs[k] = argv[i];
                k++;
        }

        newArgs[k] = NULL;
        execvpe(newArgs[0], newArgs, evs);
    }
    else {
        wait(NULL);

    }

    return 0;
}
