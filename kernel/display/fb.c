#include "fb.h"
#include "../limine/limine.h"
#include "../limine/motionlimine.h"
#include "../globals.h"
#include "font.h"

uint32_t *fb = NULL;
int fb_width = 0;
int fb_height = 0;

int fb_init() {
  if (lim_fb == NULL || lim_fb->framebuffer_count != 1 ||
      lim_fb->framebuffers[0] == NULL || lim_fb->framebuffers[0]->bpp != 32) {
    return 1;
  }
  fb = (uint32_t *)lim_fb->framebuffers[0]->address;
  fb_width = lim_fb->framebuffers[0]->width;
  fb_height = lim_fb->framebuffers[0]->height;
  return 0;
}

void fb_clear(uint32_t color) {
  for (int i = 0; i < fb_width * fb_height; i++) {
    fb[i] = color;
  }
}

void fb_draw_pixel(int x, int y, uint32_t color) {
  if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) {
    return;
  }
  fb[y * fb_width + x] = color;
}

int fb_draw_glyph(int x, int y, uint32_t fg, uint32_t bg, char c) {
  if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) {
    return 0;
  }
  int idx = c;
  unsigned char *glyph = number_font[idx];
  for (int i = 0; i < FONT_HEIGHT; i++) {
    for (int j = 0; j < FONT_WIDTH; j++) {
      if (glyph[i] & (1 << (FONT_WIDTH - 1 - j))) {
        fb_draw_pixel(x + j, y + i, fg);
      }
      else {
        fb_draw_pixel(x + j, y + i, bg);
      }
    }
  }
  return FONT_WIDTH;
}