#include <array>
#include <fstream>
#include <iostream>
#include <string>

const int kSeconds{100};
const int xSize{101};
const int ySize{103};

void getCoords(std::string &str, int &x, int &y) {
  size_t start{str.find('=') + 1};
  size_t end{str.find(',', start)};
  x = std::stoi(str.substr(start, end - start));

  start = end + 1;
  end = str.find(' ', start);
  y = std::stoi(str.substr(start, end - start));
  str = str.substr(end + 1);
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

  int xMid{xSize / 2};
  int yMid{ySize / 2};

  std::string line;
  std::array<int, 4> quads{{}};
  while (std::getline(inf, line)) {
    int px, py, vx, vy, fx, fy;
    getCoords(line, px, py);
    getCoords(line, vx, vy);

    fx = ((px + (kSeconds * vx) % xSize) + xSize) % xSize;
    fy = ((py + (kSeconds * vy) % ySize) + ySize) % ySize;

    if (fx < xMid && fy < yMid) {
      ++quads[0];
    } else if (fx > xMid && fy < yMid) {
      ++quads[1];
    } else if (fx < xMid && fy > yMid) {
      ++quads[2];
    } else if (fx > xMid && fy > yMid) {
      ++quads[3];
    }
  }

  long score{1};
  for (int i = 0; i < quads.size(); ++i) {
    score *= quads[i];
  }

  std::cout << "Safety score: " << score << "\n";
  return 0;
}
