CC	= i686-elf-gcc
AS	= nasm
LD	= i686-elf-ld

CFLAGS	:= -fno-builtin -fno-stack-protector -std=c11 -g -I ./inc -D DEBUG
ASFLAGS	:= -f elf
LDFLAGS	:= -T ./conf/link-kernel.ld

SRCS	:= $(shell find src -name "*.s" -o -name "*.c")
OBJS	:= $(patsubst %.s, %.o, $(patsubst %.c, %.o, $(SRCS)))

all: ./bin/kernel

clean:
	-rm -rf $(OBJS) ./bin/

run: ./bin/kernel
	-qemu-system-i386 -kernel ./bin/kernel -monitor stdio

debug: ./bin/kernel
	-qemu-system-i386 -kernel ./bin/kernel -s -S&
	-sleep 1
	-gdb

./bin/kernel: $(OBJS)
	-@mkdir bin/
	$(LD) $(LDFLAGS) -o ./bin/kernel $(OBJS)
