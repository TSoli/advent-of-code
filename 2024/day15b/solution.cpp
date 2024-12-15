#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Direction {
  int i, j;
};

std::unordered_map<char, Direction> kDirections{
    {'^', {-1, 0}}, {'>', {0, 1}}, {'v', {1, 0}}, {'<', {0, -1}}};

using Grid = std::vector<std::string>;

std::string expandRow(const std::string &row) {
  std::string newRow;
  for (const char c : row) {
    switch (c) {
    case '@':
      newRow += "@.";
      break;
    case 'O':
      newRow += "[]";
      break;
    default:
      newRow += c;
      newRow += c;
    }
  }

  return newRow;
}

Grid getGrid(std::ifstream &inf, int &startI, int &startJ) {
  Grid grid;
  std::string row;
  int i{0};
  while (std::getline(inf, row), row != "") {
    std::string expandedRow{expandRow(row)};
    grid.push_back(expandedRow);
    size_t j{(expandedRow.find('@'))};
    if (j != std::string::npos) {
      startI = i;
      startJ = j;
      grid[i][j] = '.';
    }
    ++i;
  }

  return grid;
}

bool canMoveObj(const Grid &grid, const Direction dir, const int i, const int j,
                const int jSize, std::unordered_set<int> &visited) {
  const int ni{i + dir.i};
  const int nj{j + dir.j};
  char nextObj{grid[ni][nj]};

  if (nextObj == '#') {
    return false;
  }
  if (visited.contains(i * jSize + j)) {
    return true;
  }

  visited.insert(i * jSize + j);
  if (nextObj == '.') {
    return true;
  }

  if (nextObj == '[' && dir.i) {
    return canMoveObj(grid, dir, ni, nj, jSize, visited) &&
           canMoveObj(grid, dir, ni, nj + 1, jSize, visited);
  } else if (nextObj == ']' && dir.i) {
    return canMoveObj(grid, dir, ni, nj - 1, jSize, visited) &&
           canMoveObj(grid, dir, ni, nj, jSize, visited);
  } else {
    return canMoveObj(grid, dir, ni, nj, jSize, visited);
  }

  return false;
}

void moveObj(Grid &grid, std::unordered_set<int> &moved, const char obj,
             Direction dir, const int i, const int j, const int jSize) {
  if (moved.contains(i * jSize + j)) {
    return;
  }

  moved.insert(i * jSize + j);

  const char currObj{grid[i][j]};
  const int ni{i + dir.i};
  const int nj{j + dir.j};

  grid[i][j] = obj;

  if (currObj == '.') {
    return;
  }

  if (currObj == '[' && dir.i) {
    grid[i][j + 1] = '.';
    moveObj(grid, moved, '[', dir, ni, nj, jSize);
    moveObj(grid, moved, ']', dir, ni, nj + 1, jSize);
  } else if (currObj == ']' && dir.i) {
    grid[i][j - 1] = '.';
    moveObj(grid, moved, '[', dir, ni, nj - 1, jSize);
    moveObj(grid, moved, ']', dir, ni, nj, jSize);
  } else if (currObj == '#') {
    std::cout << "WALL!\n";
  } else {
    moveObj(grid, moved, currObj, dir, ni, nj, jSize);
  }
}

std::string getMoves(std::ifstream &inf) {
  std::string moves;
  char c;
  while (inf.get(c)) {
    if (c != '\n')
      moves += c;
  }
  return moves;
}

long getScore(const Grid &grid) {
  long score{0};
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == '[')
        score += 100 * i + j;
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
    std::unordered_set<int> visited;
    if (canMoveObj(grid, dir, i, j, grid[0].size(), visited)) {
      i += dir.i;
      j += dir.j;
      std::unordered_set<int> moved;
      moveObj(grid, moved, '.', dir, i, j, grid[0].size());
    }
  }

  std::cout << "Sum of coords: " << getScore(grid) << "\n";

  return 0;
}
