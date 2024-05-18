#include "SettingsPanel.h"

namespace Chess_UI {

SettingsPanel::SettingsPanel(QObject* parent) : QObject(parent) {}

bool SettingsPanel::getShowRankAndFile() const { return m_showRankAndFile; }
bool SettingsPanel::getShowIndex() const { return m_showIndex; }

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
