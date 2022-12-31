#!/bin/bash -x

BUILDDIR='build'
CONFDIR='conf'
SRCDIR='src'
ASMDIR='asm'
LIBDIR='lib'

CFLAGS='-nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs'

mkdir -p $BUILDDIR

nasm -f elf $ASMDIR/kernel.asm -o $BUILDDIR/kasm.o
nasm -f elf $ASMDIR/interrupts.asm -o $BUILDDIR/interrupts.o
nasm -f elf $ASMDIR/pio.asm -o $BUILDDIR/pio.o
nasm -f elf $ASMDIR/gdt.asm -o $BUILDDIR/gdt.o

gcc -m32 $CFLAGS -c $SRCDIR/kernel.c -o $BUILDDIR/kernel.o
gcc -m32 $CFLAGS -c $SRCDIR/stdio.c -o $BUILDDIR/stdio.o
gcc -m32 $CFLAGS -c $SRCDIR/stdproc.c -o $BUILDDIR/stdproc.o
gcc -m32 $CFLAGS -c $SRCDIR/stdlib.c -o $BUILDDIR/stdlib.o
gcc -m32 $CFLAGS -c $SRCDIR/panic.c -o $BUILDDIR/panic.o
gcc -m32 $CFLAGS -c $SRCDIR/memory.c -o $BUILDDIR/memory.o

gcc -m32 $CFLAGS -c drivers/idt/idt.c -o $BUILDDIR/idt.o
gcc -m32 $CFLAGS -c drivers/isr/isr.c -o $BUILDDIR/isr.o
gcc -m32 $CFLAGS -c drivers/keyboard/keyboard.c -o $BUILDDIR/keyboard.o
gcc -m32 $CFLAGS -c drivers/console/console.c -o $BUILDDIR/console.o
gcc -m32 $CFLAGS -c drivers/gdt/mgdt.c -o $BUILDDIR/mgdt.o
gcc -m32 $CFLAGS -c drivers/vga/vga.c -o $BUILDDIR/vga.o

ld -m elf_i386 -T$CONFDIR/linker.ld -o kernel.elf $BUILDDIR/kernel.o $BUILDDIR/kasm.o $BUILDDIR/stdio.o $BUILDDIR/stdproc.o $BUILDDIR/stdlib.o $BUILDDIR/panic.o $BUILDDIR/memory.o $BUILDDIR/idt.o $BUILDDIR/isr.o $BUILDDIR/pio.o $BUILDDIR/interrupts.o $BUILDDIR/keyboard.o $BUILDDIR/console.o $BUILDDIR/mgdt.o $BUILDDIR/gdt.o $BUILDDIR/vga.o

echo "-------"
echo "Compilation succeeded!"
echo "-------"

qemu-system-i386 -kernel kernel.elf
