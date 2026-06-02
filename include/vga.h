//vga.h these are base VGA functions

#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROW = 14,
    VGA_COLOR_WHITE = 15,
    VGA_COLOR_END 
}; // enum vga_color

struct cursor_pos_t {
    size_t row;
    size_t col;
};

struct vga_cursor_t {
    uint8_t firstscanline;
    uint8_t lastscanline;
};

extern struct vga_cursor_t VGA_CURSOR_BLOCK;
extern struct vga_cursor_t VGA_CURSOR_UNDERLINE;
extern struct vga_cursor_t VGA_CURSOR_THICK_UNDERLINE;

extern struct cursor_pos_t cursorpos;

static const  size_t VGA_80X25_ROWS = 25;
static const size_t VGA_80X25_COLUMNS = 80;


static const size_t VGA_CURSOR_MAX_LINEAL_ADDRESS = 2000;
static const uint16_t VGA_CURSOR_ERROR = 65535;

// fuctions

// set the cursor position for VGA 80x25 text mode

// inputs:  row -- the row the cursor should be put on
//          col -- the col the cursor should be put on

// returns: true -- if row and column are valid
//          false -- if either is out of bounds
extern bool set80x25cursorPosXY(struct cursor_pos_t* cursor, int row, int col);
extern bool set80x25cursorPos(struct cursor_pos_t* cursorpos);
extern bool get80x25cursorPos(struct cursor_pos_t* cursorpos);

// move cursor

// inputs:  cursor          a pointer to the cursor
//          alllowscroll    allow a scroll

// returns: bool            true if we need a scroll false otherwise (could be scroll up or down)

extern bool move80x25CursorUp(struct cursor_pos_t* cursor, bool allowscroll);
extern bool move80x25CursorDown(struct cursor_pos_t* cursor, bool allowscroll);
extern bool move80x25CursorLeft(struct cursor_pos_t* cursor, bool allowscroll);
extern bool move80x25CursorRight(struct cursor_pos_t* cursor, bool allowscroll);

// validate the cursor

// inputs:  cursor  a pointer to the cursor to validate
//          fix     do we fix the cursor 

// returns: bool    true if the cursor is valid (may be valid becaue its been fixed)
//                  false if the cursor is not valid and cant be fixed

extern bool validate80x25Cursor(struct cursor_pos_t* cursor, bool fix);

// enable disable cursor
extern bool enableVGACursor(struct vga_cursor_t type);
extern void disableVGACursor();

extern uint8_t crtVGATextColor( enum vga_color fg, enum vga_color bg);
extern uint16_t crtVGAText(unsigned char uc, uint8_t color);
extern uint16_t cursorpostolineal(struct cursor_pos_t cursorpos);
extern struct cursor_pos_t linealtocursorpos(uint16_t lineal);

#endif
