#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "chip.h"
#include "gfx_sdl.h"

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    srand(time(NULL));
    chip8_t chip;
    init_chip(&chip);
    
    load_rom(&chip, "./roms/IBM.ch8");
    /*
    chip.gfx[0] = 1;
    chip.gfx[1] = 0;
    chip.gfx[2] = 1;
    chip.gfx[64] = 0;
    chip.gfx[65] = 1;
    chip.gfx[66] = 0;
    chip.gfx[128] = 1;
    chip.gfx[129] = 0;
    chip.gfx[130] = 1;
   */
    
    sdl_init();
    
    uint32_t last_timer = SDL_GetTicks();
    while (1) {
        sdl_handle_input(&chip);
        for (int i = 0; i < CPU_SPEED / 60; i++) {
            chip8_cycle(&chip);
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
