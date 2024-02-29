#include <stdint.h>
#include "../../limine/motionlimine.h"
#include "../../limine/limine.h"
#include "fontdat.h"

struct limine_framebuffer *fb;

int fb_idx = 0;
static int alt[1024 * 768][2] __attribute__((aligned(4096)));

static int* fbs[3] = {(void*)0, alt[0], alt[1]};

void init_fb(struct limine_framebuffer *framebuffer) {
    fb = framebuffer;
    fbs[0] = (void *)fb->address;
}

void putPixel(uint64_t x, uint64_t y, uint32_t color) {
    if (x >= fb->width || y >= fb->height) {
        return;
    }

    uint32_t *pixel = (uint32_t *)fbs[fb_idx] + (y * fb->width + x);
    *pixel = color;
}

void drawChar(int x, int y, int c, uint32_t fg, uint32_t bg) {
    uint8_t * bm = number_font[c];
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 8; j++) {
            if (bm[i] & (1 << (7 - j))) {
                putPixel(x + j, y + i, fg);
            } else {
                putPixel(x + j, y + i, bg);
            }
        }
    }   
}

void drawString(int x, int y, const char *str, uint32_t fg, uint32_t bg) {
    while (*str) {
        drawChar(x, y, *str, fg, bg);
        x += 8;
        str++;
    }
}

void quick_blit(int from, int to) {
    for (int i = 0; i < 1024 * 768; i++) {
        ((uint32_t *)fbs[to])[i] = ((uint32_t *)fbs[from])[i];
    }
}