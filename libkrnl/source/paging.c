// paging.c -- intialize paging
#include <string.h>
#include <kernel.h>
#include <multiboot.h>
#include <stdio.h>
#include <interupt.h>
#include <bitarray.h>
#include <paging.h>

#define PAGE_SIZE       4096
#define ENTRIES_PER_PAGE_TABLE  1024
#define MAX_PAGE_DIRECTORY_ENTRIES 1024

// page directory entry flags
#define PDE_GLOBAL      0b100000000
#define PDE_4K_PAGE     0b10000000
#define PDE_4M_PAGE     0b00000000
#define PDE_ACCESSED    0b100000
#define PDE_CACHE_DISABLE 0b10000
#define PDE_WRITE_THRU  0b000
#define PDE_USER        0b100
#define PDE_READWRITE   0b10
#define PDE_PRESENT     0b1

// page table entry flags
#define PTE_PRESENT     0x0001
#define PTE_READWRITE   0x0002
#define PTE_USER        0x0004
#define PTE_KERNEL      0x0000

#define ALIGN_4K(x) (((uint32_t)(x) + ((uint32_t)4095)) & ~((uint32_t)4095))

// types
typedef uint32_t pte_t;
typedef uint32_t pde_t;

// page tables
extern uint32_t page_table_000[1024];
extern uint32_t page_table_C00[1024];

// sections so we can set read/write flags correctly
extern uint32_t _text_start;
extern uint32_t _text_end;
extern uint32_t _rodata_start;
extern uint32_t _rodata_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

extern void pagingreload();


// array of bits.  Each bit represents a 4K page of memory
// if the corresponding bit is 0, it is already in memory
// if it is 1, then it is available

static unsigned char availMemMap[0x20000]; // 1024 x 1024

// array of bits.    Each bit represents a 4K page of memory
// if the corresponding bit is 0, it is read/only
// if it is 1 then it is writable

static unsigned char readwriteMap[0x20000]; // 1024x 1024

// used so we can keep track of virtual and physical addresses for the page tables
// This is because we deal in the virtual address, and the CPU deals with physical
// Since we need to put the PHYSICAL address in the PDT we need to know it too

struct Virt_Phys_t {
    uint32_t   physicalAddr;    // the physical address in memory that PDT uses
    uint32_t   virtualAddr;     // the virtual address that we use 
};

// for memory allocation for pages above 16m
const uint32_t  pmmLowPage = 0x1000;            // 16M as an offset
static uint32_t pmmHead;
static uint32_t pmmTail;


struct Virt_Phys_t  PDTVirtPhys[1024];

// SSSSSSSS  HH    HH        A        RRRRRR    EEEEEEEE  DDDDDD
// SS        HH    HH       AAA       RR    RR  EE        DD    DD
// SS        HH    HH      AA AA      RR    RR  EE        DD     DD
// SSSSSSSS  HHHHHHHH     AA   AA     RRRRR     EEEEEE    DD     DD
//       SS  HH    HH    AAAAAAAAA    RR  RR    EE        DD     DD 
//       SS  HH    HH   AA       AA   RR   RR   EE        DD    DD
// SSSSSSSS  HH    HH  AA         AA  RR    RR  EEEEEEEE  DDDDDD

// invalidate a page

static inline void invalidatePage(void * address) {
    asm volatile("invlpg (%0)" : : "r" (address) : "memory");
}

uint32_t pdeOffset(uint32_t address) {
    return (address / 0x400000);
}

uint32_t pteOffset(uint32_t address) {

    return (address >> 12) & 0x3FF;
}

uint32_t addrFromPdePdt(uint32_t pde, uint32_t pte) {
    return (pde * 0x400000) + (pte * 0x1000);
}


// increment pmmHead to next bit
void incrementHead() {
    // is the buffer empty
    if (pmmHead == pmmTail) return;

    // if not icrement head
    pmmHead++;

    // are we past the end?
    if (pmmHead > multiboot_info.meminfo.upper) {
        pmmHead = pmmLowPage;
    }
    return;
}

