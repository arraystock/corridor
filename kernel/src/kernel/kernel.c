#include <arch/gdt.h>
#include <arch/idt.h>
#include <arch/irq.h>
#include <arch/isr.h>
#include <boot/multiboot.h>
#include <dbg/backtrace.h>
#include <dbg/gdb.h>
#include <dev/keyboard.h>
#include <dev/pcspeak.h>
#include <dev/serial.h>
#include <dev/timer.h>
#include <dev/vga.h>
#include <libk/stdio.h>
#include <libk/stdlib.h>
#include <mem/pmm.h>
#include <mem/vmm.h>

extern int end;

int k_main(struct multiboot_info *mboot) {
  monitor_clear();
  printf("Welcome to corridor!\n");

  enable_cursor();

  // Install the descriptor tables.
  gdt_install();
  idt_install();

  // Install our ISR and IRQ handlers.
  isr_install();
  irq_install();

  // Install our devices.
  timer_install();
  keyboard_install();

#ifdef DEBUG
  serial_initialize();
  printf("Serial port initialized.\n");
  serial_print("Serial port initialized.\n");
#endif

  uint32_t memSize = 1024 + mboot->mem_lower + mboot->mem_upper * 64;

#ifdef DEBUG
  printf("Memory Map Address: %02X\n", mboot->mmap_addr);
  printf("Memory Map Length: %02X\n", mboot->mmap_length);
  printf("Memory size: %d\n", memSize);
#endif

  pmmngr_init(memSize, (int)&end + 1);

  multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mboot->mmap_addr;
  while ((int)mmap < mboot->mmap_addr + mboot->mmap_length) {
#ifdef DEBUG
    printf("Memory Area - ");
    printf("Start: %02X ", mmap->addr);
    printf("End: %02X ", mmap->addr + mmap->len);
    printf("Type: %d\n", mmap->type);
#endif

    if (mmap->type == 1) {
      pmmngr_init_region(mmap->addr, mmap->len);
    }

    mmap = (multiboot_memory_map_t *)((unsigned int)mmap + mmap->size +
                                      sizeof(mmap->size));
  }

  // Mark kernel as used!
  pmmngr_deinit_region(0x100000, (int)&end - 0x100000);

  uint32_t total_blocks = pmmngr_get_block_count(),
           used_blocks = pmmngr_get_use_block_count(),
           free_blocks = pmmngr_get_free_block_count();

  // Sanity Check
  ASSERT(total_blocks == (used_blocks + free_blocks));

#ifdef DEBUG
  printf("Total blocks: %ld\n", total_blocks);
  printf("Used blocks: %ld\n", used_blocks);
  printf("Free blocks: %ld\n", free_blocks);

  printf("Physical Memory working.\n");
#endif

  vmmngr_initialize();

#ifdef DEBUG
  printf("Virtual Memory working.\n");
  // printf("k_main() address: %02X\n", k_main);
#ifdef GDB_DEBUG
  set_debug_traps(); // Exception handlers for GDB
  printf("Waiting for GDB...\n");
  BREAKPOINT(); // Sync with GDB.
#endif
#endif

#ifdef DEBUG
  printf("Testing PC speaker... ");
  // play_sound(1000);
  // timer_wait(10);
  // nosound();
  beep();
  printf("Done.\n");
#endif

  // For testing assertions.
  // ASSERT("That's it for now!" == 0);

  // For testing stack traces.
  // kstrace(10);

  // For testing exceptions.
  //__asm__ __volatile__("int $6");
  // timer_wait(1000);
  // int zero = 0xFFFFFFFF / 0;

  return 0xDEADBEEF;
}
