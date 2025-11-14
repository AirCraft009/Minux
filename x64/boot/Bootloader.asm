; Bootloader - second stage bootloader (NASM)
; setup env. mem map, filesystem, etc.
;

bits 16
org 0x9000

start:
    cli
    mov sp, 0x9000
.initA20:
    call CheckA20
    jz activateA20
.initGDT:
    call SetupGDT
    lgdt [gdtDescriptor]
.initIDT:
    lidt [idtDescriptor]
.enableProtected:
    cli
    mov eax, cr0
    or  eax, 1            ; set PE bit
    mov cr0, eax
    jmp 0x0:protectedModeStart

protectedModeStart:
    jmp protectedModeStart

activateA20:
    ; to be implemented
    ; bochs's BIOS automatically enables A20 so I'll worry about this later

idtDescriptor:
.limit:
    dw 2048
.linAddress:
    dd 0x8000

gdtDescriptor:
.limit:
    dw 23
.linAddress:
    dd 0x800

;gdt size 24bytes
SetupGDT:
pusha
xor ax, ax
mov es, ax      ; set the segment to 0
mov di, 0x800   ; location of the gdt is 0x800
.NullDescriptor:
    ; offset 0
    mov cx, 0x4
    rep stosw  ; takes from ax saves to es:di
.CodeSegment:
    ; offset 8
    ;limit
    mov es:[di], word 0xffff
    ;base
    mov es:[di + 2], word 0x0000
    mov es:[di+4], byte 0x0
    ;access
    mov es:[di+5], byte 0x9a
    ;flags
    mov es:[di+6], byte 0xcf
    ;base
    mov es:[di+7], 0x00
    add di, 8
.DataSegment:
    ; offset 16
    ;limit
    mov es:[di], word 0xffff
    ;base
    mov es:[di+2], word 0x0000
    mov es:[di+4], byte 0x0
    ;access
    mov es:[di+5], byte 0x92
    ;flags
    mov es:[di+6], byte 0xcf
    ;base
    mov es:[di+7], byte 0x00
    ;offset 24
.exit:
    popa
    ret

failedA20:
    mov si, a20Fail
.print:
    lodsb               ; loads val from si to al
    cmp al, 0           ; check for null terminator
    jz .hangInState
    mov ah, 0x0E        ; teletype
    int 0x10
    jmp .print
    cli

.hangInState:
hlt             ; low powermode
jmp .hangInState

CheckA20:
; this should adress 0x100001 and wrap that address arround to 0x0 if A20 isn't enabled
    pusha
    push es
    push di
    mov ax, 0xF800
    mov es, ax
    mov di, 0x8000
.SetVal:
    mov word es:[di], word 0xffff
.readVal:
    xor ax, ax
    mov es, ax
    mov di, 0x0
    cmp es:[di], word 0xffff
.exit:
    pop di
    pop es
    popa
    ret





a20Fail db 'program failed while setting up a20',0x0D, 0x0A, 0
gdt_desc times 0x10 db 0