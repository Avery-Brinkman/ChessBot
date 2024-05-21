#pragma once

#include "Bitboard.hpp"

#include <QAbstractListModel>
#include <QString>

namespace Chess_UI {

class BitboardsModel : public QAbstractListModel {
  enum class BitboardsRoles {
    NameRole = Qt::UserRole + 1,
    BitsRole,
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

  Q_PROPERTY(bool whitePawnsEnabled MEMBER m_whitePawnsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteKnightsEnabled MEMBER m_whiteKnightsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteBishopsEnabled MEMBER m_whiteBishopsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteRooksEnabled MEMBER m_whiteRooksEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteQueensEnabled MEMBER m_whiteQueensEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteKingEnabled MEMBER m_whiteKingEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackPawnsEnabled MEMBER m_blackPawnsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackKnightsEnabled MEMBER m_blackKnightsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackBishopsEnabled MEMBER m_blackBishopsEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackRooksEnabled MEMBER m_blackRooksEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackQueensEnabled MEMBER m_blackQueensEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackKingEnabled MEMBER m_blackKingEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool whiteEnPassantEnabled MEMBER m_whiteEnPassantEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool blackEnPassantEnabled MEMBER m_blackEnPassantEnabled NOTIFY enabledChanged)

public:
  BitboardsModel(QObject* parent = nullptr);

  void updateBoards(const Engine_NS::Bitboards& newBoards);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

signals:
  void enabledChanged();

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
};

} // namespace Chess_UI
