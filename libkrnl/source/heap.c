// heap functions

#include <stdint.h>
#include <stddef.h>
#include <paging.h>

#include <stdio.h>
#include <heap.h>

// general flow of each 'heap'

// allocate (malloc)
//1.  A request comes in
//2.  pick proper list it should be handled by 
//3.  if no free pages, allocate one from the free list
//3a.  break it up as necessare to fill the free list
//4.  get the first free list entry and move it to the allocated list (keep in order of address)
//5.  return the address to the user

//free
//1.    a request comes in
//2.    the allocated list is walked, finding the address
//3.    the entry is moved to the proper place on the free list
//4.    a check is made to see if an entire page is free
//5.    if it is the entries are removed and the block is placed back on the free list (at the proper place)

#define PAGE_SIZE       4096
#define HEAP_HEAD_ARRAY_ADDRESS ((heap_head_t*) 0x5000)
#define HEAP_FREE_LIST_ADDRESS ((free_page_list_t*) 0x5281)

// each heap size
#define HEAD_32BTYE     0
#define HEAD_64BYTE     1
#define HEAD_128BYTE    2
#define HEAD_256BYTE    3
#define HEAD_1K         4
#define HEAD_4K         5
#define HEAD_8K         6
#define HEAD_16K        7
#define HEAD_32K        8
#define HEAD_64K        9

#define HEAP_MAX_REQUEST_PAGES 16

extern uint32_t _kernel_end;

const uint32_t MAX_KERNEL_HEAP_SIZE = 0x500000; // 5 meg
uint32_t numHeapPages = 0;

typedef struct {
    uint32_t    alloc;  // address of first element in allocated list
    uint32_t    free;   // address of first element in free list
} heap_head_t;

typedef struct {
    unsigned char free_list[160];    // bitmap of free pages in kernel heap
    uint8_t first_free;     // first free page (we use this to make our search faster
    uint32_t heap_start;    // the address of he start of the hap
} free_page_list_t;

heap_head_t* heap_head = HEAP_HEAD_ARRAY_ADDRESS;    // head for different size block lists, space allocated for 10 
free_page_list_t* heap_free_list = HEAP_FREE_LIST_ADDRESS;

// bit functions to find my bit
void set_bit(unsigned char *bitmap, uint32_t n) {
    bitmap[n / 8] |= (1 << (n % 8)); // Set the n-th bit
}

void clear_bit(unsigned char *bitmap, uint32_t n) {
    bitmap[n / 8] &= (unsigned char)~(1 << (n % 8)); // clear the n-th bit
}

int get_bit(unsigned char *bitmap, uint32_t n) {
    return (bitmap[n / 8] >> (n % 8)) & 1; // Return 1 if set, 0 if not
}


// returns the number of pages in the block, if it = numpages, we have a winner
//   if it returns < numpages its the first used block#
uint32_t checkForFreeBlock(uint32_t firstfreebit, int numpages) {
    // check parameters
    if (numpages == 0) return 0;
    if (numpages > HEAP_MAX_REQUEST_PAGES) return 0;
    if (firstfreebit > numHeapPages) return 0;

    uint32_t checkbit = firstfreebit + 1;
    uint32_t counter = 1;

    // if its one page we don't need to check we know already
    if (numpages > 1) {
        while ((numHeapPages > checkbit) && get_bit(heap_free_list->free_list, checkbit)) {
            if (counter >= (uint32_t) numpages) break; // we have our block
            checkbit++; // next bit to check
            counter++;  // if its good thats the next bit
            if (numHeapPages < checkbit) return 0;
        }
    }
    return counter;
}

// get free page returns an address of a single page, and removes the page from the free list (sets the bit to 0)

// inputs:  numpages - How many pages to get (max of )

uint32_t getFreePage(int numpages) {
    // max request is HEAP_MAX_REQUEST_PAGES
    if (numpages == 0) return 0; // I don't want one
    if (numpages > HEAP_MAX_REQUEST_PAGES) return 0;
    if (numpages > 16) return 0;

    // find the first free block
    uint8_t searchstart = heap_free_list->first_free; // bit number 
    uint32_t firstfreebit = searchstart*8;
    while((numHeapPages >= firstfreebit) && !get_bit(heap_free_list->free_list,firstfreebit))
        firstfreebit++;
    // now find out if the next x bits are also free
    // first check if we only want want one page, in which case
    // firstfreebit is the page we want
    uint32_t foundpages = 0;
    while ((numHeapPages > firstfreebit) && (numpages != (int)(foundpages = checkForFreeBlock(firstfreebit, numpages)))) {
        if (!foundpages) return 0; // some kind of error
        // find next free bit after the current checked block, we alreaday know its too short
        firstfreebit += foundpages + 1;
        while((numHeapPages >= firstfreebit) && !get_bit(heap_free_list->free_list,firstfreebit))
            firstfreebit++;
        if (numHeapPages >= firstfreebit) return 0; // end of the heap
    }
    // if we have gotten here we have found a block
    // get the address of the block
    uint32_t blockaddress = heap_free_list->heap_start + (firstfreebit * PAGE_SIZE);


    // mark block as not free
    for (uint32_t i = 0; i < (uint32_t) numpages; i++) {
        clear_bit(heap_free_list->free_list,firstfreebit+i);
    }

    // they are now free so send the address back to the caller
    return blockaddress;

}

// intitalize the free list
void initFreePageList() {
    // set up the free list 
    heap_free_list->first_free = 0; // we start full
    heap_free_list->heap_start = kernel_heap_start;

    // I will let the optimizer make this less clean
    uint32_t heap_size = kernel_heap_end - kernel_heap_start;
    uint32_t heap_pages = heap_size / PAGE_SIZE;
    uint32_t free_list_entries = heap_pages / 8; // 8 bits per entry

    // load array
    for (uint32_t i = 0; i < free_list_entries; i++) {
        heap_free_list->free_list[i] = 0xFF;
    }

    uint32_t leftoverpages = (heap_size - (free_list_entries*8*PAGE_SIZE));
    if (leftoverpages) {
        // I have at least 1
        heap_free_list->free_list[free_list_entries + 1] = 1;

    }
    numHeapPages = (free_list_entries * 8) + leftoverpages;
}
// initialize heap structures
void initHeap() {

    initFreePageList();

    for (int i = 0; i < HEAD_64K; i++) {
        heap_head[i].alloc = 0;
        heap_head[i].free = 0;
    }
    
}
