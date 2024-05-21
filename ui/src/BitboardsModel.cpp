#include "BitboardsModel.h"

namespace Chess_UI {

BitboardsModel::BitboardsModel(QObject* parent) : QAbstractListModel(parent) {
  m_names = {
      {WhitePawns, "White Pawns"},          {WhiteKnights, "White Knights"},
      {WhiteBishops, "White Bishops"},      {WhiteRooks, "White Rooks"},
      {WhiteQueens, "White Queens"},        {WhiteKing, "White King"},
      {BlackPawns, "Black Pawns"},          {BlackKnights, "Black Knights"},
      {BlackBishops, "Black Bishops"},      {BlackRooks, "Black Rooks"},
      {BlackQueens, "Black Queens"},        {BlackKing, "Black King"},
      {WhiteEnPassant, "White En Passant"}, {BlackEnPassant, "Black En Passant"},
  };
}

void BitboardsModel::updateBoards(const Engine_NS::Bitboards& newBoards) {
  m_boards = newBoards;
  emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0),
                   {static_cast<int>(BitboardsRoles::BitsRole)});
}

int BitboardsModel::rowCount(const QModelIndex& parent) const { return m_names.size(); }

QVariant BitboardsModel::data(const QModelIndex& index, int role) const {
  using enum Chess_UI::BitboardsModel::BitboardsRoles;

  if (!checkIndex(index))
    return QVariant();

  if (role == static_cast<int>(NameRole)) {
    return m_names.value(index.row());
  } else if (role == static_cast<int>(BitsRole)) {
    return format(getBits(index.row()));
  }

  return QVariant();
}

QHash<int, QByteArray> BitboardsModel::roleNames() const {
  using enum Chess_UI::BitboardsModel::BitboardsRoles;
  return {
      {static_cast<int>(NameRole), "name"},
      {static_cast<int>(BitsRole), "bits"},
  };
}

Engine_NS::Bitboard BitboardsModel::getBits(int row) const {
  using enum BitboardRows;

  switch (BitboardRows(row)) {
  case WhitePawns:
    return m_boards.pawns[1];
  case WhiteKnights:
    return m_boards.knights[1];
  case WhiteBishops:
    return m_boards.bishops[1];
  case WhiteRooks:
    return m_boards.rooks[1];
  case WhiteQueens:
    return m_boards.queens[1];
  case WhiteKing:
    return m_boards.kings[1];
  case BlackPawns:
    return m_boards.pawns[0];
  case BlackKnights:
    return m_boards.knights[0];
  case BlackBishops:
    return m_boards.bishops[0];
  case BlackRooks:
    return m_boards.rooks[0];
  case BlackQueens:
    return m_boards.queens[0];
  case BlackKing:
    return m_boards.kings[0];
  case WhiteEnPassant:
    return m_boards.enPassant[1];
  case BlackEnPassant:
    return m_boards.enPassant[0];
  default:
    return 0;
  }
}

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
