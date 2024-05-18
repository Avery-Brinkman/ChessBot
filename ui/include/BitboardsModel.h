#pragma once

#include "Bitboard.hpp"

#include <QObject>
#include <QString>

namespace Chess_UI {

class BitboardsModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString whitePawns READ getWhitePawns NOTIFY dataChanged)
  Q_PROPERTY(QString whiteKnights READ getWhiteKnights NOTIFY dataChanged)
  Q_PROPERTY(QString whiteBishops READ getWhiteBishops NOTIFY dataChanged)
  Q_PROPERTY(QString whiteRooks READ getWhiteRooks NOTIFY dataChanged)
  Q_PROPERTY(QString whiteQueens READ getWhiteQueens NOTIFY dataChanged)
  Q_PROPERTY(QString whiteKing READ getWhiteKing NOTIFY dataChanged)
  Q_PROPERTY(QString blackPawns READ getBlackPawns NOTIFY dataChanged)
  Q_PROPERTY(QString blackKnights READ getBlackKnights NOTIFY dataChanged)
  Q_PROPERTY(QString blackBishops READ getBlackBishops NOTIFY dataChanged)
  Q_PROPERTY(QString blackRooks READ getBlackRooks NOTIFY dataChanged)
  Q_PROPERTY(QString blackQueens READ getBlackQueens NOTIFY dataChanged)
  Q_PROPERTY(QString blackKing READ getBlackKing NOTIFY dataChanged)
  Q_PROPERTY(QString whiteEnPassant READ getWhiteEnPassant NOTIFY dataChanged)
  Q_PROPERTY(QString blackEnPassant READ getBlackEnPassant NOTIFY dataChanged)

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

  QString getWhitePawns() const;
  QString getWhiteKnights() const;
  QString getWhiteBishops() const;
  QString getWhiteRooks() const;
  QString getWhiteQueens() const;
  QString getWhiteKing() const;
  QString getBlackPawns() const;
  QString getBlackKnights() const;

  QString getBlackBishops() const;
  QString getBlackRooks() const;
  QString getBlackQueens() const;
  QString getBlackKing() const;
  QString getWhiteEnPassant() const;
  QString getBlackEnPassant() const;

signals:
  void dataChanged();
  void enabledChanged();

private:
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
};

} // namespace Chess_UI
