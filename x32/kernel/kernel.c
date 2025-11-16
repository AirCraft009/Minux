
#include "IO/Screen/SPut.c"
#include "kernel.h"
// make sure that kmain() can be first
void Kmain(void) __attribute__((section(".text.start")));

void Kmain(void) {
    putString(White, 75, 12, "h\nel \nlo        wo\nrld\n!!");
    for (;;) {}
}
