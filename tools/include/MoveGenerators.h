#pragma once

#include "BitBoard.hpp"
#include <array>

namespace Tools {
std::array<BitBoardBits, 64> generateWhitePawnMoves();
std::array<BitBoardBits, 64> generateBlackPawnMoves();
} // namespace Tools
