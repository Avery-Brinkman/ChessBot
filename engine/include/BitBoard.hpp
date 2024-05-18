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

  Bitboard pawns[2] = {0, 0};
  Bitboard knights[2] = {0, 0};
  Bitboard bishops[2] = {0, 0};
  Bitboard rooks[2] = {0, 0};
  Bitboard queens[2] = {0, 0};
  Bitboard kings[2] = {0, 0};

  // [0 Rook King Rook], [0 Black | 0 White]
  int_fast8_t castlingRights = 0b01110111;

  Bitboard enPassant[2] = {0, 0};
};

} // namespace Engine_NS
