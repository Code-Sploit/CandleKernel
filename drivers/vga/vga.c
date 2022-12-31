#include "vga.h"

#include "../pio/pio.h"

#include "../../lib/stdlib.h"

void __kstd_vga_set_cursor(int __offset)
{
    __offset /= 2;

    __kstd_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    __kstd_outb(VGA_DATA_REGISTER, (unsigned char) (__offset >> 8));
    __kstd_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    __kstd_outb(VGA_DATA_REGISTER, (unsigned char) (__offset & 0xff));
}

int __kstd_vga_get_cursor(void)
{
    __kstd_outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);

    int __offset = __kstd_inb(VGA_DATA_REGISTER) << 8;
    
    __kstd_outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    
    __offset += __kstd_inb(VGA_DATA_REGISTER);
    
    return __offset * 2;
}

int __kstd_vga_row_from_offset(int __offset) {
    return __offset / (2 * SCREEN_SIZE_COLS);
}

int __kstd_vga_line_from_offset(int __col, int __row) {
    return 2 * (__row * SCREEN_SIZE_COLS + __col);
}

void __kstd_vga_newline(void)
{
    VIDEO_POINTER_SIZE = __kstd_vga_line_from_offset(0, __kstd_vga_row_from_offset(VIDEO_POINTER_SIZE) + 1);
}

void __kstd_vga_tab(void)
{
	int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VIDEO_POINTER_SIZE % SCREEN_SIZE_COLS);

	if (offset < 4)
	{
		int remaining = 4 - offset;

		__kstd_vga_newline();

		VIDEO_POINTER_SIZE = (VIDEO_POINTER_SIZE) + remaining;

		return;
	}

	VIDEO_POINTER_SIZE = (VIDEO_POINTER_SIZE + 4);
}

void __kstd_vga_putchar(char __c, unsigned short __cl)
{
    if (__c == '\0')
    {
        return;
    }

    if (VIDEO_POINTER_SIZE >= SCREEN_SIZE)
    {
        __kstd_vga_clear();
    }

    if (__c == '\n')
    {
        __kstd_vga_newline();

        return;
    }

    if (__c == '\t')
    {
        __kstd_vga_tab();

        return;
    }

    VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE]     = __c;
    VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE + 1] = __cl;

    VIDEO_POINTER_SIZE = (VIDEO_POINTER_SIZE + 2);
}

void __kstd_vga_clear(void)
{
    VIDEO_POINTER_SIZE = 0;

    unsigned int __viter = VIDEO_POINTER_SIZE;

    while (__viter < SCREEN_SIZE)
    {
        __kstd_vga_putchar(' ', ATTR_BYTE_BLK_ON_BLK);

        __viter = VIDEO_POINTER_SIZE;
    }

    VIDEO_POINTER_SIZE = 0;
}

void __kstd_vga_backspace(void)
{
	VIDEO_POINTER_SIZE = (VIDEO_POINTER_SIZE - 2);

	VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE] = ' ';
	VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE + 1] = ATTR_BYTE_BLK_ON_BLK;
}
