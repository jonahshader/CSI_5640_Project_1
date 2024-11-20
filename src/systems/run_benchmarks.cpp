// Framework for executing and comparing different cellular automata implementations
// (GPU and CPU). Manages multiple benchmark runs with randomized initial states,
// collects performance metrics (timing, memory usage), and validates consistency
// of results across implementations.
#include "run_benchmarks.h"

#include <iostream>
#include <vector>
#include <random>
#include <memory>

#include "benchmark.h"
#include "types.h"


ParameterBenchmarkSet run_benchmarks(BenchmarkParams params) {
  // extract params
  auto width_height = params.width_height;
  auto num_jobs = params.num_jobs;
  auto iterations = params.iterations;
  auto seed = params.seed;

  // create random generator with constant seed
  std::mt19937 gen(seed);

  // create the jobs
  std::cout << "Initializing jobs..." << std::endl;
  std::vector<Job> jobs;
  // std::vector<std::vector<JobResult>> job_results;
  std::vector<BenchmarkResult> benchmark_results;
  for (int i = 0; i < num_jobs; ++i) {
    // each job gets a random initial state with the specified dimensions
    ca::World initial_state(width_height, width_height, gen);
    jobs.emplace_back(
        initial_state, iterations,
        "Randomlized world. TODO: string interpolate in the WIDTH, HEIGHT, iterations...");
  }

  // create instances of the benchmarks
  std::cout << "Initializing benchmarks..." << std::endl;
  std::vector<std::unique_ptr<Benchmark>> benchmarks;
  benchmarks.push_back(std::make_unique<GPUNaive>());
  benchmarks.push_back(std::make_unique<CPUNaive>());

  // create benchmark results
  for (auto& benchmark : benchmarks) {
    BenchmarkResult r(std::vector<JobResult>(), benchmark->get_description());
    benchmark_results.push_back(r);
  }

  // run the benchmarks
  std::cout << "Running benchmarks..." << std::endl;
  // for each benchmark,
  for (int i = 0; i < benchmarks.size(); ++i) {
    // grab the current benchmark and results instance
    auto &benchmark = *benchmarks[i];
    auto &results = benchmark_results[i].results;

    // for each job,
    for (int j = 0; j < jobs.size(); ++j) {
      // grab the current job
      auto &job = jobs[j];
      std::cout << "Running job " << (j + 1) << " of " << jobs.size() << " for benchmark " << (i + 1)
                << " of " << benchmarks.size() << std::endl;
      // run the job and push back the result
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

  // validate results match across benchmarks
  std::cout << "Validating results..." << std::endl;
  bool results_match = true;
  // iterate over benchmarks
  for (int i = 1; i < benchmarks.size(); ++i) {
    // grab neighboring benchmarks
    auto &results1 = benchmark_results[i - 1].results;
    auto &results2 = benchmark_results[i].results;
    // iterate through the results
    for (int result_index = 0; result_index < results1.size(); ++result_index) {
      // compare neighboring results.
      // these are two results from two benchmarks, but
      // from the same job, so we expect the exact same result.
      auto &result1 = results1[result_index].final_state;
      auto &result2 = results2[result_index].final_state;
      // compare the results (using custom (!=) operator)
      if (result1 != result2) {
        std::cerr << "Results for job " << (result_index + 1) << " do not match across benchmarks!" << std::endl;
        results_match = false;
      }
    }
  }

  if (results_match) {
    std::cout << "All results match across benchmarks!" << std::endl;
  }

  // TODO: std::move?
  ParameterBenchmarkSet bset(params, benchmark_results);
  return bset;
}
