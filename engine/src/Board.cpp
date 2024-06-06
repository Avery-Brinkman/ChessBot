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
  // White
  m_bitboards.pawns[1] = 0b0000000000000000000000000000000000000000000000001111111100000000;
  m_bitboards.rooks[1] = 0b0000000000000000000000000000000000000000000000000000000010000001;
  m_bitboards.knights[1] = 0b0000000000000000000000000000000000000000000000000000000001000010;
  m_bitboards.bishops[1] = 0b0000000000000000000000000000000000000000000000000000000000100100;
  m_bitboards.queens[1] = 0b0000000000000000000000000000000000000000000000000000000000001000;
  m_bitboards.kings[1] = 0b0000000000000000000000000000000000000000000000000000000000010000;

  // Black
  m_bitboards.pawns[0] = 0b0000000011111111000000000000000000000000000000000000000000000000;
  m_bitboards.rooks[0] = 0b1000000100000000000000000000000000000000000000000000000000000000;
  m_bitboards.knights[0] = 0b0100001000000000000000000000000000000000000000000000000000000000;
  m_bitboards.bishops[0] = 0b0010010000000000000000000000000000000000000000000000000000000000;
  m_bitboards.queens[0] = 0b0000100000000000000000000000000000000000000000000000000000000000;
  m_bitboards.kings[0] = 0b0001000000000000000000000000000000000000000000000000000000000000;

  m_boardInfo = m_bitboards.getInfo();
}

BoardIndex Board::getIndex(const size_t& row, const size_t& col) const {
  return BoardIndex((7 - row) * 8 + col);
}

Piece Board::getPiece(const size_t& row, const size_t& col) const {
  if (row > 7 || col > 7)
    return Pieces::None;
  return getPiece(getIndex(row, col));
}
Piece Board::getPiece(const BoardIndex& index) const {
  using namespace Engine_NS::Pieces;

  if (m_bitboards.pawns[1].checkBit(index))
    return WhitePawn;
  if (m_bitboards.rooks[1].checkBit(index))
    return WhiteRook;
  if (m_bitboards.knights[1].checkBit(index))
    return WhiteKnight;
  if (m_bitboards.bishops[1].checkBit(index))
    return WhiteBishop;
  if (m_bitboards.queens[1].checkBit(index))
    return WhiteQueen;
  if (m_bitboards.kings[1].checkBit(index))
    return WhiteKing;
  if (m_bitboards.pawns[0].checkBit(index))
    return BlackPawn;
  if (m_bitboards.rooks[0].checkBit(index))
    return BlackRook;
  if (m_bitboards.knights[0].checkBit(index))
    return BlackKnight;
  if (m_bitboards.bishops[0].checkBit(index))
    return BlackBishop;
  if (m_bitboards.queens[0].checkBit(index))
    return BlackQueen;
  if (m_bitboards.kings[0].checkBit(index))
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
    return {};
  }
}

void Board::movePiece(const BoardIndex& from, const BoardIndex& to) {
  Move move{};
  move.startPos = from;
  move.endPos = to;
  move.movedPiece = getPiece(from);

  move.capturedPiece = getPiece(to);
  if (move.capturedPiece != Pieces::None) {
    move.capturedPos = to;
  }

  MoveFlags flags{};

  if (move.movedPiece.type() == Pawn) {
    const bool isWhite = move.movedPiece.isWhite();
    const Bitboard& opponentEnPassant = m_bitboards.enPassant[!isWhite];

    // Double pawn push
    if (abs(from.rank() - to.rank()) > 1) {
      flags = DoublePawnPush;
    }
    // En passant capture
    else if (opponentEnPassant.checkBit(to)) {
      flags = EnPassant;

      move.capturedPos = to + move.movedPiece.backward();
      move.capturedPiece = getPiece(move.capturedPos);
    }
    // Promotion
    else if (to.rank() == Rank(1) || to.rank() == Rank(8)) {
      flags = QueenPromotion;
    }
  }

  move.flags = flags;

  updateBitboards(move);
}

// Protected Functions

