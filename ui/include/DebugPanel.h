#pragma once

#include <QObject>

namespace Chess_UI {

class DebugPanel : public QObject {

  Q_OBJECT
  Q_PROPERTY(bool showRankAndFile MEMBER m_showRankAndFile NOTIFY showRankAndFileChanged)
  Q_PROPERTY(bool showBits MEMBER m_showBits NOTIFY showBitsChanged)

  Q_PROPERTY(bool debugEnabled MEMBER m_debugEnabled NOTIFY debugEnabledChanged)
  Q_PROPERTY(bool showWhite MEMBER m_showWhite NOTIFY showWhiteChanged)
  Q_PROPERTY(bool showBlack MEMBER m_showBlack NOTIFY showBlackChanged)
  Q_PROPERTY(bool enPassant MEMBER m_enPassant NOTIFY enPassantChanged)

  Q_PROPERTY(bool bitboardBuilder MEMBER m_bitboardBuilder NOTIFY bitboardBuilderChanged)

public:
  explicit DebugPanel(QObject* parent = nullptr);

  bool getShowRankAndFile() const;
  bool getShowBits() const;

  bool getDebugEnabled() const;
  bool getShowWhite() const;
  bool getShowBlack() const;
  bool getEnPassant() const;

  bool getBitboardBuilder() const;

signals:
  void showRankAndFileChanged();
  void showBitsChanged();

  void debugEnabledChanged();
  void showWhiteChanged();
  void showBlackChanged();
  void enPassantChanged();

  void bitboardBuilderChanged();

  void updateBoard();

private:
  bool m_showRankAndFile = true;
  bool m_showBits = true;

  bool m_debugEnabled = true;
  bool m_showWhite = false;
  bool m_showBlack = false;
  bool m_enPassant = false;

  bool m_bitboardBuilder = false;
};

} // namespace Chess_UI