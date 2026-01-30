#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "chip.h"
#include "font.h"

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
    memcpy(&chip->memory[FONT_START], font, sizeof(font));
}

void chip8_cycle(chip8_t * chip) {
    uint16_t opcode = (chip->memory[chip->pc] << 8) | chip->memory[chip->pc+1];
    chip->pc += 2;
    
    if (chip->pc > 4096) { return; }
    
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = (opcode & 0x000F);
    uint8_t NN = (opcode & 0x00FF);
    uint16_t NNN = (opcode & 0x0FFF);
    
    switch (opcode & 0xF000) {
        case 0x0000:
            if (opcode == 0x00E0) {
                memset(chip->gfx, 0, sizeof(chip->gfx));
            }
            else if (opcode == 0x00EE) {
                chip->sp--;
                chip->pc = chip->stack[chip->sp];
            }
            else {
                // exec machine code instruction at NNN
                break;
            }
            break;
        case 0x1000:
            // implement case
            break;
        case 0x2000:
            // implement case
            break;
        case 0x3000:
            // implement case
            break;
        case 0x4000:
            // implement case
            break;
        case 0x5000:
            // implement case
            break;
        case 0x6000:
            // implement case
            break;
        case 0x7000:
            // implement case
            break;
        case 0x8000:
            // implement case
            break;
        case 0x9000:
            // implement case
            break;
        case 0xA000:
            // implement case
            break;
        case 0xB000:
            // implement case
            break;
        case 0xC000:
            // implement case
            break;
        case 0xD000:
            // implement case
            break;
        case 0xE000:
            // implement case
            break;
        case 0xF000:
            // implement case
            break;
    }
}

