//
// Created by cocon on 15.11.2025.
// This is the base lib for printing to the screen in text mode
// it writes directly to video memory located at 0xB8000
// this is purely for the 80 x 25 16 colors mode or 03
# include "SPut.h"
# include "globals.c"
#include <stdint.h>

/**
 *
 * @param color color inside the 16 colors represented as an int
 * @param c char to be written
 * @return
 */
int putChar(int color, char c) {
    if (cursor.x >= vgaWidth || cursor.y >= vgaHeight) {
        return 1;
    }
    if (c == '\n' || c == '\r') {
        cursor.x = 0;
        cursor.y++;
        return 0;
    }
    volatile uint16_t* videoaddr = (volatile uint16_t*) vgaStart + (cursor.y * vgaWidth + cursor.x);
    *videoaddr = (uint16_t)c | ((uint16_t)color << 8);
    // advance the cursor or break to the next line
    cursor.y += (cursor.x = (cursor.x + 1)%vgaWidth) == 0? 1:0;
    return 0;
}

int putString(int color, const char *str) {
    while (*str) {
        if (putChar(color, *str++)) {
            return 1;
        }
    }
    return 0;
}

void clearScreen() {
    for (int i = 0; i < vgaHeight ; i++) {
        for (int j = 0; j < vgaWidth ; j++) {
            cursor.x = j;
            cursor.y = i;
            putChar(Black, '\0');
        }
    }
    cursor.x = 0;
    cursor.y = 0;
}




