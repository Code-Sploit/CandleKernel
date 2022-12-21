#include "../lib/memory.h"

static KSTD_MEM_INFO kstd_mem_info;

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
