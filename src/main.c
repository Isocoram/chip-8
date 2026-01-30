#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "chip.h"

#define CPU_SPEED 500

int main(int argc, char** argv) {
    (int *)argc;
    (char *)argv;
    chip8_t chip;
    init_chip(&chip);
    sdl_init();
    
    uint32_t last_timer = SDL_GetTicks();
    while (1) {
        sdl_handle_input(&chip);
        for (int i = 0; i < CPU_SPEED / 60; i++) {
            // handle cpu cycle
            // chip8_cycle(&chip);
            continue;
        }
        if (SDL_GetTicks() - last_timer >= 16) {
            if (chip.delay_timer > 0) { chip.delay_timer--; }
            if (chip.sound_timer > 0) { chip.sound_timer--; }
            last_timer = SDL_GetTicks();
        }
        sdl_draw(chip.gfx);
        SDL_Delay(1);
    }
    return 0;
}
