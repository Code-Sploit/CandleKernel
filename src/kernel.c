/*
*  kernel.c
*/

#include "../lib/kernel.h"
#include "../lib/stdio.h"

void kmain(void)
{
	const char *_msg1 = "Booting up: CandleKernel!\n";
	const char *_msg2 = "Hello world!\n";
	const char *_msg3 = "Newline test\n";

	kstd_clear();

	kstd_write(_msg1);
	kstd_write(_msg2);
	kstd_write(_msg3);

	return;
}
