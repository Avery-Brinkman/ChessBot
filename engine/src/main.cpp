#include "Engine.h"
#include "UCICommands.h"

#include <iostream>
#include <queue>
#include <string>
#include <thread>

int main() {
  Engine_NS::Engine engine;

  std::string command = "";
  while (command != QUIT) {
    std::getline(std::cin, command);
    engine.receiveCommand(command);
  }

  return 0;
}