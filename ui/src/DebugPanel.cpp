#include "DebugPanel.h"

namespace Chess_UI {

DebugPanel::DebugPanel(QObject* parent) : QObject(parent) {
  QObject::connect(this, &DebugPanel::debugEnabledChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::showWhiteChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::showBlackChanged, this, &DebugPanel::updateBoard);
  QObject::connect(this, &DebugPanel::enPassantChanged, this, &DebugPanel::updateBoard);
}

bool DebugPanel::getShowRankAndFile() const { return m_showRankAndFile; }
bool DebugPanel::getShowIndex() const { return m_showIndex; }

bool DebugPanel::getDebugEnabled() const { return m_debugEnabled; }
bool DebugPanel::getShowBits() const { return m_showBits; }
bool DebugPanel::getShowWhite() const { return m_showWhite; }
bool DebugPanel::getShowBlack() const { return m_showBlack; }
bool DebugPanel::getEnPassant() const { return m_enPassant; }

Engine_NS::PieceType DebugPanel::getPieceType() const { return m_pieceType; }

int DebugPanel::getPieceType_QML() const { return static_cast<int>(m_pieceType); }
void DebugPanel::setPieceType(int pieceType) {
  const auto type = Engine_NS::PieceType(pieceType);

  if (m_pieceType == type)
    return;

  m_pieceType = type;
  emit pieceTypeChanged();
}

} // namespace Chess_UI