#ifndef FB_H
#define FB_H

#include <stdint.h>
int fb_init();

void fb_clear(uint32_t color);
void fb_draw_pixel(int x, int y, uint32_t color);
int fb_draw_glyph(int x, int y, uint32_t fg, uint32_t bg, char c);
extern int fb_width;
extern int fb_height;
#endif