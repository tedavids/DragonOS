// vga.c  VGA specific functions

// tty.c will be modified to use them
#include <kernel.h>
#include <stdio.h>      // for debugging

#include "vga.h"

// defines

#define VGA_CTRL_REGISTER           0x3D4
#define VGA_DATA_REGISTER           0x3D5
#define VGA_CTRL_DATA_REGISTER_LOW  0x0F
#define VGA_CTRL_DATA_REGISTER_HIGH 0x0E
#define VGA_CURSOR_ON_OFF           0x0A
#define VGA_CURSOR_FIRST_LINE       0x0A
#define VGA_CURSOR_LAST_LINE        0x0B
#define VGA_CURSOR_OFF              0x20


struct vga_cursor_t VGA_CURSOR_BLOCK = {0,15};
struct vga_cursor_t VGA_CURSOR_UNDERLINE = {0,0};
struct vga_cursor_t VGA_CURSOR_THICK_UNDERLINE = {0,2};

struct cursor_pos_t cursorpos = {1,1};

// get the color of a vga color as a byte

// inputs:  fg -- the color of the text
//          bg -- the background color

// returns: the color as a byte

inline uint8_t crtVGATextColor( enum vga_color fg, enum vga_color bg) 
{
    return (uint8_t) (fg | bg << 4);
}

// get the entry as it will be presented to the VGA adaptor

// inputs:  uc -- the character to print
//          color -- the foreground and background colors to use (from vga_entry_color)

// returns: a 16-bit value usable as a VGA buffer entry

inline uint16_t crtVGAText(unsigned char uc, uint8_t color) 
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

uint16_t cursorpostolineal(struct cursor_pos_t cursorpos) {
    // valid parameters
    if (!validate80x25Cursor(&cursorpos, false)) return VGA_CURSOR_ERROR;

    uint16_t lineal = (uint16_t) (((cursorpos.row-1) * (VGA_80X25_COLUMNS)) + (cursorpos.col-1));

    if (lineal > VGA_CURSOR_MAX_LINEAL_ADDRESS) return VGA_CURSOR_ERROR;

    return lineal;
}

struct cursor_pos_t linealtocursorpos(uint16_t lineal) {

    struct cursor_pos_t cursorpos;
    cursorpos.row = (lineal / 80) + 1;
    cursorpos.col = (lineal % 80)+1;

    validate80x25Cursor(&cursorpos,true);

    return cursorpos;
}

bool set80x25cursorPosXY(struct cursor_pos_t* cursor, int row, int col) {
    cursor->row = (uint32_t)row;
    cursor->col = (uint32_t)col;
    return set80x25cursorPos(cursor);
}

bool set80x25cursorPos(struct cursor_pos_t *cursorpos) {
    // valid parameters
    validate80x25Cursor(cursorpos,true);

    uint16_t offset = cursorpostolineal(*cursorpos);

    if (offset == VGA_CURSOR_MAX_LINEAL_ADDRESS) 
        printf("Error in set80x25cursorPos, invalid cursorpos\n\r");
    // set low byte
    putPortByte(VGA_CTRL_REGISTER, VGA_CTRL_DATA_REGISTER_LOW);
    putPortByte(VGA_DATA_REGISTER, (unsigned char) (offset & 0x00FF));
    // set high byte
    putPortByte(VGA_CTRL_REGISTER, VGA_CTRL_DATA_REGISTER_HIGH);
    putPortByte(VGA_DATA_REGISTER, (unsigned char) ((offset & 0xFF00) >> 8));

    return true;
}

bool get80x25cursorPos(struct cursor_pos_t* cursorpos) {

    uint16_t offset = 0;

    // get low byte
    putPortByte(VGA_CTRL_REGISTER, VGA_CTRL_DATA_REGISTER_LOW);
    offset |= getPortByte(VGA_DATA_REGISTER);
    // get high byte
    putPortByte(VGA_CTRL_REGISTER, VGA_CTRL_DATA_REGISTER_HIGH);
    offset |= (getPortByte(VGA_DATA_REGISTER)) << 8;

    // convert to row,col
    *cursorpos = linealtocursorpos(offset);

    return (cursorpos->row != 0);

}

