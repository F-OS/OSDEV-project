// definitions, that are always nice to have
#include "stdarg.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"

// limine definitions
#include "display/fb.h"
#include "early/limine.h"
#include "early/motionlimine.h"
#include <stdlib.h>
#define MS *1000

void kernel(void) {
  limine_init();
  // linear framebuffer is available (with 32 bits per pixel)
  if (fb_init() != 0) {
    for (;;)
      ;
  }
  fb_clear(0x77777777);
  int x = 48;
  int y = 0;
  for (int i = 0; i < 132; i++) {
    x += fb_draw_glyph(x, y, 0x00FF00FF, i);
    if (x > fb_width - 48) {
      x = 0;
      y += 8;
    }
  }
  // infinite loop
  for (;;)
    ;
}
