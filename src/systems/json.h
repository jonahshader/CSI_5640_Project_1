// Declare write_benchmark_json function, which takes in BenchmarkParams and BenchmarkPresults
// to produce one JSON file representing a single parameter sweep.
#pragma once

#include <string>
#include <iostream>

#include "run_benchmarks.h"
#include "benchmark.h"


/**
 * @brief Writes a JSON file representing a single parameter sweep.
 *
 * @param params The parameters used to run the benchmarks
 * @param results The results of the benchmarks
 * @param filename The name of the file to write to
 */
void write_benchmark_json(
    const std::vector<BenchmarkParams>& params,
    const std::vector<BenchmarkResult>& results,
    const std::string& filename);
