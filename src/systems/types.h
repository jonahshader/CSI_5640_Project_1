// Defines some types to make it easier to describe and work with cellular automata state
#pragma once

#include <cstdint>
#include <vector>
#include <random>

namespace ca {
// A single cell can be represented with 1 bit, but the smallest addressable size is a byte:
using cell_t = std::uint8_t;
// There are 8 neighbors. Counting the number of living neighbors requires log2(8)+1=4 bits,
// but the smallest addressable size is a byte:
using neighbors_t = std::uint8_t;

// The state of a cellular automata world at a single point in time.
// World is fixed-size.
struct World {
  std::vector<cell_t> state;
  int width;
  int height;

  // constructor for randomly initialized worlds
  World(int width, int height, std::mt19937 &gen) {
    this->width = width;
    this->height = height;
    state.reserve(width * height);

    // def distribution for a cell: 0 or 1
    std::uniform_int_distribution<int> dis(0, 1);
    // repeatedly sample from distribution to populate world
    for (int i = 0; i < width * height; ++i) {
      state.push_back(dis(gen));
    }
  }

  World() = default;

  unsigned long get_mem_size() {
    return width * height * sizeof(cell_t) + 2 * sizeof(int);
  }
};

// define comparison operator. used in validation.
bool operator==(const World &lhs, const World &rhs);
// define comparison operator. used in validation.
bool operator!=(const World &lhs, const World &rhs);

} // namespace ca
