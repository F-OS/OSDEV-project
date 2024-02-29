#include "font.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void putcharc(char c) {
  char **render = malloc(24 * sizeof(char *));
  for (int i = 0; i < 24; i++) {
    render[i] = malloc(24 * sizeof(char));
    memset(render[i], ' ', 24);
  }
  int width = get_char_width(c);
  const unsigned char *bitmap = get_char_bitmap(c);
  for (int col = 0; col < width; col++) {
    int prevdiv = -1;
    int prevdivmul = -1;
    for (int row = 0; row < 24; row++) {

      int rowdiv = row >> 3;
      if (rowdiv != prevdiv) {
        prevdiv = rowdiv;
        prevdivmul = rowdiv * width;
      }

      int bitmap_index = prevdivmul + col;
      int bitidx = row & 7;

      if (bitmap[bitmap_index] & (1 << bitidx)) {
        render[row][col] = '#';
      } else {
        render[row][col] = ' ';
      }
    }
  }

  // print the render with the x and y flipped
  for (int row = 0; row < 24; row++) {
    for (int col = 0; col < 24; col++) {
      printf("%c", render[col][row]);
    }
    printf("\n");
  }
}

void putsc(const char *s) {
  while (*s) {
    putcharc(*s);
    printf("\n");
    s++;
  }
}

// PSF test linux-side
int main() {
  uint16_t glyph = 0;
  putsc("Hello, World!");
  return 0;
}