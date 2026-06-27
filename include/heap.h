// heap.h heap functions and types

#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>


// allocation functions
// allocate space for the kernel

// Parameters:  size -- number of bytes to allocate (max 0x8000, 8 pages)

// return:      void pointer to the memory allocated
extern void *kmalloc(size_t size);

// allocate initialized space for the kernel

// Parameters:  size -- number of bytes to allocate (max 0x8000, 8 pages)

// returns:     void pointer to the memory allocated

extern void *kcalloc(size_t size);

// Free space allocated by kmalloc/kcalloc
extern void kfree(void * addr);

// stat functions
// get the number of allocations for the heap

// Returns: The total number of allocations in the heap
extern uint32_t getHeapSize();

// initialize the heap

// Returns: true if the heap initializes successfully

extern bool initHeap();



#endif