#pragma once

#include "Piece.hpp"
#include <QObject>

namespace Chess_UI {

class SettingsPanel : public QObject {

  Q_OBJECT
  Q_PROPERTY(bool showRankAndFile MEMBER m_showRankAndFile NOTIFY showRankAndFileChanged)
  Q_PROPERTY(bool showIndex MEMBER m_showIndex NOTIFY showIndexChanged)
  Q_PROPERTY(bool showBitboards MEMBER m_showBitboards NOTIFY showBitboardsChanged)

  Q_PROPERTY(int pieceType READ getPieceType_QML WRITE setPieceType NOTIFY pieceTypeChanged)

public:
  explicit SettingsPanel(QObject* parent = nullptr);

  bool getShowRankAndFile() const;
  bool getShowIndex() const;

  Engine_NS::PieceType getPieceType() const;

signals:
  void showRankAndFileChanged();
  void showIndexChanged();
  void showBitboardsChanged();

  void pieceTypeChanged();

  void updateBoard();

private:
  int getPieceType_QML() const;
  void setPieceType(int pieceType);

  bool m_showRankAndFile = true;
  bool m_showIndex = false;
  bool m_showBitboards = false;

  Engine_NS::PieceType m_pieceType = Engine_NS::PieceType::Pawn;
};

} // namespace Chess_UI
