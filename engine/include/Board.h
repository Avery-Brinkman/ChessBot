#pragma once

#include "Move.h"
#include "Piece.h"

#include <array>

using BitBoard = uint_fast64_t;

namespace Engine_NS {
class Board {
public:
  void clear();

  void setToStartPosition();

  void makeMove(uint16_t startIndex, uint16_t endIndex);
  void makeMove(const Move& move);

  size_t getIndex(size_t row, size_t col) const;

  Piece getPiece(size_t row, size_t col) const;
  Piece getPiece(size_t index) const;

  BitBoard getValidMoves(size_t index) const;
  BitBoard getValidCaptures(size_t index) const;

  void movePiece(size_t from, size_t to);
  void capturePiece(size_t from, size_t to);

protected:
  BitBoard getEnPassantMask() const;

private:
  BitBoard getWhitePieces() const;
  BitBoard getBlackPieces() const;

  BitBoard getValidPawnMoves(size_t index) const;
  BitBoard getValidPawnCaptures(size_t index) const;
  BitBoard getValidKnightMoves(size_t index) const;
  BitBoard getValidWhiteBishopMoves(size_t index) const;
  BitBoard getValidBlackBishopMoves(size_t index) const;
  BitBoard getValidWhiteRookMoves(size_t index) const;
  BitBoard getValidBlackRookMoves(size_t index) const;
  BitBoard getValidWhiteQueenMoves(size_t index) const;
  BitBoard getValidBlackQueenMoves(size_t index) const;
  BitBoard getValidKingMoves(size_t index) const;

  BitBoard getCastlingMoves(size_t index, bool isWhite) const;

  void addPiece(Piece piece, size_t index);
  void removePiece(Piece piece, size_t index);

  void updateBitBoards(size_t from, size_t to, Piece movedPiece, Piece capturedPiece = None);

  std::array<Piece, 64> m_board = {};

  BitBoard m_whitePawns = 0;
  BitBoard m_whiteKnights = 0;
  BitBoard m_whiteBishops = 0;
  BitBoard m_whiteRooks = 0;
  BitBoard m_whiteQueens = 0;
  BitBoard m_whiteKing = 0;

  BitBoard m_blackPawns = 0;
  BitBoard m_blackKnights = 0;
  BitBoard m_blackBishops = 0;
  BitBoard m_blackRooks = 0;
  BitBoard m_blackQueens = 0;
  BitBoard m_blackKing = 0;

  // [0 Rook King Rook], [0 Black | 0 White]
  int_fast8_t m_castlingRights = 0b01110111;

  BitBoard m_enPassant = 0;
};
} // namespace Engine_NS