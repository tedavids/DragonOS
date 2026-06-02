// strntok bring back tokens from the string

#include <string.h>

bool isdelim(char c, const char* const restrict delims) {
    if (!delims) return false;

    for (size_t i = 0; i < strlen(delims); i++) {
        if (c == delims[i]) return true;
    }
    // not in our list
    return false;
}

char* strntok(const char* const str, const char * const delim, char buffer[], int buflen) {
    // check parameters
    if (!str) return nullptr;
    if (!buflen) return nullptr;
    if (!delim) return nullptr;

    // get length of our string
    int len = (int) strlen(str);

    // pointer to first char
    size_t strptr = 0;

    // skip leading blanks, etc
    while ((strptr < (size_t) len) && (isdelim(str[strptr], delim))) strptr++;

    int bufptr = 0;
    // strptr now points to our first char
    while ((strptr < (size_t) len) && (!isdelim(str[strptr],delim))) {
        // copy to buffer
        buffer[bufptr++] = str[strptr++];
    }
    // trailing null
    buffer[bufptr] = '\0';

    return buffer;
}