#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "chip.h"

void init_chip(Chip8 * chip) {
    memset(chip->memory, 0, sizeof(chip->memory));
    memset(chip->V, 0, sizeof(chip->V));
    chip-> I = 0x200;
    chip->pc = 0;
}
