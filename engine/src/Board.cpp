#include "Board.h"
#include <numeric>

namespace Engine_NS {

void Board::clear() { m_board.fill(None); }

void Board::setToStartPosition() {
  m_whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
  m_whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
  m_whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
  m_whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000010000;
  m_whiteKing = 0b0000000000000000000000000000000000000000000000000000000000001000;
  m_whitePawns = 0b0000000000000010000000000000000000000000000000001111110100000000;

  m_blackRooks = 0b1000000100000000000000000000000000000000000000000000000000000000;
  m_blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
  m_blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
  m_blackQueens = 0b0001000000000000000000000000000000000000000000000000000000000000;
  m_blackKing = 0b0000100000000000000000000000000000000000000000000000000000000000;
  m_blackPawns = 0b0000000011111101000000000000000000000000000000000000000000000000;
}

void Board::makeMove(uint16_t startIndex, uint16_t endIndex) {
  m_board[endIndex] = m_board[startIndex];
  m_board[startIndex] = None;
}

size_t Board::getIndex(size_t row, size_t col) const { return (7 - row) * 8 + (7 - col); }

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(size_t index) const {
  const BitBoard pieceMask = 1ULL << index;
  if (m_whitePawns & pieceMask)
    return WhitePawn;
  if (m_whiteRooks & pieceMask)
    return WhiteRook;
  if (m_whiteKnights & pieceMask)
    return WhiteKnight;
  if (m_whiteBishops & pieceMask)
    return WhiteBishop;
  if (m_whiteQueens & pieceMask)
    return WhiteQueen;
  if (m_whiteKing & pieceMask)
    return WhiteKing;
  if (m_blackPawns & pieceMask)
    return BlackPawn;
  if (m_blackRooks & pieceMask)
    return BlackRook;
  if (m_blackKnights & pieceMask)
    return BlackKnight;
  if (m_blackBishops & pieceMask)
    return BlackBishop;
  if (m_blackQueens & pieceMask)
    return BlackQueen;
  if (m_blackKing & pieceMask)
    return BlackKing;

  return None;
}

BitBoard Board::getValidMoves(size_t index) const {
  const Piece piece = getPiece(index);

  switch (piece) {
  case WhitePawn:
  case BlackPawn:
    return getValidPawnMoves(index);
  case WhiteKnight:
  case BlackKnight:
    return getValidKnightMoves(index);
  case WhiteBishop:
    return getValidWhiteBishopMoves(index);
  case BlackBishop:
    return getValidBlackBishopMoves(index);
  case WhiteRook:
    return getValidWhiteRookMoves(index);
  case BlackRook:
    return getValidBlackRookMoves(index);
  case WhiteQueen:
    return getValidWhiteQueenMoves(index);
  case BlackQueen:
    return getValidBlackQueenMoves(index);
  case WhiteKing:
  case BlackKing:
    return getValidKingMoves(index);
  default:
    return 0;
  }
}

BitBoard Board::getValidCaptures(size_t index) const {
  const Piece piece = getPiece(index);

  switch (piece) {
  case WhitePawn:
  case BlackPawn:
    return getValidPawnCaptures(index);
  case WhiteKnight:
  case BlackKnight:
    // return getValidKnightMoves(index);
  case WhiteBishop:
    // return getValidWhiteBishopMoves(index);
  case BlackBishop:
    // return getValidBlackBishopMoves(index);
  case WhiteRook:
    // return getValidWhiteRookMoves(index);
  case BlackRook:
    // return getValidBlackRookMoves(index);
  case WhiteQueen:
    // return getValidWhiteQueenMoves(index);
  case BlackQueen:
    // return getValidBlackQueenMoves(index);
  case WhiteKing:
  case BlackKing:
    // return getValidKingMoves(index);
  default:
    return 0;
  }
}

void Board::movePiece(size_t from, size_t to) {
  const Piece piece = getPiece(from);

  updateBitBoards(from, to, piece);

  addPiece(piece, to);
  removePiece(piece, from);
}

void Board::capturePiece(size_t from, size_t to) {
  const Piece piece = getPiece(from);
  const Piece capturedPiece = getPiece(to);

  updateBitBoards(from, to, piece, capturedPiece);

  addPiece(piece, to);
  removePiece(piece, from);
  removePiece(capturedPiece, to);
}

// Protected Functions

BitBoard Board::getEnPassantMask() const { return m_enPassant; }

// Private Functions

BitBoard Board::getWhitePieces() const {
  return m_whitePawns | m_whiteRooks | m_whiteKnights | m_whiteBishops | m_whiteQueens |
         m_whiteKing;
}

BitBoard Board::getBlackPieces() const {
  return m_blackPawns | m_blackRooks | m_blackKnights | m_blackBishops | m_blackQueens |
         m_blackKing;
}

BitBoard Board::getValidPawnMoves(size_t index) const {
  const BitBoard whitePieces = getWhitePieces();
  const BitBoard blackPieces = getBlackPieces();
  const bool isWhite = whitePieces & (1ULL << index);
  const BitBoard allPieces = whitePieces | blackPieces;
  const BitBoard emptySquares = ~allPieces;

  const size_t row = index / 8;
  const int forward = isWhite ? 8 : -8;

  const bool canMoveForward = isWhite ? (row < 7) : (row > 0);
  const bool canMoveTwoSteps = isWhite ? (row == 1) : (row == 6);

  const BitBoard oneStep = canMoveForward ? 1ULL << (index + forward) : 0;
  const BitBoard twoSteps = canMoveTwoSteps ? 1ULL << (index + forward + forward) : 0;
  return (oneStep | twoSteps) & emptySquares;
}

BitBoard Board::getValidPawnCaptures(size_t index) const {
  const BitBoard whitePieces = getWhitePieces();
  const BitBoard blackPieces = getBlackPieces();
  const bool isWhite = whitePieces & (1ULL << index);
  const BitBoard opponentPieces = isWhite ? blackPieces : whitePieces;

  const size_t row = index / 8;
  const size_t col = index % 8;
  const int forwardLeft = isWhite ? 9 : -9;
  const int forwardRight = isWhite ? 7 : -7;

  const bool canMoveForward = isWhite ? (row < 7) : (row > 0);
  const bool canMoveForwardLeft =
      isWhite ? (canMoveForward && (col < 7)) : (canMoveForward && (col > 0));
  const bool canMoveForwardRight =
      isWhite ? (canMoveForward && (col > 0)) : (canMoveForward && (col < 7));

  const BitBoard leftCapture = canMoveForwardLeft ? 1ULL << (index + forwardLeft) : 0;
  const BitBoard rightCapture = canMoveForwardRight ? 1ULL << (index + forwardRight) : 0;
  return (leftCapture | rightCapture) & (opponentPieces | m_enPassant);
}

BitBoard Board::getValidKnightMoves(size_t index) const {
  const BitBoard whitePieces = getWhitePieces();
  const BitBoard blackPieces = getBlackPieces();
  const BitBoard validSquares =
      (whitePieces & (1ULL << index) ? blackPieces : whitePieces) | (~(whitePieces | blackPieces));

  const size_t row = index / 8;
  const size_t col = index % 8;

  const BitBoard one = (row < 6) && (col < 7) ? 1ULL << (index + 17) : 0;
  const BitBoard two = (row < 6) && (col > 0) ? 1ULL << (index + 15) : 0;
  const BitBoard three = (row < 7) && (col < 6) ? 1ULL << (index + 10) : 0;
  const BitBoard four = (row < 7) && (col > 1) ? 1ULL << (index + 6) : 0;
  const BitBoard five = (row > 0) && (col < 6) ? 1ULL << (index - 6) : 0;
  const BitBoard six = (row > 0) && (col > 1) ? 1ULL << (index - 10) : 0;
  const BitBoard seven = (row > 1) && (col < 7) ? 1ULL << (index - 15) : 0;
  const BitBoard eight = (row > 1) && (col > 0) ? 1ULL << (index - 17) : 0;

  return (one | two | three | four | five | six | seven | eight) & validSquares;
}

BitBoard Board::getValidWhiteBishopMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackBishopMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteRookMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackRookMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteQueenMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackQueenMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidKingMoves(size_t index) const {
  const BitBoard whitePieces = getWhitePieces();
  const BitBoard blackPieces = getBlackPieces();
  const bool isWhite = whitePieces & (1ULL << index);
  const BitBoard validSquares =
      (isWhite ? blackPieces : whitePieces) | (~(whitePieces | blackPieces));

  const size_t row = index / 8;
  const size_t col = index % 8;

  const BitBoard up = (row < 7) ? 1ULL << (index + 8) : 0;
  const BitBoard down = (row > 0) ? 1ULL << (index - 8) : 0;
  const BitBoard left = (col < 7) ? 1ULL << (index + 1) : 0;
  const BitBoard right = (col > 0) ? 1ULL << (index - 1) : 0;
  const BitBoard upLeft = (row < 7) && (col < 7) ? up << 1 : 0;
  const BitBoard upRight = (row < 7) && (col > 0) ? up >> 1 : 0;
  const BitBoard downLeft = (row > 0) && (col < 7) ? down << 1 : 0;
  const BitBoard downRight = (row > 0) && (col > 0) ? down >> 1 : 0;

  const BitBoard castling = getCastlingMoves(index, isWhite);

  return ((up | down | left | right | upLeft | upRight | downLeft | downRight) & validSquares) |
         castling;
}

BitBoard Board::getCastlingMoves(size_t index, bool isWhite) const {
  const BitBoard whitePieces = getWhitePieces();
  const BitBoard blackPieces = getBlackPieces();
  const BitBoard allPieces = whitePieces | blackPieces;
  const BitBoard emptySquares = ~allPieces;

  const size_t row = index / 8;
  const size_t col = index % 8;

  // 112 = 01110000
  const BitBoard leftMustBeFree = isWhite ? 112ULL : 112ULL << (8 * 7);
  // 6 = 00000110
  const BitBoard rightMustBeFree = isWhite ? 6ULL : 6ULL << (8 * 7);

  return leftMustBeFree | rightMustBeFree;
}

void Board::addPiece(Piece piece, size_t index) {
  const BitBoard pieceMask = 1ULL << index;

  switch (piece) {
  case WhitePawn:
    m_whitePawns |= pieceMask;
    break;
  case BlackPawn:
    m_blackPawns |= pieceMask;
    break;
  case WhiteKnight:
    m_whiteKnights |= pieceMask;
    break;
  case BlackKnight:
    m_blackKnights |= pieceMask;
    break;
  case WhiteBishop:
    m_whiteBishops |= pieceMask;
    break;
  case BlackBishop:
    m_blackBishops |= pieceMask;
    break;
  case WhiteRook:
    m_whiteRooks |= pieceMask;
    break;
  case BlackRook:
    m_blackRooks |= pieceMask;
    break;
  case WhiteQueen:
    m_whiteQueens |= pieceMask;
    break;
  case BlackQueen:
    m_blackQueens |= pieceMask;
    break;
  case WhiteKing:
    m_whiteKing |= pieceMask;
    break;
  case BlackKing:
    m_blackKing |= pieceMask;
    break;
  default:
    return;
  }
}

void Board::removePiece(Piece piece, size_t index) {
  const BitBoard pieceMask = 1ULL << index;
  switch (piece) {
  case WhitePawn:
    m_whitePawns &= ~pieceMask;
    break;
  case BlackPawn:
    m_blackPawns &= ~pieceMask;
    break;
  case WhiteKnight:
    m_whiteKnights &= ~pieceMask;
    break;
  case BlackKnight:
    m_blackKnights &= ~pieceMask;
    break;
  case WhiteBishop:
    m_whiteBishops &= ~pieceMask;
    break;
  case BlackBishop:
    m_blackBishops &= ~pieceMask;
    break;
  case WhiteRook:
    m_whiteRooks &= ~pieceMask;
    break;
  case BlackRook:
    m_blackRooks &= ~pieceMask;
    break;
  case WhiteQueen:
    m_whiteQueens &= ~pieceMask;
    break;
  case BlackQueen:
    m_blackQueens &= ~pieceMask;
    break;
  case WhiteKing:
    m_whiteKing &= ~pieceMask;
    break;
  case BlackKing:
    m_blackKing &= ~pieceMask;
    break;
  default:
    return;
  }
}

void Board::updateBitBoards(size_t from, size_t to, Piece movedPiece, Piece capturedPiece) {
  // Pawn move
  if ((movedPiece & TypeFlag) == Pawn) {
    // Moved two steps
    if (abs(static_cast<int>(from / 8) - static_cast<int>(to / 8)) > 1) {
      m_enPassant |= 1ULL << (std::midpoint(from, to));
    } else {
      // Clear en passant
      const int behind = (movedPiece & ColorFlag) == White ? -8 : 8;
      m_enPassant &= ~(1ULL << (from + behind));
    }
  }
}

} // namespace Engine_NS
