#include "MagicGenerators.h"
#include <iostream>
#include <random>
#include <unordered_set>

namespace {

int findMagicNumber(const std::vector<Engine_NS::Bitboard>& blockers) {
  using namespace Engine_NS;

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni(1, 999999);

  while (true) {
    std::unordered_set<BitboardBits> generatedIndicies = {};
    int possibleMagic = uni(rng) * uni(rng);

    for (const Bitboard& currentBoard : blockers) {
      BitboardBits key = currentBoard.bits * possibleMagic;

      if (generatedIndicies.contains(key))
        break;

      generatedIndicies.insert(key);
    }

    if (generatedIndicies.size() == blockers.size())
      return possibleMagic;
  }
}

int findMaximumShift(const std::vector<Engine_NS::Bitboard>& blockers, int magicNum) {
  using namespace Engine_NS;

  int maxShift = 0;

  while (true) {
    std::unordered_set<BitboardBits> generatedKeys = {};
    const int currentShift = maxShift + 1;
    if (currentShift > 57)
      throw std::exception("SHIFT TOO BIG!!! AHHH!!!");

    for (const Bitboard& blocker : blockers) {
      const BitboardBits currentKey = (blocker.bits * magicNum) >> currentShift;

      if (generatedKeys.contains(currentKey))
        return maxShift;

      generatedKeys.insert(currentKey);
    }

    maxShift = currentShift;
  }
}

} // namespace

namespace Tools {

MagicValues generateMagicValues(const std::vector<Engine_NS::Bitboard>& blockers) {

  int magicNum = findMagicNumber(blockers);
  int shift = findMaximumShift(blockers, magicNum);

  return {magicNum, shift};
}

std::vector<Engine_NS::Bitboard> generateAllBlockers(const Engine_NS::Bitboard& allowedMoves) {
  using namespace Engine_NS;

  // Get the indexes of the bits that are enabled for the current allowed moves
  std::vector<int> enabledIndexes = {};
  for (int b = 0; b < 64; b++)
    if (allowedMoves.checkBit(b))
      enabledIndexes.push_back(b);

  // Use this number to determine how many combinations of blockers there can be
  const auto numPatterns = (1 << enabledIndexes.size());

  // Make a list to hold all the occupancy patterns
  std::vector<Bitboard> blockerBoards(numPatterns);
  // We'll skip the first one
  int patternIndex = 1;

  // This is the board we'll do binary increments on
  Bitboard workingBoard = 0;
  // Start by looking at the first enabled bit
  int bitIndex = 0;

  while (bitIndex < enabledIndexes.size()) {
    // We're lookin at the bit right here
    int bitLocation = enabledIndexes.at(bitIndex);

    // If bit is 0, set to 1
    if (!workingBoard.checkBit(bitLocation)) {
      workingBoard.enableBit(bitLocation);

      // Set that blocker board to this board
      blockerBoards.at(patternIndex) = workingBoard;
      patternIndex++;

      // Restart binary
      bitIndex = 0;
    }
    // If 1, set to 0 and enable next bit
    else {
      workingBoard.disableBit(bitLocation);
      bitIndex++;
    }
  }

  // Each possible blocker combo is now set for a piece at the current location
  return blockerBoards;
}

std::vector<Engine_NS::Bitboard>
generateValidRookMoves(const Engine_NS::Bitboard& allowedMoves,
                       const std::vector<Engine_NS::Bitboard>& blockers,
                       Engine_NS::BoardIndex position) {
  using namespace Engine_NS;

  std::vector<Bitboard> validMoves(blockers.size());

  // Loop thru each of the blocker combinations
  for (int boardIndex = 0; boardIndex < blockers.size(); boardIndex++) {
    const Bitboard& blockerBoard = blockers.at(boardIndex);

    // Valid moves starts with whatever the piece can do on an empty board
    Bitboard currentValidMoves = allowedMoves;

    // Up
    for (BoardIndex currentIndex = position; currentIndex.rank() < Rank(8);
         currentIndex = currentIndex + CompassDirection::North) {
      bool blocked = false;

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Down
    for (BoardIndex currentIndex = position; currentIndex.rank() > Rank(1);
         currentIndex = currentIndex + CompassDirection::South) {
      bool blocked = false;

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Left
    for (BoardIndex currentIndex = position; currentIndex.file() > File::A;
         currentIndex = currentIndex + CompassDirection::West) {
      bool blocked = false;

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Right
    for (BoardIndex currentIndex = position; currentIndex.file() < File::H;
         currentIndex = currentIndex + CompassDirection::East) {
      bool blocked = false;

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    validMoves.at(boardIndex) = currentValidMoves;
  }

  return validMoves;
}

void generateRookMagics() {
  using namespace Engine_NS;

  for (int i = 0; i < 64; i++) {
    const BoardIndex index = Index(i);

    // Get the allowed moves for a rook at the given index
    Bitboard currentMoves = Precomputed::RookMoves.at(index);
    // Don't care about edges
    if (index.rank() < Rank(8))
      currentMoves.disableBits(TopEdge);
    if (index.rank() > Rank(1))
      currentMoves.disableBits(BottomEdge);
    if (index.file() > File::A)
      currentMoves.disableBits(LeftEdge);
    if (index.file() < File::H)
      currentMoves.disableBits(RightEdge);

    // Generate all the possible blocker layouts
    std::vector<Bitboard> blockerBoards = generateAllBlockers(currentMoves);
    // Determine what the valid moves are for each of those blocker layouts
    std::vector<Bitboard> validMoves = generateValidRookMoves(currentMoves, blockerBoards, index);

    // MAGIC!!!

    const MagicValues magicVals = generateMagicValues(blockerBoards);

    std::cout << i + 1 << "/64: Magic Num = " << magicVals.first
              << " | Shift = " << magicVals.second << "\n";
  }
}

} // namespace Tools
