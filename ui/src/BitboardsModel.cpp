#include "BitboardsModel.h"

namespace Chess_UI {

BitboardsModel::BitboardsModel(QObject* parent) : QObject(parent) {}

void BitboardsModel::updateBoards(const Engine_NS::Bitboards& newBoards) {
  m_boards = newBoards;
  emit dataChanged();
}

QString BitboardsModel::getWhitePawns() const { return format(m_boards.pawns[1]); }
QString BitboardsModel::getWhiteKnights() const { return format(m_boards.knights[1]); }
QString BitboardsModel::getWhiteBishops() const { return format(m_boards.bishops[1]); }
QString BitboardsModel::getWhiteRooks() const { return format(m_boards.rooks[1]); }
QString BitboardsModel::getWhiteQueens() const { return format(m_boards.queens[1]); }
QString BitboardsModel::getWhiteKing() const { return format(m_boards.kings[1]); }
QString BitboardsModel::getBlackPawns() const { return format(m_boards.pawns[0]); }
QString BitboardsModel::getBlackKnights() const { return format(m_boards.knights[0]); }
QString BitboardsModel::getBlackBishops() const { return format(m_boards.bishops[0]); }
QString BitboardsModel::getBlackRooks() const { return format(m_boards.rooks[0]); }
QString BitboardsModel::getBlackQueens() const { return format(m_boards.queens[0]); }
QString BitboardsModel::getBlackKing() const { return format(m_boards.kings[0]); }

QString BitboardsModel::getWhiteEnPassant() const { return format(m_boards.enPassant[1]); }
QString BitboardsModel::getBlackEnPassant() const { return format(m_boards.enPassant[0]); }

QString BitboardsModel::format(const Engine_NS::Bitboard& bits) {
  QString out = "";
  for (const QChar& c : QString("%1").arg(bits.bits, 64, 2, QChar('0'))) {
    if (c == '0')
      out += "<font color='blue'>";
    else
      out += "<font color='red'>";
    out += c;
    out += "</font>";
  }
  return out;
}

} // namespace Chess_UI
