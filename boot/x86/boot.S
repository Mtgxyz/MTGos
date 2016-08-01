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
        call init
_exit:
        cli
        hlt
        jmp _exit
.section .bss
.space 8192
kernel_stack: