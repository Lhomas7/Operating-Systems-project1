#include <stdlib.h>
#include <stdio.h>

int main() {
    int* ptr1 = malloc(4* sizeof(int));
    int* ptr2 = calloc(5, 6 * sizeof(int));

    free(ptr1);

    return 0;
}