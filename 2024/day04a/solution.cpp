#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Direction {
  int dx, dy;
};

const std::vector<Direction> kDirections{{0, -1},  {0, 1},  {-1, 0}, {1, 0},
                                         {-1, -1}, {1, -1}, {1, 1},  {-1, 1}};

const std::string kWord{"XMAS"};

void getWords(int &numWords, int i, int j, int letterPos,
              const Direction direction, const std::vector<std::string> &board,
              const std::string &word) {
  if (i >= board.size() || i < 0 || j >= board[i].size() || j < 0)
    return; // gone off board

  if (board[i][j] != word[letterPos])
    return; // word doesn't match

  if (letterPos == word.size() - 1) {
    ++numWords;
    return;
  }

  getWords(numWords, i + direction.dx, j + direction.dy, ++letterPos, direction,
           board, word);
}

int getNumWords(const std::vector<std::string> &board,
                const std::string &word) {
  int numWords{0};
  for (int i = 0; i < board.size(); ++i) {
    for (int j = 0; j < board.size(); ++j) {
      for (const auto &direction : kDirections) {
        getWords(numWords, i, j, 0, direction, board, kWord);
      }
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

  std::cout << "Number of " << kWord << ": " << getNumWords(board, kWord)
            << std::endl;

  return 0;
}
