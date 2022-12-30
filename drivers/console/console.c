#include "console.h"

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
    kstd_write("\nClearing keyboard buffer!\n");

    __kstd_keyboard_buffer[0] = '\0';

    uint64 null_idtr = 0;
    
    asm("xor %%eax, %%eax; lidt %0; int3" :: "m" (null_idtr));
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
    else
    {
        kstd_write("Unknown command: ");
        kstd_write(__cmd);
        kstd_write("\n");
    }
}
