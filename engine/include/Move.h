#pragma once

#include "BoardIndicies.h"
#include "Piece.h"
#include <cstdint>

using MoveFlags = uint_fast8_t;

namespace Engine_NS {
// Flags

constexpr MoveFlags NoFlag = 0b00000000;
constexpr MoveFlags DoublePawnPush = 0b00000001;
constexpr MoveFlags EnPassant = 0b00000010;
constexpr MoveFlags Castle = 0b00000011;
constexpr MoveFlags QueenPromotion = 0b00000100;
constexpr MoveFlags KnightPromotion = 0b00000101;
constexpr MoveFlags BishopPromotion = 0b00000110;
constexpr MoveFlags RookPromotion = 0b00000111;

struct Move {
  BoardIndex startPos = BoardIndex::A1;
  BoardIndex endPos = BoardIndex::A1;
  Piece movedPiece = Pieces::None;
  BoardIndex capturedPos = BoardIndex::A1;
  Piece capturedPiece = Pieces::None;
  MoveFlags flags = NoFlag;
};

} // namespace Engine_NS