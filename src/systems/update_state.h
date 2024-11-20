// Defines the CPU implementation of conway's game of life.
// Defined here because it is shared between the benchmarking and the SDL2 preview code.
#pragma once

#include "types.h"

namespace ca {
void update_state(const World &read, World &write);
}
