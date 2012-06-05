GLOBAL      _read_msw,_lidt

GLOBAL      _timertick_handler
GLOBAL      _keyboard_handler
GLOBAL		_syscall_handler

GLOBAL      _mask_pic_2,_mask_pic_1,_cli,_sti
GLOBAL      _outb
GLOBAL      _inb
GLOBAL		_hlt

EXTERN      timertick_handler
EXTERN      keyboard_handler
EXTERN		syscall_handler

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

_syscall_handler:					; INT 0x80 Handler (Syscall)
    push    ds
    push    es                      ; Save registers
    pusha      
	push	eax                 
    mov     ax, 10h
    mov     ds, ax                  ; Load DS and ES with the selector value
    mov     es, ax                  
	pop		eax

	push	edi						; Call syscall handler with parameters
	push	esi
	push 	edx
	push	ecx
	push	ebx
	push	eax    
	call    syscall_handler
	pop		ebx						; Discard EAX push. It's used as retval
	pop		ebx
	pop		ecx
	pop		edx
	pop		esi
	pop		edi

    mov     al, 20h                 ; Send generic EOI to PIC
    out     20h, al
    popa                            
    pop     es
    pop     ds
    iret

