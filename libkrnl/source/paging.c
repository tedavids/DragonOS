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

#define ALIGN_4K(x) (((x) + ((uint32_t)4095)) & ~((uint32_t)4095))

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

// retuns true if entry created, false if it already exists
// the user is responsable to flush the table

bool addPDTEntry(int entrynum, uint32_t physaddr, uint32_t virtaddr, bool global, bool size4m,
                 bool pcd, bool pwt, bool user, bool readwrite, bool present) {

    // does the entry already exist?  If so we really should just load new addresses in it
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
    if (page_directory->direntry[entrynum] == 0) {
        // clean up PDTVirtPhys if necessary
        PDTVirtPhys[entrynum].physicalAddr = 0;
        PDTVirtPhys[entrynum].virtualAddr = 0;
        return false;
    }
    // delete it 
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

// set the read only page in the page table
bool setPageTableReadOnly() {
    bool rtncde = true;
    // for each page directory entry that exists we will want to 
    // set the read/write flag, and make sure the memoryMap shows
    // allocated.
    for (size_t pde = 0; pde < 1024; pde++) {
        // get the page table related to the pde
        struct page_table_t* pt = getPDTVirt((int)pde);
        if (pt) {
            // set all the entries in the page table 
            uint32_t startaddr = pde * 0x400000; // start address the table entry *  4M
            uint32_t startbit = startaddr/0x1000;
            bool bitval = false;                    // value of bit 
            for (uint32_t i = 0; i < 1024; i++) {
                // check if we have an internal error
                if (pt->pte[i] != 0) {
                    // we have an entry, so availMemMap should show a zero (not avail to alloc)
                    rtncde &= clearBit(availMemMap,(sizeof(availMemMap)*8), startbit + i, &bitval);
                    // set read only flag
                    rtncde &= getBit(readwriteMap,(sizeof(readwriteMap)*8), startbit + i, &bitval);
                    // !bitval says it is read only (bitval true is readwrite)
                    if (!bitval) {
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

    return true;
}