#include <stdio.h>

#include "chip8.h"

void init_chip8(struct Chip8 *chip8) {
    printf("Running init_chip8\n");
    chip8->memory[0] = 1;
}