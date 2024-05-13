#pragma once

#include "Board.h"
#include "DebugPanel.h"

#include <QAbstractTableModel>

namespace Chess_UI {
class BoardModel : public QAbstractTableModel, public Engine_NS::Board {
  enum class BoardRoles {
    ImageRole = Qt::UserRole + 1,
    ValidMoveRole,
    SelectedRole,
    DebugInfoRole,
    BitBoardRole,
    RankAndFileRole,
    TogglePieceRole,
  };

  Q_OBJECT
  Q_PROPERTY(DebugPanel* debugPanel READ getDebugPanel CONSTANT)

public:
  explicit BoardModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  DebugPanel* getDebugPanel() const;

private:
  QUrl pieceImage(const Engine_NS::Piece& piece) const;

  Engine_NS::BitBoard getDebugInfo() const;

  Engine_NS::BoardIndex m_selectedIndex = {};
  Engine_NS::BitBoard m_currentValidMoves = {};

  DebugPanel* m_debugPanel = new DebugPanel();

  Engine_NS::BitBoard m_bitBoard = {};
};
} // namespace Chess_UI