#include "../lib/memory.h"
#include "../lib/panic.h"
#include "../lib/stdio.h"

static KSTD_MEM_INFO kstd_mem_info;

void *G_MEMORY_START_ADDR = NULL;
void *G_MEMORY_END_ADDR   = NULL;

unsigned long MEMORY_TOTAL_SIZE = 0;
unsigned long MEMORY_USED_SIZE  = 0;

MEMORY_BLOCK *MEM_HEAD = NULL;

/* 4 private functions (inline) */

static inline void kstd_mem_mmap_set(int __BIT)
{
    kstd_mem_info.__mem_map_array[__BIT / 32] |= (1 << (__BIT % 32));
}

static inline void kstd_mem_mmap_unset(int __BIT)
{
    kstd_mem_info.__mem_map_array[__BIT / 32] &= ~(1 << (__BIT % 32));
}

static inline char kstd_mem_mmap_test(int __BIT)
{
    return kstd_mem_info.__mem_map_array[__BIT / 32] & (1 << (__BIT % 32));
}

static inline int kstd_mem_mmap_ffbs(uint32 __SIZE)
{
    uint32 __ip, __jp, __kp, __free = 0;

    int __BIT;

    if (__SIZE == 0)
    {
        return -1;
    }

    if (__SIZE == 1)
    {
        return kstd_mem_first_free();
    }

    for (__ip = 0; __ip < kstd_mem_info.__maxblocks; __ip++)
    {
        if (kstd_mem_info.__mem_map_array[__ip] != 0xffffffff)
        {
            for (__jp = 0; __jp < 32; __jp++)
            {
                __BIT = 1 << __jp;

                if (!(kstd_mem_info.__mem_map_array[__ip] & __BIT))
                {
                    for (__kp = __jp; __kp < 32; __kp++)
                    {
                        __BIT = 1 << __kp;

                        if (!(kstd_mem_info.__mem_map_array[__ip] & __BIT));
                        {
                            __free++;
                        }

                        if (__free == __SIZE)
                        {
                            return __ip * 32 + __jp;
                        }
                    }
                }
            }
        }
    }

    return -1;
}

/* Memory changing functions */

void *kstd_memset(void *__DEST, char __C, uint32 __N)
{
    char *__TEMP = __DEST;

    for (; __N != 0; __N--) *__TEMP++ = __C;
    
    return __DEST;
}

void *kstd_memcpy(void *__DEST, const void *__SOURCE, uint32 __N)
{
    char *__RET = __DEST;
    char *__PTR = __DEST;

    const char *__QTR = __SOURCE;
    
    while (__N--)
        *__PTR++ = *__QTR++;

    return __RET;
}

/* String functions */

int kstd_strlen(const char *__SOURCE)
{
    int __len = 0;

    while (__SOURCE[__len] != '\0')
    {
        __len++;
    }

    return __len;
}

int kstd_strcpy(char *__DEST, const char *__SOURCE)
{
    int __ip = 0;

    while ((*__DEST++ = *__SOURCE++) != 0)
    {
        __ip++;
    }

    return __ip;
}

void kstd_strcat(char *__DEST, const char *__SOURCE)
{
    char *__END = (char *) __DEST + kstd_strlen(__DEST);

    kstd_memcpy((void *) __END, (void *) __SOURCE, kstd_strlen(__SOURCE));

    __END = __END + kstd_strlen(__SOURCE);

    *__END = '\0';
}

/* Public functions */

uint32 kstd_mem_get_max_blocks(void)
{
    return kstd_mem_info.__maxblocks;
}

uint32 kstd_mem_get_used_blocks(void)
{
    return kstd_mem_info.__usedblocks;
}

int kstd_mem_first_free()
{
    uint32 __ip, __jp;

    for (__ip = 0; __ip < kstd_mem_info.__maxblocks; __ip++)
    {
        if (kstd_mem_info.__mem_map_array[__ip] != 0xffffffff)
        {
            for (__jp = 0; __jp < 32; __jp++)
            {
                int __BIT = 1 << __jp;

                if (!(kstd_mem_info.__mem_map_array[__ip] & __BIT))
                {
                    return __ip * 32 + __jp;
                }
            }
        }
    }

    return -1;
}

int kstd_mem_pre_next_free(int __SIZE)
{
    return kstd_mem_mmap_ffbs(__SIZE);
}

void kstd_mem_pre_init(KSTD_MEM_PHYSICAL_ADDRESS __BITMAP, uint32 __t_memsize)
{
    kstd_mem_info.__memsize       = __t_memsize;
    kstd_mem_info.__mem_map_array = (uint32 *) __BITMAP;
    kstd_mem_info.__maxblocks     = (__t_memsize / KSTD_MEM_BLOCK_SIZE);
    kstd_mem_info.__usedblocks    = kstd_mem_info.__maxblocks;

    kstd_memset(kstd_mem_info.__mem_map_array, 0xff, (kstd_mem_info.__maxblocks * sizeof(uint32)));

    kstd_mem_info.__mem_map_end = (uint32) &kstd_mem_info.__mem_map_array[kstd_mem_info.__maxblocks];
}

