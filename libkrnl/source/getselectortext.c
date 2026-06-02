// getselectortext.c -- convert selectors to text strings

#include <string.h>

// inputs:  selector -- the selector you wish to print
//          buffer -- a buffer to return the string in
//          size -- the size of the buffer

// return:  nullptr if the buffer is null pointer
//          nullptr if the buffer is too small to hold the result
char* getSelectorText(int selector, char* buffer, size_t size) {
    // check for null buffer
    if (buffer == nullptr) return nullptr;

    switch (selector) {
        case 0x08:  
            // buffer is too small
            if (size < 20) return nullptr;

            strncpy(buffer,"KERNEL CODE SEGMENT",size);
            return buffer;
        case 0x10:
            // buffer too small
            if (size < 20) return nullptr;

            strncpy(buffer,"KERNEL DATA SEGMENT",size);
            return buffer;
        case 0x18:
            // buffer is too small
            if (size < 18) return nullptr;
            strncpy(buffer,"USER CODE SEGMENT",size);
            return buffer;
        case 0x20:
            // buffer is too small
            if (size < 18) return nullptr;
            strncpy(buffer,"USER DATA SEGMENT",size);
            return buffer;
       case 0x28:
            // buffer is too small
            if (size < 26) return nullptr;
            strncpy(buffer,"TRANSACTION STATE SEGMENT",size);
            return buffer;
        default:  
            if (size < 8) return nullptr;
            strncpy(buffer, "UNKNOWN",size);
            return buffer;
    }
}