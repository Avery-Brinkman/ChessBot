#include "BitBoard.hpp"

namespace Engine_NS {

BitBoard BitBoards::getWhitePieces() const {
  return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
}
BitBoard BitBoards::getBlackPieces() const {
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
