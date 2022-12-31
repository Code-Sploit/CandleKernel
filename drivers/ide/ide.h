#ifndef IDE_H
#define IDE_H

/* ATA Status */

#define ATA_SR_BSY   0x80        /* Drive is busy */
#define ATA_SR_DRDY  0x40        /* Drive is ready */
#define ATA_SR_DF    0x20        /* Drive experienced a write fault */
#define ATA_SR_DSC   0x10        /* Drive completed seek */
#define ATA_SR_DRQ   0x08        /* Data request is read */
#define ATA_SR_CORR  0x04        /* Corrected data */
#define ATA_SR_IDX   0x02        /* Index */
#define ATA_SR_ERR   0x01        /* General Error */

/* ATA Errors */

#define ATA_ER_BBK   0x80        /* Bad block */
#define ATA_ER_UNC   0x40        /* Uncorrectable data */
#define ATA_ER_MC    0x20        /* Media device changed */
#define ATA_ER_IDNF  0x10        /* ID mark of device data not found */
#define ATA_ER_MCR   0x08        /* Media change requested */
#define ATA_ER_ABRT  0x04        /* Command aborted */
#define ATA_ER_TK0NF 0x02        /* Track 0 not found */
#define ATA_ER_AMNF  0x01        /* No address mark */

/* ATA Commands */

#define ATA_CMD_READ_PIO        0x20
#define ATA_CMD_READ_PIO_EXT    0x24
#define ATA_CMD_READ_DMA        0xC8
#define ATA_CMD_READ_DMA_EXT    0x25

#define ATA_CMD_WRITE_PIO       0x30
#define ATA_CMD_WRITE_PIO_EXT   0x34
#define ATA_CMD_WRITE_DMA       0xCA
#define ATA_CMD_WRITE_DMA_EXT   0x35

#define ATA_CMD_CACHE_FLUSH     0xE7
#define ATA_CMD_CACHE_FLUSH_EXT 0xEA

#define ATA_CMD_PACKET          0xA0

#define ATA_CMD_IDENTIFY_PACKET 0xA1
#define ATA_CMD_IDENTIFY        0xEC

/* ATAPI Commands */

#define ATAPI_CMD_READ          0xA8
#define ATAPI_CMD_EJECT         0x1B

/* ATA Identifiers */

#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200

/* ATA Interface type */

#define IDE_ATA            0x00
#define IDE_ATAPI          0x01
 
#define ATA_MAIN           0x00
#define ATA_CHILD          0x01

/* ATA Registers */

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

/* ATA Channels */

#define ATA_PRIMARY        0x00
#define ATA_SECONDARY      0x01

/* ATA Directions */

#define ATA_READ           0x00
#define ATA_WRITE          0x01

/* IDE Buffer */

unsigned static char __kstd_ide_buf[2048]    = {0};
unsigned static char __kstd_atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

volatile unsigned static char __kstd_ide_irq_invoked = 0;

/* Methods */

void __kstd_ide_insl(unsigned short __register, unsigned int *__buffer_ptr, int __quads);
void __kstd_ide_outsl(unsigned short __register, unsigned int *__buffer_ptr, int __quads);

int __kstd_ide_initialize(unsigned int __BAR0, unsigned int __BAR1, unsigned int __BAR2, unsigned int __BAR3, unsigned int __BAR4);

void __kstd_ide_write_channel(unsigned char __channel, unsigned char __register, unsigned char __data);
void __kstd_ide_read_buffer(unsigned char __channel, unsigned char __register, unsigned int *__buffer, unsigned int __quads);

unsigned char __kstd_ide_read_channel(unsigned char __channel, unsigned char __register);
unsigned char __kstd_ide_polling_channel(unsigned char __channel, unsigned int __advanced_check);
unsigned char __kstd_ide_print_error(unsigned int __drive, unsigned char __err);

#endif