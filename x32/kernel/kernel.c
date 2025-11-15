//
// Created by cocon on 15.11.2025.
//

#include "kernel.h"

#include <stdint.h>

void Kmain(void) {
    volatile uint16_t* vga = (uint16_t*)0xB8000;
    vga[0] = 0x0F00 | 'K';   // character 'K', color 0x0F (white on black)

    for (;;) {}
}
