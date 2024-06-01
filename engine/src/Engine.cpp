#include "Engine.h"
#include "UCICommands.h"

#include <iostream>

namespace Engine_NS {

void Engine::receiveCommand(std::string_view command) const {
  if (command == UCI) {
    std::cout << ID << " name ChessBot" << std::endl;
    std::cout << ID << " author Mr_Grillo" << std::endl;
    std::cout << UCI_OK << std::endl;
  } else if (command == IS_READY) {
    std::cout << READY_OK << std::endl;
  } else if (command == UCI_NEW_GAME) {
    // Do nothing
  } else if (command.starts_with(POSITION)) {
    if (command.length() < 10)
      throw std::invalid_argument("Invalid position command");

    handlePositionCommand(command.substr(9));
  } else if (command.starts_with(GO)) {
    std::cout << "bestmove e7e6" << std::endl;
  } else {
    std::cout << "Unknown command: " << command << std::endl;
  }
}

// Private functions

void Engine::handlePositionCommand(std::string_view command) const {
  if (command.starts_with("startpos")) {
    m_bot->setToStartPosition();
    command.remove_prefix(std::min(size_t(9), command.length()));
  } else if (command.starts_with("fen")) {
    throw std::invalid_argument("FEN is not supported yet");
  } else {
    throw std::invalid_argument("Invalid position command");
  }

  if (command.starts_with("moves")) {
    command.remove_prefix(std::min(size_t(6), command.length()));
    while (!command.empty()) {
      std::string_view move = command.substr(0, 4);
      m_bot->makeMove(move);
      command.remove_prefix(std::min(size_t(5), command.length()));
    }
  }
}

} // namespace Engine_NS