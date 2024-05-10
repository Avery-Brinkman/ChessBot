#include "Board.h"
#include <iostream>
#include <numeric>

namespace {
struct Directions {
  int forward = 0;
  int backward = 0;
  int left = 0;
  int right = 0;
};

inline int getForward(bool isWhite) { return isWhite ? 8 : -8; }
inline int getBackward(bool isWhite) { return isWhite ? -8 : 8; }
inline int getLeft(bool isWhite) { return isWhite ? 1 : -1; }
inline int getRight(bool isWhite) { return isWhite ? -1 : 1; }

inline bool canMoveForward(size_t index, bool isWhite, int times = 0) {
  const int row = static_cast<int>(index) / 8;
  return isWhite ? (row < 7 - times) : (row > 0 + times);
}
inline bool canMoveBackward(size_t index, bool isWhite, int times = 0) {
  const int row = static_cast<int>(index) / 8;
  return isWhite ? (row > 0 + times) : (row < 7 - times);
}
inline bool canMoveLeft(size_t index, bool isWhite, int times = 0) {
  const int col = static_cast<int>(index) % 8;
  return isWhite ? (col < 7 - times) : (col > 0 + times);
}
inline bool canMoveRight(size_t index, bool isWhite, int times = 0) {
  const int col = static_cast<int>(index) % 8;
  return isWhite ? (col > 0 + times) : (col < 7 - times);
}

inline Directions getDirections(bool isWhite) {
  return Directions{.forward = getForward(isWhite),
                    .backward = getBackward(isWhite),
                    .left = getLeft(isWhite),
                    .right = getRight(isWhite)};
}
} // namespace

namespace Engine_NS {

void Board::setToStartPosition() {
  m_bitBoards.whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
  m_bitBoards.whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
  m_bitBoards.whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
  m_bitBoards.whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000010000;
  m_bitBoards.whiteKing = 0b0000000000000000000000000000000000000000000000000000000000001000;
  m_bitBoards.whitePawns = 0b0000000010000000000000000000000000000000000000000111111100000000;

  m_bitBoards.blackRooks = 0b0000000100000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackQueens = 0b0001000000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackKing = 0b0000100000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackPawns = 0b0000000001111111000000000000000000000000000000000000000000000000;
}

inline size_t Board::getIndex(size_t row, size_t col) const { return (7 - row) * 8 + (7 - col); }

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return Pieces::None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(size_t index) const {
  const BitBoard pieceMask = 1ULL << index;
  if (m_bitBoards.whitePawns & pieceMask)
    return Pieces::WhitePawn;
  if (m_bitBoards.whiteRooks & pieceMask)
    return Pieces::WhiteRook;
  if (m_bitBoards.whiteKnights & pieceMask)
    return Pieces::WhiteKnight;
  if (m_bitBoards.whiteBishops & pieceMask)
    return Pieces::WhiteBishop;
  if (m_bitBoards.whiteQueens & pieceMask)
    return Pieces::WhiteQueen;
  if (m_bitBoards.whiteKing & pieceMask)
    return Pieces::WhiteKing;
  if (m_bitBoards.blackPawns & pieceMask)
    return Pieces::BlackPawn;
  if (m_bitBoards.blackRooks & pieceMask)
    return Pieces::BlackRook;
  if (m_bitBoards.blackKnights & pieceMask)
    return Pieces::BlackKnight;
  if (m_bitBoards.blackBishops & pieceMask)
    return Pieces::BlackBishop;
  if (m_bitBoards.blackQueens & pieceMask)
    return Pieces::BlackQueen;
  if (m_bitBoards.blackKing & pieceMask)
    return Pieces::BlackKing;

  return Pieces::None;
}

BitBoard Board::getValidMoves(size_t index) const {
  const Piece piece = getPiece(index);

  switch (Pieces::getType(piece)) {
  case Pieces::Pawn:
    return getValidPawnMoves(index);
  case Pieces::Knight:
    return getValidKnightMoves(index);
  default:
    return 0;
  }
}

BitBoard Board::getValidCaptures(size_t index) const {
  const Piece piece = getPiece(index);

  switch (Pieces::getType(piece)) {
  case Pieces::Pawn:
    return getValidPawnCaptures(index);
  default:
    return 0;
  }
}

void Board::movePiece(size_t from, size_t to) {
  Move move = {};
  move.startPos = from;
  move.endPos = to;
  move.movedPiece = getPiece(from);

  move.capturedPiece = getPiece(to);
  if (move.capturedPiece != Pieces::None) {
    move.capturedPos = to;
  }

  MoveFlags flags = 0;

  if (Pieces::getType(move.movedPiece) == Pieces::Pawn) {
    // Double pawn push
    if (abs(static_cast<int>(from / 8) - static_cast<int>(to / 8)) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if ((1ULL << to) & m_bitBoards.enPassant) {
      flags = EnPassant;

      move.capturedPos = to + getBackward(Pieces::isWhite(move.movedPiece));
      move.capturedPiece = getPiece(move.capturedPos);
    }
    // Promotion
    else if (to / 8 == 0 || to / 8 == 7) {
      flags = QueenPromotion;
    }
  }

  move.flags = flags;

  updateBitBoards(move);
}

// Protected Functions

BitBoard Board::getEnPassantMask() const { return m_bitBoards.enPassant; }

// Private Functions

BitBoard Board::getWhitePieces() const { return m_bitBoards.getWhitePieces(); }

BitBoard Board::getBlackPieces() const { return m_bitBoards.getBlackPieces(); }

BitBoard Board::getValidPawnMoves(size_t index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces & (1ULL << index);

  const size_t row = index / 8;
  const int forward = getForward(isWhite);

  const bool canMoveForwards = canMoveForward(index, isWhite);
  const bool canMoveTwoSteps = canMoveForwards && (isWhite ? (row == 1) : (row == 6));

  const BitBoard oneStep = canMoveForwards ? 1ULL << (index + forward) : 0;
  const BitBoard twoSteps = canMoveTwoSteps ? 1ULL << (index + forward + forward) : 0;
  return (oneStep | twoSteps) & boardInfo.emptySquares;
}

BitBoard Board::getValidPawnCaptures(size_t index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces & (1ULL << index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;

  const int forward = getForward(isWhite);
  const int forwardLeft = forward + getLeft(isWhite);
  const int forwardRight = forward + getRight(isWhite);

  const bool canMoveForwards = canMoveForward(index, isWhite);
  const bool canMoveForwardLeft = canMoveForwards && canMoveLeft(index, isWhite);
  const bool canMoveForwardRight = canMoveForwards && canMoveRight(index, isWhite);

  const BitBoard leftCapture = canMoveForwardLeft ? 1ULL << (index + forwardLeft) : 0;
  const BitBoard rightCapture = canMoveForwardRight ? 1ULL << (index + forwardRight) : 0;
  return (leftCapture | rightCapture) & (opponentPieces | m_bitBoards.enPassant);
}

BitBoard Board::getValidKnightMoves(size_t index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces & (1ULL << index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

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
  case Pieces::WhitePawn:
    m_bitBoards.whitePawns |= pieceMask;
    break;
  case Pieces::BlackPawn:
    m_bitBoards.blackPawns |= pieceMask;
    break;
  case Pieces::WhiteKnight:
    m_bitBoards.whiteKnights |= pieceMask;
    break;
  case Pieces::BlackKnight:
    m_bitBoards.blackKnights |= pieceMask;
    break;
  case Pieces::WhiteBishop:
    m_bitBoards.whiteBishops |= pieceMask;
    break;
  case Pieces::BlackBishop:
    m_bitBoards.blackBishops |= pieceMask;
    break;
  case Pieces::WhiteRook:
    m_bitBoards.whiteRooks |= pieceMask;
    break;
  case Pieces::BlackRook:
    m_bitBoards.blackRooks |= pieceMask;
    break;
  case Pieces::WhiteQueen:
    m_bitBoards.whiteQueens |= pieceMask;
    break;
  case Pieces::BlackQueen:
    m_bitBoards.blackQueens |= pieceMask;
    break;
  case Pieces::WhiteKing:
    m_bitBoards.whiteKing |= pieceMask;
    break;
  case Pieces::BlackKing:
    m_bitBoards.blackKing |= pieceMask;
    break;
  default:
    return;
  }
}

void Board::removePiece(Piece piece, size_t index) {
  using namespace Pieces;

  const BitBoard pieceMask = 1ULL << index;
  switch (piece) {
  case WhitePawn:
    m_bitBoards.whitePawns &= ~pieceMask;
    break;
  case BlackPawn:
    m_bitBoards.blackPawns &= ~pieceMask;
    break;
  case WhiteKnight:
    m_bitBoards.whiteKnights &= ~pieceMask;
    break;
  case BlackKnight:
    m_bitBoards.blackKnights &= ~pieceMask;
    break;
  case WhiteBishop:
    m_bitBoards.whiteBishops &= ~pieceMask;
    break;
  case BlackBishop:
    m_bitBoards.blackBishops &= ~pieceMask;
    break;
  case WhiteRook:
    m_bitBoards.whiteRooks &= ~pieceMask;
    break;
  case BlackRook:
    m_bitBoards.blackRooks &= ~pieceMask;
    break;
  case WhiteQueen:
    m_bitBoards.whiteQueens &= ~pieceMask;
    break;
  case BlackQueen:
    m_bitBoards.blackQueens &= ~pieceMask;
    break;
  case WhiteKing:
    m_bitBoards.whiteKing &= ~pieceMask;
    break;
  case BlackKing:
    m_bitBoards.blackKing &= ~pieceMask;
    break;
  default:
    return;
  }
}

void Board::updateBitBoards(const Move& move) {
  Piece pieceToAdd = move.movedPiece;
  Piece pieceToRemove = move.movedPiece;

  // Pawn moves
  if (Pieces::getType(move.movedPiece) == Pieces::Pawn) {
    // Moved two steps
    if (move.flags == DoublePawnPush) {
      m_bitBoards.enPassant |= 1ULL << (std::midpoint(move.startPos, move.endPos));
    }
    // Clear en passant
    else {
      const size_t clearLoc = move.flags == EnPassant
                                  ? move.endPos
                                  : move.startPos + getBackward(Pieces::isWhite(move.movedPiece));
      m_bitBoards.enPassant &= ~(1ULL << clearLoc);
    }

    // Check for promotion
    if (move.flags & QueenPromotion) {
      pieceToAdd = Pieces::WhiteQueen;
    }
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != Pieces::None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }
}

} // namespace Engine_NS
