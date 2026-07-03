// store allocations of a page or more

#ifndef _PAGEALLOCTREE_H
#define _PAGEALLOCTREE_H

// get heap depth
extern uint16_t getPageTreeDepth();
// get number of nodes in the freelist
extern uint16_t getPageTreeFreelistSize();
// get number of nodes in the tree
extern uint32_t getPageTreeNumAlloc();

// allocate pages
void *allocPage(size_t size);
// free a page allocation
bool freePage(uint32_t ptr);

// initialize the heap tree
extern bool initPageTree();


#endif
