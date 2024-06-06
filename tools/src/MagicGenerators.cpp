#include "MagicGenerators.h"
#include <iostream>
#include <random>
#include <unordered_set>

namespace {

size_t findMagicNumber(const std::vector<Engine_NS::Bitboard>& blockers) {
  using namespace Engine_NS;

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<size_t> uni(1, 999999);

  while (true) {
    std::unordered_set<BitboardBits> generatedIndicies = {};
    size_t possibleMagic = uni(rng) * uni(rng);

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

size_t findMaximumShift(const std::vector<Engine_NS::Bitboard>& blockers, size_t magicNum) {
  using namespace Engine_NS;

  size_t maxShift = 0;

  while (true) {
    std::unordered_set<BitboardBits> generatedKeys = {};
    const size_t currentShift = maxShift + 1;
    if (currentShift > 57)
      return maxShift;

    for (const Bitboard& blocker : blockers) {
      const BitboardBits currentKey = (blocker.bits * magicNum) >> currentShift;

      if (generatedKeys.contains(currentKey))
        return maxShift;

      generatedKeys.insert(currentKey);
    }

    maxShift = currentShift;
  }
}

Tools::MagicValues generateMagicValues(const std::vector<Engine_NS::Bitboard>& blockers) {

  size_t magicNum = findMagicNumber(blockers);
  size_t shift = findMaximumShift(blockers, magicNum);

  return {magicNum, shift};
}

std::vector<Engine_NS::Bitboard> generateAllBlockers(const Engine_NS::Bitboard& allowedMoves) {
  using namespace Engine_NS;

  // Get the indexes of the bits that are enabled for the current allowed moves
  std::vector<BoardIndex> enabledIndexes = {};
  for (const BoardIndex& b : IndexList)
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
    BoardIndex bitLocation = enabledIndexes.at(bitIndex);

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
    bool blocked = false;
    for (BoardIndex currentIndex = position; currentIndex.rank() < Rank(8);
         currentIndex = currentIndex + CompassDirection::North) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Down
    blocked = false;
    for (BoardIndex currentIndex = position; currentIndex.rank() > Rank(1);
         currentIndex = currentIndex + CompassDirection::South) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Left
    blocked = false;
    for (BoardIndex currentIndex = position; currentIndex.file() > File::A;
         currentIndex = currentIndex + CompassDirection::West) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // Right
    blocked = false;
    for (BoardIndex currentIndex = position; currentIndex.file() < File::H;
         currentIndex = currentIndex + CompassDirection::East) {

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

std::vector<Engine_NS::Bitboard>
generateValidBishopMoves(const Engine_NS::Bitboard& allowedMoves,
                         const std::vector<Engine_NS::Bitboard>& blockers,
                         Engine_NS::BoardIndex position) {
  using namespace Engine_NS;

  std::vector<Bitboard> validMoves(blockers.size());

  // Loop thru each of the blocker combinations
  for (int boardIndex = 0; boardIndex < blockers.size(); boardIndex++) {
    const Bitboard& blockerBoard = blockers.at(boardIndex);

    // Valid moves starts with whatever the piece can do on an empty board
    Bitboard currentValidMoves = allowedMoves;

    // UpRight
    bool blocked = false;
    for (BoardIndex currentIndex = position;
         (currentIndex.rank() < Rank(8)) && (currentIndex.file() < File::H);
         currentIndex = currentIndex + CompassDirection::NorthEast) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // DownRight
    blocked = false;
    for (BoardIndex currentIndex = position;
         (currentIndex.rank() > Rank(1)) && (currentIndex.file() < File::H);
         currentIndex = currentIndex + CompassDirection::SouthEast) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // DownLeft
    blocked = false;
    for (BoardIndex currentIndex = position;
         (currentIndex.rank() > Rank(1)) && (currentIndex.file() > File::A);
         currentIndex = currentIndex + CompassDirection::SouthWest) {

      if (blocked) {
        currentValidMoves.disableBit(currentIndex);
      } else {
        blocked = blockerBoard.checkBit(currentIndex);
      }
    }

    // UpLeft
    blocked = false;
    for (BoardIndex currentIndex = position;
         (currentIndex.rank() < Rank(8)) && (currentIndex.file() > File::A);
         currentIndex = currentIndex + CompassDirection::NorthWest) {

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

} // namespace

namespace Tools {

void generateRookMagics() {
  using namespace Engine_NS;

  int max = 0;

  for (int i = 0; i < 64; i++) {
    const BoardIndex index = Index(i);

    // Get the allowed moves for a rook at the given index
    Bitboard currentMoves = Precomputed::RookMoves.at(index.index);
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

    max = std::max(max, (int)blockerBoards.size());

    // Determine what the valid moves are for each of those blocker layouts
    std::vector<Bitboard> validMoves = generateValidRookMoves(currentMoves, blockerBoards, index);

    // MAGIC!!!

    const MagicValues magicVals = generateMagicValues(blockerBoards);

    size_t lowestIndex = 0 - 1;
    size_t highestIndex = 0;
    for (const Bitboard& validMove : validMoves) {
      size_t currentIndex = (validMove.bits * magicVals.first) >> magicVals.second;
      lowestIndex = std::min(lowestIndex, currentIndex);
      highestIndex = std::max(highestIndex, currentIndex);
    }

    std::cout << i + 1 << "/64: Magic Num = " << magicVals.first
              << " | Shift = " << magicVals.second << "\n";
    std::cout << "  Lowest Index: " << lowestIndex << " | Highest Index: " << highestIndex << "\n";
  }

  std::cout << "Max: " << max << std::endl;
}

void generateBishopMagics() {
  using namespace Engine_NS;

  int max = 0;

  for (int i = 0; i < 64; i++) {
    const BoardIndex index = Index(i);

    // Get the allowed moves for a bishop at the given index
    Bitboard currentMoves = Precomputed::BishopMoves.at(index.index);
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

    max = std::max(max, (int)blockerBoards.size());

    // Determine what the valid moves are for each of those blocker layouts
    std::vector<Bitboard> validMoves = generateValidBishopMoves(currentMoves, blockerBoards, index);

    // MAGIC!!!

    const MagicValues magicVals = generateMagicValues(blockerBoards);

    std::cout << i + 1 << "/64: Magic Num = " << magicVals.first
              << " | Shift = " << magicVals.second << "\n";
  }
  std::cout << "Max: " << max << std::endl;
}

} // namespace Tools
