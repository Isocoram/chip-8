#include <SDL2/SDL.h>
#include "chip.h"
#include "gfx_sdl.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Texture * texture;

void sdl_init(void) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    window = SDL_CreateWindow(
        "CHIP-8",
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
            switch (event.key.keysym.sym) {
                case SDLK_1:
                    chip->keys[0x1] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x1;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_2:
                    chip->keys[0x2] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x2;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_3:
                    chip->keys[0x3] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x3;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_4:
                    chip->keys[0xC] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xC;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;

                case SDLK_q:
                    chip->keys[0x4] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x4;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_w:
                    chip->keys[0x5] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x5;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_e: 
                    chip->keys[0x6] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x6;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_r:
                    chip->keys[0xD] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xD;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;

                case SDLK_a:
                    chip->keys[0x7] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x7;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_s:
                    chip->keys[0x8] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x8;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_d:
                    chip->keys[0x9] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x9;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_f:
                    chip->keys[0xE] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xE;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;

                case SDLK_y:
                    chip->keys[0xA] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xA;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_x:
                    chip->keys[0x0] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0x0;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_c:
                    chip->keys[0xB] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xB;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
                case SDLK_v:
                    chip->keys[0xF] = pressed;
                    if (pressed && chip->waiting_for_key) {
                        chip->V[chip->wait_reg] = 0xF;
                        chip->waiting_for_key = 0;
                        chip->pc += 2;
                    }
                    break;
            }
        }
    }
}