const Bitboards& Board::getBitboards() const { return m_bitboards; }

void Board::togglePiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.type()) {
  case Pawn:
    m_bitboards.pawns[piece.isWhite()].toggleBit(index);
    break;
  case Knight:
    m_bitboards.knights[piece.isWhite()].toggleBit(index);
    break;
  case Bishop:
    m_bitboards.bishops[piece.isWhite()].toggleBit(index);
    break;
  case Rook:
    m_bitboards.rooks[piece.isWhite()].toggleBit(index);
    break;
  case Queen:
    m_bitboards.queens[piece.isWhite()].toggleBit(index);
    break;
  case King:
    m_bitboards.kings[piece.isWhite()].toggleBit(index);
    break;
  default:
    return;
  }
}

// Private Functions

Bitboard Board::getWhitePieces() const { return m_bitboards.getWhitePieces(); }

Bitboard Board::getBlackPieces() const { return m_bitboards.getBlackPieces(); }

Bitboard Board::getValidPawnMoves(const BoardIndex& index) const {
  const Piece piece = getPiece(index);
  const bool isWhite = piece.isWhite();
  const Bitboard opponentPieces = isWhite ? m_boardInfo.blackPieces : m_boardInfo.whitePieces;

  const Bitboard moves = Engine_NS::Precomputed::PawnMoves.at(isWhite).at(index.index);
  const Bitboard attacks = Engine_NS::Precomputed::PawnAttacks.at(isWhite).at(index.index);

  return (moves & m_boardInfo.emptySquares) |
         (attacks & (opponentPieces | m_bitboards.enPassant[!isWhite]));
}

Bitboard Board::getValidKnightMoves(const BoardIndex& index) const {
  const bool isWhite = m_boardInfo.whitePieces.checkBit(index);
  const Bitboard opponentPieces = isWhite ? m_boardInfo.blackPieces : m_boardInfo.whitePieces;
  const Bitboard validSquares = opponentPieces | m_boardInfo.emptySquares;

  return Precomputed::KnightMoves.at(index.index) & validSquares;
}

Bitboard Board::getValidBishopMoves(const BoardIndex& index) const {
  Bitboard validMoves = {};
  rayCast(index, validMoves, NorthEast);
  rayCast(index, validMoves, SouthEast);
  rayCast(index, validMoves, SouthWest);
  rayCast(index, validMoves, NorthWest);

  validMoves.disableBits(getPiece(index).isWhite() ? m_bitboards.getWhitePieces()
                                                   : m_bitboards.getBlackPieces());

  return validMoves;
}

Bitboard Board::getValidRookMoves(const BoardIndex& index) const {
  Bitboard validMoves = {};
  rayCast(index, validMoves, North);
  rayCast(index, validMoves, East);
  rayCast(index, validMoves, South);
  rayCast(index, validMoves, West);

  validMoves.disableBits(getPiece(index).isWhite() ? m_bitboards.getWhitePieces()
                                                   : m_bitboards.getBlackPieces());

  return validMoves;
}

Bitboard Board::getValidQueenMoves(const BoardIndex& index) const {
  return getValidBishopMoves(index) | getValidRookMoves(index);
}

Bitboard Board::getValidKingMoves(const BoardIndex& index) const {
  const bool isWhite = m_boardInfo.whitePieces.checkBit(index);
  const Bitboard opponentPieces = isWhite ? m_boardInfo.blackPieces : m_boardInfo.whitePieces;
  const Bitboard validSquares = opponentPieces | m_boardInfo.emptySquares;

  const Bitboard moves = Precomputed::KingMoves.at(index.index);

  // const Bitboard castling = getCastlingMoves(index, isWhite);
  const Bitboard castling{};

  return (moves & validSquares) | castling;
}

Bitboard Board::getCastlingMoves(const BoardIndex& index, const bool& isWhite) const {
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
  return {};
}

