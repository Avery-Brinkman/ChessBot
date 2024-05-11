#include "BoardModel.h"

#include <QColor>
#include <QUrl>
#include <bitset>
#include <iostream>

namespace Chess_UI {
BoardModel::BoardModel(QObject* parent) : QAbstractTableModel(parent) {
  setToStartPosition();

  QObject::connect(m_debugPanel, &DebugPanel::updateBoard, this,
                   [this]() { emit dataChanged(createIndex(0, 0), createIndex(7, 7)); });
}

int BoardModel::rowCount(const QModelIndex& parent) const { return 8; }
int BoardModel::columnCount(const QModelIndex& parent) const { return 8; }

QVariant BoardModel::data(const QModelIndex& index, int role) const {
  using enum Chess_UI::BoardModel::BoardRoles;

  const size_t bitIndex = getIndex(index.row(), index.column());

  switch (BoardRoles(role)) {
  case ImageRole:
    return pieceImage(getPiece(index.row(), index.column()));
  case ValidMoveRole: {
    const bool valid = m_currentValidMoves & (1ULL << bitIndex);
    return valid;
  }
  case SelectedRole: {
    if (m_selectedIndex == -1)
      return false;
    return m_selectedIndex == bitIndex;
  }
  case DebugInfoRole: {
    if (!m_debugPanel->getDebugEnabled())
      return false;

    return getDebugInfo() & (1ULL << bitIndex);
  }
  case BitBoardRole: {
    return (m_bitBoard & (1ULL << bitIndex)) > 0;
  }
  case RankAndFileRole:
    return QString::fromStdString(Engine_NS::to_string(getIndex(index.row(), index.column())));
  default:
    return QVariant();
  }
}

bool BoardModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const Engine_NS::BoardIndex bitIndex = getIndex(index.row(), index.column());
  switch (BoardRoles(role)) {
  case SelectedRole: {
    m_selectedIndex = bitIndex;
    m_currentValidMoves = getValidMoves(bitIndex);
    break;
  }
  case ValidMoveRole: {
    if (m_currentValidMoves & (1ULL << bitIndex)) {
      movePiece(m_selectedIndex, bitIndex);
    }
    m_selectedIndex = Engine_NS::BoardIndex::INVALID;
    m_currentValidMoves = 0;
    break;
  }
  case BitBoardRole: {
    // Toggle the bit
    m_bitBoard ^= (1ULL << bitIndex);

    std::cout << std::bitset<64>(m_bitBoard) << std::endl;

    break;
  }
  default:
    return false;
  }

  emit dataChanged(createIndex(0, 0), createIndex(7, 7));

  return true;
}

QHash<int, QByteArray> BoardModel::roleNames() const {
  using enum Chess_UI::BoardModel::BoardRoles;
  return {
      {static_cast<int>(ImageRole), "pieceImage"},
      {static_cast<int>(ValidMoveRole), "validMove"},
      {static_cast<int>(SelectedRole), "selected"},
      {static_cast<int>(DebugInfoRole), "debugInfo"},
      {static_cast<int>(BitBoardRole), "bitBoard"},
      {static_cast<int>(RankAndFileRole), "rankAndFile"},
  };
}

DebugPanel* BoardModel::getDebugPanel() const { return m_debugPanel; }

QUrl BoardModel::pieceImage(Piece piece) const {
  using namespace Engine_NS::Pieces;
  switch (piece) {
  case WhitePawn:
    return QUrl("qrc:/images/whitePawn.png");
  case WhiteKnight:
    return QUrl("qrc:/images/whiteKnight.png");
  case WhiteBishop:
    return QUrl("qrc:/images/whiteBishop.png");
  case WhiteRook:
    return QUrl("qrc:/images/whiteRook.png");
  case WhiteQueen:
    return QUrl("qrc:/images/whiteQueen.png");
  case WhiteKing:
    return QUrl("qrc:/images/whiteKing.png");
  case BlackPawn:
    return QUrl("qrc:/images/blackPawn.png");
  case BlackKnight:
    return QUrl("qrc:/images/blackKnight.png");
  case BlackBishop:
    return QUrl("qrc:/images/blackBishop.png");
  case BlackRook:
    return QUrl("qrc:/images/blackRook.png");
  case BlackQueen:
    return QUrl("qrc:/images/blackQueen.png");
  case BlackKing:
    return QUrl("qrc:/images/blackKing.png");
  default:
    return QUrl("");
  }
}

BitBoard BoardModel::getDebugInfo() const {
  BitBoard debugInfo = 0;
  if (m_debugPanel->getEnPassant())
    debugInfo |= getEnPassantMask();

  return debugInfo;
}

} // namespace Chess_UI
