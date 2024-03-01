/*
* The kernel's main console. Handles all input and output.
*/
#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdint.h>
#define TYPICAL_MON_WIDTH 1024
#define TYPICAL_MON_HEIGHT 768
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 12
// We can have 768 / 12 = 64 rows
// We can have 1024 / 8 = 128 columns
#define LEFT_MARGIN 1
#define TOP_MARGIN 1
#define RIGHT_MARGIN (MAX_COLS - 1)
#define BOTTOM_MARGIN (MAX_ROWS - 1)
#define FONT_HSPACING 1
#define FONT_VSPACING 2
#define FONT_WIDTH (CHAR_WIDTH + FONT_HSPACING)
#define FONT_HEIGHT (CHAR_HEIGHT + FONT_VSPACING)
#define MAX_ROWS (TYPICAL_MON_HEIGHT / FONT_HEIGHT)
#define MAX_COLS (TYPICAL_MON_WIDTH / FONT_WIDTH)
#define BUFFER_SIZE (MAX_ROWS) * (MAX_COLS)

typedef struct terminal_cell {
    char c;
    unsigned char fg;
    unsigned char bg;
} terminal_cell_t;

typedef struct terminal {
    terminal_cell_t buffer[BUFFER_SIZE];
    int bufidx;
    int bufspan;
    int cursor_x;
    int cursor_y;
    int alt_x;
    int alt_y;
    int width;
    int height;
    int spacing;
    unsigned char fg;
    unsigned char bg;
    int cursor_enabled;
    int flags;
    int escape;
} terminal_t;
    

void console_init();
void console_putchar(char c);   
void console_scroll();
void console_puts(const char *s);
void console_puthex(uint64_t n);
#endif