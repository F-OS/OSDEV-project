// definitions, that are always nice to have

// limine definitions
#include "limine/limine.h"
#include "limine/motionlimine.h"
#include "support/font/font.h"
#include <stdlib.h>

int sqrt(int x) {
  int res = 0;
  int add = 1 << 30;
  while (add > x) {
    add >>= 2;
  }
  while (add) {
    if (x >= res + add) {
      x -= res + add;
      res = (res >> 1) + add;
    } else {
      res >>= 1;
    }
    add >>= 2;
  }
  return res;
}

void kernel(void) {
  limine_init(); // Recieve data from limine
  if (lim_fb == NULL || lim_fb->framebuffer_count != 1) {
    return;
  }
  init_fb(lim_fb->framebuffers[0]);
  fb_idx = 2;
  for (int i = 0; i < 24; i++) {
    for (int j = 0; j < 24; j++) {
      putPixel(j, i, 0x00000000);
    }
  }
  drawString(0, 0, "Hello, World!", 0xFFFFFFFF, 0x00000000);
  for (int i = 0; i < 1024; i++)
  {
    for (int j = 0; j < 768; j++)
    {
      putPixel(i, j, (i << 16) | (j << 8) | (i + j));
    }
  }
  quick_blit(1, 0);
  for (;;)
    ;
}
