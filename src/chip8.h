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

typedef struct
{
    uint8_t memory[RAM_SIZE];
    uint8_t screen[SCREEN_WIDTH][SCREEN_HEIGHT];
    uint16_t stack[STACK_SIZE];
    uint8_t stack_ptr;
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t V[VARIABLE_REGISTER_SIZE];
    uint16_t pc;
    uint16_t I;

} Chip8;

void init_chip8(Chip8 *chip8);
int clear_screen(Chip8 *chip8);
uint16_t fetch_instruction(Chip8 *chip8);
void draw_to_display(Chip8 *chip8, uint8_t vx, uint8_t vy, uint8_t n);

void skip_if_eq(Chip8 *chip8, uint8_t x, uint8_t n);
void skip_if_neq(Chip8 *chip8, uint8_t x, uint8_t n);
void skip_if_xy_eq(Chip8 *chip8, uint8_t x, uint8_t y);
void skip_if_xy_neq(Chip8 *chip8, uint8_t x, uint8_t y);

void return_subrtn(Chip8 *chip8);
void call_subrtn(Chip8 *chip8, uint16_t subrtn_addr);

// 8XY0
void set_xy(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY1
void xy_or(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY2
void xy_and(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY3
void xy_xor(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY4
void xy_add(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY5
void xy_x_subtract_y(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY6
void xy_shift_right(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XY7
void xy_y_subtract_x(Chip8 *chip8, uint8_t x, uint8_t y);
// 8XYE
void xy_shift_left(Chip8 *chip8, uint8_t x, uint8_t y);

// FX55
void write_registers_to_memory(Chip8 *chip8, uint8_t x);
// FX65
void write_memory_to_registers(Chip8 *chip8, uint8_t x);


#endif // CHIP8_H__
