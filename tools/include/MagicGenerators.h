#pragma once

#include "Bitboard.hpp"
#include "PrecomputedMoves.h"
#include <utility>
#include <vector>

namespace Tools {

// Magic Number, Shift
using MagicValues = std::pair<size_t, size_t>;

const int MaxBishopMoves = 512;
const int MaxRookMoves = 4096;

const Engine_NS::Bitboard TopEdge = 0xFF00000000000000;
const Engine_NS::Bitboard BottomEdge = 0x00000000000000FF;
const Engine_NS::Bitboard LeftEdge = 0x0101010101010101;
const Engine_NS::Bitboard RightEdge = 0x8080808080808080;

void generateRookMagics();

void generateBishopMagics();

} // namespace Tools
