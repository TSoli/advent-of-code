#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using HikeMap = std::vector<std::string>;

struct Direction {
  int i, j;
};

const std::array<Direction, 4> kDirections{{{-1, 0}, {0, 1}, {1, 0}, {0, -1}}};

bool inBounds(const HikeMap &hikeMap, const int i, const int j) {
  return i >= 0 && i < hikeMap.size() && j >= 0 && j < hikeMap[i].size();
}

int getScore(const HikeMap &hikeMap, std::unordered_set<int> &visited, int i,
             int j, char height) {
  if (height == '9') {
    return 1;
  }
  char target{static_cast<char>(height + 1)};
  int total{0};
  for (const auto &direction : kDirections) {
    int ii{i + direction.i};
    int jj{j + direction.j};
    size_t pos{ii * hikeMap[0].size() + jj};
    if (inBounds(hikeMap, ii, jj) && hikeMap[ii][jj] == target &&
        !visited.contains(pos)) {
      visited.insert(pos);
      total += getScore(hikeMap, visited, ii, jj, target);
    }
  }
  return total;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::string line;
  HikeMap hikeMap;
  while (std::getline(inf, line)) {
    hikeMap.push_back(line);
  }

  long totalScore{0};
  for (int i = 0; i < hikeMap.size(); ++i) {
    for (int j = 0; j < hikeMap[i].size(); ++j) {
      if (hikeMap[i][j] == '0') {
        std::unordered_set<int> visited;
        visited.insert(i * hikeMap.size() + j);
        totalScore += getScore(hikeMap, visited, i, j, '0');
      }
    }
  }

  std::cout << "Total score: " << totalScore << std::endl;

  return 0;
}
