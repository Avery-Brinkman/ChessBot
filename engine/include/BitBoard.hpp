#pragma once

#include "BoardIndicies.h"

using BitBoardBits = uint_fast64_t;

namespace Engine_NS {

class BitBoard {
public:
  BitBoard(BoardIndex bitIndex) : BitBoard(1ULL << bitIndex) {}
  BitBoard(BitBoardBits bits) : m_bits(bits) {}

  BitBoardBits bits() const { return m_bits; }

  void enableBit(BoardIndex index) { enableBits(BitBoard(index)); };
  void disableBit(BoardIndex index) { disableBits(BitBoard(index)); };
  void toggleBit(BoardIndex index) { toggleBits(BitBoard(index)); };
  bool checkBit(BoardIndex index) const { return checkBits(BitBoard(index)); };

  void enableBits(const BitBoard& bits) { m_bits |= bits.bits(); };
  void disableBits(const BitBoard& bits) { m_bits &= ~(bits.bits()); };
  void toggleBits(const BitBoard& bits) { m_bits ^= bits.bits(); };
  bool checkBits(const BitBoard& bits) const { return m_bits & bits.bits(); };

  BitBoard operator&(const BitBoard& other) const { return m_bits & other.m_bits; }
  BitBoard operator|(const BitBoard& other) const { return m_bits | other.m_bits; }
  BitBoard operator^(const BitBoard& other) const { return m_bits ^ other.m_bits; }
  BitBoard operator~() const { return ~m_bits; }
  BitBoard operator<<(int value) const { return m_bits << value; }
  BitBoard operator>>(int value) const { return m_bits >> value; }

private:
  BitBoardBits m_bits = 0;
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
