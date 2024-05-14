#include "MoveGenerators.h"
#include "Piece.hpp"
#include <iostream>

namespace ToolMenu {
enum Option {
  PawnMovements = '1',
  KnightMovements,
  BishopMovements,
  RookMovements,
  QueenMovements,
  KingMovements,

  Invalid,
  Quit = 'q',
};

static void clearScreen() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined(__APPLE__)
  system("clear");
#endif
}

static void printMenu() {
  std::cout << "+------------------------------+" << std::endl;
  std::cout << "| Tools                        |" << std::endl;
  std::cout << "+------------------------------+" << std::endl;
  std::cout << "| 1) Generate Pawn Movements   |" << std::endl;
  std::cout << "| 2) Generate Knight Movements |" << std::endl;
  std::cout << "| 3) Generate Bishop Movements |" << std::endl;
  std::cout << "| 4) Generate Rook Movements   |" << std::endl;
  std::cout << "| 5) Generate Queen Movements  |" << std::endl;
  std::cout << "| 6) Generate King Movements   |" << std::endl;
  std::cout << "|                              |" << std::endl;
  std::cout << "| q) Quit                      |" << std::endl;
  std::cout << "+------------------------------+" << std::endl;
}

static Option getInput() {
  using enum Option;
  char input;

  std::cout << "> ";
  std::cin >> input;

  Option option = Option(input);

  if (option < PawnMovements || option > Quit || (option >= Invalid && option < Quit))
    return Invalid;

  return option;
}

static void runTool(Option option) {
  using enum Option;

  switch (option) {
  case PawnMovements: {
    std::cout << "White Pawn Moves:" << std::endl;
    std::array<Engine_NS::BitBoard, 64> pawnMoves = Tools::generateMoves(Engine_NS::WhitePawn);
    for (const Engine_NS::BitBoard& bitBoard : pawnMoves)
      std::cout << bitBoard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << "White Pawn Attacks:" << std::endl;
    pawnMoves = Tools::generateAttacks(Engine_NS::WhitePawn);
    for (const Engine_NS::BitBoard& bitBoard : pawnMoves)
      std::cout << bitBoard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << std::endl;
    std::cout << "Black Pawn Moves:" << std::endl;
    pawnMoves = Tools::generateMoves(Engine_NS::BlackPawn);
    for (const Engine_NS::BitBoard& bitBoard : pawnMoves)
      std::cout << bitBoard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << "Black Pawn Attacks:" << std::endl;
    pawnMoves = Tools::generateAttacks(Engine_NS::BlackPawn);
    for (const Engine_NS::BitBoard& bitBoard : pawnMoves)
      std::cout << bitBoard.bits << ", ";
    break;
  }
  case KnightMovements: {
    std::cout << "Knight Moves:" << std::endl;
    for (const Engine_NS::BitBoard& bitBoard : Tools::generateMoves(Engine_NS::WhiteKnight))
      std::cout << bitBoard.bits << ", ";
    break;
  }
  case BishopMovements:
    std::cout << "bishop" << std::endl;
    break;
  case RookMovements:
    std::cout << "rook" << std::endl;
    break;
  case QueenMovements:
    std::cout << "queen" << std::endl;
    break;
  case KingMovements:
    std::cout << "king" << std::endl;
    break;
  case Quit:
    std::cout << "Goodbye!  ";
    break;
  default:
    std::cout << "Invalid option!  ";
    break;
  }
  std::cout << "\b\b  " << std::endl;
  std::cout << std::endl;
}

} // namespace ToolMenu

int main() {
  using namespace ToolMenu;

  Option option = Option::Invalid;

  while (option != Option::Quit) {
    printMenu();
    option = getInput();
    clearScreen();
    runTool(option);
  }

  return 0;
}
