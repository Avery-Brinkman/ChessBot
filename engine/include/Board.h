#pragma once

#include "Bitboard.hpp"
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

  Bitboard getValidMoves(const BoardIndex& index) const;

  void movePiece(const BoardIndex& from, const BoardIndex& to);

protected:
  Bitboard getEnPassantMask() const;
  void togglePiece(const Piece& piece, const BoardIndex& index);

private:
  Bitboard getWhitePieces() const;
  Bitboard getBlackPieces() const;

  Bitboard getValidPawnMoves(const BoardIndex& index) const;
  Bitboard getValidKnightMoves(const BoardIndex& index) const;
  Bitboard getValidBishopMoves(const BoardIndex& index) const;
  Bitboard getValidRookMoves(const BoardIndex& index) const;
  Bitboard getValidQueenMoves(const BoardIndex& index) const;
  Bitboard getValidKingMoves(const BoardIndex& index) const;

  Bitboard getCastlingMoves(const BoardIndex& index, bool isWhite) const;

  void addPiece(const Piece& piece, const BoardIndex& index);
  void removePiece(const Piece& piece, const BoardIndex& index);

  void updateBitboards(const Move& move);

  Bitboards m_bitboards;
};
} // namespace Engine_NS
