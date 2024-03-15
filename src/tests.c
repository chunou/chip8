#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "chip8.h"

void test_init_chip8() {
    Chip8 chip8;
    init_chip8(&chip8);
    printf("test_init_chip8:\n");
    printf("\tchecking ram is cleared... ");
    for (size_t i=0; i<RAM_SIZE; ++i) {
        assert(chip8.ram[i] == 0);
    }
    printf("ok\n");
}
int main(int argc, char *argv[]) {
    test_init_chip8();
    printf("All tests passed!\n");
    return 0;
}