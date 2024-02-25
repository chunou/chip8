#include <stdio.h>

#include "chip8.h"

int main(int argc, char *argv[]) {
    struct Chip8 chip8;
    init_chip8(&chip8);
    for (size_t i=0; i<FONTSET_SIZE; ++i) {
        printf("%X", chip8.memory[FONT_START+i]);
        if ((i+1)%5) printf(",");
        else printf("\n");
    }
    return 0;
}
