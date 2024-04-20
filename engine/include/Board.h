#pragma once

#include "Piece.h"

#include <array>

namespace Engine_NS {
class Board {
public:
private:
  std::array<Piece, 64> m_board = {};
};
} // namespace Engine_NS