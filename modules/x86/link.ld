ENTRY(getTable)
SECTIONS
{
        . = 0x106000;
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
        }
        module_end = .;
}