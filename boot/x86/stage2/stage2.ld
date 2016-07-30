OUTPUT_FORMAT(binary);
SECTIONS {
    . = 0x0;
    .init_text :{
        *(.init_text);    
    }
    .text : {
        *(.text);
    }
    .data : {
        *(.data);
        *(.rodata);
    }
    .bss : {
        *(.bss);
        *(.end);
    }
}