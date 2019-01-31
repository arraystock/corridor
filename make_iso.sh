#! /bin/bash

mkdir -p iso/boot/grub
cp grub.cfg iso/boot/grub/
cp kernel/bin/* iso/boot/
grub-mkrescue -o corridor.iso iso
