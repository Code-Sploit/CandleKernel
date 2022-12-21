#include "stdlib.h"

#define KSTD_MEM_BLOCK_SIZE 4096

typedef uint32 KSTD_MEM_PHYSICAL_ADDRESS;

typedef struct KSTD_MEM_INFO {
    uint32 __memsize;
    uint32 __maxblocks;
    uint32 __usedblocks;
    uint32 __mem_map_end;

    uint32 *__mem_map_array;
} KSTD_MEM_INFO;

void *kstd_memcpy(void *__DEST, const void *__SOURCE, uint32 __N);
void *kstd_memset(void *__DEST, char __C, uint32 __N);

uint32 kstd_mem_get_max_blocks(void);
uint32 kstd_mem_get_used_blocks(void);

int kstd_mem_pre_next_free(int __SIZE);
int kstd_mem_first_free();

void kstd_mem_pre_init(KSTD_MEM_PHYSICAL_ADDRESS __BITMAP, uint32 __t_memsize);
void kstd_mem_pre_init_region(KSTD_MEM_PHYSICAL_ADDRESS __BASE, uint32 __region_size);
void kstd_mem_pre_deinit_region(KSTD_MEM_PHYSICAL_ADDRESS __BASE, uint32 __region);

void *kstd_mem_pre_alloc_block(void);
void *kstd_mem_pre_alloc_blocks(uint32 __SIZE);

void kstd_mem_pre_free_block(void *__BLOCK);
void kstd_mem_pre_free_blocks(void *__BLOCK, uint32 __SIZE);


int kstd_strlen(const char *__SOURCE);
int kstd_strcpy(char *__DEST, const char *__SOURCE);

void kstd_strcat(char *__DEST, const char *__SOURCE);
