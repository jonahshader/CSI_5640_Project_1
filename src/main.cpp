// Main entry point of the program.
// We perform the parameter sweeps across the width_height and iterations parameters here,
// and write the results to JSON files.

// Alternatively, if VIS_SDL2 is defined, the program will run a simple SDL2 preview of the cellular automaton.
// This requires building with cmake instead of make, as SDL2 is not included in the makefile.

// #define VIS_SDL2

#define SDL_MAIN_HANDLED
#include <iostream>

// #include <SDL.h>
#include <openacc.h>
#include <random>
#include <vector>

#include "systems/types.h"
// only include rendering stuff if VIS_SDL2 is defined
#ifdef VIS_SDL2
#include "systems/render_state.h"
#include <SDL.h>
#endif

#include "systems/update_state.h"
#include "systems/run_benchmarks.h"
#include "systems/json.h"

// only define visualizations if VIS_SDL2 is defined
#ifdef VIS_SDL2

// define some constants for the SDL2 preview
constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int CELL_SIZE = 4;

// this function will run a simple SDL2 preview of the cellular automaton.
// only used for initial debugging and visualization. this is not used in the benchmarking.
void preview() {
  // test render_state

  // initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("Cellular Automaton", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
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
}

#endif

void sweep_params() {
  if (acc_get_device_type() != acc_device_nvidia) {
    std::cerr << "No GPU device found" << std::endl;
  }

  std::vector<ParameterBenchmarkSet> benchmark_sets;

  // Explore width_height parameter
  std::cout << "Exploring various world sizes..." << std::endl;
  // try 32 different sizes
  for (int i = 0; i < 32; ++i) {
    // set up the parameters for the benchmark
    BenchmarkParams params;
    params.iterations >>= 3;  // shorten iterations because we will do multiple runs
    params.width_height = (i + 1) * 128;  // step through the width_height param
    params.num_jobs = 1 << 2;
    // run the benchmarks for this parameter set
    benchmark_sets.push_back(run_benchmarks(params));
  }

  // Create and write the width_height sweep
  ParameterSweep("width_height", benchmark_sets).write_to_json("change_width_height.json");

  // Clear for next sweep
  benchmark_sets.clear();

  // Explore iterations parameter
  std::cout << "Exploring various iterations..." << std::endl;
  // try 32 different sizes (increment by 2)
  for (int i = 2; i <= 64; i += 2) {
    // set up the parameters for the benchmark
    BenchmarkParams params;
    params.iterations = i;
    params.num_jobs = 1 << 2;
    // run the benchmarks for this parameter set
    benchmark_sets.push_back(run_benchmarks(params));
  }

  // Create and write the iterations sweep
  ParameterSweep("iterations", benchmark_sets).write_to_json("change_iters.json");
}

int main(int argc, char *argv[]) {
  // if VIS_SDL2 is defined, run the preview function
  // otherwise, run the parameter sweep function
  #ifdef VIS_SDL2
  preview();
  #else
  sweep_params();
  #endif
  return 0;
}
