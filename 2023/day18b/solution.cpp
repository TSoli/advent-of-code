#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

enum class Direction { UP, LEFT, DOWN, RIGHT };

struct Instruction {
  Direction dir;
  int len;
  std::string colour;
};

using Pos = std::array<long, 2>;

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

std::vector<Pos> getVertics(const std::vector<Instruction> &instructions,
                            long long &perimeter) {
  std::vector<Pos> vertices;
  long i, j;
  i = j = 0;

  perimeter += 1;
  vertices.push_back({i, j});

  for (const auto &instruction : instructions) {
    long len{stol(instruction.colour.substr(1, 5), nullptr, 16)};
    char dirChar{instruction.colour.back()};

    switch (dirChar) {
      case '0':
        i += len;
        break;
      case '1':
        j += len;
        break;
      case '2':
        i -= len;
        break;
      case '3':
        j -= len;
        break;
      default:
        std::cerr << "Unknown direction: " << dirChar << "\n";
    }

    vertices.push_back({i, j});
    perimeter += len;
  }

  return vertices;
}

long long getTrenchArea(const std::vector<Pos> &vertices) {
  // shoelace formula
  long long area{0};

  for (int i = 0; i < vertices.size(); ++i) {
    long x{vertices[i][0]};
    long y1, y2;

    if (i == 0) {
      y1 = vertices[i + 1][1];
      y2 = vertices.back()[1];
    } else if (i == vertices.size() - 1) {
      y1 = vertices.front()[1];
      y2 = vertices[i - 1][1];
    } else {
      y1 = vertices[i + 1][1];
      y2 = vertices[i - 1][1];
    }
    area += x * (y1 - y2);
  }

  return area / 2;
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
  long long perimeter{0};
  std::vector<Pos> vertices{getVertics(instructions, perimeter)};
  long long area{getTrenchArea(vertices)};
  // Pick's theorem
  long long trenchSize{area + perimeter / 2 + 1};

  std::cout << "Trench size: " << trenchSize << "\n";

  return 0;
}
