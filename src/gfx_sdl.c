#include <SDL2/SDL.h>
#include "chip.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;

#define SCALE 10
#define WIDTH 64
#define HEIGHT 32

void sdl_init(void) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
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
    for (int i = 0; i < WIDTH * HEIGHT; i++)  {
        pixels[i] = gfx[i] ? 0xFFFFFFFF : 0x00000000;
    }
    SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void sdl_handle_input(Chip8 * chip) {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { exit(0); }
        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            int pressed = (event.type == SDL_KEYDOWN);
            switch (event.key.keysym.sym) {
                case SDLK_1: chip->keys[0x1] = pressed; break;
                case SDLK_2: chip->keys[0x2] = pressed; break;
                case SDLK_3: chip->keys[0x3] = pressed; break;
                case SDLK_4: chip->keys[0xC] = pressed; break;

                case SDLK_q: chip->keys[0x4] = pressed; break;
                case SDLK_w: chip->keys[0x5] = pressed; break;
                case SDLK_e: chip->keys[0x6] = pressed; break;
                case SDLK_r: chip->keys[0xD] = pressed; break;

                case SDLK_a: chip->keys[0x7] = pressed; break;
                case SDLK_s: chip->keys[0x8] = pressed; break;
                case SDLK_d: chip->keys[0x9] = pressed; break;
                case SDLK_f: chip->keys[0xE] = pressed; break;

                case SDLK_y: chip->keys[0xA] = pressed; break;
                case SDLK_x: chip->keys[0xB] = pressed; break;
                case SDLK_c: chip->keys[0xC] = pressed; break;
                case SDLK_v: chip->keys[0xF] = pressed; break;
            }
        }
    }
}

const uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
