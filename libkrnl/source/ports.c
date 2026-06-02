// ports.c support for ports 

#include <kernel.h>


unsigned char getPortByte(uint16_t port) {
        unsigned char data;
        __asm__ ("in %%dx, %%al" : "=a" (data) : "d" (port));
        return data;
}

void putPortByte(uint16_t port, unsigned char data) {
    __asm__ ("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t getPortWord(uint16_t port) {
        unsigned char data;
        __asm__ ("in %%dx, %%ax" : "=a" (data) : "d" (port));
        return data;
}

void putPortWord(uint16_t port, uint16_t data) {
    __asm__ ("out %%ax, %%dx" : : "a" (data), "d" (port));
}

uint32_t getPortLong(uint16_t port) {
        unsigned char data;
        __asm__ ("in %%dx, %%eax" : "=a" (data) : "d" (port));
        return data;
}

void putPortLong(uint16_t port, uint32_t data) {
    __asm__ ("out %%eax, %%dx" : : "a" (data), "d" (port));
}