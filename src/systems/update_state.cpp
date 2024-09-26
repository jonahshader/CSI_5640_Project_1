#include "update_state.h"

#include "types.h"

namespace ca
{
  // helper function to wrap around the edges of the world
  int wrap(int x, int max) {
    return (x + max) % max;
  }

  /**
   * @brief Count the number of living neighbors of a cell at (x, y)
   * 
   * @param world the current state of the world
   * @param x the x coordinate of the cell
   * @param y the y coordinate of the cell
   * @return neighbors_t number of neighbors
   */
  neighbors_t get_neighbors(const World &world, int x, int y)
  {
    neighbors_t neighbors = 0;
    // iterate over the 8 neighbors of the cell
    for (int dy = -1; dy <= 1; dy++)
    {
      for (int dx = -1; dx <= 1; dx++)
      {
        // ignore ourself (center cell)
        if (dx == 0 && dy == 0)
        {
          continue;
        }
        // compute the neighbor's coordinates
        int nx = wrap(x + dx, world.width);
        int ny = wrap(y + dy, world.height);
        // if the neighbor is alive, increment the count
        if (world.state[ny * world.width + nx] != 0)
        {
          neighbors++;
        }
      }
    }
    return neighbors;
  }

  /**
   * @brief perform one iteration of conway's game of life
   * 
   * @param read the current state of the world
   * @param write the next state of the world
   */
  void update_state(const World &read, World &write) {
    // iterate over each cell in the world
    for (int y = 0; y < read.height; y++)
    {
      for (int x = 0; x < read.width; x++)
      {
        // get the number of neighbors of the current cell
        neighbors_t neighbors = get_neighbors(read, x, y);
        // apply the rules of conway's game of life

        if (read.state[y * read.width + x] != 0) // if cell is alive,
        {
          if (neighbors < 2 || neighbors > 3)    //   if cell has less than 2 or more than 3 neighbors,
          {
            write.state[y * read.width + x] = 0; //     cell dies
          }
          else                                   //   if cell has 2 or 3 neighbors,
          {
            write.state[y * read.width + x] = 1; //     cell remains alive
          }
        }
        else                                     // if cell is dead,
        {
          if (neighbors == 3)                    //   if cell has exactly 3 neighbors,
          {
            write.state[y * read.width + x] = 1; //     cell becomes alive
          }
          else                                   //   if cell has any other number of neighbors,
          {
            write.state[y * read.width + x] = 0; //     cell remains dead
          }
        }
      }
    }
  }
}