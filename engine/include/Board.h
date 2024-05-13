#pragma once

#include "BitBoard.hpp"
#include "BoardIndicies.hpp"
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
  Piece getPiece(const BoardIndex& index) const;

  BitBoard getValidMoves(const BoardIndex& index) const;

  void movePiece(const BoardIndex& from, const BoardIndex& to);

protected:
  BitBoard getEnPassantMask() const;
  void togglePiece(const Piece& piece, const BoardIndex& index);

private:
  BitBoard getWhitePieces() const;
  BitBoard getBlackPieces() const;

  BitBoard getValidPawnMoves(const BoardIndex& index) const;
  BitBoard getValidKnightMoves(const BoardIndex& index) const;
  BitBoard getValidBishopMoves(const BoardIndex& index) const;
  BitBoard getValidWhiteRookMoves(const BoardIndex& index) const;
  BitBoard getValidBlackRookMoves(const BoardIndex& index) const;
  BitBoard getValidWhiteQueenMoves(const BoardIndex& index) const;
  BitBoard getValidBlackQueenMoves(const BoardIndex& index) const;
  BitBoard getValidKingMoves(const BoardIndex& index) const;

  BitBoard getCastlingMoves(const BoardIndex& index, bool isWhite) const;

  void addPiece(const Piece& piece, const BoardIndex& index);
  void removePiece(const Piece& piece, const BoardIndex& index);

  void updateBitBoards(const Move& move);

  BitBoards m_bitBoards;
};
} // namespace Engine_NS
