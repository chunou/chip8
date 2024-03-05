#ifndef CHIP8_H__
#define CHIP8_H__

#include <stdint.h>

#define RAM_SIZE 4096
#define PROGRAM_START 0x200

#define STACK_SIZE 16

#define FONTSET_SIZE 80
#define FONTSET_OFFSET 0x50

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

typedef struct {
    uint8_t memory[RAM_SIZE];
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    uint16_t stack[STACK_SIZE];

} Chip8;

void init_chip8(Chip8 *chip8);


#endif // CHIP8_H__
