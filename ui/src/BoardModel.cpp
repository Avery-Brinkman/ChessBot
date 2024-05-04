#include "BoardModel.h"

#include <QColor>
#include <QUrl>

namespace Chess_UI {
BoardModel::BoardModel(QObject* parent) : QAbstractTableModel(parent) { setToStartPosition(); }

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
  default:
    return QVariant();
  }
}

bool BoardModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const size_t bitIndex = getIndex(index.row(), index.column());
  switch (BoardRoles(role)) {
  case SelectedRole: {
    m_selectedIndex = static_cast<int>(bitIndex);
    m_currentValidMoves = getValidMoves(bitIndex);
    break;
  }
  case MoveRole: {
    if (m_currentValidMoves & (1ULL << bitIndex)) {
      movePiece(m_selectedIndex, bitIndex);
      m_selectedIndex = -1;
      m_currentValidMoves = 0;
    }
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
      {static_cast<int>(ValidMoveRole), "hasValidMove"},
      {static_cast<int>(SelectedRole), "selected"},
      {static_cast<int>(MoveRole), "moveHere"},
  };
}

QUrl BoardModel::pieceImage(Piece piece) const {
  switch (piece) {
  case Engine_NS::WhitePawn:
    return QUrl("qrc:/images/whitePawn.png");
  case Engine_NS::WhiteKnight:
    return QUrl("qrc:/images/whiteKnight.png");
  case Engine_NS::WhiteBishop:
    return QUrl("qrc:/images/whiteBishop.png");
  case Engine_NS::WhiteRook:
    return QUrl("qrc:/images/whiteRook.png");
  case Engine_NS::WhiteQueen:
    return QUrl("qrc:/images/whiteQueen.png");
  case Engine_NS::WhiteKing:
    return QUrl("qrc:/images/whiteKing.png");
  case Engine_NS::BlackPawn:
    return QUrl("qrc:/images/blackPawn.png");
  case Engine_NS::BlackKnight:
    return QUrl("qrc:/images/blackKnight.png");
  case Engine_NS::BlackBishop:
    return QUrl("qrc:/images/blackBishop.png");
  case Engine_NS::BlackRook:
    return QUrl("qrc:/images/blackRook.png");
  case Engine_NS::BlackQueen:
    return QUrl("qrc:/images/blackQueen.png");
  case Engine_NS::BlackKing:
    return QUrl("qrc:/images/blackKing.png");
  default:
    return QUrl("");
  }
}

} // namespace Chess_UI
