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
    // TODO:
    BenchmarkResult result(0.0, 0);
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