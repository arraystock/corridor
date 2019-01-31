# corridor
Homebrew microkernel for x86. Originally written by naliferopoulos.

![alt Kernel Panic](/kernel_panic.png)

## What does it do?
corridor only targets x86 (i386+) computers, and as of now it supports Multiboot (GRUB *should* load it),
Paging, Physical and Virtual Memory Managers, Interrupts, Exception Handling, PIC, a simple Keyboard driver and
a Text Mode VGA driver. Application loading will come in the next updates.

## Nice! How do I compile?
Compiling and correctly linking the kernel requires setting up a cross-compiler and toolchain (GCC and Binutils) for
your system. After you have set up your cross-toolchain, edit the Makefile so that it points to your cross-binaries,
type `make` and voila!

## I've compiled it, now what?
You can test it under QEMU, or you can set up a partition with GRUB (or even LILO, SystemCommander and so on) to boot it.
If you want to run it under QEMU using the `-kernel` option, you can use `make run`.

## It crashed! :(
This will probably happen more often than not. Send me a screenshot of the panic screen and I might be able to fix it!

Wait, are you still reading? If so, you may also be crazy enough to try and debug corridor in order to identify
and/or patch the problem. Run `make debug` to set up remote debugging with GDB & QEMU, then type `file bin/kernel`
to pass symbols to GDB, and type `target remote localhost:1234` to make it attach to QEMU. Then you can debug corridor
like any other binary (with a little bit of bonus SEGFAULT). Oh, also, you can use QEMU's monitor which is attached
to stdin/stdout when you use `make run` to identify bugs on low-level, arch specific components.

Go nuts :)

### *corridor* is made with <3 and GCC. All credit for the original code goes to naliferopoulos. Special thanks to the patient and helpful community of the OSDev Forums, James Molloy, BrokenThorn Entertainment, and Bran M. for their excellent OS Development material.
