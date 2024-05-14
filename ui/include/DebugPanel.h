#pragma once

#include "Piece.hpp"
#include <QObject>

namespace Chess_UI {

class DebugPanel : public QObject {

  Q_OBJECT
  Q_PROPERTY(bool showRankAndFile MEMBER m_showRankAndFile NOTIFY showRankAndFileChanged)
  Q_PROPERTY(bool showIndex MEMBER m_showIndex NOTIFY showIndexChanged)

  Q_PROPERTY(bool debugEnabled MEMBER m_debugEnabled NOTIFY debugEnabledChanged)
  Q_PROPERTY(bool showBits MEMBER m_showBits NOTIFY showBitsChanged)
  Q_PROPERTY(bool showWhite MEMBER m_showWhite NOTIFY showWhiteChanged)
  Q_PROPERTY(bool showBlack MEMBER m_showBlack NOTIFY showBlackChanged)
  Q_PROPERTY(bool enPassant MEMBER m_enPassant NOTIFY enPassantChanged)

  Q_PROPERTY(int pieceType READ getPieceType_QML WRITE setPieceType NOTIFY pieceTypeChanged)

public:
  explicit DebugPanel(QObject* parent = nullptr);

  bool getShowRankAndFile() const;
  bool getShowIndex() const;

  bool getDebugEnabled() const;
  bool getShowBits() const;
  bool getShowWhite() const;
  bool getShowBlack() const;
  bool getEnPassant() const;

  Engine_NS::PieceType getPieceType() const;

signals:
  void showRankAndFileChanged();
  void showIndexChanged();

  void debugEnabledChanged();
  void showBitsChanged();
  void showWhiteChanged();
  void showBlackChanged();
  void enPassantChanged();

  void pieceTypeChanged();

  void updateBoard();

private:
  int getPieceType_QML() const;
  void setPieceType(int pieceType);

  bool m_showRankAndFile = true;
  bool m_showIndex = false;

  bool m_debugEnabled = true;
  bool m_showBits = true;
  bool m_showWhite = false;
  bool m_showBlack = false;
  bool m_enPassant = false;

  Engine_NS::PieceType m_pieceType = Engine_NS::PieceType::Pawn;
};

} // namespace Chess_UI