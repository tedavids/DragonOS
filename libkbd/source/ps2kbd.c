// ps2kbd.c -- PS2 keyboard functions

#define _PS2KBD_C

#include <stdint.h>
#include <apic.h>
#include <gdt.h>
#include <idt.h>
#include <interupt.h>

#include <stdio.h>

#include <kernel.h>
#include <ps2kbd.h>



#define KBD_STATUS_PORT         0x64
#define KBD_DATA_PORT           0x60

#define KBD_STATUS_OUTBUF_HAS_DATA  0b00000001
#define KBD_STATUS_INBUF_HAS_DATA   0b00000010
#define KBD_STATUS_POST_PASSED      0b00000100
#define KBD_STATUS_CMD_DATA         0b00001000
#define KBD_STATUS_KBD_LOCKED       0b00010000
#define KBD_STATUS_AUXOUT_MOUSE_DTA 0b00100000
#define KBD_STATUS_TIMEOUT          0b01000000
#define KBD_STATUS_ODD_PARITY       0b10000000

#define KBD_ERROR_COUNTDOWN         5
#define KBD_MAX_ERRORS              10

#define KBD_BUFFER_SIZE             32

extern void kbd_handler();

static uint16_t kbdBuffer[KBD_BUFFER_SIZE];
static uint8_t kbdHead = 0;
static uint8_t kbdTail = 0;

static bool ctrlLeft = false;
static bool ctrlRight = false;
static bool ctrl = false;

static bool altLeft = false;
static bool altRight = false;
static bool alt = false;

static bool capsLock = false;
static bool shiftLeft = false;
static bool shiftRight = false;
static bool shift = false;

static bool scrollLock = false;
static bool numLock = false;

static int errorCount = 0;
static int errorCountdown = 0;
static bool keyboardDisabled = false;

bool kbdBufferEmpty() {
    return (kbdHead == kbdTail);
}

bool kbdBufferFull() {
    if (kbdHead > kbdTail) {
        return ((kbdHead -1 ) == kbdTail);
    } else {
        // handle wrap
        return ((kbdHead + KBD_BUFFER_SIZE - 1) == kbdTail );
    }
}

uint16_t kbdGetKey() {
    // check if empty
    if (kbdBufferEmpty()) return 0;
    uint16_t entry = kbdBuffer[kbdTail++];

    if (kbdTail >= KBD_BUFFER_SIZE) kbdTail = 0;

    return entry;
}

uint16_t kbdPeekKey() {
    // check if buffer is empty
    if (kbdBufferEmpty()) return 0;

    return kbdBuffer[kbdTail];
}

inline static void setShift() {
    shift = ((shiftLeft || shiftRight) ^ capsLock);
}

inline static void setCtrl() {
    ctrl = (ctrlLeft || ctrlRight);
}

inline static void setAlt() {
    alt = (altLeft || altRight);
}



// handle keyboard errors
inline static void handleKbdError() {
    errorCount++;

    if (errorCount > KBD_MAX_ERRORS) keyboardDisabled = true;

    errorCountdown = KBD_ERROR_COUNTDOWN;   // reset countdown
}

// decrement the count (call on every non error keypress)
inline static void decrementErrorCount() {
    if (errorCount) {
        errorCount--;
    }
}

void putKeypressInBuffer(uint8_t key, bool extended) {
    // only put it there if the buffer isn't full
    if (!kbdBufferFull()) {
        // save key
        kbdBuffer[kbdHead] = key;
        // set flags
        if (alt) kbdBuffer[kbdHead] |= KBD_ALT_PRESSED;
        if (ctrl) kbdBuffer[kbdHead] |= KBD_CTRL_PRESSED;
        if (extended) kbdBuffer[kbdHead] |= KBD_EXTENDED_KEYCODE;
        // move head
        kbdHead++;
        // are we rolling back to the start
        if (kbdHead >= KBD_BUFFER_SIZE) kbdHead = 0;
    } else {
        // TODO: beep
    }
}

