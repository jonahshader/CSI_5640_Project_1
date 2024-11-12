#define SDL_MAIN_HANDLED
#include <iostream>

#include <SDL.h>
#include <openacc.h>
#include <random>
#include <vector>

#include "systems/types.h"
#include "systems/render_state.h"
#include "systems/update_state.h"
#include "systems/run_benchmarks.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int CELL_SIZE = 4;

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

int main(int argc, char *argv[]) {
  // At the start of your program
  if (acc_get_device_type() != acc_device_nvidia) {
      std::cerr << "No GPU device found" << std::endl;
  }
  // preview();
  // BenchmarkParams params;
  // run_benchmarks(params);


  std::vector<BenchmarkParams> params_vec;
  std::vector<BenchmarkResult> results_vec;
  constexpr int RUNS = 1 << 3;
  // explore width_height parameter
  std::cout << "Exploring various world sizes..." << std::endl;
  for (int i = 0; i < RUNS; ++i) {
    BenchmarkParams params;
    params.iterations >>= 3; // shorten iterations because we will do multiple runs
    params.width_height >>= (i-1); // step through the width_height param, powers of 2
    params.num_jobs = 1 << 2;
    results_vec.push_back(run_benchmarks(params));
    params_vec.push_back(params);
  }

  write_benchmark_json(params_vec, results_vec, "change_width_height.json");
  params_vec.clear();
  results_vec.clear();

  // explore the iterations parameter
  std::cout << "Exploring various iterations..." << std::endl;
  for (int i = 0; i < 64; ++i) {
    BenchmarkParams params;
    params.iterations = i + 1;
    params.num_jobs = 1 << 2;
    results_vec.push_back(run_benchmarks(params));
    params_vec.push_back(params);
  }

  write_benchmark_json(params_vec, results_vec, "change_iters.json");
  return 0;
}
