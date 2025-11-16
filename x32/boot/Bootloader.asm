; Bootloader - second stage bootloader (NASM)
; setup env. mem map, filesystem, etc.

bits 16
org 0x9000

start:
    cli
    mov sp, 0x9000
    mov [driveNum], dl
    mov si, startup
    call print
.ClearScreen:
; clear the screen and enforce text mode 80 x 25 (03)
    mov al, 0x3
    mov ah, 0x0
    int 0x10
    mov dl, [driveNum]
.initA20:
    mov si, checka20
    call print
    call CheckA20
    jc activateA20
.loadKernel:
    ; load the kernel to 0x600
    ; because int 13h ah 2h can't handle boundary crossing
    mov si, loadingKern
    call print
    call LoadKernel
.RectifyKernel:
    ; copy the kernel to 0x100000
    call CopyKernel
.initGDT:
    mov si, loadtable
    call print
    call SetupGDT
    lgdt [gdtDescriptor]
.initIDT:
    lidt [idtDescriptor]
.enableProtected:
    cli
    mov eax, cr0
    or  eax, 1                  ; set PE bit
    mov cr0, eax                ; set cr0 to 1 (enable protected mode)
    jmp 0x8:protectedModeStart  ; should clear the prefetchqueue (cpu loads multiple inst. into mem at a time. Jumping frees the queue)

bits 32                         ; !!!important some instruction will still be treated as 16 bit if not used
protectedModeStart:
    nop                         ; make sure the queue is empty
    nop
.setupSegments:
    mov ax, 0x10                ; offset of segment 2 in GDT (=Data Segment)
    mov ds, ax                  ; set all segments
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    mov esp, 0x0030000           ; esp( extended stack pointer) grows downwards from 0x30000
    sti                          ; enable interrupts
.Kernel:
    jmp 0x8:0x00100000

bits 16

CopyKernel:
pusha
push es
push di
; first loadedkernel to 0x0600 now copying it to 0x100000 = 1 MiB
.Setup:
    xor ax, ax
    mov ds, ax
    mov ax, 0xF800
    mov es, ax
    mov si, 0x600
    mov di, 0x8000
    mov cx, 0x0800  ; 4 sectors (512 b) = 2048b = 2KB
.Loop:
    rep movsw       ; repeat cx ammount of times
.exit:
    pop di
    pop es
    popa
    ret

LoadKernel:
    pusha
    sti
.SetupRead:
; !!!es:bx may not be over or equal to 0x100000!!!
; !!!bochs just allows it but other bios's might give back faulty data, or hang!!!
    mov ax, 0x0
    mov es, ax
    mov bx, 0x0600
    mov ch, 0
    mov cl, 4           ; start at sector 4
    mov dh, 0
    mov ah, 0x2
    mov al, 4           ; load 4 sector 512 bytes
    int 0x13
    jc kernelLoadingError
    mov si, KernelLoad
    call print
.exit:
    cli
    popa
    ret

activateA20:
  mov ax, 0x2401
  int 0x15
  call CheckA20
  jnc .end
  ;; rest is to be implemented
.end:
    ret

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

;; SEGMENT 0 ;;
.NullDescriptor:
    ; offset 0
    mov cx, 0x4
    rep stosw  ; takes from ax saves to es:di

;; SEGMENT 1;;
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

;; SEGMENT 2 ;;
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
    call print
    cli
    jmp hangInState

kernelLoadingError:
    mov si, KernelLoaderror
    call print
    cli
    jmp hangInState

print:
    pusha
    push ds
    mov ax, 0x0000
    mov ds, ax

.print_loop:
    lodsb
    cmp al, 0
    jz .print_end
    mov ah, 0x0E
    int 0x10
    jmp .print_loop
.print_end:
    pop ds
    popa
    ret

hangInState:
hlt             ; low powermode
jmp hangInState

CheckA20:
; this should adress 0x100500 and wrap that address arround to 0x500 if A20 isn't enabled
    pusha
    push es
    push di
.setRegsLow:
    xor ax, ax
    mov es, ax
    mov di, 0x500
.setValLow:
    mov word es:[di], word 0x5555
.setRegsHigh:
    mov ax, 0xF800
    mov es, ax
    mov di, 0x8500
.SetValHigh:
    mov word es:[di], word 0xffff
.setRegsLowRead:
    xor ax, ax
    mov es, ax
    mov di, 0x500
.readVal:
    cmp es:[di], word 0x5555
    jz .active
.inactive:
    mov si, a20Inactive
    call print
    stc
    jmp .exit
.active:
    mov si, a20Active
    call print
    clc
    jmp .exit
.exit:
    pop di
    pop es
    popa
    ret
; clearing the screen will also clear dl
; so it's saved here
driveNum db 0

a20Inactive db 'a20 line is deactivated, trying to activate', 0x0d, 0x0A, 0
a20Active db 'a20 line is activated, continiung', 0x0D, 0x0A,0
a20Fail db 'program failed while setting up a20',0x0D, 0x0A, 0
checka20 db 'checking a20 line',0x0D, 0x0A, 0
startup db 'starting second-stage',0x0D, 0x0A, 0
KernelLoaderror db 'program failed while loading kernel',0x0D, 0x0A, 0
loadingKern db 'loading the kernel starting at sector 3',0x0D, 0x0A, 0
loadtable db 'loading the LGDT and IDGT',0x0D, 0x0A, 0
KernelLoad db 'kernel loaded without issues',0x0D, 0x0A, 0
gdt_desc times 0x10 db 0
times  1022 - ($ - $$) db 0
dw 0xAA55