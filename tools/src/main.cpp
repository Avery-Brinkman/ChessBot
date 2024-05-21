#include "MoveGenerators.h"
#include "Piece.hpp"
#include <iostream>

namespace ToolMenu {

enum class Option {
  PawnMovements = '1',
  KnightMovements,
  BishopMovements,
  RookMovements,
  QueenMovements,
  KingMovements,
  MagicBits,

  Invalid,
  Quit = 'q',
};

enum class MagicOption { BishopBits = '1', RookBits, QueenBits, Back, Invalid, Quit = 'q' };

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
  std::cout << "| 7) Generate Magic Bits       |" << std::endl;
  std::cout << "|                              |" << std::endl;
  std::cout << "| q) Quit                      |" << std::endl;
  std::cout << "+------------------------------+" << std::endl;
}

static void magicMenu() {
  std::cout << "+------------------------------+" << std::endl;
  std::cout << "| Magic Bits                   |" << std::endl;
  std::cout << "+------------------------------+" << std::endl;
  std::cout << "| 1) Generate Bishop Bits      |" << std::endl;
  std::cout << "| 2) Generate Rook Bits        |" << std::endl;
  std::cout << "| 3) Generate Queen Bits       |" << std::endl;
  std::cout << "|                              |" << std::endl;
  std::cout << "| 4) Back                      |" << std::endl;
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

static MagicOption getMagicInput() {
  using enum MagicOption;
  char input;

  std::cout << "> ";
  std::cin >> input;

  MagicOption option = MagicOption(input);

  if (option < BishopBits || option > Quit || (option >= Invalid && option < Quit))
    return Invalid;

  return option;
}

static void runTool(Option option) {
  using enum Option;

  switch (option) {
  case PawnMovements: {
    std::cout << "White Pawn Moves:" << std::endl;
    std::array<Engine_NS::Bitboard, 64> pawnMoves = Tools::generateMoves(Engine_NS::WhitePawn);
    for (const Engine_NS::Bitboard& bitboard : pawnMoves)
      std::cout << bitboard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << "White Pawn Attacks:" << std::endl;
    pawnMoves = Tools::generateAttacks(Engine_NS::WhitePawn);
    for (const Engine_NS::Bitboard& bitboard : pawnMoves)
      std::cout << bitboard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << std::endl;
    std::cout << "Black Pawn Moves:" << std::endl;
    pawnMoves = Tools::generateMoves(Engine_NS::BlackPawn);
    for (const Engine_NS::Bitboard& bitboard : pawnMoves)
      std::cout << bitboard.bits << ", ";
    std::cout << "\b\b  " << std::endl;
    std::cout << "Black Pawn Attacks:" << std::endl;
    pawnMoves = Tools::generateAttacks(Engine_NS::BlackPawn);
    for (const Engine_NS::Bitboard& bitboard : pawnMoves)
      std::cout << bitboard.bits << ", ";
    break;
  }
  case KnightMovements: {
    std::cout << "Knight Moves:" << std::endl;
    for (const Engine_NS::Bitboard& bitboard : Tools::generateMoves(Engine_NS::WhiteKnight))
      std::cout << bitboard.bits << ", ";
    break;
  }
  case BishopMovements: {
    std::cout << "Bishop Moves:" << std::endl;
    for (const Engine_NS::Bitboard& bitboard : Tools::generateMoves(Engine_NS::WhiteBishop))
      std::cout << bitboard.bits << ", ";
    break;
  }
  case RookMovements: {
    std::cout << "Rook Moves:" << std::endl;
    for (const Engine_NS::Bitboard& bitboard : Tools::generateMoves(Engine_NS::WhiteRook))
      std::cout << bitboard.bits << ", ";
    break;
  }
  case QueenMovements: {
    std::cout << "Queen Moves:" << std::endl;
    for (const Engine_NS::Bitboard& bitboard : Tools::generateMoves(Engine_NS::WhiteQueen))
      std::cout << bitboard.bits << ", ";
    break;
  }
  case KingMovements: {
    std::cout << "King Moves:" << std::endl;
    for (const Engine_NS::Bitboard& bitboard : Tools::generateMoves(Engine_NS::WhiteKing))
      std::cout << bitboard.bits << ", ";
    break;
  }
  case MagicBits: {
    std::cout << "Unimplemented  ";
    break;
  }
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

static void runMagicTool(MagicOption option) {
  using enum MagicOption;
  switch (option) {
  case BishopBits:
    std::cout << "Bishop" << std::endl;
    break;
  case RookBits:
    std::cout << "Rook" << std::endl;
    break;
  case QueenBits:
    std::cout << "Queen" << std::endl;
    break;
  case Back:
    break;
  case Quit:
    std::cout << "Goodbye!" << std::endl;
    break;
  default:
    std::cout << "Invalid option!" << std::endl;
    break;
  }
}

} // namespace ToolMenu

bool magicMain() {
  using namespace ToolMenu;

  MagicOption option = MagicOption::Invalid;

  while (option != MagicOption::Quit && option != MagicOption::Back) {
    magicMenu();
    option = getMagicInput();
    clearScreen();
    runMagicTool(option);
  }

  return option == MagicOption::Quit;
};

int main() {
  using namespace ToolMenu;

  Option option = Option::Invalid;
  bool quit = false;

  while (option != Option::Quit) {
    printMenu();
    option = getInput();
    clearScreen();
    if (option == Option::MagicBits) {
      quit = magicMain();
    } else {
      runTool(option);
    }

    if (quit)
      break;
  }

  return 0;
}
