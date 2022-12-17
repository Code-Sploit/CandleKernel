/*
 * stdio.c
*/

#include "../lib/stdio.h"

void kstd_write(const char *__sptr)
{
	unsigned int __ip = 0;
	unsigned int __jp = 0;

	while (__jp < (SCREEN_SIZE))
	{
		VIDEO_MEMORY_POINTER[__jp] = ' ';
		VIDEO_MEMORY_POINTER[__jp + 1] = ATTR_BYTE_LG_ON_BLK;

		__jp = __jp + 2;
	}

	__jp = 0;

	while (__sptr[__jp] != '\0')
	{
		VIDEO_MEMORY_POINTER[__ip] = __sptr[__jp];
		VIDEO_MEMORY_POINTER[__ip + 2] = ATTR_BYTE_LG_ON_BLK;

		++__jp;

		__ip = __ip + 2;
	}
}
