// Framework for executing and comparing different cellular automata implementations
// (GPU and CPU). Manages multiple benchmark runs with randomized initial states,
// collects performance metrics (timing, memory usage), and validates consistency
// of results across implementations.
#pragma once

#include <vector>
#include <fstream>

#include "benchmark.h"
#include "json_helper.h"

// default parameters
constexpr int WIDTH_HEIGHT = 1 << 10;
constexpr int NUM_JOBS = 1 << 3;
constexpr int ITERATIONS = 1 << 9;
constexpr unsigned long SEED = 0;

// struct of the parameters describing one benchmark
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

// struct representing a single parameter setting and all its benchmark types
struct ParameterBenchmarkSet {
    BenchmarkParams params;
    std::vector<BenchmarkResult> benchmark_types;

    std::string to_json() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"parameters\": " << params.to_json() << ",";
        ss << "\"benchmark_types\": [";
        for (size_t i = 0; i < benchmark_types.size(); ++i) {
            if (i > 0) ss << ",";
            ss << benchmark_types[i].to_json();
        }
        ss << "]}";
        return ss.str();
    }
};

// struct representing an entire parameter sweep
struct ParameterSweep {
    std::string sweep_type;  // e.g., "width_height" or "iterations"
    std::vector<ParameterBenchmarkSet> benchmark_sets;

    void write_to_json(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        file << "{\n";
        file << "  \"sweep_type\": \"" << escape_json_string(sweep_type) << "\",\n";
        file << "  \"benchmark_sets\": [";

        for (size_t i = 0; i < benchmark_sets.size(); ++i) {
            if (i > 0) file << ",";
            file << "\n    " << benchmark_sets[i].to_json();
        }

        file << "\n  ]\n}\n";
        file.close();
    }
};

// Runs all benchmarks
ParameterBenchmarkSet run_benchmarks(BenchmarkParams params);
