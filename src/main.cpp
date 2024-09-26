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

    // initialize the world
    ca::World world_write, world_read;
    world_write.width = SCREEN_WIDTH / CELL_SIZE;
    world_write.height = SCREEN_HEIGHT / CELL_SIZE;
    world_write.state = new ca::cell_t[world_write.width * world_write.height];
    // copy the world
    world_read = world_write; 
    // give the world its own memory for state
    world_read.state = new ca::cell_t[world_read.width * world_read.height];
    

    // randomize the state
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 1);
    for (int i = 0; i < world_write.width * world_write.height; i++)
    {
        world_write.state[i] = dis(gen);
    }

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