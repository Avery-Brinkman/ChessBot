#include "BoardModel.h"

#include <QColor>
#include <QUrl>
#include <iostream>

namespace Chess_UI {
BoardModel::BoardModel(QObject* parent) : QAbstractTableModel(parent), Engine_NS::Board() {
  setToStartPosition();

  m_bitboardsModel = std::make_unique<BitboardsModel>();
  m_bitboardsModel->updateBoards(getBitboards());

  // Update when we change which boards we want to show
  QObject::connect(m_bitboardsModel.get(), &BitboardsModel::debugBitsChanged, this,
                   [this]() { emitDataChangeSignals({BoardRoles::BitboardRole}); });

  // Update when we enable/disable custom bits
  QObject::connect(m_bitboardsModel.get(), &BitboardsModel::useCustomValueChanged, this,
                   [this]() { emitDataChangeSignals({BoardRoles::BitboardRole}); });

  // Update when we change the custom bits
  QObject::connect(m_bitboardsModel.get(), &BitboardsModel::customValueChanged, this,
                   [this]() { emitDataChangeSignals({BoardRoles::BitboardRole}); });
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
    if (m_selectedIndex == Engine_NS::INVALID)
      return false;
    return m_selectedIndex == bitIndex;
  }
  case RankAndFileRole:
    return QString::fromStdString(getIndex(index.row(), index.column()).toString());
  case BoardIndexRole:
    return bitIndex.index;
  case BitboardRole:
    return m_bitboardsModel->getDebugBits().checkBit(bitIndex);
  case HasPieceRole:
    return getBitboards().getInfo().allPieces.checkBit(bitIndex);
  default:
    return QVariant();
  }
}

bool BoardModel::setData(const QModelIndex& index, const QVariant& value, int role) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const Engine_NS::BoardIndex bitIndex = getIndex(index.row(), index.column());
  switch (BoardRoles(role)) {
  case SelectedRole:
    return setSelected(index, bitIndex);
  case ValidMoveRole:
    return makeValidMove(bitIndex);
  case TogglePieceRole:
    return addRemovePiece(bitIndex, value.toBool());
  case ToggleCustomBitRole: {
    m_bitboardsModel->toggleCustomBit(bitIndex);

    // Data change signal is emitted through the connection made in the constructor

    return true;
  }
  default:
    break;
  }

  return false;
}

QHash<int, QByteArray> BoardModel::roleNames() const {
  using enum Chess_UI::BoardModel::BoardRoles;
  return {
      {static_cast<int>(ImageRole), "pieceImage"},
      {static_cast<int>(ValidMoveRole), "validMove"},
      {static_cast<int>(SelectedRole), "selected"},
      {static_cast<int>(RankAndFileRole), "rankAndFile"},
      {static_cast<int>(BoardIndexRole), "boardIndex"},
      {static_cast<int>(TogglePieceRole), "togglePiece"},
      {static_cast<int>(BitboardRole), "bitboard"},
      {static_cast<int>(ToggleCustomBitRole), "toggleCustomBit"},
      {static_cast<int>(HasPieceRole), "hasPiece"},
  };
}

SettingsPanel* BoardModel::getSettingsPanel() const { return m_settingsPanel.get(); }

BitboardsModel* BoardModel::getBitboardsModel() const { return m_bitboardsModel.get(); }

QUrl BoardModel::pieceImage(const Engine_NS::Piece& piece) const {
  using namespace Engine_NS::Pieces;
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

QModelIndex BoardModel::getQModelIndex(const Engine_NS::BoardIndex& index) const {
  return createIndex(8 - index.rank(), index.file());
}

// Set data handlers

bool BoardModel::setSelected(const QModelIndex& qIndex, const Engine_NS::BoardIndex& bitIndex) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const Engine_NS::BoardIndex prevIndex = m_selectedIndex;
  m_selectedIndex = bitIndex;
  m_currentValidMoves = getValidMoves(bitIndex);

  if (prevIndex != Engine_NS::Index::INVALID)
    emitDataChangeSignals(prevIndex, {SelectedRole});
  emitDataChangeSignals(qIndex, {SelectedRole});
  emitDataChangeSignals({ValidMoveRole});

  return true;
}

bool BoardModel::makeValidMove(const Engine_NS::BoardIndex& bitIndex) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const Engine_NS::BoardIndex prevIndex = m_selectedIndex;
  if (!m_currentValidMoves.checkBit(bitIndex))
    return false;

  movePiece(m_selectedIndex, bitIndex);
  m_selectedIndex = {};
  m_currentValidMoves = 0;

  m_bitboardsModel->updateBoards(getBitboards());
  m_bitboardsModel->updateDebugBits();

  emitDataChangeSignals(prevIndex, {SelectedRole, ImageRole});
  emitDataChangeSignals(bitIndex, {ImageRole});
  emitDataChangeSignals({ValidMoveRole, BitboardRole});

  return true;
}

bool BoardModel::addRemovePiece(const Engine_NS::BoardIndex& bitIndex, bool isWhite) {
  using enum Chess_UI::BoardModel::BoardRoles;

  const Engine_NS::Color color = isWhite ? Engine_NS::Color::White : Engine_NS::Color::Black;
  togglePiece(Engine_NS::Piece(color, m_settingsPanel->getPieceType()), bitIndex);

  m_bitboardsModel->updateBoards(getBitboards());
  m_bitboardsModel->updateDebugBits();

  emitDataChangeSignals(bitIndex, {ImageRole});
  emitDataChangeSignals({BitboardRole});

  return true;
}

void BoardModel::emitDataChangeSignals(const QModelIndex& index,
                                       const std::vector<BoardRoles>& roles) {
  for (const BoardRoles& role : roles)
    emit dataChanged(index, index, {static_cast<int>(role)});
}

void BoardModel::emitDataChangeSignals(const std::vector<BoardRoles>& roles) {
  for (const BoardRoles& role : roles)
    emit dataChanged(createIndex(0, 0), createIndex(7, 7), {static_cast<int>(role)});
}

} // namespace Chess_UI
