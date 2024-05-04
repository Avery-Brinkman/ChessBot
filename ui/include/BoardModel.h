#pragma once

#include "Board.h"

#include <QAbstractTableModel>

namespace Chess_UI {
class BoardModel : public QAbstractTableModel, public Engine_NS::Board {
  enum class BoardRoles {
    ImageRole = Qt::UserRole + 1,
    ValidMoveRole,
    SelectedRole,
    MoveRole,
  };

  Q_OBJECT

public:
  explicit BoardModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

private:
  QUrl pieceImage(Piece piece) const;

  int m_selectedIndex = -1;
  BitBoard m_currentValidMoves = 0;
};
} // namespace Chess_UI