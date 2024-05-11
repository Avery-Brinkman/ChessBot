#include "BitBoard.h"

namespace Engine_NS {

inline BitBoard getBitBoard(BoardIndex index) { return 1ULL << index; }

inline void enableBit(BitBoard& board, BoardIndex index) { board |= (1ULL << index); }
inline void disableBit(BitBoard& board, BoardIndex index) { board &= ~(1ULL << index); }
inline void toggleBit(BitBoard& board, BoardIndex index) { board ^= (1ULL << index); }
inline bool checkBit(BitBoard board, BoardIndex index) { return board & (1ULL << index); }

inline BitBoard BitBoards::getWhitePieces() const {
  return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
}
inline BitBoard BitBoards::getBlackPieces() const {
  return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
}

BoardInfo BitBoards::getInfo() const {
  BoardInfo info = {};
  info.whitePieces = getWhitePieces();
  info.blackPieces = getBlackPieces();
  info.allPieces = info.whitePieces | info.blackPieces;
  info.emptySquares = ~info.allPieces;
  return info;
}

} // namespace Engine_NS