#ifndef KAPI_H
#define KAPI_H

#define KAPI_ACTIVE     0x0A
#define KAPI_INACTIVE   0x0B

#define KAPI_HALT       0x01
#define KAPI_SHUTDOWN   0x02
#define KAPI_REBOOT     0x03

#define KAPI_VGA_CLEAR  0x04

typedef struct __kstd_kapi_t
{
    unsigned char __call;
    unsigned char __vlid;
    
    /* Reserved space */
} __kstd_kapi_t;

void __kstd_kapi_run(__kstd_kapi_t *__obj);

#endif