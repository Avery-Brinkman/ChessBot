#pragma once

#include "Piece.h"

using MoveBits = uint_fast16_t;

namespace Engine_NS {
class Move {
  // Masks

  constexpr static MoveBits StartPosMask = 0b0000000000111111;
  constexpr static MoveBits EndPosMask = 0b0000111111000000;
  constexpr static MoveBits FlagMask = 0b1111000000000000;

  // Flags

  constexpr static MoveBits NoFlag = 0b0000000000000000;
  constexpr static MoveBits PawnMoveTwoFlag = 0b0001000000000000;
  constexpr static MoveBits EnPassantFlag = 0b0010000000000000;
  constexpr static MoveBits CastleFlag = 0b0011000000000000;
  constexpr static MoveBits QueenPromotionFlag = 0b1000000000000000;
  constexpr static MoveBits KnightPromotionFlag = 0b1001000000000000;
  constexpr static MoveBits BishopPromotionFlag = 0b1010000000000000;
  constexpr static MoveBits RookPromotionFlag = 0b1011000000000000;

public:
  Move(MoveBits start, MoveBits end, MoveBits flags = NoFlag);
  explicit Move(MoveBits move);

private:
  MoveBits m_move;
};
} // namespace Engine_NS