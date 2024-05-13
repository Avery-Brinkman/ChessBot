#pragma once

#include "BoardIndicies.hpp"

namespace Engine_NS {
using BitBoardBits = uint_fast64_t;

struct BitBoard {
public:
  BitBoard(BoardIndex index) : BitBoard(1ULL << index.index) {}
  BitBoard(BitBoardBits bits) : bits(bits) {}
  BitBoard() = default;

  BitBoardBits bits = 0;

  void enableBit(const BoardIndex& index) { enableBits(BitBoard(index)); };
  void disableBit(const BoardIndex& index) { disableBits(BitBoard(index)); };
  void toggleBit(const BoardIndex& index) { toggleBits(BitBoard(index)); };
  bool checkBit(const BoardIndex& index) const { return checkBits(BitBoard(index)); };

  void enableBits(const BitBoard& bits) { this->bits |= bits.bits; };
  void disableBits(const BitBoard& bits) { this->bits &= ~(bits.bits); };
  void toggleBits(const BitBoard& bits) { this->bits ^= bits.bits; };
  bool checkBits(const BitBoard& bits) const { return this->bits & bits.bits; };

  BitBoard operator&(const BitBoard& other) const { return bits & other.bits; }
  BitBoard operator|(const BitBoard& other) const { return bits | other.bits; }
  BitBoard operator^(const BitBoard& other) const { return bits ^ other.bits; }
  BitBoard operator~() const { return ~bits; }
  BitBoard operator<<(int value) const { return bits << value; }
  BitBoard operator>>(int value) const { return bits >> value; }
};

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
