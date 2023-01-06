#ifndef PIO_H
#define PIO_H

void __kstd_outb(unsigned short __port, unsigned char __data);
void __kstd_outs(unsigned short __port, unsigned short __data);
void __kstd_outl(unsigned short __port, unsigned short __data);

unsigned char __kstd_inb(unsigned short __port);
unsigned short __kstd_ins(unsigned short __port);
unsigned int __kstd_inl(unsigned short __port);

void __kstd_io_sleep(void);

#endif
