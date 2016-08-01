ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)
SECTIONS
{
        . = 0x100000;
        kernel_start = .;
        .text : {
                *(multiboot)
                *(.text)
        }
        .data : {
                start_ctors = .;
                        KEEP(*( .init_array ));
                        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
                        *(.ctors)
                end_ctors = .;
                start_dtors = .;
                        *(.dtors)
                end_dtors = .;
                *(.data)
        }
        .rodata : {
                *(.rodata)
        }
        .bss : {
                *(.bss)
        }
        kernel_end = .;
}