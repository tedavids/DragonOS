// snprintf.c -- implement the snprintf function
#include <stdint.h>
#include <stdarg.h>

#include <string.h>

int copystring( char* restrict buff, size_t buffsize, size_t* buffptr, const char* s) {
    int count = 0;

    // iterate through the string copying to buffer
    // along with making sure we don't overrun the buffer
    while ((*buffptr < buffsize) && (s[count] != '\0')) {
        buff[(*buffptr)++] = s[count++];
    }

    return count;
}

int (printstring(char* restrict buff, size_t buffsize, size_t * buffptr, va_list* parameters)) {

    char* s = va_arg((*parameters), char*);

    return copystring(buff, buffsize, buffptr, s);
}

int printHexLower(char* restrict buff, size_t buffsize, size_t *buffptr, const char * restrict format, 
                  int *formatptr, va_list* parameters) {
    // assumes format pointer is alreay at the next char

    // buffer used for all hex
    char hexbuffer[20];
    memset(hexbuffer,0,sizeof(hexbuffer));

    int count = 0;

    // check what next thing in format is:
    switch (format[(int)(*formatptr)]) {
        case 'b': {
            // valid consume the character
            *formatptr += 1;
            int tmp = (int) va_arg((*parameters), int);
            uint8_t val = (uint8_t) tmp;
            if (toHex8lower(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'w': {
            // valid consume the character
            *formatptr += 1;
            int tmp = (int) va_arg((*parameters), int);
            uint16_t val = (uint16_t) tmp;
            if (toHex16lower(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'l': {
            // valid consume the character
            *formatptr += 1;
            uint32_t val = (uint32_t) va_arg((*parameters), uint32_t);
            if (toHex32lower(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'q': {
            // valid consume the character
            *formatptr += 1;
            uint64_t val = (uint64_t) va_arg((*parameters), uint64_t);
            if (toHex64lower(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        default:
            // undefined, we have an issue
            // we will process the next char
            break;
    }
    return count;
}

int printHexUpper(char* restrict buff, size_t buffsize, size_t *buffptr, const char * restrict format, 
                  int *formatptr, va_list* parameters) {
    // assumes formatptr is on next char
    // buffer used for all hex
    char hexbuffer[20];
    memset(hexbuffer,0,sizeof(hexbuffer));

    int count = 0;

    // check what next thing in format is:
    switch (format[(int)(*formatptr)]) {
        case 'b': {
            // valid consume the character
            (*formatptr)++;
            int tmp = (int) va_arg((*parameters), int);
            uint8_t val = (uint8_t) tmp;
            if (toHex8upper(val, hexbuffer, sizeof(hexbuffer))) 
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'w': {
            // valid consume the character
            (*formatptr)++;
            int tmp = (int) va_arg((*parameters), int);
            uint16_t val = (uint16_t) tmp;
            if (toHex16upper(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'l': {
            // valid consume the character
            (*formatptr)++;
            uint32_t val = (uint32_t) va_arg((*parameters), uint32_t);
            if (toHex32upper(val, hexbuffer, sizeof(hexbuffer))) 
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        case 'q': {
            // valid consume the character
            (*formatptr)++;
            uint64_t val = (uint64_t) va_arg((*parameters), uint64_t);
            if (toHex64upper(val, hexbuffer, sizeof(hexbuffer)))
                copystring(buff, buffsize, buffptr, hexbuffer);
            break;
        }
        default:
            // undefined, we have an issue
            // we will process the next char
            break;
    }
    return count;
}

int printInt(char* restrict buff, size_t buffsize, size_t *buffptr, const char * restrict format, 
                  int *formatptr, va_list* parameters) {
   // assumes formatptr is on next char
    // buffer used for all hex
    char workbuffer[22];

    int count = 0;

    // check what next thing in format is:
    switch (format[(int)(*formatptr)]) {
        case 'b': {
            // valid consume the character
            (*formatptr)++;
            int8_t tmp = (int8_t) va_arg((*parameters), int);
            int8_t val = (int8_t) tmp;
            if (toBase10ib(val, workbuffer, sizeof(workbuffer))) 
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'w': {
            // valid consume the character
            (*formatptr)++;
            int16_t tmp = ((int16_t) va_arg((*parameters), int));
            int16_t val = (int16_t) tmp;
            if (toBase10iw(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'l': {
            // valid consume the character
            (*formatptr)++;
            int32_t val = (int32_t) va_arg((*parameters), int32_t);
            if (toBase10il(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'q': {
            // valid consume the character
            (*formatptr)++;
            int64_t val = (int64_t) va_arg((*parameters), int64_t);
            if (toBase10iq(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        default:
            // undefined, we have an issue
            // we will process the next char
            break;
    }
    return count;
}

int printUInt(char* restrict buff, size_t buffsize, size_t *buffptr, const char * restrict format, 
                  int *formatptr, va_list* parameters) {
   // assumes formatptr is on next char
    // buffer used for all hex
    char workbuffer[22];

    int count = 0;

    // check what next thing in format is:
    switch (format[(int)(*formatptr)]) {
        case 'b': {
            // valid consume the character
            (*formatptr)++;
            uint8_t tmp = (uint8_t) va_arg((*parameters), int);
            uint8_t val = (uint8_t) tmp;
            if (toBase10ub(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'w': {
            // valid consume the character
            (*formatptr)++;
            uint16_t tmp = (uint16_t) va_arg((*parameters), int);
            uint16_t val = (uint16_t) tmp;
            if (toBase10uw(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'l': {
            // valid consume the character
            (*formatptr)++;
            uint32_t val = (uint32_t) va_arg((*parameters), uint32_t);
            if (toBase10ul(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        case 'q': {
            // valid consume the character
            (*formatptr)++;
            uint64_t val = (uint64_t) va_arg((*parameters), uint64_t);
            if (toBase10uq(val, workbuffer, sizeof(workbuffer)))
                copystring(buff, buffsize, buffptr, workbuffer);
            break;
        }
        default:
            // undefined, we have an issue
            // we will process the next char
            break;
    }
    return count;
}

void formattedText( char* restrict buff, size_t buffsize, size_t *buffptr, const char* restrict format, 
                    int *formatptr, va_list* parameters) {
    // handle special case
    switch (format[(*formatptr)]) {
        case '%': 
            // consume the second %
            (*formatptr)++;
            copystring(buff, buffsize, buffptr, "%");
            break;
        case 'b': {
            // print bool as 0|1
            //consume the 'b'
            (*formatptr)++;
            bool val = (bool) va_arg((*parameters), int /* char promotes to int?*/);
            if (val) 
                copystring(buff, buffsize, buffptr, "1");
            else
                copystring(buff, buffsize, buffptr, "0");
            }
            break;
        case 'B': {
            // print bool as true|false;
            // consume the 'B'
            (*formatptr)++;
            bool val = (bool) va_arg((*parameters), int /* char promotes to int?*/);
            if (val) 
                copystring(buff, buffsize, buffptr, "true");
            else
                copystring(buff, buffsize, buffptr, "false");
            }
            break;
        case 'c': 
            // we need to copy a single character
            // consume the c
            (*formatptr)++;

            char c = (char) va_arg((*parameters), int /* char promotes to int?*/);
            buff[(*buffptr)++] = c;
            break;
        case 's': 
            // print a string
            // consume the 's'
            (*formatptr)++;
            printstring(buff, buffsize, buffptr, parameters);
            break;
        case 'i':
            // do integers
            // consume the 'i'
            (*formatptr)++;
            printInt(buff, buffsize, buffptr, format, formatptr, parameters);
            break;
        case 'u': 
            // do unsigned integers
            // consume the 'u'
            (*formatptr)++;
            printUInt(buff, buffsize, buffptr, format, formatptr, parameters);
            break;
        case 'x':
            // do lower case hex
            // consume the 'x'
            (*formatptr)++;
            printHexLower(buff, buffsize, buffptr, format, formatptr, parameters);
            break;
        case 'X': 
            // do upper case hex
            // consume the 'X'
            (*formatptr)++;
            printHexUpper(buff, buffsize, buffptr, format, formatptr, parameters);
            break;
        default:
            // undefined behavior, good luck
            // consume the spurious char
            (*formatptr)++;
            
        break;
    }
}

// internal function
int snprintfint(char* restrict buff, size_t buffsize, const char* restrict format, va_list* parameters) {
    // set up my counters
    size_t buffptr = 0;        // pointer to the buffer
    int formatptr = 0;

    // iterate through the format
    while ((format[formatptr] != '\0') && (buffptr < buffsize)) {

        switch (format[formatptr]) {
            case '%':
                // we have something special
                // skip the '%' in the format
                formatptr++;
                formattedText(buff, buffsize, &buffptr, format, &formatptr, parameters);
                break;
            default:
                // a standard move of a char
                buff[buffptr++] = format[formatptr++];
                break;
        }
        // next char in format
    }
    buff[buffptr] = '\0';
    return (int) buffptr;    
}

int snprintf(char* restrict buff, size_t buffsize, const char* restrict format, ...) {
    if (!buff) return -1;     // no buffer
    if (!format) return -2;   // no format
    if (!buffsize) return -3; // 0 length buffer

    // set up variable argument variables
    va_list parameters;
    va_start(parameters, format);

    return snprintfint(buff, buffsize, format, &parameters);
}