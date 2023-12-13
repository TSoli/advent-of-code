#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

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
  std::getline(inf, line);
  for (auto it = line.begin(); it != line.end();) {
    if (!std::isdigit(*it)) {
      it = line.erase(it);
    } else {
      ++it;
    }
  }

  long time{std::stol(line)};

  std::getline(inf, line);
  for (auto it = line.begin(); it != line.end();) {
    if (!std::isdigit(*it)) {
      it = line.erase(it);
    } else {
      ++it;
    }
  }

  long distance{std::stol(line)};

  // d = t(n - t)
  // => t^2 -nt + d = 0
  int t{static_cast<int>(
      std::ceil((time + std::sqrt((time * time) - (4 * distance))) / 2) - 1)};
  std::cout << static_cast<int>(1 + 2 * (t - (time / 2.0))) << "\n";

  return 0;
}
