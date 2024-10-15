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
    
    auto duration = std::chrono::duration_cast<seconds>(end_time - start_time);
    unsigned long mem_size = read.get_mem_size() + write.get_mem_size();
    return {duration, mem_size};
}