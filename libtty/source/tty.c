// tty.c  These are our initial screen functions used at boot time

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <tty.h>

#include <vga.h>


static uint16_t* VGA_MEMORY = (uint16_t*) (uint16_t*) 0xB8000; // 0xc03ff000;


static enum vga_color terminal_color;
static uint16_t* terminal_buffer;

bool terminal_initialize(void) 
{
    terminal_color = crtVGATextColor(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer =  VGA_MEMORY;

    bool rtncde = terminal_clear_screen();

    if (rtncde) {
        rtncde &= enableVGACursor(VGA_CURSOR_BLOCK);
        if (rtncde) {
            rtncde &=  set80x25cursorPos(&cursorpos);
        }
    }
    return rtncde;
} // void terminal_initialize(void)

bool terminal_clear_line(size_t line) {
    // check line is in reange
    if (line > VGA_80X25_ROWS) return false;

    // change for use in offset
    line -= 1;

    size_t rowoffset = line * VGA_80X25_COLUMNS;
    for (size_t col = 0; col < VGA_80X25_COLUMNS; col++) {
        terminal_buffer[rowoffset++] = crtVGAText(' ',terminal_color);
    }

    return true;
}

bool terminal_clear_rows(size_t first, size_t last) {
    // make sure we are in range
    if ((first > VGA_80X25_ROWS) || (first > last) || (last > VGA_80X25_ROWS)) {
        return false;
    }
    // clear lines
    for (size_t row = first; row <= last; row++)
        if (!terminal_clear_line(row)) {
            return false;
        }
     
    return true;
}

bool terminal_clear_screen() {
    // put cursor at top of screen
    cursorpos.row = 1;
    cursorpos.col = 1;
    set80x25cursorPos(&cursorpos);
    // clear the screen
    return terminal_clear_rows(1,VGA_80X25_ROWS);
}

void terminal_setcolor(uint8_t color) 
{
    terminal_color = color;
} // void terminal_setcolor(uint8_t color)

bool terminal_putentryat(unsigned char c, uint8_t color, struct cursor_pos_t* cursorpos)
{
    // validate parameters, fix if necessary
    validate80x25Cursor(cursorpos, true);

    // write the character
    terminal_buffer[cursorpostolineal(*cursorpos)] = crtVGAText(c, color);
    // keep cursor the right place
    return move80x25CursorRight(cursorpos, true);

} // bool terminal_putentryat(unsigned char c, uint8_color, struct cursor_pos_t* cursorpos)

void terminal_scroll_line(size_t line) 
{   // line has to be at least 1
    if (line < 1) return;
    // can't be line 25
    if (line > 24) return;

    // get the two rows involved, also accounts for row being 1 based and offsets beign 0 based
    size_t srcrow = line -1;
    size_t destrow = srcrow - 1;

    for (size_t col = 0; col < VGA_80X25_COLUMNS; col++) {
        terminal_buffer[(destrow*VGA_80X25_COLUMNS) + col] =
            terminal_buffer[(srcrow*VGA_80X25_COLUMNS)+col];
    }
    //terminal_clear_line(destrow + 1);
    return;
} // void terminal_scroll_line(int line) 

void terminal_clear_last_line() {
    int row = (int) VGA_80X25_ROWS - 1;   // row 24

    // Build blank character with attribute
    unsigned short blank = crtVGAText(' ', terminal_color);;

    for (int col = 0; col < (int) VGA_80X25_COLUMNS; col++)
    {
        terminal_buffer[(row * (int) VGA_80X25_COLUMNS) + col] = blank;
    }
    
} // void terminal_delete_last_line()

void terminal_scroll_up_from(int line) {
    if ((line < 0) || (line >= (int) ((VGA_80X25_ROWS - 1))))  return;

    for (int y = line; y < (int)(VGA_80X25_ROWS - 1); y++) {
        for (int x = 0; x < (int) VGA_80X25_COLUMNS; x++) {
            terminal_buffer[(y * (int) VGA_80X25_COLUMNS) + x] =
                terminal_buffer[(((y+1) * (int)VGA_80X25_COLUMNS) + x)];
        }
    }
    terminal_clear_last_line();
}

void terminal_scroll() {
    terminal_scroll_up_from(1);
    // move cursor down 1
    move80x25CursorDown(&cursorpos,false);
} // void terminal_scroll()

void terminal_putchar(char c) 
{
    unsigned char uc = (unsigned char) c;

    // so we can handle non-printing characters
    switch (c) {
    case '\n':
        if (move80x25CursorDown(&cursorpos, true)) {
            terminal_scroll();
            }
        return;
    case '\r':
        cursorpos.col=1;
        set80x25cursorPos(&cursorpos);
        return;
    default:
        if (terminal_putentryat(uc, terminal_color, &cursorpos)) {
            terminal_scroll();
        }
        
    } // switch (c)
 

} // void terminal_putchar(unsigned char c)


void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    } // for (size_t i = 0; i < size; i++)
} // void terminal_write(const char* data, size_t size)

void terminal_writestring(const char* data) 
{
    terminal_write(data, strlen(data));
} // void terminal_writestring(const char* data)
