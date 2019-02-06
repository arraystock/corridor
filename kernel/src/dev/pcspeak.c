#include <arch/common.h>
#include <dev/timer.h>
#include <libk/types.h>

void play_sound(uint32_t freq) {
  uint32_t div;

  // Set the frequency.
  div = 1193180 / freq;
  outb(0x43, 0xb6);
  outb(0x42, (uint8_t)(div));
  outb(0x42, (uint8_t)(div >> 8));

  // Play the sound using the PC speaker.
  uint8_t tmp = inb(0x61);
  if (tmp != (tmp | 3)) {
    outb(0x61, tmp | 3);
  }
}

void nosound() { outb(0x61, inb(0x61) & 0xFC); }

void beep() {
  play_sound(1000);
  timer_wait(10);
  nosound();
}
