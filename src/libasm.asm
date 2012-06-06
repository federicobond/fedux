GLOBAL      _read_msw,_lidt

GLOBAL      _timertick_handler
GLOBAL      _keyboard_handler
GLOBAL      _serial_handler

GLOBAL      _mask_pic_2,_mask_pic_1,_cli,_sti
GLOBAL      _outb
GLOBAL      _inb
GLOBAL		_hlt

EXTERN      timertick_handler
EXTERN      keyboard_handler
EXTERN      serial_handler

SECTION .text

_outb:
    push    ebp
    mov     ebp, esp
    push    eax
    push    edx
    mov     eax, [ss:ebp+8]
    mov     edx, [ss:ebp+12]
    out     dx, al
    pop     edx
    pop     eax
    mov     esp, ebp
    pop     ebp
    ret

_inb:
    push    ebp
    mov     ebp, esp
    push    edx
    mov     edx, [ss:ebp+8]
    xor     eax, eax
    in      al, dx
    pop     edx
    mov     esp, ebp
    pop     ebp
	ret

_hlt:
	hlt
	ret

_cli:
    cli
    ret

_sti:
    sti
    ret

_mask_pic_1:
    push    ebp
    mov     ebp, esp
    mov     ax, [ss:ebp+8]          ; ax = 16 bit mask
    out     21h,al
    pop     ebp
    retn

_mask_pic_2:
    push    ebp
    mov     ebp, esp
    mov     ax, [ss:ebp+8]          ; ax = 16 bit mask
    out     0A1h,al
    pop     ebp
    retn

_read_msw:
    smsw    ax                      ; Get machine status word
    retn


_lidt:                              ; Load the IDTR
    push    ebp
    mov     ebp, esp
    push    ebx
    mov     ebx, [ss:ebp+6]         ; ds:bx = IDTR pointer 
    rol     ebx,16
    lidt    [ds:ebx]                ; Load IDTR
    pop     ebx
    pop     ebp
    retn


_timertick_handler:                 ; INT 0x08 Handler (Timertick)
    push    ds
    push    es                      ; Save registers
    pusha                       
    mov     ax, 10h
    mov     ds, ax                  ; Load DS and ES with the selector value
    mov     es, ax                  
    call    timertick_handler
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popa                            
    pop     es
    pop     ds
    iret

_keyboard_handler:                  ; INT 0x09 Handler (Keyboard)
    push    ds
    push    es                      ; Save registers
    pusha
    mov     ax, 10h
    mov     ds, ax                  ; Load DS and ES with the selector value
    mov     es, ax
    call    keyboard_handler
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popa
    pop     es
    pop     ds
    iret

_serial_handler:                    ; INT 0x09 Handler (Keyboard)
    push    ds
    push    es                      ; Save registers
    pusha
    mov     ax, 10h
    mov     ds, ax                  ; Load DS and ES with the selector value
    mov     es, ax
    call    serial_handler
    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popa
    pop     es
    pop     ds
    iret

_syscall:
    push    ebp
    mov     ebp, esp
    pusha
    pushf
    mov     eax, [ss:ebp+28]
    mov     ebx, [ss:ebp+24]
    mov     ecx, [ss:ebp+20]
    mov     edx, [ss:ebp+16]
    mov     esi, [ss:ebp+12]
    mov     edi, [ss:ebp+8]
    int     80h
    popf
    popa
    mov     esp, ebp
    pop     ebp
    ret

