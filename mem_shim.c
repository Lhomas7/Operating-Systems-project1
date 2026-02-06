#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

//set starting number of pointers to zero
size_t numPtrs = 0;

//define struct to hold each allocated pointer along with its size
typedef struct {
    void* ptr;
    size_t size;
} ptrData;

//declare array to hold structs of each dynamic pointer allocation
ptrData* allocatedPtrs = NULL;

//destructor to print data when program finishes
__attribute__((destructor)) void destroy(void) {

    //create function pointer to the original free function not overridden
    void *(*original_free) (void *ptr1);
    original_free = dlsym(RTLD_NEXT, "free");

    //iterate through the array of allocated pointers, 
    //printing the leaks for all un-freed pointers
    //and keeping track of the total number of leaked bytes
    size_t totalLeak = 0;
    int leakCount = 0;
    for (int i = 0; i < numPtrs; ++i) {
        if (allocatedPtrs[i].ptr != NULL) {
            totalLeak += allocatedPtrs[i].size;
            ++leakCount;
            fprintf(stderr, "LEAK\t%zu\n", allocatedPtrs[i].size);
        }
    }

    //display total leak amount, then free list of pointers
    fprintf(stderr,"TOTAL\t%d\t%zu\n", leakCount, totalLeak);
    original_free(allocatedPtrs);
    
}

//overridden free function
void free(void *ptr) {

    //create pointer to original free function
    void *(*original_free) (void* ptr1);
    original_free = dlsym(RTLD_NEXT, "free");

    //iterate though array until the given pointer is reached
    int i = 0;
    while(allocatedPtrs[i].ptr != ptr && i < numPtrs) {
        ++i;
    }

    //set the pointer in the array to NULL
    if (allocatedPtrs[i].ptr == ptr) {
        allocatedPtrs[i].ptr = NULL;
    }

    //free the given pointer
    original_free(ptr);

}

//Overridden malloc function
void* malloc(size_t size) {

    //create pointer to original realloc function
    void *(*original_realloc) (void* vptr, size_t newSize);
    original_realloc = dlsym(RTLD_NEXT, "realloc");

    //create pointer to original malloc function
    void *(*original_malloc) (size_t size1);
    original_malloc = dlsym(RTLD_NEXT, "malloc");

    //increase the array size and add the data for the pointer to be allocated
    //and allocate pointer data
    allocatedPtrs = original_realloc(allocatedPtrs, ++numPtrs * sizeof(ptrData));
    void* newData = original_malloc(size);
    ptrData newPtr = {newData, size};
    allocatedPtrs[numPtrs - 1] = newPtr;

    //return the allocated pointer
    return newData;
}

//Overridden calloc function
void* calloc(size_t count, size_t size) {

    //create pointer to original realloc function
    void *(*original_realloc) (void* vptr, size_t newSize);
    original_realloc = dlsym(RTLD_NEXT, "realloc");

    //create pointer to original calloc function
    void *(*original_calloc) (size_t count1, size_t size1);
    original_calloc = dlsym(RTLD_NEXT, "calloc");

    //increase the array size and add the data for the pointer to be allocated
    //and allocate pointer data
    allocatedPtrs = original_realloc(allocatedPtrs, ++numPtrs * sizeof(ptrData));
    void* newData = original_calloc(count, size);
    ptrData newPtr = {newData, size};
    allocatedPtrs[numPtrs - 1] = newPtr;

    //return the allocated pointer
    return newData;
}

//Overridden realloc function
void* realloc(void *ptr, size_t size) {

    //free the current pointer
    free(ptr);
    
    //reallocate the pointer to its new size
    void* newPtr = malloc(size);

    //return the pointer
    return newPtr;
}