void kstd_mem_pre_init_region(KSTD_MEM_PHYSICAL_ADDRESS __BASE, uint32 __region_size)
{
    int __ALIGN  = (__BASE / KSTD_MEM_BLOCK_SIZE);
    int __BLOCKS = (__region_size / KSTD_MEM_BLOCK_SIZE);

    while (__BLOCKS >= 0)
    {
        kstd_mem_mmap_unset(__ALIGN++);

        kstd_mem_info.__usedblocks--;
        
        __BLOCKS--;
    }
}

void kstd_mem_pre_deinit_region(KSTD_MEM_PHYSICAL_ADDRESS __BASE, uint32 __region_size)
{
    int __ALIGN  = (__BASE / KSTD_MEM_BLOCK_SIZE);
    int __BLOCKS = (__region_size / KSTD_MEM_BLOCK_SIZE);

    while (__BLOCKS >= 0)
    {
        kstd_mem_mmap_set(__ALIGN++);

        kstd_mem_info.__usedblocks++;

        __BLOCKS--;
    }
}

void *kstd_mem_pre_alloc_block(void)
{
    if ((kstd_mem_info.__maxblocks - kstd_mem_info.__usedblocks) <= 0)
    {
        /* If debug warn kernel MSG */

        return NULL;
    }

    int __FRAME = kstd_mem_first_free();

    if (__FRAME == -1)
    {
        /* If debug warn kernel MSG */

        return NULL;
    }

    kstd_mem_mmap_set(__FRAME);

    KSTD_MEM_PHYSICAL_ADDRESS __ADDR = (__FRAME * KSTD_MEM_BLOCK_SIZE) + kstd_mem_info.__mem_map_end;

    kstd_mem_info.__usedblocks++;

    return (void *) __ADDR;
}

void *kstd_mem_pre_alloc_blocks(uint32 __SIZE)
{
    uint32 __ip;

    if ((kstd_mem_info.__maxblocks - kstd_mem_info.__usedblocks) <= __SIZE)
    {
        /* If debug warn kernel MSG */

        return NULL;
    }

    int __FRAME = kstd_mem_mmap_ffbs(__SIZE);

    if (__FRAME == -1)
    {
        return NULL;
    }

    for (__ip = 0; __ip < __SIZE; __ip++)
    {
        kstd_mem_mmap_set(__FRAME + __ip);
    }

    KSTD_MEM_PHYSICAL_ADDRESS __ADDR = (__FRAME * KSTD_MEM_BLOCK_SIZE) + kstd_mem_info.__mem_map_end;

    kstd_mem_info.__usedblocks = (kstd_mem_info.__usedblocks + __SIZE);

    return (void *) __ADDR;
}

void kstd_mem_pre_free_block(void *__BLOCK)
{
    KSTD_MEM_PHYSICAL_ADDRESS __ADDR = (KSTD_MEM_PHYSICAL_ADDRESS) __BLOCK;

    __ADDR = (__ADDR - kstd_mem_info.__mem_map_end);

    int __FRAME = (__ADDR / KSTD_MEM_BLOCK_SIZE);

    kstd_mem_mmap_unset(__FRAME);

    kstd_mem_info.__usedblocks = (kstd_mem_info.__usedblocks - 1);
}

void kstd_mem_pre_free_blocks(void *__BLOCK, uint32 __SIZE)
{
    uint32 __ip;

    KSTD_MEM_PHYSICAL_ADDRESS __ADDR = (KSTD_MEM_PHYSICAL_ADDRESS) __BLOCK;

    __ADDR = (__ADDR - kstd_mem_info.__mem_map_end);

    int __FRAME = (__ADDR / KSTD_MEM_BLOCK_SIZE);

    for (__ip = 0; __ip < __SIZE; __ip++)
    {
        kstd_mem_mmap_unset(__FRAME + __ip);
    }

    kstd_mem_info.__usedblocks = (kstd_mem_info.__usedblocks - __SIZE);
}

int kstd_mem_init(void *__START_ADDR, void *__END_ADDR)
{
	if (G_MEMORY_START_ADDR > G_MEMORY_END_ADDR)
	{
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

	__ADDR = G_MEMORY_START_ADDR + MEMORY_USED_SIZE + __SIZE + sizeof(void *);

	MEMORY_USED_SIZE += __SIZE + sizeof(void *);

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

void tostring(char str[], int num)
{
    int i, rem, len = 0, n;
 
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void kstd_mem_print_blocks(void)
{
    MEMORY_BLOCK *__BLOCK = MEM_HEAD;

    int __block_count = 0;

    char _size[256];

    while (__BLOCK != NULL)
    {
        kstd_itoa(_size, 10, __BLOCK->meta.size);

        kstd_write("Size: ");
        kstd_write(_size);
        kstd_write("\nFree: ");
        
        assert(__BLOCK->meta.is_free == 0) ? kstd_write("NO") : kstd_write("YES");
        
        kstd_write("\n\n");

        __BLOCK = __BLOCK->__next;

        __block_count++;
    }

    char _bcs[256];

    kstd_itoa(_bcs, 10, __block_count);

    kstd_write("Total block count: ");
    kstd_write(_bcs);
    kstd_write("\n");
}