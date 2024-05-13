#include "DebugPanel.h"

namespace Chess_UI {

DebugPanel::DebugPanel(QObject* parent) : QObject(parent) {
  QObject::connect(this, &DebugPanel::debugEnabledChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::showWhiteChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::showBlackChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::enPassantChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::bitboardBuilderChanged, this, &DebugPanel::updateBoard);
}

bool DebugPanel::getShowRankAndFile() const { return m_showRankAndFile; }
bool DebugPanel::getShowBits() const { return m_showBits; }

bool DebugPanel::getDebugEnabled() const { return m_debugEnabled; }
bool DebugPanel::getShowWhite() const { return m_showWhite; }
bool DebugPanel::getShowBlack() const { return m_showBlack; }
bool DebugPanel::getEnPassant() const { return m_enPassant; }

bool DebugPanel::getBitboardBuilder() const { return m_bitboardBuilder; }

} // namespace Chess_UI