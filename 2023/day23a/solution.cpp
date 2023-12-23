#include <array>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using Pos = std::array<int, 2>;

template <>
struct std::hash<Pos> {
  size_t operator()(const Pos &s) const noexcept {
    size_t h1 = std::hash<int>{}(s[0]);
    size_t h2 = std::hash<int>{}(s[1]);
    return h1 ^ (h2 << 1);
  }
};

void getLongestPath(const Pos &startPos, int currSteps,
                    const std::vector<std::string> &map,
                    std::unordered_set<Pos> visited,
                    std::unordered_map<Pos, int> &maxDistances) {
  const std::array<Pos, 4> directions{{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

  visited.insert(startPos);
  if (currSteps < maxDistances[startPos]) {
    return;
  }

  maxDistances[startPos] = currSteps;

  for (const auto &direction : directions) {
    Pos newPos{startPos[0] + direction[0], startPos[1] + direction[1]};
    if (newPos[0] < 0 || newPos[1] < 0 || newPos[0] > map.size() - 1 ||
        newPos[1] > map.front().size() - 1 || visited.contains(newPos)) {
      continue;
    }

    char tile{map[newPos[0]][newPos[1]]};
    switch (tile) {
      case '.':
        getLongestPath(newPos, currSteps + 1, map, visited, maxDistances);
        break;
      case 'v':
        newPos[0] += directions[0][0];
        newPos[1] += directions[0][1];
        if (!visited.contains(newPos)) {
          getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
        }
        break;
      case '>':
        newPos[0] += directions[1][0];
        newPos[1] += directions[1][1];
        if (!visited.contains(newPos)) {
          getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
        }
        break;
      case '^':
        newPos[0] += directions[2][0];
        newPos[1] += directions[2][1];
        if (!visited.contains(newPos)) {
          getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
        }
        break;
      case '<':
        newPos[0] += directions[3][0];
        newPos[1] += directions[3][1];
        if (!visited.contains(newPos)) {
          getLongestPath(newPos, currSteps + 2, map, visited, maxDistances);
        }
        break;
    }
  }
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
  std::string line;
  while (inf >> line) {
    map.push_back(line);
  }

  int i{0};
  while (i < map[0].size()) {
    if (map[0][i] == '.') {
      break;
    }
    ++i;
  }

  Pos startPos{0, i};

  i = 0;
  while (i < map.back().size()) {
    if (map.back()[i] == '.') {
      break;
    }
    ++i;
  }

  Pos endPos{static_cast<int>(map.size()) - 1, i};

  int currSteps{0};
  std::unordered_set<Pos> visited;
  std::unordered_map<Pos, int> maxDistances;
  getLongestPath(startPos, currSteps, map, visited, maxDistances);

  std::cout << "Longest path distance: " << maxDistances.at(endPos) << "\n";

  return 0;
}
