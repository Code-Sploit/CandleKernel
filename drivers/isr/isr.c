#include "../../lib/stdio.h"
#include "../pio/pio.h"
#include "../idt/idt.h"

#include "isr.h"

isr_t __kstd_interrupt_handlers[256];

char *__kstd_isr_exceptions[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",

        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",

        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};

void __kstd_isr_enable() {
    __kstd_set_idt_gate(0, (uint32) isr0);
    __kstd_set_idt_gate(1, (uint32) isr1);
    __kstd_set_idt_gate(2, (uint32) isr2);
    __kstd_set_idt_gate(3, (uint32) isr3);
    __kstd_set_idt_gate(4, (uint32) isr4);
    __kstd_set_idt_gate(5, (uint32) isr5);
    __kstd_set_idt_gate(6, (uint32) isr6);
    __kstd_set_idt_gate(7, (uint32) isr7);
    __kstd_set_idt_gate(8, (uint32) isr8);
    __kstd_set_idt_gate(9, (uint32) isr9);
    __kstd_set_idt_gate(10, (uint32) isr10);
    __kstd_set_idt_gate(11, (uint32) isr11);
    __kstd_set_idt_gate(12, (uint32) isr12);
    __kstd_set_idt_gate(13, (uint32) isr13);
    __kstd_set_idt_gate(14, (uint32) isr14);
    __kstd_set_idt_gate(15, (uint32) isr15);
    __kstd_set_idt_gate(16, (uint32) isr16);
    __kstd_set_idt_gate(17, (uint32) isr17);
    __kstd_set_idt_gate(18, (uint32) isr18);
    __kstd_set_idt_gate(19, (uint32) isr19);
    __kstd_set_idt_gate(20, (uint32) isr20);
    __kstd_set_idt_gate(21, (uint32) isr21);
    __kstd_set_idt_gate(22, (uint32) isr22);
    __kstd_set_idt_gate(23, (uint32) isr23);
    __kstd_set_idt_gate(24, (uint32) isr24);
    __kstd_set_idt_gate(25, (uint32) isr25);
    __kstd_set_idt_gate(26, (uint32) isr26);
    __kstd_set_idt_gate(27, (uint32) isr27);
    __kstd_set_idt_gate(28, (uint32) isr28);
    __kstd_set_idt_gate(29, (uint32) isr29);
    __kstd_set_idt_gate(30, (uint32) isr30);
    __kstd_set_idt_gate(31, (uint32) isr31);

    // Remap the PIC
    __kstd_outb(0x20, 0x11);
    __kstd_outb(0xA0, 0x11);
    __kstd_outb(0x21, 0x20);
    __kstd_outb(0xA1, 0x28);
    __kstd_outb(0x21, 0x04);
    __kstd_outb(0xA1, 0x02);
    __kstd_outb(0x21, 0x01);
    __kstd_outb(0xA1, 0x01);
    __kstd_outb(0x21, 0x0);
    __kstd_outb(0xA1, 0x0);

    // Install the IRQs
    __kstd_set_idt_gate(32, (uint32)irq0);
    __kstd_set_idt_gate(33, (uint32)irq1);
    __kstd_set_idt_gate(34, (uint32)irq2);
    __kstd_set_idt_gate(35, (uint32)irq3);
    __kstd_set_idt_gate(36, (uint32)irq4);
    __kstd_set_idt_gate(37, (uint32)irq5);
    __kstd_set_idt_gate(38, (uint32)irq6);
    __kstd_set_idt_gate(39, (uint32)irq7);
    __kstd_set_idt_gate(40, (uint32)irq8);
    __kstd_set_idt_gate(41, (uint32)irq9);
    __kstd_set_idt_gate(42, (uint32)irq10);
    __kstd_set_idt_gate(43, (uint32)irq11);
    __kstd_set_idt_gate(44, (uint32)irq12);
    __kstd_set_idt_gate(45, (uint32)irq13);
    __kstd_set_idt_gate(46, (uint32)irq14);
    __kstd_set_idt_gate(47, (uint32)irq15);

    __kstd_load_idt(); // Load with ASM

    //asm volatile("sti"); // Enable interrupts
}

void __kstd_register_interrupt_handler(uint8 __index, isr_t __handler) {
    __kstd_interrupt_handlers[__index] = __handler;
}

void __kstd_isr_handler(registers_t *__regs)
{
    return;
}

void __kstd_irq_handler(registers_t *__regs) {
    if (__kstd_interrupt_handlers[__regs->int_no] != 0) {
        isr_t __handler = __kstd_interrupt_handlers[__regs->int_no];

        __handler(__regs);
    }

    if (__regs->int_no >= 40) {
        __kstd_outb(0xA0, 0x20);
    }
    
    __kstd_outb(0x20, 0x20);
}
