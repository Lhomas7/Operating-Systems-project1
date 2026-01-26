#define _GNU_SOURCE
#define INVERSE = -1;
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>


size_t totalLeak = 0;
FILE* fptr;

__attribute__((destructor)) void destroy(void) {

    fclose(fptr);
}


void free(void *ptr) {
    void *(*original_free) (void *ptr1);
    original_free = dlsym(RTLD_NEXT, "free");

    totalLeak -= sizeof(*ptr);

    original_free(ptr);

}

void* malloc(size_t size) {
    void *(*original_malloc) (size_t size1);
    original_malloc = dlsym(RTLD_NEXT, "malloc");
    totalLeak += size;

    return original_malloc(size);
}

void* calloc(size_t count, size_t size) {
    void *(*original_calloc) (size_t count1, size_t size1);
    original_calloc = dlsym(RTLD_NEXT, "calloc");
    totalLeak += size;

    return original_calloc(count, size);
}

void* realloc(void *ptr, size_t size) {
    free(ptr);

    return malloc(size);
}