bits 32
section .text
        ; MULTIBOOT

        align 4
        dd 0x1BADB002            ; magic
        dd 0x00                  ; flags
        dd - (0x1BADB002 + 0x00) ; checksum. m + f + c should be zero

global _start

extern kmain	        ; EXTERN C MAIN KERNEL

_start:
  cli 			; NO INTERRUPTS
  mov esp, stack_space	; STACK POINTER

  call kmain
  jmp $

  hlt		 	; STOP

section .bss
resb 8192		; ~ 8KB STORAGE FOR STACK
stack_space:
