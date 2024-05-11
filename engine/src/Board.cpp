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
  m_bitBoards.whiteRooks = 1ULL << 37;
  m_bitBoards.whiteKnights = 0;
  m_bitBoards.whiteBishops = 0;
  m_bitBoards.whiteQueens = 0;
  m_bitBoards.whiteKing = 0;
  m_bitBoards.whitePawns = 0;

  m_bitBoards.blackRooks = 0;
  m_bitBoards.blackKnights = 0;
  m_bitBoards.blackBishops = 0;
  m_bitBoards.blackQueens = 0;
  m_bitBoards.blackKing = 0;
  m_bitBoards.blackPawns = 0;
}

inline BoardIndex Board::getIndex(size_t row, size_t col) const {
  return BoardIndex((7 - row) * 8 + col);
}

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return Pieces::None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(BoardIndex index) const {
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

BitBoard Board::getValidMoves(BoardIndex index) const {
  using namespace Pieces;
  const Piece piece = getPiece(index);

  switch (getType(piece)) {
  case Pawn:
    return getValidPawnMoves(index);
  case Knight:
    return getValidKnightMoves(index);
  case Bishop:
    return getValidBishopMoves(index);
  default:
    return 0;
  }
}

void Board::movePiece(BoardIndex from, BoardIndex to) {
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
    if (abs((from / 8) - (to / 8)) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if ((1ULL << to) & m_bitBoards.enPassant) {
      flags = EnPassant;

      move.capturedPos = BoardIndex(to + getBackward(Pieces::isWhite(move.movedPiece)));
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
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;

  const size_t row = index / 8;
  const int forward = getForward(isWhite);
  const int forwardLeft = forward + getLeft(isWhite);
  const int forwardRight = forward + getRight(isWhite);

  const bool canMoveForwards = canMoveForward(index, isWhite);
  const bool canMoveTwoSteps = canMoveForwards && (isWhite ? (row == 1) : (row == 6));
  const bool canMoveForwardLeft = canMoveForwards && canMoveLeft(index, isWhite);
  const bool canMoveForwardRight = canMoveForwards && canMoveRight(index, isWhite);

  const BitBoard oneStep = canMoveForwards ? 1ULL << (index + forward) : 0;
  const BitBoard twoSteps = canMoveTwoSteps ? 1ULL << (index + forward + forward) : 0;
  const BitBoard moves = (oneStep | twoSteps) & boardInfo.emptySquares;

  const BitBoard leftCapture = canMoveForwardLeft ? 1ULL << (index + forwardLeft) : 0;
  const BitBoard rightCapture = canMoveForwardRight ? 1ULL << (index + forwardRight) : 0;
  const BitBoard captures = (leftCapture | rightCapture) & (opponentPieces | m_bitBoards.enPassant);

  return moves | captures;
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

BitBoard Board::getValidBishopMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteRookMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackRookMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteQueenMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackQueenMoves(size_t index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidKingMoves(size_t index) const {
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces & (1ULL << index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

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
      m_bitBoards.enPassant |= 1ULL << (std::midpoint(static_cast<unsigned char>(move.startPos),
                                                      static_cast<unsigned char>(move.endPos)));
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

  // Remove En Passant when a pawn is captured
  if (Pieces::getType(move.capturedPiece) == Pieces::Pawn) {
    if (move.capturedPos / 8 == 3 || move.capturedPos / 8 == 4) {
      m_bitBoards.enPassant &=
          ~(1ULL << (move.capturedPos + getBackward(Pieces::isWhite(move.capturedPiece))));
    }
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != Pieces::None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }
}

} // namespace Engine_NS
