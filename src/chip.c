#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "chip.h"
#include "font.h"

void load_font(uint8_t * memory) {
    for (unsigned i = 0; i < CHIP8_FONT_SIZE; i++) {
        memory[i] = font[i];
    }
}

void init_chip(chip8_t * chip) {
    memset(chip->memory, 0, sizeof(chip->memory));
    memset(chip->V, 0, sizeof(chip->V));
    chip->I = 0;
    chip->pc = 0x200;
    memset(chip->gfx, 0, 64 * 32);
    chip->delay_timer = 0;
    chip->sound_timer = 0;
    memset(chip->stack, 0, sizeof(chip->stack));
    chip->sp = 0;
    memset(chip->keys, 0, 16);
    // memcpy(&chip->memory[FONT_START], font, sizeof(font));
    load_font(&chip->memory[FONT_START]);
    chip->waiting_for_key = 0;
    chip->wait_reg = 0;
}

void load_rom(chip8_t * chip, const char * path) {
    FILE * file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open ROM: %s\n", path);
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long rom_size = ftell(file);
    rewind(file);
    if (rom_size > (4096 - 0x200)) {
        fprintf(stderr, "ROM too large: %ld bytes\n", rom_size);
        fclose(file);
        exit(1);
    }
    fread(&chip->memory[0x200], sizeof(uint8_t), rom_size, file);
    fclose(file);
}

void chip8_cycle(chip8_t * chip) {
    if (chip->pc > 4096 || chip->waiting_for_key) {
        return;
    }
    uint16_t opcode = (chip->memory[chip->pc] << 8) | chip->memory[chip->pc+1];
    chip->pc += 2;
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
                if (chip->sp == 0) {
                    fprintf(stderr, "Stack Overflow!\n");
                    return;
                }
                chip->pc = chip->stack[--chip->sp];
            }
            else {
                // exec machine code instruction at NNN
                break;
            }
            break;
        case 0x1000:
            chip->pc = NNN;
            break;
        case 0x2000:
            if (chip->sp >= STACK_SIZE) { fprintf(stderr,"Stack overflow!\n");
                return; }
            chip->stack[chip->sp++] = chip->pc;
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
        case 0xC000: {
            uint8_t random = (uint8_t)(rand() ^ (rand() >> 8)) & 0xFF;
            chip->V[X] = (random & 0xFF) & NN;
            break;
        }
        case 0xD000: {
            uint8_t VX = chip->V[X] & 63; // = 0b00111111
            uint8_t VY = chip->V[Y] & 31; // = 0b00011111
            uint8_t height = N;
            chip->V[0xF] = 0;
            for (unsigned i = 0; i < height; i++) {
                uint8_t sprite = chip->memory[chip->I + i];
                for (unsigned j = 0; j < 8; j++) {
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
            switch (NN) {
                case 0x009E: {
                    uint8_t key = chip->V[X] & 0x0F;
                    if (chip->keys[key]) {
                        chip->pc += 2;
                    }
                    break;
                }
                case 0x00A1: {
                    uint8_t key = chip->V[X] & 0x0F;
                    if (!(chip->keys[key])) {
                        chip->pc += 2;
                    }
                    break;
                }
            }
            break;
        case 0xF000:
            switch (NN) {
                case 0x0007:
                    chip->V[X] = chip->delay_timer;
                    break;
                case 0x0015:
                    chip->delay_timer = chip->V[X];
                    break;
                case 0x0018:
                    chip->sound_timer = chip->V[X];
                    break;
                case 0x001E:
                    chip->I += chip->V[X];
                    if (chip->I > 0xFFF) {
                        chip->V[0xF] = 1;
                    } else {
                        chip->V[0xF] = 0;
                    }
                    break;
                case 0x000A:
                    chip->waiting_for_key = 1;
                    chip->wait_reg = X;
                    chip->pc -= 2;
                    break;
                    
        }
        break;
    }
}

