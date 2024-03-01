// definitions, that are always nice to have

// limine definitions
#include "display/console.h"
#include "display/fb.h"
#include "limine/limine.h"
#include "limine/motionlimine.h"
void print_memory_map(void);
void kernel(void) {
  limine_init();
  // Check for video
  if (fb_init() != 0) {
    for (;;)
      ;
  }
  // Clear the screen
  fb_clear(0x00000000);
  //const char *hello = "Hello, World!";
  // Draw the string
  console_init();
  console_puts("Kernel started\n");
  console_puts("Memory map:\n");
  print_memory_map();
}

void print_memory_map(void) {
  for(unsigned i = 0; i < lim_memmap->entry_count; i++)
  {
    struct limine_memmap_entry *entry = lim_memmap->entries[i];

    console_puts("Base: ");
    console_puthex(entry->base);
  }
}