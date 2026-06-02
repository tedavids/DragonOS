// heap.h heap functions and types

#ifndef _HEAP_H
#define _HEAP_H

extern void initHeap();
extern void* krealloc(void* ptr, size_t newsize); // must be the address passed by malloc
extern void* kcalloc(size_t elements, size_t elemsize);

#endif