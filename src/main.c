#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <SDL.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char *argv[]) {

    Chip8 chip8;
    init_chip8(&chip8);
    chip8.delay_timer = 60;
    chip8.sound_timer = 30;
    chip8.memory[0] = 0xD0;
    chip8.memory[1] = 0x05;
    chip8.index_register = 0;
    chip8.I = 0;
    chip8.I = FONTSET_OFFSET+35;

    Display display;
    init_display(&display);;
    bool quit = false;
    SDL_Event e;
    
    draw_to_display(&chip8, 0, 0, 5);
    // Enter the main loop of the emulator
    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }
        // Clear Renderer
        SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 255);
        SDL_RenderClear(display.renderer);

        // Do chip8 stuff here
        uint16_t nibble_mask = 0b1111;
        uint16_t instruction = fetch_instruction(&chip8);
        uint8_t nibble0 = (instruction & (nibble_mask << 12)) >> 12;
        uint8_t nibble1 = (instruction & (nibble_mask << 8)) >> 8;
        uint8_t nibble2 = (instruction & (nibble_mask << 4)) >> 4;
        uint8_t nibble3 = (instruction & (nibble_mask));
        printf("n0: %X, n1: %X, n2: %X, n3: %X\n", nibble0, nibble1, nibble2, nibble3);

        // Decode
        switch (nibble0) {
        case 0x0:
            if (instruction == 0x00E0) {
                clear_screen(&chip8);
            }
            break;
        
        case 0xD:
            draw_to_display(&chip8, nibble1, nibble2, nibble3);
            break;
        
        default:
            printf("Fallthrough on instruction %04X\n", instruction);
            break;
        }
            // Fetch instruction
            // Decode instruction
            // Execute instruction

        // Compose Scene
        SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 0);
        compose_scene(&display, &chip8.screen[0][0], SCREEN_WIDTH, SCREEN_HEIGHT);

        // Present Renderer
        // Mabye we only do this if the screen has changed?
        SDL_RenderPresent(display.renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();

    return (0);
}
