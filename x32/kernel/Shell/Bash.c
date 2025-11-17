//
// Created by Mxsxll on 17.11.2025.
//


#include "../IO/Keyboard/stdin.c"
#include "../IO/Screen/printf.c"

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
            putChar(White, ch);
    }
}
