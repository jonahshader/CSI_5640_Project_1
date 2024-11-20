// render_state.h: Defines a helper function for rendering the state of the cellular automaton using SDL2.
#pragma once

#include <SDL.h>

#include "types.h"

namespace ca {

void render_state(World &world, SDL_Renderer *renderer, SDL_Window *window);

}
