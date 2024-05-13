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

  const Engine_NS::BoardIndex bitIndex = getIndex(index.row(), index.column());

  switch (BoardRoles(role)) {
  case ImageRole:
    return pieceImage(getPiece(index.row(), index.column()));
  case ValidMoveRole: {
    const bool valid = m_currentValidMoves.checkBit(bitIndex);
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

    return getDebugInfo().checkBit(bitIndex);
  }
  case BitBoardRole: {
    return m_bitBoard.checkBit(bitIndex);
  }
  case RankAndFileRole:
    return QString::fromStdString(getIndex(index.row(), index.column()).toString());
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
    if (m_currentValidMoves.checkBit(bitIndex)) {
      movePiece(m_selectedIndex, bitIndex);
    }
    m_selectedIndex = {};
    m_currentValidMoves = 0;
    break;
  }
  case BitBoardRole: {
    // Toggle the bit
    m_bitBoard.toggleBit(bitIndex);

    std::cout << std::bitset<64>(m_bitBoard.bits) << std::endl;

    break;
  }
  case TogglePieceRole: {
    if (value.toBool()) {
      togglePiece(Engine_NS::Piece(m_debugPanel->getPieceType() | Engine_NS::Color::White),
                  bitIndex);
    } else {
      togglePiece(Engine_NS::Piece(m_debugPanel->getPieceType() | Engine_NS::Color::Black),
                  bitIndex);
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
  return {{static_cast<int>(ImageRole), "pieceImage"},
          {static_cast<int>(ValidMoveRole), "validMove"},
          {static_cast<int>(SelectedRole), "selected"},
          {static_cast<int>(DebugInfoRole), "debugInfo"},
          {static_cast<int>(BitBoardRole), "bitBoard"},
          {static_cast<int>(RankAndFileRole), "rankAndFile"},
          {static_cast<int>(TogglePieceRole), "togglePiece"}};
}

DebugPanel* BoardModel::getDebugPanel() const { return m_debugPanel; }

QUrl BoardModel::pieceImage(const Engine_NS::Piece& piece) const {
  using namespace Engine_NS;
  switch (piece.bits) {
  case WhitePawn.bits:
    return QUrl("qrc:/images/whitePawn.png");
  case WhiteKnight.bits:
    return QUrl("qrc:/images/whiteKnight.png");
  case WhiteBishop.bits:
    return QUrl("qrc:/images/whiteBishop.png");
  case WhiteRook.bits:
    return QUrl("qrc:/images/whiteRook.png");
  case WhiteQueen.bits:
    return QUrl("qrc:/images/whiteQueen.png");
  case WhiteKing.bits:
    return QUrl("qrc:/images/whiteKing.png");
  case BlackPawn.bits:
    return QUrl("qrc:/images/blackPawn.png");
  case BlackKnight.bits:
    return QUrl("qrc:/images/blackKnight.png");
  case BlackBishop.bits:
    return QUrl("qrc:/images/blackBishop.png");
  case BlackRook.bits:
    return QUrl("qrc:/images/blackRook.png");
  case BlackQueen.bits:
    return QUrl("qrc:/images/blackQueen.png");
  case BlackKing.bits:
    return QUrl("qrc:/images/blackKing.png");
  default:
    return QUrl("");
  }
}

Engine_NS::BitBoard BoardModel::getDebugInfo() const {
  Engine_NS::BitBoard debugInfo = 0;
  if (m_debugPanel->getEnPassant())
    debugInfo.enableBits(getEnPassantMask());

  return debugInfo;
}

} // namespace Chess_UI