bool enableVGACursor(struct vga_cursor_t type) {
    // validate parameters
     if ((type.firstscanline > 15) || (type.firstscanline > type.lastscanline)) return false;
    if (type.lastscanline > 15) return false;

    putPortByte(VGA_CTRL_REGISTER,VGA_CURSOR_FIRST_LINE);
    putPortByte(VGA_DATA_REGISTER, type.firstscanline);

    putPortByte(VGA_CTRL_REGISTER, VGA_CURSOR_LAST_LINE);
    putPortByte(VGA_DATA_REGISTER, type.lastscanline);

    return true;
}

void disableVGACursor() {
    putPortByte(VGA_CTRL_REGISTER, VGA_CURSOR_ON_OFF);
    putPortByte(VGA_DATA_REGISTER, VGA_CURSOR_OFF);
}

bool validate80x25Cursor(struct cursor_pos_t* cursor, bool fix) {
    if (cursor->row < 1) {
        if (!fix) return false;
        cursor->row = 1;
    }

    if (cursor->row > VGA_80X25_ROWS) {
        if (!fix) return false;
        cursor->row = VGA_80X25_ROWS;
    }

    if (cursor->col < 1) {
        if (!fix) return false;
        cursor->col = 1;
    }

    if (cursor->col > VGA_80X25_COLUMNS) {
        if (!fix) return false;
        cursor->col = VGA_80X25_COLUMNS;
    }

    return true;
}

bool move80x25CursorUp(struct cursor_pos_t* cursor, bool allowscroll) {
    // validate curser is already valid, fix if necessary
    validate80x25Cursor(cursor, true);
    
    cursor->row--;

    if (cursor->row < 1) {
        if (allowscroll) {
            cursor->row = 1;
            set80x25cursorPos(cursor);
            return true;
        } else {
            cursor->row = 1;
            set80x25cursorPos(cursor);
            return false;
        }
    }
    set80x25cursorPos(cursor);
    return false;
}
bool move80x25CursorDown(struct cursor_pos_t* cursor, bool allowscroll){
    // validate curser is already valid, fix if necessary
    validate80x25Cursor(cursor, true);
    
    cursor->row++;

    if (cursor->row > VGA_80X25_ROWS) {
        if (allowscroll) {
            cursor->row = VGA_80X25_ROWS;
            set80x25cursorPos(cursor);
            return true;
        } else {
            cursor->row = VGA_80X25_ROWS;
            set80x25cursorPos(cursor);
            return false;
        }
    }
    set80x25cursorPos(cursor);
    return false;
}

bool move80x25CursorLeft(struct cursor_pos_t* cursor, bool allowscroll) {
    // validate curser is already valid, fix if necessary
    validate80x25Cursor(cursor, true);

    cursor->col--;

    if (cursor->col < 1) {
        if (allowscroll) {
            cursor->col = VGA_80X25_COLUMNS;
            set80x25cursorPos(cursor);
            return move80x25CursorUp(cursor, allowscroll);
        } else {
            cursor->col = 1;
            set80x25cursorPos(cursor);
            return false;
        }
    }
    set80x25cursorPos(cursor);
    return false;
}

bool move80x25CursorRight(struct cursor_pos_t* cursor, bool allowscroll) {
    // validate cursor is already valid, fix if necessary
    validate80x25Cursor(cursor, true);

    cursor->col++;
    if (cursor->col > VGA_80X25_COLUMNS) {
        if (allowscroll) {
            cursor->col = 1;
            set80x25cursorPos(cursor);
            return move80x25CursorUp(cursor, allowscroll);
        } else {
            cursor->col = 1;
            set80x25cursorPos(cursor);
            return true;
        }
    }
    set80x25cursorPos(cursor);
    return false;
}