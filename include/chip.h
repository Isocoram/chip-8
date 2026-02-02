#ifndef CHIP_H
#define CHIP_H

#include <stdint.h>

#define CPU_SPEED 60
#define FONT_START 0x000
#define STACK_SIZE 16

typedef struct  {
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint8_t gfx[64*32];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t keys[16];
    uint8_t waiting_for_key;
    uint8_t wait_reg;
} chip8_t;

void init_chip(chip8_t * chip);
void load_rom(chip8_t * chip, const char *path);
void chip8_cycle(chip8_t * chip);


#endif
