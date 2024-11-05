#include "run_benchmarks.h"

#include <iostream>
#include <vector>
#include <random>
#include <memory>

#include "benchmark.h"
#include "types.h"

constexpr int WIDTH = 1 << 10;
constexpr int HEIGHT = 1 << 10;
constexpr int NUM_JOBS = 1 << 3;
constexpr int ITERATIONS = 1 << 9;
constexpr unsigned long SEED = 0;

void run_benchmarks() {
  // create random generator with constant seed
  std::mt19937 gen(SEED);

  // create the jobs
  std::cout << "Initializing jobs..." << std::endl;
  std::vector<Job> jobs;
  std::vector<std::vector<BenchmarkResult>> job_results;
  for (int i = 0; i < NUM_JOBS; ++i) {
    ca::World initial_state(WIDTH, HEIGHT, gen);
    jobs.emplace_back(
        initial_state, ITERATIONS,
        "Randomlized world. TODO: string interpolate in the WIDTH, HEIGHT, iterations...");
    job_results.push_back(std::vector<BenchmarkResult>());
  }

  // create the benchmarks
  // TODO: verify push_back use
  std::cout << "Initializing benchmarks..." << std::endl;
  std::vector<std::unique_ptr<Benchmark>> benchmarks;
  benchmarks.push_back(std::make_unique<GPUNaive>());
  // benchmarks.push_back(std::make_unique<GPUOptimized>());
  benchmarks.push_back(std::make_unique<CPUNaive>());
  // benchmarks.push_back(std::make_unique<CPUOptimized>());


  // TODO: verify unique_ptr use
  // run the benchmarks
  std::cout << "Running benchmarks..." << std::endl;
  for (int i = 0; i < benchmarks.size(); ++i) {
    auto &benchmark = *benchmarks[i];
    auto &results = job_results[i];

    for (int j = 0; j < jobs.size(); ++j) {
      auto &job = jobs[j];
      std::cout << "Running job " << (j + 1) << " of " << jobs.size() << " for benchmark " << (i + 1)
                << " of " << benchmarks.size() << std::endl;
      results.push_back(benchmark.run(job));
    }

    // print results
    // for now, just print the results as-is.
    // TODO: compute mean and std. maybe export to csv to be processed by a python script
    // maybe we use matplotlib to generate some histograms later...
    std::cout << "Benchmark " << (i + 1) << ": " << benchmark.get_description() << std::endl;
    for (int j = 0; j < jobs.size(); ++j) {
      // auto &job = jobs[j]; // TODO: maybe jobs should have descriptions?
      auto &result = results[j];
      std::cout << "Job " << (j + 1) << " Duration: " << result.duration
                << " seconds, Memory: " << result.memory_required << " bytes." << std::endl;
    }
    std::cout << std::endl; // additional newline for clarity
  }

  // validate results match across benchmarks'
  // iterate over benchmarks
  std::cout << "Validating results..." << std::endl;
  bool results_match = true;
  for (int i = 1; i < benchmarks.size(); ++i) {
    auto &results1 = job_results[i - 1];
    auto &results2 = job_results[i];
    for (int result_index = 0; result_index < results1.size(); ++result_index) {
      auto &result1 = results1[result_index].final_state;
      auto &result2 = results2[result_index].final_state;
      if (result1 != result2) {
        std::cerr << "Results for job " << (result_index + 1) << " do not match across benchmarks!" << std::endl;
        results_match = false;
      }
    }
  }

  if (results_match) {
    std::cout << "All results match across benchmarks!" << std::endl;
  }
}
