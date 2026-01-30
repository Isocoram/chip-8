#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "chip.h"
#include "font.h"

#define FONT_START 0x50

void init_chip(chip8_t * chip) {
    memset(chip->memory, 0, sizeof(chip->memory));
    memset(chip->V, 0, sizeof(chip->V));
    chip-> I = 0;
    chip->pc = 0x200;
    memset(chip->gfx, 0, 64 * 32);
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    memset(chip->stack, 0, sizeof(chip->stack));
    chip->sp = 0;
    memset(chip->keys, 0, 16);
    
    // Chip-8 font must be loaded at 0x50, 16 chars x 5 bytes = 80 bytes
    memcpy(&chip->memory[0x50], font, sizeof(font));
}
