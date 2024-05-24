#pragma once

#include "Bitboard.hpp"
#include "PrecomputedMoves.h"
#include <utility>
#include <vector>

namespace Tools {

// Magic Number, Shift
using MagicValues = std::pair<int, int>;

const Engine_NS::Bitboard EdgeMask = 0xFF818181818181FF;
const Engine_NS::Bitboard TopEdge = 0xFF00000000000000;
const Engine_NS::Bitboard BottomEdge = 0x00000000000000FF;
const Engine_NS::Bitboard LeftEdge = 0x0101010101010101;
const Engine_NS::Bitboard RightEdge = 0x8080808080808080;

MagicValues generateMagicValues(const std::vector<Engine_NS::Bitboard>& blockers);

std::vector<Engine_NS::Bitboard> generateAllBlockers(const Engine_NS::Bitboard& allowedMoves);

std::vector<Engine_NS::Bitboard>
generateValidRookMoves(const Engine_NS::Bitboard& allowedMoves,
                       const std::vector<Engine_NS::Bitboard>& blockers,
                       Engine_NS::BoardIndex position);

void generateRookMagics();

} // namespace Tools
