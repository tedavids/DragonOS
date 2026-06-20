// heap functions

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <paging.h>
#include <heaptree.h>
#include <slaballoc.h>
#include <stdio.h>

#include <heap.h>

// enums
enum eStack {
    eStack4 = 0,
    eStack8 = 1,
    eStack16 = 2,
    eStack32 = 3,
    eStack64  = 4,
    eStack128 = 5,
    eStack256 = 6,
    eStack512 = 7,
    eStack1024 = 8,
    eStack2048 = 9,
    eStack4K = 10,
    eStackEnd = 256
};

static struct heapTreeNode *heapTreeRoot = nullptr;

// stats
uint32_t getHeapSize() {
    return getHeapTreeNumAlloc();
}

// External functions

void *kmalloc(size_t size) {
    void * ptr = nullptr;
    enum eStack stack = eStackEnd;

    // send request to proper stack
    if (size <= 4) {
        ptr = alloc4(size);
        stack = eStack4;
    } else if (size <= 8) {
        ptr = alloc8(size);
        stack = eStack8;
    } else if (size <= 16) {
        ptr = alloc16(size);
        stack = eStack16;
    } else if (size <= 32) {
        ptr = alloc32(size);
        stack = eStack32;
    } else if (size <= 64) {
        ptr = alloc64(size);
        stack = eStack64;
    } else if (size <= 128) {
        ptr = alloc128(size);
        stack = eStack128;
    } else if (size <= 256) {
        ptr = alloc256(size);
        stack = eStack256;
    } else if (size <= 512) {
        ptr = alloc512(size);
        stack = eStack512;
    } else if (size <= 1024) {
        ptr = alloc1024(size);
        stack = eStack1024;
    } else if (size <= 2048) {
        ptr = alloc2048(size);
        stack = eStack2048;
    }

    // if we got a poniter 
    if (ptr) {
        // make sure we don't have an internal eror
        if (getAllocTreeNode((uint32_t)ptr)){
            printf("Internal allocation failure, duplicate allocation\n\r");
            abort();
        }
        insertAllocTreeNode((uint32_t) ptr, (unsigned char) stack);
    }

    return ptr;
}

void kfree(void * addr) {
    // make sure they asked for something to be freed
    if (!addr) return;

    // was it used in an allocation
    struct heapTreeNode *node = getAllocTreeNode((uint32_t) addr);
    if (!node) {
        // we didn't allocate this address
        printf("Possible double free attempt.  Address: 0x%Xl\n\r", addr);
        return;
    }

    bool freesuccessful = false;
    // valid address
    // free it then delete the allocation
    switch ((enum eStack) node->memstack) {
        case eStack4:   freesuccessful = free4((uint32_t)addr);
                        break;
        case eStack8:   freesuccessful = free8((uint32_t) addr);
                        break;
        case eStack16:  freesuccessful = free16((uint32_t) addr);
                        break;
        case eStack32:  freesuccessful = free32((uint32_t) addr);
                        break;
        case eStack64:  freesuccessful = free64((uint32_t) addr);
                        break;
        case eStack128: freesuccessful = free128((uint32_t) addr);
                        break;
       case eStack256: freesuccessful = free256((uint32_t) addr);
                        break;
       case eStack512: freesuccessful = free512((uint32_t) addr);
                        break;
       case eStack1024: freesuccessful = free1024((uint32_t) addr);
                        break;
       case eStack2048: freesuccessful = free2048((uint32_t) addr);
                        break;
        default: break;
    }

    if (freesuccessful) deleteAllocTreeNode((uint32_t) addr);
    
}
// init heap

bool initHeap() {

    heapTreeRoot = heapTreeRoot;

    bool rtncde = initHeapTree();
    rtncde &= initSlabAlloc();

    return rtncde;
}