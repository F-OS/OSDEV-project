/*
* The kernel's main console. Handles all input and output.
*/
#ifndef CONSOLE_H
#define CONSOLE_H

#define TYPICAL_MON_WIDTH 1024
#define TYPICAL_MON_HEIGHT 768
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 12
// We can have 768 / 12 = 64 rows
#define MAX_ROWS (TYPICAL_MON_HEIGHT / CHAR_HEIGHT)
// We can have 1024 / 8 = 128 columns
#define MAX_COLS (TYPICAL_MON_WIDTH / CHAR_WIDTH)
#define BUFFER_SIZE (MAX_ROWS) * (MAX_COLS)

typedef struct terminal_cell {
    char c;
    unsigned char fg;
    unsigned char bg;
} terminal_cell_t;

typedef struct terminal {
    terminal_cell_t buffer[BUFFER_SIZE];
    int cursor_x;
    int cursor_y;
    int alt_x;
    int alt_y;
    int width;
    int height;
    unsigned char fg;
    unsigned char bg;
    int cursor_enabled;
    int flags;
    int escape;
} terminal_t;
    

#endif