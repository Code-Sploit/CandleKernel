/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdlib.h"
#include "../lib/stdio.h"

void kmain(void)
{
	/* Clear the screen */
	kstd_clear();

	/* Initialize memory */
	kstd_mem_init(NULL, (void *) 16000);

	/* Test allocating a string */

	char *str1 = kstd_mem_malloc(sizeof(char));

	return;
}
