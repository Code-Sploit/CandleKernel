#ifndef MEMORY_H
#define MEMORY_H

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

typedef struct memory_block {
    struct {
        uint32 size;  // memory size
        uint8 is_free; // block is free or not
    } meta;

    struct memory_block *__next;

    void *__data;  // data pointer
} __attribute__((packed)) MEMORY_BLOCK;

/* Stdlib memory function */

BOOL kstd_mem_isfree(MEMORY_BLOCK *__BLOCK);

MEMORY_BLOCK *kstd_mem_worst(int __SIZE);

int kstd_mem_init(void *__START_ADDR, void *__END_ADDR);

void *kstd_mem_brk(int __SIZE);
void *kstd_mem_malloc(int __SIZE);
void *kstd_mem_calloc(int __N, int __SIZE);
void *kstd_mem_realloc(void *__PTR, int __SIZE);

void kstd_mem_free(void *__ADDR);

void kstd_itoa(char *__BUF, int __BASE, int __D);

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

void kstd_mem_print_blocks(void);

#endif
