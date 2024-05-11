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

int getForward(bool isWhite) { return isWhite ? 8 : -8; }
int getBackward(bool isWhite) { return isWhite ? -8 : 8; }
int getLeft(bool isWhite) { return isWhite ? 1 : -1; }
int getRight(bool isWhite) { return isWhite ? -1 : 1; }

bool canMoveForward(size_t index, bool isWhite, int times = 0) {
  const int row = static_cast<int>(index) / 8;
  return isWhite ? (row < 7 - times) : (row > 0 + times);
}
bool canMoveBackward(size_t index, bool isWhite, int times = 0) {
  const int row = static_cast<int>(index) / 8;
  return isWhite ? (row > 0 + times) : (row < 7 - times);
}
bool canMoveLeft(size_t index, bool isWhite, int times = 0) {
  const int col = static_cast<int>(index) % 8;
  return isWhite ? (col < 7 - times) : (col > 0 + times);
}
bool canMoveRight(size_t index, bool isWhite, int times = 0) {
  const int col = static_cast<int>(index) % 8;
  return isWhite ? (col > 0 + times) : (col < 7 - times);
}

Directions getDirections(bool isWhite) {
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

BoardIndex Board::getIndex(size_t row, size_t col) const { return BoardIndex((7 - row) * 8 + col); }

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(BoardIndex index) const {
  if (m_bitBoards.whitePawns.checkBit(index))
    return WhitePawn;
  if (m_bitBoards.whiteRooks.checkBit(index))
    return WhiteRook;
  if (m_bitBoards.whiteKnights.checkBit(index))
    return WhiteKnight;
  if (m_bitBoards.whiteBishops.checkBit(index))
    return WhiteBishop;
  if (m_bitBoards.whiteQueens.checkBit(index))
    return WhiteQueen;
  if (m_bitBoards.whiteKing.checkBit(index))
    return WhiteKing;
  if (m_bitBoards.blackPawns.checkBit(index))
    return BlackPawn;
  if (m_bitBoards.blackRooks.checkBit(index))
    return BlackRook;
  if (m_bitBoards.blackKnights.checkBit(index))
    return BlackKnight;
  if (m_bitBoards.blackBishops.checkBit(index))
    return BlackBishop;
  if (m_bitBoards.blackQueens.checkBit(index))
    return BlackQueen;
  if (m_bitBoards.blackKing.checkBit(index))
    return BlackKing;

  return None;
}

BitBoard Board::getValidMoves(BoardIndex index) const {
  const Piece piece = getPiece(index);

  switch (piece.type()) {
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
  if (move.capturedPiece != None) {
    move.capturedPos = to;
  }

  MoveFlags flags = 0;

  if (move.movedPiece.type() == Pawn) {
    // Double pawn push
    if (abs((from / 8) - (to / 8)) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if (m_bitBoards.enPassant.checkBit(to)) {
      flags = EnPassant;

      move.capturedPos = BoardIndex(to + getBackward(move.movedPiece.isWhite()));
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

BitBoard Board::getValidPawnMoves(BoardIndex index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
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

BitBoard Board::getValidKnightMoves(BoardIndex index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
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

BitBoard Board::getValidBishopMoves(BoardIndex index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteRookMoves(BoardIndex index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackRookMoves(BoardIndex index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteQueenMoves(BoardIndex index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackQueenMoves(BoardIndex index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidKingMoves(BoardIndex index) const {
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
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

void Board::addPiece(Piece piece, BoardIndex index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitBoards.whitePawns.enableBit(index);
    break;
  case BlackPawn.bits:
    m_bitBoards.blackPawns.enableBit(index);
    break;
  case WhiteKnight.bits:
    m_bitBoards.whiteKnights.enableBit(index);
    break;
  case BlackKnight.bits:
    m_bitBoards.blackKnights.enableBit(index);
    break;
  case WhiteBishop.bits:
    m_bitBoards.whiteBishops.enableBit(index);
    break;
  case BlackBishop.bits:
    m_bitBoards.blackBishops.enableBit(index);
    break;
  case WhiteRook.bits:
    m_bitBoards.whiteRooks.enableBit(index);
    break;
  case BlackRook.bits:
    m_bitBoards.blackRooks.enableBit(index);
    break;
  case WhiteQueen.bits:
    m_bitBoards.whiteQueens.enableBit(index);
    break;
  case BlackQueen.bits:
    m_bitBoards.blackQueens.enableBit(index);
    break;
  case WhiteKing.bits:
    m_bitBoards.whiteKing.enableBit(index);
    break;
  case BlackKing.bits:
    m_bitBoards.blackKing.enableBit(index);
    break;
  default:
    return;
  }
}

void Board::removePiece(Piece piece, BoardIndex index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitBoards.whitePawns.disableBit(index);
    break;
  case BlackPawn.bits:
    m_bitBoards.blackPawns.disableBit(index);
    break;
  case WhiteKnight.bits:
    m_bitBoards.whiteKnights.disableBit(index);
    break;
  case BlackKnight.bits:
    m_bitBoards.blackKnights.disableBit(index);
    break;
  case WhiteBishop.bits:
    m_bitBoards.whiteBishops.disableBit(index);
    break;
  case BlackBishop.bits:
    m_bitBoards.blackBishops.disableBit(index);
    break;
  case WhiteRook.bits:
    m_bitBoards.whiteRooks.disableBit(index);
    break;
  case BlackRook.bits:
    m_bitBoards.blackRooks.disableBit(index);
    break;
  case WhiteQueen.bits:
    m_bitBoards.whiteQueens.disableBit(index);
    break;
  case BlackQueen.bits:
    m_bitBoards.blackQueens.disableBit(index);
    break;
  case WhiteKing.bits:
    m_bitBoards.whiteKing.disableBit(index);
    break;
  case BlackKing.bits:
    m_bitBoards.blackKing.disableBit(index);
    break;
  default:
    return;
  }
}

void Board::updateBitBoards(const Move& move) {
  Piece pieceToAdd = move.movedPiece;
  Piece pieceToRemove = move.movedPiece;

  // Pawn moves
  if (move.movedPiece.type() == Pawn) {
    // Moved two steps
    if (move.flags == DoublePawnPush) {
      m_bitBoards.enPassant.enableBit(BoardIndex(std::midpoint(
          static_cast<unsigned char>(move.startPos), static_cast<unsigned char>(move.endPos))));
    }
    // Clear en passant
    else {
      const BoardIndex clearLoc = BoardIndex(
          move.flags == EnPassant ? move.endPos
                                  : move.startPos + getBackward(move.movedPiece.isWhite()));
      m_bitBoards.enPassant.disableBit(clearLoc);
    }

    // Check for promotion
    if (move.flags & QueenPromotion) {
      // TODO: This gets triggered for a promotion of any type
      pieceToAdd = WhiteQueen;
    }
  }

  // Remove En Passant when a pawn is captured
  if (move.capturedPiece.type() == Pawn) {
    if (move.capturedPos / 8 == 3 || move.capturedPos / 8 == 4) {
      m_bitBoards.enPassant.disableBit(
          BoardIndex(move.capturedPos + getBackward(move.capturedPiece.isWhite())));
    }
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }
}

} // namespace Engine_NS
