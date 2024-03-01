#include "fb.h"
#include "console.h"
#include "palette.h"

static struct terminal console;

void console_init(void) {
  console.cursor_x = LEFT_MARGIN;
    console.cursor_y = TOP_MARGIN;
    for(int i = 0; i < console.width * console.height; i++)
    {
        console.buffer[i].c = ' ';
        console.buffer[i].fg = C_WHITE;
        console.buffer[i].bg = C_BLACK;
    }
    console.bufidx = 0;
    console.alt_x = 0;
    console.alt_y = 0;
    console.fg = C_WHITE;
    console.bg = C_BLACK;
    console.width = RIGHT_MARGIN;
    console.height = BOTTOM_MARGIN;
    console.spacing = 1;
    console.cursor_enabled = 1;
    console.flags = 0;
    console.escape = 0;
}
 
static void buffer_putchar(char c);
void console_putchar(char c) 
{
    buffer_putchar(c);
    if (console.cursor_x >= console.width) {
        console.cursor_x = LEFT_MARGIN;
        console.cursor_y += 1;
    }
    if (console.cursor_y >= console.height) {
        console_scroll();
    }  
    if (c == '\n') {
        console.cursor_x = LEFT_MARGIN;
        console.cursor_y += 1;
    } else if (c == '\r') {
        console.cursor_x = 0;
    } else if (c == '\b') {
        if (console.cursor_x > LEFT_MARGIN) {
        console.cursor_x -= 1;
        }
        else if (console.cursor_y > TOP_MARGIN) {
            console.cursor_x = RIGHT_MARGIN - 1;
            console.cursor_y -= 1;
        }
    } else if (c == '\t') {
        console.cursor_x += 4;
    } else if (c == '\033') {
        console.escape = 1;
    } else if (console.escape) {
        if (c == '[') {
            console.escape = 2;
        } else {
            console.escape = 0;
        }
    } else if(console.escape == 2)
    {
        // Control sequence - Color
        switch(c)
        {
            case '0':
                console.fg = C_BLACK;
                break;
            case '1':
                console.fg = C_RED;
                break;
            case '2':
                console.fg = C_GREEN;
                break;
            case '3':
                console.fg = C_BROWN;
                break;
            case '4':
                console.fg = C_NAVY;
                break;
            case '5':
                console.fg = C_PURPLE;
                break;
            case '6':
                console.fg = C_D_CYAN;
                break;
            case '7':
                console.fg = C_GRAY;
                break;
            case '8':
                console.fg = C_D_GRAY;
                break;
            case '9':
                console.fg = C_L_RED;
                break;
            case 'A':
                console.fg = C_L_GREEN;
                break;
            case 'B':
                console.fg = C_YELLOW;
                break;
            case 'C':
                console.fg = C_L_BLUE;
                break;
            case 'D':
                console.fg = C_MAGENTA;
                break;
            case 'E':
                console.fg = C_CYAN;
                break;
            case 'F':
                console.fg = C_WHITE;
                break;
            case 's':
                {
                    int tmp = console.fg;
                    console.fg = console.bg;
                    console.bg = tmp;
                    break;
                }
            default:
                break;
        }
        console.escape = 0;
    } else {
        uint32_t fgcolor = term_colors[console.fg];
        uint32_t bgcolor = term_colors[console.bg];
        fb_draw_glyph(console.cursor_x * FONT_WIDTH, console.cursor_y * FONT_HEIGHT, fgcolor, bgcolor, c);
        console.cursor_x += 1;
    }
}

static void buffer_putchar(char c) {
    if (console.bufidx >= console.width * console.height) {
        console.bufidx = MAX_ROWS + 1;
        console.bufspan = BUFFER_SIZE - (MAX_ROWS + 1);
    }
    // Handle backspaces and tabs
    switch(c)
    {
        case '\b':
            if (console.bufidx > 0) {
                console.bufidx -= 1;
            }
            break;
        case '\t':
            for(int i = 0; i < 4; i++)
            {
                console.buffer[console.bufidx].c = ' ';
                console.buffer[console.bufidx].fg = console.fg;
                console.buffer[console.bufidx].bg = console.bg;
                console.bufidx += 1;
                console.bufspan += 1;
            }
            break;
        default:
            console.buffer[console.bufidx].c = c;
            console.buffer[console.bufidx].fg = console.fg;
            console.buffer[console.bufidx].bg = console.bg;
            console.bufidx += 1;
            console.bufspan += 1;
    break;
    }
}

// Scrolling is done by reprinting the buffer offset by RIGHT_MARGIN
void console_scroll(void) {
    for(int i = 0; i < console.bufspan; i++)
    {
        console.buffer[i].c = console.buffer[i + RIGHT_MARGIN].c;
        console.buffer[i].fg = console.buffer[i + RIGHT_MARGIN].fg;
        console.buffer[i].bg = console.buffer[i + RIGHT_MARGIN].bg;
    }
    console.cursor_y -= 1;
    console.bufidx -= RIGHT_MARGIN;
    console.bufspan -= RIGHT_MARGIN;
    for(int i = 0; i < console.bufspan; i++)
    {
        fb_draw_glyph((i % RIGHT_MARGIN) * FONT_WIDTH, (i / RIGHT_MARGIN) * FONT_HEIGHT, console.buffer[i].fg, console.buffer[i].bg, console.buffer[i].c);
    }
}

void console_puts(const char *s) {
    while (*s) {
        console_putchar(*s++);
    }
}

void console_puthex(uint64_t n)
{
    char buf[18] = "0x0000000000000000";
    for(int i = 16; i > 1; i--)
    {
        int digit = n & 0xf;
        if (digit < 10)
        {
            buf[i] = '0' + digit;
        }
        else
        {
            buf[i] = 'a' + digit - 10;
        }
        n >>= 4;
    }
    console_puts(buf);
}