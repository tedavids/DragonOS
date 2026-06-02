// toHex.c  Conversions for numbers to hex strings

#include <string.h>
#include <stdint.h>
#include <stdio.h>

static const char hexcharsupper[] = "0123456789ABCDEF";
static const char hexcharslower[] = "0123456789abcdef";

// trim the hex code (minimum 1 hex chars)

char* trimHex(char* restrict s) {
    // internal error it should never happen, but we can't add to a nullterminated buffer
    if (strlen(s) == 0) return s;

    s = trimc('0',s);

    // completely blank add a 0
    if (*s == '\0') {
        s[0] = '0';
        s[1] = '\0';
    }

    return s;
}
// lower case hex codes 

char nibbleToHexCharlower(unsigned char nibble) {
    // set high order bits to 0, we just want to low order ones
    nibble &= 0x0F;

    return hexcharslower[(unsigned char)nibble];
}

// these functions take a value and convert it to hex
// if the buffer is too short it returns nullptr
char* toHex8lower(unsigned char byte, char* buffer, size_t size) {

    // check buffer size (must be at least 3)
    if (size < 3) return nullptr;

    // buffer is big enough
    unsigned char lownibble = byte & 0x0F;
    unsigned char highnibble = byte >> 4;

    // build buffer
    buffer[0] = nibbleToHexCharlower(highnibble);
    buffer[1] = nibbleToHexCharlower(lownibble);
    buffer[2] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} // char* toHex(unsigned char byte, char* buffer, size_t size)

char* toHex16lower(uint16_t val, char* buffer, size_t size) {

    // check buffer size (must be at least 5)
    if (size < 5) return nullptr;

    for (int i = 3; i >= 0; i--) {
        buffer[i] = nibbleToHexCharlower((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[4] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} // char* toHex16(uint16_t val, char* buffer, size_t size)

char* toHex32lower(uint32_t val, char * buffer, size_t size) {

        // check buffer size (must be at least 5)
    if (size < 9) return nullptr;

    for (int i = 7; i >= 0; i--) {
        buffer[i] = nibbleToHexCharlower((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[8] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} // char* toHex32(uint32_t val, char* buffer, size_t size)


char* toHex64lower(uint64_t val, char * buffer, size_t size) {

        // check buffer size (must be at least 5)
    if (size < 17) return nullptr;

    for (int i = 15; i >= 0; i--) {
        buffer[i] = nibbleToHexCharlower((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[17] = '\0';

       // trim leading zeros
    trimHex(buffer);

    return buffer;
} 

// upper case hex codes 

char nibbleToHexCharupper(unsigned char nibble) {
    // set high order bits to 0, we just want to low order ones
    nibble &= 0x0F;

    return hexcharsupper[(unsigned char)nibble];
}

// these functions take a value and convert it to hex
// if the buffer is too short it returns nullptr
char* toHex8upper(unsigned char byte, char* buffer, size_t size) {

    // check buffer size (must be at least 3)
    if (size < 3) return nullptr;

    // buffer is big enough
    unsigned char lownibble = byte & 0x0F;
    unsigned char highnibble = byte >> 4;

    // build buffer
    buffer[0] = nibbleToHexCharupper(highnibble);
    buffer[1] = nibbleToHexCharupper(lownibble);
    buffer[2] = '\0';

    // trim leading zeros
    trimHex(buffer);

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} 

char* toHex16upper(uint16_t val, char* buffer, size_t size) {

    // check buffer size (must be at least 5)
    if (size < 5) return nullptr;

    for (int i = 3; i >= 0; i--) {
        buffer[i] = nibbleToHexCharupper((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[4] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} 

char* toHex32upper(uint32_t val, char * buffer, size_t size) {

        // check buffer size (must be at least 5)
    if (size < 9) return nullptr;

    for (int i = 7; i >= 0; i--) {
        buffer[i] = nibbleToHexCharupper((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[8] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} 


char* toHex64upper(uint64_t val, char * buffer, size_t size) {

        // check buffer size (must be at least 5)
    if (size < 17) return nullptr;

    for (int i = 15; i >= 0; i--) {
        buffer[i] = nibbleToHexCharupper((val & 0x000F));
        val >>= 4;
    }

    // trailing null
    buffer[17] = '\0';

   // trim leading zeros
    trimHex(buffer);

    return buffer;
} 