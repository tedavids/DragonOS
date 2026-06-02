// strncpy copy a string to another string

#include <string.h>

// inputs: dest -- a pointer to the destination 
//         src -- a pointer to the source
//         size -- the length of the destination

// returns: nullptr if dest or src are nullptrs
//          nullptr if size <= 0
//          dest with the string is copied
//            if (strlen(dest) +1) > size a string of length 
//            size -1 is returned with the size'th char '\0'

char* strncpy(char* dest, const char* const src, size_t size) {
    // check invalid parms
    if(!dest || !src || (size <= 0)) return nullptr;

    size_t copylen = ( (strlen(src) + 1) < size) ? strlen(src) : size;

    for (size_t i = 0; i < copylen; i++) {
        dest[i] = src[i];
        if (dest[i] == '\0') return dest;
    }
    dest[copylen] = '\0';

    return dest;
} // char* strncpy(char* dest, const char* src, size_t size)