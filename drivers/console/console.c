#include "console.h"

#include "../../lib/memory.h"
#include "../../lib/stdlib.h"
#include "../../lib/stdio.h"
#include "../../lib/kapi.h"

#include "../ext2/ext2.h"

#include "../keyboard/keyboard.h"

void __kstd_execute_read(void)
{
    char *__fdata = kstd_mem_malloc(sizeof(*__fdata));

    __fdata = __kstd_ext2_read_file("hello.txt");

    kstd_write("File: [hello.txt]\n");
    kstd_write(__fdata);
    kstd_write("\n");
}

void __kstd_console_run(char *__cmd)
{
    if (__kstd_strcmp(__cmd, __COMMAND_HALT) == 0)
    {
        __kstd_kapi_t *__obj = kstd_mem_malloc(sizeof(*__obj));

        __obj->__call = KAPI_HALT;
        __obj->__vlid = KAPI_ACTIVE;

        __kstd_kapi_run(__obj);
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_REBOOT) == 0)
    {
        __kstd_kapi_t *__obj = kstd_mem_malloc(sizeof(*__obj));

        __obj->__call = KAPI_REBOOT;
        __obj->__vlid = KAPI_ACTIVE;

        __kstd_kapi_run(__obj);
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_HELP) == 0)
    {
        kstd_write(__KSTD_CONSOLE_HELP_MSG);
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_PRINT_BLOCKS) == 0)
    {
        kstd_mem_print_blocks();
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_CLEAR) == 0)
    {
        __kstd_kapi_t *__obj = kstd_mem_malloc(sizeof(*__obj));

        __obj->__call = KAPI_VGA_CLEAR;
        __obj->__vlid = KAPI_ACTIVE;

        __kstd_kapi_run(__obj);
    }
    else if (__kstd_strcmp(__cmd, __COMMAND_READ) == 0)
    {
        __kstd_execute_read();
    }
    else
    {
        kstd_write("Unknown command: ");
        kstd_write(__cmd);
        kstd_write("\n");
    }
}