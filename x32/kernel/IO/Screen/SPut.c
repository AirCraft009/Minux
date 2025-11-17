//
// Created by cocon on 15.11.2025.
// This is the base lib for printing to the screen in text mode
// it writes directly to video memory located at 0xB8000
// this is purely for the 80 x 25 16 colors mode or 03
# include "SPut.h"
# include "globals.c"
# include "../../util/mem.c"
#include <stdint.h>




int putCharCords(int color, char c, volatile uint8_t x, volatile uint8_t y) {
    if (x >= vgaWidth || y >= vgaHeight) {
        return 1;
    }
    if (c == '\n' || c == '\r') {
        return 2;
    }
    volatile uint16_t* videoaddr = (volatile uint16_t*) vgaStart + (y * vgaWidth + x);
    *videoaddr = (uint16_t)c | ((uint16_t)color << 8);
    return 0;
}

uint16_t * getRow(int row) {
    if (row >= vgaHeight || row < 0) {
        return NULL;
    }
    uint16_t * startAddr = (uint16_t*) vgaStart + (row * vgaWidth);
    return startAddr;
}

/**
 *
 * @param rowCount what row should be copied
 * @param destRow a uint16_t array with lenght 80(vgaWidth)
 * fills the destRow with the according row
 */
int fillRow(int rowCount, uint16_t destRow[80]) {
    if (destRow == NULL || rowCount > vgaHeight-1 || rowCount < 0) {
        return 1;
    }
    uint16_t * srcRow = getRow(rowCount);
    if (srcRow == NULL) {
        return 1;
    }
    memcpy(destRow, srcRow,  vgaWidth);
    return 0;
}

/**
 *
 * @param rowCount which row should be saved to a new array
 * @return a copy of the original array
 *
 */
uint16_t * saveRow(int rowCount, uint16_t destRow[80]) {
    if (fillRow(rowCount, destRow) == 1) {
        return NULL;
    }
    return destRow;
}

int clearRow(int rowCount) {
    if (rowCount > vgaHeight-1 || rowCount < 0) {
        return 1;
    }
    uint16_t * srcRow = getRow(rowCount);
    for (int i = 0; i < vgaWidth; i++) {
        putCharCords(Black, '\0', i, rowCount);
    }
    return 0;
}


void scrollUp() {
    uint16_t rowBuf[vgaWidth];

    // Shift every row y = 1..(vgaHeight-1) up to y-1
    for (int y = 1; y < vgaHeight; y++) {
        // read row y into a temp buffer
        saveRow(y, rowBuf);
        // write into row y-1
        memcpy(getRow(y - 1), rowBuf, sizeof(rowBuf));
    }

    clearRow(vgaHeight-1);
}


/**
 *
 * @param color color inside the 16 colors represented as an int
 * @param c char to be written
 * @return
 */
int putChar(int color, char c) {
    int ret = putCharCords(color, c, cursor.x, cursor.y);
    if (ret == 1) {
        return 1;
    }
    if (ret == 2) {
        scrollUp();
        cursor.x = 0;
        cursor.y ++;
    }
    // advance the cursor or break to the next line
    if ((cursor.x = (cursor.x + 1)%vgaWidth) == 0) {
        scrollUp();
        cursor.y ++;
    }
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
            putCharCords(Black, '\0', j, i);
        }
    }
    cursor.x = 0;
    cursor.y = 0;
}




