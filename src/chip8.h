#ifndef CHIP8_H__
#define CHIP8_H__

#include <stdint.h>

#define RAM_SIZE 4096

#define FONTSET_SIZE 80
#define FONTSET_OFFSET 0x50

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct Chip8 {
    uint8_t memory[RAM_SIZE];
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
};

void init_chip8(struct Chip8 *chip8);


#endif // CHIP8_H__
