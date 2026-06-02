// toBase10.c convert integers to base 10 (decimal)
#include <stdint.h>

#include <string.h>

char* toBase10ib(int8_t byte, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 5) return nullptr; // buffer to small

    char wrkbuf[22];
    int64_t wrkval = byte;

    // check if it worked
    if (!toBase10iq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10iw(int16_t val, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 7) return nullptr; // buffer to small

    char wrkbuf[22];
    int64_t wrkval = val;

    // check if it worked
    if (!toBase10iq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10il(int32_t val, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 12) return nullptr; // buffer to small

    char wrkbuf[22];
    int64_t wrkval = val;

    // check if it worked
    if (!toBase10iq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10iq(int64_t val, char* buffer, size_t size) {
    if (!buffer) return buffer; // no buffer
    if (size < 22) return nullptr; // to short for largest value

    char revbuf[22];     // has digits in reverse

    bool isneg = val < 0;  // save negative to put at beginning
    if (isneg) {
        val = -val;
    }

    revbuf[0] = '\0';  //will be the trailing nullptr

    // convert to digits
    int revidx = 1;

    if (val == 0) {
        // handle zero
        revbuf[revidx++] = '0';
    } else {
        while (val > 0) {
            revbuf[revidx++] = '0' + (char) (val % 10);
            val /= 10;
        }
    }

    // put in the minus if necessary
    if (isneg) {
        revbuf[revidx++] = '-';
    }

    // make output buffer in correct order

    int bufidx = 0;

    for (int i = (revidx -1); i >= 0; i--)
        buffer[bufidx++] = revbuf[i];

    return buffer;

}

char* toBase10ub(uint8_t byte, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 5) return nullptr; // buffer to small

    char wrkbuf[22];
    uint64_t wrkval = byte;

    // check if it worked
    if (!toBase10uq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10uw(uint16_t val, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 7) return nullptr; // buffer to small

    char wrkbuf[22];
    uint64_t wrkval = val;

    // check if it worked
    if (!toBase10uq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10ul(uint32_t val, char* buffer, size_t size) {
    if (!buffer) return nullptr; // no buffer
    if (size < 13) return nullptr; // buffer to small

    char wrkbuf[22];
    uint64_t wrkval = val;

    // check if it worked
    if (!toBase10uq(wrkval, wrkbuf, sizeof(wrkbuf))) return nullptr;

    // copy the work buffer to the output buffer
    auto len = strlen(wrkbuf);

    if (len > size) len = size-1;

    // copy the buffer
    for (size_t i = 0; i < len; i++)
        buffer[i] = wrkbuf[i];
    
    // trailing null
    buffer[len] = '\0';

    return buffer;
}

char* toBase10uq(uint64_t val, char* buffer, size_t size) {
    if (!buffer) return buffer; // no buffer
    if (size < 22) return nullptr; // to short for largest value

    char revbuf[22];     // has digits in reverse

    revbuf[0] = '\0';  //will be the trailing nullptr

    // convert to digits
    int revidx = 1;

    if (val == 0) {
        // handle zero
        revbuf[revidx++] = '0';
    } else {
        while (val > 0) {
            revbuf[revidx++] = '0' + (char) (val % 10);
            val /= 10;
        }
    }

    // make output buffer in correct order

    int bufidx = 0;

    for (int i = (revidx - 1); i >= 0; i--)
        buffer[bufidx++] = revbuf[i];

    return buffer;

}

