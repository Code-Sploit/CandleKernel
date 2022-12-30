/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"
#include "../lib/memory.h"
#include "../lib/stdproc.h"

#include "../drivers/idt/idt.h"
#include "../drivers/isr/isr.h"
#include "../drivers/keyboard/keyboard.h"

void kmain(void)
{
	kstd_clear();

	__kstd_isr_enable();
	asm volatile("sti");
	__kstd_enable_keyboard();

	kstd_clear();

	kstd_write("Initializing VGA buffer... [Done]\n");
	kstd_write("Initializing ISR handler... [Done]\n");
	kstd_write("Initializing Keyboard... [Done]\n");

	/* Memory */

	void *__KSTD_MEM_START_ADDR = kstd_mem_pre_alloc_blocks(20);
	void *__KSTD_MEM_END_ADDR	= __KSTD_MEM_START_ADDR + (kstd_mem_pre_next_free(1) * KSTD_MEM_BLOCK_SIZE);

	kstd_mem_init(__KSTD_MEM_START_ADDR, __KSTD_MEM_END_ADDR);

	kstd_write("Initializing 20 blocks of memory... [Done]\n");

	kstd_write("\nWelcome to CandleOS\nAwaiting Commands...\nroot@candleos > ");

	return;
}
