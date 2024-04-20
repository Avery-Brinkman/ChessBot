#pragma once

#include <cstdint>

using Piece = uint_fast8_t;

namespace Engine_NS {

enum PieceBits : uint_fast8_t {
  None = 0U,
  Pawn = 1U,
  Knight = 2U,
  Bishop = 3U,
  Rook = 4U,
  Queen = 5U,
  King = 6U,
};

enum ColorBits : uint_fast8_t {
  White = 0U,
  Black = 8U,
};

namespace Pieces {

constexpr Piece None = PieceBits::None;

constexpr Piece WhitePawn = PieceBits::Pawn | ColorBits::White;
constexpr Piece WhiteKnight = PieceBits::Knight | ColorBits::White;
constexpr Piece WhiteBishop = PieceBits::Bishop | ColorBits::White;
constexpr Piece WhiteRook = PieceBits::Rook | ColorBits::White;
constexpr Piece WhiteQueen = PieceBits::Queen | ColorBits::White;
constexpr Piece WhiteKing = PieceBits::King | ColorBits::White;

constexpr Piece BlackPawn = PieceBits::Pawn | ColorBits::Black;
constexpr Piece BlackKnight = PieceBits::Knight | ColorBits::Black;
constexpr Piece BlackBishop = PieceBits::Bishop | ColorBits::Black;
constexpr Piece BlackRook = PieceBits::Rook | ColorBits::Black;
constexpr Piece BlackQueen = PieceBits::Queen | ColorBits::Black;
constexpr Piece BlackKing = PieceBits::King | ColorBits::Black;

} // namespace Pieces

} // namespace Engine_NS