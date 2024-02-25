#ifndef CHIP8_H__
#define CHIP8_H__

#include <stdint.h>

#define RAM_SIZE 4096

struct Chip8 {
    uint8_t memory[RAM_SIZE];
};

void init_chip8(struct Chip8 *chip8);


#endif // CHIP8_H__
