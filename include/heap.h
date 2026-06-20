// heap.h heap functions and types

#ifndef _HEAP_H
#define _HEAP_H

#include <stdint.h>


// allocation functions
extern void *kmalloc(size_t size);
extern void *kcalloc(size_t size);
extern void kfree(void * addr);

// stat functions
extern uint32_t getHeapSize();

// initialize the heap

extern bool initHeap();



#endif