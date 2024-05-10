#pragma once

#include <QObject>

namespace Chess_UI {

class DebugPanel : public QObject {

  Q_OBJECT
  Q_PROPERTY(
      bool debugEnabled READ getDebugEnabled WRITE setDebugEnabled NOTIFY debugEnabledChanged)
  Q_PROPERTY(bool showWhite READ getShowWhite WRITE setShowWhite NOTIFY showWhiteChanged)
  Q_PROPERTY(bool showBlack READ getShowBlack WRITE setShowBlack NOTIFY showBlackChanged)
  Q_PROPERTY(bool enPassant READ getEnPassant WRITE setEnPassant NOTIFY enPassantChanged)
  Q_PROPERTY(bool bitboardBuilder READ getBitboardBuilder WRITE setBitboardBuilder NOTIFY
                 bitboardBuilderChanged)

public:
  bool getDebugEnabled() const;
  void setDebugEnabled(bool debugEnabled);

  bool getShowWhite() const;
  void setShowWhite(bool showWhite);

  bool getShowBlack() const;
  void setShowBlack(bool showBlack);

  bool getEnPassant() const;
  void setEnPassant(bool enPassant);

  bool getBitboardBuilder() const;
  void setBitboardBuilder(bool bitboardBuilder);

signals:
  void debugEnabledChanged();
  void showWhiteChanged();
  void showBlackChanged();
  void enPassantChanged();
  void bitboardBuilderChanged();
  void updateBoard();

private:
  bool m_debugEnabled = false;
  bool m_showWhite = false;
  bool m_showBlack = false;
  bool m_enPassant = false;
  bool m_bitboardBuilder = false;
};

} // namespace Chess_UI