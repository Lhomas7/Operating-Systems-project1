
#define _GNU_SOURCE
#include <sys/ptrace.h>
#include <linux/ptrace.h>
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

//struct to hold the system call # and 
//the number of occurrences
typedef struct {
    int syscall_num;
    int count;
}syscall_info;

//main function
int main(int argc, char **argv) {
    pid_t child = fork();
    if (child == 0) {
        char* newArgs[2];

        newArgs[0] = argv[1];
        newArgs[1] = NULL;

        //get ready to trace myself
        ptrace(PTRACE_TRACEME);

        kill(getpid(), SIGSTOP);
        //child = getpid();

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
        ptrace(PTRACE_SYSCALL, child, 0, 0);

        //keep tracking until the child program is finished
        while (!WIFEXITED(status)) {
            //wait for child status to change
            
            if(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80) {
                //get the system call number
                struct ptrace_syscall_info psi;
                ptrace(PTRACE_GET_SYSCALL_INFO, child, sizeof(struct ptrace_syscall_info), &psi);
                syscall_num = psi.entry.nr;
                //check if we are getting an entry number or not
                if (psi.op == PTRACE_SYSCALL_INFO_ENTRY) {
                    //if there have already been system calls, iterate through and check if the current one has
                    //occurred or not, then adjust array accordingly
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
                    //if the call is new, create a new instance and store it
                    else {
                        syscall_info call = {syscall_num, 1};
                        syscalls = realloc(syscalls, ++syscall_size * sizeof(syscall_info));
                        syscalls[syscall_size - 1] = call;

                    }
                }
            }
            //printf("%d",syscall_num);

            ptrace(PTRACE_SYSCALL, child, 0, 0);
            waitpid(child, &status, 0);
        }

        //TODO: sort array from smallest to largest system call #
        
        int count = 0 
        int min;
        int index = 0;
        while (count < syscall_size) {
            min = syscalls[count].syscall_num;
            for (int i = 0; i < syscall_size; ++i) {
                if (syscalls[i].syscall_num < min) {
                    min = syscalls[i].syscall_num;
                    index = i;
                }
            }
            syscall_info temp = syscalls[index];
            syscalls[index] = syscalls[count];
            syscalls[count] = temp.
            ++count;
        }
        //send call numbers and their counts to output file
        //in descending order
        FILE* outFile = fopen(argv[2], "w");
        for (int i = 0; i < syscall_size; ++i) {
            fprintf(outFile, "%d\t%d\n",syscalls[i].syscall_num, syscalls[i].count);
        }
        fclose(outFile);
        free(syscalls);
    }

    return 0;
}