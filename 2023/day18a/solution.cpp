#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Direction { UP, LEFT, DOWN, RIGHT };
enum class Corner { NONE, TOP, BOTTOM };

struct Instruction {
  Direction dir;
  int len;
  std::string colour;
};

using Trench = std::vector<std::vector<std::string>>;

std::vector<Instruction> getInstructions(std::ifstream &inf) {
  std::vector<Instruction> instructions;

  char dir;
  int len;
  std::string colour;
  while (inf >> dir) {
    inf >> len;
    inf >> colour;
    Direction direction;

    colour = colour.substr(1, colour.size() - 2);

    switch (dir) {
    case 'U':
      direction = Direction::UP;
      break;
    case 'L':
      direction = Direction::LEFT;
      break;
    case 'D':
      direction = Direction::DOWN;
      break;
    default:
      direction = Direction::RIGHT;
    }

    instructions.emplace_back(direction, len, colour);
  }

  return instructions;
}

Trench getTrenchOutline(const std::vector<Instruction> &instructions) {
  Trench trench(1, std::vector<std::string>(1, "#"));
  int i, j, iEnd, jEnd, step;
  i = j = 0;
  for (const auto &instruction : instructions) {
    switch (instruction.dir) {
    case Direction::UP:
      iEnd = i - instruction.len;
      jEnd = j;
      step = -1;
      break;
    case Direction::LEFT:
      jEnd = j - instruction.len;
      iEnd = i;
      step = -1;
      break;
    case Direction::DOWN:
      iEnd = i + instruction.len;
      jEnd = j;
      step = 1;
      break;
    default:
      jEnd = j + instruction.len;
      iEnd = i;
      step = 1;
    }

    if (iEnd < 0) {
      for (int k = 0; k < -iEnd; ++k) {
        trench.insert(trench.begin(),
                      std::vector<std::string>(trench.front().size()));
      }

      i += -iEnd;
      iEnd = 0;
    }

    if (jEnd < 0) {
      for (auto &row : trench) {
        for (int k = 0; k < -jEnd; ++k) {
          row.insert(row.begin(), std::string());
        }
      }

      j += -jEnd;
      jEnd = 0;
    }

    if (iEnd > trench.size() - 1) {
      size_t extendBy{iEnd - (trench.size() - 1)};
      for (size_t k = 0; k < extendBy; ++k) {
        trench.push_back(std::vector<std::string>(trench.front().size()));
      }
    }

    if (jEnd > trench.front().size() - 1) {
      size_t extendBy{jEnd - (trench.front().size() - 1)};
      for (auto &row : trench) {
        for (size_t k = 0; k < extendBy; ++k) {
          row.push_back(std::string());
        }
      }
    }

    while (i != iEnd) {
      i += step;
      trench[i][j] = instruction.colour;
    }

    while (j != jEnd) {
      j += step;
      trench[i][j] = instruction.colour;
    }
  }

  return trench;
}

bool isEdge(int i, const std::vector<std::string> &row) {
  if (row[i].empty()) {
    return false;
  }

  if (i == row.size() - 1) {
    return false;
  }

  if (i == 0) {
    if (row[i + 1].empty()) {
      return true;
    }

    return false;
  }

  return row[i - 1].empty() && row[i + 1].empty();
}

Corner getCorner(int i, int j, const Trench &trench) {
  if (trench[i][j].empty()) {
    return Corner::NONE;
  }

  if (i == 0) {
    if (!trench[i + 1][j].empty()) {
      return Corner::TOP;
    }

    return Corner::NONE;
  }

  if (i == trench.size() - 1) {
    if (!trench[i - 1][j].empty()) {
      return Corner::BOTTOM;
    }

    return Corner::NONE;
  }

  if (!trench[i + 1][j].empty() && trench[i - 1][j].empty()) {
    return Corner::TOP;
  }

  if (trench[i + 1][j].empty() && !trench[i - 1][j].empty()) {
    return Corner::BOTTOM;
  }

  return Corner::NONE;
}

int digOutTrench(const Trench &trench) {
  int size{0};
  for (int i = 0; i < trench.size(); ++i) {
    bool inside{false};
    Corner lastCorner{Corner::NONE};
    for (int j = 0; j < trench[i].size(); ++j) {
      if (!trench[i][j].empty() || inside) {
        ++size;
      }

      Corner currCorner{getCorner(i, j, trench)};
      if (isEdge(j, trench[i])) {
        inside = !inside;
      } else if (lastCorner == Corner::NONE) {
        lastCorner = currCorner;
      } else if (lastCorner == currCorner) {
        lastCorner = Corner::NONE;
      } else if (lastCorner != currCorner && currCorner != Corner::NONE) {
        lastCorner = Corner::NONE;
        inside = !inside;
      }
    }
  }

  return size;
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

  std::vector<Instruction> instructions{getInstructions(inf)};
  Trench trench{getTrenchOutline(instructions)};

  std::cout << "Trench size: " << digOutTrench(trench) << "\n";

  return 0;
}
