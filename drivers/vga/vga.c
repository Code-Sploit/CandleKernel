#include "vga.h"

#include "../pio/pio.h"

#include "../../lib/stdlib.h"
#include "../../lib/memory.h"

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

void printf(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    kstd_memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            __kstd_vga_putchar(c, ATTR_BYTE_TEXT_COLOR_STD);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    kstd_itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        __kstd_vga_putchar(pad0 ? '0' : ' ', ATTR_BYTE_TEXT_COLOR_STD);
                    while (*p)
                        __kstd_vga_putchar(*p++, ATTR_BYTE_TEXT_COLOR_STD);
                    break;

                default:
                    __kstd_vga_putchar(*((int *)arg++), ATTR_BYTE_TEXT_COLOR_STD);
                    break;
            }
        }
    }
}
