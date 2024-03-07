SDL_CFLAGS = $(shell sdl2-config --cflags) 
SDL_LIBS = $(shell sdl2-config --libs)
chip8: src/main.c src/chip8.c src/display.c
	cc -o chip8 src/main.c src/chip8.c src/display.c -Isrc $(SDL_CFLAGS) $(SDL_LIBS)

.PHONY: clean
clean:
	rm chip8
