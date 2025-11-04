; location of the buffer in si
; color in bh, bl
print:
    pusha           ; push all general purp. reg.
    lodsb           ; from si to al
    cmp al, 0       ; check for null ptr
    je  .terminate
    mov ah, 0x0e
    int 0x10
    jmp print

.terminate:
    popa
    ret