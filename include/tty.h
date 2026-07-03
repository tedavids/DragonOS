// tty.h  functions provided by the libtty.a object

#ifndef __KERNEL_TTY_H
#define __KERNEL_TTY_H

#include <stddef.h>

#include <vga.h>

// initialize the screen

// Parameters:  None

// Returns:     true on success, false otherwise
bool terminal_initialize(void);

// put a character to the scree at the current cursor positon

// Parameters:  c -- The character to print

// Returns:     None
void terminal_putchar(char c);

// Write data to the screen

// Parameters:  data -- The data to display
//              size -- The length of the data

// Returns:     None
void terminal_write(const char* data, size_t size);

// Write a string to the screen

// Parameters:  data -- The string to display

// Returns:     None
void terminal_writestring(const char* data);

// scroll the entire screen
// Parameters:  None

// Return:      None
void terminal_scroll();

// clear the last line of the screen

// Parameters:  None

// Returns:     None
void terminal_clear_last_line();

// Scroll a single line up 1 line

// Parameters:  line -- The line to scroll

// Returns:     None
void terminal_scroll_line(size_t line);

// clear a line on the screen

// Parameters:  line -- The line to clear

// Returns:     true if the line is valid
bool terminal_clear_line(size_t line);

// clear the screen

// Parameters:  None

// Returns:     true if successful, false otherwise
bool terminal_clear_screen();

// clear a ranger of rows

// Parameters:  first -- The first row to clear
//              last -- The last row to clear
bool terminal_clear_rows(size_t first, size_t last);

// Move cursor to next position, also scrolls if necessary

// Parameters:  cursorpos - The current cursor postion

// Returns:     true if successful, false otherwise
bool terminal_cursor_next_pos(struct cursor_pos_t* cursorpos);

#endif
