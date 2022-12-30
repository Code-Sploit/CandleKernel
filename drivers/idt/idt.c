#include "idt.h"

#define low_16(address) (uint16)((address) & 0xFFFF)
#define high_16(address) (uint16)(((address) >> 16) & 0xFFFF)

idt_gate_t __kstd_idt[IDT_ENTRIES];
idt_register_t __kstd_idt_reg;

void __kstd_set_idt_gate(int __index, uint32 __handler)
{
    __kstd_idt[__index].low_offset  = low_16(__handler);
    __kstd_idt[__index].sel         = KERNEL_CS;
    __kstd_idt[__index].always0     = 0;
    __kstd_idt[__index].flags       = 0x8E;
    __kstd_idt[__index].high_offset = high_16(__handler);
}

void __kstd_load_idt(void)
{
    __kstd_idt_reg.base  = (uint32) &__kstd_idt;
    __kstd_idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;

    asm volatile("lidt (%0)" : : "r" (&__kstd_idt_reg));
}
