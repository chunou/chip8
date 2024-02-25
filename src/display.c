#include <stdio.h>

#include <SDL.h>

#include "display.h"

void init_display(Display *display) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
    display->window = SDL_CreateWindow("Test", 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
    if (display->window == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }
    display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
    if (display->renderer == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}