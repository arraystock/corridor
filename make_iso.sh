#! /bin/bash

if (($# != 3)); then
	echo "Usage: config kernel iso"
	exit 1
fi

mkdir -p iso/boot/grub
cp $1 iso/boot/grub/
cp $2 iso/boot/
grub-mkrescue -o $3 iso
