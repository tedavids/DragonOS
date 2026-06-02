// our memcmp implementation
#include <string.h>

int memcmp(const void* aptr, const void* bptr, size_t size) {
    const unsigned char* a = (const unsigned char*) aptr;
    const unsigned char* b = (const unsigned char*) bptr;

    for (size_t i = 0; i < size; i++) {
        // check if a is less than b
        if (a[i] < b[i]) 
            return -1;
        // check b is greater
        if (a[i] > b[i])
            return 1;
    } // for (size_t i = 0; i < size; i++)

    // both were the same
    return 0;
} //  int memcmp(const void* aptr, const void* bptr, size_t size)