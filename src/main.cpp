#define SDL_MAIN_HANDLED
#include <iostream>

#include <SDL.h>
#include <random>

#include "systems/types.h"
#include "systems/render_state.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int CELL_SIZE = 4;

int main(int argc, char* argv[]) {
    // test render_state

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Cellular Automaton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // initialize the state
    const int width = SCREEN_WIDTH / CELL_SIZE;
    const int height = SCREEN_HEIGHT / CELL_SIZE;
    ca::cell state[width * height] = {0};

    // randomize the state
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 1);
    for (int i = 0; i < width * height; i++) {
        state[i] = dis(gen);
    }

    // render the state
    ca::render_state(state, width, height, renderer, window);

    // present the renderer
    SDL_RenderPresent(renderer);

    // wait for the user to close the window
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}