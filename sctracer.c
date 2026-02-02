
#define _GNU_SOURCE
#include <sys/ptrace.h>
#include <sys/user.h>
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
#include <stdbool.h>

typedef struct {
    int syscall_num;
    int count;
}syscall_info;

int main(int argc, char **argv) {
    pid_t child = fork();
    if (child == 0) {
        char* newArgs[2];

        newArgs[0] = argv[1];
        newArgs[1] = NULL;

        //get ready to trace myself
        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);
        child = getpid();

        //execute program to trace system calls from
        execv(newArgs[0],newArgs); 

        

    }
    else {
        int status, syscall_num;
        waitpid(child, &status, 0);

        syscall_info* syscalls = NULL;
        int syscall_size = 0;

        //make it easier to differentiate system calls from traps
        ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

        //keep tracking until the child program is finished
        while (!WIFEXITED(status)) {
            //wait for child status to change
            ptrace(PTRACE_SYSCALL, child, 0, 0);
            
            if(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
                //get the system call number
                syscall_num = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX, NULL);
            }
            if (syscall_size) {
                int i = 0;
                while(syscalls[i].syscall_num != syscall_num && i < syscall_size) {
                    ++i;
                }
            
                if (i == syscall_size) {
                    syscall_info call = {syscall_num, 1};
                    syscalls = realloc(syscalls, ++syscall_size * sizeof(syscall_info));
                    syscalls[syscall_size - 1] = call;
                }
                else {
                    if (i) {
                        ++syscalls[i - 1].count;
                    }
                    else {
                        ++syscalls[i].count;
                    }
                }
            }
            else {
                syscall_info call = {syscall_num, 1};
                syscalls = realloc(syscalls, ++syscall_size * sizeof(syscall_info));
                syscalls[syscall_size - 1] = call;

            }
            //printf("%d",syscall_num);

            ptrace(PTRACE_CONT, child, NULL, NULL);
            waitpid(child, &status, 0);
        }

        FILE* outFile = fopen(argv[2], "w");
        for (int i = 0; i < syscall_size; ++i) {
            fprintf(outFile, "%d\t%d\n",syscalls[i].syscall_num, syscalls[i].count);
        }
        fclose(outFile);
        free(syscalls);
    }

    return 0;
}