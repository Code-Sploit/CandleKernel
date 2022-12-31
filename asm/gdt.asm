;Load the Global Descriptor Table

global __kstd_mgdt_load_gdt
global __kstd_mgdt_load_registers

__kstd_mgdt_load_gdt:
	lgdt [esp + 4]
	ret

__kstd_mgdt_load_registers:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	jmp 0x08:flush_cs

flush_cs:
	ret
