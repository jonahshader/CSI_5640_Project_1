#define SDL_MAIN_HANDLED
#include <iostream>

#include <SDL.h>
#include <random>

#include "systems/types.h"
#include "systems/render_state.h"
#include "systems/update_state.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int CELL_SIZE = 4;

int main(int argc, char *argv[])
{
    // test render_state

    // initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Cellular Automaton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // initalize world with random state
    std::random_device rd;
    std::mt19937 gen(rd());
    ca::World world_write(SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE, gen);

    // define second buffer, copy to maintain width, height, state size
    ca::World world_read = world_write;

    // setup the render loop
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // render the state
        ca::render_state(world_write, renderer, window);

        // swap the world buffers
        std::swap(world_read.state, world_write.state);

        // update the state
        ca::update_state(world_read, world_write);

        // present the renderer
        SDL_RenderPresent(renderer);
    }
    return 0;
}