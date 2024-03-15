SDL_CFLAGS = $(shell sdl2-config --cflags) 
SDL_LIBS = $(shell sdl2-config --libs)
chip8: src/main.c src/chip8.h src/chip8.c src/display.c
	cc -o chip8 src/main.c src/chip8.c src/display.c -Isrc $(SDL_CFLAGS) $(SDL_LIBS)

tests: src/tests.c src/chip8.h src/chip8.c
	cc -O0 -o tests src/tests.c src/chip8.c -Isrc

.PHONY: clean
clean:
	rm -f chip8* tests*
