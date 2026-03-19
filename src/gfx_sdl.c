#include <SDL2/SDL.h>
#include "chip.h"
#include "gfx_sdl.h"

keymap_t layout[] = {
    {SDLK_1, 0x1}, {SDLK_2, 2}, {SDLK_3, 3}, {SDLK_4, 0xC},
    {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
    {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
    {SDLK_y, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
};

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;

void sdl_init(void) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    window = SDL_CreateWindow(
        "CHIP-8 EMULATOR",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH * SCALE,
        HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
        );
}

void sdl_draw(uint8_t * gfx) {
    uint32_t pixels[WIDTH * HEIGHT];
    for (unsigned i = 0; i < WIDTH * HEIGHT; i++)  {
        pixels[i] = gfx[i] ? 0xFFFFFFFF : 0x00000000;
    }
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_handle_input(chip8_t * chip) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { exit(0); }
        if (event.type == SDL_KEYDOWN ||event.type == SDL_KEYUP) {
            int pressed = (event.type == SDL_KEYDOWN);
            SDL_Keycode pressed_key = event.key.keysym.sym;
            for (unsigned i = 0; i < 16; i++) {
                if (pressed_key == layout[i].key) {
                    keymap_t current_key = layout[i];
                    chip->keys[current_key.value] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = current_key.value;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }

                }
            }
        }
    }
}
