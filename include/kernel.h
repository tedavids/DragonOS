// kernel.h  basic info on the kernel

#ifndef _KERNEL_H
#define _KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define KERNEL_VERSION      "0.000"

extern const char*              OSNAME;
extern const char*              OSVERSION;
extern const char*              OSDATE;
extern uint32_t                 BOOTSTATUS;             // status of boot tasks


// public functions

// allocation functions

// kmalloc -- allocate memory

// inputs: size -- the number of bytes you wish to allocate

// return: a void pointer to the memory block, or nullptr if it fails

extern void* kmalloc(size_t size);

// kfree -- free memory gotten from kmalloc

// inputs:  the void pointer returned by kmalloc

extern void kfree(void* ptr);

// getSelectorText -- get the text string for a given GDT selector
// inputs:  selector -- the selector you wish to print
//          buffer -- a buffer to return the string in
//          size -- the size of the buffer

// return:  nullptr if the buffer is null pointer
//          nullptr if the buffer is too small to hold the result
extern char* getSelectorText(int selector, char* buffer, size_t size);

// get data from a port
// inputs:  port    -- the port number you wish to get data from

// returns: unsigned char -- the data from the port

extern unsigned char getPortByte(uint16_t port);
extern uint16_t getPortWord(uint16_t port);
extern uint32_t getPortLong(uint16_t port);

// send data to a port

// inputs:  port -- the port number you wish to put data to
//          data -- the data you wish to put on the port

extern void putPortByte(uint16_t port, unsigned char data);
extern void putPortWord(uint16_t port, uint16_t data);
extern void putPortLong(uint16_t port, uint32_t data);

// reboot the system
extern void reboot();


#endif