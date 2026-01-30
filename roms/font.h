#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#define CHIP8_FONT_SIZE 80
#define CHIP8_FONT_ADDR 0x50

extern const uint8_t font[CHIP8_FONT_SIZE];

#endif
