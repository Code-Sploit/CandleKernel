/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"
#include "../lib/memory.h"
#include "../lib/stdproc.h"

void kmain(void)
{
	/* Clear the screen */
	kstd_clear();

	/* Initialize memory */
	void *__KSTD_MEM_START_ADDR = kstd_mem_pre_alloc_blocks(20);
	void *__KSTD_MEM_END_ADDR	= __KSTD_MEM_START_ADDR + (kstd_mem_pre_next_free(1) * KSTD_MEM_BLOCK_SIZE);

	kstd_mem_init(__KSTD_MEM_START_ADDR, __KSTD_MEM_END_ADDR);

	/* Process management testing */

	__initialize_proc();

	int pid = __create_proc(__PROCESS_RUNNING, 1);
	int pid2 = __create_proc(__PROCESS_RUNNING, 1);

	__print_procs();

	return;
}
