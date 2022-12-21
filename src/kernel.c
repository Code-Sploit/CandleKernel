/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"
#include "../lib/memory.h"

void kmain(void)
{
	/* Clear the screen */
	kstd_clear();

	/* Initialize memory */
	void *__KSTD_MEM_START_ADDR = kstd_mem_pre_alloc_blocks(20);
	void *__KSTD_MEM_END_ADDR	= __KSTD_MEM_START_ADDR + (kstd_mem_pre_next_free(1) * KSTD_MEM_BLOCK_SIZE);

	kstd_mem_init(__KSTD_MEM_START_ADDR, __KSTD_MEM_END_ADDR);

	/* Test allocating & deallocating a string */
	char *str = kstd_mem_malloc(sizeof(char));

	kstd_strcat(str, "Hello, World!\n");
	kstd_write(str);
	kstd_mem_free(str);

	return;
}
