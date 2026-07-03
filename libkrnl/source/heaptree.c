// self balancing tree that keeps track of the address given out by the heap
#include <paging.h>
#include <stdlib.h>
#include <stdio.h>

#include <heaptree.h>

// number of nodes
uint32_t   htNumNodes = 0;

// number of nodes in a 4k page
const uint16_t htFreeListNodesPerPage = 4096 / sizeof(struct heapTreeNode);

// free list info
struct heapTreeNode *htFreeList = nullptr;
uint16_t htFreeListCount = 0;

static struct heapTreeNode *htRoot = nullptr;

// get stats
uint16_t getHeapTreeDepth(struct heapTreeNode *root) {
    return root->height;
}

uint16_t getHeapTreehtFreeListSize() {
    return htFreeListCount;
}

uint32_t getHeapTreeNumAlloc() {
    return htNumNodes;
}

// add free nodes to the htFreeList

bool htAddFreeHeapNodes() {
    // make sure free list is empty
    if (htFreeList) return false;
    if (htFreeListCount > 0) return false;

    // get a new page
    struct heapTreeNode *freepage = (struct heapTreeNode *) mapKernelPage(0, nullptr);

    if (!freepage) return false; // out of memory

    struct heapTreeNode *temp = freepage;

    for (uint16_t i = 0; i < htFreeListNodesPerPage; i++) {
        // set address of the next structure
        if (i == (htFreeListNodesPerPage - 1)) {
            temp->leftChild = nullptr;
        } else {
            temp->leftChild = temp + 1;
        }

        temp->rightChild = nullptr;
        temp->address = 0;
        temp->height = 0;

        // on to the next 
        temp++;
    }

    htFreeList = freepage;
    htFreeListCount += htFreeListNodesPerPage;

    return true;
}

struct heapTreeNode *htGetNodeFromhtFreeList() {
    if (!htFreeList) htAddFreeHeapNodes();
    // check for out of memory
    if (!htFreeList) {
        printf("Out of memory in getNodeHTFromhtFreeList\n\r");
        abort();
    }
    struct heapTreeNode *node = htFreeList;

    // htFreeList now start at the leftChils
    htFreeList = node->leftChild;
    // clear everything
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->address = 0;
    node->memstack = 0;
    node->height = 0;

    // update stats
    htFreeListCount--;

    return node;
}

bool htPutNodeInFreeList(struct heapTreeNode *node) {
    // clear our node
    node->address = 0;
    node->memstack = 0;
    node->height = 0;
    node->rightChild = nullptr;
    node->leftChild = nullptr;

    // decrement how many we have
   htNumNodes--;

    // if free list is empty, then we just add this one
    if (!htFreeList) {
        htFreeListCount = 1;
        htFreeList = node;
        return true;
    }

    // put this at the beginning of the list
    node->leftChild = htFreeList->leftChild->leftChild;
    htFreeList->leftChild = node;

    htFreeListCount++;

    return true;
}

uint16_t htHeight(struct heapTreeNode* node) {
    // nullptr node is 0 height
    if (!node) return 0;
    
    return node->height;
}

uint16_t htMax(uint16_t a, uint16_t b) {
    return (a > b) ? a : b;
}

struct heapTreeNode* htNewNode(uint32_t address, unsigned char memstack) {
    struct heapTreeNode* node = (struct heapTreeNode*) htGetNodeFromhtFreeList();

    if (!node) return nullptr; // out of memory

    node->address = address;
    node->memstack = memstack;
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->height = 1;
   htNumNodes++;
    return (node);
}

struct heapTreeNode* htRotateRight(struct heapTreeNode* node) {
    struct heapTreeNode* left = node->leftChild;
    struct heapTreeNode* right = left->rightChild;

    left->rightChild = node;
    left->height = htMax(htHeight(left->leftChild), htHeight(left->rightChild)) + 1;

    node->leftChild = right;
    node->height = htMax(htHeight(node->leftChild), htHeight(node->rightChild)) + 1;

    return left;
}

