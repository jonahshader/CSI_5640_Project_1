#pragma once

#include <cstdint>

namespace ca
{
  using cell_t = std::uint8_t;

  struct World {
    cell_t *state;
    int width;
    int height;
  };

}