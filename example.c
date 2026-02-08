#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int* ptr1 = malloc(4* sizeof(atoi(argv[1])));
    int* ptr2 = calloc(5, 6 * sizeof(atoi(arg[2])));

    free(ptr1);

    return 0;
}