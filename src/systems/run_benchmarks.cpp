#include "run_benchmarks.h"

#include <iostream>
#include <vector>
#include <random>
#include <memory>

#include "benchmarks.h"

constexpr int WIDTH = 1<<8;
constexpr int HEIGHT = 1<<8;
constexpr int NUM_JOBS = 1<<5;
constexpr unsigned long SEED = 0;

void run_benchmarks()
{
    // create random generator with constant seed
    std::mt19937 gen(SEED);

    // create the jobs
    std::vector<Job> jobs;
    for (int i = 0; i < NUM_JOBS; ++i)
    {
        jobs.emplace_back(WIDTH, HEIGHT, gen);
    }

    // create the benchmarks
    // TODO: verify push_back use
    std::vector<std::unique_ptr<Benchmark>> benchmarks;
    benchmark.push_back(std::make_unique<CPUNaive>());
    benchmark.push_back(std::make_unique<CPUOptimized());
    benchmark.push_back(std::make_unique<GPUNaive>());
    benchmark.push_back(std::make_unique<GPUOptimized());

    // TODO: verify unique_ptr use
    for (int i = 0; i < benchmarks.size(); ++i) {
        auto benchmark = benchmarks[i];
        std::vector<BenchmarkResult> results;

        for (int j = 0; j < jobs.size(); ++j) {
            auto& job = jobs[j];
            results.push_back(benchmark.run(job));
        }

        // print results
        // for now, just print the results as-is.
        // TODO: compute mean and std. maybe export to csv to be processed by a python script
        // maybe we use matplotlib to generate some histograms later...
        std::cout << "Benchmark " << (i+1) << ": " << job.get_description() << std::endl;
        for (int j = 0; j < jobs.size(); ++j) {
            auto& job = jobs[i];
            std::cout << "Job " << (j+1) << " Duration: " << job.duration << " seconds, Memory: " << job.memory_required << " bytes." << std::endl;
        }
        std::cout << std::endl; // additional newline for clarity
    }
}