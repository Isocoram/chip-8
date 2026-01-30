#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
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
    memcpy(&chip->memory[FONT_START], font, sizeof(font));
}

void chip8_cycle(chip8_t * chip) {
    uint16_t opcode = (chip->memory[chip->pc] << 8) | chip->memory[chip->pc+1];
    chip->pc += 2;
    
    if (chip->pc > 4096) {
        return;
    }
    
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
            chip->pc = NNN;
            // implement case
            break;
        case 0x2000:
            chip->stack[chip->sp] = chip->pc;
            chip->sp++;
            chip->pc = NNN;
            break;
        case 0x3000:
            if (chip->V[X] == NN) {
                chip->pc += 2;
            }
            break;
        case 0x4000:
            if (chip->V[X] != NN) {
                chip->pc += 2;
            }
            break;
        case 0x5000:
            if (chip->V[X] == chip->V[Y]) {
                chip->pc += 2;
            }
            break;
        case 0x6000:
            chip->V[X] = NN;
            break;
        case 0x7000:
            chip->V[X] += NN;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    chip->V[X] = chip->V[Y];
                    break;
                case 0x0001:
                    chip->V[X] = (chip->V[X] | chip->V[Y]);
                    break;
                case 0x0002:
                    chip->V[X] = (chip->V[X] & chip->V[Y]);
                    break;
                case 0x0003:
                    chip->V[X] = (chip->V[X] ^ chip->V[Y]);
                    break;
                case 0x0004: {
                    uint16_t sum = (chip->V[X] + chip->V[Y]);
                    chip->V[0xF] = (sum > 0xFF);
                    chip->V[X] = sum & 0xFF;
                    break;
                }
                case 0x0005:
                    chip->V[0xF] = (chip->V[X] < chip->V[Y]);
                    chip->V[X] = chip->V[X] - chip->V[Y];
                    break;
                case 0x0006:
                    chip->V[0xF] = chip->V[X] & 0x01;
                    chip->V[X] >>= 1;
                    break;
                case 0x0007:
                    chip->V[0xF] = (chip->V[Y] < chip->V[X]);
                    chip->V[X] = chip->V[Y] - chip->V[X];
                    break;
                case 0x000E:
                    chip->V[0xF] = (chip->V[X] & 0x80) >> 7;
                    chip->V[X] <<= 1;
                    break;
            }
            break;
        case 0x9000:
            if (chip->V[X] != chip->V[Y]) {
                chip->pc += 2;
            }
            break;
        case 0xA000:
            chip->I = NNN;
            break;
        case 0xB000:
            // handle BXNN support later
            chip->I = chip->V[0] + NNN;
            break;
        case 0xC000:
            srand((int)time(NULL));
            uint8_t random = (uint8_t)(rand() ^ (rand() >> 8)) & 0xFF;
            chip->V[X] = random & NN;
            break;
        case 0xD000: {
            uint8_t VX = chip->V[X] % 64;
            uint8_t VY = chip->V[Y] % 32;
            uint8_t height = N;
            chip->V[0xF] = 0;
            for (int i = 0; i < height; i++) {
                uint8_t sprite = chip->memory[chip->I + i];
                for (int j = 0; j < 8; j++) {
                    if (sprite & (0x80 >> j)) {
                        int x = (VX + j) % 64;
                        int y = (VY + i) % 32;
                        int index = x + 64 * y;
                        if (chip->gfx[index] == 1) {
                            chip->V[0xF] = 1;
                        }
                        chip->gfx[index] ^= 1;
                    }
                }
            }
            break;
        }
        case 0xE000:
            // implement case
            break;
        case 0xF000:
            // implement case
            break;
    }
}

