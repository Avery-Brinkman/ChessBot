#pragma once

#include "BitBoard.hpp"
#include "BoardIndicies.h"
#include "Move.h"
#include "Piece.hpp"

namespace Engine_NS {
class Board {
public:
  void setToStartPosition();

  // Gets 1D index based on row col (where 0,0 is top left)
  BoardIndex getIndex(size_t row, size_t col) const;

  // Gets the piece from the TL row col
  Piece getPiece(size_t row, size_t col) const;
  // Get the piece from the 1D index
  Piece getPiece(BoardIndex index) const;

  BitBoard getValidMoves(BoardIndex index) const;

  void movePiece(BoardIndex from, BoardIndex to);

protected:
  BitBoard getEnPassantMask() const;

private:
  BitBoard getWhitePieces() const;
  BitBoard getBlackPieces() const;

  BitBoard getValidPawnMoves(BoardIndex index) const;
  BitBoard getValidKnightMoves(BoardIndex index) const;
  BitBoard getValidBishopMoves(BoardIndex index) const;
  BitBoard getValidWhiteRookMoves(BoardIndex index) const;
  BitBoard getValidBlackRookMoves(BoardIndex index) const;
  BitBoard getValidWhiteQueenMoves(BoardIndex index) const;
  BitBoard getValidBlackQueenMoves(BoardIndex index) const;
  BitBoard getValidKingMoves(BoardIndex index) const;

  BitBoard getCastlingMoves(size_t index, bool isWhite) const;

  void addPiece(Piece piece, BoardIndex index);
  void removePiece(Piece piece, BoardIndex index);

  void updateBitBoards(const Move& move);

  BitBoards m_bitBoards;
};
} // namespace Engine_NS
