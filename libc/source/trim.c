// trim.c  kernel trim functions

#include <stdint.h>

#include <string.h>

char* trimc(char c, char* restrict s) {
    if (!s) return nullptr;     // nothign to trim
    // set up pointers
 
    int newstart = 0;

    while ((s[newstart] == c) && (s[newstart] != '\0')) {
        newstart++;
    }

    // nothing to do
    if (!newstart) return s;

    int buffptr = 0;

    // move the string to the left
    for (int i = newstart; (s[i] != '\0'); i++) {
        s[buffptr++] = s[i];
    }

    // add trailing null
    s[buffptr] = 0;

    return s;
}

char* rtrimc(char c, char* restrict s) {
    if (!s) return nullptr; // nothing to do
    if (!(*s)) return s;  // 0 length string

    for (int i = (int) (strlen(s) - 1); i > 0; i--) {
        if (s[i] != c) return s; // nothing left to trim
        s[i] = '\0';
    }

    return s;
}

char* trimd(const char* const delims, char* restrict s) {
    if (!s) return nullptr;     // nothign to trim
    if (!delims) return nullptr;
    if (!delims[0]) return nullptr; // no delims
    // set up pointers
 
    int newstart = 0;

    while (isdelim(s[newstart], delims) && (s[newstart] != '\0')) {
        newstart++;
    }

    // nothing to do
    if (!newstart) return s;

    int buffptr = 0;

    // move the string to the left
    for (int i = newstart; (s[i] != '\0'); i++) {
        s[buffptr++] = s[i];
    }

    // add trailing null
    s[buffptr] = 0;

    return s;
}

char* rtrimd(const char * const delims, char* restrict s) {
    if (!s) return nullptr; // nothing to do
    if (!(*s)) return s;  // 0 length string
    if (!delims) return nullptr;
    if (!delims[0]) return nullptr; // no delims

    int len = (int) strlen(s);
    for (int i = (len - 1); i > 0; i--) {
        if (!isdelim(s[i],delims))  return s; // nothing left to trim
        s[i] = '\0';
    }

    return s;
}