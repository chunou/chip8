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

#define VARIABLE_REGISTER_SIZE 16

typedef struct {
    uint8_t memory[RAM_SIZE];
    uint16_t index_register;
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    uint16_t stack[STACK_SIZE];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t variable_register[VARIABLE_REGISTER_SIZE];
    uint16_t I;

} Chip8;

void init_chip8(Chip8 *chip8);
int clear_screen(Chip8 *chip8);
void draw_to_display(Chip8 *chip8, uint8_t vx, uint8_t vy, uint8_t n);


#endif // CHIP8_H__
