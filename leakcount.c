#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    pid_t p = fork();
    char* newArgs[argc];
    if (p == 0) {
        //char* newArgs[] = {"./a.out", NULL};
        for (int i = 1; i < argc - 1; ++i) {
            strcpy(newArgs[i - 1], argv[i]);
        }
        newArgs[argc - 1] = NULL;
        execvp(newArgs[0], newArgs);         
    }
    else {
        wait(NULL);
        printf("exec function Failed!\n");
        
    }

    return 0;
}