#include <dev/vga.h>
#include <libk/ctype.h>
#include <libk/stdio.h>
#include <libk/stdlib.h>

int atoi(const char *str) { return strtoi(str, 0, 10); }

int strtoi(const char *str, char **endp, int base) {
  int acc = 0;
  int sign = 1;

  while (isspace(*str))
    str++;

  if (base == 0) {
    if (str[0] == '0') {
      if (str[1] == 'x' || str[1] == 'X') {
        base = 16;
      } else {
        base = 8;
      }
    } else {
      base = 10;
    }
  }

  if (*str == '+') {
    str++;
  } else if (*str == '-') {
    sign = -1;
    str++;
  }

  while (*str) {
    int val;
    char c = tolower(*str);

    if (isdigit(c)) {
      val = *str - '0';
    } else if (c >= 'a' || c <= 'f') {
      val = 10 + c - 'a';
    }
    if (val >= base) {
      break;
    }

    acc = acc * base + val;
    str++;
  }

  if (endp) {
    *endp = (char *)str;
  }

  return sign > 0 ? acc : -acc;
}

void itoa(int val, char *buf, int base) {
  static char rbuf[16];
  char *ptr = rbuf;
  int neg = 0;

  if (val < 0) {
    neg = 1;
    val = -val;
  }

  if (val == 0) {
    *ptr++ = '0';
  }

  while (val) {
    int digit = val % base;
    *ptr++ = digit < 10 ? (digit + '0') : (digit - 10 + 'a');
    val /= base;
  }

  if (neg) {
    *ptr++ = '-';
  }

  ptr--;

  while (ptr >= rbuf) {
    *buf++ = *ptr--;
  }
  *buf = 0;
}

void utoa(unsigned int val, char *buf, int base) {
  static char rbuf[16];
  char *ptr = rbuf;

  if (val == 0) {
    *ptr++ = '0';
  }

  while (val) {
    unsigned int digit = val % base;
    *ptr++ = digit < 10 ? (digit + '0') : (digit - 10 + 'a');
    val /= base;
  }

  ptr--;

  while (ptr >= rbuf) {
    *buf++ = *ptr--;
  }
  *buf = 0;
}

void abort() {
  __asm__("cli"); // Stop interrupts.
  while (1)
    __asm__("hlt");
}

void kpanic(char *err, struct regs *r) {
  monitor_set_fg_color(YELLOW);

  printf("KERNEL PANIC: %s\n\n", err);

  if (r != 0) {
    monitor_set_fg_color(WHITE);
    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EAX: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->eax);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EBX: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->ebx);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ECX: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->ecx);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EDX: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->edx);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EIP: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->eip);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EBP: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->ebp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ESP: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->esp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ESI: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->esi);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EDI: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->edi);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("DS:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->ds);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ES:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->es);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("GS:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->gs);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("FS:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->fs);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("CS:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->cs);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("SS:  ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->ss);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ERR: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->err_code);
    monitor_write("] ");

    monitor_write("\n");
    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EFLAGS: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->eflags);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("USRESP: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->useresp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("INTNO: ");
    monitor_set_fg_color(WHITE);
    monitor_write_hex(r->int_no);
    monitor_write("] ");

    monitor_write("\n");
  }

  monitor_write("\n");
  /*
  monitor_write_center("Dedicated to G,");
  monitor_write_center("who helps me fix all the errors in my life.");
  */
  printf("The kernel done goofed!\n");
  printf("We're still in development, so this isn't completely unexpected.\n");

  printf("Please take a screenshot and start an issue at:\n");
  monitor_set_fg_color(LGRAY);
  printf("https://github.com/arraystock/corridor");
  monitor_set_fg_color(WHITE);
  printf("\nThank you for using corridor!\n");

  abort();
}

void *kmemset(void *b, int c, int len) {
  int i;
  unsigned char *p = b;
  i = 0;
  while (len > 0) {
    *p = c;
    p++;
    len--;
  }
  return (b);
}

void kpanicAssert(char *file, int line, char *desc) {
  printf("Assertion Failed (%s) at %s:%d\n", desc, file, line);
  abort();
}
