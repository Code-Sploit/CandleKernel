#include "ide.h"

#include "../pio/pio.h"

#include "../../lib/stdlib.h"
#include "../../lib/stdio.h"

/* IDE Channel Container Struct */

struct IDEChannelRegisters
{
    unsigned short __base;
    unsigned short __cbase;
    unsigned short __bmide;

    unsigned char __nint;
} __kstd_ide_channels[2];

/* IDE Device Container Struct */
struct IDEDevice
{
    unsigned char __model[41];
    unsigned char __reserved;
    unsigned char __channel;
    unsigned char __drive;

    unsigned short __capabilities;
    unsigned short __signature;
    unsigned short __type;

    unsigned int __commandsets;
    unsigned int __size;
} __kstd_ide_devices[4];

void __kstd_ide_insl(unsigned short __register, unsigned int *__buffer_ptr, int __quads)
{
    int __index;

    for (__index = 0; __index < __quads; __index++)
    {
        __buffer_ptr[__index] = __kstd_inb(__register);
    }
}

void __kstd_ide_outsl(unsigned short __register, unsigned int *__buffer_ptr, int __quads)
{
    int __index = 0;

    for (__index = 0; __index < __quads; __index++)
    {
        __kstd_outb(__register, __buffer_ptr[__index]);
    }
}

void __kstd_ide_write_channel(unsigned char __channel, unsigned char __register, unsigned char __data)
{
    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    if (__register < 0x08)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__base + __register - 0x00, __data);
    }
    else if (__register < 0x0C)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__base + __register - 0x06, __data);
    }
    else if (__register < 0x0E)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__cbase + __register - 0x0A, __data);
    }
    else if (__register < 0x16)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__bmide + __register - 0x0E, __data);
    }

    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }
}

void __kstd_ide_read_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads)
{
    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    /* Save ES Register */

    asm ("pushw %es");
    asm ("movw %ds, %ax");
    asm ("movw %ax, %es");

    if (__register < 0x08)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__base + __register - 0x00, __buffer, __quads);
    }
    else if (__register < 0x0C)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__base + __register - 0x06, __buffer, __quads);
    }
    else if (__register < 0x0E)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__cbase + __register - 0x0A, __buffer, __quads);
    }
    else if (__register < 0x16)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__bmide + __register - 0x0E, __buffer, __quads);
    }

    /* Restore ES Register */

    asm ("popw %es");

    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }
}

unsigned char __kstd_ide_read_channel(unsigned char __channel, unsigned char __register)
{
    unsigned char __result;

    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    if (__register < 0x08)
    {
        __result = __kstd_inb(__kstd_ide_channels[__channel].__base + __register - 0x00);
    }
    else if (__register < 0x0C)
    {
        __result = __kstd_inb(__kstd_ide_channels[__channel].__base + __register - 0x06);
    }
    else if (__register < 0x0E)
    {
        __result = __kstd_inb(__kstd_ide_channels[__channel].__cbase + __register - 0x0A);
    }
    else if (__register < 0x16)
    {
        __result = __kstd_inb(__kstd_ide_channels[__channel].__bmide + __register - 0x0E);
    }

    if (__register > 0x07 && __register < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }

    return __result;
}

