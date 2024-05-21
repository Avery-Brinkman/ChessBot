#pragma once

#include "Bitboard.hpp"
#include "PrecomputedMoves.h"

namespace Tools {

const Engine_NS::Bitboard EdgeMask = 0xFF818181818181FF;

void generateRookMagics();

} // namespace Tools
