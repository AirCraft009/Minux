; Bootloader - second stage bootloader (NASM)
; setup env. mem map, filesystem, etc.
;

bits 16
org 0x9000

start:
    cli
    mov sp, 0x9000
    mov bh, 0
    mov bl, 0x7
.initA20:
    mov ax, 0x2401
    int 0x15
    jc failedA20
.initGDT:
    lgdt[gdt_desc]
.enableProtected:
    cli
    mov eax, cr0
    or  eax, 1            ; set PE bit
    mov cr0, eax
    jmp 0x0:protectedModeStart


protectedModeStart:


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

a20Fail db 'program failed while setting up a20',0x0D, 0x0A, 0
gdt_desc times 0x10 db 0