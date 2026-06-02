// compare two string


#include <string.h>

int strncmp(int len, const char* const s1, const char* const s2) {
    if (len == 0) return 0;

    for (int i = 0; i < len; i++) {
        if (s1[i] < s2[i]) return -1;
        if (s1[i] > s2[i]) return 1;
        // check if both strings are done
        if ((s1[i] == 0) && (s2[i]== 0)) return 0;
        // if we get here they are equal, try next char
    }

    return 0;
}