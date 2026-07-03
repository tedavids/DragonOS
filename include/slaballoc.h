// this is my slab allocation functions

#ifndef _SLABALLOC_H
#define _SLABALLOC_H

#include <stdint.h>
#include <stdbool.h>


// structures

// I use this type of tructure for allocations less than a page

struct slabAlloc4_t {
    uint32_t            address;        // address of page in memory, if zero the page for the array
                                        //  is not yet allocated
    struct slabAlloc4_t *nextaddr;      // address next slabAlloc4_t info
    uint16_t            avail;          // number of addresses available in this block
    unsigned char       availmap[128];  // a bitmap array, to the 4 byte allocaion
};

struct slabAlloc8_t {
    uint32_t            address;        // address of page in memory, if zero the page for the array
    struct slabAlloc8_t *nextaddr;      // address next slabAlloc8_t info
    uint16_t            avail;          // number of addresses available in this block
    unsigned char       availmap[64];   // a bitmap array, to the 8 byte allocaion
};

struct slabAlloc16_t {
    uint32_t                address;        // address of page in memory
    struct slabAlloc16_t    *nextaddr;      // address next slabAlloc16_t info
    uint16_t                avail;          // number of addresses available in this block
    unsigned char           availmap[32];   // a bitmap array, to the 16 byte allocaion
};

struct slabAlloc32_t {
    uint32_t                address;        // address of page in memory
    struct slabAlloc32_t    *nextaddr;      // address next slabAlloc32_t info
    uint16_t                avail;          // number of addresses available in this block
    unsigned char           availmap[16];   // a bitmap array, to the 32 byte allocaion
};

struct slabAlloc64_t {
    uint32_t                address;        // address of page in memory
    struct slabAlloc64_t    *nextaddr;      // address next slabAlloc464t info
    uint16_t                avail;          // number of addresses available in this block
    unsigned char           availmap[8];    // a bitmap array, to the 64 byte allocaion
};

struct slabAlloc128_t {
    uint32_t                 address;        // address of page in memory
    struct slabAlloc128_t    *nextaddr;      // address next slabAlloc128_t info
    uint16_t                 avail;          // number of addresses available in this block
    unsigned char            availmap[4];    // a bitmap array, to the 128 byte allocaion
};

struct slabAlloc256_t {
    uint32_t                 address;        // address of page in memory
    struct slabAlloc256_t    *nextaddr;      // address next slabAlloc256_t info
    uint16_t                 avail;          // number of addresses available in this block
    unsigned char            availmap[2];    // a bitmap array, to the 256 byte allocaion
};

struct slabAlloc512_t {
    uint32_t                 address;        // address of page in memory
    struct slabAlloc512_t    *nextaddr;      // address next slabAlloc512_t info
    uint16_t                 avail;          // number of addresses available in this block
    unsigned char            availmap[1];    // a bitmap array, to the 512 byte allocaion
};

struct slabAlloc1024_t {
    uint32_t                 address;        // address of page in memory
    struct slabAlloc1024_t   *nextaddr;      // address next slabAlloc1024_t info
    uint16_t                 avail;          // number of addresses available in this block
    unsigned char            availmap[1];    // a bitmap array, to the 512 byte allocaion
};

struct slabAlloc2048_t {
    uint32_t                 address;        // address of page in memory
    struct slabAlloc2048_t   *nextaddr;      // address next slabAlloc2048_t info
    uint16_t                 avail;          // number of addresses available in this block
    unsigned char            availmap[1];    // a bitmap array, to the 2048 byte allocaion
};


// allocation functions
extern void *alloc4(size_t size);
extern void *alloc8(size_t size);
extern void *alloc16(size_t size);
extern void *alloc32(size_t size);
extern void *alloc64(size_t size);
extern void *alloc128(size_t size);
extern void *alloc256(size_t size);
extern void *alloc512(size_t size);
extern void *alloc1024(size_t size);
extern void *alloc2048(size_t size);

extern bool free4(uint32_t addr);
extern bool free8(uint32_t addr);
extern bool free16(uint32_t addr);
extern bool free32(uint32_t addr);
extern bool free64(uint32_t addr);
extern bool free128(uint32_t addr);
extern bool free256(uint32_t addr);
extern bool free512(uint32_t addr);
extern bool free1024(uint32_t addr);
extern bool free2048(uint32_t addr);

// external functions

// init the base structures
extern bool initSlabAlloc();

#endif