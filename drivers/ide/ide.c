#include "ide.h"

#include "../pio/pio.h"

#include "../../lib/memory.h"
#include "../../lib/stdio.h"

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

static unsigned char __kstd_ide_read_channel(unsigned char __channel, unsigned char __reg)
{
    unsigned char __data;

    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }
    
    if (__reg < 0x08)
    {
        __data = __kstd_inb(__kstd_ide_channels[__channel].__base + __reg - 0x00);
    }
    else if (__reg < 0x0C)
    {
        __data = __kstd_inb(__kstd_ide_channels[__channel].__base + __reg - 0x06);
    }
    else if (__reg < 0x0E)
    {
        __data = __kstd_inb(__kstd_ide_channels[__channel].__cbase + __reg - 0x0A);
    }
    else if (__reg < 0x16)
    {
        __data = __kstd_inb(__kstd_ide_channels[__channel].__bmide + __reg - 0x0E);
    }

    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }

    return __data;
}

static void __kstd_ide_write_channel(unsigned char __channel, unsigned char __reg, unsigned char __data)
{
    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    if (__reg < 0x08)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__base + __reg - 0x00, __data);
    }
    else if (__reg < 0x0C)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__base + __reg - 0x06, __data);
    }
    else if (__reg < 0x0E)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__cbase + __reg - 0x0A, __data);
    }
    else if (__reg < 0x16)
    {
        __kstd_outb(__kstd_ide_channels[__channel].__bmide + __reg - 0x0E, __data);
    }

    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }
}

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
    int __index;

    for (__index = 0; __index < __quads; __index++)
    {
        __kstd_outb(__register, __buffer_ptr[__index]);
    }
}

void __kstd_ide_read_buffer(unsigned char __channel, unsigned char __reg, unsigned int *__buffer, unsigned int __quads)
{
    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    /* Save ES register */

    asm volatile ("pushw %es");
    asm volatile ("movw %ds, %ax");
    asm volatile ("movw %ax, %es");

    if (__reg < 0x08)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__base + __reg - 0x00, __buffer, __quads);
    }
    else if (__reg < 0x0C)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__base + __reg - 0x06, __buffer, __quads);
    }
    else if (__reg < 0x0E)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__cbase + __reg - 0x0A, __buffer, __quads);
    }
    else if (__reg < 0x16)
    {
        __kstd_ide_insl(__kstd_ide_channels[__channel].__bmide + __reg - 0x0E, __buffer, __quads);
    }

    /* Restore ES register */

    asm volatile ("popw %es");

    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }
}

void __kstd_ide_write_buffer(unsigned char __channel, unsigned char __reg, unsigned int *__buffer, unsigned int __quads)
{
    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, 0x80 | __kstd_ide_channels[__channel].__nint);
    }

    /* Save ES register */

    asm volatile ("pushw %es");
    asm volatile ("movw %ds, %ax");
    asm volatile ("movw %ax, %es");

    if (__reg < 0x08)
    {
        __kstd_ide_outsl(__kstd_ide_channels[__channel].__base + __reg - 0x00, __buffer, __quads);
    }
    else if (__reg < 0x0C)
    {
        __kstd_ide_outsl(__kstd_ide_channels[__channel].__base + __reg - 0x06, __buffer, __quads);
    }
    else if (__reg < 0x0E)
    {
        __kstd_ide_outsl(__kstd_ide_channels[__channel].__cbase + __reg - 0x0A, __buffer, __quads);
    }
    else if (__reg < 0x16)
    {
        __kstd_ide_outsl(__kstd_ide_channels[__channel].__bmide + __reg - 0x0E, __buffer, __quads);
    }

    /* Restore ES register */

    asm volatile ("popw %es");

    if (__reg > 0x07 && __reg < 0x0C)
    {
        __kstd_ide_write_channel(__channel, ATA_REG_CONTROL, __kstd_ide_channels[__channel].__nint);
    }
}

