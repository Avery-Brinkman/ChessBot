#include "MoveGenerators.h"

namespace Tools {

std::array<Engine_NS::BitBoardBits, 64> generateWhitePawnMoves() {
  std::array<Engine_NS::BitBoardBits, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    moves[i] = 0;
  }
  return moves;
}

std::array<Engine_NS::BitBoardBits, 64> generateBlackPawnMoves() {
  return generateWhitePawnMoves();
}

} // namespace Tools
