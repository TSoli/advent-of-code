#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

using Grid = std::vector<std::string>;
using IdGrid = std::vector<std::vector<int>>;

struct Direction {
  int i, j;
};

std::vector<Direction> kDirections{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

bool inbounds(const Grid &grid, int i, int j) {
  return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
}

bool inbounds(const IdGrid &grid, int i, int j) {
  return i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size();
}

int getArea(const Grid &grid, IdGrid &idGrid, const int id,
            std::unordered_set<int> &visited, const int i, const int j) {
  const int numCols{static_cast<int>(grid[i].size())};
  visited.insert(i * numCols + j);
  idGrid[i][j] = id;

  char target{grid[i][j]};
  int total{1};
  for (const auto &direction : kDirections) {
    int ii{i + direction.i};
    int jj{j + direction.j};
    if (inbounds(grid, ii, jj) && grid[ii][jj] == target &&
        !visited.contains(ii * numCols + jj)) {
      total += getArea(grid, idGrid, id, visited, ii, jj);
    }
  }

  return total;
}

// sides now
void getPerimeter(std::vector<int> &perimeters, const IdGrid &grid, int i,
                  int j) {
  int dIdx{0};
  size_t lIdx{(((dIdx - 1) % kDirections.size()) + kDirections.size()) %
              kDirections.size()};
  int li{i + kDirections[lIdx].i};
  int lj{j + kDirections[lIdx].j};
  bool surrounded{inbounds(grid, li, lj)};
  const int leftId{surrounded ? grid[li][lj] : -1};

  Direction startDir{kDirections[dIdx]};
  Direction startPos{i, j};

  const int target{grid[i][j]};
  int perimeter{0};

  do {
    lIdx = (((dIdx - 1) % kDirections.size()) + kDirections.size()) %
           kDirections.size();
    li = i + kDirections[lIdx].i;
    lj = j + kDirections[lIdx].j;
    int ni{i + kDirections[dIdx].i};
    int nj{j + kDirections[dIdx].j};

    if (!inbounds(grid, li, lj) || grid[li][lj] != target) {
      // fence on left
      if (!inbounds(grid, li, lj) || grid[li][lj] != leftId) {
        surrounded = false;
      }
    }

    if (inbounds(grid, li, lj) && grid[li][lj] == target) {
      // turn left
      dIdx = lIdx;
      ++perimeter;
    } else if (!inbounds(grid, ni, nj) || grid[ni][nj] != target) {
      ++perimeter;
      int ci{i + kDirections[lIdx].i + kDirections[dIdx].i};
      int cj{j + kDirections[lIdx].j + kDirections[dIdx].j};

      // literal corner case
      if (inbounds(grid, ci, cj) && grid[ci][cj] != leftId) {
        surrounded = false;
      }

      // turn right
      dIdx = (dIdx + 1) % kDirections.size();
      continue;
    }

    i += kDirections[dIdx].i;
    j += kDirections[dIdx].j;

  } while (!(kDirections[dIdx].i == startDir.i &&
             kDirections[dIdx].j == startDir.j && i == startPos.i &&
             j == startPos.j));

  perimeters[target] += perimeter;

  perimeters[leftId] += surrounded * perimeter;
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
  Grid grid;
  IdGrid idGrid;
  while (std::getline(inf, line)) {
    grid.push_back(line);
    idGrid.push_back(std::vector<int>(line.size(), -1));
  }

  std::unordered_set<int> visited;
  const int numCols{static_cast<int>(grid[0].size())};
  int id{0};
  std::vector<int> areas;
  std::vector<int> startI;
  std::vector<int> startJ;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (!visited.contains(i * numCols + j)) {
        areas.push_back(getArea(grid, idGrid, id, visited, i, j));
        startI.push_back(i);
        startJ.push_back(j);
        ++id;
      }
    }
  }

  std::vector<int> perimeters(areas.size(), 0);

  for (int n = 0; n < areas.size(); ++n) {
    getPerimeter(perimeters, idGrid, startI[n], startJ[n]);
  }

  long total{0};
  for (int i = 0; i < areas.size(); ++i) {
    total += areas[i] * perimeters[i];
  }

  std::cout << "Cost: " << total << std::endl;

  return 0;
}
