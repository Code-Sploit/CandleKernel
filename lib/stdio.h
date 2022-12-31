#ifndef STDIO_H
#define STDIO_H

#include "../drivers/vga/vga.h"
#include "../lib/stdlib.h"

void kstd_write(const char *__sptr);
void kstd_writec(const char *__sptr, uint16 __bcolor);

void kstd_clear(void);

void printk(const char *__format, ...);

#endif
