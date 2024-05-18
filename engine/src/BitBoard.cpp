#include "Bitboard.hpp"

namespace Engine_NS {

Bitboard Bitboards::getWhitePieces() const {
  return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
}
Bitboard Bitboards::getBlackPieces() const {
  return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
}

BoardInfo Bitboards::getInfo() const {
  BoardInfo info = {};
  info.whitePieces = getWhitePieces();
  info.blackPieces = getBlackPieces();
  info.allPieces = info.whitePieces | info.blackPieces;
  info.emptySquares = ~info.allPieces;
  return info;
}

} // namespace Engine_NS