void processKeypress(uint8_t keycode) {
    switch (keycode) {
        case 0x00:  // keyboard error
            handleKbdError();
            break;
        case 0x01: // escape
            decrementErrorCount();
            putKeypressInBuffer(0x1B, false);
            break;
        case 0x02: // 1
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('!', false);
            } else {
                putKeypressInBuffer('1', false);
            }
            break;
        case 0x03: // 2
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('@', false);
            } else {
                putKeypressInBuffer('2', false);
            }
            break;
        case 0x04: // 3
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('#', false);
            } else {
                putKeypressInBuffer('3', false);
            }
            break;
        case 0x05: // 4
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('$', false);
            } else {
                putKeypressInBuffer('4', false);
            }
            break;
        case 0x06: // 5
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('%', false);
            } else {
                putKeypressInBuffer('5', false);
            }
            break;
        case 0x07: // 6
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('^',false);
            } else {
                putKeypressInBuffer('6',false);
            }
            break;
        case 0x08: // 7
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('&',false);
            } else {
                putKeypressInBuffer('7',false);
            }
            break;
        case 0x09: // 8
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('*',false);
            } else {
                putKeypressInBuffer('8',false);
            }
            break;
        case 0x0A: // 9
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('(',false);
            } else {
                putKeypressInBuffer('9',false);
            }
            break;
        case 0x0B: // 0 (zero)
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(')',false);
            } else {
                putKeypressInBuffer('0',false);
            }
            break;
        case 0x0C: // - (minus)
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('_',false);
            } else {
                putKeypressInBuffer('-', false);
            }
            break;
        case 0x0D: // =
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('+',false);
            } else {
                putKeypressInBuffer('=',false);
            }
            break;
        case 0x0E: // backspace
            decrementErrorCount();
            putKeypressInBuffer(0x08,false);
            break;
        case 0x0F: // tab
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x0B,false);
            } else {
                putKeypressInBuffer(0x09,false);
            }
            break;
        case 0x10: // Q
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('Q',false);
            } else {
                putKeypressInBuffer('q',false);
            }
            break;
        case 0x11: // W
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('W',false);
            } else {
                putKeypressInBuffer('w',false);
            }
            break;
        case 0x12: // E
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('E',false);
            } else {
                putKeypressInBuffer('e',false);
            }
            break;
        case 0x13: // R
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('R',false);
            } else {
                putKeypressInBuffer('r',false);
            }
            break;
        case 0x14: // T
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('T',false);
            } else {
                putKeypressInBuffer('t',false);
            }
            break;
        case 0x15: // Y
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('Y', false);
            } else {
                putKeypressInBuffer('y',false);
            }
            break;
        case 0x16: // U
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('U',false);
            } else {
                putKeypressInBuffer('u',false);
            }
            break;
        case 0x17: // I
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('I',false);
            } else {
                putKeypressInBuffer('i',false);
            }
            break;
        case 0x18: // O (letter o)
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('O',false);
            } else {
                putKeypressInBuffer('o',false);
            }
            break;
        case 0x19: // P
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('P',false);
            } else {
                putKeypressInBuffer('p',false);
            }
            break;
        case 0x1A: // [
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('{',false);
            } else {
                putKeypressInBuffer('[',false);
            }
            break;
        case 0x1B: // ]
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('}',false);
            } else {
                putKeypressInBuffer(']',false);
            }
            break;
        case 0x1C: // enter
            decrementErrorCount();
            putKeypressInBuffer('\n',false);
            break;
        case 0x1D: // left ctrt
            decrementErrorCount();
            ctrlLeft = true;
            setCtrl();
            break;
        case 0x1E: // A
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('A',false);
            } else {
                putKeypressInBuffer('a',false);
            }
            break;
        case 0x1F: // S
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('S',false);
            } else {
                putKeypressInBuffer('s',false);
            }
            break;
        case 0x20: // D
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('D',false);
            } else {
                putKeypressInBuffer('d',false);
            }
            break;
        case 0x21: // F
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('F',false);
            } else {
                putKeypressInBuffer('f',false);
            }
            break;
        case 0x22: // G
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('G',false);
            } else {
                putKeypressInBuffer('g',false);
            }
            break;
        case 0x23: // H
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('H',false);
            } else {
                putKeypressInBuffer('h',false);
            }
            break;
        case 0x24: // J
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('J',false);
            } else {
                putKeypressInBuffer('j',false);
            }
            break;
        case 0x25: // K
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('K',false);
            } else {
                putKeypressInBuffer('k',false);
            }
            break;
        case 0x26: // L
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('L',false);
            } else {
                putKeypressInBuffer('l',false);
            }
            break;
        case 0x27: // ;
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(':',false);
            } else {
                putKeypressInBuffer(';',false);
            }
            break;
        case 0x28: // ' (single quote)
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('"', false);
            } else {
                putKeypressInBuffer('\'',false);
            }
            break;
        case 0x29: // ` (back tic)
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('~',false);
            } else {
                putKeypressInBuffer('`',false);
            }
            break;
        case 0x2A: // left shift
            decrementErrorCount();
            shiftLeft = true;
            setShift();
            break;
        case 0x2B: // backslash
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('|',false);
            } else {
                putKeypressInBuffer(0x5C,false);
            }
            break;
        case 0x2C: // Z
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('Z',false);
            } else {
                putKeypressInBuffer('z',false);
            }
            break;
        case 0x2D: // X
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('X',false);
            } else {
                putKeypressInBuffer('x',false);
            }
            break;
        case 0x2E: // C
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('C',false);
            } else {
                putKeypressInBuffer('c',false);
            }
            break;
        case 0x2F: // V
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('V',false);
            } else {
                putKeypressInBuffer('v',false);
            }
            break;
        case 0x30: // B
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('B',false);
            } else {
                putKeypressInBuffer('b',false);
            }
            break;
        case 0x31: // N
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('N',false);
            } else {
                putKeypressInBuffer('n',false);
            }
            break;
        case 0x32: // M
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('M',false);
            } else {
                putKeypressInBuffer('m',false);
            }
            break;
        case 0x33: // ,
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('<',false);
            } else {
                putKeypressInBuffer(',',false);
            }
            break;
        case 0x34: // .
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('>',false);
            } else {
                putKeypressInBuffer('.',false);
            }
            break;
        case 0x35: // slash
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer('?', false);
            } else {
                putKeypressInBuffer('/',false);
            }
            break;
        case 0x36: // right shift pressed
            decrementErrorCount();
            shiftRight = true;
            setShift();
            break;
        case 0x37: // keypad *
            decrementErrorCount();
            putKeypressInBuffer('*',false);
            break;
        case 0x38: // left alt pressed
            decrementErrorCount();
            altLeft = true;
            setAlt();
            break;
        case 0x39: // space bar
            decrementErrorCount();
            putKeypressInBuffer(' ',false);
            break;
        case 0x3A: // caps lock pressed
            decrementErrorCount();
            capsLock = !capsLock;
            setShift();
            break;
        case 0x3B: // F1
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x8C,false); // F13
            } else {
                putKeypressInBuffer(0x80,false); // F1
            }
            break;
        case 0x3C: // F2
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x8D,false); // F14
            } else {
                putKeypressInBuffer(0x81, false); // F2
            }
            break;
        case 0x3D: // F3
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x8E,false); // F15
            }
            putKeypressInBuffer(0x82,false); // F3
            break;
        case 0x3E: // F4
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x8F,false); // F16
            } else {
                putKeypressInBuffer(0x83, false); // F4
            }
            break;
        case 0x3F: // F5
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x90,false); // F17
            }
                putKeypressInBuffer(0x84,false); // F5
            break;
        case 0x40: // F6
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x91,false); // F18
            } else {
                putKeypressInBuffer(0x85, false); // F6
            }
            break;
        case 0x41: // F7
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x92,false); // F19
            } else {
                putKeypressInBuffer(0x86,false); // F7
            }
            break;
        case 0x42: // F8
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x93,false); // F20
            } else {
                putKeypressInBuffer(0x87, false); // F8
            }
            break;
        case 0x43: // F9
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x94,false); // F21
            } else {
                putKeypressInBuffer(0x88, false); // F9
            }
            break;
        case 0x44: // F10
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x95,false); // F22
            } else {
                putKeypressInBuffer(0x89,false); // F10
            }
            break;
        case 0x45: // num locked pressed
            decrementErrorCount();
            numLock = !numLock;
            break;
        case 0x46: // scroll lock pressed
            decrementErrorCount();
            scrollLock = !scrollLock;
            break;
        case 0x47: // keypad 7
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('7',false);
            } else {
                putKeypressInBuffer(0x9C,false); // home
            }
            break;
        case 0x48: // keypad 8
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('8',false);
            } else {
                putKeypressInBuffer(0x98,false); // arrow-up
            }
            break;
        case 0x49: // keypad 9
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('9',false);
            } else {
                putKeypressInBuffer(0x9D,false); // page up
            }
            break;
        case 0x4A: // keypad -
            decrementErrorCount();
            putKeypressInBuffer('-',false);
            break;
        case 0x4B: // keypad 4
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('4',false);
            } else {
                putKeypressInBuffer(0x9b,false); // arrow left
            }
            break;
        case 0x4C: // keypad 5
            decrementErrorCount();
            if(numLock) {
                putKeypressInBuffer('5',false);
            }
            break;
        case 0x4D: // keypad 6
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('6', false);
            } else {
                putKeypressInBuffer(0x9A,false); // arrow right
            }
            break;
        case 0x4E: // keypad +
            decrementErrorCount();
            putKeypressInBuffer('+',false);
            break;
        case 0x4F: // keypad 1
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('1',false);
            } else {
                putKeypressInBuffer(0x9F,false); // End
            }
            break;
        case 0x50: // keypad 2
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('2',false);
            } else {
                putKeypressInBuffer(0x99,false); // arrow down
            }
            break;
        case 0x51: // keypad 3
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('3',false);
            } else {
                putKeypressInBuffer(0x9E,false); // page down
            }
            break;
        case 0x52: // keypad 0
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('0',false);
            } else {
                putKeypressInBuffer(0xA3,false);
            }
            break;
        case 0x53: // keypad .
            decrementErrorCount();
            if (numLock) {
                putKeypressInBuffer('.',false);
            } else {
                putKeypressInBuffer(0x7F,false); // ascii DEL
            }
            break;
        case 0x57: // F11
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x96,false); // F23
            } else {
                putKeypressInBuffer(0x8A,false); // F11
            }
            break;
        case 0x58: // F12
            decrementErrorCount();
            if (shift) {
                putKeypressInBuffer(0x97,false); // F24
            } else {
                putKeypressInBuffer(0x8B,false);
            }
            break;
        case 0x9D: // left control released
            decrementErrorCount();
            ctrlLeft = false;
            setAlt();
            break;
        case 0xAA: // left shift released
            shiftLeft = false;
            setShift();
            break;
        case 0XB6: // right shift released
            shiftRight = false;
            setShift();
            break;
        case 0xB8: // left alt released
            altLeft = false;
            setAlt();
            break;
        case 0xE0: // extended keys
            // check that we have a key
            auto status = getPortByte(KBD_STATUS_PORT);
            if (status & KBD_STATUS_OUTBUF_HAS_DATA) {
                uint8_t extcode = (uint8_t) getPortByte(KBD_DATA_PORT);
                // process keycode
                switch (extcode) {
                case 0x1C: // keypad enter
                    putKeypressInBuffer('\n',false);
                    break;
                case 0x1D: // right control pressed
                    // complex one if we have yet another byte, its a pause
                    // otherwise its a right control pressed
                    auto status1D = getPortByte(KBD_STATUS_PORT);
                    if (status1D & KBD_STATUS_OUTBUF_HAS_DATA) {
                        if (shift) {
                            putKeypressInBuffer(0xA2,false); // break
                        } else {
                            putKeypressInBuffer(0xA1,false); // pause
                        }
                    } else {
                       ctrlRight = true;
                       setCtrl();
                    }
                    break;
                case 0x2A: // print screen
                    if (shift) {
                        putKeypressInBuffer(0xA4,false); // sys req
                    } else {
                        putKeypressInBuffer(0xA4,false); // print screen
                    }
                    break;
                case 0x35: // keypad /
                    putKeypressInBuffer('/',false);
                    break;
                case 0x38: // right alt pressed
                    altRight = true;
                    setAlt();
                    break;
                case 0x47: // home
                    putKeypressInBuffer(0x9C,false);
                    break;
                case 0x48: // arrow up
                    putKeypressInBuffer(0x98,false); // arrow up
                    break;
                case 0x49: // page up
                    putKeypressInBuffer(0x9D,false);
                    break;
                case 0x4B: // left arrow
                    putKeypressInBuffer(0x9B,false);
                    break;
                case 0x4D: // right arrow
                    putKeypressInBuffer(0x9A,false);
                    break;
                case 0x4F: // end
                    putKeypressInBuffer(0x9F,false);
                    break;
                case 0x50: // down arrow
                    putKeypressInBuffer(0x99,false);
                    break;
                case 0x51: // page down
                    putKeypressInBuffer(0x9E,false);
                    break;
                case 0x52: // insert
                    putKeypressInBuffer(0xA3,false);
                    break;
                case 0x53: // delete
                    putKeypressInBuffer(0x7F,false);
                    break;
                case 0x9D: // right ctl released
                    ctrlRight = false;
                    setCtrl();
                    break;
                case 0x8B: // right alt released
                    altRight = false;
                    setAlt();
                    break;
                default: // ignore
                    break;
                }
            }
    default:    // ignore
        break;
    }
}

