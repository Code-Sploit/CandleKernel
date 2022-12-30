global __kstd_outb

__kstd_outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

global __kstd_inb

__kstd_inb:
    mov dx, [esp + 4]
    in  al, dx
    ret
