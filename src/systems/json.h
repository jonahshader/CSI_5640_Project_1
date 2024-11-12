#pragma once

#include <string>
#include <iostream>

#include "run_benchmarks.h"
#include "benchmark.h"

void write_benchmark_json(
    const std::vector<BenchmarkParams>& params,
    const std::vector<BenchmarkResult>& results,
    const std::string& filename);