/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"
#include "../lib/memory.h"
#include "../lib/stdproc.h"

#include "../drivers/idt/idt.h"
#include "../drivers/isr/isr.h"
#include "../drivers/ide/ide.h"
#include "../drivers/keyboard/keyboard.h"

#include "../drivers/gdt/mgdt.h"
#include "../drivers/ext2/ext2.h"

#define assert(condition) (condition == 0) ? 0 : 1

#define __KSTD_MEMORY_BLOCK_ALLOC 20

void kmain(void)
{
	kstd_clear();
	
	int __gdt_status = __kstd_mgdt_enable();
	int __isr_status = __kstd_isr_enable();
	int __key_status = __kstd_enable_keyboard();
	int __ide_status = __kstd_ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
	int __asr_status = 0;

	kstd_clear();

	kstd_write("Initializing VGA buffer...");

	__asr_status = assert(__gdt_status == 0); (__asr_status) != 0 ? kstd_writec("[Done]\n", ATTR_BYTE_GRN_ON_BLK) : kstd_writec("[Failed]\n", ATTR_BYTE_RED_ON_BLK);

	kstd_write("Initializing ISR handler...");
	
	__asr_status = assert(__isr_status == 0); (__asr_status) != 0 ? kstd_writec("[Done]\n", ATTR_BYTE_GRN_ON_BLK) : kstd_writec("[Failed]\n", ATTR_BYTE_RED_ON_BLK);
	
	kstd_write("Initializing Keyboard...");
	
	__asr_status = assert(__key_status == 0); (__asr_status) != 0 ? kstd_writec("[Done]\n", ATTR_BYTE_GRN_ON_BLK) : kstd_writec("[Failed]\n", ATTR_BYTE_RED_ON_BLK);

	void *__KSTD_MEM_START_ADDR = kstd_mem_pre_alloc_blocks(__KSTD_MEMORY_BLOCK_ALLOC);
	void *__KSTD_MEM_END_ADDR	= __KSTD_MEM_START_ADDR + (kstd_mem_pre_next_free(1) * KSTD_MEM_BLOCK_SIZE);

	int __mem_status = kstd_mem_init(__KSTD_MEM_START_ADDR, __KSTD_MEM_END_ADDR);

	kstd_write("Initializing 20 blocks of memory...");

	__asr_status = assert(__mem_status == 0); (__asr_status) != 0 ? kstd_writec("[Done]\n", ATTR_BYTE_GRN_ON_BLK) : kstd_writec("[Failed]\n", ATTR_BYTE_RED_ON_BLK);

	kstd_write("Initializing IDE...");

	__asr_status = assert(__ide_status == 0); (__asr_status) != 0 ? kstd_writec("[Done]\n", ATTR_BYTE_GRN_ON_BLK) : kstd_writec("[Failed]\n", ATTR_BYTE_RED_ON_BLK);

	__kstd_ext2_read_superblock();
	__kstd_ext2_init();

	kstd_write("\nSuccessfully booted: CandleOS!\n");
	kstd_write("\nroot@candleos > ");
}
