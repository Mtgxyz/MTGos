bits 16
section .init_text
start:
    mov ax, 0x1000 ; update segment registers
    mov ds, ax
    mov es, ax
    mov dl, [bootdrv]
    mov si, bootmsg
    call putstr
    call a20_gate
    mov si, a20success
    call putstr
    mov si, unrealstart
    call putstr
    cli
    mov si, smiley
    call putstr
extern main
    call main
    jmp $
bootmsg db "Loaded MTGosloader. Checking for any open A20 gates...",13,10,0
a20open db "It seems that A20 is already activated... Weird",13,10,0
a20m2 db "Your system doesn't support opening A20 gates via BIOS",13,10,0
a20m3 db "Your system doesn't support fast A20 gate",13,10,0
nodetect1 db "Your BIOS can't doesn't know whether the A20 gate is open or not.",13,10,0
a20failed db "Can't access the A20 gate. Sure you meet the requirements? (286)",13,10,0
a20success db "Successfully opened the A20 gate!",13,10,0
unrealstart db "Trying to init unreal mode",13,10,0
smiley db "Do you see a smiley in the top-left corner?",13,10,0
bootdrv db 0
global putstring

putstring:
    push bp
    mov bp, sp
    push si
    push bx
    mov si, [bp+4]
    call putstr
    pop bx
    pop si
    pop bp
    ret
putstr:
    lodsb
    or al, al
    jz short eputstr
    mov ah,0x0E
    mov bx,0x0007
    int 0x10 ;Write letter to screen
    jmp putstr
eputstr:
    retn
check_a20:
    pushf
    push ds
    push es
    push di
    push si
 
    cli
 
    xor ax, ax ; ax = 0
    mov es, ax
 
    not ax ; ax = 0xFFFF
    mov ds, ax
 
    mov di, 0x0500
    mov si, 0x0510
 
    mov al, byte [es:di]
    push ax
 
    mov al, byte [ds:si]
    push ax
 
    mov byte [es:di], 0x00
    mov byte [ds:si], 0xFF
 
    cmp byte [es:di], 0xFF
 
    pop ax
    mov byte [ds:si], al
 
    pop ax
    mov byte [es:di], al
 
    mov ax, 0
    je check_a20__exit
 
    mov ax, 1
 
check_a20__exit:
    pop si
    pop di
    pop es
    pop ds
    popf
 
    ret

a20_gate:
    call check_a20
    or ax, ax
    jne a20_active_lol
    mov ax, 0x2403
    int 0x15
    jb a20_method2
    cmp ah,0 
    jnz a20_method2
    mov ax ,0x2402
    int 0x15
    jb a20_nodetect1
    cmp ah, 0
    jnz a20_nodetect1
    cmp al, 1
    jz a20_active_lol
    mov ax, 0x2401
    int 0x15
    jb a20_failed
    cmp ah,0
    jnz a20_failed
    ret

a20_active_lol:
    mov si, a20open
    jmp putstr
a20_nodetect1:
    mov si, nodetect1
    call putstr
a20_method2:
    mov si, a20m2
    call putstr
    in al, 0x92 ;Read in FA20 
    test al, 2 ;Check if it supports
    jnz a20_method3
    or al, 2 ;Turn it on
    and al, 0xFE ;Make sure that the CPU doesn't reset
    out 0x92, al
    ret
a20_method3: ;Our last attempt
    mov si, a20m3
    call putstr
    cli
    call a20wait
    mov al, 0xAD
    out 0x64, al
    call a20wait
    mov al, 0xD0
    out 0x64,al
    call a20wait2
    in al, 0x60
    push ax
    call a20wait
    mov al, 0xD1
    out 0x64,al
    call a20wait
    pop ax
    or al,2
    out 0x60,al
    call a20wait
    mov al, 0xAE
    out 0x64, al
    call a20wait
    sti
    call check_a20
    or ax, ax
    jz a20_failed
    ret
a20wait:
    in al, 0x64
    test al, 2
    jnz a20wait
    ret
a20wait2:
    in al, 0x64
    test al, 1
    jnz a20wait2
    ret
a20_failed:
    mov si,a20_failed
    call putstr
    jmp $
    
    
gdtinfo:
   dw gdt_end - gdt - 1   ;last byte in table
   dd gdt                 ;start of table
 
gdt         dd 0,0        ; entry 0 is always unused
flatcode    db 0xff, 0xff, 0, 0, 0, 10011010b, 10001111b, 0
flatdesc    db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:
section .end
times 512 hlt