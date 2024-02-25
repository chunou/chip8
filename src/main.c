#include <stdio.h>

#include "chip8.h"

int main(int argc, char *argv[]) {
    struct Chip8 chip8;
    init_chip8(&chip8);
    printf("Memory address %x has value %d\n", 0, chip8.memory[0]);
    printf("hello world\n");
    return 0;
}
