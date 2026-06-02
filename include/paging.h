// paging header

#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

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
extern struct page_directory_t  *page_directory;
extern struct page_table_t      *kernel_page_table;
extern void                     postBootPageInit();

extern void                     FlushPageTable();
#endif