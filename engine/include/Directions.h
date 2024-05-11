#pragma once

#include <cstdint>

namespace Engine_NS {

enum CompassDirection : int_fast8_t {
  North = 8,
  South = -8,
  East = 1,
  West = -1,
  NorthEast = North + East,
  NorthWest = North + West,
  SouthEast = South + East,
  SouthWest = South + West
};

} // namespace Engine_NS
