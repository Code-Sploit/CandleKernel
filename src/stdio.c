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

void kstd_handle_escape(char __p)
{
	if (__p == '\n')
	{
		/* We need to get the current position in video memory */

		int pos = VIDEO_POINTER_SIZE;

		/* Now we need to see where we are in the row */

		pos = pos % SCREEN_SIZE_COLS;

		/* Then we need to compute the offset between the end of the line and where we are now */

		int offset = (SCREEN_SIZE_COLS * SCREEN_SIZE_BLEN) - pos;

		/* Now write a ' ' char for that offset */

		int i = 0;

		while (i < offset)
		{
			VIDEO_MEMORY_POINTER[i] = ' ';
			VIDEO_MEMORY_POINTER[i + 1] = ATTR_BYTE_BLK_ON_BLK;

			i = i + 2;
		}

		/* Globally increment VIDEO_POINTER_SIZE */

		VIDEO_POINTER_SIZE = VIDEO_POINTER_SIZE + offset;
	}
}

void kstd_write(const char *__sptr)
{
	unsigned int __string_len = (kstd_strlen(__sptr) * SCREEN_SIZE_BLEN);
	unsigned int __jp	  = VIDEO_POINTER_SIZE;
	unsigned int __ip	  = VIDEO_POINTER_SIZE;
	unsigned int __kp	  = 0;

	while (__ip < (VIDEO_POINTER_SIZE + __string_len))
	{
		VIDEO_MEMORY_POINTER[__ip] = ' ';
		VIDEO_MEMORY_POINTER[__ip + 1] = ATTR_BYTE_LG_ON_BLK;
		
		__ip = __ip + 2;
	}

	while (__jp < (VIDEO_POINTER_SIZE + __string_len))
	{
		if (__sptr[__kp] == '\n')
		{
			kstd_handle_escape(__sptr[__kp]);

			__jp = VIDEO_POINTER_SIZE;
			__kp = __kp + 1;

			continue;
		}

		VIDEO_MEMORY_POINTER[__jp] = __sptr[__kp];
		VIDEO_MEMORY_POINTER[__jp + 1] = ATTR_BYTE_LG_ON_BLK;

		__jp = __jp + 2;
		__kp = __kp + 1;
	}
	
	kstd_set_vidptr(VIDEO_POINTER_SIZE + __string_len);
}
