#ifndef DISPLAY_H__
#define DISPLAY_H__

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 320

#include <SDL.h>

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
} Display;

void init_display(Display* display);

#endif // DISPLAY_H__
