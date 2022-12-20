#include "../lib/panic.h"
#include "../lib/stdio.h"

void kstd_panic(const char *__M)
{
    kstd_write("KERNEL PANIC!\n");
    kstd_write("Info: ");
    kstd_write(__M);
    kstd_write("\n");

    asm volatile("hlt");
}

void kstd_warn(const char *__M)
{
    kstd_write("KERNEL WARNING!\n");
    kstd_write("Info: ");
    kstd_write(__M);
    kstd_write("\n");
}

void kstd_report(const char *__M)
{
    kstd_write("KERNEL INFO!\n");
    kstd_write("Info: ");
    kstd_write(__M);
    kstd_write("\n");
}
