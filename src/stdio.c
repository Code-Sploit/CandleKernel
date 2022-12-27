/*
 * stdio.c
*/

#include "../lib/stdio.h"
#include "../lib/memory.h"
#include "../lib/stdlib.h"

void kstd_set_vidptr(int __size)
{
	VIDEO_POINTER_SIZE = __size;
}

void kstd_free_space(int __size)
{
	unsigned int cursor = VIDEO_POINTER_SIZE;

	while (cursor < __size)
	{
		VIDEO_MEMORY_POINTER[cursor] = ' ';
		VIDEO_MEMORY_POINTER[cursor + 1] = ATTR_BYTE_BLK_ON_BLK;

		cursor = cursor + 2;
	}
}

void kstd_clear(void)
{
	unsigned int cursor = 0;

	while (cursor < (SCREEN_SIZE))
	{
		VIDEO_MEMORY_POINTER[cursor] = ' ';
		VIDEO_MEMORY_POINTER[cursor + 1] = ATTR_BYTE_BLK_ON_BLK;

		cursor = cursor + 2;
	}
}

void kstd_handle_escape(char __p)
{
	switch (__p)
	{
		case '\n':
		{
			int offset_to_eol = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VIDEO_POINTER_SIZE % SCREEN_SIZE_COLS);

			VIDEO_POINTER_SIZE = VIDEO_POINTER_SIZE + offset_to_eol;
		}
	}
}

void kstd_newline(void)
{
	int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - (VIDEO_POINTER_SIZE % SCREEN_SIZE_COLS);

	VIDEO_POINTER_SIZE = (VIDEO_POINTER_SIZE + offset);
}

void kstd_putchar(char __c)
{
	VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE]     = __c;
	VIDEO_MEMORY_POINTER[VIDEO_POINTER_SIZE + 1] = 0x07;

	VIDEO_POINTER_SIZE += 2;
}

void kstd_write(const char *__sptr)
{
	unsigned int strlen  = kstd_strlen(__sptr);

	unsigned int viditer = VIDEO_POINTER_SIZE;
	unsigned int striter = 0;

	kstd_free_space(strlen);

	while (striter < strlen)
	{
		if (__sptr[striter] == '\n')
		{
			kstd_newline();

			viditer = VIDEO_POINTER_SIZE;

			striter++;

			continue;
		}

		VIDEO_MEMORY_POINTER[viditer] = __sptr[striter];
		VIDEO_MEMORY_POINTER[viditer + 1] = ATTR_BYTE_TEXT_COLOR_STD;

		viditer = viditer + 2;

		striter++;
	}

	VIDEO_POINTER_SIZE = viditer;
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
			kstd_putchar(c);
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
						kstd_putchar(pad_x ? '0' : ' ');

					while (*__p1)
						kstd_putchar(*__p1);

					break;

				default:
				{
					kstd_putchar(*((int *) __args++));

					break;
				}
			}
		}
	}
}
