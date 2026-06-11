// paging header

#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>
#include <stdbool.h>


typedef uint32_t pte_t;         // page table entry
typedef uint32_t pde_t;         // page directory entry

struct page_directory_t {
    pde_t direntry[1024];
};

struct page_table_t {
    pte_t pte[1024];
};

extern const uint32_t           PAGINGIMPLEMENTED;
extern uint32_t                 kernel_heap_end;
extern uint32_t                 kernel_heap_start;             // end of the kernel (beginning of the heap)
extern uint32_t                 page_directory[1024];
extern uint32_t                 kernel_page_table[1024];
extern void                     postBootPageInit();

extern void                     FlushPageTable();

extern                          bool initPaging();

// public paging functions

// these functions only allocate space above 16M (0x1000000)
// I will need to write a different allocator to handle below
// the 16M boundry

// map read/write page(s) to kernel memory

// Parameters: uint32_t address -- the requested address, this is a suggestion
//                            the returned address may be different, a
//                            zero address means you don't care where it is
//             uint32_t *physaddr -- the physical address of the page
//                            this could be useful if you need it for a page page
//                            table

// returns:     void* pointer to the first page mapped, or null if it fails

extern void *mapKernelPage(uint32_t address, uint32_t *physaddr);
extern void *mapUserPage(uint32_t address);

// unmap a page, the page should have been created with the mapKernelPage, mapUserPage,
//     mapKernelROPage, or mapUserROPage function

/// @param uint32_t address -- Address of page block
///
/// @return bool  success or failure
extern bool unmapPage(uint32_t address);

/// @brief Set a page to read only.  Page must have been created by the map*Page functions
///         and be readwrite
/// @param address -- Address of the page to change
/// @return true if successfull, false otherwise

extern bool setPageReadOnly(uint32_t address);


/// @brief Set a page to read write.  Page must have been created by the map*Page functions
///         and be readwrite
/// @param address -- Address of the page to change
/// @return true if successfull, false otherwise

extern bool setPageReadWrite(uint32_t address);
#endif