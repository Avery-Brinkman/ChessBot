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
  m_bitboards.whitePawns = 0b0000000000000000000000000000000000000000000000001111111100000000;
  m_bitboards.whiteRooks = 0b0000000000000000000000000000000000000000000000000000000010000001;
  m_bitboards.whiteKnights = 0b0000000000000000000000000000000000000000000000000000000001000010;
  m_bitboards.whiteBishops = 0b0000000000000000000000000000000000000000000000000000000000100100;
  m_bitboards.whiteQueens = 0b0000000000000000000000000000000000000000000000000000000000001000;
  m_bitboards.whiteKing = 0b0000000000000000000000000000000000000000000000000000000000010000;

  m_bitboards.blackPawns = 0b0000000011111111000000000000000000000000000000000000000000000000;
  m_bitboards.blackRooks = 0b1000000100000000000000000000000000000000000000000000000000000000;
  m_bitboards.blackKnights = 0b0100001000000000000000000000000000000000000000000000000000000000;
  m_bitboards.blackBishops = 0b0010010000000000000000000000000000000000000000000000000000000000;
  m_bitboards.blackQueens = 0b0000100000000000000000000000000000000000000000000000000000000000;
  m_bitboards.blackKing = 0b0001000000000000000000000000000000000000000000000000000000000000;
}

BoardIndex Board::getIndex(size_t row, size_t col) const { return Index((7 - row) * 8 + col); }

Piece Board::getPiece(size_t row, size_t col) const {
  if (row > 7 || col > 7)
    return None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(const BoardIndex& index) const {
  if (m_bitboards.whitePawns.checkBit(index))
    return WhitePawn;
  if (m_bitboards.whiteRooks.checkBit(index))
    return WhiteRook;
  if (m_bitboards.whiteKnights.checkBit(index))
    return WhiteKnight;
  if (m_bitboards.whiteBishops.checkBit(index))
    return WhiteBishop;
  if (m_bitboards.whiteQueens.checkBit(index))
    return WhiteQueen;
  if (m_bitboards.whiteKing.checkBit(index))
    return WhiteKing;
  if (m_bitboards.blackPawns.checkBit(index))
    return BlackPawn;
  if (m_bitboards.blackRooks.checkBit(index))
    return BlackRook;
  if (m_bitboards.blackKnights.checkBit(index))
    return BlackKnight;
  if (m_bitboards.blackBishops.checkBit(index))
    return BlackBishop;
  if (m_bitboards.blackQueens.checkBit(index))
    return BlackQueen;
  if (m_bitboards.blackKing.checkBit(index))
    return BlackKing;

  return None;
}

Bitboard Board::getValidMoves(const BoardIndex& index) const {
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
    const Bitboard& opponentEnPassant =
        isWhite ? m_bitboards.blackEnPassant : m_bitboards.whiteEnPassant;

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

  updateBitboards(move);
}

// Protected Functions

Bitboard Board::getEnPassantMask() const {
  return m_bitboards.whiteEnPassant | m_bitboards.blackEnPassant;
}

void Board::togglePiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitboards.whitePawns.toggleBit(index);
    break;
  case BlackPawn.bits:
    m_bitboards.blackPawns.toggleBit(index);
    break;
  case WhiteKnight.bits:
    m_bitboards.whiteKnights.toggleBit(index);
    break;
  case BlackKnight.bits:
    m_bitboards.blackKnights.toggleBit(index);
    break;
  case WhiteBishop.bits:
    m_bitboards.whiteBishops.toggleBit(index);
    break;
  case BlackBishop.bits:
    m_bitboards.blackBishops.toggleBit(index);
    break;
  case WhiteRook.bits:
    m_bitboards.whiteRooks.toggleBit(index);
    break;
  case BlackRook.bits:
    m_bitboards.blackRooks.toggleBit(index);
    break;
  case WhiteQueen.bits:
    m_bitboards.whiteQueens.toggleBit(index);
    break;
  case BlackQueen.bits:
    m_bitboards.blackQueens.toggleBit(index);
    break;
  case WhiteKing.bits:
    m_bitboards.whiteKing.toggleBit(index);
    break;
  case BlackKing.bits:
    m_bitboards.blackKing.toggleBit(index);
    break;
  default:
    return;
  }
}

// Private Functions

Bitboard Board::getWhitePieces() const { return m_bitboards.getWhitePieces(); }

Bitboard Board::getBlackPieces() const { return m_bitboards.getBlackPieces(); }

Bitboard Board::getValidPawnMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitboards.getInfo();
  const Piece piece = getPiece(index);
  const bool isWhite = piece.isWhite();
  const Bitboard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const Bitboard enPassant = isWhite ? m_bitboards.blackEnPassant : m_bitboards.whiteEnPassant;

  const Bitboard moves = Engine_NS::Precomputed::PawnMoves.at(isWhite).at(index.index);
  const Bitboard attacks = Engine_NS::Precomputed::PawnAttacks.at(isWhite).at(index.index);

  return (moves & boardInfo.emptySquares) | (attacks & (opponentPieces | enPassant));
}

