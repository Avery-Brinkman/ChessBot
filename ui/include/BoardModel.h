#pragma once

#include "BitboardsModel.h"
#include "Board.h"
#include "SettingsPanel.h"

#include <QAbstractTableModel>
#include <memory>

namespace Chess_UI {
class BoardModel : public QAbstractTableModel, public Engine_NS::Board {
  enum class BoardRoles {
    ImageRole = Qt::UserRole + 1,
    ValidMoveRole,
    SelectedRole,
    RankAndFileRole,
    BoardIndexRole,
    TogglePieceRole,
    BitboardRole,
    ToggleCustomBitRole,
    HasPieceRole,
  };

  Q_OBJECT
  Q_PROPERTY(SettingsPanel* settingsPanel READ getSettingsPanel CONSTANT)
  Q_PROPERTY(BitboardsModel* bitboards READ getBitboardsModel CONSTANT)

public:
  explicit BoardModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

private:
  SettingsPanel* getSettingsPanel() const;

  BitboardsModel* getBitboardsModel() const;

  QUrl pieceImage(const Engine_NS::Piece& piece) const;

  QModelIndex getQModelIndex(const Engine_NS::BoardIndex& index) const;

  // Set data handlers

  bool setSelected(const QModelIndex& qIndex, const Engine_NS::BoardIndex& bitIndex);

  bool makeValidMove(const Engine_NS::BoardIndex& bitIndex);

  bool addRemovePiece(const Engine_NS::BoardIndex& bitIndex, bool isWhite);

  void emitDataChangeSignals(const Engine_NS::BoardIndex& bitIndex,
                             const std::vector<BoardRoles>& roles) {
    emitDataChangeSignals(getQModelIndex(bitIndex), roles);
  }
  void emitDataChangeSignals(const QModelIndex& index, const std::vector<BoardRoles>& roles);
  void emitDataChangeSignals(const std::vector<BoardRoles>& roles);

  Engine_NS::BoardIndex m_selectedIndex{};
  Engine_NS::Bitboard m_currentValidMoves{};

  std::unique_ptr<SettingsPanel> m_settingsPanel = std::make_unique<SettingsPanel>();
  std::unique_ptr<BitboardsModel> m_bitboardsModel;

  Engine_NS::Bitboard m_bitboard{};
};
} // namespace Chess_UI
