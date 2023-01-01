#include "../lib/kapi.h"

#include "../lib/stdio.h"
#include "../lib/memory.h"

void __kstd_kapi_run(__kstd_kapi_t *__obj)
{
    if (!(__obj->__vlid == KAPI_ACTIVE))
    {
        return;
    }

    if (__obj->__call == KAPI_HALT)
    {
        kstd_mem_free(__obj);

        kstd_write("\nKERNEL: Received syscall [HALT]!\n");

        asm volatile ("hlt");
    }

    if (__obj->__call == KAPI_REBOOT)
    {
        kstd_mem_free(__obj);

        kstd_write("\nKERNEL: Received syscall [REBOOT]!\n");

        uint64 null = 0;

        asm ("xor %%eax, %%eax; lidt %0; int3" :: "m" (null));
    }

    if (__obj->__call == KAPI_VGA_CLEAR)
    {
        kstd_mem_free(__obj);

        kstd_write("\nKERNEL: Received syscall [REBOOT]!\n");

        __kstd_vga_clear();
    }
}