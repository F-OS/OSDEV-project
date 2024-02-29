// definitions, that are always nice to have
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

// limine definitions
#include "early/limine.h"
#include "early/motionlimine.h"
#include <stdlib.h>
#define MS * 1000

uint32_t *blit_buf = NULL;
int fb_write_pixel(uint64_t x, uint64_t y, uint8_t r, uint8_t g, uint8_t b) {
  if (blit_buf == NULL) {
    blit_buf = alloca(lim_fb->framebuffers[0]->width *
                      lim_fb->framebuffers[0]->height * 4);
  }
  if (x >= lim_fb->framebuffers[0]->width ||
      y >= lim_fb->framebuffers[0]->height) {
    return -1;
  }

  uint32_t color = 0xFF000000 | (r << 16) | (g << 8) | b;
  blit_buf[y * lim_fb->framebuffers[0]->width + x] = color;
  return 0;
}

int fb_blit(void) {
  if (lim_fb == NULL || lim_fb->framebuffer_count != 1 ||
      lim_fb->framebuffers[0] == NULL || lim_fb->framebuffers[0]->bpp != 32) {
    return -1;
  }
  // Use long for the copy to speed up the process
  for (uint64_t i = 0;
       i < lim_fb->framebuffers[0]->width * lim_fb->framebuffers[0]->height;
       i++) {
    ((uint32_t *)lim_fb->framebuffers[0]->address)[i] = blit_buf[i];
  }
  return 0;
}

// our mighty kernel
enum rainbow_colors {
  RED = 0xFF0000,
  GREEN = 0x00FF00,
  BLUE = 0x0000FF,
  YELLOW = 0xFFFF00,
  CYAN = 0x00FFFF,
  MAGENTA = 0xFF00FF,
  WHITE = 0xFFFFFF
};
void kernel(void) {
  limine_init();
  // linear framebuffer is available (with 32 bits per pixel)?
  if (lim_fb == NULL || lim_fb->framebuffer_count != 1 ||
      lim_fb->framebuffers[0] == NULL || lim_fb->framebuffers[0]->bpp != 32)
    // no, it's not available
    for (;;)
      ;

  // Rainbow
  unsigned char r = 100, g = 1, b = 1, i = 0;
  for (;;) {
    for (uint64_t y = 0; y < lim_fb->framebuffers[0]->height; y++) {
      for (uint64_t x = 0; x < lim_fb->framebuffers[0]->width; x++) {
        fb_write_pixel(x, y, r, g, b);
      }
    }
    switch (i++ % 7) {
    case 0:
      r = 100;
      g = 1;
      b = 1;
      break;
    case 1:
      r = 1;
      g = 100;
      b = 1;
      break;
    case 2:
      r = 1;
      g = 1;
      b = 100;
      break;
    case 3:
      r = 100;
      g = 100;
      b = 1;
      break;
    case 4:
      r = 1;
      g = 100;
      b = 100;
      break;
    case 5:
      r = 100;
      g = 1;
      b = 100;
      break;
    case 6:
      r = 100;
      g = 100;
      b = 100;
      break;
    }
    fb_blit();
  }
  // infinite loop
  for (;;)
    ;
}