Bitboard Board::getValidKnightMoves(const BoardIndex& index) const {
  // TODO: This only needs to be calculated once per move
  const BoardInfo boardInfo = m_bitboards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const Bitboard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const Bitboard validSquares = opponentPieces | boardInfo.emptySquares;

  return Precomputed::KnightMoves.at(index.index) & validSquares;
}

Bitboard Board::getValidBishopMoves(const BoardIndex& index) const {
  return Precomputed::BishopMoves.at(index.index);
}

Bitboard Board::getValidRookMoves(const BoardIndex& index) const {
  return Precomputed::RookMoves.at(index.index);
}

Bitboard Board::getValidQueenMoves(const BoardIndex& index) const {
  const BoardInfo boardInfo = m_bitboards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const Bitboard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const Bitboard validSquares = opponentPieces | boardInfo.emptySquares;

  return Precomputed::QueenMoves.at(index.index) & validSquares;
}

Bitboard Board::getValidKingMoves(const BoardIndex& index) const {
  const BoardInfo boardInfo = m_bitboards.getInfo();
  const bool isWhite = boardInfo.whitePieces.checkBit(index);
  const Bitboard opponentPieces = isWhite ? boardInfo.blackPieces : boardInfo.whitePieces;
  const Bitboard validSquares = opponentPieces | boardInfo.emptySquares;

  const Bitboard moves = Precomputed::KingMoves.at(index.index);

  // const Bitboard castling = getCastlingMoves(index, isWhite);
  const Bitboard castling = 0;

  return (moves & validSquares) | castling;
}

Bitboard Board::getCastlingMoves(const BoardIndex& index, bool isWhite) const {
  // const Bitboard whitePieces = getWhitePieces();
  // const Bitboard blackPieces = getBlackPieces();
  // const Bitboard allPieces = whitePieces | blackPieces;
  // const Bitboard emptySquares = ~allPieces;

  // const size_t row = index / 8;
  // const size_t col = index % 8;

  //// 112 = 01110000
  // const Bitboard leftMustBeFree = isWhite ? 112ULL : 112ULL << (8 * 7);
  //// 6 = 00000110
  // const Bitboard rightMustBeFree = isWhite ? 6ULL : 6ULL << (8 * 7);

  // return leftMustBeFree | rightMustBeFree;
  return 0;
}

void Board::addPiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitboards.whitePawns.enableBit(index);
    break;
  case BlackPawn.bits:
    m_bitboards.blackPawns.enableBit(index);
    break;
  case WhiteKnight.bits:
    m_bitboards.whiteKnights.enableBit(index);
    break;
  case BlackKnight.bits:
    m_bitboards.blackKnights.enableBit(index);
    break;
  case WhiteBishop.bits:
    m_bitboards.whiteBishops.enableBit(index);
    break;
  case BlackBishop.bits:
    m_bitboards.blackBishops.enableBit(index);
    break;
  case WhiteRook.bits:
    m_bitboards.whiteRooks.enableBit(index);
    break;
  case BlackRook.bits:
    m_bitboards.blackRooks.enableBit(index);
    break;
  case WhiteQueen.bits:
    m_bitboards.whiteQueens.enableBit(index);
    break;
  case BlackQueen.bits:
    m_bitboards.blackQueens.enableBit(index);
    break;
  case WhiteKing.bits:
    m_bitboards.whiteKing.enableBit(index);
    break;
  case BlackKing.bits:
    m_bitboards.blackKing.enableBit(index);
    break;
  default:
    return;
  }
}

void Board::removePiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.bits) {
  case WhitePawn.bits:
    m_bitboards.whitePawns.disableBit(index);
    break;
  case BlackPawn.bits:
    m_bitboards.blackPawns.disableBit(index);
    break;
  case WhiteKnight.bits:
    m_bitboards.whiteKnights.disableBit(index);
    break;
  case BlackKnight.bits:
    m_bitboards.blackKnights.disableBit(index);
    break;
  case WhiteBishop.bits:
    m_bitboards.whiteBishops.disableBit(index);
    break;
  case BlackBishop.bits:
    m_bitboards.blackBishops.disableBit(index);
    break;
  case WhiteRook.bits:
    m_bitboards.whiteRooks.disableBit(index);
    break;
  case BlackRook.bits:
    m_bitboards.blackRooks.disableBit(index);
    break;
  case WhiteQueen.bits:
    m_bitboards.whiteQueens.disableBit(index);
    break;
  case BlackQueen.bits:
    m_bitboards.blackQueens.disableBit(index);
    break;
  case WhiteKing.bits:
    m_bitboards.whiteKing.disableBit(index);
    break;
  case BlackKing.bits:
    m_bitboards.blackKing.disableBit(index);
    break;
  default:
    return;
  }
}

void Board::updateBitboards(const Move& move) {
  Piece pieceToAdd = move.movedPiece;
  Piece pieceToRemove = move.movedPiece;

  const bool isWhite = move.movedPiece.isWhite();

  // Pawn moves
  if (move.movedPiece.type() == Pawn) {
    Bitboard& enPassant = isWhite ? m_bitboards.whiteEnPassant : m_bitboards.blackEnPassant;

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
      Bitboard& opponentEnPassant =
          isWhite ? m_bitboards.blackEnPassant : m_bitboards.whiteEnPassant;

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
