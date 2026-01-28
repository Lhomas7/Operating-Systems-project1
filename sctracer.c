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
    char* newArgs[argc + 1];
    if (p == 0) {
        for (int i = 1; i < argc; ++i) {
            strcpy(newArgs[i - 1], argv[i]);
        }
        newArgs[argc] = NULL;

        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);
        child = getpid();
        execvp(newArgs[0], newArgs); 

        

    }
    else {
        int staus, syscall_num;
        waitpid(child, &status, 0);

        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
    }

    return 0;
}