void ps2kbd_handler(struct interupt_frame_t *parms) {
    parms->erroraddress = parms->erroraddress;
    auto status = getPortByte(KBD_STATUS_PORT);
    if (status & KBD_STATUS_OUTBUF_HAS_DATA) {
        uint8_t keycode = (uint8_t) getPortByte(KBD_DATA_PORT);
        // if we get too many errors, we disable the keyboard
        if (!keyboardDisabled) {
            processKeypress(keycode);
        }
    }
    putPortByte(APIC_PIC1_COMMAND_PORT, APIC_END_OF_INTERRUPT);
}

bool initKbd() {
    // add idt entry to table

    if (!idt_set_gate(IDT_KEYBORD_INTERRUPT, (uint32_t) &kbd_handler,  KERNEL_CODE_SELECTOR, 
                 IDT_PRIVLEDGE_KERNEL, IDT_GATE_TYPE_INTERRUPT32, false)) {
        printf("Failed to set keyboard interrupt\r\n");
        return false;
    }

    // unmask the interrupt
    uint8_t byte = getPortByte(APIC_PIC1_DATA_PORT);
    byte &= 0xFD; // 0b1111 1101 the zero is the second port (21)

    // PIC1 starts at interrupt 20, the keyboard is interrupt 21
    putPortByte(APIC_PIC1_DATA_PORT, byte); 

    return true;
}