unsigned char __kstd_ide_polling_channel(unsigned char __channel, unsigned char __deepcheck)
{
    for (int i = 0; i < 4; i++)
    {
        __kstd_ide_read_channel(__channel, ATA_REG_ALTSTATUS);
    }

    while (__kstd_ide_read_channel(__channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Wait */
    }

    if (__deepcheck)
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
    if (__err == 0)
        return __err;

    kstd_write("IDE:");
    if (__err == 1) {
        kstd_write("- Device Fault\n");
        __err = 19;
    } else if (__err == 2) {
        uint8 st = __kstd_ide_read_channel(__kstd_ide_devices[__drive].__channel, ATA_REG_ERROR);
        if (st & ATA_ER_AMNF) {
            kstd_write("- No Address Mark Found\n");
            __err = 7;
        }
        if (st & ATA_ER_TK0NF) {
            kstd_write("- No Media or Media Error\n");
            __err = 3;
        }
        if (st & ATA_ER_ABRT) {
            kstd_write("- Command Aborted\n");
            __err = 20;
        }
        if (st & ATA_ER_MCR) {
            kstd_write("- No Media or Media Error\n");
            __err = 3;
        }
        if (st & ATA_ER_IDNF) {
            kstd_write("- ID mark not Found\n");
            __err = 21;
        }
        if (st & ATA_ER_MC) {
            kstd_write("- No Media or Media Error\n");
            __err = 3;
        }
        if (st & ATA_ER_UNC) {
            kstd_write("- Uncorrectable Data Error\n");
            __err = 22;
        }
        if (st & ATA_ER_BBK) {
            kstd_write("- Bad Sectors\n");
            __err = 13;
        }
    } else if (__err == 3) {
        kstd_write("- Reads Nothing\n");
        __err = 23;
    } else if (__err == 4) {
        kstd_write("- Write Protected\n");
        __err = 8;
    }
    kstd_write(
           (const char *[]){"Primary", "Secondary"}[__kstd_ide_devices[__drive].__channel]);
    kstd_write(" ");
    kstd_write((const char *[]){"Master", "Slave"}[__kstd_ide_devices[__drive].__drive]);
    kstd_write(" ");
    kstd_write(__kstd_ide_devices[__drive].__model);

    return __err;
}

void __kstd_ide_initialize(unsigned int __PC1, unsigned int __PCC1, unsigned int __PC2, unsigned int __PCC2, unsigned int __BUS)
{
    int i, j, k, count = 0;

    unsigned char __kstd_ide_buf[2048] = {0};

    __kstd_ide_channels[ATA_PRIMARY].__base    = __PC1;
    __kstd_ide_channels[ATA_PRIMARY].__cbase   = __PCC1;
    __kstd_ide_channels[ATA_PRIMARY].__bmide   = __BUS;

    __kstd_ide_channels[ATA_SECONDARY].__base  = __PC2;
    __kstd_ide_channels[ATA_SECONDARY].__cbase = __PCC2;
    __kstd_ide_channels[ATA_SECONDARY].__bmide = __BUS;

    __kstd_ide_write_channel(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    __kstd_ide_write_channel(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2; j++)
        {
            unsigned char __err = 0;
            unsigned char __type = IDE_ATA;
            unsigned char __status;

            __kstd_ide_devices[count].__reserved = 0;

            __kstd_ide_write_channel(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));
            __kstd_ide_write_channel(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

            if (__kstd_ide_read_channel(i, ATA_REG_STATUS) == 0)
            {
                continue;
            }

            while (1)
            {
                __status = __kstd_ide_read_channel(i, ATA_REG_STATUS);

                if ((__status & ATA_SR_ERR))
                {
                    __err = 1;

                    break;
                }

                if (!(__status & ATA_SR_BSY) && (__status & ATA_SR_DRQ))
                {
                    break;
                }
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

            __kstd_ide_read_buffer(i, ATA_REG_DATA, (unsigned int *) __kstd_ide_buf, 128);

            __kstd_ide_devices[count].__reserved     = 1;
            __kstd_ide_devices[count].__type         = __type;
            __kstd_ide_devices[count].__channel      = i;
            __kstd_ide_devices[count].__drive        = j;
            __kstd_ide_devices[count].__signature    = *((unsigned short *) (__kstd_ide_buf + ATA_IDENT_DEVICETYPE));
            __kstd_ide_devices[count].__capabilities = *((unsigned short *) (__kstd_ide_buf + ATA_IDENT_CAPABILITIES));
            __kstd_ide_devices[count].__commandsets  = *((unsigned int *)   (__kstd_ide_buf + ATA_IDENT_COMMANDSETS));

            if (__kstd_ide_devices[count].__commandsets & (1 << 26))
            {
                __kstd_ide_devices[count].__size = *((unsigned int *) (__kstd_ide_buf + ATA_IDENT_MAX_LBA_EXT));
            }
            else
            {
                __kstd_ide_devices[count].__size = *((unsigned int *) (__kstd_ide_buf + ATA_IDENT_MAX_LBA));
            }

            for (k = 0; k < 40; k += 2)
            {
                __kstd_ide_devices[count].__model[k]     = __kstd_ide_buf[ATA_IDENT_MODEL + k + 1];
                __kstd_ide_devices[count].__model[k + 1] = __kstd_ide_buf[ATA_IDENT_MODEL + k];
            }

            __kstd_ide_devices[count].__model[40] = '\0';

            for (k = 39; i >= 0; k--)
            {
                char __ch = __kstd_ide_devices[count].__model[k];

                if (__ch == ' ')
                {
                    __kstd_ide_devices[count].__model[k] = '\0';
                }
                else
                {
                    break;
                }
            }

            count++;
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
    }
}

unsigned char __kstd_ide_ata_access(unsigned char __direction, unsigned char __drive, unsigned int __lba, unsigned char __nsectors, unsigned int __buffer)
{
    unsigned char lba_mode, dma, cmd;
    unsigned char lba_io[6];
    
    unsigned int channel    = __kstd_ide_devices[__drive].__channel;
    unsigned int childbit   = __kstd_ide_devices[__drive].__drive;
    unsigned int bus        = __kstd_ide_channels[channel].__base;
    unsigned int words      = 256;
    
    unsigned short cyl, i;

    unsigned char head, sect, err;

    __kstd_ide_write_channel(channel, ATA_REG_CONTROL, __kstd_ide_channels[channel].__nint = (__kstd_ide_irq_invoked = 0x0) + 0x02);

    // (I) Select one from LBA28, LBA48 or CHS;
    if (__lba >= 0x10000000)
    {
        lba_mode = LBA_MODE_48;
        lba_io[0] = (__lba & 0x000000FF) >> 0;
        lba_io[1] = (__lba & 0x0000FF00) >> 8;
        lba_io[2] = (__lba & 0x00FF0000) >> 16;
        lba_io[3] = (__lba & 0xFF000000) >> 24;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = 0;
    }
    else if (__kstd_ide_devices[__drive].__capabilities & 0x200)
    {
        lba_mode = LBA_MODE_28;
        lba_io[0] = (__lba & 0x00000FF) >> 0;
        lba_io[1] = (__lba & 0x000FF00) >> 8;
        lba_io[2] = (__lba & 0x0FF0000) >> 16;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (__lba & 0xF000000) >> 24;
    }
    else
    {
        lba_mode = LBA_MODE_CHS;
        sect = (__lba % 63) + 1;
        cyl = (__lba + 1 - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (__lba + 1 - sect) % (16 * 63) / (63);
    }

    dma = 0;

    while (__kstd_ide_read_channel(channel, ATA_REG_STATUS) & ATA_SR_BSY)
    {
        /* Wait */
    }

    if (lba_mode == LBA_MODE_CHS)
        __kstd_ide_write_channel(channel, ATA_REG_HDDEVSEL, 0xA0 | (childbit << 4) | head);
    else
        __kstd_ide_write_channel(channel, ATA_REG_HDDEVSEL, 0xE0 | (childbit << 4) | head);

    // (V) Write Parameters;
    if (lba_mode == LBA_MODE_48)
    {
        __kstd_ide_write_channel(channel, ATA_REG_SECCOUNT1, 0);
        __kstd_ide_write_channel(channel, ATA_REG_LBA3, lba_io[3]);
        __kstd_ide_write_channel(channel, ATA_REG_LBA4, lba_io[4]);
        __kstd_ide_write_channel(channel, ATA_REG_LBA5, lba_io[5]);
    }

    __kstd_ide_write_channel(channel, ATA_REG_SECCOUNT0, __nsectors);
    __kstd_ide_write_channel(channel, ATA_REG_LBA0, lba_io[0]);
    __kstd_ide_write_channel(channel, ATA_REG_LBA1, lba_io[1]);
    __kstd_ide_write_channel(channel, ATA_REG_LBA2, lba_io[2]);

    // (VI) Select the command and send it;
    if (lba_mode == LBA_MODE_CHS && dma == 0 && __direction == ATA_READ)  cmd = ATA_CMD_READ_PIO;
    if (lba_mode == LBA_MODE_28  && dma == 0 && __direction == ATA_READ)  cmd = ATA_CMD_READ_PIO;
    if (lba_mode == LBA_MODE_48  && dma == 0 && __direction == ATA_READ)  cmd = ATA_CMD_READ_PIO_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 1 && __direction == ATA_READ)  cmd = ATA_CMD_READ_DMA;
    if (lba_mode == LBA_MODE_28  && dma == 1 && __direction == ATA_READ)  cmd = ATA_CMD_READ_DMA;
    if (lba_mode == LBA_MODE_48  && dma == 1 && __direction == ATA_READ)  cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 0 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == LBA_MODE_28  && dma == 0 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == LBA_MODE_48  && dma == 0 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 1 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == LBA_MODE_28  && dma == 1 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == LBA_MODE_48  && dma == 1 && __direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA_EXT;

    __kstd_ide_write_channel(channel, ATA_REG_COMMAND, cmd);

    if (dma)
    {
        if (__direction == ATA_READ)
        {
            // DMA Read
        }
        else
        {
            // DMA write
        }
    }
    else if (__direction == ATA_READ)
    {
        // PIO Read.
        for (i = 0; i < __nsectors; i++)
        {
            if ((err = __kstd_ide_polling_channel(channel, 1)))
                return err;

            asm("pushw %es");
            asm("rep insw"
                :
                : "c"(words), "d"(bus), "D"(__buffer));
            asm("popw %es");
            
            __buffer += (words * 2);
        }
    }
    else
    {
        // PIO Write.
        for (i = 0; i < __nsectors; i++)
        {
            __kstd_ide_polling_channel(channel, 0);

            asm("pushw %ds");
            asm("rep outsw" ::"c"(words), "d"(bus), "S"(__buffer));
            asm("popw %ds");

            __buffer += (words * 2);
        }

        __kstd_ide_write_channel(channel, ATA_REG_COMMAND, (char[]) {ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
        __kstd_ide_polling_channel(channel, 0);
    }

    return 0;
}

void __kstd_ide_waitfor_irq(void)
{
    while (!__kstd_ide_irq_invoked)
    {
        /* Wait */
    }

    __kstd_ide_irq_invoked = 0;
}

void __kstd_ide_irq(void)
{
    __kstd_ide_irq_invoked = 1;
}

int __kstd_ide_read_sectors(unsigned char __drive, unsigned char __nsectors, unsigned int __lba, unsigned int __buffer)
{
    if (__drive > MAXIMUM_IDE_DEVICES || __kstd_ide_devices[__drive].__reserved == 0)
    {
        kstd_write("IDE: - Drive not found!\n");

        return -1;
    }
    else if (((__lba + __nsectors) > __kstd_ide_devices[__drive].__size) && (__kstd_ide_devices[__drive].__type == IDE_ATA))
    {
        kstd_write("IDE: - LBA Address exceeded disk size!\n");

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__kstd_ide_devices[__drive].__type == IDE_ATA)
        {
            __err = __kstd_ide_ata_access(ATA_READ, __drive, __lba, __nsectors, __buffer);
        }

        return __kstd_ide_print_error(__drive, __err);
    }

    return 0;
}

int __kstd_ide_write_sectors(unsigned char __drive, unsigned char __nsectors, unsigned int __lba, unsigned int __buffer)
{
    if (__drive > MAXIMUM_IDE_DEVICES || __kstd_ide_devices[__drive].__reserved == 0)
    {
        kstd_write("IDE: - Drive not found!\n");

        return -1;
    }
    else if (((__lba + __nsectors) > __kstd_ide_devices[__drive].__size) && (__kstd_ide_devices[__drive].__type == IDE_ATA))
    {
        kstd_write("IDE: - LBA Address exceeded disk size!\n");

        return -2;
    }
    else
    {
        unsigned char __err;

        if (__kstd_ide_devices[__drive].__type == IDE_ATA)
        {
            __err = __kstd_ide_ata_access(ATA_WRITE, __drive, __lba, __nsectors, __buffer);
        }

        return __kstd_ide_print_error(__drive, __err);
    }

    return 0;
}

int __kstd_ide_ata_initialize(void)
{
    __kstd_ide_initialize(0x1F0, 0x3F6, 0x170, 0x376, 0x000);

    return 0;
}