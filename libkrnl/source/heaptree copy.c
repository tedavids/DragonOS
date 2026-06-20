// self balancing tree that keeps track of the address given out by the heap
#include <paging.h>
#include <stdlib.h>
#include <stdio.h>

#include <heaptree.h>

// number of nodes in a 4k page
const uint16_t freelistNodesPerPage = 4096 / sizeof(struct heapTreeNode);

// free list info
struct heapTreeNode *freelist = nullptr;
uint16_t freelistcount = 0;

struct heapTreeNode *root = nullptr;

// get stats
uint16_t getHeapDepth(struct heapTreeNode *root) {
    return root->height;
}

uint16_t getFreelistSize() {
    return freelistcount;
}

// add free nodes to the freelist

bool addFreeHeapNodes() {
    // make sure free list is empty
    if (freelist) return false;
    if (freelistcount > 0) return false;

    // get a new page
    uint32_t freepage = (uint32_t) mapKernelPage(0, nullptr);

    if (!freepage) return false; // out of memory

    for (uint32_t i = 0; i < freelistNodesPerPage; i++) {
        struct heapTreeNode *node = (struct heapTreeNode *) (freepage + (i * sizeof(struct heapTreeNode)));
        // we will use the left child for the free list
        // we use i + 1 because that would be the address of the next node
        if ((i + 1) *sizeof(struct heapTreeNode) < 4096) {
            node->leftChild = (struct heapTreeNode *) ((uint32_t)(node) + sizeof(struct heapTreeNode));
        } else {
            node->leftChild = nullptr;
        }

        node->rightChild = nullptr;
        node->address = 0;
        node->height = 0;
    }

    freelist = (struct heapTreeNode *) freepage;
    freelistcount += freelistNodesPerPage;

    return true;
}

struct heapTreeNode *getNodeFromFreeList() {
    if (!freelist) addFreeHeapNodes();
    // check for out of memory
    if (!freelist) {
        printf("Out of memory in getNodeFromFreeList\n\r");
        abort();
    }
    struct heapTreeNode *node = freelist;

    // freelist now start at the leftChils
    freelist = node->leftChild;
    // clear everything
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->address = 0;
    node->memstack = 0;
    node->height = 0;

    // update stats
    freelistcount--;

    return node;
}

bool putNodeInFreeList(struct heapTreeNode *node) {
    // clear our node
    node->address = 0;
    node->memstack = 0;
    node->height = 0;
    node->rightChild = nullptr;
    node->leftChild = nullptr;

    // if free list is empty, then we just add this one
    if (!freelist) {
        freelistcount = 1;
        freelist = node;
        return true;
    }

    // put this at the beginning of the list
    node->leftChild = freelist->leftChild->leftChild;
    freelist->leftChild = node;

    freelistcount++;

    return true;
}

uint16_t height(struct heapTreeNode* node) {
    // nullptr node is 0 height
    if (!node) return 0;
    
    return node->height;
}

uint16_t max(uint16_t a, uint16_t b) {
    return (a > b) ? a : b;
}

struct heapTreeNode* newNode(uint32_t address, unsigned char memstack) {
    struct heapTreeNode* node = (struct heapTreeNode*) getNodeFromFreeList();

    if (!node) return nullptr; // out of memory

    node->address = address;
    node->memstack = memstack;
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->height = 1;
    return (node);
}

struct heapTreeNode* rotateRight(struct heapTreeNode* node) {
    struct heapTreeNode* left = node->leftChild;
    struct heapTreeNode* right = left->rightChild;

    left->rightChild = node;
    left->height = max(height(left->leftChild), height(left->rightChild)) + 1;

    node->leftChild = right;
    node->height = max(height(node->leftChild), height(node->rightChild)) + 1;

    return left;
}

struct heapTreeNode* rotateLeft(struct heapTreeNode* node) {
    struct heapTreeNode* right = node->rightChild;
    struct heapTreeNode* left = right->leftChild;
    right->leftChild = node;
    node->rightChild = left;
    node->height = max(height(node->leftChild), height(node->rightChild)) + 1;
    right->height = max(height(right->leftChild), height(right->rightChild)) + 1;
    return right;
}

