#include "BoardIndicies.hpp"

namespace Engine_NS {

BoardIndex BoardIndex::fromString(std::string_view index) {
  const int rank = std::stoi(std::string(index.substr(1))) - 1;
  if (rank < 0 || rank > 7)
    return BoardIndex{INVALID};

  const auto findFile = std::ranges::find(FileToString, index.at(0));
  if (findFile == FileToString.end())
    return BoardIndex{INVALID};
  const auto file = static_cast<int>(std::distance(FileToString.begin(), findFile));

  return BoardIndex{Index(rank * 8 + file)};
};

} // namespace Engine_NS