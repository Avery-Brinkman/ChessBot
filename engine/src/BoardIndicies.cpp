#include "BoardIndicies.h"

namespace {

constexpr std::array<char[3], 64> BoardIndexStrings = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2",
    "A3", "B3", "C3", "D3", "E3", "F3", "G3", "H3", "A4", "B4", "C4", "D4", "E4", "F4", "G4", "H4",
    "A5", "B5", "C5", "D5", "E5", "F5", "G5", "H5", "A6", "B6", "C6", "D6", "E6", "F6", "G6", "H6",
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7", "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"};

const std::map<std::string, Engine_NS::BoardIndex, std::less<>> BoardIndexMap = {
    {"A1", Engine_NS::BoardIndex::A1}, {"B1", Engine_NS::BoardIndex::B1},
    {"C1", Engine_NS::BoardIndex::C1}, {"D1", Engine_NS::BoardIndex::D1},
    {"E1", Engine_NS::BoardIndex::E1}, {"F1", Engine_NS::BoardIndex::F1},
    {"G1", Engine_NS::BoardIndex::G1}, {"H1", Engine_NS::BoardIndex::H1},
    {"A2", Engine_NS::BoardIndex::A2}, {"B2", Engine_NS::BoardIndex::B2},
    {"C2", Engine_NS::BoardIndex::C2}, {"D2", Engine_NS::BoardIndex::D2},
    {"E2", Engine_NS::BoardIndex::E2}, {"F2", Engine_NS::BoardIndex::F2},
    {"G2", Engine_NS::BoardIndex::G2}, {"H2", Engine_NS::BoardIndex::H2},
    {"A3", Engine_NS::BoardIndex::A3}, {"B3", Engine_NS::BoardIndex::B3},
    {"C3", Engine_NS::BoardIndex::C3}, {"D3", Engine_NS::BoardIndex::D3},
    {"E3", Engine_NS::BoardIndex::E3}, {"F3", Engine_NS::BoardIndex::F3},
    {"G3", Engine_NS::BoardIndex::G3}, {"H3", Engine_NS::BoardIndex::H3},
    {"A4", Engine_NS::BoardIndex::A4}, {"B4", Engine_NS::BoardIndex::B4},
    {"C4", Engine_NS::BoardIndex::C4}, {"D4", Engine_NS::BoardIndex::D4},
    {"E4", Engine_NS::BoardIndex::E4}, {"F4", Engine_NS::BoardIndex::F4},
    {"G4", Engine_NS::BoardIndex::G4}, {"H4", Engine_NS::BoardIndex::H4},
    {"A5", Engine_NS::BoardIndex::A5}, {"B5", Engine_NS::BoardIndex::B5},
    {"C5", Engine_NS::BoardIndex::C5}, {"D5", Engine_NS::BoardIndex::D5},
    {"E5", Engine_NS::BoardIndex::E5}, {"F5", Engine_NS::BoardIndex::F5},
    {"G5", Engine_NS::BoardIndex::G5}, {"H5", Engine_NS::BoardIndex::H5},
    {"A6", Engine_NS::BoardIndex::A6}, {"B6", Engine_NS::BoardIndex::B6},
    {"C6", Engine_NS::BoardIndex::C6}, {"D6", Engine_NS::BoardIndex::D6},
    {"E6", Engine_NS::BoardIndex::E6}, {"F6", Engine_NS::BoardIndex::F6},
    {"G6", Engine_NS::BoardIndex::G6}, {"H6", Engine_NS::BoardIndex::H6},
    {"A7", Engine_NS::BoardIndex::A7}, {"B7", Engine_NS::BoardIndex::B7},
    {"C7", Engine_NS::BoardIndex::C7}, {"D7", Engine_NS::BoardIndex::D7},
    {"E7", Engine_NS::BoardIndex::E7}, {"F7", Engine_NS::BoardIndex::F7},
    {"G7", Engine_NS::BoardIndex::G7}, {"H7", Engine_NS::BoardIndex::H7},
    {"A8", Engine_NS::BoardIndex::A8}, {"B8", Engine_NS::BoardIndex::B8},
    {"C8", Engine_NS::BoardIndex::C8}, {"D8", Engine_NS::BoardIndex::D8},
    {"E8", Engine_NS::BoardIndex::E8}, {"F8", Engine_NS::BoardIndex::F8},
    {"G8", Engine_NS::BoardIndex::G8}, {"H8", Engine_NS::BoardIndex::H8}};

} // namespace

namespace Engine_NS {

std::string to_string(BoardIndex index) { return BoardIndexStrings.at(index); }

BoardIndex from_string(std::string_view index) {
  if (!BoardIndexMap.contains(index))
    return BoardIndex::INVALID;

  return BoardIndexMap.at(index.data());
}

} // namespace Engine_NS