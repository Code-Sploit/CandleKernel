/*
 * stdio.c
*/

#include "../lib/stdio.h"
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
			kstd_handle_escape(__sptr[striter]);

			viditer = viditer + 2;

			striter++;

			continue;
		}

		VIDEO_MEMORY_POINTER[viditer] = __sptr[striter];
		VIDEO_MEMORY_POINTER[viditer + 1] = ATTR_BYTE_LG_ON_BLK;

		viditer = viditer + 2;

		striter++;
	}
}