struct heapTreeNode* htRotateLeft(struct heapTreeNode* node) {
    struct heapTreeNode* right = node->rightChild;
    struct heapTreeNode* left = right->leftChild;
    right->leftChild = node;
    node->rightChild = left;
    node->height = htMax(htHeight(node->leftChild), htHeight(node->rightChild)) + 1;
    right->height = htMax(htHeight(right->leftChild), htHeight(right->rightChild)) + 1;
    return right;
}

int htGetBalance(struct heapTreeNode* node) {
    // a null node counts as zero
    if (!node) return 0;

    return htHeight(node->leftChild) - htHeight(node->rightChild);
}


struct heapTreeNode* htInsertNodeInternal(struct heapTreeNode* node, uint32_t address, unsigned char memstack) {
    if (!node) return (htNewNode(address, memstack));

    if (address < node->address) {
        node->leftChild = htInsertNodeInternal(node->leftChild, address, memstack);
    }
    else {
        if (address > node->address) {
            node->rightChild = htInsertNodeInternal(node->rightChild, address, memstack);
        }
        else {
            return (node);
        }
    }

    node->height = 1 + htMax(htHeight(node->leftChild), htHeight(node->rightChild));

    int balance = htGetBalance(node);

    if (balance > 1) {
        if (address < node->leftChild->address) {
            return htRotateRight(node);
        } else {
            if (address > node->leftChild->address) {
                node->leftChild = htRotateLeft(node->leftChild);
                return htRotateRight(node);
            }
        }
    }

    if (balance < -1) {
        if (address > node->rightChild->address) {
            return htRotateLeft(node);
        }
        else {
            if (address < node->rightChild->address) {
                node->rightChild = htRotateRight(node->rightChild);
                return htRotateLeft(node);
            }
        }
    }
 
    return node;
}

void insertAllocTreeNode(uint32_t address, unsigned char memstack) {

    htRoot = htInsertNodeInternal(htRoot,address, memstack);
}

struct heapTreeNode* htMinValueNode(struct heapTreeNode* node) {
    struct heapTreeNode* current = node;

    while (current->leftChild)
        current = current->leftChild;

    return current;
}

struct heapTreeNode *htDeleteNodeInternal(struct heapTreeNode *root, uint32_t address) {
    if (!root) return root;

    if (address < root->address) {
        root->leftChild = htDeleteNodeInternal(root->leftChild, address);
    } else {
        if (address > root->address) {
            root->rightChild = htDeleteNodeInternal(root->rightChild, address);
        } else {
            if ((!root->leftChild) || ((!root->rightChild))) {
                struct heapTreeNode *temp = root->leftChild ? root->leftChild : root->rightChild;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }

                htPutNodeInFreeList(temp);

            } else {
                struct heapTreeNode *temp = htMinValueNode(root->rightChild);
                root->address = temp->address;
                root->rightChild = htDeleteNodeInternal(root->rightChild, temp->address);
            }
        }
        if (!root) return root;

        root->height = 1 + htMax(htHeight(root->leftChild), htHeight(root->rightChild));
        int balance = htGetBalance(root);

        if ((balance > 1) && (htGetBalance(root->leftChild) >= 0)) 
            return htRotateRight(root);

        if ((balance > 1) && (htGetBalance(root->leftChild) < 0)) {
            root->leftChild = htRotateLeft(root->leftChild);
            return htRotateRight(root);
        }

        if ((balance < -1) && (htGetBalance(root->rightChild) < 0))
            return htRotateLeft(root);
        if ((balance < -1) && (htGetBalance(root->rightChild) > 0)) {
            root->rightChild = htRotateRight(root->rightChild);
            return htRotateLeft(root);
        }
    }

    return root;
}

void deleteAllocTreeNode(uint32_t address) {
    htRoot =  htDeleteNodeInternal(htRoot, address);
}

struct heapTreeNode *htGetNodeInternal(struct heapTreeNode *root, uint32_t address) {
    // empty tree?
    if (!root) return nullptr;

    // found it
    if (root->address == address) return root;

    if (root->address > address) {
        return htGetNodeInternal(root->leftChild, address);
    }

    return htGetNodeInternal(root->rightChild, address);
}

struct heapTreeNode *getAllocTreeNode(uint32_t address) {
    return htGetNodeInternal(htRoot, address);
}

bool initHeapTree() {
    bool rtncde = htAddFreeHeapNodes();

    return rtncde;
}