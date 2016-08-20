.section multiboot
#define MB_MAGIC 0x1BADB002
#define MB_FLAGS 0x0
#define MB_CHECKSUM -(MB_MAGIC + MB_FLAGS)
.align 4
.int MB_MAGIC
.int MB_FLAGS
.int MB_CHECKSUM
.int 0,0,0,0,0
.int 0
.int 1024, 768, 32
.section .text
.extern init
.global _start
_start:
        finit
        mov $kernel_stack, %esp
        push %ebx
        push %eax
        lgdt gdtr
        mov $0x10, %eax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        ljmp $0x08,$_st
_st:
        call init
_exit:
        cli
        hlt
        jmp _exit
.section .data
gdt:
//NULL-descriptor
.quad 0
//code kernel
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0x98
.byte 0xCF
.byte 0x00
//data kernel
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0x92
.byte 0xCF
.byte 00
//code user
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0xF8
.byte 0xCF
.byte 0x00
//data user
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0xF2
.byte 0xCF
.byte 00
gdtr:
  .word 5 * 8
  .int gdt
.section .bss
.space 8192
kernel_stack: