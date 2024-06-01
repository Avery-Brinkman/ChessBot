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
  Q_PROPERTY(bool useCustomValue MEMBER m_useCustomValue NOTIFY useCustomValueChanged)
  Q_PROPERTY(bool showBitboards MEMBER m_showBitboards NOTIFY showBitboardsChanged)

  Q_PROPERTY(bool showAsBin MEMBER m_showAsBin NOTIFY showAsBinChanged)
  Q_PROPERTY(bool showAsDec MEMBER m_showAsDec NOTIFY showAsDecChanged)
  Q_PROPERTY(bool showAsHex MEMBER m_showAsHex NOTIFY showAsHexChanged)

  Q_PROPERTY(
      QString customValue READ getCustomValue WRITE setCustomValue NOTIFY customValueTextChanged)

public:
  BitboardsModel(QObject* parent = nullptr);

  // Sets the set of bitboards to whatever is given
  void updateBoards(const Engine_NS::Bitboards& newBoards);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role) override;

  QHash<int, QByteArray> roleNames() const override;

  // Gets the bits to show over the board
  Engine_NS::Bitboard getDebugBits() const;

  // Gets the bits that were manually set by the user
  Engine_NS::Bitboard getCustomBits() const;
  // Gets the string version of the custom bits
  QString getCustomValue() const;

  void setCustomValue(const QString& customVal);

  void updateDebugBits();

  Q_INVOKABLE void copyBitsToClipboard(int row) const;
  Q_INVOKABLE void pasteBitsFromClipboard(int row);

  void toggleCustomBit(Engine_NS::BoardIndex index);

signals:
  // Emitted when we show/hide the bitboard overlay
  void showBitboardsChanged();
  // Emitted when the bits that are shown change
  void debugBitsChanged();

  void useCustomValueChanged();

  void showAsBinChanged();
  void showAsDecChanged();
  void showAsHexChanged();

  // Emitted when the actual custom value has changed
  void customValueChanged();
  // Emitted when QML should update what is shown for custom value (format or value change)
  void customValueTextChanged();

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

  Engine_NS::Bitboards m_boards = {};

  QHash<int, QString> m_names = {};
  QList<bool> m_enabled = {};

  bool m_showBitboards = false;

  Engine_NS::Bitboard m_debugBits = {};

  bool m_showAsBin = true;
  bool m_showAsDec = false;
  bool m_showAsHex = false;

  bool m_useCustomValue = false;
  Engine_NS::Bitboard m_customBits = 0;
};

} // namespace Chess_UI
