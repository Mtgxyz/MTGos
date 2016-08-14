ENTRY(getTable)
SECTIONS
{
        . = /*0x106000;*/0;
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
                *(.plt)
                *(.rel.plt)
                *(.rel.dyn)
        }
        module_end = .;
}     
