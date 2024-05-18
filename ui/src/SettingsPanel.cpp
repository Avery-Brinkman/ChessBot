#include "SettingsPanel.h"

namespace Chess_UI {

SettingsPanel::SettingsPanel(QObject* parent) : QObject(parent) {
  QObject::connect(this, &SettingsPanel::debugEnabledChanged, this, &SettingsPanel::updateBoard);
  QObject::connect(this, &SettingsPanel::showWhiteChanged, this, &SettingsPanel::updateBoard);
  QObject::connect(this, &SettingsPanel::showBlackChanged, this, &SettingsPanel::updateBoard);
  QObject::connect(this, &SettingsPanel::enPassantChanged, this, &SettingsPanel::updateBoard);
}

bool SettingsPanel::getShowRankAndFile() const { return m_showRankAndFile; }
bool SettingsPanel::getShowIndex() const { return m_showIndex; }

bool SettingsPanel::getDebugEnabled() const { return m_debugEnabled; }
bool SettingsPanel::getShowBits() const { return m_showBits; }
bool SettingsPanel::getShowWhite() const { return m_showWhite; }
bool SettingsPanel::getShowBlack() const { return m_showBlack; }
bool SettingsPanel::getEnPassant() const { return m_enPassant; }

Engine_NS::PieceType SettingsPanel::getPieceType() const { return m_pieceType; }

int SettingsPanel::getPieceType_QML() const { return static_cast<int>(m_pieceType); }
void SettingsPanel::setPieceType(int pieceType) {
  const auto type = Engine_NS::PieceType(pieceType);

  if (m_pieceType == type)
    return;

  m_pieceType = type;
  emit pieceTypeChanged();
}

} // namespace Chess_UI
