#pragma once

#include "BitBoard.h"
#include "Move.h"
#include "Piece.h"

namespace Engine_NS {
class Board {
public:
  void setToStartPosition();

  // Gets 1D index based on row col (where 0,0 is top left)
  size_t getIndex(size_t row, size_t col) const;

  // Gets the piece from the TL row col
  Piece getPiece(size_t row, size_t col) const;
  // Get the piece from the 1D index
  Piece getPiece(size_t index) const;

  BitBoard getValidMoves(size_t index) const;

  void movePiece(size_t from, size_t to);

protected:
  BitBoard getEnPassantMask() const;

private:
  BitBoard getWhitePieces() const;
  BitBoard getBlackPieces() const;

  BitBoard getValidPawnMoves(size_t index) const;
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

  void updateBitBoards(const Move& move);

  BitBoards m_bitBoards;
};
} // namespace Engine_NS