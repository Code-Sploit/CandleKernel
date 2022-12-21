#include "../lib/stdlib.h"
#include "../lib/panic.h"

/* Memory definitions */

void *G_MEMORY_START_ADDR = NULL;
void *G_MEMORY_END_ADDR   = NULL;

unsigned long MEMORY_TOTAL_SIZE = 0;
unsigned long MEMORY_USED_SIZE  = 0;

MEMORY_BLOCK *MEM_HEAD;

/* Memory */

int kstd_mem_init(void *__START_ADDR, void *__END_ADDR)
{
	if (G_MEMORY_START_ADDR > G_MEMORY_END_ADDR)
	{
		kstd_panic("Failed to initialize memory!\n");

		return -1;
	}

	G_MEMORY_START_ADDR = __START_ADDR;
	G_MEMORY_END_ADDR   = __END_ADDR;

	MEMORY_TOTAL_SIZE = (__START_ADDR - __END_ADDR);
	MEMORY_USED_SIZE  = 0;

	return 0;
}

void *kstd_mem_brk(int __SIZE)
{
	void *__ADDR = NULL;

	if (__SIZE <= 0)
	{
		kstd_warn("Warning: kstd_mem_brk() called with a SIZE <= 0\n");

		return NULL;
	}

	if ((int) (MEMORY_TOTAL_SIZE - MEMORY_USED_SIZE) <= __SIZE)
	{
		kstd_warn("Warning: kstd_mem_brk() out of memory!\n");

		return NULL;
	}

	__ADDR = (G_MEMORY_START_ADDR + MEMORY_TOTAL_SIZE) + __SIZE + sizeof(void *);

	MEMORY_USED_SIZE = (MEMORY_USED_SIZE + __SIZE + sizeof(void *));

	return __ADDR;
}

BOOL kstd_mem_isfree(MEMORY_BLOCK *__BLOCK)
{
	if (!__BLOCK)
	{
		return FALSE;
	}

	return (__BLOCK->meta.is_free == TRUE);
}

MEMORY_BLOCK *kstd_mem_worst(int __SIZE)
{
    MEMORY_BLOCK *__BTEMP = MEM_HEAD;

    while (__BTEMP != NULL)
    {
        if (kstd_mem_isfree(__BTEMP))
        {
            if ((int) __BTEMP->meta.size >= __SIZE)
            {
                return __BTEMP;
            }
        }

        __BTEMP = __BTEMP->__next;
    }

    return NULL;
}

MEMORY_BLOCK *kstd_mem_new_block(int __SIZE)
{
	MEMORY_BLOCK *__TEMP = MEM_HEAD;

	while (__TEMP->__next != NULL)
	{
		__TEMP = __TEMP->__next;
	}

	MEMORY_BLOCK *__NBLOCK = (MEMORY_BLOCK *) kstd_mem_brk(sizeof(MEMORY_BLOCK));

	__NBLOCK->meta.is_free = FALSE;
	__NBLOCK->meta.size	   = __SIZE;
	__NBLOCK->__data	   = kstd_mem_brk(__SIZE);
	__NBLOCK->__next	   = NULL;

	__TEMP->__next = __NBLOCK;

	return __NBLOCK;
}

void *kstd_mem_malloc(int __SIZE)
{
	if (__SIZE <= 0)
	{
		return NULL;
	}

	if (MEM_HEAD == NULL)
	{
		MEM_HEAD = (MEMORY_BLOCK *) kstd_mem_brk(sizeof(MEMORY_BLOCK));

		MEM_HEAD->meta.is_free = FALSE;
		MEM_HEAD->meta.size	   = __SIZE;
		MEM_HEAD->__data	   = kstd_mem_brk(__SIZE);
		MEM_HEAD->__next	   = NULL;

		return MEM_HEAD->__data;
	}
	else
	{
		MEMORY_BLOCK *__WORST = kstd_mem_worst(__SIZE);

		if (__WORST == NULL)
		{
			MEMORY_BLOCK *__BLOCK = kstd_mem_new_block(__SIZE);

			__BLOCK->meta.is_free = FALSE;
			__BLOCK->meta.size	  = __SIZE;
			__BLOCK->__data		  = kstd_mem_brk(__SIZE);

			return __BLOCK->__data;
		}
		else
		{
			__WORST->meta.is_free = FALSE;

			return __WORST->__data;
		}
	}

	return NULL;
}

void *kstd_mem_calloc(int __N, int __SIZE)
{
	/* Todo implement memset */
}

void *kstd_mem_realloc(void *__PTR, int __SIZE)
{
	/* Todo implement realloc */
}

void kstd_mem_free(void *__ADDR)
{
	MEMORY_BLOCK *__BTEMP = MEM_HEAD;

	while (__BTEMP != NULL)
	{
		if (__BTEMP->__data == __ADDR)
		{
			__BTEMP->meta.is_free = TRUE;

			return;
		}

		__BTEMP = __BTEMP->__next;
	}
}

/* Assert string functions */

int kstd_isupper(char __c)
{
	if (__c >= 'A' && __c <= 'Z')
	{
		return TRUE;
	}

	return FALSE;
}

int kstd_islower(char __c)
{
	if (__c >= 'a' && __c <= 'z')
	{
		return TRUE;
	}

	return FALSE;
}

char kstd_toupper(char __c)
{
	if (__c >= 'a' && __c <= 'z')
	{
		return (__c - 32);
	}

	return __c;
}

char kstd_tolower(char __c)
{
	if (__c >= 'A' && __c <= 'Z')
	{
		return (__c + 32);
	}

	return __c;
}

int kstd_cmatch(char __c1, char __c2)
{
	if (__c1 == __c2)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

int kstd_strcmp(char *__s1, char *__s2)
{
	int __len1 = kstd_strlen(__s1);
	int __len2 = kstd_strlen(__s2);

	if (__len1 == 0 && __len2 == 0)
	{
		return TRUE;
	}

	if (__len1 != __len2)
	{
		return FALSE;
	}

	for (int i = 0; i < __len1; i++)
	{
		int _cmatch = kstd_cmatch(__s1[i], __s2[i]);

		if (_cmatch == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}
