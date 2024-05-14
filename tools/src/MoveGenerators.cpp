#include "MoveGenerators.h"

#include "BoardIndicies.hpp"
#include "Piece.hpp"

namespace Tools {

std::array<Engine_NS::BitBoard, 64> generateMoves(const Engine_NS::Piece& piece) {
  using enum Engine_NS::PieceType;
  switch (piece.type()) {
  case Pawn:
    return generatePawnMoves(piece);
  case Knight:
    return generateKnightMoves(piece);
  default:
    break;
  }
  return std::array<Engine_NS::BitBoard, 64>();
}
std::array<Engine_NS::BitBoard, 64> generateAttacks(const Engine_NS::Piece& piece) {
  if (piece.type() == Engine_NS::PieceType::Pawn)
    return generatePawnAttacks(piece);
  return generateMoves(piece);
}

std::array<Engine_NS::BitBoard, 64> generatePawnMoves(const Engine_NS::Piece& piece) {
  const bool isWhite = piece.isWhite();

  std::array<Engine_NS::BitBoard, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    const Engine_NS::BoardIndex index = Engine_NS::Index(i);
    // Check if piece can move forward
    if (isWhite ? index.rank() < 8 : index.rank() > 1)
      moves.at(i).enableBit(index + piece.forward());

    // Check if piece can double push
    if (isWhite ? index.rank() == 2 : index.rank() == 7)
      moves.at(i).enableBit(index + piece.forward() + piece.forward());
  }
  return moves;
}
std::array<Engine_NS::BitBoard, 64> generatePawnAttacks(const Engine_NS::Piece& piece) {
  const bool isWhite = piece.isWhite();

  std::array<Engine_NS::BitBoard, 64> attacks = {};
  for (int i = 0; i < 64; i++) {
    const Engine_NS::BoardIndex index = Engine_NS::Index(i);
    // Check if piece can move forward
    if (isWhite ? index.rank() < 8 : index.rank() > 1) {
      // Forward left
      if (isWhite ? index.file() > Engine_NS::File::A : index.file() < Engine_NS::File::H)
        attacks.at(i).enableBit(index + piece.forwardLeft());

      // Forward right
      if (isWhite ? index.file() < Engine_NS::File::H : index.file() > Engine_NS::File::A)
        attacks.at(i).enableBit(index + piece.forwardRight());
    }
  }
  return attacks;
}

std::array<Engine_NS::BitBoard, 64> generateKnightMoves(const Engine_NS::Piece& piece) {
  return std::array<Engine_NS::BitBoard, 64>();
}

} // namespace Tools
