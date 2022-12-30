#ifndef IDT_H
#define IDT_H

#include "../../lib/stdlib.h"

#define KERNEL_CS 0x08
#define IDT_ENTRIES 256

typedef struct {
    uint16 low_offset;
    uint16 sel;
    uint8 always0;
    uint8 flags;
    uint16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct {
    uint16 limit;
    uint32 base;
} __attribute__((packed)) idt_register_t;

void __kstd_set_idt_gate(int __index, uint32 __handler);

void __kstd_load_idt(void);

#endif
