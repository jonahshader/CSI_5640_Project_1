#pragma once

#include <chrono>
#include <vector>
#include <string>

#include "types.h"

struct Job {
    ca::World initial_state;
    int iterations;
    std::string description;
}

struct BenchmarkResult {
    double duration{0};
    unsigned long memory_required{0};
}

class Benchmark {
public:
    virtual ~Benchmark(){};

    // TODO: docs
    virtual BenchmarkResult run(const Job &job) = 0;
    virtual std::string get_description() = 0;
}

class CPUNaive : public Benchmark {
public:
    BenchmarkResult run(const Job &job) override;
    std::string get_description() override;
}

class GPUNaive : public Benchmark {
public:
    BenchmarkResult run(const Job &job) override;
    std::string get_description() override;
}

class CPUOptimized : public Benchmark {
public:
    BenchmarkResult run(const Job &job) override;
    std::string get_description() override;
}

class GPUOptimized : public Benchmark {
public:
    BenchmarkResult run(const Job &job) override;
    std::string get_description() override;
}