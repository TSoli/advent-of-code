#include <aoc/io.h>

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::ifstream inf;
  aoc::getInput(argc, argv, inf);

  std::string line;
  int pos{50};
  int count{0};
  while (std::getline(inf, line)) {
    char dir{line[0]};
    int mag{std::stoi(line.substr(1))};

    if (dir == 'L') {
      count += std::abs((pos - mag) / 100);
      count += pos != 0 && (pos - mag) <= 0;
      pos = (pos - mag) % 100;
    } else { // Right turn
      count += (pos + mag) / 100;
      pos = (pos + mag) % 100;
    }

    if (pos < 0) {
      pos += 100;
    }
  }

  std::cout << "The password is " << count << std::endl;
  return 0;
}
