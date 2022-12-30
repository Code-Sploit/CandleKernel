bit_PrimaryVolumeDescriptor   resd 1
bit_BootFileLocation          resd 1
bit_BootFileLength            resd 1
bit_Checksum                  resd 1
bit_Reserved                  resb 40

global _start;
[bits 32]

_start:
    [extern kmain]
    call kmain
    jmp $
