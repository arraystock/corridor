#include <libk/stdio.h>

void kstrace(int depth) {
  // Stack contains:
  //  Second function argument.
  //  First function argument (depth).
  //  Return address in calling function.
  //  EBP of calling function (pointed to by current EBP).
  unsigned int *ebp = &depth - 2, addr;
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
