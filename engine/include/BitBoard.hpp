#pragma once

#include "BoardIndicies.hpp"

namespace Engine_NS {
using BitboardBits = uint_fast64_t;

struct Bitboard {
public:
  explicit Bitboard(const BoardIndex& index) { enableBit(index); }
  explicit(false) Bitboard(const BitboardBits& bits) : bits(bits) {}
  Bitboard() = default;

  BitboardBits bits = 0;

  void enableBit(const BoardIndex& index) { enableBits(Bitboard(index)); };
  void disableBit(const BoardIndex& index) { disableBits(Bitboard(index)); };
  void toggleBit(const BoardIndex& index) { toggleBits(Bitboard(index)); };
  bool checkBit(const BoardIndex& index) const { return checkBits(Bitboard(index)); };

  void enableBits(const Bitboard& bitsToEnable) { bits |= bitsToEnable.bits; };
  void disableBits(const Bitboard& bitsToDisable) { bits &= ~(bitsToDisable.bits); };
  void toggleBits(const Bitboard& bitsToToggle) { bits ^= bitsToToggle.bits; };
  bool checkBits(const Bitboard& bitsToCheck) const {
    return (bits & bitsToCheck.bits) == bitsToCheck.bits;
  };

  Bitboard operator&(const Bitboard& other) const { return bits & other.bits; }
  Bitboard operator|(const Bitboard& other) const { return bits | other.bits; }
  Bitboard operator^(const Bitboard& other) const { return bits ^ other.bits; }
  Bitboard operator~() const { return ~bits; }
  Bitboard operator<<(const int& value) const { return bits << value; }
  Bitboard operator>>(const int& value) const { return bits >> value; }
};

struct BoardInfo {
  Bitboard whitePieces{0};
  Bitboard blackPieces{0};
  Bitboard allPieces{0};
  Bitboard emptySquares{0};
};

struct Bitboards {
  Bitboard getWhitePieces() const;
  Bitboard getBlackPieces() const;

  BoardInfo getInfo() const;

  std::array<Bitboard, 2> pawns{};
  std::array<Bitboard, 2> knights{};
  std::array<Bitboard, 2> bishops{};
  std::array<Bitboard, 2> rooks{};
  std::array<Bitboard, 2> queens{};
  std::array<Bitboard, 2> kings{};

  // BlackQueenSide, BlackKingSide, WhiteQueenSide, WhiteKingSide
  int_fast8_t castlingRights = 0b1111;

  std::array<Bitboard, 2> enPassant{};
};

} // namespace Engine_NS
