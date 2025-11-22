//
// Created by Mxsxll on 17.11.2025.
//


#include "../IO/Keyboard/stdin.c"
#include "../IO/Screen/printf.c"
static char buffer[vgaWidth];
static int buffPtr = 0;
typedef struct commandBuff {
    char cmdbuff[vgaWidth];
    int cmdptr;
}command_buff;

int handleCommand(command_buff *cb) {
    return 0;
}

void startShell() {
    while (1) {
        unsigned char sc = keyboard_poll();
        struct KeyEvent ev = decode_scancode(sc);
        char ch = handle_key(ev);

        if (ch == (char)0) {
            continue;
        }
        if (ch == 'q') {
            printf("%fQuitting console", Red);
            return;
        }

        if (ch == 'c') {
            clearScreen();
            continue;
        }
        int ret = putChar(White, ch);
        if (ret == 1) {
            printf("%fIllegal char or out of bounds access", Red);
            continue;
        }
        if (ch == '\n') {
            command_buff cb;
            cb.cmdptr = buffPtr;
            memcpy(cb.cmdbuff, buffer, buffPtr);
            cb.cmdbuff[cb.cmdptr] = '\0';
            handleCommand(&cb);
            memset(buffer, 0, vgaWidth);
            buffPtr = 0;
            continue;
        }
        buffer[buffPtr] = ch;
        buffPtr = (buffPtr + 1)%vgaWidth;
    }
}
