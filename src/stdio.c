/*
 * stdio.c
*/

#include "../lib/stdio.h"
#include "../lib/stdlib.h"
#include "../lib/memory.h"

void kstd_write(const char *__sptr)
{
	unsigned int strlen  = __kstd_strlen(__sptr);
	unsigned int striter = 0;

	int cursor = __kstd_vga_get_cursor();

	while (striter < strlen)
	{
		__kstd_vga_putchar(__sptr[striter], ATTR_BYTE_TEXT_COLOR_STD);
		__kstd_vga_set_cursor(VIDEO_POINTER_SIZE);

		striter++;
	}
}

void kstd_writec(const char *__sptr, uint16 __bcolor)
{
	unsigned int strlen  = __kstd_strlen(__sptr);
	unsigned int striter = 0;

	while (striter < strlen)
	{
		__kstd_vga_putchar(__sptr[striter], __bcolor);
		__kstd_vga_set_cursor(VIDEO_POINTER_SIZE);

		striter++;
	}
}

void kstd_clear(void)
{
	__kstd_vga_clear();
}

void kstd_backspace(void)
{
	__kstd_vga_backspace();
}

void printk(const char *__format, ...)
{
	char **__args = (char **)__format;
	char __buf[32];

	int c;

	__args++;

	kstd_memset(__buf, 0, sizeof(__buf));

	while ((c = *__format++) != 0)
	{
		if (c != '%')
		{
			__kstd_vga_putchar(c, ATTR_BYTE_TEXT_COLOR_STD);
		}
		else
		{
			char *__p1;
			char *__p2;

			int pad_x;
			int pad_y;

			c = *__format++;

			if (c == '0')
			{
				pad_x = 1;

				c = *__format++;
			}

			if (c >= '0' && c <= '9')
			{
				pad_y = c - '0';

				c = *__format++;
			}

			switch (c)
			{
				case 'd':
				case 'u':
				case 'x':
				{
					kstd_itoa(__buf, c, *((int *) __args++));

					__p1 = __buf;

					goto __puts;

					break;
				}

				case 's':
				{
					__p1 = *__args++;

					if (!__p1)
					{
						__p1 = "(__NULL__)";
					}
				}

				__puts:
					for (__p2 = __p1; *__p2; __p2++)
						;

					for (; __p2 < __p1 + pad_y; __p2++)
						__kstd_vga_putchar(pad_x ? '0' : ' ', ATTR_BYTE_TEXT_COLOR_STD);

					while (*__p1)
						__kstd_vga_putchar(*__p1, ATTR_BYTE_TEXT_COLOR_STD);

					break;

				default:
				{
					__kstd_vga_putchar(*((int *) __args++), ATTR_BYTE_TEXT_COLOR_STD);

					break;
				}
			}
		}
	}
}
