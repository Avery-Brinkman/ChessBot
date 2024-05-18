#include "Bitboard.hpp"

namespace Engine_NS {

Bitboard Bitboards::getWhitePieces() const {
  return pawns[1] | rooks[1] | knights[1] | bishops[1] | queens[1] | kings[1]
}
Bitboard Bitboards::getBlackPieces() const {
  return pawns[0] | rooks[0] | knights[0] | bishops[0] | queens[0] | kings[0];
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
