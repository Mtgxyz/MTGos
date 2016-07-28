org 0x7C00
jmp 0x0000:start
start:
    cli
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0 ;Set stack
    sti
    mov ax, 0x0000
    mov es, ax
    mov ds, ax ;Set datasegments
    mov [bootdrv],dl
    call load ;Load stage 2
    mov ax, 0x1000
    mov es, ax
    mov ds, ax
    mov dl, [bootdrv]
    jmp 0x1000:0x0000

bootdrv db 0
loadmsg db "loading MTGosLoader x86...",13,10,0
resetmsg db "Could not reset drive...",13,10,0
loaderrmsg db "ERROR: Could not read from drive!",13,10,0
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

reseterr:
    mov si, resetmsg
    call putstr
    jmp load1
loadererr:
    mov si, loaderrmsg
    call putstr
    jmp $
    
load:
    mov cx, 11 ;Ten tries to reset drive
resetloop:
    dec cx
    jz reseterr
    mov ax, 0
    mov dl, [bootdrv]
    ;int 0x10
    jc resetloop
load1:
    mov dl, [bootdrv]
    mov cx, 2
    mov dh, 0
    mov ax, 0x1000
    mov es, ax
    mov bx, 0
    mov al, 10
    call read_sectors_16
    mov si,loadmsg
    call putstr
    retn
read_sectors_16:
    pusha
    mov si, 0x02    ; maximum attempts - 1
.top:
    mov ah, 0x02    ; read sectors into memory (int 0x13, ah = 0x02)
    int 0x13
    jnc .end        ; exit if read succeeded
    dec si          ; decrement remaining attempts
    jc  .end        ; exit if maximum attempts exceeded
    xor ah, ah      ; reset disk system (int 0x13, ah = 0x00)
    int 0x13
    jnc .top        ; retry if reset succeeded, otherwise exit
.end:
    popa
    retn

    
times 512-($-$$)-2 db 0
dw 0xAA55