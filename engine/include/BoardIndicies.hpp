#pragma once

#include <array>
#include <format>
#include <map>
#include <string>

namespace Engine_NS {

// clang-format off
enum Index : unsigned char {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  INVALID
};
// clang-format on

using Rank = unsigned char;
enum File : unsigned char { A, B, C, D, E, F, G, H };

static constexpr std::array<char, 8> FileToString = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

struct BoardIndex {
  BoardIndex(Index index) : index(index){};
  BoardIndex() = default;

  std::string toString() const { return std::format("{}{}", FileToString.at(file()), rank()); }
  static BoardIndex fromString(std::string_view index);

  Rank rank() const { return (index / 8) + 1; }
  File file() const { return File(index % 8); }

  Index index = INVALID;

  operator Index() const { return index; }
};

} // namespace Engine_NS