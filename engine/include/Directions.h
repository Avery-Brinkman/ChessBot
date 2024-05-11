#pragma once

#include <cstdint>

namespace Engine_NS {

enum CompassDirection : int_fast8_t {
  NORTH = 8,
  SOUTH = -8,
  EAST = 1,
  WEST = -1,
  NORTH_EAST = NORTH + EAST,
  NORTH_WEST = NORTH + WEST,
  SOUTH_EAST = SOUTH + EAST,
  SOUTH_WEST = SOUTH + WEST
};

} // namespace Engine_NS