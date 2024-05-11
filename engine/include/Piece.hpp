#pragma once

#include "Directions.h"
#include <cstdint>

using PieceBits = uint_fast8_t;

namespace Engine_NS {

enum Color : uint_fast8_t { White = 0b0000, Black = 0b1000 };
enum PieceType : uint_fast8_t {
  NoneType = 0b0000,
  Pawn = 0b0001,
  Knight = 0b0010,
  Bishop = 0b0011,
  Rook = 0b0100,
  Queen = 0b0101,
  King = 0b0110
};

struct Piece {
  Color color() const { return Color(bits & 0b1000); }
  PieceType type() const { return PieceType(bits & 0b0111); }
  bool isWhite() const { return color() == Color::White; }

  CompassDirection forward() const { return isWhite() ? North : South; }
  CompassDirection backward() const { return isWhite() ? South : North; }
  CompassDirection left() const { return isWhite() ? West : East; }
  CompassDirection right() const { return isWhite() ? East : West; }
  CompassDirection forwardLeft() const { return isWhite() ? NorthWest : SouthEast; }
  CompassDirection forwardRight() const { return isWhite() ? NorthEast : SouthWest; }
  CompassDirection backwardLeft() const { return isWhite() ? SouthWest : NorthEast; }
  CompassDirection backwardRight() const { return isWhite() ? SouthEast : NorthWest; }

  PieceBits bits = Color::White | PieceType::NoneType;

  bool operator!=(const Piece& other) const { return bits != other.bits; };
  bool operator!=(const Piece& other) { return bits != other.bits; }
};

// Pieces

constexpr Piece None = {};

constexpr Piece WhitePawn(White | Pawn);
constexpr Piece WhiteKnight(Knight | White);
constexpr Piece WhiteBishop(Bishop | White);
constexpr Piece WhiteRook(Rook | White);
constexpr Piece WhiteQueen(Queen | White);
constexpr Piece WhiteKing(King | White);

constexpr Piece BlackPawn(Pawn | Black);
constexpr Piece BlackKnight(Knight | Black);
constexpr Piece BlackBishop(Bishop | Black);
constexpr Piece BlackRook(Rook | Black);
constexpr Piece BlackQueen(Queen | Black);
constexpr Piece BlackKing(King | Black);

} // namespace Engine_NS
