#ifndef CHIP8_H__
#define CHIP8_H__

#include <stdint.h>

struct Chip8 {
    uint8_t memory[4096];
};

void init_chip8(struct Chip8 *chip8);


#endif // CHIP8_H__
