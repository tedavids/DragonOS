// functions for bit arrays

#include <bitarray.h>

bool getBit(const unsigned char *array, size_t numbits, size_t index, bool *value) {
   // check parameters
    if (!array || (index > numbits) || (!value)) return false;

    const size_t byteIndex = index / (sizeof(unsigned char) * 8);
    const size_t bitIndex = index % (sizeof(unsigned char) * 8); 

    *value = (array[byteIndex] & (1u << bitIndex)) != 0;

    return true;
}

bool setBit(unsigned char *array, size_t numbits, size_t index, bool *oldValue) {
   // check parameters
    if (!array || (index > numbits)) return false;

    const size_t byteIndex = index / (sizeof(unsigned char) * 8);
    const size_t bitIndex = index % (sizeof(unsigned char) * 8);
    const unsigned char mask = (unsigned char) (1u << bitIndex);

    if (oldValue) {
        *oldValue = (array[byteIndex] & mask) != 0;
    }

    array[byteIndex] |= mask;

    return true;
}

bool clearBit(unsigned char *array, size_t numbits, size_t index, bool *oldValue) {
    // check parameters
    if (!array || (index > numbits)) return false;

    const size_t byteIndex = index / (sizeof(unsigned char) * 8);
    const size_t bitIndex = index % (sizeof(unsigned char) * 8);
    const unsigned char mask = (unsigned char) (1u << bitIndex);

    if (oldValue) {
        *oldValue = (array[byteIndex] & mask) != 0;
    }

    array[byteIndex] &= (unsigned char)~mask;

    return true;
}

bool toggleBit(unsigned char *array, size_t numbits, size_t index, bool *value) {
    // validate parameters
    if (!array || (index > numbits) || (!value)) return false;

    value = false;
    if (getBit(array,numbits,index,value)) {
        if (value) {
            return clearBit(array, numbits, index, nullptr);
        } else {
            return setBit(array, numbits, index, nullptr);
        }
    }

    return false;
}