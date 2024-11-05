#include "benchmark.h"

#include <chrono>

#include "types.h"
#include "update_state.h"

// TODO: finish

BenchmarkResult CPUNaive::run(const Job &job) {
  // copy initial state
  ca::World read = job.initial_state;
  ca::World write = job.initial_state;
  auto start_time = std::chrono::high_resolution_clock::now();
  // run main computation
  for (int i = 0; i < job.iterations; ++i) {
    ca::update_state(read, write);
    std::swap(read.state, write.state);
  }
  auto end_time = std::chrono::high_resolution_clock::now();

  // Convert duration to double seconds
  auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);
  unsigned long mem_size = read.get_mem_size() + write.get_mem_size();

  BenchmarkResult result(duration.count(), mem_size, read);
  return result;
}

std::string CPUNaive::get_description() {
  return "Fixed-size world running on CPU";
}

BenchmarkResult GPUNaive::run(const Job &job) {
  // allocate memory (on the host) for our two cell arrays
  auto width = job.initial_state.width;
  auto height = job.initial_state.height;
  auto world_size = width * height;
  ca::cell_t *read_cells = (ca::cell_t *)malloc(world_size * sizeof(ca::cell_t));
  ca::cell_t *write_cells = (ca::cell_t *)malloc(world_size * sizeof(ca::cell_t));

  // copy initial state to the read buffer
  std::copy(job.initial_state.state.begin(), job.initial_state.state.end(), read_cells);

  // start the timer
  auto start_time = std::chrono::high_resolution_clock::now();

  // variable for tracking which buffer has the final result
  ca::cell_t *result_cells = read_cells;

#pragma acc data copy(read_cells[0 : width * height]) create(write_cells[0 : width * height])
  {
    for (int iter = 0; iter < job.iterations; iter++) {
#pragma acc parallel loop gang num_gangs(height) vector_length(32)
      for (int y = 0; y < height; y++) {
#pragma acc loop vector
        for (int x = 0; x < width; x++) {
          int alive_neighbors = 0;
          const int idx = y * width + x;

          // unrolled the neighbor loop manually for better performance
          // top row
          alive_neighbors +=
              read_cells[((y - 1 + height) % height) * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[((y - 1 + height) % height) * width + x];
          alive_neighbors += read_cells[((y - 1 + height) % height) * width + ((x + 1) % width)];

          // middle row
          alive_neighbors += read_cells[y * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[y * width + ((x + 1) % width)];

          // bottom row
          alive_neighbors += read_cells[((y + 1) % height) * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[((y + 1) % height) * width + x];
          alive_neighbors += read_cells[((y + 1) % height) * width + ((x + 1) % width)];

          // apply the rules of conway's game of life
          write_cells[idx] = (read_cells[idx]) ? (alive_neighbors == 2 || alive_neighbors == 3)
                                               : (alive_neighbors == 3);
        }
      }
      std::swap(read_cells, write_cells);
      result_cells = read_cells;
    }
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time);

  // calculate memory usage: two cell arrays of width*height size
  size_t memory_usage = 2 * width * height * sizeof(ca::cell_t);

  // copy final state into a ca::World instance
  ca::World final_state;
  final_state.width = width;
  final_state.height = height;
  final_state.state = std::vector<ca::cell_t>(result_cells, result_cells + width * height);

  // build result instance
  BenchmarkResult result(duration.count(), memory_usage, final_state);

  // free buffers
  free(read_cells);
  free(write_cells);

  return result;
}

std::string GPUNaive::get_description() {
  return "Fixed-size world running on GPU";
}

BenchmarkResult CPUOptimized::run(const Job &job) {
  // TODO:
  BenchmarkResult result(0.0, 0, ca::World());
  return result;
}

std::string CPUOptimized::get_description() {
  return "Expanding, sparse world running on CPU";
}

BenchmarkResult GPUOptimized::run(const Job &job) {
  // TODO:
  BenchmarkResult result(0.0, 0, ca::World());
  return result;
}

std::string GPUOptimized::get_description() {
  return "Expanding, sparse world running on GPU";
}
