#pragma once

#include <cstdint>

using BitBoard = uint_fast64_t;

namespace Engine_NS {

struct BoardInfo {
  BitBoard whitePieces = 0;
  BitBoard blackPieces = 0;
  BitBoard allPieces = 0;
  BitBoard emptySquares = 0;
};

struct BitBoards {
  BitBoard getWhitePieces() const;
  BitBoard getBlackPieces() const;

  BoardInfo getInfo() const;

  BitBoard whitePawns = 0;
  BitBoard whiteKnights = 0;
  BitBoard whiteBishops = 0;
  BitBoard whiteRooks = 0;
  BitBoard whiteQueens = 0;
  BitBoard whiteKing = 0;

  BitBoard blackPawns = 0;
  BitBoard blackKnights = 0;
  BitBoard blackBishops = 0;
  BitBoard blackRooks = 0;
  BitBoard blackQueens = 0;
  BitBoard blackKing = 0;

  // [0 Rook King Rook], [0 Black | 0 White]
  int_fast8_t castlingRights = 0b01110111;

  BitBoard enPassant = 0;
};

} // namespace Engine_NS