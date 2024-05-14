#pragma once

#include "BitBoard.hpp"
#include "Piece.hpp"
#include <array>

namespace Tools {

std::array<Engine_NS::BitBoard, 64> generateMoves(const Engine_NS::Piece& piece);
std::array<Engine_NS::BitBoard, 64> generateAttacks(const Engine_NS::Piece& piece);

std::array<Engine_NS::BitBoard, 64> generatePawnMoves(const Engine_NS::Piece& piece);
std::array<Engine_NS::BitBoard, 64> generatePawnAttacks(const Engine_NS::Piece& piece);

std::array<Engine_NS::BitBoard, 64> generateKnightMoves();

std::array<Engine_NS::BitBoard, 64> generateBishopMoves();

std::array<Engine_NS::BitBoard, 64> generateRookMoves();

std::array<Engine_NS::BitBoard, 64> generateQueenMoves();

std::array<Engine_NS::BitBoard, 64> generateKingMoves();

} // namespace Tools
