ENTRY(_start)
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