unsigned char __kstd_ide_polling_channel(unsigned char __channel, unsigned int __advanced_check)
{
    /* Delay for 400 ms */
    for (int i = 0; i < 4; i++)
    {
        __kstd_ide_read_channel(__channel, ATA_REG_ALTSTATUS);
    }

    while (__kstd_ide_read_channel(__channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Wait until register status is NOT BUSY */
    }

    if (__advanced_check)
    {
        unsigned char __state = __kstd_ide_read_channel(__channel, ATA_REG_STATUS);

        if (__state & ATA_SR_ERR)
        {
            return 2;
        }

        if (__state & ATA_SR_DF)
        {
            return 1;
        }

        if ((__state & ATA_SR_DRQ) == 0)
        {
            return 3;
        }
    }

    return 0;
}

unsigned char __kstd_ide_print_error(unsigned int __drive, unsigned char __err)
{
    if (__err = 0)
    {
        return __err;
    }

    kstd_write("IDE: ");

    if (__err == 1)
    {
        kstd_write("- Device Fault\n ");
        
        __err = 19;
    }
    else if (__err == 2)
    {
        unsigned char __st = __kstd_ide_read_channel(__kstd_ide_devices[__drive].__channel, ATA_REG_ERROR);

        if (__st & ATA_ER_AMNF)     {kstd_write("- No Address Mark Found\n"); __err = 7;}
        if (__st & ATA_ER_TK0NF)    {kstd_write("- No Media or Media Error\n"); __err = 3;}
        if (__st & ATA_ER_ABRT)     {kstd_write("- Command Aborted\n"); __err = 20;}
        if (__st & ATA_ER_MCR)      {kstd_write("- No Media or Media Error\n"); __err = 3;}
        if (__st & ATA_ER_IDNF)     {kstd_write("- ID Mark Not Found\n"); __err = 21;}
        if (__st & ATA_ER_MC)       {kstd_write("- No Media or Media Error\n"); __err = 3;}
        if (__st & ATA_ER_UNC)      {kstd_write("- Uncorrectable Data Error\n"); __err = 22;}
        if (__st & ATA_ER_BBK)      {kstd_write("- Bad Sectors\n"); __err = 13;}
    }
    else if (__err == 3)
    {
        kstd_write("- Nil read\n");
        
        __err = 23;
    }
    else if (__err = 4)
    {
        kstd_write("- Write Protected\n");

        __err = 8;
    }

    kstd_write(" - [");

    kstd_write((const char *[]) {"Primary ", "Secondary "}[__kstd_ide_devices[__drive].__channel]);
    kstd_write((const char *[]) {"Main", "Child"}[__kstd_ide_devices[__drive].__drive]);

    kstd_write("] ");

    kstd_write(__kstd_ide_devices[__drive].__model);

    kstd_write("\n");

    return __err;
}

int __kstd_ide_initialize(unsigned int __BAR0, unsigned int __BAR1, unsigned int __BAR2, unsigned int __BAR3, unsigned int __BAR4)
{
    int i, j, k, __count = 0;

    __kstd_ide_channels[ATA_PRIMARY].__base        = (__BAR0 & 0xFFFFFFFC) + 0x1F0 * (!__BAR0);
    __kstd_ide_channels[ATA_PRIMARY].__cbase       = (__BAR1 & 0xFFFFFFFC) + 0x3F6 * (!__BAR1);
    __kstd_ide_channels[ATA_PRIMARY].__bmide       = (__BAR4 & 0xFFFFFFFC) + 0;

    __kstd_ide_channels[ATA_SECONDARY].__base      = (__BAR2 & 0xFFFFFFFC) + 0x170 * (!__BAR2);
    __kstd_ide_channels[ATA_SECONDARY].__cbase     = (__BAR3 & 0xFFFFFFFC) + 0x376 * (!__BAR3);
    __kstd_ide_channels[ATA_SECONDARY].__bmide     = (__BAR4 & 0xFFFFFFFC) + 8;

    __kstd_ide_write_channel(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    __kstd_ide_write_channel(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    /* Detect devices */

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            unsigned char __err  = 0;
            unsigned char __type = IDE_ATA;
            unsigned char __status;

            __kstd_ide_devices[__count].__reserved = 0;

            __kstd_ide_write_channel(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            __kstd_ide_write_channel(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            if (__kstd_ide_read_channel(i, ATA_REG_STATUS) == 0) continue;

            while (1)
            {
                __status = __kstd_ide_read_channel(i, ATA_REG_STATUS);

                if ((__status & ATA_SR_ERR))
                {
                    __err = 1;

                    break;
                }

                if (!(__status & ATA_SR_BSY) && (__status & ATA_SR_DRQ)) break;
            }

            if (__err != 0)
            {
                unsigned char __cl = __kstd_ide_read_channel(i, ATA_REG_LBA1);
                unsigned char __ch = __kstd_ide_read_channel(i, ATA_REG_LBA2);

                if (__cl == 0x14 && __ch == 0xEB)
                {
                    __type = IDE_ATAPI;
                }
                else if (__cl == 0x69 && __ch == 0x96)
                {
                    __type = IDE_ATAPI;
                }
                else
                {
                    continue;
                }

                __kstd_ide_write_channel(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            }

            __kstd_ide_write_channel(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);

            __kstd_ide_devices[__count].__reserved      = 1;
            __kstd_ide_devices[__count].__type          = __type;
            
            __kstd_ide_devices[__count].__channel       = i;
            __kstd_ide_devices[__count].__drive         = j;

            __kstd_ide_devices[__count].__signature     = *((unsigned short *) (__kstd_ide_buf + ATA_IDENT_DEVICETYPE));
            __kstd_ide_devices[__count].__capabilities  = *((unsigned short *) (__kstd_ide_buf + ATA_IDENT_CAPABILITIES));
            __kstd_ide_devices[__count].__commandsets   = *((unsigned int *)   (__kstd_ide_buf + ATA_IDENT_COMMANDSETS));

            if (__kstd_ide_devices[__count].__commandsets & (1 << 26))
            {
                __kstd_ide_devices[__count].__size = *((unsigned int *) (__kstd_ide_buf + ATA_IDENT_MAX_LBA_EXT));
            }
            else
            {
                __kstd_ide_devices[__count].__size = *((unsigned int *) (__kstd_ide_buf + ATA_IDENT_MAX_LBA));
            }

            for (k = 0; k < 40; k = (k + 2))
            {
                __kstd_ide_devices[__count].__model[k]     = __kstd_ide_buf[ATA_IDENT_MODEL + k + 1];
                __kstd_ide_devices[__count].__model[k + 1] = __kstd_ide_buf[ATA_IDENT_MODEL + k];
            }

            __kstd_ide_devices[__count].__model[40] = 0;

            __count++;
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (__kstd_ide_devices[i].__reserved == 1)
        {
            kstd_write("Found ");
            kstd_write((const char *[]) {"ATA", "ATAPI"}[__kstd_ide_devices[i].__type]);
            kstd_write(" Drive ");
            
            char *__ssize;

            kstd_itoa(__ssize, 10, __kstd_ide_devices[i].__size / 1024 / 1024 / 2);

            kstd_write(__ssize);
            kstd_write("GB - ");
            kstd_write(__kstd_ide_devices[i].__model);
        }
    }

    return 0;
}