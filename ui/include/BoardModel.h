#pragma once

#include "Board.h"
#include "SettingsPanel.h"

#include <QAbstractTableModel>

namespace Chess_UI {
class BoardModel : public QAbstractTableModel, public Engine_NS::Board {
  enum class BoardRoles {
    ImageRole = Qt::UserRole + 1,
    ValidMoveRole,
    SelectedRole,
    DebugInfoRole,
    BitboardRole,
    RankAndFileRole,
    BoardIndexRole,
    TogglePieceRole,
  };

  Q_OBJECT
  Q_PROPERTY(SettingsPanel* settingsPanel READ getSettingsPanel CONSTANT)

public:
  explicit BoardModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  SettingsPanel* getSettingsPanel() const;

private:
  QUrl pieceImage(const Engine_NS::Piece& piece) const;

  Engine_NS::Bitboard getDebugInfo() const;

  Engine_NS::BoardIndex m_selectedIndex = {};
  Engine_NS::Bitboard m_currentValidMoves = {};

  SettingsPanel* m_settingsPanel = new SettingsPanel();

  Engine_NS::Bitboard m_bitboard = {};
};
} // namespace Chess_UI