void setTail(uint32_t offset) {
    // head is ahead of tail
    if (pmmHead > pmmTail) {
        // tail is before this bit, set it to it
        if (offset > pmmTail) {
            pmmTail = offset;
            return;
        }
    }
    // head is behind tail
    if (pmmHead < pmmTail) {
        if (offset < pmmTail) {
            pmmTail = offset;
            return;
        }
    }

    // both must be equal
    pmmTail = offset;
}
// create a page table entry
// array of bits.  Each bit represents a 4K page of memory
// if the corresponding bit is 0, it is already in memory
// if it is 1, then it is available

static unsigned char availMemMap[0x20000]; // 1024 x 1024

// array of bits.    Each bit represents a 4K page of memory
// if the corresponding bit is 0, it is read/only
// if it is 1 then it is writable

static unsigned char readwriteMap[0x20000]; // 1024x 1024

// used so we can keep track of virtual and physical addresses for the page tables
// This is because we deal in the virtual address, and the CPU deals with physical
// Since we need to put the PHYSICAL address in the PDT we need to know it too

struct PDT_Virt_Phys_t {
    uint32_t   physicalAddr;    // the physical address in memory that PDT uses
    uint32_t   virtualAddr;     // the virtual address that we use 
};

struct PDT_Virt_Phys_t  PDTVirtPhys[1024];

// Page Directory functions

void initPDTVirtPhys() {
    // first meg
    PDTVirtPhys[0].physicalAddr = (uint32_t) (&page_table_000 - 0xC0000000);
    PDTVirtPhys[0].virtualAddr = (uint32_t) &page_table_000;

    // Kernel pages
    PDTVirtPhys[768].physicalAddr = (uint32_t) (&page_table_C00 - 0xC0000000);
    PDTVirtPhys[768].virtualAddr = (uint32_t) &page_table_C00;

    // all others will need to be added via the putPDTEntry function

}

pte_t createPTE(uint32_t address, bool user, bool readwrite, bool present) {
    pte_t entry = address & 0xFFFFF000;
    if (user) entry |= PTE_USER;
    if (readwrite) entry |= PTE_READWRITE;
    if (present) entry |= PTE_PRESENT;

    return entry;
}

// get the proper pdt entry

// notes:  pcd is page cache disable, pwt is page write through
pde_t createPDTEntry(uint32_t physaddress,bool global, bool size4m, bool pcd, 
                     bool pwt, bool user, bool readwrite, bool present) {
    pde_t entry = physaddress & 0xFFFFF000;
    if (global) physaddress |= PDE_GLOBAL;
    if (size4m) physaddress |= PDE_4M_PAGE;
    if (pcd) physaddress |= PDE_CACHE_DISABLE;
    if (pwt) physaddress |= PDE_WRITE_THRU;
    if (user) physaddress |= PDE_USER;
    if (readwrite) physaddress |= PDE_READWRITE;
    if (present) physaddress |= PDE_PRESENT;

    return entry;
}

// get next free page offset

// parameters:  none

// returns the offset to the next free page, or zero if nothing avail

uint32_t getFreePage() {
    // nothing here?
    if (pmmHead == pmmTail) return 0;

    bool bitval;

    while (getBit(availMemMap,(sizeof(availMemMap)*8),pmmHead,&bitval)) {
        // is this available?
        if (bitval) {
            auto page = pmmHead;
            incrementHead();
            return page;
        }
        incrementHead();
        if (pmmHead == pmmTail) return 0;
    }

    return 0;
}

bool virtPageUsed(uint32_t address) {
    uint32_t pdOffset = pdeOffset(address);

    // if we don't have a page table for this offset it can't exist
    if (!page_directory[pdOffset]) return false;

    // if we do get the page table
    uint32_t *pt = (uint32_t *) page_directory[pdOffset];
    uint32_t ptOffset = pteOffset(address);

    return (bool)(pt[ptOffset]);
}

uint32_t mapPage(uint32_t address, uint32_t physpageoffset, 
                bool kernel, bool writable) 
{
 
    // get offset to page directory
    uint32_t pde = pdeOffset(address);

    // get address of the page table
    uint32_t *pt =  (uint32_t *)(page_directory[pde] & 0xFFFFF000);

    // if the page table already exists
    if (pt) {
        // entry exists
        // get offset
        uint32_t pte = pteOffset(address);

        pt[pte] = createPTE(physpageoffset << 12, !kernel, writable, true);
        clearBit(availMemMap,(sizeof(availMemMap)*8),physpageoffset,nullptr);

        // invalidate the page
        invalidatePage((uint32_t *) (physpageoffset << 12));

        setTail(physpageoffset);

        return address;
    } else {
        // TODO: get page from heap once we have one
        return 0;
    }

    return 0;
}
// unmaps a page

