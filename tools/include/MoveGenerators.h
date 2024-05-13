#pragma once

#include "BitBoard.hpp"
#include <array>

namespace Tools {
std::array<Engine_NS::BitBoardBits, 64> generateWhitePawnMoves();
std::array<Engine_NS::BitBoardBits, 64> generateBlackPawnMoves();
} // namespace Tools
