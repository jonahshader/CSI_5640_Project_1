#pragma once

#include <chrono>
#include <vector>

struct BenchmarkResult {

}

class Benchmark {
public:
    // cannot construct Benchmark: it is an interface
    Benchmark() = delete;

    // TODO: docs
    virtual std::vector<double> run(int runs, bool warmup) = 0;
    virtual unsigned long get_mem_required() = 0;
protected:

}