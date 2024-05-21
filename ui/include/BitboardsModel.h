#pragma once

#include "Bitboard.hpp"

#include <QAbstractListModel>
#include <QList>
#include <QString>

namespace Chess_UI {

class BitboardsModel : public QAbstractListModel {
  enum class BitboardsRoles {
    NameRole = Qt::UserRole + 1,
    BitsRole,
    EnabledRole,
  };

  enum BitboardRows {
    WhitePawns,
    WhiteKnights,
    WhiteBishops,
    WhiteRooks,
    WhiteQueens,
    WhiteKing,
    BlackPawns,
    BlackKnights,
    BlackBishops,
    BlackRooks,
    BlackQueens,
    BlackKing,

    WhiteEnPassant,
    BlackEnPassant,
  };

  Q_OBJECT

public:
  BitboardsModel(QObject* parent = nullptr);

  void updateBoards(const Engine_NS::Bitboards& newBoards);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  Engine_NS::Bitboard getDebugBits() const;

  void updateDebugBits();

signals:
  void enabledChanged();
  void debugBitsChanged();

private:
  Engine_NS::Bitboard getBits(int row) const;

  static QString format(const Engine_NS::Bitboard& bits);

  bool m_whitePawnsEnabled = false;
  bool m_whiteKnightsEnabled = false;
  bool m_whiteBishopsEnabled = false;
  bool m_whiteRooksEnabled = false;
  bool m_whiteQueensEnabled = false;
  bool m_whiteKingEnabled = false;
  bool m_blackPawnsEnabled = false;
  bool m_blackKnightsEnabled = false;
  bool m_blackBishopsEnabled = false;

  bool m_blackRooksEnabled = false;
  bool m_blackQueensEnabled = false;
  bool m_blackKingEnabled = false;
  bool m_whiteEnPassantEnabled = false;
  bool m_blackEnPassantEnabled = false;

  Engine_NS::Bitboards m_boards = {};

  QHash<int, QString> m_names = {};
  QList<bool> m_enabled = {};

  Engine_NS::Bitboard m_debugBits = {};
};

} // namespace Chess_UI
