#include <stdbool.h>
#include <stdio.h>
#include <time.h>


#include <SDL.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char *argv[]) {

    Chip8 chip8;
    init_chip8(&chip8);
    chip8.delay_timer = 60;
    chip8.sound_timer = 30;
    chip8.memory[0] = 0x4F;
    chip8.memory[1] = 0xA3;
    chip8.index_register = 0;
    chip8.I = FONTSET_OFFSET+55;

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
        

        // Compose Scene
        SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 0);
        compose_scene(&display, &chip8.screen[0][0], SCREEN_WIDTH, SCREEN_HEIGHT);

        // Present Renderer
        SDL_RenderPresent(display.renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();

    return (0);
}
