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

// converts a Binary Coded Decimal (BDD) number to binary

// Parameters:  bcd -- The number in BCD format

// Returns:     an integer value for the value
extern unsigned int bcd_to_bin(unsigned int bcd);

// Read the system clock and return the time

// Parameters:  None

// Returns:     The time in rtime_t format
extern rtime_t read_system_clock();

// execute the rdtsc machine instruction

// Parameters:  None

// Returns:     the timestamp counter
extern uint64_t rdtsc(); // read the time stamp counter

#endif