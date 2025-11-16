//
// Created by cocon on 16.11.2025.
//

typedef struct keyboard{
    unsigned char commandqueue[128];
    unsigned char responsequeue[128];
}Keyboard;

static Keyboard keyboard;