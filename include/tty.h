// tty.h  functions provided by the libtty.a object

#ifndef __KERNEL_TTY_H
#define __KERNEL_TTY_H

#include <stddef.h>

#include <vga.h>

bool terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_scroll();
void terminal_clear_last_line();
void terminal_scroll_line(size_t line);
bool terminal_clear_line(size_t line);
bool terminal_clear_screen();
bool terminal_clear_rows(size_t first, size_t last);
bool terminal_cursor_next_pos(struct cursor_pos_t* cursorpos);

#endif
