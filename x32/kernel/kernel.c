#include "kernel.h"
#include "Shell/Bash.c"
// make sure that kmain() can be first
void Kmain(void) __attribute__((section(".text.start")));


void init() {
    cursor.x = 0;
    cursor.y = 12;
}

void Kmain(void) {
    init();
    startShell();
    for (;;) {}
}

