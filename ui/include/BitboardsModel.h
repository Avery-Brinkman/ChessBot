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
  Q_PROPERTY(bool showAsBin MEMBER m_showAsBin NOTIFY showAsBinChanged)
  Q_PROPERTY(bool showAsDec MEMBER m_showAsDec NOTIFY showAsDecChanged)
  Q_PROPERTY(bool showAsHex MEMBER m_showAsHex NOTIFY showAsHexChanged)

  Q_PROPERTY(bool useCustomValue MEMBER m_useCustomValue NOTIFY useCustomValueChanged)
  Q_PROPERTY(QString customValue READ getCustomValue WRITE setCustomValue NOTIFY customValueChanged)

public:
  BitboardsModel(QObject* parent = nullptr);

  void updateBoards(const Engine_NS::Bitboards& newBoards);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  Engine_NS::Bitboard getDebugBits() const;

  Engine_NS::Bitboard getCustomBits() const;
  QString getCustomValue() const;

  void setCustomValue(const QString& customVal);

  void updateDebugBits();

  Q_INVOKABLE void copyBitsToClipboard(int row) const;
  Q_INVOKABLE void pasteBitsFromClipboard(int row);

  void toggleCustomBit(Engine_NS::BoardIndex index);

signals:
  // Emitted when we show/hide the bitboard overlay
  void enabledChanged();
  // Emitted when the bits that are shown change
  void debugBitsChanged();

  void showAsBinChanged();
  void showAsDecChanged();
  void showAsHexChanged();

  void useCustomValueChanged();
  void customValueChanged();

private:
  Engine_NS::Bitboard getBits(int row) const;

  // Converts number strings to Bitboards

  Engine_NS::Bitboard format(const QString& bits) const;
  static Engine_NS::Bitboard binFormat(const QString& bits);
  static Engine_NS::Bitboard hexFormat(const QString& bits);

  // Converts Bitboards to QStr

  QString format(const Engine_NS::Bitboard& bits, bool colors = true) const;
  static QString binFormat(const Engine_NS::Bitboard& bits, bool colors);
  static QString hexFormat(const Engine_NS::Bitboard& bits);

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

  bool m_showAsBin = true;
  bool m_showAsDec = false;
  bool m_showAsHex = false;

  bool m_useCustomValue = false;
  Engine_NS::Bitboard m_customBits = 0;
};

} // namespace Chess_UI
