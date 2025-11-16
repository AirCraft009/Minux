//
// Created by cocon on 15.11.2025.
//

#ifndef MINUX_SPUT_H
#define MINUX_SPUT_H
#include <stdint.h>
#define vgaStart 0xb8000
#define vgaWidth 80
#define vgaHeight 25
#define vgaEnd (vgaStart + (vgaWidth * vgaHeight)*2)
#define Black 0
#define Blue 1
#define Green 2
#define Cyan 3
#define Red 4
#define Purple 5
#define Brown 6
#define Gray 7
#define DarkGray 8
#define LightBlue 9
#define LightGreen 10
#define LightCyan 11
#define LightRed 12
#define LightPurple 13
#define Yellow 14
#define White 15
#endif //MINUX_SPUT_H