int getBalance(struct heapTreeNode* node) {
    // a null node counts as zero
    if (!node) return 0;

    return height(node->leftChild) - height(node->rightChild);
}


struct heapTreeNode* insertNodeInternal(struct heapTreeNode* node, uint32_t address, unsigned char memstack) {
    if (!node) return (newNode(address, memstack));

    if (address < node->address) {
        node->leftChild = insertNodeInternal(node->leftChild, address, memstack);
    }
    else {
        if (address > node->address) {
            node->rightChild = insertNodeInternal(node->rightChild, address, memstack);
        }
        else {
            return node;
        }
    }

    node->height = 1 + max(height(node->leftChild), height(node->rightChild));

    int balance = getBalance(node);

    if (balance > 1) {
        if (address < node->leftChild->address) {
            return rotateRight(node);
        } else {
            if (address > node->leftChild->address) {
                node->leftChild = rotateLeft(node->leftChild);
                return rotateRight(node);
            }
        }
    }

    if (balance < -1) {
        if (address > node->rightChild->address) {
            return rotateLeft(node);
        }
        else {
            if (address < node->rightChild->address) {
                node->rightChild = rotateRight(node->rightChild);
                return rotateLeft(node);
            }
        }
    }
 
    return node;
}

struct heapTreeNode* insertAllocTreeNode(uint32_t address, unsigned char memstack) {
    return insertNodeInternal(root, address, memstack);
}

struct heapTreeNode* minValueNode(struct heapTreeNode* node) {
    struct heapTreeNode* current = node;

    while (current->leftChild)
        current = current->leftChild;

    return current;
}

struct heapTreeNode *deleteNodeInternal(struct heapTreeNode *root, uint32_t address) {
    if (!root) return root;

    if (address < root->address) {
        root->leftChild = deleteNodeInternal(root->leftChild, address);
    } else {
        if (address > root->address) {
            root->rightChild = deleteNodeInternal(root->rightChild, address);
        } else {
            if ((!root->leftChild) || ((!root->rightChild))) {
                struct heapTreeNode *temp = root->leftChild ? root->leftChild : root->rightChild;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }

                putNodeInFreeList(temp);

            } else {
                struct heapTreeNode *temp = minValueNode(root->rightChild);
                root->address = temp->address;
                root->rightChild = deleteNodeInternal(root->rightChild, temp->address);
            }
        }
        if (!root) return root;

        root->height = 1 + max(height(root->leftChild), height(root->rightChild));
        int balance = getBalance(root);

        if ((balance > 1) && (getBalance(root->leftChild) >= 0)) 
            return rotateRight(root);

        if ((balance > 1) && (getBalance(root->leftChild) < 0)) {
            root->leftChild = rotateLeft(root->leftChild);
            return rotateRight(root);
        }

        if ((balance < -1) && (getBalance(root->rightChild) < 0))
            return rotateLeft(root);
        if ((balance < -1) && (getBalance(root->rightChild) > 0)) {
            root->rightChild = rotateRight(root->rightChild);
            return rotateLeft(root);
        }
    }

    return root;
}

struct heapTreeNode *deleteAllocTreeNode(uint32_t address) {
    return deleteNodeInternal(root, address);
}

struct heapTreeNode *getNodeInternal(struct heapTreeNode *root, uint32_t address) {
    // empty tree?
    if (!root) return nullptr;

    // found it
    if (root->address == address) return root;

    if (root->address > address) {
        return getNodeInternal(root->leftChild, address);
    }

    return getNodeInternal(root->rightChild, address);
}

struct heapTreeNode *getAllocTreeNode(uint32_t address) {
    return getNodeInternal(root, address);
}

bool initHeapTree() {
    bool rtncde = addFreeHeapNodes();

    return rtncde;
}