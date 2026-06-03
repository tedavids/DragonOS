// paging.c -- intialize paging
#include <kernel.h>
#include <multiboot.h>
#include <stdio.h>
#include <interupt.h>
#include <paging.h>

#define PAGE_SIZE       4096
#define PAGE_DIRECTORY_ADDRESS 0x2000
#define KERNEL_PAGE_TABLE_ADDRESS 0x3000
#define KERNEL_HEAP_PAGE_TABLE_ADDRESS 0x4000
#define POST_BOOT_PAGE_TABLE_ADDRESS 0x100000
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

//struct page_directory_t *page_directory = (struct page_directory_t* ) PAGE_DIRECTORY_ADDRESS;
struct page_table_t     *kernel_page_table;

static int numPageDirectoryEntries = 0;
uint32_t kernel_heap_end = 0;    // end of the heap everything else is user space
uint32_t kernel_heap_start = 0; // start of kernel heap

pte_t createPTE(uint32_t address, bool user, bool readwrite, bool present) {
    pte_t entry = address & 0xFFFFF000;
    if (user) entry |= PTE_USER;
    if (readwrite) entry |= PTE_READWRITE;
    if (present) entry |= PTE_PRESENT;

    return entry;
}

// get the proper pdt entry

// notes:  pcd is page cache disable, pwt is page write through
pde_t createPDEEntry(uint32_t address, bool global, bool size4m, bool pcd, 
                     bool pwt, bool user, bool readwrite, bool present) {
    pde_t entry = address & 0xFFFFF000;
    if (global) address |= PDE_GLOBAL;
    if (size4m) address |= PDE_4M_PAGE;
    if (pcd) address |= PDE_CACHE_DISABLE;
    if (pwt) address |= PDE_WRITE_THRU;
    if (user) address |= PDE_USER;
    if (readwrite) address |= PDE_READWRITE;
    if (present) address |= PDE_PRESENT;

    return entry;
}


void mapMemoryRange(struct page_table_t *table, int *tableidx, int *ptecounter, 
                    uint32_t startaddress, uint32_t endaddress)  {
 
    // if the end is before the kernel end, we already have it mapped, 
    // so this code won't execute
    if (endaddress > kernel_heap_end)  {
        // it needs mapping
        if (startaddress < (uint32_t) kernel_heap_end) {
            // if start address is less than kernel end get new start address
            startaddress = kernel_heap_end;   // kernel_heap_start has already been 4k aligned
        }
        // now we have a start and end address we can map
        while (startaddress < endaddress) {
            // sanity check
            if ((*ptecounter < 0) || (*ptecounter >= 1024)) kernelabort(0,0,0,0,0); 
            table[*tableidx].pte[(*ptecounter)++] = createPTE(startaddress,false, true, true); 
            // check if the table is full
            if (*ptecounter >= ENTRIES_PER_PAGE_TABLE) {
                *ptecounter = 0;         // reset it
                // save current table
                page_directory->direntry[numPageDirectoryEntries++] = 
                    createPDEEntry((uint32_t) &table[(*tableidx)++],
                        false, false, false, false, true, true,true ); 
                // check if we are at the the max size 
                if (numPageDirectoryEntries >= MAX_PAGE_DIRECTORY_ENTRIES) return;
            }
            // next block
            startaddress += PAGE_SIZE;
        }
        
    } 
}

// get the proper pte entry

pte_t createPTEEntry(uint32_t address) {
    // check if kernel code
    if ((address >= (uint32_t) &_text_start) && 
        (address <= (uint32_t) &_text_end)) {
        // kernel, read only, present
        return createPTE(address, false, false, true);
    }
    // check if kernel data
    if ((address >= (uint32_t) &_data_start) && 
        (address <= (uint32_t) &_data_end)) {
        // kernel, readwrite, present
        return createPTE(address, false, true, true);
    }
    // check if rodata
    if ((address >= (uint32_t) &_rodata_start) && 
        (address <= (uint32_t) &_rodata_end)) {
        // kernel, read only, present
        return createPTE(address, false, false, true);
    }
    // check if kernel unitialized data
    if ((address >= (uint32_t) &_bss_start) && 
        (address <= (uint32_t) &_bss_end)) {
        // kernel, readwrite, present
        return createPTE(address, false, true, true);
    } 
    // check if BIOS
    if ((address >= 0xC8000) && 
        (address <= 0xFFFFF)) {
        // user, read only, present
        return createPTE(address, true, false, true);
    }
    // anything else
    return createPTE(address,false,true,true);
}

// load the page tables for initial memory

void postBootPageInit() {

    struct page_table_t *table = (struct page_table_t*) POST_BOOT_PAGE_TABLE_ADDRESS;
    int tableidx = 0; 
    int ptecounter = 0;

    // get the kernel end on a page boundty (global variable)
    // kernel_heap_start = ALIGN_4K(((uint32_t) (&_kernel_end)));

    for (uint16_t i = 0; i < multiboot_info.mmap.count; i++) {
        uint32_t startaddress = (uint32_t) multiboot_info.mmap.region[i].baseaddr;
        uint32_t endaddress = startaddress + (uint32_t) multiboot_info.mmap.region[i].end;

        mapMemoryRange(table, &tableidx, &ptecounter, startaddress, endaddress);
    }
    // check if we need to save the last entries
    if (!ptecounter) {
        page_directory->direntry[numPageDirectoryEntries++] = 
                createPDEEntry((uint32_t) &table[tableidx++],
                    false, false, false, false, true, true,true ); 
            }
    
    pagingreload(); 

}

// this sets up the initial paging table resources
void initpaging() {
    // set where to put our table 
    kernel_page_table = (struct page_table_t *) KERNEL_PAGE_TABLE_ADDRESS;
    page_directory = (struct page_directory_t* ) PAGE_DIRECTORY_ADDRESS;

    // initialize page table entries
    uint32_t kernel_end = (uint32_t) &_kernel_end;
    for (uint32_t i = 0; ((((i * PAGE_SIZE) < kernel_end)) && (i < 1024)); i++) {
        uint32_t address = (i * PAGE_SIZE);
        kernel_page_table->pte[i] = createPTEEntry(address);
    }

    // set our page directory entry
    page_directory->direntry[numPageDirectoryEntries++] = 
        createPDEEntry((uint32_t) KERNEL_PAGE_TABLE_ADDRESS,
        false, false, false, false, false, true,true );

    // get address of kernel page table
    struct page_table_t * kernel_heap_page_table = (struct page_table_t *) KERNEL_HEAP_PAGE_TABLE_ADDRESS;

    kernel_heap_start = ALIGN_4K(kernel_end);
    kernel_heap_end = kernel_heap_start + 0x100000; // TODO come back to this 1M heap for now

    // get number of 4K pages
    uint32_t heap_page_count = ALIGN_4K(kernel_heap_end - kernel_heap_start) / PAGE_SIZE;
    if (heap_page_count > 1024) heap_page_count = 1024;

    // TODO:  set up for multiple pages?
    // load heap pages
    for (uint32_t i = 0; i < heap_page_count; i++) {
        uint32_t address = (i * PAGE_SIZE) + kernel_heap_start;
        kernel_heap_page_table->pte[i] = createPTE(address, false, true, true);
    }

    // set our page directory entry
    page_directory->direntry[numPageDirectoryEntries++] = 
        createPDEEntry((uint32_t) KERNEL_HEAP_PAGE_TABLE_ADDRESS,
        false, false, false, false, false, true,true );

    pagingreload();
}