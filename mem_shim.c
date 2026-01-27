#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>


size_t numPtrs = 0;

typedef struct {
    void* ptr;
    size_t size;
} ptrData;

ptrData* allocatedPtrs = NULL;


__attribute__((destructor)) void destroy(void) {
    void *(*original_free) (void *ptr1);
    original_free = dlsym(RTLD_NEXT, "free");

    size_t totalLeak = 0;
    int leakCount = 0;
    for (int i = 0; i < numPtrs; ++i) {
        if (allocatedPtrs[i].ptr != NULL) {
            totalLeak += allocatedPtrs[i].size;
            ++leakCount;
            fprintf(stderr, "LEAK\t%zu\n", allocatedPtrs[i].size);
        }
    }

    fprintf(stderr,"TOTAL\t%d\t%zu\n", leakCount, totalLeak);
    original_free(allocatedPtrs);
    
}


void free(void *ptr) {
    void *(*original_free) (void* ptr1);
    original_free = dlsym(RTLD_NEXT, "free");

    int i = 0;
    while(allocatedPtrs[i].ptr != ptr && i < numPtrs) {
        ++i;
    }

    if (allocatedPtrs[i].ptr == ptr) {
        allocatedPtrs[i].ptr = NULL;
    }


    original_free(ptr);

}

void* malloc(size_t size) {
    void *(*original_realloc) (void* vptr, size_t newSize);
    original_realloc = dlsym(RTLD_NEXT, "realloc");
    
    void *(*original_malloc) (size_t size1);
    original_malloc = dlsym(RTLD_NEXT, "malloc");

    allocatedPtrs = original_realloc(allocatedPtrs, ++numPtrs * sizeof(ptrData));
    void* newData = original_malloc(size);
    ptrData newPtr = {newData, size};
    allocatedPtrs[numPtrs - 1] = newPtr;

    return newData;
}

void* calloc(size_t count, size_t size) {
    void *(*original_realloc) (void* vptr, size_t newSize);
    original_realloc = dlsym(RTLD_NEXT, "realloc");

    void *(*original_calloc) (size_t count1, size_t size1);
    original_calloc = dlsym(RTLD_NEXT, "calloc");

    allocatedPtrs = original_realloc(allocatedPtrs, ++numPtrs * sizeof(ptrData));
    void* newData = original_calloc(count, size);
    ptrData newPtr = {newData, size};
    allocatedPtrs[numPtrs - 1] = newPtr;

    return newData;
}


void* realloc(void *ptr, size_t size) {
    free(ptr);
    void* newPtr = malloc(size);

    return newPtr;
}