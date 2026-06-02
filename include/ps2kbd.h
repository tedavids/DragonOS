// ps2kbd.c -- functions available for our keyboard handler

#ifndef _PS2KBD_H_
#define _PS2KBD_H_

#include <stdint.h>

#define KBD_EXTENDED_KEYCODE        0b0000000100000000
#define KBD_ALT_PRESSED             0b0000001000000000
#define KBD_CTRL_PRESSED            0b0000010000000000

// define keys
#define KBD_KEY_ESC                 0x1B

// init the keyboard

#ifndef _PS2KBD_C
extern bool initKbd();
extern bool kbdBufferEmpty();
extern bool kbdBufferFull();
extern uint16_t kbdGetKey();
extern uint16_t kbdPeekKey();

#endif

#endif