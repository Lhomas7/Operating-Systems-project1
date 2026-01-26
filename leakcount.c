#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    pid_t p = fork();
    if (p == 0) {
    }
    else {
        wait(NULL);
        
    }

    return 0;
}