#include "MoveGenerators.h"

#include "BoardIndicies.hpp"
#include "Piece.hpp"

namespace Tools {

std::array<Engine_NS::Bitboard, 64> generateMoves(const Engine_NS::Piece& piece) {
  using enum Engine_NS::PieceType;
  switch (piece.type()) {
  case Pawn:
    return generatePawnMoves(piece);
  case Knight:
    return generateKnightMoves();
  case Bishop:
    return generateBishopMoves();
  case Rook:
    return generateRookMoves();
  case Queen:
    return generateQueenMoves();
  case King:
    return generateKingMoves();
  default:
    break;
  }
  return std::array<Engine_NS::Bitboard, 64>();
}
std::array<Engine_NS::Bitboard, 64> generateAttacks(const Engine_NS::Piece& piece) {
  if (piece.type() == Engine_NS::PieceType::Pawn)
    return generatePawnAttacks(piece);
  return generateMoves(piece);
}

std::array<Engine_NS::Bitboard, 64> generatePawnMoves(const Engine_NS::Piece& piece) {
  const bool isWhite = piece.isWhite();

  std::array<Engine_NS::Bitboard, 64> moves = {};
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
std::array<Engine_NS::Bitboard, 64> generatePawnAttacks(const Engine_NS::Piece& piece) {
  const bool isWhite = piece.isWhite();

  std::array<Engine_NS::Bitboard, 64> attacks = {};
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

std::array<Engine_NS::Bitboard, 64> generateKnightMoves() {
  using namespace Engine_NS;

  std::array<Bitboard, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    const BoardIndex index = Index(i);

    const bool canMoveNorth = index.rank() < 8;
    const bool canMoveSouth = index.rank() > 1;
    const bool canMoveEast = index.file() < File::H;
    const bool canMoveWest = index.file() > File::A;

    // Clockwise, from north 2 west 1
    if (index.rank() < 7) {
      if (canMoveWest)
        moves.at(i).enableBit(index + North + NorthWest);
      if (canMoveEast)
        moves.at(i).enableBit(index + North + NorthEast);
    }
    if (index.file() < File::G) {
      if (canMoveNorth)
        moves.at(i).enableBit(index + East + NorthEast);
      if (canMoveSouth)
        moves.at(i).enableBit(index + East + SouthEast);
    }
    if (index.rank() > 2) {
      if (canMoveEast)
        moves.at(i).enableBit(index + South + SouthEast);
      if (canMoveWest)
        moves.at(i).enableBit(index + South + SouthWest);
    }
    if (index.file() > File::B) {
      if (canMoveSouth)
        moves.at(i).enableBit(index + West + SouthWest);
      if (canMoveNorth)
        moves.at(i).enableBit(index + West + NorthWest);
    }
  }
  return moves;
}

std::array<Engine_NS::Bitboard, 64> generateBishopMoves() {
  using namespace Engine_NS;

  std::array<Bitboard, 64> moves = {};
  for (int i = 0; i < 64; i++) {
    const BoardIndex index = Index(i);

    // NorthEast
    BoardIndex diagIndex = index;
    while (diagIndex.rank() < 8 && diagIndex.file() < File::H) {
      diagIndex = diagIndex + NorthEast;
      moves.at(i).enableBit(diagIndex);
    }

    // SouthEast
    diagIndex = index;
    while (diagIndex.rank() > 1 && diagIndex.file() < File::H) {
      diagIndex = diagIndex + SouthEast;
      moves.at(i).enableBit(diagIndex);
    }

    // SouthWest
    diagIndex = index;
    while (diagIndex.rank() > 1 && diagIndex.file() > File::A) {
      diagIndex = diagIndex + SouthWest;
      moves.at(i).enableBit(diagIndex);
    }

    // NorthWest
    diagIndex = index;
    while (diagIndex.rank() < 8 && diagIndex.file() > File::A) {
      diagIndex = diagIndex + NorthWest;
      moves.at(i).enableBit(diagIndex);
    }
  }
  return moves;
}

std::array<Engine_NS::Bitboard, 64> generateRookMoves() {
  std::array<Engine_NS::Bitboard, 64> moves = {};

  for (int i = 0; i < 64; i++) {
    const Engine_NS::BoardIndex index = Engine_NS::Index(i);
    for (int s = 0; s < 64; s++) {
      const Engine_NS::BoardIndex square = Engine_NS::Index(s);
      if ((index.rank() == square.rank()) || (index.file() == square.file()))
        moves.at(i).enableBit(square);
    }

    moves.at(i).disableBit(index);
  }
  return moves;
}

std::array<Engine_NS::Bitboard, 64> generateQueenMoves() {
  std::array<Engine_NS::Bitboard, 64> moves = {};
  const std::array<Engine_NS::Bitboard, 64> bishopMoves = generateBishopMoves();
  const std::array<Engine_NS::Bitboard, 64> rookMoves = generateRookMoves();
  for (int i = 0; i < 64; i++)
    moves.at(i).enableBits(bishopMoves.at(i) | rookMoves.at(i));
  return moves;
}

std::array<Engine_NS::Bitboard, 64> generateKingMoves() {
  std::array<Engine_NS::Bitboard, 64> moves = {};

  for (int i = 0; i < 64; i++) {
    const Engine_NS::BoardIndex index = Engine_NS::Index(i);

    const bool canMoveNorth = index.rank() < 8;
    const bool canMoveSouth = index.rank() > 1;
    const bool canMoveEast = index.file() < Engine_NS::File::H;
    const bool canMoveWest = index.file() > Engine_NS::File::A;

    if (canMoveNorth)
      moves.at(i).enableBit(index + Engine_NS::North);
    if (canMoveSouth)
      moves.at(i).enableBit(index + Engine_NS::South);
    if (canMoveEast)
      moves.at(i).enableBit(index + Engine_NS::East);
    if (canMoveWest)
      moves.at(i).enableBit(index + Engine_NS::West);

    if (canMoveNorth && canMoveEast)
      moves.at(i).enableBit(index + Engine_NS::NorthEast);
    if (canMoveNorth && canMoveWest)
      moves.at(i).enableBit(index + Engine_NS::NorthWest);
    if (canMoveSouth && canMoveEast)
      moves.at(i).enableBit(index + Engine_NS::SouthEast);
    if (canMoveSouth && canMoveWest)
      moves.at(i).enableBit(index + Engine_NS::SouthWest);
  }

  return moves;
}

} // namespace Tools
