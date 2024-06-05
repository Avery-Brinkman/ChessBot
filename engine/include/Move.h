#pragma once

#include "BoardIndicies.hpp"
#include "Piece.hpp"

namespace Engine_NS {

using MoveFlags = std::byte;

// Flags

constexpr MoveFlags NoFlag{0b00000000};
constexpr MoveFlags DoublePawnPush{0b00000001};
constexpr MoveFlags EnPassant{0b00000010};
constexpr MoveFlags Castle{0b00000011};
constexpr MoveFlags QueenPromotion{0b00000100};
constexpr MoveFlags KnightPromotion{0b00000101};
constexpr MoveFlags BishopPromotion{0b00000110};
constexpr MoveFlags RookPromotion{0b00000111};

struct Move {
  BoardIndex startPos{};
  BoardIndex endPos{};
  Piece movedPiece{};
  BoardIndex capturedPos{};
  Piece capturedPiece{};
  MoveFlags flags{};
};

} // namespace Engine_NS
