#pragma once

#include "BoardIndicies.hpp"

namespace Engine_NS {
using BitboardBits = uint_fast64_t;

struct Bitboard {
public:
  Bitboard(BoardIndex index) : Bitboard(1ULL << index.index) {}
  Bitboard(BitboardBits bits) : bits(bits) {}
  Bitboard() = default;

  BitboardBits bits = 0;

  void enableBit(const BoardIndex& index) { enableBits(Bitboard(index)); };
  void disableBit(const BoardIndex& index) { disableBits(Bitboard(index)); };
  void toggleBit(const BoardIndex& index) { toggleBits(Bitboard(index)); };
  bool checkBit(const BoardIndex& index) const { return checkBits(Bitboard(index)); };

  void enableBits(const Bitboard& bits) { this->bits |= bits.bits; };
  void disableBits(const Bitboard& bits) { this->bits &= ~(bits.bits); };
  void toggleBits(const Bitboard& bits) { this->bits ^= bits.bits; };
  bool checkBits(const Bitboard& bits) const { return this->bits & bits.bits; };

  Bitboard operator&(const Bitboard& other) const { return bits & other.bits; }
  Bitboard operator|(const Bitboard& other) const { return bits | other.bits; }
  Bitboard operator^(const Bitboard& other) const { return bits ^ other.bits; }
  Bitboard operator~() const { return ~bits; }
  Bitboard operator<<(int value) const { return bits << value; }
  Bitboard operator>>(int value) const { return bits >> value; }
};

struct BoardInfo {
  Bitboard whitePieces = 0;
  Bitboard blackPieces = 0;
  Bitboard allPieces = 0;
  Bitboard emptySquares = 0;
};

struct Bitboards {
  Bitboard getWhitePieces() const;
  Bitboard getBlackPieces() const;

  BoardInfo getInfo() const;

  Bitboard whitePawns = 0;
  Bitboard whiteKnights = 0;
  Bitboard whiteBishops = 0;
  Bitboard whiteRooks = 0;
  Bitboard whiteQueens = 0;
  Bitboard whiteKing = 0;

  Bitboard blackPawns = 0;
  Bitboard blackKnights = 0;
  Bitboard blackBishops = 0;
  Bitboard blackRooks = 0;
  Bitboard blackQueens = 0;
  Bitboard blackKing = 0;

  // [0 Rook King Rook], [0 Black | 0 White]
  int_fast8_t castlingRights = 0b01110111;

  Bitboard whiteEnPassant = 0;
  Bitboard blackEnPassant = 0;
};

} // namespace Engine_NS
