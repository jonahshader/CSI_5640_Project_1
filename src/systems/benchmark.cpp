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

  BenchmarkResult result(duration.count(), mem_size);
  return result;
}

std::string CPUNaive::get_description() {
  return "Fixed-size world running on CPU";
}

BenchmarkResult GPUNaive::run(const Job &job) {
  ca::cell_t *read_cells =
      (ca::cell_t *)malloc(job.initial_state.state.size() * sizeof(ca::cell_t));
  ca::cell_t *write_cells =
      (ca::cell_t *)malloc(job.initial_state.state.size() * sizeof(ca::cell_t));
  int width = job.initial_state.width;
  int height = job.initial_state.height;

  // copy initial state
  std::copy(job.initial_state.state.begin(), job.initial_state.state.end(), read_cells);

  auto start_time = std::chrono::high_resolution_clock::now();

  // Track which buffer has the final result
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

          // Unroll the neighbor loop manually for better GPU performance
          // Top row
          alive_neighbors +=
              read_cells[((y - 1 + height) % height) * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[((y - 1 + height) % height) * width + x];
          alive_neighbors += read_cells[((y - 1 + height) % height) * width + ((x + 1) % width)];

          // Middle row
          alive_neighbors += read_cells[y * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[y * width + ((x + 1) % width)];

          // Bottom row
          alive_neighbors += read_cells[((y + 1) % height) * width + ((x - 1 + width) % width)];
          alive_neighbors += read_cells[((y + 1) % height) * width + x];
          alive_neighbors += read_cells[((y + 1) % height) * width + ((x + 1) % width)];

          // Apply rules
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

  // Count live cells in final state (using result_cells pointer)
  int final_alive = 0;
  for (int i = 0; i < width * height; i++) {
    final_alive += result_cells[i];
  }

  // Calculate memory usage: two cell arrays of width*height size
  size_t memory_usage = 2 * width * height * sizeof(ca::cell_t);

  // Clean up the buffer that doesn't have the result
  ca::cell_t *other_cells = (result_cells == read_cells) ? write_cells : read_cells;
  free(other_cells);

  // Return the result and free its memory
  BenchmarkResult result(duration.count(), memory_usage);
  free(result_cells);

  return result;
}

std::string GPUNaive::get_description() {
  return "Fixed-size world running on GPU";
}

BenchmarkResult CPUOptimized::run(const Job &job) {
  // TODO:
  BenchmarkResult result(0.0, 0);
  return result;
}

std::string CPUOptimized::get_description() {
  return "Expanding, sparse world running on CPU";
}

BenchmarkResult GPUOptimized::run(const Job &job) {
  // TODO:
  BenchmarkResult result(0.0, 0);
  return result;
}

std::string GPUOptimized::get_description() {
  return "Expanding, sparse world running on GPU";
}
