bits 32
section .text
        ; MULTIBOOT
        
        align 4
        dd 0x1BADB002            ; magic
        dd 0x00                  ; flags
        dd - (0x1BADB002 + 0x00) ; checksum. m + f + c should be zero

global start

extern kmain	        ; EXTERN C MAIN KERNEL

start:
  cli 			; NO INTERRUPTS
  mov esp, stack_space	; STACK POINTER
  
  call kmain
  
  hlt		 	; STOP

section .bss
resb 8192		; ~ 8KB STORAGE FOR STACK
stack_space:
