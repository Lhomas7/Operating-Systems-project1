
#define _GNU_SOURCE
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
        char* newArgs[argc];
        int k = 0;
        for (int i = 1; i < argc; ++i) {
                newArgs[k] = argv[i];
                k++;
        }

        newArgs[k] = NULL;

        //get ready to trace myself
        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);
        child = getpid();

        //execute program to trace system calls from
        execvp(newArgs[0],newArgs); 

        

    }
    else {
        int status, syscall_num;
        waitpid(child, &status, 0);

        //make it easier to differentiate system calls from traps
        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        //keep tracking until the child program is finished
        while (!WIFEXITED(status)) {
            //wait for child status to change
            ptrace(PTRACE_SYSCALL, child, 0, 0);
            
            if(WIFSTOPPED(status) && !(WSTOPSIG(status) & 0x80)) {
                //get the system call number
                syscall_num = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX, NULL);
            }
            printf("%d",syscall_num);

            ptrace(PTRACE_CONT, child, NULL, NULL);

        }
    }

    return 0;
}