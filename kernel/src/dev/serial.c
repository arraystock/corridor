#include <arch/common.h>

#define PORT 0x3f8 /* COM1 */

#define BAUD_115200 0x01
#define BAUD_57600 0x02
#define BAUD_38400 0x03

void serial_initialize() {
  outb(PORT + 1, 0x00);       // Disable all interrupts
  outb(PORT + 3, 0x80);       // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, BAUD_38400); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00);       //                  (hi byte)
  outb(PORT + 3, 0x03);       // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7);       // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B);       // IRQs enabled, RTS/DSR set
}

int serial_received() { return inb(PORT + 5) & 1; }

char serial_read() {
  while (serial_received() == 0)
    ;

  return inb(PORT);
}

int is_transmit_empty() { return inb(PORT + 5) & 0x20; }

void serial_write(char c) {
  while (is_transmit_empty() == 0)
    ;
  outb(PORT, c);
}

void serial_print(const char *str) {
  for (int i = 0; str[i] != '\0'; i++)
    serial_write(str[i]);
}
