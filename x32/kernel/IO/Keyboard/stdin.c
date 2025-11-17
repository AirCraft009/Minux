//
// Created by cocon on 16.11.2025.
//

#include "globals.c"
#include "../../util/portHelper.c"



char keyboard_poll(void) {
    // Wait for Output Buffer Full (bit 0)
    while (!(inb(0x64) & 0x01)){
        // this is blocking IO ok for a console but bad in general
    }

    // Now the scan code is ready
    return inb(0x60);
}

static int extended_state = 0;

struct KeyEvent decode_scancode(unsigned char sc) {
    struct KeyEvent ev = {0};

    if (sc == 0xE0) {
        extended_state = 1;
        return ev;  // no event yet
    }

    ev.scancode = sc & 0x7F;           // strip break bit
    ev.pressed  = !(sc & 0x80);        // pressed if bit7 = 0
    ev.extended = extended_state;

    extended_state = 0;
    return ev;
}
static int shift = 0;

static const char table_normal[128] = {
    [0x1E] = 'a', [0x30] = 'b', [0x2E] = 'c', [0x20] = 'd',
    [0x12] = 'e', [0x21] = 'f', [0x22] = 'g', [0x23] = 'h',
    [0x17] = 'i', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l',
    [0x32] = 'm', [0x31] = 'n', [0x18] = 'o', [0x19] = 'p',
    [0x10] = 'q', [0x13] = 'r', [0x1F] = 's', [0x14] = 't',
    [0x16] = 'u', [0x2F] = 'v', [0x11] = 'w', [0x2D] = 'x',
    [0x15] = 'y', [0x2C] = 'z',

    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0',

    [0x1C] = '\n',
    [0x39] = ' ',
    [0x0E] = '\b',
};

static const char table_shift[128] = {
    [0x1E] = 'A', [0x30] = 'B', [0x2E] = 'C', [0x20] = 'D',
    [0x12] = 'E', [0x21] = 'F', [0x22] = 'G', [0x23] = 'H',
    [0x17] = 'I', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',
    [0x32] = 'M', [0x31] = 'N', [0x18] = 'O', [0x19] = 'P',
    [0x10] = 'Q', [0x13] = 'R', [0x1F] = 'S', [0x14] = 'T',
    [0x16] = 'U', [0x2F] = 'V', [0x11] = 'W', [0x2D] = 'X',
    [0x15] = 'Y', [0x2C] = 'Z',

    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$',
    [0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*',
    [0x0A] = '(', [0x0B] = ')',

    [0x1C] = '\n',
    [0x39] = ' ',
    [0x0E] = '\b',
};

// Shift scancodes (Set 1)
#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36

char handle_key(struct KeyEvent ev) {
    if (ev.scancode == SC_LSHIFT || ev.scancode == SC_RSHIFT) {
        shift = ev.pressed;
        return 0;
    }

    if (!ev.pressed) return 0;

    return shift ? table_shift[ev.scancode] : table_normal[ev.scancode];
}



