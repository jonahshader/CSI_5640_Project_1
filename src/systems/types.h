#pragma once

#include <cstdint>
#include <vector>
#include <random>

namespace ca
{
  using cell_t = std::uint8_t;
  using neighbors_t = std::uint8_t;

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
      for (int i = 0; i < width * height; ++i)
      {
        state.push_back(dis(gen));
      }
    }

    unsigned long get_mem_size() {
      return width * height * sizeof(cell_t) + 2 * sizeof(int);
    }
  };

}