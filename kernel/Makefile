CC	= i686-elf-gcc
AS	= nasm
LD	= i686-elf-ld

CFLAGS	:= -fno-builtin -fno-stack-protector -std=c11 -g -I ./inc -D DEBUG -D GDB_DEBUG
ASFLAGS	:= -f elf -I ./inc/
LDFLAGS	:= -T ./conf/link-kernel.ld

SRCS	:= $(shell find src -name "*.s" -o -name "*.c")
OBJS	:= $(patsubst %.s, %.o, $(patsubst %.c, %.o, $(SRCS)))

FORMAT	:= $(shell find . -name "*.c") $(shell find . -name "*.h")

.PHONY: all clean run debug clang-format

all: ./bin/kernel

clean:
	-rm -rf $(OBJS) ./bin/ corridor.iso ./iso

run: ../corridor.iso
	qemu-system-i386 -hda ../corridor.iso -monitor stdio -serial telnet:127.0.0.1:1234,server,nowait

debug: ../corridor.iso
	qemu-system-i386 -hda ../corridor.iso -serial telnet:127.0.0.1:1234,server,nowait -s&
	sleep 1
	gdb

clang-format:
	clang-format -style=llvm -i $(FORMAT)

./bin/kernel: $(OBJS)
	-@mkdir bin/
	$(LD) $(LDFLAGS) -o ./bin/kernel $(OBJS)

../corridor.iso: all
	../make_iso.sh ../grub.cfg ./bin/kernel ../corridor.iso
