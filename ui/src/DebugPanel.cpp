#include "DebugPanel.h"

namespace Chess_UI {

bool DebugPanel::getDebugEnabled() const { return m_debugEnabled; }
void DebugPanel::setDebugEnabled(bool debugEnabled) {
  if (m_debugEnabled == debugEnabled)
    return;
  m_debugEnabled = debugEnabled;
  emit debugEnabledChanged();

  if (!debugEnabled) {
    setShowWhite(false);
    setShowBlack(false);
    setEnPassant(false);
  }

  emit updateBoard();
}

bool DebugPanel::getShowWhite() const { return m_showWhite; }
void DebugPanel::setShowWhite(bool showWhite) {
  if (m_showWhite == showWhite)
    return;
  m_showWhite = showWhite;
  emit showWhiteChanged();

  if (showWhite)
    setDebugEnabled(true);

  emit updateBoard();
}

bool DebugPanel::getShowBlack() const { return m_showBlack; }
void DebugPanel::setShowBlack(bool showBlack) {
  if (m_showBlack == showBlack)
    return;
  m_showBlack = showBlack;
  emit showBlackChanged();

  if (showBlack)
    setDebugEnabled(true);

  emit updateBoard();
}

bool DebugPanel::getEnPassant() const { return m_enPassant; }
void DebugPanel::setEnPassant(bool enPassant) {
  if (m_enPassant == enPassant)
    return;
  m_enPassant = enPassant;
  emit enPassantChanged();

  if (enPassant)
    setDebugEnabled(true);

  emit updateBoard();
}
bool DebugPanel::getBitboardBuilder() const { return m_bitboardBuilder; }
void DebugPanel::setBitboardBuilder(bool bitboardBuilder) {
  if (m_bitboardBuilder == bitboardBuilder)
    return;
  m_bitboardBuilder = bitboardBuilder;
  emit bitboardBuilderChanged();

  if (bitboardBuilder)
    setDebugEnabled(false);

  emit updateBoard();
}
} // namespace Chess_UI