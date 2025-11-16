#include "kernel.h"
#include "IO/Screen/printf.c"
// make sure that kmain() can be first
void Kmain(void) __attribute__((section(".text.start")));


void init() {
    cursor.x = 40;
    cursor.y = 12;
}

void Kmain(void) {
    init();
    printf("%f Hello %f world%f Du hund %d", Red, LightBlue, Red, -123);
    for (;;) {}
}

