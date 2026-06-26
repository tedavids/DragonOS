// store heap allocations of a page or larget
#include <paging.h>
#include <stdlib.h>
#include <stdio.h>

#include <pagealloctree.h>

struct pageTreeNode {
    struct pageTreeNode *leftChild;
    struct pageTreeNode *rightChild;
    uint32_t            address;        // addess we are keeing track of
    unsigned char       numpages;       // the number of pages in the allocation
    uint16_t            height;
};

// number of nodes
uint32_t    numPTNodes = 0;

// number of nodes in a 4k page
const uint16_t freeListPTNodesPerPage = 4096 / sizeof(struct pageTreeNode);

// free list info
struct pageTreeNode *ptFreeList = nullptr;
uint16_t ptFreeListCount = 0;

static struct pageTreeNode *root = nullptr;

// get stats
uint16_t getPageTreeDepth() {
    return root->height;
}

uint16_t getPageTreeFreelistSize() {
    return ptFreeListCount;
}

uint32_t getPageTreeNumAlloc() {
    return numPTNodes;
}

// add free nodes to the freelist

bool addFreePTNodes() {
    // make sure free list is empty
    if (ptFreeList) return false;
    if (ptFreeListCount > 0) return false;

    // get a new page
    struct pageTreeNode *freepage = (struct pageTreeNode *) mapKernelPage(0, nullptr);

    if (!freepage) return false; // out of memory

    struct pageTreeNode *temp = freepage;

    for (uint16_t i = 0; i < freeListPTNodesPerPage; i++) {
        // set address of the next structure
        if (i == (freeListPTNodesPerPage - 1)) {
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

    ptFreeList = freepage;
    ptFreeListCount += freeListPTNodesPerPage;

    return true;
}

struct pageTreeNode *getNodeFromFreeList() {
    if (!ptFreeList) addFreePTNodes();
    // check for out of memory
    if (!ptFreeList) {
        printf("Out of memory in getNodeFromFreeList\n\r");
        abort();
    }
    struct pageTreeNode *node = ptFreeList;

    // ptFreeList now start at the leftChils
    ptFreeList = node->leftChild;
    // clear everything
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->address = 0;
    node->numpages = 0;
    node->height = 0;

    // update stats
    ptFreeListCount--;

    return node;
}

bool putNodeInFreeList(struct pageTreeNode *node) {
    // clear our node
    node->address = 0;
    node->numpages = 0;
    node->height = 0;
    node->rightChild = nullptr;
    node->leftChild = nullptr;

    // decrement how many we have
    numPTNodes--;

    // if free list is empty, then we just add this one
    if (!ptFreeList) {
        ptFreeListCount = 1;
        ptFreeList = node;
        return true;
    }

    // put this at the beginning of the list
    node->leftChild = ptFreeList->leftChild->leftChild;
    ptFreeList->leftChild = node;

    ptFreeListCount++;

    return true;
}

uint16_t ptHeight(struct pageTreeNode* node) {
    // nullptr node is 0 height
    if (!node) return 0;
    
    return node->height;
}

uint16_t ptMax(uint16_t a, uint16_t b) {
    return (a > b) ? a : b;
}

struct pageTreeNode* nePTNode(uint32_t address, unsigned char numpages) {
    struct pageTreeNode* node = (struct pageTreeNode*) getNodeFromFreeList();

    if (!node) return nullptr; // out of memory

    node->address = address;
    node->numpages = numpages;
    node->leftChild = nullptr;
    node->rightChild = nullptr;
    node->height = 1;
    numPTNodes++;
    return (node);
}

struct pageTreeNode* rotatePTRight(struct pageTreeNode* node) {
    struct pageTreeNode* left = node->leftChild;
    struct pageTreeNode* right = left->rightChild;

    left->rightChild = node;
    left->height = ptMax(ptHeight(left->leftChild), ptHeight(left->rightChild)) + 1;

    node->leftChild = right;
    node->height = ptMax(ptHeight(node->leftChild), ptHeight(node->rightChild)) + 1;

    return left;
}

struct pageTreeNode* rotatePTLeft(struct pageTreeNode* node) {
    struct pageTreeNode* right = node->rightChild;
    struct pageTreeNode* left = right->leftChild;
    right->leftChild = node;
    node->rightChild = left;
    node->height = ptMax(ptHeight(node->leftChild), ptHeight(node->rightChild)) + 1;
    right->height = ptMax(ptHeight(right->leftChild), ptHeight(right->rightChild)) + 1;
    return right;
}

int getPTBalance(struct pageTreeNode* node) {
    // a null node counts as zero
    if (!node) return 0;

    return ptHeight(node->leftChild) - ptHeight(node->rightChild);
}


struct pageTreeNode* insertPTNodeInternal(struct pageTreeNode* node, uint32_t address, unsigned char numpages) {
    if (!node) return (nePTNode(address, numpages));

    if (address < node->address) {
        node->leftChild = insertPTNodeInternal(node->leftChild, address, numpages);
    }
    else {
        if (address > node->address) {
            node->rightChild = insertPTNodeInternal(node->rightChild, address, numpages);
        }
        else {
            return (node);
        }
    }

    node->height = 1 + ptMax(ptHeight(node->leftChild), ptHeight(node->rightChild));

    int balance = getPTBalance(node);

    if (balance > 1) {
        if (address < node->leftChild->address) {
            return rotatePTRight(node);
        } else {
            if (address > node->leftChild->address) {
                node->leftChild = rotatePTLeft(node->leftChild);
                return rotatePTRight(node);
            }
        }
    }

    if (balance < -1) {
        if (address > node->rightChild->address) {
            return rotatePTLeft(node);
        }
        else {
            if (address < node->rightChild->address) {
                node->rightChild = rotatePTRight(node->rightChild);
                return rotatePTLeft(node);
            }
        }
    }
 
    return node;
}

void insertPTTreeNode(uint32_t address, unsigned char numpages) {

    root = insertPTNodeInternal(root,address, numpages);
}

struct pageTreeNode* minValuePTNode(struct pageTreeNode* node) {
    struct pageTreeNode* current = node;

    while (current->leftChild)
        current = current->leftChild;

    return current;
}

struct pageTreeNode *deletePTNodeInternal(struct pageTreeNode *root, uint32_t address) {
    if (!root) return root;

    if (address < root->address) {
        root->leftChild = deletePTNodeInternal(root->leftChild, address);
    } else {
        if (address > root->address) {
            root->rightChild = deletePTNodeInternal(root->rightChild, address);
        } else {
            if ((!root->leftChild) || ((!root->rightChild))) {
                struct pageTreeNode *temp = root->leftChild ? root->leftChild : root->rightChild;

                if (!temp) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }

                putNodeInFreeList(temp);

            } else {
                struct pageTreeNode *temp = minValuePTNode(root->rightChild);
                root->address = temp->address;
                root->rightChild = deletePTNodeInternal(root->rightChild, temp->address);
            }
        }
        if (!root) return root;

        root->height = 1 + ptMax(ptHeight(root->leftChild), ptHeight(root->rightChild));
        int balance = getPTBalance(root);

        if ((balance > 1) && (getPTBalance(root->leftChild) >= 0)) 
            return rotatePTRight(root);

        if ((balance > 1) && (getPTBalance(root->leftChild) < 0)) {
            root->leftChild = rotatePTLeft(root->leftChild);
            return rotatePTRight(root);
        }

        if ((balance < -1) && (getPTBalance(root->rightChild) < 0))
            return rotatePTLeft(root);
        if ((balance < -1) && (getPTBalance(root->rightChild) > 0)) {
            root->rightChild = rotatePTRight(root->rightChild);
            return rotatePTLeft(root);
        }
    }

    return root;
}

void deletePTTreeNode(uint32_t address) {
    root =  deletePTNodeInternal(root, address);
}

struct pageTreeNode *getPTNodeInternal(struct pageTreeNode *root, uint32_t address) {
    // empty tree?
    if (!root) return nullptr;

    // found it
    if (root->address == address) return root;

    if (root->address > address) {
        return getPTNodeInternal(root->leftChild, address);
    }

    return getPTNodeInternal(root->rightChild, address);
}

struct pageTreeNode *getPageTreeNode(uint32_t address) {
    return getPTNodeInternal(root, address);
}

pte_t getPTEFromAddress(uint32_t addr) {
    auto pte = addr % 0x400000;
    pte /= 4096;
    return (pte_t) pte;
}

bool haveAvailPages(pde_t pde, pte_t pte, size_t numPages) {
    for (pde_t pdestart = pde; pdestart < 1024; pdestart++) {
        struct page_table_t *pt = (struct page_table_t *) PDTVirtPhys[pde].virtualAddr;
        // check if we have a page table h ere
        if (!pt) {
            // if no page table add it, and we have enough
            uint32_t physAddr = 0;
            uint32_t virtAddr= (uint32_t) mapKernelPage(0,&physAddr);
            // if we can't map a page we are out of memory
            if (!virtAddr) return false;
            // put in table
            PDTVirtPhys[pde+1].physicalAddr = physAddr;
            PDTVirtPhys[pde+1].virtualAddr = virtAddr;
            page_directory[pde+1] = physAddr;
            // return false because the page we just mapped might be in the range
            return false;
        }
        for (pte_t ptestart = pte; ptestart < 1024; ptestart++) {
            numPages--;
            if (!numPages) return true;
        }
        pte = 0;
    }
    return false;
}

void* allocPage(size_t size) {
    // make sure size is right
    if ((size < 0x1000) || (size > 0x8000)) return nullptr;

    // get number of pages
    unsigned char numpages =  (unsigned char) (size / 4096);
    if (size % 0x1000) numpages++;

    // find block of addresses
    pde_t pde = 768;
    pte_t pte = getPTEFromAddress(kernel_heap_start);

    uint32_t start_address = 0;

    // walk the tables
    while ((pde < 1024) && (!start_address)){
        struct page_table_t *pt = (struct page_table_t *) PDTVirtPhys[pde].virtualAddr;
        if (pt) {
            while ((pte < 1024) && (!start_address)) {

                // is it free (0)
                uint32_t addr = (uint32_t)pt + (pte*sizeof(uint32_t));
                if (!((pte_t *)(addr))) {
                    // do we have enough available pages
                    if (haveAvailPages(pde,pte, numpages)) {
                        start_address = (pde * 0x400000) + (pte * 0x1000);
                        break;
                    }
                } else {
                    // check the next pte
                    pte++;
                }
            }
        } else {
            // no page table at this address, 
            // so create it and it is our start address
            uint32_t physicalAddress = 0;
            // if we can't map, we are out of memory
            uint32_t virtAddr = (uint32_t) mapKernelPage(0,&physicalAddress);
            if (!virtAddr) return nullptr;
            PDTVirtPhys[pde].physicalAddr = physicalAddress;
            PDTVirtPhys[pde].virtualAddr = virtAddr;
            page_directory[pde] = physicalAddress;
            start_address = pde*0x400000;
            FlushPageTable();
            break;
        }
        pde++;
    }
    // if we have a start address we are not out of memory
    if (start_address) {
        // allocate the range

        // map the page
        auto address = start_address;
        for (size_t i = 0; i < numpages; i++) {
            if (!mapKernelPage(address,nullptr)) {
                // internal error
                // unmap
                address -= 0x1000;
                for (size_t j = 0; j < i; j++) {
                    unmapPage(address);
                    address -= 0x1000;
                }
                return nullptr;
            }
            address += 0x1000; // next page
        }
        // add the address and size to the tree
        insertPTTreeNode(start_address,numpages);
    }

    return (void *)start_address;

}

bool freePage(uint32_t ptr) {
    // make sure got a pointr
    if (!ptr) return false;

    // do we have the allocation
    struct pageTreeNode *node = getPageTreeNode(ptr);
    if (!node) return false;

    uint32_t address = node->address;

    // free the pages
    for (int i = 0; i < node->numpages; i++) {
        if (!unmapPage(address)) {
            //internal error
            printf("Internal error in freePage, address 0x%Xl not mapped\n\r", address);
        }
        address += 0x1000;
    }

    return true;
}

bool initPageTree() {
    bool rtncde = addFreePTNodes();

    return rtncde;
}