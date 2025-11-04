; MBR.asm
; x64 bootloader if looking for commented code look at the x86 version

org 0x600
bits 16

start:
    cli                 ; disable intr.
    xor ax, ax
    mov sp, 0x7c00      ; stack pointer to 0x7c00
    mov ss, ax          ; stack seg. to 0 full stack addr 0:0x7c00
.relocate:
    mov si, 0x7c00      ; currentloc
    mov cx, 0x100       ; ammount
    mov di, 0x600       ; new location
    rep movsw
    jmp 0:loader

; loads the second stage bootloader
loader:
    mov byte[DriveNum], dl
.CheckPartitions:
    mov bx, PT1         ; set the base to the beginning of the 1st parti.
    mov cx, 4           ; ammount of partitions
.CheckLoop:
    mov al, byte[bx]    ; get the drive atr. bit 7 set Partition layout (https://wiki.osdev.org/MBR_(x86))
    test al, 0x40       ; 0x40 = 01000000
    jnz .ActivePart
    add bx, 0x10        ; increment by 16 (Part. size)
    dec cx              ; counter --
    jnz .CheckLoop      ; dec doesn't set CF but ZF
    jmp PartitionError
.ActivePart:
    xor ax, ax
    mov word[actPartitionoffset], bx
    add bx, 8           ; get to 4byte LBA addr (https://wiki.osdev.org/MBR_(x86))
    mov EBX, dword[bx]   ; move the actual val into EBX 32bit
    mov es, ax
    mov cx, 0x1         ;
    mov di, 0x7c00
    call readSectors
    jmp 0x00:0x7c00



readSectors:
; make a Disk address pack https://en.wikipedia.org/wiki/INT_13H#INT_13h_AH=42h:_Read_Sectors_From_Drive
.initDap:
    mov byte [DAP], 0x10
    mov word [DAP + 2], cx
    mov word [DAP + 4], di
    mov word [DAP + 6], 0x0
    ; start at the 0th lba block
.setup:
    xor ax, ax
    mov ah, 0x42
    mov dl, byte[DriveNum]
    mov ds, ax
    mov si, DAP
.exeuteRead:
    int 0x13        ; read sectors extended(for LBA)
    jc disk_error
.JumpToLoaded:
    cmp Word[0x7DFE], 0xAA55
    jnz PartitionError
    mov si, word[actPartitionoffset]
    mov dl, byte[DriveNum]
    jmp 0x7c00



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

print:
    lodsb               ; loads val from si to al
    cmp al, 0           ; check for null terminator
    jz .terminate
    mov ah, 0x0E        ; teletype
    int 0x10
    jmp print

.terminate:
    ret

hangInState:
hlt
jmp hangInState


error_dsk db 'Disk read error', 0
error_partition db 'No active Partition found', 0

DAP times 16 db 0
DriveNum db 0
actPartitionoffset dw 0         ; 16-bit offset

times (0x1BE -  ($-$$)) db 0

DriveID times 4 db 0

; partition sectors nesecarry to boot from hdd
PT1 times 16 db 0
PT2 times 16 db 0
PT3 times 16 db 0
PT4 times 16 db 0
; magic number
dw 0xAA55