#include "Board.h"
#include "PrecomputedMoves.h"
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
  m_bitBoards.whitePawns = 0b0000000000000000000000000000000000000000000000001111111100000000;
  m_bitBoards.whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
  m_bitBoards.whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
  m_bitBoards.whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
  m_bitBoards.whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000001000;
  m_bitBoards.whiteKing = 0b0000000000000000000000000000000000000000000000000000000000010000;

  m_bitBoards.blackPawns = 0b0000000011111111000000000000000000000000000000000000000000000000;
  m_bitBoards.blackRooks = 0b1000000100000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackQueens = 0b0000100000000000000000000000000000000000000000000000000000000000;
  m_bitBoards.blackKing = 0b0001000000000000000000000000000000000000000000000000000000000000;
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
  case Rook:
    return getValidRookMoves(index);
  case Queen:
    return getValidQueenMoves(index);
  case King:
    return getValidKingMoves(index);
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
    const bool isWhite = move.movedPiece.isWhite();
    const BitBoard& opponentEnPassant =
        isWhite ? m_bitBoards.blackEnPassant : m_bitBoards.whiteEnPassant;

    // Double pawn push
    if (abs(from.rank() - to.rank()) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if (opponentEnPassant.checkBit(to)) {
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

BitBoard Board::getEnPassantMask() const {
  return m_bitBoards.whiteEnPassant | m_bitBoards.blackEnPassant;
}

void Board::togglePiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitBoards.whitePawns.toggleBit(index);
    break;
  case BlackPawn.bits:
    m_bitBoards.blackPawns.toggleBit(index);
    break;
  case WhiteKnight.bits:
    m_bitBoards.whiteKnights.toggleBit(index);
    break;
  case BlackKnight.bits:
    m_bitBoards.blackKnights.toggleBit(index);
    break;
  case WhiteBishop.bits:
    m_bitBoards.whiteBishops.toggleBit(index);
    break;
  case BlackBishop.bits:
    m_bitBoards.blackBishops.toggleBit(index);
    break;
  case WhiteRook.bits:
    m_bitBoards.whiteRooks.toggleBit(index);
    break;
  case BlackRook.bits:
    m_bitBoards.blackRooks.toggleBit(index);
    break;
  case WhiteQueen.bits:
    m_bitBoards.whiteQueens.toggleBit(index);
    break;
  case BlackQueen.bits:
    m_bitBoards.blackQueens.toggleBit(index);
    break;
  case WhiteKing.bits:
    m_bitBoards.whiteKing.toggleBit(index);
    break;
  case BlackKing.bits:
    m_bitBoards.blackKing.toggleBit(index);
    break;
  default:
    return;
  }
}

// Private Functions

BitBoard Board::getWhitePieces() const { return m_bitBoards.getWhitePieces(); }

BitBoard Board::getBlackPieces() const { return m_bitBoards.getBlackPieces(); }

BitBoard Board::getValidPawnMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const Piece piece = getPiece(index);
  const bool isWhite = piece.isWhite();
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard enPassant = isWhite ? m_bitBoards.blackEnPassant : m_bitBoards.whiteEnPassant;

  const BitBoard moves = Engine_NS::Precomputed::PawnMoves.at(isWhite).at(index.index);
  const BitBoard attacks = Engine_NS::Precomputed::PawnAttacks.at(isWhite).at(index.index);

  return (moves & boardInfo.emptySquares) | (attacks & (opponentPieces | enPassant));
}

BitBoard Board::getValidKnightMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

  return Precomputed::KnightMoves.at(index.index) & validSquares;
}

BitBoard Board::getValidBishopMoves(const BoardIndex& index) const {
  return Precomputed::BishopMoves.at(index.index);
}

BitBoard Board::getValidRookMoves(const BoardIndex& index) const {
  return Precomputed::RookMoves.at(index.index);
}

BitBoard Board::getValidQueenMoves(const BoardIndex& index) const {
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

  return Precomputed::QueenMoves.at(index.index) & validSquares;
}

BitBoard Board::getValidKingMoves(const BoardIndex& index) const {
  const BoardInfo boardInfo = m_bitBoards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const BitBoard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const BitBoard validSquares = opponentPieces | boardInfo.emptySquares;

  const BitBoard moves = Precomputed::KingMoves.at(index.index);

  // const BitBoard castling = getCastlingMoves(index, isWhite);
  const BitBoard castling = 0;

  return (moves & validSquares) | castling;
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

  const bool isWhite = move.movedPiece.isWhite();

  // Pawn moves
  if (move.movedPiece.type() == Pawn) {
    BitBoard& enPassant = isWhite ? m_bitBoards.whiteEnPassant : m_bitBoards.blackEnPassant;

    // Moved two steps
    if (move.flags == DoublePawnPush) {
      enPassant.enableBit(Index(std::midpoint(static_cast<unsigned char>(move.startPos),
                                              static_cast<unsigned char>(move.endPos))));
    }
    // Clear en passant
    else {
      const BoardIndex clearLoc =
          move.flags == EnPassant ? move.endPos : move.startPos + move.movedPiece.backward();
      enPassant.disableBit(clearLoc);
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
      BitBoard& opponentEnPassant =
          isWhite ? m_bitBoards.blackEnPassant : m_bitBoards.whiteEnPassant;

      opponentEnPassant.disableBit(Index(move.capturedPos.index + move.capturedPiece.backward()));
    }
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }
}

} // namespace Engine_NS