bool unmapPage(uint32_t address) {
    // validate address, we don't unmap thekernel
    if ((address >= (uint32_t)&_kernel_start) && (address < ALIGN_4K(&_kernel_end))) return false;

    uint32_t pde = pdeOffset(address);
    uint32_t pte = pteOffset(address);

    // check if the page table entry exists
    if (!page_directory[pde]) return false;
    uint32_t *pt = (uint32_t *)(page_directory[pde] & 0xFFFFF000);

    // check if the pte exists
    if (!pt[pte]) return false;

    // everything is good, get the offset to the physical page
    uint32_t physaddroffset = pt[pte] >> 12;
 
    // make sure memory is avaiable, otherwise something is amis
    bool bitval;
    getBit(availMemMap, (sizeof(availMemMap) * 8),physaddroffset, &bitval);

    if (bitval) {
        printf("memory and page table out of sync 0x%Xl\n\r", address);
        return false;
    }

    // all is ok
    clearBit(availMemMap, (sizeof(availMemMap)*8), physaddroffset, nullptr);

    // clear the pte
    pt[pte] = 0;

    // invalidate the page
    invalidatePage((uint32_t *) (physaddroffset << 12));

    return true;
}


bool setPageReadOnly(uint32_t address) {
    // validate address isn't in the kernel
    if ((address >= (uint32_t)&_kernel_start) && (address < ALIGN_4K(&_kernel_end))) return false;

    // get offsets
    uint32_t pde = pdeOffset(address);
    uint32_t pte = pteOffset(address);

    // check page_directory and make sure address is mapped there
    if (!page_directory[pde]) return false;
    uint32_t *pt = (uint32_t *) (page_directory[pde] & 0xFFFFF000);

    // check that the page is mapped
    if (!pt[pte]) return false;

    // set the bit to read only
    pt[pte] &= 0xFFFFFFFD;

    invalidatePage((uint32_t *)address);

    return true;
} 

bool setPageReadWrite(uint32_t address) {
    // validate address isn't in the kernel
    if ((address >= (uint32_t)&_kernel_start) && (address < ALIGN_4K(&_kernel_end))) return false;

    // get offsets
    uint32_t pde = pdeOffset(address);
    uint32_t pte = pteOffset(address);

    // check page_directory and make sure address is mapped there
    if (!page_directory[pde]) return false;
    uint32_t *pt = (uint32_t *) (page_directory[pde] & 0xFFFFF000);

    // check that the page is mapped
    if (!pt[pte]) return false;

    // set the bit to read only
    pt[pte] |= 0x00000002;

    invalidatePage((uint32_t *)address);

    return true;
} 

// retuns true if entry created, false if it already exists
// the user is responsable to flush the table

// retuns true if entry created, false if it already exists
// the user is responsable to flush the table

bool addPDTEntry(int entrynum, uint32_t physaddr, uint32_t virtaddr, bool global, bool size4m,
                 bool pcd, bool pwt, bool user, bool readwrite, bool present) {

    // does the entry already exist?  If so we really should just load new addresses in it
    if (page_directory[entrynum]) return false;

    pde_t entry = createPDTEntry(physaddr, global, size4m, pcd, pwt, user, readwrite, present);

    if (entry) {
        page_directory[entrynum] = entry;
        PDTVirtPhys[entrynum].physicalAddr = physaddr;
        PDTVirtPhys[entrynum].virtualAddr = virtaddr;
    }
    return true;
}


//  UU    UU  SSSSSSSS  EEEEEEEE  RRRRR
//  UU    UU  SS        EE        RR   RR
//  UU    UU  SS        EE        RR   RR
//  UU    UU  SSSSSSSS  EEEEE     RRRRR
//  UU    UU        SS  EE        RR RR
//  UU    UU        SS  EE        RR  RR
//  UUUUUUUU  SSSSSSSS  EEEEEEEE  RR   RR

// this function walks the page directory to find a 
// page that is not being used

// returns: uint32_t the address or 0 if none available

