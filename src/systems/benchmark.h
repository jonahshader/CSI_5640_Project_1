// benchmark.h contains the definitions of some structs and classes used for describing benchmarks
// generically, so that multiple benchmarks can share the same interface.
#pragma once

#include <chrono>
#include <vector>
#include <string>

#include "types.h"

// A Job describes the work that is to be done by a Benchmark.
// It is passed into the benchmark's run method.
struct Job {
  ca::World initial_state;
  int iterations;
  std::string description;
};

// After a job is ran through a benchmark, a JobResult is returned
// which contains some useful metrics and the final state.
struct JobResult {
  double duration{0};
  unsigned long memory_required{0};
  ca::World final_state;
};

// The results for multiple runs from a benchmark 
struct BenchmarkResult {
  std::vector<JobResult> results{};
  std::string description;
}

// Interface describing a benchmark. Implemented by CPUNaive and GPUNaive
class Benchmark {
public:
  virtual ~Benchmark() {};

  // Execute the benchmark on the specified job, returning results
  // as a JobResult instance.
  virtual JobResult run(const Job &job) = 0;
  // Returns a description of this benchmark.
  virtual std::string get_description() = 0;
};

// CPU implementation of Conway's Game of Life on a fixed-size grid
class CPUNaive : public Benchmark {
public:
  JobResult run(const Job &job) override;
  std::string get_description() override;
};
// GPU implementation of Conway's Game of Life on a fixed-size grid (using openacc)
class GPUNaive : public Benchmark {
public:
  JobResult run(const Job &job) override;
  std::string get_description() override;
};
