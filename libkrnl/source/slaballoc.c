// our functions for our slab allocators

#include <bitarray.h>
#include <string.h>
#include <paging.h>
#include <stdio.h>

#include <slaballoc.h>

// constants
const uint16_t  SLAB4PERPAGE = (4096 / sizeof(struct slabAlloc4_t));
const uint16_t  SLAB8PERPAGE = (4096 / sizeof(struct slabAlloc8_t));
const uint16_t  SLAB16PERPAGE = (4096 / sizeof(struct slabAlloc16_t));
const uint16_t  SLAB32PERPAGE = (4096 / sizeof(struct slabAlloc32_t));
const uint16_t  SLAB64PERPAGE = (4096 / sizeof(struct slabAlloc64_t));
const uint16_t  SLAB128PERPAGE = (4096 / sizeof(struct slabAlloc128_t));
const uint16_t  SLAB256PERPAGE = (4096 / sizeof(struct slabAlloc256_t));
const uint16_t  SLAB512PERPAGE = (4096 / sizeof(struct slabAlloc512_t));
const uint16_t  SLAB1024PERPAGE = (4096 / sizeof(struct slabAlloc1024_t));
const uint16_t  SLAB2048PERPAGE = (4096 / sizeof(struct slabAlloc2048_t));

const uint16_t  ALLOCSPERPAGE4 = 1024;
const uint16_t  ALLOCSPERPAGE8 = 512;
const uint16_t  ALLOCSPERPAGE16 = 256;
const uint16_t  ALLOCSPERPAGE32 = 128;
const uint16_t  ALLOCSPERPAGE64 = 64;
const uint16_t  ALLOCSPERPAGE128 = 32;
const uint16_t  ALLOCSPERPAGE256 = 16;
const uint16_t  ALLOCSPERPAGE512 = 8;
const uint16_t  ALLOCSPERPAGE1024 = 4;
const uint16_t  ALLOCSPERPAGE2048 = 2;

// pointers to our slab structures

struct slabAlloc4_t     *slab4 = nullptr;
struct slabAlloc8_t     *slab8 = nullptr;
struct slabAlloc16_t    *slab16 = nullptr;
struct slabAlloc32_t    *slab32 = nullptr;
struct slabAlloc64_t    *slab64 = nullptr;
struct slabAlloc128_t   *slab128 = nullptr;
struct slabAlloc256_t   *slab256 = nullptr;
struct slabAlloc512_t   *slab512 = nullptr;
struct slabAlloc1024_t  *slab1024 = nullptr;
struct slabAlloc2048_t  *slab2048 = nullptr;

// initialization functions

//  IIIIIIII  NN    NN  IIIIIIII  TTTTTTTT
//     II     NNN   NN     II        TT
//     II     NNNN  NN     II        TT
//     II     NN NN NN     II        TT
//     II     NN  NNNN     II        TT
//     II     NN   NNN     II        TT
//  IIIIIIII  NN    NN  IIIIIIII     TT

