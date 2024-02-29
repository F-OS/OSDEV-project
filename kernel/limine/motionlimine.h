#ifndef MOTIONLIMINE_H
#define MOTIONLIMINE_H
#include "../globals.h"
extern volatile struct limine_bootloader_info_response *lim_boot_info;
extern volatile struct limine_stack_size_response *lim_stack_size;
extern volatile struct limine_hhdm_response *lim_hhdm;
extern volatile struct limine_framebuffer_response *lim_fb;
extern volatile struct limine_smp_response *lim_smp;
extern volatile struct limine_memmap_response *lim_memmap;
extern volatile struct limine_rsdp_response *lim_rsdp;
extern volatile struct limine_smbios_response *lim_smbios;

void limine_init();
#endif