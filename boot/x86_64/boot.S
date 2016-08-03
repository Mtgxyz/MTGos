.code32
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
  cli
  finit
  mov $mb_ptr, %edi
  stosl
  mov %ebx, %eax
  stosl //Temponarily store both multiboot header pointer and multiboot
  mov $kernel_stack, %esp
  mov $0x80000001, %eax //Check if Long Mode is supported
  cpuid
  and $0x20000000, %edx
  jz x86_64_err

  xor %eax, %eax //Check if PAE is supported
  inc %eax
  cpuid
  and $0x40, %dl
  jz x86_64_err
  jmp x86_64_OK
x86_64_err:
  cli
  hlt
  jmp x86_64_err
x86_64_OK:
  //Fill one entry of both the pagemap L4 and the Pagedir PT
  mov $pmfill, %esi
  mov $pagemapL4, %edi
  lodsl
  stosl
  lodsl
  stosl
  mov $pagedirPT, %edi
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  lodsl
  stosl
  //Activate PAE
  mov %cr4, %eax
  or $0x20, %al
  mov %eax, %cr4
  //Activate x86_64
  mov $0xC0000080, %ecx
  rdmsr
  or $0x00000100, %eax
  wrmsr
  //Load Page Table
  mov $pagemapL4, %eax
  mov %eax, %cr3
  //Activate Paging
  mov %cr0, %eax
  bswap %eax
  or $0x80, %al
  bswap %eax
  mov %eax, %cr0
  //Jump to x86_64loader
  lgdt gdtr
  jmp $0x18, $start

.code64
//extern "C" void init(uint32_t eax, multiboot_info * ebx, uint64_t **** pt);
.global init
start:
  xor %rax, %rax
  mov $mb_ptr, %rsi
  lodsl
  mov %rax, %rdi
  lodsl
  mov %rax, %rsi
  mov $pagemapL4, %rdx
  call init
stop:
  cli
  hlt
  jmp stop
.section .data
gdt:
//NULL-descriptor
.quad 0
//32-bit code
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0x98
.byte 0xCF
.byte 0x00
//data
.word 0xFFFF
.word 0x0000
.byte 0x00
.byte 0x92
.byte 0xCF
.byte 00
//64-bit code
.int 0
.byte 0
.byte 0x98
.byte 0x20
.byte 0

gdtr:
  .word 4 * 8
  .int gdt
pmfill:
.int pagedirPT + 0x7
.int 0
pdptfill:
.quad 0x87
.quad 0x40000087
.quad 0x80000087
.quad 0xC0000087
.section .bss
mb_ptr:
// These 8 Bytes will never be used when the stack comes near it
.space 4096
kernel_stack:
.align 4096
pagemapL4:
.space 4096
pagedirPT:
.space 4096