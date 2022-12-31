#include "console.h"

#include "../../lib/memory.h"
#include "../../lib/stdlib.h"
#include "../../lib/stdio.h"

#include "../keyboard/keyboard.h"

void __kstd_execute_shutdown(void)
{
    kstd_write("\nSystem halted!\n");

    asm volatile("hlt");
}

void __kstd_execute_reboot(void)
{
    kstd_write("\nSystem rebooting!\n");

    uint64 null_idtr = 0;
    
    asm("xor %%eax, %%eax; lidt %0; int3" :: "m" (null_idtr));
}

void __kstd_execute_help(void)
{
    kstd_write("\nHelp window:\n");
    kstd_write("HELP:\t\tDisplays this help message.\n");
    kstd_write("MPRINT:  Prints current memory allocation table.\n");
    kstd_write("CLEAR: Clear the screen.\n");
    kstd_write("HALT: \t Halts the CPU.\n");
    kstd_write("REBOOT:\tReboots the system.\n");
}

void __kstd_execute_clear(void)
{
    __kstd_vga_clear();
}

void __kstd_execute_print_mem_blocks(void)
{
    kstd_mem_print_blocks();
}

void __kstd_console_run(char *__cmd)
{
    if (__kstd_strcmp(__cmd, __COMMAND_HALT) == 0)
    {
        __kstd_execute_shutdown(); 
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_REBOOT) == 0)
    {
        __kstd_execute_reboot();
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_HELP) == 0)
    {
        __kstd_execute_help();
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_PRINT_BLOCKS) == 0)
    {
        __kstd_execute_print_mem_blocks();
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_CLEAR) == 0)
    {
        __kstd_execute_clear();
    }
    else
    {
        kstd_write("Unknown command: ");
        kstd_write(__cmd);
        kstd_write("\n");
    }
}