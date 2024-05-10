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
  case ValidCaptureRole: {
    const bool valid = m_currentValidCaptures & (1ULL << bitIndex);
    return valid;
  }
  case SelectedRole: {
    if (m_selectedIndex == -1)
      return false;
    return m_selectedIndex == bitIndex;
  }
  case DebugInfoRole: {
    if (!m_debugEnabled)
      return false;

    return getDebugInfo() & (1ULL << bitIndex);
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
    m_currentValidCaptures = getValidCaptures(bitIndex);
    break;
  }
  case ValidMoveRole:
  case ValidCaptureRole: {
    if ((m_currentValidCaptures | m_currentValidMoves) & (1ULL << bitIndex)) {
      movePiece(m_selectedIndex, bitIndex);
    }
    m_selectedIndex = -1;
    m_currentValidMoves = 0;
    m_currentValidCaptures = 0;
    break;
  }
  default:
    return false;
  }

  emit dataChanged(createIndex(0, 0), createIndex(7, 7));

  return true;
}

bool BoardModel::getDebugEnabled() const { return m_debugEnabled; }
void BoardModel::setDebugEnabled(bool debugEnabled) {
  if (m_debugEnabled == debugEnabled)
    return;
  m_debugEnabled = debugEnabled;
  emit debugEnabledChanged();

  if (!debugEnabled) {
    setShowWhite(false);
    setShowBlack(false);
    setEnPassant(false);
  }

  emit dataChanged(createIndex(0, 0), createIndex(7, 7),
                   {static_cast<int>(BoardRoles::DebugInfoRole)});
}

bool BoardModel::getShowWhite() const { return m_showWhite; }
void BoardModel::setShowWhite(bool showWhite) {
  if (m_showWhite == showWhite)
    return;
  m_showWhite = showWhite;
  emit showWhiteChanged();

  if (showWhite)
    setDebugEnabled(true);

  emit dataChanged(createIndex(0, 0), createIndex(7, 7),
                   {static_cast<int>(BoardRoles::DebugInfoRole)});
}

bool BoardModel::getShowBlack() const { return m_showBlack; }
void BoardModel::setShowBlack(bool showBlack) {
  if (m_showBlack == showBlack)
    return;
  m_showBlack = showBlack;
  emit showBlackChanged();

  if (showBlack)
    setDebugEnabled(true);

  emit dataChanged(createIndex(0, 0), createIndex(7, 7),
                   {static_cast<int>(BoardRoles::DebugInfoRole)});
}

bool BoardModel::getEnPassant() const { return m_enPassant; }
void BoardModel::setEnPassant(bool enPassant) {
  if (m_enPassant == enPassant)
    return;
  m_enPassant = enPassant;
  emit enPassantChanged();

  if (enPassant)
    setDebugEnabled(true);

  emit dataChanged(createIndex(0, 0), createIndex(7, 7),
                   {static_cast<int>(BoardRoles::DebugInfoRole)});
}

QHash<int, QByteArray> BoardModel::roleNames() const {
  using enum Chess_UI::BoardModel::BoardRoles;
  return {
      {static_cast<int>(ImageRole), "pieceImage"},
      {static_cast<int>(ValidMoveRole), "validMove"},
      {static_cast<int>(ValidCaptureRole), "validCapture"},
      {static_cast<int>(SelectedRole), "selected"},
      {static_cast<int>(DebugInfoRole), "debugInfo"},
  };
}

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
  if (m_enPassant)
    debugInfo |= getEnPassantMask();

  return debugInfo;
}

} // namespace Chess_UI
