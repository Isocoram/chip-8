#pragma once

typedef struct  {
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint8_t gfx[64*32];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t keys[16];
} Chip8; 

void init_chip(Chip8 * chip);
void sdl_init(void);
void sdl_draw(uint8_t * gfx);
void sdl_handle_input(Chip8 * chip);
