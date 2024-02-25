#include <stdbool.h>
#include <stdio.h>


#include <SDL.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char *argv[]) {

    Display d;
    init_display(&d);
    SDL_SetRenderDrawColor(d.renderer, 0, 0, 0, 255);
    SDL_RenderClear(d.renderer);
    SDL_RenderPresent(d.renderer);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
        }
    }
    SDL_DestroyRenderer(d.renderer);
    SDL_DestroyWindow(d.window);
    SDL_Quit();

    return (0);
}
