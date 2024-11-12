// High-level benchmarking functions
#pragma once

#include <vector>

#include "benchmark.h"



// default parameters
constexpr int WIDTH_HEIGHT = 1 << 10;
constexpr int NUM_JOBS = 1 << 3;
constexpr int ITERATIONS = 1 << 9;
constexpr unsigned long SEED = 0;

struct BenchmarkParams {
    int width_height{WIDTH_HEIGHT};
    int num_jobs{NUM_JOBS};
    int iterations{ITERATIONS};
    unsigned long seed{SEED};

    std::string to_json() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"width_height\": " << width_height << ",";
        ss << "\"num_jobs\": " << num_jobs << ",";
        ss << "\"iterations\": " << iterations << ",";
        ss << "\"seed\": " << seed;
        ss << "}";
        return ss.str();
    }
};

// Runs all benchmarks
std::vector<BenchmarkResult> run_benchmarks(BenchmarkParams params);
