; MBR.asm
; x64 bootloader (16-bit real mode)
org 0x7c00
bits 16

start:

    cli                 ; disable interrupts
    xor ax, ax
    mov ss, ax          ; set stack segment
    mov ds, ax
    mov es, ax
    mov sp, 0x7c00      ; set stack pointer
    jmp 0x00:loader     ; enforce cs:ip

loader:
    mov si, boot
    call print
.CheckPartitions:
    mov bx, PT1 ; start at the first offset
    mov cx, 4
.CheckLoop:
    mov al, byte [bx]
    test al, 0x80       ; check active bit
    jnz .ActivePart     ; load partition
    add bx, 0x10
    dec cx
    jnz .CheckLoop
    jmp PartitionError

.ActivePart:
.SetupRead:
    ; dl already contains the drive index
    xor ax, ax
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov es, ax
    mov ah, 0x2
    mov al, 4
    mov bx, 0x9000
    int 0x13
    jc PartitionError
    mov si, bootGood
    call print
    jmp 0:0x9000


; ---------------------------
; Print string routine
print:
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
    ret

; ---------------------------

no_edd:
    mov si, error_partition
    call print
    cli
    jmp hangInState

PartitionError:
    mov si, error_partition
    call print
    cli
    jmp hangInState

disk_error:
    mov si, error_dsk
    call print
    cli
    jmp hangInState

hangInState:
    hlt
    jmp hangInState

; ---------------------------
error_dsk        db 'Disk read error', 0
boot             db 'Booting into Minux...',0x0A,0x0D,0
error_partition  db 'No active Partition found',0x0A,0x0D,0
posRelocate      db 'successfully relocated',0x0A,0x0D,0
bootGood         db 'Booting 2nd stage bootloader',0x0A,0x0D,0
loadingDAP       db 'building the DAP',0x0A,0x0D,0
executingDAP     db 'Read the next sectors to 0x7c00',0x0A,0x0D,0

; ---------------------------
times (0x1BE - ($-$$)) db 0

PT1 times 16 db 0
PT2 times 16 db 0
PT3 times 16 db 0
PT4 times 16 db 0

dw 0xAA55
