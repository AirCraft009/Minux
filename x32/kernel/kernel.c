#include "kernel.h"
#include "Shell/Bash.c"
// make sure that kmain() can be first
void Kmain(void) __attribute__((section(".text.start")));


void init() {
    cursor.x = 0;
    cursor.y = 0;
}

void Kmain(void) {
    clearScreen();
    init();
    startShell();
    for (;;) {}
}

