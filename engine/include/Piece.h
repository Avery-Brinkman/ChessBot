#pragma once

#include <cstdint>

using Piece = uint_fast8_t;

namespace Engine_NS {

// Flags
constexpr Piece TypeFlag = 0b0111;
constexpr Piece ColorFlag = 0b1000;

// Types

constexpr Piece None = 0b0000;
constexpr Piece Pawn = 0b0001;
constexpr Piece Knight = 0b0010;
constexpr Piece Bishop = 0b0011;
constexpr Piece Rook = 0b0100;
constexpr Piece Queen = 0b0101;
constexpr Piece King = 0b0110;

// Colors

constexpr Piece White = 0b0000;
constexpr Piece Black = 0b1000;

// Pieces

constexpr Piece WhitePawn = Pawn | White;
constexpr Piece WhiteKnight = Knight | White;
constexpr Piece WhiteBishop = Bishop | White;
constexpr Piece WhiteRook = Rook | White;
constexpr Piece WhiteQueen = Queen | White;
constexpr Piece WhiteKing = King | White;

constexpr Piece BlackPawn = Pawn | Black;
constexpr Piece BlackKnight = Knight | Black;
constexpr Piece BlackBishop = Bishop | Black;
constexpr Piece BlackRook = Rook | Black;
constexpr Piece BlackQueen = Queen | Black;
constexpr Piece BlackKing = King | Black;

} // namespace Engine_NS