#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

const uint8_t FONT[FONTSET_SIZE] = {
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

void init_chip8(Chip8 *chip8) {
    for (size_t i=0; i<RAM_SIZE; ++i) {
        chip8->memory[i] = 0;
    }
    chip8->index_register = 0;
    clear_screen(chip8);
    for (size_t i=0; i<STACK_SIZE; ++i) {
        chip8->stack[i] = 0;
    }
    chip8->delay_timer = 0;
    chip8->sound_timer = 0;
    for (size_t i=0; i<VARIABLE_REGISTER_SIZE; ++i) {
        chip8->variable_register[i] = 0;
    }
    chip8->I = 0;
    chip8->pc = 0;

    // Load font
    for (size_t i=0; i<FONTSET_SIZE; ++i) {
        chip8->memory[i + FONTSET_OFFSET] = FONT[i];
    }
}

int clear_screen(Chip8 *chip8) {
    for (size_t i=0; i<SCREEN_WIDTH; ++i) {
        for (size_t j=0; j<SCREEN_HEIGHT; ++j) {
            chip8->screen[i][j] = 0;
        }
    }
    return 0;
}

uint16_t fetch_instruction(Chip8 *chip8) {
    // Gets a 16 bit instruction from memory
    // Does not perform any saftey checks.
    // Instruction pointer can run off end of memory
    if (chip8->pc >= RAM_SIZE - 1) {
        exit(1);
    }
    uint8_t lower_byte = chip8->memory[chip8->pc++];
    uint8_t upper_byte = chip8->memory[chip8->pc++];
    uint16_t instruction = (((uint16_t)lower_byte)<< 8) | upper_byte;
    return instruction;
}

void draw_to_display(Chip8 *chip8, uint8_t vx, uint8_t vy, uint8_t n) {
    uint8_t x = chip8->variable_register[vx] % SCREEN_WIDTH;
    uint8_t y = chip8->variable_register[vy] % SCREEN_HEIGHT;

    for (int yy = y; yy < y + n; ++yy) {
        uint8_t byte = chip8->memory[chip8->I+yy];
        for (int xx = x; xx < x + 8; ++xx) {
            if (xx >= SCREEN_WIDTH) break;
            uint8_t bit = 0b10000000 >> (xx-x);
            uint8_t pixel = (byte & bit) >> (7-xx+x);
            if (chip8->screen[xx][yy] == pixel) {
                chip8->screen[xx][yy] ^= pixel;
                chip8->variable_register[0x0F] = 1;
            } else {
                chip8->screen[xx][yy] = pixel;
            }
        }
    }
};
