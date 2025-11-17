//
// Created by cocon on 16.11.2025.
//

typedef struct keyboard{
    unsigned char commandQueue[128];
    unsigned char responseQueue[128];
    unsigned char status;
    unsigned char data;
}Keyboard;

struct KeyEvent {
    unsigned char scancode;
    int pressed;       // 1 = down, 0 = up
    int extended;      // 1 if preceded by E0
};


static Keyboard keyboard;

//https://wiki.osdev.org/I8042_PS/2_Controller
void initRead() {
    keyboard.status |= 0x01;
}

void initWrite() {
    keyboard.status |= 0x02;
}

void initSendCommand() {
    keyboard.status |= 0x08;
}

void initSendData() {
    keyboard.status ^= 0x08;
}

