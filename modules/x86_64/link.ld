ENTRY(getTable)
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
SECTIONS
{
        . = 0x109000;
        module_start = .;
        .text : {
                *(.text)
                *(.text.*)
                start_ctors = .;
                        KEEP(*( .init_array ));
                        KEEP(*(SORT_BY_INIT_PRIORITY( .init_array.* )));
                        *(.ctors)
                end_ctors = .;
                start_dtors = .;
                        *(.dtors)
                end_dtors = .;
                *(.data)
                *(.data.*)
                *(.rodata)
                *(.bss)
                *(COMMON)
        }
        module_end = .;
}