uint32_t getNextUserAddr() {

    uint32_t pde = 768;
    uint32_t pte = pteOffset((uint32_t)&_kernel_start);
    
    while (pde > 0) {
        // check page table entries for an available address
        if (page_directory[pde]) {
            uint32_t *pt = (uint32_t *) page_directory[pde];
            //scan for an address
            while (pte < 1024)  {
                if (!pt[pte]) {
                    return addrFromPdePdt(pde,pte);
                }
                pte++;
            }
            pte = 0;
        }
        pde--;
    }

    return 0;
}

// map a kernel page

void *mapUserPage(uint32_t address) {

   // if page is < kernel end we will pick the address
    if (address < (uint32_t) ALIGN_4K(&_kernel_end)) address = 0;

    // if we don't have an address, or it is used, get the 
    // next available kernel address
    if (!address || virtPageUsed(address)) {
        address = getNextUserAddr();
        // if we can't find one, fail
        if (!address) return nullptr;
    };

    // next free page has our next free page if it is 0 then we are out of memory
    uint32_t physpage = getFreePage();

    if (!physpage) return nullptr;

    return (void*) mapPage(address, physpage, false, true);
}

//  KK   KK  EEEEEEEE  RRRRRR    NN     NN  EEEEEEEE  LL
//  KK  KK   EE        RR    RR  NNN    NN  EE        LL
//  KK KK    EE        RR    RR  NN NN  NN  EE        LL
//  KKKK     EEEEE     RRRRRR    NN  N  NN  EEEEE     LL
//  KK KK    EE        RR  RR    NN  NN NN  EE        LL
//  KK  KK   EE        RR   RR   NN    NNN  EE        LL
//  KK   KK  EEEEEEEE  RR    RR  NN     NN  EEEEEEEE  LLLLLLLL

// paging functions

// this function walks the page directory to find a 
// page that is not being used

// returns: uint32_t the address or 0 if none available

uint32_t getNextKernelAddr() {

    uint32_t pde = 768;
    uint32_t pte = pteOffset(ALIGN_4K(&_kernel_end));
    
    while (pde < 1024) {
        // check page table entries for an available address
        if (page_directory[pde]) {
            uint32_t *pt = (uint32_t *) page_directory[pde];
            //scan for an address
            while (pte < 1024)  {
                if (!pt[pte]) {
                    return addrFromPdePdt(pde,pte);
                }
                pte++;
            }
            pte = 0;
        }
        pde++;
    }

    return 0;
}

// map a kernel page

void *mapKernelPage(uint32_t address, uint32_t *physaddr) {

   // if page is < kernel end we will pick the address
    if (address < ALIGN_4K(&_kernel_end)) address = 0;

    // if we don't have an address, or it is used, get the 
    // next available kernel address
    if (!address || virtPageUsed(address)) {
        address = getNextKernelAddr();
        // if we can't find one, fail
        if (!address) return nullptr;
    };

    // next free page has our next free page if it is 0 then we are out of memory
    uint32_t physpage = getFreePage();

    if (!physpage) return nullptr;

    // send back the physical page address
    if (physaddr) *physaddr = physpage << 12;

    return (void*) mapPage(address, physpage, true, true);
}

bool initPhysicalMemoryManager() {
    // set head and tail of our circular available buffer
    pmmTail = multiboot_info.meminfo.upper;
    pmmHead = pmmLowPage;

    return true;
}
// Page Directory functions

void initPDTVirtPhys() {
    // first meg
    PDTVirtPhys[0].physicalAddr = (uint32_t) (&page_table_000 - 0xC0000000);
    PDTVirtPhys[0].virtualAddr = (uint32_t) &page_table_000;

    // Kernel pages
    PDTVirtPhys[768].physicalAddr = (uint32_t) (&page_table_C00 - 0xC0000000);
    PDTVirtPhys[768].virtualAddr = (uint32_t) &page_table_C00;

    // all others will need to be added via the putPDTEntry function

}
    if (page_directory->direntry[entrynum]) return false;

    pde_t entry = createPDTEntry(physaddr, global, size4m, pcd, pwt, user, readwrite, present);

    if (entry) {
        page_directory->direntry[entrynum] = entry;
        PDTVirtPhys[entrynum].physicalAddr = physaddr;
        PDTVirtPhys[entrynum].virtualAddr = virtaddr;
    }
    return true;
}

// delete a pdt entry, the user will need to do the flush

