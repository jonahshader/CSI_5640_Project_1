#pragma once

#include <SDL.h>

#include "types.h"

namespace ca {

void render_state(cell state[], int width, int height, SDL_Renderer* renderer, SDL_Window* window);

}
