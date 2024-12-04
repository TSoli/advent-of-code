#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Direction {
  int dx, dy;
};

const std::vector<Direction> kDirections{{-1, -1}, {1, -1}};

bool isXMAS(int i, int j, const std::vector<std::string> &board) {
  if (board[i][j] != 'A')
    return false;

  for (const auto &direction : kDirections) {
    if (!((board[i + direction.dx][j + direction.dy] == 'M' &&
           board[i - direction.dx][j - direction.dy] == 'S') ||
          (board[i + direction.dx][j + direction.dy] == 'S' &&
           board[i - direction.dx][j - direction.dy] == 'M')))
      return false;
  }

  return true;
}

int getNumWords(const std::vector<std::string> &board) {
  int numWords{0};
  for (int i = 1; i < board.size() - 1; ++i) {
    for (int j = 1; j < board.size() - 1; ++j) {
      numWords += isXMAS(i, j, board);
    }
  }
  return numWords;
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

  std::vector<std::string> board;
  std::string line;
  while (std::getline(inf, line)) {
    board.push_back(line);
  }

  std::cout << "Number of X-MAS: " << getNumWords(board) << std::endl;

  return 0;
}
