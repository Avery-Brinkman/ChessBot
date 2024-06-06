#pragma once

#include "Directions.h"

#include <compare>
#include <cstdint>

namespace Engine_NS {

using PieceBits = uint_fast8_t;

enum Color : PieceBits { White = 0b0000, Black = 0b1000 };
enum PieceType : PieceBits {
  NoneType = 0b0000,
  Pawn = 0b0001,
  Knight = 0b0010,
  Bishop = 0b0011,
  Rook = 0b0100,
  Queen = 0b0101,
  King = 0b0110
};

struct Piece {
  Piece(int) = delete;
  constexpr Piece(const Color& color, const PieceType& type) : bits(color | type) {}
  explicit(false) Piece(const PieceBits& bits) : bits(bits) {}
  Piece() = default;

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

  std::strong_ordering operator<=>(const Piece& other) const = default;
};

namespace Pieces {

constexpr Piece None{};

constexpr Piece WhitePawn(White, Pawn);
constexpr Piece WhiteKnight(White, Knight);
constexpr Piece WhiteBishop(White, Bishop);
constexpr Piece WhiteRook(White, Rook);
constexpr Piece WhiteQueen(White, Queen);
constexpr Piece WhiteKing(White, King);

constexpr Piece BlackPawn(Black, Pawn);
constexpr Piece BlackKnight(Black, Knight);
constexpr Piece BlackBishop(Black, Bishop);
constexpr Piece BlackRook(Black, Rook);
constexpr Piece BlackQueen(Black, Queen);
constexpr Piece BlackKing(Black, King);

} // namespace Pieces

} // namespace Engine_NS
