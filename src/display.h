#ifndef DISPLAY_H__
#define DISPLAY_H__

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 320

#include <SDL.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Window *window;
} Display;

void init_display(Display *display);

void compose_scene(Display *display, uint8_t *screen, int rows, int cols);

#endif // DISPLAY_H__
