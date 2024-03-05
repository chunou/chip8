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

    Display d;
    init_display(&d);
    SDL_SetRenderDrawColor(d.renderer, 0, 0, 0, 255);
    SDL_RenderClear(d.renderer);
    SDL_RenderPresent(d.renderer);
    bool quit = false;
    SDL_Event e;

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000000 / 60;
    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }
        if (chip8.delay_timer > 0 || chip8.sound_timer > 0) {
            chip8.delay_timer = chip8.delay_timer ? chip8.delay_timer - 1 : 0;
            chip8.sound_timer = chip8.sound_timer ? chip8.sound_timer - 1 : 0;
            printf("Delay timer: %d\n", chip8.delay_timer);
            printf("Sound timer: %d\n", chip8.sound_timer);
            nanosleep(&ts, NULL);
        }
    }
    SDL_DestroyRenderer(d.renderer);
    SDL_DestroyWindow(d.window);
    SDL_Quit();

    return (0);
}