void Board::addPiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.type()) {
  case Pawn:
    m_bitboards.pawns[piece.isWhite()].enableBit(index);
    break;
  case Knight:
    m_bitboards.knights[piece.isWhite()].enableBit(index);
    break;
  case Bishop:
    m_bitboards.bishops[piece.isWhite()].enableBit(index);
    break;
  case Rook:
    m_bitboards.rooks[piece.isWhite()].enableBit(index);
    break;
  case Queen:
    m_bitboards.queens[piece.isWhite()].enableBit(index);
    break;
  case King:
    m_bitboards.kings[piece.isWhite()].enableBit(index);
    break;
  default:
    return;
  }

  m_boardInfo = m_bitboards.getInfo();
}

void Board::removePiece(const Piece& piece, const BoardIndex& index) {
  switch (piece.type()) {
  case Pawn:
    m_bitboards.pawns[piece.isWhite()].disableBit(index);
    break;
  case Knight:
    m_bitboards.knights[piece.isWhite()].disableBit(index);
    break;
  case Bishop:
    m_bitboards.bishops[piece.isWhite()].disableBit(index);
    break;
  case Rook:
    m_bitboards.rooks[piece.isWhite()].disableBit(index);
    break;
  case Queen:
    m_bitboards.queens[piece.isWhite()].disableBit(index);
    break;
  case King:
    m_bitboards.kings[piece.isWhite()].disableBit(index);
    break;
  default:
    return;
  }

  m_boardInfo = m_bitboards.getInfo();
}

void Board::updateBitboards(const Move& move) {
  Piece pieceToAdd = move.movedPiece;
  Piece pieceToRemove = move.movedPiece;

  const bool isWhite = move.movedPiece.isWhite();

  // Pawn moves
  if (move.movedPiece.type() == Pawn) {
    Bitboard& enPassant = m_bitboards.enPassant[isWhite];

    // Moved two steps
    if (move.flags == DoublePawnPush) {
      enPassant.enableBit(BoardIndex(std::midpoint(static_cast<unsigned char>(move.startPos.index),
                                                   static_cast<unsigned char>(move.endPos.index))));
    }
    // Clear en passant
    else {
      const BoardIndex clearLoc =
          move.flags == EnPassant ? move.endPos : move.startPos + move.movedPiece.backward();
      enPassant.disableBit(clearLoc);
    }

    // TODO: Allow for promotion to other pieces
    // Check for promotion
    if ((move.flags & QueenPromotion) == QueenPromotion) {
      pieceToAdd = Piece(pieceToAdd.color(), Queen);
    }
  }

  // Remove En Passant when a pawn is captured
  if ((move.capturedPiece.type() == Pawn) &&
      (move.capturedPos.rank() == 4 || move.capturedPos.rank() == 5)) {
    Bitboard& opponentEnPassant = m_bitboards.enPassant[!isWhite];

    opponentEnPassant.disableBit(move.capturedPos + move.capturedPiece.backward());
  }

  addPiece(pieceToAdd, move.endPos);
  removePiece(pieceToRemove, move.startPos);

  if (move.capturedPiece != Pieces::None) {
    removePiece(move.capturedPiece, move.capturedPos);
  }

  m_boardInfo = m_bitboards.getInfo();
}

void Board::rayCast(const Engine_NS::BoardIndex& index, Engine_NS::Bitboard& moves,
                    const Engine_NS::CompassDirection& direction) const {
  BoardIndex currentIndex = index;

  while (true) {
    if ((direction == North || direction == NorthEast || direction == NorthWest) &&
        (currentIndex.rank() >= 8))
      return;
    if ((direction == East || direction == NorthEast || direction == SouthEast) &&
        (currentIndex.file() >= File::H))
      return;
    if ((direction == South || direction == SouthEast || direction == SouthWest) &&
        (currentIndex.rank() <= 1))
      return;
    if ((direction == West || direction == NorthWest || direction == SouthWest) &&
        (currentIndex.file() <= File::A))
      return;
    currentIndex = currentIndex + direction;
    moves.enableBit(currentIndex);

    if (m_boardInfo.allPieces.checkBit(currentIndex))
      return;
  }
}

} // namespace Engine_NS
