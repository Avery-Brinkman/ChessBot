#include "Bot.h"

namespace {
uint16_t indexFromName(std::string_view move) {
  const uint16_t rank = move[1];
  const uint16_t file = move[0] - 'a';
  return rank * 8 + file;
}
} // namespace

namespace Engine_NS {

void Bot::startNewGame() {}

void Bot::setToStartPosition() { m_board.setToStartPosition(); }

void Bot::setPosition(std::string_view position) {}

void Bot::makeMove(std::string_view move) {
  const uint16_t startIndex = indexFromName(move.substr(0, 2));
  const uint16_t endIndex = indexFromName(move.substr(2, 2));
}

} // namespace Engine_NS