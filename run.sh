#!/bin/bash

BUILDDIR='build'
CONFDIR='conf'
SRCDIR='src'
ASMDIR='asm'
LIBDIR='lib'

mkdir -p $BUILDDIR
mkdir -p $CONFDIR
mkdir -p $SRCDIR
mkdir -p $ASMDIR
mkdir -p $LIBDIR

nasm -f elf32 $ASMDIR/kernel.asm -o $BUILDDIR/kasm.o

gcc -m32 -c $SRCDIR/kernel.c -o $BUILDDIR/kernel.o
gcc -m32 -c $SRCDIR/stdio.c -o $BUILDDIR/stdio.o
gcc -m32 -c $SRCDIR/stdproc.c -o $BUILDDIR/stdproc.o
gcc -m32 -c $SRCDIR/stdlib.c -o $BUILDDIR/stdlib.o
gcc -m32 -c $SRCDIR/panic.c -o $BUILDDIR/panic.o
gcc -m32 -c $SRCDIR/memory.c -o $BUILDDIR/memory.o

ld -m elf_i386 -T $CONFDIR/linker.ld -o kernel $BUILDDIR/kasm.o $BUILDDIR/kernel.o $BUILDDIR/stdio.o $BUILDDIR/stdproc.o $BUILDDIR/stdlib.o $BUILDDIR/panic.o $BUILDDIR/memory.o

echo "-------"
echo "Compilation succeeded!"
echo "-------"

qemu-system-i386 -kernel kernel
