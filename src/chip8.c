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
    /*
    * Zeros out
    *     ram
    *     stack
    *     stack pointer
    *     screen
    *     delay and sound timers
    *     variable registers
    *     memory pointer
    * 
    * Sets program counter to PROGRAM_START (0x200)
    */
    for (size_t i=0; i<RAM_SIZE; ++i) {
        chip8->ram[i] = 0;
    }
    for (size_t i=0; i<STACK_SIZE; ++i) {
        chip8->stack[i] = 0;
    }
    chip8->stack_ptr = 0;
    for (size_t i=0; i<V_REG_SIZE; ++i) {
        chip8->V[i] = 0;
    }
}

size_t load_rom(char *rom_path, Chip8 *chip8) {
    // Loads a chip8 rom into memory
    // If the file path doesn't exist, exit
    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL) {
        printf("Could not open file %s in load_rom\n", rom_path);
        exit(1);
    }
    size_t program_size = fread(
        chip8->ram + 0x200,
        sizeof(chip8->ram[0]),
        sizeof(chip8->ram) - 0x200,
        fptr);
    printf("Read rom %s of size %lu bytes\n", rom_path, program_size);
    fclose(fptr);
    return program_size;

}

// void init_chip8(Chip8 *chip8)
// {
//     for (size_t i = 0; i < RAM_SIZE; ++i)
//     {
//         chip8->ram[i] = 0;
//     }
//     clear_screen(chip8);
//     for (size_t i = 0; i < STACK_SIZE; ++i)
//     {
//         chip8->stack[i] = 0;
//     }
//     chip8->delay_timer = 0;
//     chip8->sound_timer = 0;
//     for (size_t i = 0; i < VARIABLE_REGISTER_SIZE; ++i)
//     {
//         chip8->V[i] = 0;
//     }
//     chip8->I = 0;
//     chip8->pc = 0;

//     // Load font
//     for (size_t i = 0; i < FONTSET_SIZE; ++i)
//     {
//         chip8->ram[i + FONTSET_OFFSET] = FONT[i];
//     }
// }

int clear_screen(Chip8 *chip8)
{
    for (size_t i = 0; i < SCREEN_WIDTH; ++i)
    {
        for (size_t j = 0; j < SCREEN_HEIGHT; ++j)
        {
            chip8->screen[i][j] = 0;
        }
    }
    return 0;
}

uint16_t fetch_instruction(Chip8 *chip8)
{
    if (chip8->pc >= RAM_SIZE - 1)
    {
        exit(1);
    }
    uint8_t lower_byte = chip8->ram[chip8->pc++];
    uint8_t upper_byte = chip8->ram[chip8->pc++];
    uint16_t instruction = (((uint16_t)lower_byte) << 8) | upper_byte;
    return instruction;
}

void draw_to_display(Chip8 *chip8, uint8_t vx, uint8_t vy, uint8_t n)
{
    uint8_t xloc = chip8->V[vx] % SCREEN_WIDTH;
    uint8_t yloc = chip8->V[vy] % SCREEN_HEIGHT;
    chip8->V[0xF] = 0;
    for (int row = 0; row < n; ++row)
    {
        if (row + yloc >= SCREEN_HEIGHT)
            break;
        uint8_t sprite_row = chip8->ram[chip8->I + row];
        uint8_t mask = 0x80;
        for (int col = 0; col < 8; ++col)
        {
            if (col + xloc >= SCREEN_WIDTH)
                break;
            uint8_t flag = (sprite_row & mask) >> (7 - col);
            if (chip8->screen[xloc + col][yloc + row] == 1 && flag == 1)
            {
                chip8->screen[xloc + col][yloc + row] = 0;
                chip8->V[0xF] = 1;
            }
            else
            {
                chip8->screen[xloc + col][yloc + row] = flag;
            }
            mask >>= 1;
            // vx++;
        }
    }
};

void skip_if_eq(Chip8 *chip8, uint8_t x, uint8_t n)
{
    if (chip8->V[x] == n)
    {
        chip8->pc += 2;
    }
}

void skip_if_neq(Chip8 *chip8, uint8_t x, uint8_t n)
{
    if (chip8->V[x] != n)
    {
        chip8->pc += 2;
    }
}

void skip_if_xy_eq(Chip8 *chip8, uint8_t x, uint8_t y)
{
    if (chip8->V[x] == chip8->V[y])
    {
        chip8->pc += 2;
    }
}

void skip_if_xy_neq(Chip8 *chip8, uint8_t x, uint8_t y)
{
    if (chip8->V[x] != chip8->V[y])
    {
        chip8->pc += 2;
    }
}

void return_subrtn(Chip8 *chip8)
{
    chip8->pc = chip8->stack[chip8->stack_ptr];
    chip8->stack_ptr--;
}

void call_subrtn(Chip8 *chip8, uint16_t subrtn_addr)
{
    chip8->stack_ptr++;
    chip8->stack[chip8->stack_ptr] = chip8->pc;
    chip8->pc = subrtn_addr;
}

void set_xy(Chip8 *chip8, uint8_t x, uint8_t y)
{
    chip8->V[x] = chip8->V[y];
}

void xy_or(Chip8 *chip8, uint8_t x, uint8_t y)
{
    chip8->V[x] |= chip8->V[y];
}

void xy_and(Chip8 *chip8, uint8_t x, uint8_t y)
{
    chip8->V[x] &= chip8->V[y];
}

void xy_xor(Chip8 *chip8, uint8_t x, uint8_t y)
{
    chip8->V[x] ^= chip8->V[y];
}

void xy_add(Chip8 *chip8, uint8_t x, uint8_t y)
{
    uint8_t vx = chip8->V[x];
    uint8_t vy = chip8->V[y];
    if (0xFF - vy < vx)
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] += chip8->V[y];
}

void xy_x_subtract_y(Chip8 *chip8, uint8_t x, uint8_t y)
{
    uint8_t vx = chip8->V[x];
    uint8_t vy = chip8->V[y];
    if (vx > vy)
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] = vx - vy;
}

void xy_y_subtract_x(Chip8 *chip8, uint8_t x, uint8_t y)
{
    uint8_t vx = chip8->V[x];
    uint8_t vy = chip8->V[y];
    if (vy > vx)
    {
        chip8->V[0xF] = 1;
    }
    else
    {
        chip8->V[0xF] = 0;
    }
    chip8->V[x] = vy - vx;
}

void xy_shift_left(Chip8 *chip8, uint8_t x, uint8_t y)
{
    // TODO: set a toggle for this function
    // chip8->V[x] = chip8->V[y];
    chip8->V[0xF] = (chip8->V[x] & 0x80) ? 1 : 0;
    chip8->V[x] <<= 1;
}

void xy_shift_right(Chip8 *chip8, uint8_t x, uint8_t y)
{
    // TODO: set a toggle for this function
    // chip8->V[x] = chip8->V[y];
    chip8->V[0xF] = (chip8->V[x] & 0x01) ? 1 : 0;
    chip8->V[x] >>= 1;
}

// FX55
// TODO: This instruction is ambiguous. Maybe add in flag to choose the behavior?
void write_registers_to_memory(Chip8 *chip8, uint8_t x)
{
    for (size_t i = 0; i <= x; ++i)
    {
        chip8->ram[chip8->I + i] = chip8->V[i];
    }
}

void write_memory_to_registers(Chip8 *chip8, uint8_t x)
{
    for (size_t i = 0; i <= x; ++i)
    {
        chip8->V[i] = chip8->ram[chip8->I + i];
    }
}
