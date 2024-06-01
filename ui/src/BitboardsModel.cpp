#include "BitboardsModel.h"

#include <QClipboard>
#include <QGuiApplication>
#include <iostream>

namespace Chess_UI {

BitboardsModel::BitboardsModel(QObject* parent) : QAbstractListModel(parent) {
  QObject::connect(this, &BitboardsModel::showAsBinChanged, this, [this]() {
    if (!m_showAsBin)
      return;

    emit customValueChanged();
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0),
                     {static_cast<int>(BitboardsRoles::BitsRole)});
  });
  QObject::connect(this, &BitboardsModel::showAsDecChanged, this, [this]() {
    if (!m_showAsDec)
      return;

    emit customValueChanged();
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0),
                     {static_cast<int>(BitboardsRoles::BitsRole)});
  });
  QObject::connect(this, &BitboardsModel::showAsHexChanged, this, [this]() {
    if (!m_showAsHex)
      return;

    emit customValueChanged();
    emit dataChanged(createIndex(0, 0), createIndex(rowCount() - 1, 0),
                     {static_cast<int>(BitboardsRoles::BitsRole)});
  });

  m_names = {
      {WhitePawns, "White Pawns"},          {WhiteKnights, "White Knights"},
      {WhiteBishops, "White Bishops"},      {WhiteRooks, "White Rooks"},
      {WhiteQueens, "White Queens"},        {WhiteKing, "White King"},
      {BlackPawns, "Black Pawns"},          {BlackKnights, "Black Knights"},
      {BlackBishops, "Black Bishops"},      {BlackRooks, "Black Rooks"},
      {BlackQueens, "Black Queens"},        {BlackKing, "Black King"},
      {WhiteEnPassant, "White En Passant"}, {BlackEnPassant, "Black En Passant"},
  };

  m_enabled = QList<bool>(rowCount(), false);
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
  } else if (role == static_cast<int>(EnabledRole)) {
    return m_enabled.at(index.row());
  }

  return QVariant();
}

bool BitboardsModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  using enum Chess_UI::BitboardsModel::BitboardsRoles;
  if (!checkIndex(index))
    return false;

  if (role == static_cast<int>(EnabledRole)) {
    const bool currentVal = m_enabled.at(index.row());
    const bool newVal = value.toBool();

    if (currentVal != newVal) {
      m_enabled[index.row()] = newVal;
      emit dataChanged(index, index, {static_cast<int>(EnabledRole)});

      updateDebugBits();

      return true;
    }
  }

  return false;
}

QHash<int, QByteArray> BitboardsModel::roleNames() const {
  using enum Chess_UI::BitboardsModel::BitboardsRoles;
  return {
      {static_cast<int>(NameRole), "name"},
      {static_cast<int>(BitsRole), "bits"},
      {static_cast<int>(EnabledRole), "isEnabled"},
  };
}

Engine_NS::Bitboard BitboardsModel::getDebugBits() const {
  if (m_useCustomValue)
    return getCustomBits();

  return m_debugBits;
}

Engine_NS::Bitboard BitboardsModel::getCustomBits() const { return m_customBits; }

QString BitboardsModel::getCustomValue() const { return format(getCustomBits(), false); }

void BitboardsModel::setCustomValue(const QString& customVal) {
  Engine_NS::Bitboard newBoard = format(customVal);
  if (m_customBits.bits != newBoard.bits) {
    m_customBits = newBoard;
    emit customValueChanged();
  }
}

void BitboardsModel::updateDebugBits() {
  if (m_useCustomValue)
    return;

  const Engine_NS::Bitboard initialVal = m_debugBits;
  m_debugBits = 0;

  for (int row = 0; row < rowCount(); row++) {
    if (m_enabled.at(row))
      m_debugBits.enableBits(getBits(row));
  }

  if (m_debugBits.bits != initialVal.bits)
    emit debugBitsChanged();
}

void BitboardsModel::copyBitsToClipboard(int row) const {
  QGuiApplication::clipboard()->setText(format(getBits(row), false));
}

void BitboardsModel::pasteBitsFromClipboard(int row) {
  // TODO: Support pasting
}

void BitboardsModel::toggleCustomBit(Engine_NS::BoardIndex index) {
  m_customBits.toggleBit(index);
  emit customValueChanged();
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

Engine_NS::Bitboard BitboardsModel::format(const QString& bits) const {
  if (m_showAsBin)
    return binFormat(bits);
  if (m_showAsHex)
    return hexFormat(bits);

  return bits.toULongLong();
}

Engine_NS::Bitboard BitboardsModel::binFormat(const QString& bits) {
  return bits.toULongLong(nullptr, 2);
}

Engine_NS::Bitboard BitboardsModel::hexFormat(const QString& bits) {
  return bits.sliced(2).toULongLong(nullptr, 16);
}

QString BitboardsModel::format(const Engine_NS::Bitboard& bits, bool colors) const {
  if (m_showAsBin)
    return binFormat(bits, colors);
  if (m_showAsHex)
    return hexFormat(bits);

  return QString::number(bits.bits);
}

QString BitboardsModel::binFormat(const Engine_NS::Bitboard& bits, bool colors) {
  if (!colors)
    return QString("%1").arg(bits.bits, 64, 2, QChar('0'));

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

QString BitboardsModel::hexFormat(const Engine_NS::Bitboard& bits) {
  return "0x" + QString("%1").arg(bits.bits, 16, 16, QChar('0'));
}

} // namespace Chess_UI
