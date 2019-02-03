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
  while (1) {
  }
}

void kstrace(int depth) {
  // Stack contains:
  //  Second function argument.
  //  First function argument (depth).
  //  Return address in calling function.
  //  EBP of calling function (pointed to by current EBP).
  unsigned int *ebp = &depth - 2;
  printf("Stack trace:\n");
  for (unsigned int frame = 0; frame < depth; ++frame) {
    unsigned int eip = ebp[1];
    if (ebp == 0)
      break;
    // Unwind to previous stack frame.
    ebp = (unsigned int *)(ebp[0]);
    unsigned int *arguments = &ebp[2];
    printf("  %02X\n", eip);
  }
  printf("End of stack trace.\n");
}

void kpanic(char *err, struct regs *r) {
  monitor_set_bg_color(BLUE);
  monitor_clear();

  monitor_set_fg_color(YELLOW);
  monitor_set_bg_color(LBLUE);

  for (int i = 0; i < 80; i++)
    putchar(' ');

  monitor_write_center("KERNEL PANIC");

  for (int i = 0; i < 80; i++)
    putchar(' ');

  monitor_set_fg_color(YELLOW);
  monitor_set_bg_color(BLUE);

  monitor_write("\n");
  monitor_write_center(err);
  monitor_write("\n");

  if (r != 0) {
    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EAX: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->eax);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EBX: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->ebx);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ECX: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->ecx);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EDX: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->edx);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EIP: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->eip);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EBP: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->ebp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ESP: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->esp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ESI: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->esi);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EDI: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->edi);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("DS:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->ds);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ES:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->es);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("GS:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->gs);
    monitor_write("] ");

    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("FS:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->fs);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("CS:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->cs);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("SS:  ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->ss);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("ERR: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->err_code);
    monitor_write("] ");

    monitor_write("\n");
    monitor_write("\n");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("EFLAGS: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->eflags);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("USRESP: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->useresp);
    monitor_write("] ");

    monitor_write("[");
    monitor_set_fg_color(LGRAY);
    monitor_write("INTNO: ");
    monitor_set_fg_color(YELLOW);
    monitor_write_hex(r->int_no);
    monitor_write("] ");

    monitor_write("\n");
  }

  monitor_write("\n\n");
  /*
  monitor_write_center("Dedicated to G,");
  monitor_write_center("who helps me fix all the errors in my life.");
  */
  monitor_write_center("The kernel done goofed!");
  monitor_write("\n");
  monitor_write_center(
      "We're still in development, so this isn't completely unexpected.");

  monitor_write("\n");

  monitor_write_center("Please start an issue and submit a screenshot.");
  monitor_write_center(
      "You can do this at 'https://github.com/arraystock/corridor'.");
  monitor_write("\n");
  monitor_write_center(" Thank "
                       "you for using corridor!");

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
  char string[5];
  itoa(line, string, 10);

  printf("Assertion Failed (%s) at %s:%s\n", desc, file, string);

  abort();
}
