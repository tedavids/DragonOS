// Binary Tree keeping track of addresses passed out by the heap

#ifndef _HEAPTREE_H
#define _HEAPTREE_H

#include <stdint.h>
#include <stdbool.h>

struct heapTreeNode {
    struct heapTreeNode *leftChild;
    struct heapTreeNode *rightChild;
    uint32_t            address;        // addess we are keeing track of
    unsigned char       memstack;       // the memory stack it came from
    uint16_t            height;
};


// get heap depth
extern uint16_t getHeapTreeDepth(struct heapTreeNode *root);
// get number of nodes in the freelist
extern uint16_t getHeapTreeFreelistSize();
// get number of nodes in the tree
extern uint32_t getHeapTreeNumAlloc();

// get a node
extern struct heapTreeNode *getAllocTreeNode(uint32_t address);
// delete a node
void deleteAllocTreeNode(uint32_t address);
// insert a node
void insertAllocTreeNode(uint32_t address, unsigned char memstack);

// initialize the heap tree
extern bool initHeapTree();

#endif