#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Direction {
  int dx, dy;
};

using Grid = std::vector<std::string>;
const std::vector<Direction> kDirections{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

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

  Grid grid;
  std::string row;
  int i{0};
  int startX, startY;
  while (std::getline(inf, row)) {
    size_t pos{row.find("^")};
    if (pos != std::string::npos) {
      startY = i;
      startX = pos;
      row[startX] = 'X';
    }
    grid.push_back(row);
    ++i;
  }

  int numVisited{1};
  int currX{startX};
  int currY{startY};
  int dIdx{0};
  Direction currDirection{kDirections[dIdx]};
  while (currY >= 0 && currY < grid.size() && currX >= 0 &&
         currX < grid[currY].size()) {
    char square{grid[currY][currX]};
    if (square == '#') {
      currX -= currDirection.dx;
      currY -= currDirection.dy;
      dIdx = (dIdx + 1) % 4;
      currDirection = kDirections[dIdx];
    } else if (square != 'X') {
      grid[currY][currX] = 'X';
      ++numVisited;
    }

    currX += currDirection.dx;
    currY += currDirection.dy;
  }

  std::cout << "Visited grids: " << numVisited << std::endl;

  return 0;
}
