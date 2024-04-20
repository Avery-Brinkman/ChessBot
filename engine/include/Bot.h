#pragma once

#include "Board.h"
#include "Move.h"

#include <forward_list>
#include <string_view>

namespace Engine_NS {
class Bot {
public:
  void startNewGame();

  void setPosition(std::string_view position);

private:
  Board m_board = {};

  std::forward_list<Move> m_moves = {};
  std::forward_list<Move>::iterator m_lastMove;
};
} // namespace Engine_NS