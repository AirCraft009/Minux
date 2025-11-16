//
// Created by cocon on 15.11.2025.
// This is the base lib for printing to the screen in text mode
// it writes directly to video memory located at 0xB8000
// this is purely for the 80 x 25 16 colors mode or 03
# include "SPut.h"

#include <stdint.h>

/**
 *
 * @param color color inside the 16 colors represented as an int
 * @param c char to be written
 * @param x x location inside the 80 x 25 grid
 * @param y y location inside the 80 x 25 grid
 * @return
 */
int putChar(int color, char c, uint8_t x, uint8_t y) {
    if (x >= vgaWidth || y >= vgaHeight) {
        return 1;
    }
    volatile uint16_t* videoaddr = (volatile uint16_t*) vgaStart + (y * vgaWidth + x);
    *videoaddr = (uint16_t)c | ((uint16_t)color << 8);
    return 0;
}

int putString(int color, uint8_t x, uint8_t y, const char *str) {
    while (*str) {

        if (*str == '\n' || *str == '\r') {
            x = 0;
            y++;
            str++;
        }else {
            if (putChar(color, *str++, x, y)) {
                return 1;
            }
            // break to the next line
            y += (x = (x + 1)%vgaWidth) == 0;
        }
    }
    return 0;
}



