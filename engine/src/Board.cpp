#include "Board.h"
#include <iostream>
#include <numeric>

namespace {
bool canMoveForward(const Engine_NS::BoardIndex& index, bool isWhite, int times = 0) {
  const Engine_NS::Rank rank = index.rank();
  return isWhite ? (rank < 8 - times) : (rank > 1 + times);
}
bool canMoveBackward(const Engine_NS::BoardIndex& index, bool isWhite, int times = 0) {
  const Engine_NS::Rank rank = index.rank();
  return isWhite ? (rank > 1 + times) : (rank < 8 - times);
}
bool canMoveLeft(const Engine_NS::BoardIndex& index, bool isWhite, int times = 0) {
  const Engine_NS::File file = index.file();
  return isWhite ? (file > Engine_NS::File::A + times) : (file < Engine_NS::File::H - times);
}
bool canMoveRight(const Engine_NS::BoardIndex& index, bool isWhite, int times = 0) {
  const Engine_NS::File file = index.file();
  return isWhite ? (file < Engine_NS::File::H - times) : (file > Engine_NS::File::A + times);
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

BoardIndex Board::getIndex(size_t row, size_t col) const { return Index((7 - row) * 8 + col); }

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(const BoardIndex& index) const {
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

BitBoard Board::getValidMoves(const BoardIndex& index) const {
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

void Board::movePiece(const BoardIndex& from, const BoardIndex& to) {
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
    if (abs(from.rank() - to.rank()) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if (m_bitBoards.enPassant.checkBit(to)) {
      flags = EnPassant;

      move.capturedPos = Index(to.index + move.movedPiece.backward());
      move.capturedPiece = getPiece(move.capturedPos);
    }
    // Promotion
    else if (to.rank() == 0 || to.rank() == 7) {
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

BitBoard Board::getValidPawnMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const Piece piece = getPiece(index);
  const bool isWhite = piece.isWhite();
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;

  const Rank row = index.rank();
  const CompassDirection forward = piece.forward();
  const CompassDirection forwardLeft = piece.forwardLeft();
  const CompassDirection forwardRight = piece.forwardRight();

  const bool canMoveForwards = canMoveForward(index, isWhite);
  const bool canMoveTwoSteps = canMoveForwards && (isWhite ? (row == 1) : (row == 6));
  const bool canMoveForwardLeft = canMoveForwards && canMoveLeft(index, isWhite);
  const bool canMoveForwardRight = canMoveForwards && canMoveRight(index, isWhite);

  const BitBoard oneStep = canMoveForwards ? index.index + forward : 0;
  const BitBoard twoSteps = canMoveTwoSteps ? index.index + forward + forward : 0;
  const BitBoard moves = (oneStep | twoSteps) & boardInfo.emptySquares;

  const BitBoard leftCapture = canMoveForwardLeft ? index.index + forwardLeft : 0;
  const BitBoard rightCapture = canMoveForwardRight ? index.index + forwardRight : 0;
  const BitBoard captures = (leftCapture | rightCapture) & (opponentPieces | m_bitBoards.enPassant);

  return moves | captures;
}

BitBoard Board::getValidKnightMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

  const Rank rank = index.rank();
  const File file = index.file();

  const BitBoard one = (rank < 7) && (file > File::A) ? index.index + 17 : 0;
  // const BitBoard two = (row < 6) && (col > 0) ? index + 15 : 0;
  // const BitBoard three = (row < 7) && (col < 6) ? index + 10 : 0;
  // const BitBoard four = (row < 7) && (col > 1) ? index + 6 : 0;
  // const BitBoard five = (row > 0) && (col < 6) ? index - 6 : 0;
  // const BitBoard six = (row > 0) && (col > 1) ? index - 10 : 0;
  // const BitBoard seven = (row > 1) && (col < 7) ? index - 15 : 0;
  // const BitBoard eight = (row > 1) && (col > 0) ? index - 17 : 0;

  return one & validSquares;
  // return (one | two | three | four | five | six | seven | eight) & validSquares;
}

BitBoard Board::getValidBishopMoves(const BoardIndex& index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteRookMoves(const BoardIndex& index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidBlackRookMoves(const BoardIndex& index) const { return 0xFFFFFFFFFFFFFFFF; }

BitBoard Board::getValidWhiteQueenMoves(const BoardIndex& index) const {
  return 0xFFFFFFFFFFFFFFFF;
}

BitBoard Board::getValidBlackQueenMoves(const BoardIndex& index) const {
  return 0xFFFFFFFFFFFFFFFF;
}

BitBoard Board::getValidKingMoves(const BoardIndex& index) const {
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

  // const size_t row = index / 8;
  // const size_t col = index % 8;

  // const BitBoard up = (row < 7) ? index + 8 : 0;
  // const BitBoard down = (row > 0) ? index - 8 : 0;
  // const BitBoard left = (col < 7) ? index + 1 : 0;
  // const BitBoard right = (col > 0) ? index - 1 : 0;
  // const BitBoard upLeft = (row < 7) && (col < 7) ? up << 1 : 0;
  // const BitBoard upRight = (row < 7) && (col > 0) ? up >> 1 : 0;
  // const BitBoard downLeft = (row > 0) && (col < 7) ? down << 1 : 0;
  // const BitBoard downRight = (row > 0) && (col > 0) ? down >> 1 : 0;

  // const BitBoard castling = getCastlingMoves(index, isWhite);

  // return ((up | down | left | right | upLeft | upRight | downLeft | downRight) & validSquares) |
  //        castling;
  return 0;
}

BitBoard Board::getCastlingMoves(const BoardIndex& index, bool isWhite) const {
  // const BitBoard whitePieces = getWhitePieces();
  // const BitBoard blackPieces = getBlackPieces();
  // const BitBoard allPieces = whitePieces | blackPieces;
  // const BitBoard emptySquares = ~allPieces;

  // const size_t row = index / 8;
  // const size_t col = index % 8;

  //// 112 = 01110000
  // const BitBoard leftMustBeFree = isWhite ? 112ULL : 112ULL << (8 * 7);
  //// 6 = 00000110
  // const BitBoard rightMustBeFree = isWhite ? 6ULL : 6ULL << (8 * 7);

  // return leftMustBeFree | rightMustBeFree;
  return 0;
}

void Board::addPiece(const Piece& piece, const BoardIndex& index) {
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

void Board::removePiece(const Piece& piece, const BoardIndex& index) {
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
      m_bitBoards.enPassant.enableBit(Index(std::midpoint(
          static_cast<unsigned char>(move.startPos), static_cast<unsigned char>(move.endPos))));
    }
    // Clear en passant
    else {
      const BoardIndex clearLoc = Index(
          move.flags == EnPassant ? move.endPos : move.startPos.index + move.movedPiece.backward());
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
    if (move.capturedPos.rank() == 4 || move.capturedPos.rank() == 5) {
      m_bitBoards.enPassant.disableBit(
          Index(move.capturedPos.index + move.capturedPiece.backward()));
    }
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }
}

} // namespace Engine_NS