bool dltPDTEntry(int entrynum) {

    // if it doesnt exist we can't delete it
    if (page_directory[entrynum] == 0) {
    if (page_directory->direntry[entrynum] == 0) {
        // clean up PDTVirtPhys if necessary
        PDTVirtPhys[entrynum].physicalAddr = 0;
        PDTVirtPhys[entrynum].virtualAddr = 0;
        return false;
    }
    // delete it 
    page_directory[entrynum] = 0;
    page_directory->direntry[entrynum] = 0;
    PDTVirtPhys[entrynum].physicalAddr = 0;
    PDTVirtPhys[entrynum].virtualAddr = 0;

    return true;
}


// get the virtual address of a page table entry in the PDT

// Parameters:  int pde -- the directory entry to get
// Returns:     page_table_t* -- a pointer to the virtual addres of the page table
//                               if the PDE is invalid or an entry does not exist
//                               returns the nullptr
uint32_t* getPDTVirt(int pde) {
    // validate it is in range
    if ((pde < 0) || (pde > 1023)) return nullptr;
    if (PDTVirtPhys[pde].virtualAddr == 0x0) return nullptr;
    return (uint32_t *) PDTVirtPhys[pde].virtualAddr;
struct page_table_t *getPDTVirt(int pde) {
    // validate it is in range
    if ((pde < 0) || (pde > 1023)) return nullptr;
    if (PDTVirtPhys[pde].virtualAddr == 0x0) return nullptr;
    return (struct page_table_t*) PDTVirtPhys[pde].virtualAddr;
}

// page table functions

// paging functions

// initialize the arrays holding what is in memory and what isn't

// initialize arrary with actual memory

bool initActualMemory() {
    bool rtncde = true;

    // we need to map the video memory as it isn't part of the memory map

    for (size_t i = 0XA0; i <= 0xEF; i++) {
        rtncde &= setBit(availMemMap, (sizeof(availMemMap)*8), i, nullptr);
    }

    //mark memory as available if it is in the memory map provided by GRUB
    for (uint32_t i = 0; i < multiboot_info.mmap.count; i++) {
        for (uint32_t j = (uint32_t)multiboot_info.mmap.region[i].baseaddr/0x1000; 
            j <= multiboot_info.mmap.region[i].endaddr/0x1000; j++) {
            rtncde &= setBit(availMemMap, (sizeof(availMemMap)*8), j, nullptr);
        }
    }

    return rtncde;
}

// initailize the read write map
bool initReadOnlyRanges() {

    bool rtncde = true;
    // set bios areas as read only

    // real mode vector table (0x0-0x3ff)
    // page 0
    rtncde &= clearBit(readwriteMap,(sizeof(readwriteMap)*8),0,nullptr);
    
    // Bios data area (0x400-0x4ff)
    // page 1
    rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), 1, nullptr);

    // extended BIOS data area (0x80000-0x9FFFF)
    // pages 0x80-0x9F
    for (size_t i = 0x80; i <= 0x9F; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), i, nullptr);
    }

    // video bios (0xC0000-0xC7FFF)
    // pages 0xC0-0xC7
    for (size_t i = 0xC0; i < 0xC7; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), i, nullptr);
    }

    // BIOS expansions (0xC8000-0xEFFFF)
    // pages 0xC8-0xEF
    for (size_t i = 0xC80; i <= 0xEF; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }

    // Motherboard BIOS (0xF0000 - 0xFFFFF)
    // pages 0xF0-0xFF
    for (size_t i = 0xF0; i <= 0xFF; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }

    // .text
    for (size_t i = ((uint32_t)&_text_start)/0x1000; i <= ((uint32_t)&_text_end)/0x1000; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }



    // real mode vector table (0x0-0x3ff)
    // page 0
    rtncde &= clearBit(readwriteMap,(sizeof(readwriteMap)*8),0,nullptr);
    
    // Bios data area (0x400-0x4ff)
    // page 1
    rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), 1, nullptr);

    // extended BIOS data area (0x80000-0x9FFFF)
    // pages 0x80-0x9F
    for (size_t i = 0x80; i <= 0x9F; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), i, nullptr);
    }

    // video bios (0xC0000-0xC7FFF)
    // pages 0xC0-0xC7
    for (size_t i = 0xC0; i < 0xC7; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap) * 8), i, nullptr);
    }

    // BIOS expansions (0xC8000-0xEFFFF)
    // pages 0xC8-0xEF
    for (size_t i = 0xC80; i <= 0xEF; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }

    // Motherboard BIOS (0xF0000 - 0xFFFFF)
    // pages 0xF0-0xFF
    for (size_t i = 0xF0; i <= 0xFF; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }

    // .text
    for (size_t i = ((uint32_t)&_text_start)/0x1000; i <= ((uint32_t)&_text_end)/0x1000; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }


    // .rodata
    for (size_t i = ((uint32_t)&_rodata_start)/0x1000; i <= ((uint32_t)&_rodata_end)/0x1000; i++) {
        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
    }

    // processs memory map
    for (uint32_t i = 0; i < multiboot_info.mmap.count; i++) {
        // we have already done memory < 0xFFFFF, and type 1 is available
        if ((multiboot_info.mmap.region[i].baseaddr > 0xFFFFF) &&
            (multiboot_info.mmap.region[i].type != 1)) {
                for (uint32_t i = (uint32_t)multiboot_info.mmap.region[i].baseaddr/0x1000; 
                     i <= multiboot_info.mmap.region[i].endaddr/0x1000; i++) {
                        rtncde &= clearBit(readwriteMap, (sizeof(readwriteMap)*8), i, nullptr);
                     }
        }
    }

    return rtncde;
}

