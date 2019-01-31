;
; boot.s -- Kernel start location. Also defines multiboot header.
;

extern enable_paging
extern k_main

section .multiboot

align 4
dd 0x1BADB002                   ; Magic
dd 0x00                         ; Flags
dd - (0x1BADB002 + 0x00)        ; Checksum

section .text

global start
start:
  cli                   ; Disable interrupts.
  call enable_paging    ; Enable paging and remap kernel to 3GB.
  xor ebp, ebp          ; Prepare NULL stack frame so traces know where to stop
  push ebp
  push ebx              ; Load multiboot header location
  call k_main           ; Call k_main().
  jmp $                 ; Loop forever.
