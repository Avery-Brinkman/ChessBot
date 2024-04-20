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
    m_bot->startNewGame();
  } else if (command.starts_with(POSITION)) {
    if (command.length() < 10)
      throw std::invalid_argument("Invalid position command");

    m_bot->setPosition(command.substr(9));
  } else if (command == "go infinite") {
    std::cout << "bestmove e7e6" << std::endl;
  } else {
    std::cout << "Unknown command: " << command << std::endl;
  }
}

} // namespace Engine_NS