// And the array with the read write tags
bool initPagingArrays() {
    // initialize array that keeps track of what we have
    initPDTVirtPhys();

    // initialze memory map to available
    memset(availMemMap,0,sizeof(availMemMap));
    // initalize readwrite map to 1 everything read write
    memset(readwriteMap,0xFF,sizeof(readwriteMap));

    bool rtncde = initReadOnlyRanges();

    rtncde &= initActualMemory();

    return rtncde;
}


// And the array with the read write tags
bool initPagingArrays() {
    // initialize array that keeps track of what we have
    initPDTVirtPhys();

    // initialze memory map to available
    memset(availMemMap,0,sizeof(availMemMap));
    // initalize readwrite map to 1 everything read write
    memset(readwriteMap,0xFF,sizeof(readwriteMap));

    bool rtncde = initReadOnlyRanges();

    rtncde &= initActualMemory();

    return rtncde;
}

// set the read only page in the page table
bool setPageTableReadOnly() {
    bool rtncde = true;
    // for each page directory entry that exists we will want to 
    // set the read/write flag, and make sure the memoryMap shows
    // allocated.
    for (size_t pde = 0; pde < 1024; pde++) {
        // get the page table related to the pde
        uint32_t *pt = (uint32_t *) getPDTVirt((int)pde);
        struct page_table_t* pt = getPDTVirt((int)pde);
        if (pt) {
            // set all the entries in the page table 
            uint32_t startaddr = pde * 0x400000; // start address the table entry *  4M
            uint32_t startbit = startaddr/0x1000;
            bool bitval = false;                    // value of bit 
            for (uint32_t i = 0; i < 1024; i++) {
                // check if we have an internal error
                if (pt[i] != 0) {
                    // we have an entry
                    // get the 'real' pageoffset
                    uint32_t pageoffset = pt[i]/0x1000;
                    rtncde &= clearBit(availMemMap,(sizeof(availMemMap)*8), pageoffset, &bitval);
                if (pt->pte[i] != 0) {
                    // we have an entry, so availMemMap should show a zero (not avail to alloc)
                    rtncde &= clearBit(availMemMap,(sizeof(availMemMap)*8), startbit + i, &bitval);
                    // set read only flag
                    rtncde &= getBit(readwriteMap,(sizeof(readwriteMap)*8), startbit + i, &bitval);
                    // !bitval says it is read only (bitval true is readwrite)
                    if (!bitval) {
                        pt[i] ^= 0x00000002;
                        pt->pte[i] ^= 0x00000002;
                    }
                }
            }
        }
    }

    return rtncde;
}


// this sets up the initial paging table resources
bool initPaging() {
 
    // initialize the PDT Virtual/Physical table with what we know at the beginning
    if (!initPagingArrays()) return false;

    if (!setPageTableReadOnly()) return false;

    FlushPageTable();

    if (!initPhysicalMemoryManager()) return false;

    return true;
}