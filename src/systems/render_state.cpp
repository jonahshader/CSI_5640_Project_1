// render_state.cpp: Defines a helper function for rendering the state of the cellular automaton using SDL2.
#include "render_state.h"

#include <SDL.h>
#include <algorithm>

namespace ca {
/**
 * @brief draws the current state of the cellular automaton to the screen
 *
 * @param state array of cells representing CA state
 * @param width width of the CA state
 * @param height height of the CA state
 * @param renderer SDL renderer to draw to
 * @param window SDL window to draw to
 */
void render_state(World &world, SDL_Renderer *renderer, SDL_Window *window) {
  // clear the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // set the draw color to white
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  // calculate the size of each cell
  int screen_width, screen_height;
  SDL_GetWindowSize(window, &screen_width, &screen_height);
  int cell_size = std::min(screen_width / world.width, screen_height / world.height);

  // draw each cell
  for (int y = 0; y < world.height; y++) {
    for (int x = 0; x < world.width; x++) {
      // only draw cells that are alive
      if (world.state[y * world.width + x] != 0) {
        SDL_Rect rect = {x * cell_size, y * cell_size, cell_size, cell_size};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
}

} // namespace ca
