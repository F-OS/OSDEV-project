// MotionLimine.c stores pointers to data passed from the bootloader (limine) to
// the kernel.
#include "limine.h"
#include "../globals.h"
#define NULL ((void *)0)
static volatile struct limine_bootloader_info_request
    limine_bootloader_info_request = {.id = LIMINE_BOOTLOADER_INFO_REQUEST,
                                      .revision = 0};

static volatile struct limine_stack_size_request limine_stack_size_request = {
    .id = LIMINE_STACK_SIZE_REQUEST,
    .revision = 0,
    .stack_size = KERNEL_STACK_SIZE};

static volatile struct limine_hhdm_request limine_hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
};

static volatile struct limine_framebuffer_request limine_framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

static volatile struct limine_smp_request limine_smp_request = {
    .id = LIMINE_SMP_REQUEST, .revision = 0};

static volatile struct limine_memmap_request limine_memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST, .revision = 0};

static volatile struct limine_rsdp_request limine_rsdp_request = {
    .id = LIMINE_RSDP_REQUEST, .revision = 0};

static volatile struct limine_smbios_request limine_smbios_request = {
    .id = LIMINE_SMBIOS_REQUEST, .revision = 0};

volatile struct limine_bootloader_info_response *lim_boot_info;
volatile struct limine_stack_size_response *lim_stack_size;
volatile struct limine_hhdm_response *lim_hhdm;
volatile struct limine_framebuffer_response *lim_fb;
volatile struct limine_smp_response *lim_smp;
volatile struct limine_memmap_response *lim_memmap;
volatile struct limine_rsdp_response *lim_rsdp;
volatile struct limine_smbios_response *lim_smbios;

static char inb_lim(uint16_t port) {
  char ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
  return ret;
}

static void outb_lim(uint16_t port, char val) {
  asm volatile("outb %0, %1" : : "a"(val), "dN"(port));
}

static void crash_now_lim(const char *msg) {
  // Write the given message to the serial port
  for (const char *c = msg; *c; c++) {
    while ((inb_lim(0x3FD) & 0x20) == 0)
      ;
    outb_lim(0x3F8, *c);
  }

  for (;;) {
    asm volatile("cli");
    asm volatile("hlt");
  }
}

void limine_init(void) {
  lim_boot_info = limine_bootloader_info_request.response;
  lim_stack_size = limine_stack_size_request.response;
  lim_hhdm = limine_hhdm_request.response;
  lim_fb = limine_framebuffer_request.response;
  lim_smp = limine_smp_request.response;
  lim_memmap = limine_memmap_request.response;
  lim_rsdp = limine_rsdp_request.response;
  lim_smbios = limine_smbios_request.response;
  if (lim_boot_info == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_stack_size == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_hhdm == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_fb == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_smp == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_memmap == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_rsdp == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
  if (lim_smbios == NULL) {
    crash_now_lim("Limine did not fufill a request");
  }
}