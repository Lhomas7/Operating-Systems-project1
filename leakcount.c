#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#define SET_ENV "LD_PRELOAD=./mem_shim.so"

int main(int argc, char** argv) {
    //split processes into child and parent
    pid_t p = fork();
    char* newArgs[argc];

    //set environment variable to load shim library first
    char* evs[] = {SET_ENV, NULL};
    
    //check if the process is the child or parent
    if (p == 0) {
        
        //change the arguments to be without the leakcount name in them
        int k = 0;
        for (int i = 1; i < argc; ++i) {
                newArgs[k] = argv[i];
                k++;
        }

        //add Null for exec function, then run program with environment variables set
        newArgs[k] = NULL;
        execvpe(newArgs[0], newArgs, evs);
    }
    else {
        //wait for child process to finish
        wait(NULL);

    }

    return 0;
}
