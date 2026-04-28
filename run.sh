#!/bin/bash
alr build
if [ $? -eq 0 ]; then
    mkdir -p iso_root/boot/grub
    cp bin/syros.elf iso_root/boot/
    echo 'set timeout=0
    menuentry "ada" {
        multiboot /boot/kernel.elf
    }' > iso_root/boot/grub/grub.cfg
    grub-mkrescue -o syros.iso iso_root
    qemu-system-i386 -cdrom syros.iso
fi
