[org 0x7c00]
KERNEL_OFFSET equ 0x1000

mov [BOOT_DRIVE], dl
mov bp, 0x9000
mov sp, bp

call __load_kernel
call __switch_to_32bit
jmp $

%include "asm/print-16bit.asm"
%include "asm/print-32bit.asm"
%include "asm/disk.asm"
%include "asm/gdt.asm"
%include "asm/switch-to-32bit.asm"

[bits 16]
__load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, 31
    mov dl, [BOOT_DRIVE]
    call __disk_load
    ret

[bits 32]
BEGIN_32BIT:
    call KERNEL_OFFSET
    jmp $


BOOT_DRIVE db 0

times 510 - ($-$$) db 0
dw 0xaa55
