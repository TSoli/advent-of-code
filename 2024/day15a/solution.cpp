#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Direction {
  int i, j;
};

std::unordered_map<char, Direction> kDirections{
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};

using Grid = std::vector<std::string>;

Grid getGrid(std::ifstream &inf, int &startI, int &startJ) {
  Grid grid;
  std::string row;
  int i{0};
  while (std::getline(inf, row), row != "") {
    grid.push_back(row);
    size_t j{(row.find('@'))};
    if (j != std::string::npos) {
      startI = i;
      startJ = j;
    }
    ++i;
  }

  return grid;
}

bool moveObj(Grid &grid, const Direction dir, const int i, const int j) {
  int ni{i + dir.i};
  int nj{j + dir.j};
  char nextObj{grid[ni][nj]};
  if (nextObj == '#') {
    return false;
  } else if (nextObj == '.' || moveObj(grid, dir, ni, nj)) {
    grid[ni][nj] = grid[i][j];
    grid[i][j] = '.';
    return true;
  }

  return false;
}

std::string getMoves(std::ifstream &inf) {
  std::string moves;
  char c;
  while (inf.get(c)) {
    if (c != '\n') moves += c;
  }
  return moves;
}

long getScore(const Grid &grid) {
  long score{0};
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == 'O') score += 100 * i + j;
    }
  }

  return score;
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

  int i, j;
  Grid grid{getGrid(inf, i, j)};
  std::string moves{getMoves(inf)};

  for (const char m : moves) {
    Direction dir{kDirections[m]};
    if (moveObj(grid, dir, i, j)) {
      i += dir.i;
      j += dir.j;
    }
  }

  std::cout << "Sum of coords: " << getScore(grid) << "\n";

  return 0;
}
