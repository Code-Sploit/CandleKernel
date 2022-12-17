/*
 * stdio.c
*/

#include "../lib/stdio.h"
#include "../lib/stdlib.h"

void kstd_set_vidptr(int __size)
{
	VIDEO_POINTER_SIZE = __size;
}

void kstd_clear(void)
{
	unsigned int __jp = 0;

	while (__jp < (SCREEN_SIZE))
	{
		VIDEO_MEMORY_POINTER[__jp] = ' ';
		VIDEO_MEMORY_POINTER[__jp + 1] = ATTR_BYTE_BLK_ON_BLK;

		__jp = __jp + 2;
	}
}

void kstd_write(const char *__sptr)
{
	unsigned int __string_len = (kstd_strlen(__sptr) * 2);
	unsigned int __ip         = VIDEO_POINTER_SIZE;
	unsigned int __jp	  = VIDEO_POINTER_SIZE;
	unsigned int __kp	  = 0;

	while (__ip < (VIDEO_POINTER_SIZE + __string_len))
	{
		VIDEO_MEMORY_POINTER[__ip] = ' ';
		VIDEO_MEMORY_POINTER[__ip + 1] = ATTR_BYTE_LG_ON_BLK;
		
		__ip = __ip + 2;
	}

	while (__jp < (VIDEO_POINTER_SIZE + __string_len))
	{
		VIDEO_MEMORY_POINTER[__jp] = __sptr[__kp];
		VIDEO_MEMORY_POINTER[__jp + 1] = ATTR_BYTE_LG_ON_BLK;

		__jp = __jp + 2;
		__kp = __kp + 1;
	}
	
	kstd_set_vidptr(VIDEO_POINTER_SIZE + __string_len);
}
