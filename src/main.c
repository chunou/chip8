#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL.h>

#include "chip8.h"
#include "display.h"

int main(int argc, char *argv[]) {

  Chip8 chip8;
  init_chip8(&chip8);
  chip8.pc = PROGRAM_START;
  Display display;
  init_display(&display);
  bool quit = false;
  SDL_Event e;

  FILE *fptr;
  fptr = fopen("roms/3-corax+.ch8", "rd");
  while (!feof(fptr)) {
    size_t r =
        fread(&chip8.memory[PROGRAM_START], sizeof(chip8.memory), 1, fptr);
  }
  fclose(fptr);

  // Enter the main loop of the emulator
  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = true;
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
    printf("Found instruction %04X\n", instruction);

    // Decode & execute
    switch (nibble0) {
    case 0x0:
      if (instruction == 0x00E0) {
        printf("Clearing screen\n");
        clear_screen(&chip8);
      } else if (instruction == 0x00EE) {
        return_subrtn(&chip8);
      }
      break;

    case 0x1: {
      uint16_t loc = instruction & 0b0000111111111111;
      chip8.pc = loc;
      printf("Jumping to location: %04X\n", loc);
    } break;

    case 0x2: {
      uint16_t mask = 0x0FFF;
      uint16_t addr = instruction & mask;
      call_subrtn(&chip8, addr);
    } break;

    case 0x3: {
      uint8_t val = (nibble2 << 4) | nibble3;
      skip_if_eq(&chip8, nibble1, val);

    } break;

    case 0x4: {
      uint8_t val = (nibble2 << 4) | nibble3;
      skip_if_neq(&chip8, nibble1, val);
    } break;

    case 0x5: {
      skip_if_xy_eq(&chip8, nibble1, nibble2);
    } break;

    case 0x6: {
      uint8_t register_index = nibble1;
      uint8_t register_value = (nibble2 << 4) | nibble3;
      printf("Setting register V%X to %02X\n", register_index, register_value);
      chip8.V[register_index] = register_value;
    } break;

    case 0x7: {
      // This instruction does not care about overflow
      uint8_t register_index = nibble1;
      uint8_t value = (nibble2 << 4) | nibble3;
      chip8.V[register_index] += value;
    } break;

    case 0x8:
      switch (nibble3) {
      case 0x0:
        set_xy(&chip8, nibble1, nibble2);
        break;
      case 0x1:
        xy_or(&chip8, nibble1, nibble2);
        break;
      case 0x2:
        xy_and(&chip8, nibble1, nibble2);
        break;
      case 0x3:
        xy_xor(&chip8, nibble1, nibble2);
        break;
      case 0x4:
        xy_add(&chip8, nibble1, nibble2);
        break;
      case 0x5:
        xy_x_subtract_y(&chip8, nibble1, nibble2);
        break;
      case 0x6:
        xy_shift_right(&chip8, nibble1, nibble2);
        break;
      case 0x7:
        xy_y_subtract_x(&chip8, nibble1, nibble2);
        break;
      case 0xE:
        xy_shift_left(&chip8, nibble1, nibble2);
        break;
      default:
        break;
      }
      break;

    case 0x9: {
      skip_if_xy_neq(&chip8, nibble1, nibble2);
    } break;

    case 0xA: {
      uint16_t index = instruction & 0x0FFF;
      printf("setting index to %03X\n", index);
      chip8.I = index;
    } break;

    case 0xD:
      printf("Calling `draw_to_display`\n");
      printf("Register V%X = %02X, Register V%X = %02X, Height = %d\n\n",
             nibble1, chip8.V[nibble1], nibble2, chip8.V[nibble2], nibble3);
      draw_to_display(&chip8, nibble1, nibble2, nibble3);
      break;

    default:
      printf("Fallthrough on instruction %04X\n", instruction);
      exit(1);
      break;
    }
    // Compose Scene
    SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 0);
    compose_scene(&display, &chip8.screen[0][0], SCREEN_WIDTH, SCREEN_HEIGHT);

    // Present Renderer
    // Mabye we only do this if the screen has changed?
    SDL_RenderPresent(display.renderer);

    // Maybe wait here?
    usleep(1000000 / 10);
  }

  // Cleanup
  SDL_DestroyRenderer(display.renderer);
  SDL_DestroyWindow(display.window);
  SDL_Quit();

  return (0);
}
