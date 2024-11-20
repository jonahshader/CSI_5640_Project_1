// Declare write_benchmark_json function, which takes in BenchmarkParams and BenchmarkPresults
// to produce one JSON file representing a single parameter sweep.
#include "json.h"

#include <sstream>
#include <iomanip>
#include <fstream>

void write_benchmark_json(
    const std::vector<BenchmarkParams>& params,
    const std::vector<BenchmarkResult>& results,
    const std::string& filename) {

    // params and results are parallel vectors. they need to be the same size
    if (params.size() != results.size()) {
        throw std::runtime_error("Params and results vectors must have the same size");
    }
    // open the file
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // write the JSON, starting with the array of benchmarks
    file << "{\n\"benchmarks\": [";
    // iterate over each benchmark and write their json representation
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) file << ",";
        file << "\n  {";
        file << "\n    \"parameters\": " << params[i].to_json() << ",";
        file << "\n    \"results\": " << results[i].to_json();
        file << "\n  }";
    }

    file << "\n]}\n";
    file.close();
}