bool initSlab4Page(struct slabAlloc4_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc4_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc4_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB4PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB4PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE4;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab8Page(struct slabAlloc8_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc8_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc8_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB4PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB4PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE8;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab16Page(struct slabAlloc16_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc16_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc16_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB16PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB16PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE16;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab32Page(struct slabAlloc32_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc32_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc32_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB32PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB32PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE32;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab64Page(struct slabAlloc64_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc64_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc64_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB64PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB64PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE64;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab128Page(struct slabAlloc128_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc128_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc128_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB128PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB64PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE128;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab256Page(struct slabAlloc256_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc256_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc256_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB256PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB256PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE256;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab512Page(struct slabAlloc512_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc512_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc512_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB512PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB512PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE512;

        // all will be available at the end
        memset(temp->availmap,0xff,sizeof(temp->availmap));
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab1024Page(struct slabAlloc1024_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc1024_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc1024_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB1024PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB1024PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE1024;

        // all will be available at the end
        temp->availmap[0] = 0x0F; // first 4 bits
 
        // on to the next
        temp++;
    }
 
    return true;
}

bool initSlab2048Page(struct slabAlloc2048_t **root) {
    // make sure root is null
    if (!root) return false;

    uint32_t address;
    // allocate a page
    *root = (struct slabAlloc2048_t *) mapKernelPage(0,&address);
    // check for out of memory
    if (!root) return false;

    // address of structure we are initiating
    struct slabAlloc2048_t *temp = *root;

    // map the pages
    for (int i = 0; i < SLAB2048PERPAGE; i++) {
        temp->address = 0;
        
        // set address of next structure
        if (i == (SLAB2048PERPAGE - 1)) {
            temp->nextaddr = nullptr;
        } else {
            temp->nextaddr = temp + 1;
        }

        // all the bits will be available
        temp->avail = ALLOCSPERPAGE2048;

        // all will be available at the end
        temp->availmap[0] = 0xFF; // first 2 bits
 
        // on to the next
        temp++;
    }
 
    return true;
}
//  FFFFFFFF  RRRRRRR   EEEEEEEE  EEEEEEEE
//  FF        RR    RR  EE        EE
//  FF        RR    RR  EE        EE
//  FFFFF     RRRRRRR   EEEEE     EEEEE
//  FF        RR  RR    EE        EE
//  FF        RR   RR   EE        EE
//  FF        RR    RR  EEEEEEEE  EEEEEEEE

// free functions
bool free4(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc4_t *slab = slab4;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/4;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\4", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE4) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 4 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free8(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc8_t *slab = slab8;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/8;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE8) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 8 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free16(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc16_t *slab = slab16;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/16;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE16) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 16 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free32(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc32_t *slab = slab32;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/32;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE32) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 32 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free64(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc64_t *slab = slab64;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/64;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE64) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 64 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free128(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc128_t *slab = slab128;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/128;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE128) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 128 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free256(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc256_t *slab = slab256;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/256;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE256) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 256 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free512(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc512_t *slab = slab512;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/512;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE512) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 512 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free1024(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc1024_t *slab = slab1024;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/1024;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE1024) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    }
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 1024 byte allocation 0x%Xl\n\r",addr);
    return false;
}

bool free2048(uint32_t addr) {
    // make sure I am sent something
    if (!addr) return false;

    struct slabAlloc2048_t *slab = slab2048;

    uint32_t pageaddr = addr & 0xFFFFF000;
    uint16_t offset = addr & 0x00000FFF/2048;

    while (slab) {
        // page has allocations
        if (slab->address) {
            // matches our page addr
            if (slab->address == pageaddr) {
                // clear the allocation
                bool bitval = false;
                setBit(slab->availmap,(sizeof(slab->availmap)*8),offset,&bitval);
                if (!bitval) {
                    slab->avail++;
                } else {
                    printf("Double free of address: 0x%X1\n\r", addr);
                    return false;
                }
                // if they are all available again, give the page back
                if (slab->avail == ALLOCSPERPAGE2048) {
                    if (unmapPage(slab->address)) {
                        // and mark ready to be reallocated if necessary
                        slab->address = 0;
                    };
                }
                return true;
            }
        }
        slab = slab->nextaddr;
    }
    // we shouldn't get here, but if we do let the console know
    printf("Unable to free 128 byte allocation 0x%Xl\n\r",addr);
    return false;
}


//       AA        LL        LL        OOOOOOOO  CCCCCCCC
//      AAAA       LL        LL        OO    OO  CC
//     AA  AA      LL        LL        OO    OO  CC
//    AAAAAAAA     LL        LL        OO    OO  CC
//   AA      AA    LL        LL        OO    OO  CC
//  AA        AA   LL        LL        OO    OO  CC
// AA          AA  LLLLLLLL  LLLLLLLL  OOOOOOOO  CCCCCCCC

// allocate functions
void *alloc4(size_t size) {
    // make sure it's valid
    if (!size || (size > 4)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc4_t *curslab = slab4;
    struct slabAlloc4_t *priorslab = nullptr;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE4; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (4 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab4Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc4(size);

    return ptr;
}

void *alloc8(size_t size) {
    // make sure it's valid
    if (!size || (size < 5) || (size > 8)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc8_t *curslab = slab8;
    struct slabAlloc8_t *priorslab = nullptr;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE8; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (8 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab8Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc8(size);

    return ptr;
}

void *alloc16(size_t size) {
    // make sure it's valid
    if (!size || (size < 9) || (size > 16)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc16_t *curslab = slab16;
    struct slabAlloc16_t *priorslab = nullptr;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE16; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (16 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab16Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc16(size);

    return ptr;
}

void *alloc32(size_t size) {
    // make sure it's valid
    if (!size || (size < 17) || (size > 32)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc32_t *curslab = slab32;
    struct slabAlloc32_t *priorslab = nullptr;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE32; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (32 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab32Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc32(size);

    return ptr;
}

void *alloc64(size_t size) {
    // make sure it's valid
    if (!size || (size < 33) || (size > 64)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc64_t *curslab = slab64;
    struct slabAlloc64_t *priorslab = nullptr;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE64; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (64 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab64Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc64(size);

    return ptr;
}

void *alloc128(size_t size) {
    // make sure it's valid
    if (!size || (size < 65) || (size > 128)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc128_t *priorslab = nullptr;
    struct slabAlloc128_t *curslab = slab128;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE128; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (128 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab128Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc128(size);

    return ptr;
}

void *alloc256(size_t size) {
    // make sure it's valid
    if (!size || (size < 129) || (size > 256)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc256_t *priorslab = nullptr;
    struct slabAlloc256_t *curslab = slab256;
    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE256; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (256 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab256Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc256(size);

    return ptr;
}

void *alloc512(size_t size) {
    // make sure it's valid
    if (!size || (size < 257) || (size > 512)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc512_t *priorslab = nullptr;
    struct slabAlloc512_t *curslab = slab512;

    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE512; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (512 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab512Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc512(size);

    return ptr;
}

void *alloc1024(size_t size) {
    // make sure it's valid
    if (!size || (size < 513) || (size > 1024)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc1024_t *priorslab = nullptr;
    struct slabAlloc1024_t *curslab = slab1024;
    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE1024; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (1024 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorslab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab1024Page(&priorslab->nextaddr)) return nullptr;
    // try again
    ptr = alloc1024(size);

    return ptr;
}

void *alloc2048(size_t size) {
    // make sure it's valid
    if (!size || (size < 1025) || (size > 2048)) return nullptr;

    void * ptr = nullptr;

    // start at the beginning 
    struct slabAlloc2048_t *priorSlab = nullptr;
    struct slabAlloc2048_t *curslab = slab2048;
    while (curslab) {
        if (curslab->avail > 0) {
            // there are available allocations
            for (size_t i = 0; i < ALLOCSPERPAGE2048; i++) {
                bool bitval = false;
                // check that we get a bit, if not something is corrupt
                if (!getBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval)) {
                    return nullptr;
                }
                if (bitval) {
                    // we have an available allocation
                    // check the page has been allocated
                    // if not allocate it now
                    if (!curslab->address) curslab->address = (uint32_t) mapKernelPage(0,nullptr);
                    // check for out of memory
                    if (!curslab->address) return nullptr; 
                    // get the address of our allocation
                    ptr = (void *) (curslab->address + (2048 * i));
                    // mark the bit as unavailable
                    clearBit(curslab->availmap, (sizeof(curslab->availmap) * 8), i, &bitval);
                    // and decrease the number of bits left in this slab
                    curslab->avail--;
                    return ptr;
                }
            }
        }
        priorSlab = curslab;
        curslab = curslab->nextaddr;
    }

    // if we get here there were no available allocations
    // make a new page and send back the first
    // if we fail return the nullptr, unknown error
    if (!initSlab2048Page(&priorSlab->nextaddr)) return nullptr;
    // try again
    ptr = alloc2048(size);

    return ptr;
}

bool initSlab4() {
    // init the first page
    bool rtncde = initSlab4Page(&slab4);

    return rtncde;
}


bool initSlab8() {
    // init first page
    bool rtncde = initSlab8Page(&slab8);

    return rtncde;
}



bool initSlab16() {
    // init first page
    bool rtncde = initSlab16Page(&slab16);

    return rtncde;
}

bool initSlab32() {
    bool rtncde = initSlab32Page(&slab32);
    return rtncde;
} 

bool initSlab64() {
    bool rtncde = initSlab64Page(&slab64);
    return rtncde;
} 

bool initSlab128() {
    bool rtncde = initSlab128Page(&slab128);
    return rtncde;
} 

bool initSlab256() {
    bool rtncde = initSlab256Page(&slab256);
    return rtncde;
} 

bool initSlab512() {
    bool rtncde = initSlab512Page(&slab512);
    return rtncde;
} 

bool initSlab1024() {
    bool rtncde = initSlab1024Page(&slab1024);
    return rtncde;
} 

bool initSlab2048() {
    bool rtncde = initSlab2048Page(&slab2048);
    return rtncde;
} 

bool initSlabAlloc() {
    // init the slabs
    bool rtncde = initSlab4();
    rtncde &= initSlab8();
    rtncde &= initSlab16();
    rtncde &= initSlab32();
    rtncde &= initSlab64();
    rtncde &= initSlab128();
    rtncde &= initSlab256();
    rtncde &= initSlab512();
    rtncde &= initSlab1024();
    rtncde &= initSlab2048();

    return rtncde;
}