#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <unordered_set>
#include <vector>

using Pos = std::array<int, 2>;

template <> struct std::hash<Pos> {
  size_t operator()(const Pos &s) const noexcept {
    size_t h1 = std::hash<int>{}(s[0]);
    size_t h2 = std::hash<int>{}(s[1]);
    return h1 ^ (h2 << 1);
  }
};

Pos getMap(std::ifstream &inf, std::vector<std::string> &map) {
  std::string line;
  Pos pos;

  int row{0};
  while (inf >> line) {
    map.push_back(line);
    size_t startCol{line.find('S')};
    if (startCol != std::string::npos) {
      pos[0] = row;
      pos[1] = startCol;
      map[row][startCol] = '.';
    }
    ++row;
  }

  return pos;
}

void updatePos(std::unordered_set<Pos> &startPositions,
               const std::vector<std::string> &map) {
  std::unordered_set<Pos> nextPositions{};

  for (const auto &pos : startPositions) {
    if (pos[0] > 0 && map[pos[0] - 1][pos[1]] != '#') {
      nextPositions.insert({pos[0] - 1, pos[1]});
    }

    if (pos[0] < map.size() - 1 && map[pos[0] + 1][pos[1]] != '#') {
      nextPositions.insert({pos[0] + 1, pos[1]});
    }

    if (pos[1] > 0 && map[pos[0]][pos[1] - 1] != '#') {
      nextPositions.insert({pos[0], pos[1] - 1});
    }

    if (pos[1] < map[pos[0]].size() - 1 && map[pos[0]][pos[1] + 1] != '#') {
      nextPositions.insert({pos[0], pos[1] + 1});
    }
  }

  startPositions = nextPositions;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::vector<std::string> map;
  Pos startPos{getMap(inf, map)};
  std::unordered_set<Pos> startPositions;
  startPositions.insert(startPos);

  int steps{64};
  while (steps--) {
    updatePos(startPositions, map);
  }

  std::cout << "Number of positions: " << startPositions.size() << "\n";

  return 0;
}
