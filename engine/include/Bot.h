#pragma once

#include "Board.h"
#include "Move.h"

#include <forward_list>
#include <string_view>

namespace Engine_NS {
class Bot {
public:
  void startNewGame();

  void setToStartPosition();

  void setPosition(std::string_view position);

  void makeMove(std::string_view move);

private:
  Board m_board = {};
};
} // namespace Engine_NS