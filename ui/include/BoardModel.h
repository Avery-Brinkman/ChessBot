#pragma once

#include "Board.h"

#include <QAbstractTableModel>

namespace Chess_UI {
class BoardModel : public QAbstractTableModel, public Engine_NS::Board {
  enum class BoardRoles {
    ImageRole = Qt::UserRole + 1,
    ValidMoveRole,
    ValidCaptureRole,
    SelectedRole,
    DebugInfoRole,
  };

  Q_OBJECT
  Q_PROPERTY(bool debugEnabled READ getDebugEnabled WRITE setDebugEnabled NOTIFY debugEnabledChanged)
  Q_PROPERTY(bool showWhite READ getShowWhite WRITE setShowWhite NOTIFY showWhiteChanged)
  Q_PROPERTY(bool showBlack READ getShowBlack WRITE setShowBlack NOTIFY showBlackChanged)
  Q_PROPERTY(bool enPassant READ getEnPassant WRITE setEnPassant NOTIFY enPassantChanged)

public:
  explicit BoardModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  bool getDebugEnabled() const;
  void setDebugEnabled(bool debugEnabled);

  bool getShowWhite() const;
  void setShowWhite(bool showWhite);

  bool getShowBlack() const;
  void setShowBlack(bool showBlack);

  bool getEnPassant() const;
  void setEnPassant(bool enPassant);

signals:
  void debugEnabledChanged();
  void showWhiteChanged();
  void showBlackChanged();
  void enPassantChanged();

private:
  QUrl pieceImage(Piece piece) const;

  BitBoard getDebugInfo() const;

  int m_selectedIndex = -1;
  BitBoard m_currentValidMoves = 0;
  BitBoard m_currentValidCaptures = 0;

  bool m_debugEnabled = false;
  bool m_showWhite = false;
  bool m_showBlack = false;
  bool m_enPassant = false;
};
} // namespace Chess_UI