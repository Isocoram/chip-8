#ifndef GFX_SDL_H
#define GFX_SDL_H

#include <stdint.h>

void sdl_init(void);
void sdl_draw(uint8_t * gfx);
void sdl_handle_input(chip8_t * chip);

#endif
