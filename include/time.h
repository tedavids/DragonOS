// time functions

#ifndef _TIME_H
#define _TIME_H

#include <stdint.h>

typedef struct {
    uint8_t     second;
    uint8_t     minute;
    uint8_t     hour;
    uint8_t     day;
    uint8_t     month;
    uint16_t    year;
} rtime_t;

extern unsigned int bcd_to_bin(unsigned int bcd);
extern rtime_t read_system_clock();
extern uint64_t rdtsc(); // read the time stamp counter
#endif