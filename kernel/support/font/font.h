#ifndef FONT_H
#define FONT_H  
#include <stdint.h>
void init_fb(struct limine_framebuffer *framebuffer);
void putPixel(uint64_t x, uint64_t y, uint32_t color);
void drawChar(int x, int y, int c, uint32_t fg, uint32_t bg);
void drawString(int x, int y, const char *str, uint32_t fg, uint32_t bg);
void quick_blit(int from, int to);
extern int fb_idx;
extern int fb_x;
extern int fb_y;
#endif