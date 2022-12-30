C_SOURCES = $(wildcard src/*.c drivers/console/*.c drivers/idt/*.c drivers/isr/*.c drivers/keyboard/*.c drivers/pio/*.c)
HEADERS = $(wildcard lib/*.h  drivers/console/*.h drivers/idt/*.h drivers/isr/*.h drivers/keyboard/*.h drivers/pio/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o asm/interrupts.o asm/pio.o}

all: run

kernel.bin: asm/kentry.o ${OBJ_FILES}
	i686-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: asm/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

kernel.elf: asm/kernel_entry.o ${OBJ_FILES}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
	gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	i686-elf-gcc -g -m32 -ffreestanding -c $< -o $@ # -g for debugging

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@
