// time and timer functions
#include <kernel.h>

#include <time.h>

#define CMOS_TIME_PORT   0x70
#define CMOS_DATA_PORT   0x71

#define CMOS_GET_FLAGS      0xA
#define RTC_SECOND          0x00
#define RTC_MINUITE         0x02
#define RTC_HOUR            0x04
#define RTC_DAY             0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

// get flag that says time is being updated
int get_update_in_progress_flag() {
    putPortByte(CMOS_TIME_PORT, CMOS_GET_FLAGS);
    auto result = getPortByte(CMOS_DATA_PORT) & 0x80;
    return result;
}

// read the real time clock register
uint8_t read_rtc_register(int reg) {
    putPortByte(CMOS_TIME_PORT, (unsigned char) reg);
    return getPortByte(CMOS_DATA_PORT);
}

unsigned int bcd_to_bin(unsigned int bcd) {
    unsigned int bin = 0;
    unsigned int multiplier = 1;

    while (bcd > 0) {
        // Extract the lowest 4 bits (one BCD digit)
        unsigned int digit = bcd & 0x0F;
        
        // Add weighted value to result
        bin += digit * multiplier;
        
        // Prepare for next digit
        multiplier *= 10;
        bcd >>= 4;
    }
    return bin;
}

rtime_t read_system_clock() {
    // this is where we keep our time
    rtime_t time;

    // wait for update to finish
    while (get_update_in_progress_flag());

    // get the time
    time.second = read_rtc_register(RTC_SECOND);
    time.minute = read_rtc_register(RTC_MINUITE);
    time.hour = read_rtc_register(RTC_HOUR);
    time.day = read_rtc_register(RTC_DAY);
    time.month = read_rtc_register(RTC_MONTH);
    time.year = read_rtc_register(RTC_YEAR);

    uint8_t register_b = read_rtc_register(0x0B);

    // convert bcd
    if (!(register_b & 0x04)) {
        time.second = (uint8_t) bcd_to_bin((unsigned int) time.second);
        time.minute = (uint8_t) bcd_to_bin((unsigned int) time.minute);
        time.hour = (uint8_t) bcd_to_bin((unsigned int) time.hour);
        time.day = (uint8_t) bcd_to_bin((unsigned int) time.day);
        time.month = (uint8_t) bcd_to_bin((unsigned int) time.month);
        time.year = (uint16_t) bcd_to_bin((unsigned int) time.year);
    }

    time.year += 2000;

    return time;
}

uint64_t rdtsc() {
    unsigned int low, high;
    __asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
    uint64_t low64 = (uint64_t) low;
    uint64_t high64 = (uint64_t) high << 32;
    return (high64 | low64);
}
