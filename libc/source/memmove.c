// our implementation of memmove function

#include <string.h>

void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;


    if (dst < src) {
        // overlap doesnt matter
        for (size_t i = 0; i < size; i++) 
            dst[i] = src[i]; 
    } else {
        // overlap might matter, so go backwards
        for (size_t i= size; i != 0; i--) 
            dst[i] = src[i];
    } // if (dst < src)

    return dstptr;
} // void* memmove(void* dstptr, const void* srcptr, size_t size) 
